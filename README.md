# 🚚 Sistema Inteligente de Gestión Logística y Optimización de Rutas

## 📌 Descripción del Proyecto

Este proyecto fue desarrollado como trabajo académico para la materia **Estructura de Datos y Algoritmos II (EDA II)**.

La aplicación permite gestionar una red logística de distribución mediante el uso de **teoría de grafos**, permitiendo administrar ciudades, rutas de conexión y calcular el camino óptimo entre dos puntos.

---

## 🎯 Objetivos

- Gestionar ciudades dentro de una red logística.
- Crear, modificar y eliminar rutas entre nodos.
- Calcular rutas óptimas entre origen y destino.
- Gestionar contingencias como rutas cortadas.
- Mantener persistencia de los datos.
- Registrar historial de consultas realizadas.

---

## 🛠 Tecnologías Utilizadas

- **Lenguaje:** C++
- **Paradigma:** Programación Orientada a Objetos (POO)

## Estructuras de Datos

- Matriz de Adyacencia
- `vector`
- `stack`

### Algoritmo

- Dijkstra (camino mínimo)

---

## 📂 Estructura del Proyecto

PROYECTOUPE_GESTIONLOGISTICA/
├── main.cpp
├── grafo.h
├── grafo.cpp
├── sistemagestionlogistico.h
├── sistemagestionlogistico.cpp
├── ciudad.h
├── ciudades.dat
└── README.md