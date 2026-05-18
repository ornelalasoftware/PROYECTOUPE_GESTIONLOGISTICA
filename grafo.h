#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>

const float INF = 999999.0f;

;struct ResultadoRuta {
    int* camino;
    int cantidadCiudades;
    float distanciaTotal;
    std::string descripcionTexto;
};

class Grafo {
private:
    float** matriz;
    int n;

public:
    Grafo(int numCiudades);
    ~Grafo();

    void agregarRuta(int origen, int destino, float km);
    void cortarRuta(int origen, int destino);

    ResultadoRuta calcularDijkstra(int inicio, int fin);
};

#endif