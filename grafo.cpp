#include "grafo.h"
#include <algorithm>

Grafo::Grafo(int numCiudades) {//numCiudades viene de grafo.h
    n = numCiudades;// cantidadCiudades= numCiudades

    matriz = new float*[n];// se crean n filas en vector de punteros con memoria dinamica

    for (int i = 0; i < n; i++) {
        matriz[i] = new float[n]; //se crean n columnas, en cada fila se crea un vector de n elementos
        // aca ya se creo la matriz cuadrada

        for (int j = 0; j < n; j++) {
            matriz[i][j] = (i == j) ? 0 : INF; // aca se consulta si las ciudades son las mismas
            //si no es 0 km se coloca INF por el momento porque no se sabe la distancia
        }
    }
}

void Grafo::agregarRuta(int origen, int destino, float km) {
    matriz[origen][destino] = km;
    matriz[destino][origen] = km;
}
//esta funcion guarda los km que hay entre ciudades, de ida y de vuelta

void Grafo::cortarRuta(int origen, int destino) {
    matriz[origen][destino] = INF;
    matriz[destino][origen] = INF;
}
//esta funcion le asigna el valor INF a una ruta porque esta cortada

ResultadoRuta Grafo::calcularDijkstra(int inicio, int fin) {

    float* distancias = new float[n]; // se crea un arreglo dinámico para guardar las distancias
    bool* visitados = new bool[n];    // se crea un arreglo dinámico para marcar las ciudades visitadas
    int* padres = new int[n];          // en vez de padres poner ciudad_padre/ se crea una lista para recordar de que ciudad vinimos

    // Como los arreglos dinámicos con 'new' no se inicializan solos, usamos este bucle para darles sus valores iniciales
    for (int i = 0; i < n; i++) {
        distancias[i] = INF; // todas comienzan con valor INF
        visitados[i] = false; // todas son F al principio
        padres[i] = -1; // empieza en -1 porque no hay ninguna ruta al principio(padre)
    }

    distancias[inicio] = 0;//la distancia que hay entre una ciudad origen es 0

    for (int i = 0; i < n - 1; i++) {//recorre casi todas las ciudades(nodos)

        float minimo = INF;
        int u = -1;

        for (int j = 0; j < n; j++) {
            if (!visitados[j] && distancias[j] < minimo) {
                minimo = distancias[j];
                u = j;
            }
        }//Este bucle busca cuál es la ciudad j que no ha sido visitada aún y que tenga la distancia más corta acumulada hasta el momento 
        //La ciudad elegida se guarda en la variable u

        if (u == -1) break;//Si u se quedó en -1, rompe el ciclo

        visitados[u] = true; //Se marca la ciudad u como visitada para no volver a analizarla

        for (int v = 0; v < n; v++) {//Revisa todas las ciudades vecinas v de la ciudad actual u

            if (!visitados[v] &&
                matriz[u][v] != INF &&
                distancias[u] + matriz[u][v] < distancias[v]) {

                distancias[v] = distancias[u] + matriz[u][v];
                padres[v] = u;
            }
        }
    }//Si encontramos que ir a v pasando por u (distancias[u] + matriz[u][v]) 
    //es más corto que la distancia que ya teníamos anotada para v (distancias[v]), 
    //actualizamos el registro de distancia y anotamos que el "padre" de v ahora es u

    ResultadoRuta resultado;
    resultado.distanciaTotal = distancias[fin];
    //Crea el objeto para el reporte final y le asigna la distancia definitiva que costo llegar a la ciudad destino(fin)

    if (distancias[fin] == INF) {
        resultado.descripcionTexto = "No existe ruta.";
        
        //Antes de salir de la función con 'return', debemos liberar la memoria de los arreglos auxiliares
        delete[] distancias;
        delete[] visitados;
        delete[] padres;
        
        return resultado;
    }
    //Si la distancia final sigue siendo INF 
    //significa que es imposible llegar de forma terrestre
    //Escribe el error y termina la función de inmediato

    int actual = fin;

    //contamos cuántas ciudades componen el camino final
    int contadorCiudades = 0;
    while (actual != -1) {
        contadorCiudades++;
        actual = padres[actual];
    }

    // Reservamos la memoria exacta para el arreglo del camino y guardamos su tamaño en la estructura
    resultado.camino = new int[contadorCiudades];
    resultado.cantidadCiudades = contadorCiudades;

    // Como Dijkstra guarda los padres de atrás para adelante, 
    // llenamos el arreglo dinámico desde la última posición hacia la primera (de fin a inicio).
    actual = fin;
    for (int i = contadorCiudades - 1; i >= 0; i--) {
        resultado.camino[i] = actual;
        actual = padres[actual];
    }

    resultado.descripcionTexto = "Ruta calculada correctamente.";

    // liberamos los arreglos auxiliares creados dentro de la función para evitar fugas de memoria
    delete[] distancias;
    delete[] visitados;
    delete[] padres;

    return resultado;
}
//escribe el mensaje de éxito y devuelve todo el resultado

Grafo::~Grafo() {

    for (int i = 0; i < n; i++) {
        delete[] matriz[i];
    }

    delete[] matriz;
}
//Muy importante!!! Como usamos new en el constructor, hay que liberar esa memoria con delete
//Primero recorre fila por fila y borra cada uno de los vectores de las columnas (delete[]).
//Finalmente, borra el arreglo principal que sostenía a las filas.