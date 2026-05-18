#include "sistemagestionlogistico.h"
#include <fstream>
#include <iostream>
#include <cstring>

SistemaLogistico::SistemaLogistico() {
    grafo = new Grafo(10);
    
    cantidadCiudades = 0;
    capacidadCiudades = 10; // Capacidad inicial estándar
    ciudades = new Ciudad[capacidadCiudades]; // Se reserva memoria para las primeras 10 ciudades
    
    // la pila arranca vacía apuntando a null
    historial = nullptr; 
}
//Constructor: cuando el sistema arranca, reserva memoria dinámica para un nuevo 
//objeto Grafo que se configura inicialmente para soportar hasta 10 ciudades
//NO agrada mucho, hay que modificarlo porque se puede desbordar

SistemaLogistico::~SistemaLogistico() {
    delete grafo;
    
    //liberamos el arreglo dinámico de ciudades.. importante!!
    delete[] ciudades;

    //vaciamos la pila de historial nodo por nodo para evitar fugas de memoria.. importante!!
    while (historial != nullptr) {
        NodoHistorial* auxiliar = historial;
        historial = historial->siguiente; //avanzamos al siguiente nodo de abajo
        
        //como cada ResultadoRuta tiene un arreglo dinámico para el camino, lo liberamos uno por uno
        delete[] auxiliar->dato.camino; 
        
        delete auxiliar; //elimininamos el contenedor auxiliar (nodo)
    }
}
//Destructor: cuando el programa se cierra, libera la memoria que ocupaba 
//el objeto grafo para evitar fugas de memoria.

void SistemaLogistico::inicializar() {
    cargarDatos();
}
//Prepara el sistema cuando arranca
//En este caso, su única tarea es llamar a la función cargarDatos() 
//para recuperar el mapa guardado en el disco.

void SistemaLogistico::agregarCiudad(std::string nombre, float x, float y) {
    
    //si el vector se llena, se tiene que expandir manualmente
    if (cantidadCiudades == capacidadCiudades) {
        capacidadCiudades *= 2; //duplicamos la capacidad máxima
        Ciudad* nuevoArreglo = new Ciudad[capacidadCiudades]; //creamos un arreglo nuevo más grande

        //copiamos todas las ciudades del anterior al nuevo
        for (int i = 0; i < cantidadCiudades; i++) {
            nuevoArreglo[i] = ciudades[i];
        }

        delete[] ciudades; // destruimos el viejo vector que quedó chico
        ciudades = nuevoArreglo; // apuntamos al nuevo espacio ampliado
    }

    Ciudad nueva;
    //Se crea una estructura temporal/auxiliar de tipo Ciudad llamada nueva

    nueva.id = cantidadCiudades;//contador de ciudades

    strncpy(nueva.nombre, nombre.c_str(), 49);
    //strncpy: Copia el texto de nombre dentro del arreglo de caracteres fijo de la estructura Ciudad
    nueva.nombre[49] = '\0';
    //Se asegura que el último carácter sea el terminador nulo, indicando el fin de la cadena de texto. 
    //Esto es importante para cuando usemos los archivos binarios despues

    nueva.x = x;
    nueva.y = y;

    // se le asigna la nueva ciudad en la última posición libre e incrementamos el contador
    ciudades[cantidadCiudades] = nueva;
    cantidadCiudades++;
    //queda apuntando al siguiente elemento que marcara el id del proximo
}

void SistemaLogistico::agregarRuta(int origen, int destino, float km) {
    grafo->agregarRuta(origen, destino, km);
}
//el sistema logístico no sabe cómo manejar grafos directamente, así que le "delega" el 
//trabajo a su objeto interno grafo usando el operador flecha (->) porque es un puntero

void SistemaLogistico::cortarRuta(int origen, int destino) {
    grafo->cortarRuta(origen, destino);
}
//igual que el anterior, le pide al grafo que rompa la conexión entre esas dos ciudades en la matriz de adyacencia

ResultadoRuta SistemaLogistico::calcularRutaOptima(int origen, int destino) {
    ResultadoRuta resultado = grafo->calcularDijkstra(origen, destino);
    //le pide al grafo que ejecute el Algoritmo de Dijkstra entre el origen y el destino
    //el camino y los kilómetros se guardan en la variable resultado.

    apilarHistorial(resultado);
    //método para registrar el resultado en la pila historial
    
    return resultado;
    //Finalmente, devuelve el resultado
}

// se implementa el método para apilar
void SistemaLogistico::apilarHistorial(ResultadoRuta ruta) {
    NodoHistorial* nuevoNodo = new NodoHistorial(); //se crea un nodo dinámico en memoria
    
    nuevoNodo->dato = ruta; // Almacenamos el resultado.
    if (ruta.camino != nullptr) {
        nuevoNodo->dato.camino = new int[ruta.cantidadCiudades];
        for (int i = 0; i < ruta.cantidadCiudades; i++) {
            nuevoNodo->dato.camino[i] = ruta.camino[i];
        }
    }//como ruta.camino es un vector dinámico, para evitar que dos
    // nodos apunten al mismo bloque de memoria, creamos una copia

    nuevoNodo->siguiente = historial; // el nuevo nodo apunta al que estaba en la cima
    historial = nuevoNodo;            // el nuevo nodo pasa a ser la nueva cima de la pila
}

//se implementa el método para desapilar
void SistemaLogistico::desapilarHistorial() {
    if (historial != nullptr) {
        NodoHistorial* auxiliar = historial; 
        historial = historial->siguiente;    //la cima baja al nodo inferior
        
        delete[] auxiliar->dato.camino;      //se libera el camino dinámico de la ruta
        delete auxiliar;                     //se destruye el nodo superior desapilado
    }
}

void SistemaLogistico::guardarDatos() {
    std::ofstream archivo("ciudades.dat", std::ios::binary);
    //Abre (o crea) un archivo llamado ciudades.dat en modo de escritura binaria (std::ios::binary)
    //ofstream (Output File Stream): Se usa exclusivamente para escribir/guardar datos en un archivo (crea el archivo si no existe)
    //Cuando se abre un archivo con fstream, por defecto se maneja en modo .txt
    //pero para guardar estructuras como el struct o la matriz utilizamos el modificador ios::binary
    
    if (!archivo.is_open()) {
        std::cout << "Error al guardar archivo.\n";
        return;
    }//chequea que el archivo se abra correctamente, sino tira error

    for (int i = 0; i < cantidadCiudades; i++) {
        archivo.write(
            reinterpret_cast<const char*>(&ciudades[i]),//reinterpreta al struct como char y le manda por referencia la direccion
            sizeof(Ciudad) //calcula tamaño en bytes de Ciudad
        );
        //Al hacer todo esto la ciudad se guarda en el archivo .dat como una copia identica o "foto"
    }

    archivo.close(); //cierra el archivo .dat

    std::cout << "Datos guardados correctamente.\n";
}

void SistemaLogistico::cargarDatos() {
    std::ifstream archivo("ciudades.dat", std::ios::binary);
    //ifstream (Input File Stream): Se usa exclusivamente para leer/cargar datos desde un archivo hacia tu programa.
    //Abre el archivo ciudades.dat en modo de lectura binaria

    if (!archivo.is_open()) {
        return;
    }
    //archivo.is_open(): Es un método propio de los flujos de fstream
    //Si el archivo no existe, sale en silencio sin lanzar errores

    // se reinicia el contador de ciudades a 0
    cantidadCiudades = 0;
   
    Ciudad ciudadAuxiliar; //auxiliar para guardar datos que se usan en el bucle que sigue

    while (
        archivo.read(
            reinterpret_cast<char*>(&ciudadAuxiliar),
            sizeof(Ciudad)
        )
    ) {
        agregarCiudad(ciudadAuxiliar.nombre, ciudadAuxiliar.x, ciudadAuxiliar.y);
    }
    //bucle de lectura: archivo.read extrae del archivo el número exacto de bytes 
    //que componen una Ciudad y los vuelca en la variable temporal ciudadAuxiliar
    //Si todo sale bien, mete esa ciudad en el vector ciudades
    //el ciclo se detiene automáticamente cuando se llega al final del archivo (EOF)

    archivo.close();
}

void SistemaLogistico::mostrarHistorial() {
    std::cout << "\n===== HISTORIAL =====\n";

    //hacemos una copia de historial para despues recorrerla sin tocar la original
    NodoHistorial* auxiliar = historial;
    //se crea una copia completa de la pila historial

    while (auxiliar != nullptr) {
        ResultadoRuta actual = auxiliar->dato;//guarda el dato del elemento que esta en la cima en variable actual

        std::cout
            << "Distancia: "
            << actual.distanciaTotal//Muestra los kilómetros en la consola
            << " km\n";

        //se valida si la ruta tiene un camino calculado antes de recorrerlo
        if (actual.camino != nullptr && actual.cantidadCiudades > 0) {
            std::cout << "Camino: ";
            
            // se recorre el vector dinámico imprimiendo cada ciudad de la ruta
            for (int i = 0; i < actual.cantidadCiudades; i++) {
                std::cout << actual.camino[i];
                
                //detallito estético para poner una flechita entre ciudades menos la ultima jeje
                if (i < actual.cantidadCiudades - 1) {
                    std::cout << " -> ";
                }
            }
            std::cout << "\n";//salto de linea para que no quede pegao
        } else {
            std::cout << "Camino: " << actual.descripcionTexto << "\n";
            //la descripcion.texto es la que se encuentra en el struct ResultadoRuta
        }
        
        std::cout << "-----------------------\n"; 
        //linea para dividir cada viaje

        auxiliar = auxiliar->siguiente;
        //y luego queda apuntando al siguiente nodo
    }
}//termina cuando apunta a un nodo vacio(null)

Grafo& SistemaLogistico::getGrafo() {
    return *grafo;
}
//Getter: Como grafo es un puntero interno (Grafo*), este método devuelve el objeto 
//"desapuntado" (utilizando el asterisco *) en forma de referencia (Grafo&), 
//permitiendo que otras partes del programa interactúen con la matriz sin romper el 
//encapsulamiento. Esto es para evitar que alguien lo pueda modificar y romperlo

std::string SistemaLogistico::obtenerNombreCiudad(int id) {
    //verificamos que el ID sea válido y exista
    if (id >= 0 && id < cantidadCiudades) {
        return ciudades[id].nombre;
    }
    return "Ciudad Desconocida";//si no encuentra nada
}