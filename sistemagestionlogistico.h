#ifndef SISTEMAGESTIONLOGISTICO_H
#define SISTEMAGESTIONLOGISTICO_H

#include "grafo.h"
#include "ciudad.h"
#include <string>

// struct para manejar la pila mediante nodos enlazados dinámicamente
struct NodoHistorial {
    ResultadoRuta dato;          // guarda el resultado de la ruta calculada
    NodoHistorial* siguiente;    // apunta al nodo que está abajo en la pila
};

class SistemaLogistico {
private:
    Grafo* grafo;//puntero a la clase Grafo declarada en grafo.h
    
    Ciudad* ciudades;          
    int cantidadCiudades;      
    int capacidadCiudades;
    // vector de objetos Ciudad 

    NodoHistorial* historial;
    //puntero al final la pila de nodos

public:
    SistemaLogistico();
    ~SistemaLogistico(); // Aca se vacia la pila nodo por nodo con delete 
    //y borrar el vector de ciudades 

    void inicializar();
    void agregarCiudad(std::string nombre, float x, float y);
    void agregarRuta(int origen, int destino, float km);
    void cortarRuta(int origen, int destino);

    ResultadoRuta calcularRutaOptima(int origen, int destino);
    std::string obtenerNombreCiudad(int id);

    // Funciones para manejar la pila
    void apilarHistorial(ResultadoRuta ruta);
    void desapilarHistorial();

    void guardarDatos();
    void cargarDatos();
    void mostrarHistorial();

    Grafo& getGrafo();
};

#endif