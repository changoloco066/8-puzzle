#include <iostream>
#include <vector>

int heuristica(std::vector<int> tablero, std::vector<int> meta){
    int contador = 0;

    for(int i = 0; i < tablero.size(); i ++){
        if(tablero[i] != meta[i] && tablero[i] != 0){
            contador ++;
        }
    }
    return contador;
}

int main(){
    using namespace std;
    
    std::vector<int> estado = {1, 2, 3, 4, 0, 5, 6, 7, 8};
    std::vector<int> meta = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    std::vector<std::vector<int>> vecinos;

    while(estado != meta){
        vecinos.clear();
            
         int indiceDelCero = -1;
         for(int i = 0; i < estado.size(); i ++){
            if(estado[i] == 0) indiceDelCero = i;
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
        
        int mejorHeuristica = 999;
        int indiceMejor = - 1;

        for(int i = 0; i < vecinos.size(); i ++){
            int h = heuristica(vecinos[i], meta);
            if(h < mejorHeuristica){
                mejorHeuristica = h;
                indiceMejor = i;
            }
        }
    }
    return 0;
}

