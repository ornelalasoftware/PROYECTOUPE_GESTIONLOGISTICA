#include <iostream>
#include <vector>
#include <string>
#include "sistemagestionlogistico.h" // Conectamos con la lógica de archivos y grafos

using namespace std;

void mostrarMenu() {
    cout << "\n==============================================" << endl;
    cout << "   SISTEMA LOGISTICO INTELIGENTE - CORDOBA    " << endl;
    cout << "==============================================" << endl;
    cout << " 1. Ver listado de ciudades" << endl;
    cout << " 2. Calcular ruta mas corta (Dijkstra)" << endl;
    cout << " 3. Reportar corte de ruta (Contingencia)" << endl;
    cout << " 4. Ver historial de consultas" << endl;
    cout << " 5. Guardar datos y Salir" << endl;
    cout << "==============================================" << endl;
    cout << " Seleccione una opcion: ";
}

int main() {
    SistemaLogistico sistema;

    // --- CARGA INICIAL (Solo la primera vez o para demostración) ---
    // Agregamos los nodos (Ciudades de Córdoba)
    sistema.agregarCiudad("Cordoba Capital", 0, 0);      // ID 0
    sistema.agregarCiudad("Villa Maria", 150, 150);      // ID 1
    sistema.agregarCiudad("Rio Cuarto", 0, 230);         // ID 2
    sistema.agregarCiudad("Villa Carlos Paz", -35, 0);   // ID 3
    sistema.agregarCiudad("San Francisco", 210, 0);      // ID 4

    // Agregamos las aristas (Rutas con sus KM)
    sistema.agregarRuta(0, 1, 150.0); // Cba - Villa María
    sistema.agregarRuta(1, 2, 130.0); // Villa María - Río Cuarto
    sistema.agregarRuta(0, 3, 35.0);  // Cba - Carlos Paz
    sistema.agregarRuta(0, 4, 210.0); // Cba - San Francisco
    sistema.agregarRuta(3, 2, 215.0); // Carlos Paz - Río Cuarto (Ruta alternativa)

    int opcion;
    do {
        mostrarMenu();
        if (!(cin >> opcion)) { // Validar que entre un número
            cin.clear(); //desbloque al cin porque si le colocan una letra se enoja
            cin.ignore(1000, '\n'); //limpia el buffer hasta 1000 caracteres hasta que hagan un salto de linea
            continue; //continua el bucle y vuelve a mostrarMenu()
        }

        switch (opcion) {
            case 1:
                cout << "\n--- CIUDADES EN LA RED ---" << endl;
                cout << "0: Cordoba Cap. | 1: Villa Maria | 2: Rio Cuarto" << endl;
                cout << "3: Carlos Paz   | 4: San Francisco" << endl;
                break;

            case 2: {
                int origen, destino;
                cout << "Ingrese ID de ciudad de salida: "; cin >> origen;
                cout << "Ingrese ID de ciudad de destino: "; cin >> destino;

                ResultadoRuta resultado = sistema.calcularRutaOptima(origen, destino);

                if (resultado.distanciaTotal < 999999) {
                    cout << "\n[EXITO] Ruta encontrada: " << resultado.distanciaTotal << " km." << endl;
                    cout << "Estado: " << resultado.descripcionTexto << endl;
                } else {
                    cout << "\n[ERROR] No hay rutas disponibles" << endl;
                }
                break;
            }

            case 3: {
                int origenId, destinoId; 
                cout << "Ingrese IDs de la ruta bloqueada (ej: 0 1): ";
                cin >> origenId >> destinoId;
                sistema.cortarRuta(origenId, destinoId);
                break;
            }

            case 4:
                sistema.mostrarHistorial();
                break;

            case 5:
                sistema.guardarDatos();
                cout << "Saliendo del sistema..." << endl;
                break;

            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 5);

    return 0;
}