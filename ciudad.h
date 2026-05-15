#ifndef CIUDAD_H
#define CIUDAD_H

// Definimos una estructura (struct) para representar cada nodo del grafo
struct Ciudad {
    int id;           // Identificador numérico (0, 1, 2...)
    char nombre[50];  // Nombre de la ciudad (ej: "Villa Maria")
    float x;          // Coordenada X (para ubicación en un plano)
    float y;          // Coordenada Y
};

#endif