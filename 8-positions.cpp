#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <random>

int heuristicaFichas(const std::vector<int>& tablero, const std::vector<int>& meta){
    int contador = 0;

    for(std::size_t i = 0; i < tablero.size(); i ++){
        if(tablero[i] != meta[i] && tablero[i] != 0){
            contador ++;
        }
    }
    return contador;
}

int heuristicaManhattan(const std::vector<int>& tablero, const std::vector<int>& meta){
    int suma = 0;

    for(std::size_t i = 0; i < tablero.size(); i ++){
        if(tablero[i] == 0){
            continue;
        }

        std::size_t indiceMeta = 0;
        for(std::size_t j = 0; j < meta.size(); j ++){
            if(meta[j] == tablero[i]){
                indiceMeta = j;
            }
        }

        int fila = static_cast<int>(i) / 3;
        int columna = static_cast<int>(i) % 3;
        int filaMeta = static_cast<int>(indiceMeta) / 3;
        int columnaMeta = static_cast<int>(indiceMeta) % 3;

        suma += std::abs(fila - filaMeta) + std::abs(columna - columnaMeta);
    }
    return suma;
}

int heuristica(const std::vector<int>& tablero, const std::vector<int>& meta, bool usarManhattan){
    if(usarManhattan){
        return heuristicaManhattan(tablero, meta);
    }
    return heuristicaFichas(tablero, meta);
}

void imprimirTablero(const std::vector<int>& tablero){
    for(int fila = 0; fila < 3; fila ++){
        for(int columna = 0; columna < 3; columna ++){
            int indice = fila * 3 + columna;

            if(tablero[indice] == 0){
                std::cout << ' ';
            }
            else{
                std::cout << tablero[indice];
            }
        }
        std::cout << '\n';
    }
}

std::vector<std::vector<int>> generarVecinos(const std::vector<int>& estado){
    std::vector<std::vector<int>> vecinos;

    int indiceDelCero = -1;
    for(std::size_t i = 0; i < estado.size(); i ++){
        if(estado[i] == 0){
            indiceDelCero = static_cast<int>(i);
        }
    }

    int fila = indiceDelCero / 3;
    int columna = indiceDelCero % 3;

    if(fila > 0){
        std::vector<int> nuevoEstado = estado;
        int indiceDestino = indiceDelCero - 3;
        std::swap(nuevoEstado[indiceDelCero], nuevoEstado[indiceDestino]);
        vecinos.push_back(nuevoEstado);
    }

    if(fila < 2){
        std::vector<int> nuevoEstado = estado;
        int indiceDestino = indiceDelCero + 3;
        std::swap(nuevoEstado[indiceDelCero], nuevoEstado[indiceDestino]);
        vecinos.push_back(nuevoEstado);
    }

    if(columna > 0){
        std::vector<int> nuevoEstado = estado;
        int indiceDestino = indiceDelCero - 1;
        std::swap(nuevoEstado[indiceDelCero], nuevoEstado[indiceDestino]);
        vecinos.push_back(nuevoEstado);
    }

    if(columna < 2){
        std::vector<int> nuevoEstado = estado;
        int indiceDestino = indiceDelCero + 1;
        std::swap(nuevoEstado[indiceDelCero], nuevoEstado[indiceDestino]);
        vecinos.push_back(nuevoEstado);
    }

    return vecinos;
}

int contarInversiones(const std::vector<int>& estado){
    int inversiones = 0;

    for(std::size_t i = 0; i < estado.size(); i ++){
        for(std::size_t j = i + 1; j < estado.size(); j ++){
            if(estado[i] != 0 && estado[j] != 0 && estado[i] > estado[j]){
                inversiones ++;
            }
        }
    }
    return inversiones;
}

bool esSoluble(const std::vector<int>& estado, const std::vector<int>& meta){
    return (contarInversiones(estado) % 2) == (contarInversiones(meta) % 2);
}

int fichaMovida(const std::vector<int>& padre, const std::vector<int>& hijo){
    for(std::size_t i = 0; i < padre.size(); i ++){
        if(padre[i] == 0){
            return hijo[i];
        }
    }
    return -1;
}

struct Nodo{
    std::vector<int> estado;
    int g;           // costo desde el inicio
    int h;           // heuristica
    int f;           // g + h
    int padre;       // id del nodo anterior en el historial (-1 si es el inicio)
    int movimiento;  // ficha que se movio para llegar aca (-1 si es el inicio)
};

std::vector<int> generarEstadoAleatorio(const std::vector<int>& meta, unsigned int semilla){
    std::vector<int> estado(meta.size());

    std::mt19937 generador(semilla);

    do{
        for(std::size_t i = 0; i < estado.size(); i ++){
            estado[i] = static_cast<int>(i);
        }
        std::shuffle(estado.begin(), estado.end(), generador);
    }while(estado == meta || !esSoluble(estado, meta));

    return estado;
}

int main(){
    using namespace std;

    // Meta: 123 / 8 4 / 765  (el 0 es el espacio vacio)
    const std::vector<int> meta = {1, 2, 3, 8, 0, 4, 7, 6, 5};

    // Estado inicial generado al azar y siempre soluble.
    // Poner una semilla fija distinta de 0 para repetir siempre el mismo puzzle.
    const unsigned int SEMILLA_FIJA = 0;
    unsigned int semilla = (SEMILLA_FIJA != 0) ? SEMILLA_FIJA : std::random_device{}();
    std::vector<int> estado = generarEstadoAleatorio(meta, semilla);

    // Cambiar a false para usar la heuristica de piezas fuera de lugar
    const bool usarManhattan = true;
    const int LIMITE_NODOS = 1000000;

    std::cout << "Heuristica: "
              << (usarManhattan ? "Manhattan" : "piezas fuera de lugar")
              << '\n';
    std::cout << "Semilla: " << semilla << "\n\n";

    if(!esSoluble(estado, meta)){
        std::cout << "El estado inicial no tiene solucion (paridad de inversiones).\n";
        return 0;
    }

    std::vector<Nodo> historial;            // todos los nodos generados; el indice es el id
    std::vector<int> abiertos;              // ids de nodos pendientes de expandir
    std::set<std::vector<int>> cerrados;    // estados ya expandidos

    Nodo inicio;
    inicio.estado = estado;
    inicio.g = 0;
    inicio.h = heuristica(estado, meta, usarManhattan);
    inicio.f = inicio.h;
    inicio.padre = -1;
    inicio.movimiento = -1;

    historial.push_back(inicio);
    abiertos.push_back(0);

    int nodosExpandidos = 0;
    int idMeta = -1;

    while(!abiertos.empty() && nodosExpandidos < LIMITE_NODOS){
        // Buscar el nodo con menor f (no hace falta priority_queue para 9 fichas)
        std::size_t mejorPos = 0;
        for(std::size_t i = 1; i < abiertos.size(); i ++){
            if(historial[abiertos[i]].f < historial[abiertos[mejorPos]].f){
                mejorPos = i;
            }
        }

        int idActual = abiertos[mejorPos];
        abiertos.erase(abiertos.begin() + mejorPos);
        Nodo actual = historial[idActual];

        if(cerrados.find(actual.estado) != cerrados.end()){
            continue;  // ya se expandio este estado con un f igual o mejor
        }
        cerrados.insert(actual.estado);
        nodosExpandidos ++;

        if(actual.estado == meta){
            idMeta = idActual;
            break;
        }

        std::vector<std::vector<int>> vecinos = generarVecinos(actual.estado);

        for(std::size_t i = 0; i < vecinos.size(); i ++){
            if(cerrados.find(vecinos[i]) != cerrados.end()){
                continue;
            }

            Nodo hijo;
            hijo.estado = vecinos[i];
            hijo.g = actual.g + 1;
            hijo.h = heuristica(hijo.estado, meta, usarManhattan);
            hijo.f = hijo.g + hijo.h;
            hijo.padre = idActual;
            hijo.movimiento = fichaMovida(actual.estado, hijo.estado);

            historial.push_back(hijo);
            abiertos.push_back(static_cast<int>(historial.size()) - 1);
        }
    }

    if(idMeta == -1){
        std::cout << "Corte de seguridad: no se llego a la meta.\n";
        std::cout << "Nodos expandidos: " << nodosExpandidos << '\n';
        return 0;
    }

    // Reconstruir el camino desde la meta hasta el inicio y darlo vuelta
    std::vector<int> camino;
    for(int id = idMeta; id != -1; id = historial[id].padre){
        camino.push_back(id);
    }
    std::reverse(camino.begin(), camino.end());

    for(std::size_t paso = 0; paso < camino.size(); paso ++){
        const Nodo& nodo = historial[camino[paso]];

        std::cout << "=== Paso " << nodo.g;
        if(nodo.movimiento != -1){
            std::cout << " (mueve el " << nodo.movimiento << ")";
        }
        std::cout << " ===\n";
        imprimirTablero(nodo.estado);
        std::cout << '\n';
    }

    std::cout << "=== Meta alcanzada ===\n";
    std::cout << "Movimientos: " << historial[idMeta].g << '\n';
    std::cout << "Nodos expandidos: " << nodosExpandidos << '\n';
    std::cout << "Nodos generados: " << historial.size() << '\n';

    return 0;
}
