#ifndef SISTEMAGESTIONLOGISTICO_H
#define SISTEMAGESTIONLOGISTICO_H

#include "grafo.h"
#include "ciudad.h"
#include <vector>
#include <stack>
#include <string>

class SistemaLogistico {
private:
    Grafo* grafo;
    std::vector<Ciudad> ciudades;
    std::stack<ResultadoRuta> historial;

public:
    SistemaLogistico();
    ~SistemaLogistico();

    void inicializar();
    void agregarCiudad(std::string nombre, float x, float y);
    void agregarRuta(int origen, int destino, float km);
    void cortarRuta(int origen, int destino);

    ResultadoRuta calcularRutaOptima(int origen, int destino);

    void guardarDatos();
    void cargarDatos();
    void mostrarHistorial();

    Grafo& getGrafo();
};

#endif