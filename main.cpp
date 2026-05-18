#include <iostream>
#include <string>
#include "sistemagestionlogistico.h" // conectamos con la lógica de archivos y grafos

using namespace std;

void mostrarMenu() {
    cout << "\n==============================================" << endl;
    cout << "   SISTEMA DE GESTION LOGISTICA - CORDOBA    " << endl;
    cout << "==============================================" << endl;
    cout << " 1. Ver listado de ciudades" << endl;
    cout << " 2. Calcular ruta mas corta" << endl;
    cout << " 3. Reportar corte de ruta" << endl;
    cout << " 4. Ver historial de consultas" << endl;
    cout << " 5. Guardar datos y Salir" << endl;
    cout << "==============================================" << endl;
    cout << " Seleccione una opcion: ";
}

int main() {
    SistemaLogistico sistema;

    // CARGA INICIAL DEMOSTRATIVA.. HAY QUE CREAR EL ARCHIVO BINARIO TODAVIA :(
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
    sistema.agregarRuta(3, 2, 215.0); // Carlos Paz - Río Cuarto 

    int opcion;
    do {
        mostrarMenu();
        if (!(cin >> opcion)) { // Validar que entre un número
            cin.clear(); //desbloquea al cin porque si le colocan una letra se enoja
            cin.ignore(1000, '\n'); //limpia el buffer hasta 1000 caracteres hasta que hagan un salto de linea
            opcion = -1; // le asignamos un valor inválido para que el switch vaya al default y no rompa nada
        }

        switch (opcion) {
            case 1:
                cout << "\n--- CIUDADES EN LA RED ---" << endl;
                cout << "0: Cordoba Cap. | 1: Villa Maria | 2: Rio Cuarto" << endl;
                cout << "3: Carlos Paz   | 4: San Francisco" << endl;
                cout << "----------------------------------------\n";
                break;

            case 2: { // Caso para calcular ruta óptima
                int origen, destino;
                cout << "Ingrese el ID de la ciudad de origen: ";
                cin >> origen;
                cout << "Ingrese el ID de la ciudad de destino: ";
                cin >> destino;

                // llamada a función que ejecuta Dijkstra
                ResultadoRuta resultado = sistema.calcularRutaOptima(origen, destino);

                cout << "\n----------------------------------------\n";
                cout << "ESTADO: " << resultado.descripcionTexto << "\n";

                if (resultado.camino != nullptr && resultado.cantidadCiudades > 0) {
                    cout << "DISTANCIA TOTAL: " << resultado.distanciaTotal << " km\n";
                    cout << "RUTA RECOMENDADA: ";

                    // Recorre vector para mostrar las ciudades
                    for (int i = 0; i < resultado.cantidadCiudades; i++) {
                        int idCiudadActual = resultado.camino[i];
                        
                        cout << sistema.obtenerNombreCiudad(idCiudadActual);//imprimimos nombre de ciudad

                        //ponemos flecha menos en la ultima
                        if (i < resultado.cantidadCiudades - 1) {
                            cout << " -> ";
                        }
                    }
                    cout << "\n";
                } else {
                    cout << "No hay ruta disponible para mostrar\n";
                }
                cout << "----------------------------------------\n";
                break;
            }

            case 3: {
                int origenId, destinoId; 
                cout << "Ingrese IDs de la ruta bloqueada (ej: 0 1): ";
                cin >> origenId >> destinoId;
                sistema.cortarRuta(origenId, destinoId);
                cout <<"Se ha informado correctamente al sistema que la ruta esta bloqueada"<<endl;
                cout << "----------------------------------------\n";
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
                cout << "Opcion no valida" << endl;
                cout << "----------------------------------------\n";
        }
    } while (opcion != 5);

    return 0;
}