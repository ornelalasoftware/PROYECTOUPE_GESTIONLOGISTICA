#include "sistemagestionlogistico.h"
#include <fstream>
#include <iostream>
#include <cstring>

SistemaLogistico::SistemaLogistico() {
    grafo = new Grafo(10);
}

SistemaLogistico::~SistemaLogistico() {
    delete grafo;
}

void SistemaLogistico::inicializar() {
    cargarDatos();
}

void SistemaLogistico::agregarCiudad(std::string nombre, float x, float y) {
    Ciudad nueva;

    nueva.id = ciudades.size();

    strncpy(nueva.nombre, nombre.c_str(), 49);
    nueva.nombre[49] = '\0';

    nueva.x = x;
    nueva.y = y;

    ciudades.push_back(nueva);
}

void SistemaLogistico::agregarRuta(int origen, int destino, float km) {
    grafo->agregarRuta(origen, destino, km);
}

void SistemaLogistico::cortarRuta(int origen, int destino) {
    grafo->cortarRuta(origen, destino);
}

ResultadoRuta SistemaLogistico::calcularRutaOptima(int origen, int destino) {
    ResultadoRuta resultado = grafo->calcularDijkstra(origen, destino);

    historial.push(resultado);

    return resultado;
}

void SistemaLogistico::guardarDatos() {
    std::ofstream archivo("ciudades.dat", std::ios::binary);

    if (!archivo.is_open()) {
        std::cout << "Error al guardar archivo.\n";
        return;
    }

    for (const auto& ciudad : ciudades) {
        archivo.write(
            reinterpret_cast<const char*>(&ciudad),
            sizeof(Ciudad)
        );
    }

    archivo.close();

    std::cout << "Datos guardados correctamente.\n";
}

void SistemaLogistico::cargarDatos() {
    std::ifstream archivo("ciudades.dat", std::ios::binary);

    if (!archivo.is_open()) {
        return;
    }

    ciudades.clear();

    Ciudad ciudad;

    while (
        archivo.read(
            reinterpret_cast<char*>(&ciudad),
            sizeof(Ciudad)
        )
    ) {
        ciudades.push_back(ciudad);
    }

    archivo.close();
}

void SistemaLogistico::mostrarHistorial() {
    std::cout << "\n===== HISTORIAL =====\n";

    std::stack<ResultadoRuta> copia = historial;

    while (!copia.empty()) {
        ResultadoRuta actual = copia.top();

        std::cout
            << "Distancia: "
            << actual.distanciaTotal
            << " km\n";

        copia.pop();
    }
}

Grafo& SistemaLogistico::getGrafo() {
    return *grafo;
}