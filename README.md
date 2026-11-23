# 8-Puzzle con A\*

Solver del 8-puzzle en C++ que usa **A\*** con dos heurísticas (piezas fuera de lugar y
distancia Manhattan) y reporta la solución óptima junto con métricas de búsqueda.

Proyecto 2 del curso de Inteligencia Artificial.

## Meta

El objetivo es siempre el mismo arreglo en espiral:

```
123
8 4
765
```

El `0` representa el espacio vacío y se imprime como un espacio en blanco.

## Características

- **A\* correcto**: lista de abiertos ordenada por `f = g + h` y conjunto de estados
  cerrados, por lo que nunca se cicla ni se expande dos veces el mismo estado.
- **Dos heurísticas**:
  - `heuristicaFichas`: cantidad de fichas fuera de lugar. Es admisible.
  - `heuristicaManhattan`: suma de distancias Manhattan de cada ficha a su posición
    meta. Es admisible, consistente y expande muchos menos nodos.
- **Estado inicial aleatorio**: en cada corrida se genera un puzzle al azar y se
  descarta automáticamente si no es soluble.
- **Verificación de solubilidad** por paridad de inversiones (evita buscar en estados
  imposibles).
- **Camino solución** reconstruido desde la meta hasta el inicio, impreso paso a paso.
- **Métricas**: movimientos de la solución, nodos expandidos y nodos generados.

## Compilación

Requiere `g++` con C++17:

```bash
g++ -std=c++17 -O2 -Wall -o 8-positions.exe 8-positions.cpp
```

## Ejecución

```bash
./8-positions.exe
```

Ejemplo de salida:

```
Heuristica: Manhattan
Semilla: 938558578

=== Paso 0 ===
624
 81
537

=== Paso 1 (mueve el 8) ===
624
8 1
537

...

=== Meta alcanzada ===
123
8 4
765
Movimientos: 24
Nodos expandidos: 498
Nodos generados: 819
```

## Configuración

Todo se ajusta en las primeras líneas del `main`:

```cpp
// Cambiar a false para usar la heurística de piezas fuera de lugar
const bool usarManhattan = true;

// Semilla fija (distinta de 0) para repetir siempre el mismo puzzle
const unsigned int SEMILLA_FIJA = 0;

// Meta: 123 / 8 4 / 765 (el 0 es el espacio vacío)
const std::vector<int> meta = {1, 2, 3, 8, 0, 4, 7, 6, 5};
```

Para fijar un estado inicial a mano, se puede reemplazar la llamada a
`generarEstadoAleatorio` por un `std::vector<int>` explícito, por ejemplo:

```cpp
std::vector<int> estado = {2, 8, 3, 1, 6, 4, 7, 0, 5};
```

Un estado escrito de izquierda a derecha y de arriba a abajo, con `0` como hueco:

```
283
164
7 5
```

## Resultados

Las longitudes de solución fueron verificadas contra una búsqueda en anchura (BFS)
sobre los 181 440 estados alcanzables, así que las soluciones que reporta el A\* son
óptimas.

| Distancia óptima | Nodos expandidos (Manhattan) | Nodos expandidos (Fichas) |
| --- | --- | --- |
| 5  | 6     | 7      |
| 10 | 15    | 46     |
| 15 | 102   | 388    |
| 20 | 260   | 3 901  |
| 25 | 852   | 27 543 |

Conclusión: las dos heurísticas encuentran la misma solución óptima, pero Manhattan
expande hasta ~30 veces menos nodos en los casos difíciles.

## Nota sobre solubilidad

En el 8-puzzle no todas las configuraciones son alcanzables. Dos estados son solubles
entre sí si la paridad del número de inversiones coincide. Como la meta en espiral
tiene 7 inversiones (impar), el estado inicial también debe tener una cantidad impar
de inversiones. Si no coincide, el programa lo informa y termina sin buscar:

```
El estado inicial no tiene solucion (paridad de inversiones).
```

Por eso un estado como `123 / 4 5 / 678` no tiene solución hacia la meta espiral.

## Estructura del código

| Función | Descripción |
| --- | --- |
| `heuristicaFichas` | Fichas fuera de lugar. |
| `heuristicaManhattan` | Suma de distancias Manhattan. |
| `imprimirTablero` | Imprime el tablero en formato compacto. |
| `generarVecinos` | Estados alcanzables moviendo el hueco. |
| `contarInversiones` / `esSoluble` | Chequeo de solubilidad. |
| `fichaMovida` | Determina qué ficha se movió entre dos estados. |
| `generarEstadoAleatorio` | Genera un estado inicial aleatorio y soluble. |
| `main` | A\* con lista de abiertos, cerrados, camino y métricas. |
