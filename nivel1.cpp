#include <iostream>
#include <string>
#include <cstdlib>
#include <queue>
using namespace std;

struct Nodo {
    int id;
    string nombre;
    int anioReinado;    // anioReinado
    string cargo;       // cargo
    Nodo *izq;
    Nodo *der;
};

Nodo *arbol = NULL;

// Prototipos de funciones
Nodo *crearNodo(int id, string nombre, int anioReinado, string cargo);
void insertarNodo(Nodo *&arbol, int id, string nombre, int anioReinado, string cargo);
void mostrarArbol(Nodo *arbol, int contador);
Nodo *buscarMiembro(Nodo *arbol, int id);
Nodo *buscarPorNombre(Nodo *arbol, string nombre);
void mostrarAscendentes(Nodo *arbol, int id);
void mostrarDescendientes(Nodo *nodo);
int contarDescendientes(Nodo *nodo);
int contarGeneraciones(Nodo *nodo);
Nodo *gobernanteMasAntiguo(Nodo *arbol); 
Nodo *eliminarNodo(Nodo *arbol, int id);
void recorridoInorden(Nodo *arbol);
void recorridoPreorden(Nodo *arbol);
void recorridoPostorden(Nodo *arbol);
void recorridoPorNiveles(Nodo *arbol);
void mostrarPorGeneracion(Nodo *arbol, int generacionActual, int generacionBuscada);
void mostrarLinajeReal(Nodo *arbol);  // Nueva función específica
void menu();

// Función principal
int main() {
    system("cls");
    cout << "=====================================================\n";
    cout << "   SISTEMA DE ARBOL GENEALOGICO DEL TAWANTINSUYO\n";
    cout << "          (Imperio Inca - Siglos XII-XVI)\n";
    cout << "=====================================================\n";
    menu();
    return 0;
}

// Crear nuevo nodo 
Nodo *crearNodo(int id, string nombre, int anioReinado, string cargo) {
    Nodo *nuevoNodo = new Nodo();
    nuevoNodo->id = id;
    nuevoNodo->nombre = nombre;
    nuevoNodo->anioReinado = anioReinado;
    nuevoNodo->cargo = cargo;
    nuevoNodo->izq = NULL;
    nuevoNodo->der = NULL;
    return nuevoNodo;
}

// Insertar nodo en Arbol
void insertarNodo(Nodo *&arbol, int id, string nombre, int anioReinado, string cargo) {
    if (arbol == NULL) {
        arbol = crearNodo(id, nombre, anioReinado, cargo);
        cout << "\n* " << cargo << " '" << nombre << "' agregado al linaje.\n";
    } else {
        if (id < arbol->id) {
            insertarNodo(arbol->izq, id, nombre, anioReinado, cargo);
        } else if (id > arbol->id) {
            insertarNodo(arbol->der, id, nombre, anioReinado, cargo);
        } else {
            cout << "\n Error: ID " << id << " ya existe. Use un ID unico.\n";
        }
    }
}

// Mostrar árbol de forma jerárquica
void mostrarArbol(Nodo *arbol, int contador) {
    if (arbol == NULL) {
        return;
    }
    mostrarArbol(arbol->der, contador + 1);
    
    for (int i = 0; i < contador; i++) {
        cout << "    ";
    }
    cout << "[" << arbol->id << "] " << arbol->nombre << " (" << arbol->cargo << ")\n";
    
    mostrarArbol(arbol->izq, contador + 1);
}

// Búsqueda por ID
Nodo *buscarMiembro(Nodo *arbol, int id) {
    if (arbol == NULL) {
        return NULL;
    } else if (arbol->id == id) {
        return arbol;
    } else if (id < arbol->id) {
        return buscarMiembro(arbol->izq, id);
    } else {
        return buscarMiembro(arbol->der, id);
    }
}

// Búsqueda por nombre
Nodo *buscarPorNombre(Nodo *arbol, string nombre) {
    if (arbol == NULL) return NULL;
    
    if (arbol->nombre == nombre) return arbol;
    
    Nodo *izq = buscarPorNombre(arbol->izq, nombre);
    if (izq != NULL) return izq;
    
    return buscarPorNombre(arbol->der, nombre);
}

// Mostrar ascendentes (adaptado para linaje real)
void mostrarAscendentes(Nodo *arbol, int id) {
    if (arbol == NULL) return;
    
    if (arbol->id == id) {
        cout << "  • " << arbol->nombre << " (ACTUAL) - " << arbol->cargo << endl;
        return;
    }
    
    if (id < arbol->id) {
        mostrarAscendentes(arbol->izq, id);
        if (arbol->id != id) {
            cout << "  • " << arbol->nombre << " - " << arbol->cargo 
                 << " (Reinó: " << arbol->anioReinado << ")" << endl;
        }
    } else {
        mostrarAscendentes(arbol->der, id);
        if (arbol->id != id) {
            cout << "  • " << arbol->nombre << " - " << arbol->cargo 
                 << " (Reinó: " << arbol->anioReinado << ")" << endl;
        }
    }
}

// Mostrar descendientes directos
void mostrarDescendientes(Nodo *nodo) {
    if (nodo == NULL) return;
    
    cout << "  • " << nodo->nombre << " [" << nodo->id << "] - " 
         << nodo->cargo << " (Reinó: " << nodo->anioReinado << ")" << endl;
    mostrarDescendientes(nodo->izq);
    mostrarDescendientes(nodo->der);
}

// Contar total de descendientes
int contarDescendientes(Nodo *nodo) {
    if (nodo == NULL) return 0;
    return 1 + contarDescendientes(nodo->izq) + contarDescendientes(nodo->der);
}

// Contar generaciones desde un nodo
int contarGeneraciones(Nodo *nodo) {
    if (nodo == NULL) return 0;
    
    int izq = contarGeneraciones(nodo->izq);
    int der = contarGeneraciones(nodo->der);
    
    return 1 + max(izq, der);
}

// Encontrar gobernante más antiguo
Nodo *gobernanteMasAntiguo(Nodo *arbol) {
    if (arbol == NULL) return NULL;
    
    Nodo *actual = arbol;
    Nodo *izq = gobernanteMasAntiguo(arbol->izq);
    Nodo *der = gobernanteMasAntiguo(arbol->der);
    
    Nodo *masAntiguo = actual;
    
    if (izq != NULL && izq->anioReinado < masAntiguo->anioReinado) {
        masAntiguo = izq;
    }
    if (der != NULL && der->anioReinado < masAntiguo->anioReinado) {
        masAntiguo = der;
    }
    
    return masAntiguo;
}

// Eliminar nodo
Nodo *eliminarNodo(Nodo *arbol, int id) {
    if (arbol == NULL) {
        cout << "\n✗ Miembro no encontrado.\n";
        return arbol;
    }
    
    if (id < arbol->id) {
        arbol->izq = eliminarNodo(arbol->izq, id);
    } else if (id > arbol->id) {
        arbol->der = eliminarNodo(arbol->der, id);
    } else {
        cout << "\n✓ Gobernante '" << arbol->nombre << "' eliminado del registro.\n";
        
        if (arbol->izq == NULL) {
            Nodo *temp = arbol->der;
            delete arbol;
            return temp;
        } else if (arbol->der == NULL) {
            Nodo *temp = arbol->izq;
            delete arbol;
            return temp;
        }
        
        Nodo *sucesor = arbol->der;
        while (sucesor && sucesor->izq != NULL) {
            sucesor = sucesor->izq;
        }
        
        arbol->id = sucesor->id;
        arbol->nombre = sucesor->nombre;
        arbol->anioReinado = sucesor->anioReinado;
        arbol->cargo = sucesor->cargo;
        
        arbol->der = eliminarNodo(arbol->der, sucesor->id);
    }
    return arbol;
}

// RECORRIDO INORDEN
void recorridoInorden(Nodo *arbol) {
    if (arbol == NULL) return;
    
    recorridoInorden(arbol->izq);
    cout << "  • " << arbol->nombre << " [" << arbol->id << "] - " 
         << arbol->cargo << " (Reinó: " << arbol->anioReinado << ")\n";
    recorridoInorden(arbol->der);
}

// RECORRIDO PREORDEN
void recorridoPreorden(Nodo *arbol) {
    if (arbol == NULL) return;
    
    cout << "  • " << arbol->nombre << " [" << arbol->id << "] - " 
         << arbol->cargo << " (Reinó: " << arbol->anioReinado << ")\n";
    recorridoPreorden(arbol->izq);
    recorridoPreorden(arbol->der);
}

// RECORRIDO POSTORDEN
void recorridoPostorden(Nodo *arbol) {
    if (arbol == NULL) return;
    
    recorridoPostorden(arbol->izq);
    recorridoPostorden(arbol->der);
    cout << "  • " << arbol->nombre << " [" << arbol->id << "] - " 
         << arbol->cargo << " (Reinó: " << arbol->anioReinado << ")\n";
}

// RECORRIDO POR NIVELES
void recorridoPorNiveles(Nodo *arbol) {
    if (arbol == NULL) return;
    
    queue<Nodo*> cola;
    cola.push(arbol);
    
    int nivel = 0;
    while (!cola.empty()) {
        int nodosEnNivel = cola.size();
        cout << "\n--- Generacion " << nivel << " ---\n";
        
        for (int i = 0; i < nodosEnNivel; i++) {
            Nodo *actual = cola.front();
            cola.pop();
            
            cout << "  • " << actual->nombre << " [" << actual->id << "] - " 
                 << actual->cargo << " (Reinó: " << actual->anioReinado << ")\n";
            
            if (actual->izq != NULL) cola.push(actual->izq);
            if (actual->der != NULL) cola.push(actual->der);
        }
        nivel++;
    }
}

//Mostrar miembros por generación específica
void mostrarPorGeneracion(Nodo *arbol, int generacionActual, int generacionBuscada) {
    if (arbol == NULL) return;
    
    if (generacionActual == generacionBuscada) {
        cout << "  • " << arbol->nombre << " [" << arbol->id << "] - " 
             << arbol->cargo << " (Reinó: " << arbol->anioReinado << ")\n";
        return;
    }
    
    mostrarPorGeneracion(arbol->izq, generacionActual + 1, generacionBuscada);
    mostrarPorGeneracion(arbol->der, generacionActual + 1, generacionBuscada);
}

//NUEVA FUNCIÓN: Mostrar linaje real completo en orden histórico
void mostrarLinajeReal(Nodo *arbol) {
    if (arbol == NULL) return;
    
    mostrarLinajeReal(arbol->izq);
    cout << "  • " << arbol->nombre << " - " << arbol->cargo 
         << " (Aprox. " << arbol->anioReinado << " d.C.)\n";
    mostrarLinajeReal(arbol->der);
}

//MENU PRINCIPAL - Tawantinsuyo
void menu() {
    int opcion, id, anio, generacion;
    string nombre, cargo;
    
    // ============================================
    // DATOS HISTÓRICOS DEL TAWANTINSUYO -IMPERIO INCA
    // ============================================
    cout << "\nCargando datos historicos del Tawantinsuyo...\n";
    
    // Sapa Incas (Gobernantes principales)
    insertarNodo(arbol, 100, "Manco Cápac", 1200, "Sapa Inca Fundador");
    insertarNodo(arbol, 50, "Sinchi Roca", 1230, "Sapa Inca");
    insertarNodo(arbol, 150, "Lloque Yupanqui", 1260, "Sapa Inca");
    insertarNodo(arbol, 30, "Mayta Cápac", 1290, "Sapa Inca");
    insertarNodo(arbol, 70, "Cápac Yupanqui", 1320, "Sapa Inca");
    insertarNodo(arbol, 130, "Inca Roca", 1350, "Sapa Inca");
    insertarNodo(arbol, 10, "Yáhuar Huácac", 1380, "Sapa Inca");
    insertarNodo(arbol, 90, "Viracocha Inca", 1410, "Sapa Inca");
    insertarNodo(arbol, 180, "Pachacútec", 1438, "Sapa Inca (Expansor)");
    insertarNodo(arbol, 60, "Túpac Inca Yupanqui", 1471, "Sapa Inca");
    insertarNodo(arbol, 200, "Huayna Cápac", 1493, "Sapa Inca");
    insertarNodo(arbol, 170, "Huáscar", 1525, "Sapa Inca (Guerra Civil)");
    insertarNodo(arbol, 220, "Atahualpa", 1532, "Último Sapa Inca");
    
    // Coya (Esposas principales) y familia real
    insertarNodo(arbol, 25, "Mama Ocllo", 1200, "Coya Fundadora");
    insertarNodo(arbol, 55, "Mama Cora", 1230, "Coya");
    insertarNodo(arbol, 125, "Mama Cahua", 1350, "Coya");
    insertarNodo(arbol, 185, "Mama Anahuarque", 1440, "Coya de Pachacútec");
    
    // Hijos importantes y nobles
    insertarNodo(arbol, 210, "Ninan Cuyuchi", 1520, "Príncipe Heredero");
    insertarNodo(arbol, 140, "Amaru Inca Yupanqui", 1450, "Príncipe");
    insertarNodo(arbol, 80, "Pahuac Gualpa Mayta", 1325, "Noble Real");
    
    // Gobernantes durante la conquista
    insertarNodo(arbol, 250, "Manco Inca Yupanqui", 1533, "Sapa Inca (Rebelde)");
    insertarNodo(arbol, 280, "Sayri Túpac", 1545, "Sapa Inca en Vilcabamba");
    insertarNodo(arbol, 300, "Titu Cusi Yupanqui", 1560, "Sapa Inca en Vilcabamba");
    insertarNodo(arbol, 320, "Túpac Amaru I", 1571, "Último Inca de Vilcabamba");
    
    //cout << " " << contarDescendientes(arbol) << " miembros cargados.\n";
    //system("pause");
    
    do {
        system("cls");
        cout << "============================================================\n";
        cout << "      SISTEMA DE ARBOL GENEALOGICO DEL TAWANTINSUYO\n";
        cout << "            Imperio Inca (Siglos XIII-XVI)\n";
        cout << "============================================================\n";
        cout << "\nLinaje Real Actual (Estructura Jerárquica):\n";
        cout << "---------------------------------------------\n";
        mostrarArbol(arbol, 0);
        cout << "\n============================================================\n";
        cout << "MENU PRINCIPAL - ADMINISTRACION DEL LINAGE INCA:\n";
        cout << "============================================================\n";
        cout << "\n1.  Agregar nuevo miembro al linaje\n";
        cout << "2.  Mostrar árbol genealógico jerárquico\n";
        cout << "3.  Buscar miembro por ID\n";
        cout << "4.  Buscar miembro por nombre\n";
        cout << "5.  Mostrar ascendentes (antepasados reales)\n";
        cout << "6.  Mostrar descendientes (sucesores)\n";
        cout << "7.  Contar total de descendientes\n";
        cout << "8.  Encontrar gobernante más antiguo\n";
        cout << "9.  Eliminar miembro del registro\n";
        cout << "\n--- CONSULTAS HISTORICAS ---\n";
        cout << "10. Mostrar linaje en orden histórico (Inorden)\n";
        cout << "11. Recorrido Preorden (Raíz primero)\n";
        cout << "12. Recorrido Postorden (Hojas primero)\n";
        cout << "13. Mostrar por generaciones (Recorrido por niveles)\n";
        cout << "14. Mostrar miembros por generación específica\n";
        cout << "15. Mostrar profundidad del linaje (generaciones)\n";
        cout << "16. Mostrar linaje real completo en orden cronológico\n";
        cout << "\n0.  Salir del sistema\n";
        cout << "============================================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                cout << "\n--- AGREGAR NUEVO MIEMBRO AL LINAJE ---\n";
                cout << "ID (unico, ej: 400): ";
                cin >> id;
                cout << "Nombre: ";
                cin.ignore();
                getline(cin, nombre);
                cout << "Año aproximado de reinado/nacimiento (ej: 1550): ";
                cin >> anio;
                cout << "Cargo/Rol (ej: 'Sapa Inca', 'Coya', 'Principe', 'Noble'): ";
                cin.ignore();
                getline(cin, cargo);
                insertarNodo(arbol, id, nombre, anio, cargo);
                break;
                
            case 2:
                cout << "\n--- ARBOL GENEALOGICO JERARQUICO DEL LINAJE ---\n";
                mostrarArbol(arbol, 0);
                break;
                
            case 3:
                cout << "\n--- BUSCAR POR ID ---\n";
                cout << "Ingrese ID del miembro: ";
                cin >> id;
                {
                    Nodo *encontrado = buscarMiembro(arbol, id);
                    if (encontrado != NULL) {
                        cout << "\n✓ MIEMBRO DEL LINAJE ENCONTRADO:\n";
                        cout << "  Nombre: " << encontrado->nombre << endl;
                        cout << "  ID: " << encontrado->id << endl;
                        cout << "  Año de reinado: " << encontrado->anioReinado << " d.C.\n";
                        cout << "  Cargo: " << encontrado->cargo << endl;
                    } else {
                        cout << "\n✗ Miembro no encontrado en el linaje.\n";
                    }
                }
                break;
                
            case 4:
                cout << "\n--- BUSCAR POR NOMBRE ---\n";
                cout << "Ingrese nombre del miembro: ";
                cin.ignore();
                getline(cin, nombre);
                {
                    Nodo *encontrado = buscarPorNombre(arbol, nombre);
                    if (encontrado != NULL) {
                        cout << "\n MIEMBRO DEL LINAJE ENCONTRADO:\n";
                        cout << "  Nombre: " << encontrado->nombre << endl;
                        cout << "  ID: " << encontrado->id << endl;
                        cout << "  Año de reinado: " << encontrado->anioReinado << " d.C.\n";
                        cout << "  Cargo: " << encontrado->cargo << endl;
                    } else {
                        cout << "\n✗ Miembro no encontrado en el linaje.\n";
                    }
                }
                break;
                
            case 5:
                cout << "\n--- ASCENDENTES (ANTEPASADOS REALES) ---\n";
                cout << "Ingrese ID del miembro: ";
                cin >> id;
                {
                    Nodo *encontrado = buscarMiembro(arbol, id);
                    if (encontrado != NULL) {
                        cout << "\nAscendentes de " << encontrado->nombre << ":\n";
                        cout << "=================================\n";
                        mostrarAscendentes(arbol, id);
                    } else {
                        cout << "\n Miembro no encontrado.\n";
                    }
                }
                break;
                
            case 6:
                cout << "\n--- DESCENDIENTES (SUCESORES) ---\n";
                cout << "Ingrese ID del miembro: ";
                cin >> id;
                {
                    Nodo *encontrado = buscarMiembro(arbol, id);
                    if (encontrado != NULL) {
                        cout << "\nDescendientes de " << encontrado->nombre << ":\n";
                        cout << "==================================\n";
                        cout << "Descendientes directos:\n";
                        mostrarDescendientes(encontrado->izq);
                        mostrarDescendientes(encontrado->der);
                    } else {
                        cout << "\n Miembro no encontrado.\n";
                    }
                }
                break;
                
            case 7:
                cout << "\n--- CONTAR DESCENDIENTES ---\n";
                cout << "Ingrese ID del miembro: ";
                cin >> id;
                {
                    Nodo *encontrado = buscarMiembro(arbol, id);
                    if (encontrado != NULL) {
                        int total = contarDescendientes(encontrado) - 1;
                        cout << "\n" << encontrado->nombre << " tiene " << total 
                             << " descendiente(s) en total.\n";
                    } else {
                        cout << "\n Miembro no encontrado.\n";
                    }
                }
                break;
                
            case 8:
                {
                    cout << "\n--- GOBERNANTE MAS ANTIGUO ---\n";
                    Nodo *antiguo = gobernanteMasAntiguo(arbol);
                    if (antiguo != NULL) {
                        cout << "\nEl gobernante más antiguo registrado es:\n";
                        cout << "  Nombre: " << antiguo->nombre << endl;
                        cout << "  Año aproximado: " << antiguo->anioReinado << " d.C.\n";
                        cout << "  Cargo: " << antiguo->cargo << endl;
                    }
                }
                break;
                
            case 9:
                cout << "\n--- ELIMINAR MIEMBRO DEL REGISTRO ---\n";
                cout << "Ingrese ID del miembro a eliminar: ";
                cin >> id;
                arbol = eliminarNodo(arbol, id);
                break;
                
            case 10:
                cout << "\n--- LINAJE EN ORDEN HISTORICO (INORDEN) ---\n";
                cout << "(Ordenado por ID ascendente)\n";
                cout << "=============================================\n";
                recorridoInorden(arbol);
                break;
                
            case 11:
                cout << "\n--- RECORRIDO PREORDEN ---\n";
                cout << "(Raíz primero, luego subárboles)\n";
                recorridoPreorden(arbol);
                break;
                
            case 12:
                cout << "\n--- RECORRIDO POSTORDEN ---\n";
                cout << "(Subárboles primero, luego raíz)\n";
                recorridoPostorden(arbol);
                break;
                
            case 13:
                cout << "\n--- GENERACIONES DEL LINAJE REAL ---\n";
                cout << "(Recorrido por niveles)\n";
                recorridoPorNiveles(arbol);
                break;
                
            case 14:
                cout << "\n--- MIEMBROS POR GENERACION ESPECIFICA ---\n";
                cout << "Ingrese número de generación (0 = raíz): ";
                cin >> generacion;
                cout << "\nMiembros en generación " << generacion << ":\n";
                mostrarPorGeneracion(arbol, 0, generacion);
                break;
                
            case 15:
                cout << "\n--- PROFUNDIDAD DEL LINAJE ---\n";
                cout << "El linaje real tiene " << contarGeneraciones(arbol) 
                     << " generación(es) en total.\n";
                break;
                
            case 16:
                cout << "\n--- LINAJE REAL COMPLETO EN ORDEN CRONOLOGICO ---\n";
                cout << "===================================================\n";
                mostrarLinajeReal(arbol);
                break;
                
            case 0:
                cout << "\n============================================================\n";
                cout << "  Gracias por usar el Sistema Genealógico del Tawantinsuyo\n";
                cout << "            ¡Que el Inti ilumine tu camino!\n";
                cout << "============================================================\n";
                return;
                
            default:
                cout << "\n Opción inválida. Intente nuevamente.\n";
        }
        
        if (opcion != 0) {
            cout << "\n============================================================\n";
            cout << "Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 0);
}