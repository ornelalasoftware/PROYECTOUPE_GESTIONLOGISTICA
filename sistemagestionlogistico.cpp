#include "sistemagestionlogistico.h"
#include <fstream>
#include <iostream>
#include <cstring>

SistemaLogistico::SistemaLogistico() {
    grafo = new Grafo(10);
}
//Constructor: cuando el sistema arranca, reserva memoria dinámica para un nuevo 
//objeto Grafo que se configura inicialmente para soportar hasta 10 ciudades
//NO agrada mucho, hay que modificarlo porque se puede desbordar

SistemaLogistico::~SistemaLogistico() {
    delete grafo;
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
    Ciudad nueva;
    //Se crea una estructura temporal/auxiliar de tipo Ciudad llamada nueva

    nueva.id = ciudades.size();
    //.size() es una funcion de "std:" te devuelve la cantidad de elementos 
    //que hay dentro del vector ciudades en ese preciso momento

    strncpy(nueva.nombre, nombre.c_str(), 49);
    //strncpy: Copia el texto de nombre dentro del arreglo de caracteres fijo de la estructura Ciudad
    nueva.nombre[49] = '\0';
    //Se asegura que el último carácter sea el terminador nulo, indicando el fin de la cadena de texto. 
    //Esto es importante para cuando usemos los archivos binarios despues

    nueva.x = x;
    nueva.y = y;

    ciudades.push_back(nueva);//la funcion push_back(nueva) hace tres tareas importantes:
    //Revisa el espacio: Verifica si el vector tiene suficiente memoria reservada para un elemento más. Si no la tiene, el vector pide automáticamente más memoria a la computadora y se expande solo.
    //Copia el objeto: Toma una copia exacta de la estructura nueva (que contiene el id, el nombre y las coordenadas x, y de la ciudad) y la coloca en la última posición disponible.
    //Actualiza el tamaño: Incrementa internamente el contador de elementos. Así, la próxima vez que llames a .size(), el número reflejará que hay una ciudad más.
    //Guarda las coordenadas espaciales en la estructura y mete la nueva ciudad al final del vector ciudades
}

void SistemaLogistico::agregarRuta(int origen, int destino, float km) {
    grafo->agregarRuta(origen, destino, km);
}
//El sistema logístico no sabe cómo manejar grafos directamente, así que le "delega" el 
//trabajo a su objeto interno grafo usando el operador flecha (->) porque es un puntero

void SistemaLogistico::cortarRuta(int origen, int destino) {
    grafo->cortarRuta(origen, destino);
}
//igual que el anterior, le pide al grafo que rompa la conexión entre esas dos ciudades en la matriz de adyacencia

ResultadoRuta SistemaLogistico::calcularRutaOptima(int origen, int destino) {
    ResultadoRuta resultado = grafo->calcularDijkstra(origen, destino);
    //Le pide al grafo que ejecute el Algoritmo de Dijkstra entre el origen y el destino
    //El camino y los kilómetros se guardan en la variable resultado.

    historial.push(resultado);
    //push() se usa en pilas y colas para insertar un elemento al final
    //historial es una pila declarada en sistemagestionlogistico.h
    
    return resultado;
    //guarda una copia de este resultado en la Pila (std::stack)
    //Al usar push, esta última búsqueda se coloca en la cima de la pila
    //Finalmente, devuelve el resultado
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
    }//Chequea que el archivo se abra correctamente, sino tira error

    for (const auto& ciudad : ciudades) {
        //ciclo for moderno (llamado range-based for)
        //Va pasando por cada ciudad que esté guardada dentro del vector ciudades, una por una
        //const auto& = & opera por referencia y const se asegura que sea solo de lectura

        archivo.write(
            reinterpret_cast<const char*>(&ciudad),//reinterpreta al struct como char y le manda por referencia la direccion
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

    ciudades.clear();
    //.clear() limpia el vector ciudades para que no se vuelvan a duplicar datos

    Ciudad ciudad; //auxiliar para guardar datos que se usan en el bucle que sigue

    while (
        archivo.read(
            reinterpret_cast<char*>(&ciudad),
            sizeof(Ciudad)
        )
    ) {
        ciudades.push_back(ciudad);
    }
    //bucle de lectura: archivo.read extrae del archivo el número exacto de bytes 
    //que componen una Ciudad y los vuelca en la variable temporal ciudad
    //Si todo sale bien, mete esa ciudad en el vector ciudades
    //el ciclo se detiene automáticamente cuando se llega al final del archivo (EOF)

    archivo.close();
}

void SistemaLogistico::mostrarHistorial() {
    std::cout << "\n===== HISTORIAL =====\n";

    std::stack<ResultadoRuta> copia = historial;
    //Crea una copia completa de la pila historial. Esto se hace porque para leer una pila 
    //hay que "destruirla" (vaciarla), y no queremos perder el historial real del sistema

    while (!copia.empty()) {
        ResultadoRuta actual = copia.top();
        //Mientras la pila copia no esté vacía, inspecciona el elemento que está arriba del 
        //todo mediante .top()
        //en pilas el elemento de arriba es el mas reciente

        std::cout
            << "Distancia: "
            << actual.distanciaTotal
            << " km\n";

        copia.pop();
        //Muestra los kilómetros en la consola y usa .pop() para eliminar el elemento de la 
        //cima de la copia, permitiendo que en la siguiente vuelta del ciclo se pueda 
        //ver el viaje anterior.
    }
}//Cuando el ciclo while vuelve a empezar, mira el nuevo tope, lo imprime, lo destruye 
//con .pop(), y así sucesivamente hasta que la pila se queda completamente vacía (empty()) 
//y el bucle termina. Por eso hicimos la copia del historial.

Grafo& SistemaLogistico::getGrafo() {
    return *grafo;
}
//Getter: Como grafo es un puntero interno (Grafo*), este método devuelve el objeto 
//"desapuntado" (utilizando el asterisco *) en forma de referencia (Grafo&), 
//permitiendo que otras partes del programa interactúen con la matriz sin romper el 
//encapsulamiento. Esto es para evitar que alguien lo pueda modificar y romperlo