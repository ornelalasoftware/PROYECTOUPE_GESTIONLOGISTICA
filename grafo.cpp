#include "grafo.h"
#include <algorithm>

Grafo::Grafo(int numCiudades) {
    n = numCiudades;

    matriz = new float*[n];

    for (int i = 0; i < n; i++) {
        matriz[i] = new float[n];

        for (int j = 0; j < n; j++) {
            matriz[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void Grafo::agregarRuta(int origen, int destino, float km) {
    matriz[origen][destino] = km;
    matriz[destino][origen] = km;
}

void Grafo::cortarRuta(int origen, int destino) {
    matriz[origen][destino] = INF;
    matriz[destino][origen] = INF;
}

ResultadoRuta Grafo::calcularDijkstra(int inicio, int fin) {

    std::vector<float> distancias(n, INF);
    std::vector<bool> visitados(n, false);
    std::vector<int> padres(n, -1);

    distancias[inicio] = 0;

    for (int i = 0; i < n - 1; i++) {

        float minimo = INF;
        int u = -1;

        for (int j = 0; j < n; j++) {
            if (!visitados[j] && distancias[j] < minimo) {
                minimo = distancias[j];
                u = j;
            }
        }

        if (u == -1) break;

        visitados[u] = true;

        for (int v = 0; v < n; v++) {

            if (!visitados[v] &&
                matriz[u][v] != INF &&
                distancias[u] + matriz[u][v] < distancias[v]) {

                distancias[v] = distancias[u] + matriz[u][v];
                padres[v] = u;
            }
        }
    }

    ResultadoRuta resultado;
    resultado.distanciaTotal = distancias[fin];

    if (distancias[fin] == INF) {
        resultado.descripcionTexto = "No existe ruta.";
        return resultado;
    }

    int actual = fin;

    while (actual != -1) {
        resultado.camino.push_back(actual);
        actual = padres[actual];
    }

    std::reverse(
        resultado.camino.begin(),
        resultado.camino.end()
    );

    resultado.descripcionTexto = "Ruta calculada correctamente.";

    return resultado;
}

Grafo::~Grafo() {

    for (int i = 0; i < n; i++) {
        delete[] matriz[i];
    }

    delete[] matriz;
}