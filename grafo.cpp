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

    std::vector<float> distancias(n, INF);//se crea un vector dinamico para guardar las distancias, todas comienzan con valor INF
    std::vector<bool> visitados(n, false);//se crea un vector dinamico para marcar las ciudades visitadas, todas son F al principio
    std::vector<int> padres(n, -1);//en vez de padres poner ciudad_padre/ se crea una lista para recordar de que ciudad vinimps
    //sirve para reconstruir la ruta al final, empieza en -1 porque no hay ninguna ruta al principio(padre)

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
    //Crea el objeto para el reporte final y le asigna la distancia definitiva que costó llegar a la ciudad destino(fin)

    if (distancias[fin] == INF) {
        resultado.descripcionTexto = "No existe ruta.";
        return resultado;
    }
    //Si la distancia final sigue siendo INF 
    //significa que es imposible llegar de forma terrestre
    //Escribe el error y termina la función de inmediato

    int actual = fin;

    while (actual != -1) {
        resultado.camino.push_back(actual);//push_back() es una funcion que añade un elemento al final de contenedores dinamicos
        //redimensiona el contenedor si es necesario
        actual = padres[actual];
    }
    //Como Dijkstra guarda los padres de atrás para adelante, 
    //empezamos en la ciudad fin y vamos saltando hacia atrás (padres[actual]) 
    //guardando cada ciudad en la lista camino, hasta llegar al origen (-1).

    std::reverse(//funcion que da vuelta el orden de los elementos, es una funcion de <algorithm>
        resultado.camino.begin(),
        resultado.camino.end()
    );
    //Como el camino se guardó al revés (ej: [Destino, Ciudad_B, Ciudad_A, Inicio]), 
    //usamos std::reverse para darle la vuelta y dejarlo en el orden correcto: 
    //[Inicio, Ciudad_A, Ciudad_B, Destino].

    resultado.descripcionTexto = "Ruta calculada correctamente.";

    return resultado;
}
//Escribe el mensaje de éxito y devuelve todo el resultado

Grafo::~Grafo() {

    for (int i = 0; i < n; i++) {
        delete[] matriz[i];
    }

    delete[] matriz;
}
//Muy importante!!! Como usamos new en el constructor, hay que liberar esa memoria con delete
//Primero recorre fila por fila y borra cada uno de los vectores de las columnas (delete[]).
//Finalmente, borra el arreglo principal que sostenía a las filas.