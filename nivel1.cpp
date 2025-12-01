#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstring>
using namespace std;

struct Persona {
    int id;
    string nombre;
    int anioNacimiento;  // Cambiado a solo año
    Persona* padre;
    Persona* madre;
    vector<Persona*> hijos;
    
    Persona(int id, string nombre, int anio) {  // Cambiado parámetro
        this->id = id;
        this->nombre = nombre;
        this->anioNacimiento = anio;  // Solo año
        this->padre = NULL;
        this->madre = NULL;
    }
    
    void agregarHijo(Persona* hijo) {
        hijos.push_back(hijo);
    }
};

struct NodoABB {
    Persona* persona;
    NodoABB* izquierdo;
    NodoABB* derecho;
    int altura;
    
    NodoABB(Persona* p) {
        persona = p;
        izquierdo = NULL;
        derecho = NULL;
        altura = 1;
    }
};

class ArbolGenealogicoABB {
private:
    NodoABB* raiz;
    map<int, Persona*> registro;
    map<int, bool> yaMostradas;
    
    int obtenerAltura(NodoABB* nodo) {
        if (nodo == NULL) return 0;
        return nodo->altura;
    }
    
    int obtenerBalance(NodoABB* nodo) {
        if (nodo == NULL) return 0;
        return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
    }
    
    NodoABB* rotacionDerecha(NodoABB* y) {
        NodoABB* x = y->izquierdo;
        NodoABB* T2 = x->derecho;
        
        x->derecho = y;
        y->izquierdo = T2;
        
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        
        return x;
    }
    
    NodoABB* rotacionIzquierda(NodoABB* x) {
        NodoABB* y = x->derecho;
        NodoABB* T2 = y->izquierdo;
        
        y->izquierdo = x;
        x->derecho = T2;
        
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        
        return y;
    }
    
    NodoABB* insertarEnABB(NodoABB* nodo, Persona* persona) {
        if (nodo == NULL) return new NodoABB(persona);
        
        if (persona->id < nodo->persona->id)
            nodo->izquierdo = insertarEnABB(nodo->izquierdo, persona);
        else if (persona->id > nodo->persona->id)
            nodo->derecho = insertarEnABB(nodo->derecho, persona);
        else
            return nodo;
        
        nodo->altura = 1 + max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));
        int balance = obtenerBalance(nodo);
        
        if (balance > 1 && persona->id < nodo->izquierdo->persona->id)
            return rotacionDerecha(nodo);
        
        if (balance < -1 && persona->id > nodo->derecho->persona->id)
            return rotacionIzquierda(nodo);
        
        if (balance > 1 && persona->id > nodo->izquierdo->persona->id) {
            nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
            return rotacionDerecha(nodo);
        }
        
        if (balance < -1 && persona->id < nodo->derecho->persona->id) {
            nodo->derecho = rotacionDerecha(nodo->derecho);
            return rotacionIzquierda(nodo);
        }
        
        return nodo;
    }
    
    Persona* buscarEnABB(NodoABB* nodo, int id) {
        if (nodo == NULL) return NULL;
        
        if (id < nodo->persona->id)
            return buscarEnABB(nodo->izquierdo, id);
        else if (id > nodo->persona->id)
            return buscarEnABB(nodo->derecho, id);
        else
            return nodo->persona;
    }
    
    void limpiarArbol(NodoABB* nodo) {
        if (nodo == NULL) return;
        limpiarArbol(nodo->izquierdo);
        limpiarArbol(nodo->derecho);
        delete nodo->persona;
        delete nodo;
    }
    
    void mostrarPersona(Persona* p, int nivel) {
        if (p == NULL) return;
        
        string indent(nivel * 4, ' ');
        cout << indent << "+-- " << p->nombre 
             << " (ID: " << p->id 
             << ", Nac: " << p->anioNacimiento << ")";  // Cambiado para mostrar solo año
        
        if (p->padre != NULL || p->madre != NULL) {
            cout << " [";
            if (p->padre != NULL) cout << "Padre: " << p->padre->nombre;
            if (p->padre != NULL && p->madre != NULL) cout << ", ";
            if (p->madre != NULL) cout << "Madre: " << p->madre->nombre;
            cout << "]";
        }
        cout << endl;
    }
    
    void mostrarArbolRecursivo(Persona* persona, int nivel) {
        if (persona == NULL) return;
        
        if (yaMostradas.find(persona->id) != yaMostradas.end()) {
            string indent(nivel * 4, ' ');
            cout << indent << "+-- [Ya mostrado: " << persona->nombre << "]" << endl;
            return;
        }
        
        yaMostradas[persona->id] = true;
        mostrarPersona(persona, nivel);
        
        if (persona->padre != NULL) {
            mostrarArbolRecursivo(persona->padre, nivel + 1);
        }
        
        if (persona->madre != NULL) {
            mostrarArbolRecursivo(persona->madre, nivel + 1);
        }
        
        for (int i = 0; i < persona->hijos.size(); i++) {
            mostrarArbolRecursivo(persona->hijos[i], nivel + 1);
        }
    }
    
    void mostrarAscendencia(Persona* persona, int nivel) {
        if (persona == NULL) return;
        
        mostrarPersona(persona, nivel);
        
        if (persona->padre != NULL) {
            cout << string((nivel+1) * 4, ' ') << "¦" << endl;
            cout << string((nivel+1) * 4, ' ') << "+-- Línea PATERNA:" << endl;
            mostrarAscendencia(persona->padre, nivel + 2);
        }
        
        if (persona->madre != NULL) {
            cout << string((nivel+1) * 4, ' ') << "¦" << endl;
            cout << string((nivel+1) * 4, ' ') << "+-- Línea MATERNA:" << endl;
            mostrarAscendencia(persona->madre, nivel + 2);
        }
    }
    
public:
    ArbolGenealogicoABB() {
        raiz = NULL;
    }
    
    ~ArbolGenealogicoABB() {
        limpiarArbol(raiz);
        registro.clear();
    }
    
    bool insertarPersona(int id, string nombre, int anio,  // Cambiado parámetro
                        int idPadre = -1, int idMadre = -1) {
        if (registro.find(id) != registro.end()) {
            cout << "Error: El ID " << id << " ya existe!\n";
            return false;
        }
        
        Persona* nuevaPersona = new Persona(id, nombre, anio);  // Solo año
        
        if (idPadre != -1) {
            map<int, Persona*>::iterator itPadre = registro.find(idPadre);
            if (itPadre != registro.end()) {
                nuevaPersona->padre = itPadre->second;
                itPadre->second->agregarHijo(nuevaPersona);
            } else {
                cout << "Advertencia: Padre con ID " << idPadre << " no encontrado. Se insertará sin padre.\n";
            }
        }
        
        if (idMadre != -1) {
            map<int, Persona*>::iterator itMadre = registro.find(idMadre);
            if (itMadre != registro.end()) {
                nuevaPersona->madre = itMadre->second;
                itMadre->second->agregarHijo(nuevaPersona);
            } else {
                cout << "Advertencia: Madre con ID " << idMadre << " no encontrado. Se insertará sin madre.\n";
            }
        }
        
        raiz = insertarEnABB(raiz, nuevaPersona);
        registro[id] = nuevaPersona;
        
        return true;
    }
    
    Persona* buscarPersona(int id) {
        map<int, Persona*>::iterator it = registro.find(id);
        if (it != registro.end()) {
            return it->second;
        }
        return buscarEnABB(raiz, id);
    }
    
    vector<Persona*> obtenerAncestros(int id) {
        vector<Persona*> ancestros;
        Persona* persona = buscarPersona(id);
        
        if (persona != NULL) {
            stack<Persona*> pila;
            if (persona->padre != NULL) pila.push(persona->padre);
            if (persona->madre != NULL) pila.push(persona->madre);
            
            while (!pila.empty()) {
                Persona* actual = pila.top();
                pila.pop();
                
                ancestros.push_back(actual);
                
                if (actual->padre != NULL) pila.push(actual->padre);
                if (actual->madre != NULL) pila.push(actual->madre);
            }
        }
        
        return ancestros;
    }
    
    vector<Persona*> obtenerDescendientes(int id) {
        vector<Persona*> descendientes;
        Persona* persona = buscarPersona(id);
        
        if (persona != NULL) {
            queue<Persona*> cola;
            for (int i = 0; i < persona->hijos.size(); i++) {
                cola.push(persona->hijos[i]);
            }
            
            while (!cola.empty()) {
                Persona* actual = cola.front();
                cola.pop();
                
                descendientes.push_back(actual);
                
                for (int i = 0; i < actual->hijos.size(); i++) {
                    cola.push(actual->hijos[i]);
                }
            }
        }
        
        return descendientes;
    }
    
    void mostrarArbolCompleto(int id) {
        Persona* persona = buscarPersona(id);
        if (persona == NULL) {
            cout << "Persona no encontrada.\n";
            return;
        }
        
        cout << "\n=== ÁRBOL GENEALÓGICO COMPLETO ===\n";
        yaMostradas.clear();
        mostrarArbolRecursivo(persona, 0);
        yaMostradas.clear();
    }
    
    void mostrarAscendenciaCompleta(int id) {
        Persona* persona = buscarPersona(id);
        if (persona == NULL) {
            cout << "Persona no encontrada.\n";
            return;
        }
        
        cout << "\n=== ASCENDENCIA ===\n";
        mostrarAscendencia(persona, 0);
    }
    
    void mostrarDescendenciaCompleta(int id) {
        Persona* persona = buscarPersona(id);
        if (persona == NULL) {
            cout << "Persona no encontrada.\n";
            return;
        }
        
        cout << "\n=== DESCENDENCIA ===\n";
        
        queue<pair<Persona*, int> > cola;
        cola.push(make_pair(persona, 0));
        
        while (!cola.empty()) {
            Persona* actual = cola.front().first;
            int nivel = cola.front().second;
            cola.pop();
            
            mostrarPersona(actual, nivel);
            
            for (int i = 0; i < actual->hijos.size(); i++) {
                cola.push(make_pair(actual->hijos[i], nivel + 1));
            }
        }
    }
    
    string determinarRelacion(int id1, int id2) {
        Persona* p1 = buscarPersona(id1);
        Persona* p2 = buscarPersona(id2);
        
        if (p1 == NULL || p2 == NULL) {
            return "Una o ambas personas no existen";
        }
        
        if (p1 == p2) {
            return "Es la misma persona";
        }
        
        vector<Persona*> ancestrosP2 = obtenerAncestros(id2);
        for (int i = 0; i < ancestrosP2.size(); i++) {
            if (ancestrosP2[i] == p1) {
                return p1->nombre + " es ancestro de " + p2->nombre;
            }
        }
        
        vector<Persona*> ancestrosP1 = obtenerAncestros(id1);
        for (int i = 0; i < ancestrosP1.size(); i++) {
            if (ancestrosP1[i] == p2) {
                return p2->nombre + " es ancestro de " + p1->nombre;
            }
        }
        
        vector<Persona*> todosP1 = ancestrosP1;
        todosP1.push_back(p1);
        vector<Persona*> todosP2 = ancestrosP2;
        todosP2.push_back(p2);
        
        Persona* ancestroComun = NULL;
        for (int i = 0; i < todosP1.size(); i++) {
            for (int j = 0; j < todosP2.size(); j++) {
                if (todosP1[i] == todosP2[j]) {
                    ancestroComun = todosP1[i];
                    break;
                }
            }
            if (ancestroComun != NULL) break;
        }
        
        if (ancestroComun == NULL) {
            return "No hay relación familiar directa";
        }
        
        int dist1 = 0, dist2 = 0;
        Persona* temp = p1;
        
        while (temp != NULL && temp != ancestroComun) {
            dist1++;
            if (temp->padre != NULL && 
                find(ancestrosP1.begin(), ancestrosP1.end(), temp->padre) != ancestrosP1.end()) {
                temp = temp->padre;
            }
            else if (temp->madre != NULL) {
                temp = temp->madre;
            }
            else {
                break;
            }
        }
        
        temp = p2;
        while (temp != NULL && temp != ancestroComun) {
            dist2++;
            if (temp->padre != NULL && 
                find(ancestrosP2.begin(), ancestrosP2.end(), temp->padre) != ancestrosP2.end()) {
                temp = temp->padre;
            }
            else if (temp->madre != NULL) {
                temp = temp->madre;
            }
            else {
                break;
            }
        }
        
        if (dist1 == 1 && dist2 == 1) {
            return p1->nombre + " y " + p2->nombre + " son hermanos";
        }
        else if (dist1 == 1 && dist2 == 2) {
            return p1->nombre + " es tío/tía de " + p2->nombre;
        }
        else if (dist1 == 2 && dist2 == 1) {
            return p2->nombre + " es tío/tía de " + p1->nombre;
        }
        else if (dist1 == 2 && dist2 == 2) {
            return p1->nombre + " y " + p2->nombre + " son primos hermanos";
        }
        else {
            stringstream ss;
            ss << p1->nombre << " y " << p2->nombre 
               << " están relacionados a través de " << ancestroComun->nombre
               << " (distancia: " << dist1 << " y " << dist2 << " generaciones)";
            return ss.str();
        }
    }
    
    void mostrarEstadisticas() {
        cout << "\n=== ESTADÍSTICAS ===\n";
        cout << "Total de personas: " << registro.size() << endl;
        
        int conPadre = 0, conMadre = 0, conAmbosPadres = 0, sinHijos = 0;
        
        for (map<int, Persona*>::iterator it = registro.begin(); it != registro.end(); ++it) {
            Persona* p = it->second;
            if (p->padre != NULL) conPadre++;
            if (p->madre != NULL) conMadre++;
            if (p->padre != NULL && p->madre != NULL) conAmbosPadres++;
            if (p->hijos.empty()) sinHijos++;
        }
        
        cout << "Personas con padre conocido: " << conPadre << endl;
        cout << "Personas con madre conocida: " << conMadre << endl;
        cout << "Personas con ambos padres: " << conAmbosPadres << endl;
        cout << "Personas sin hijos: " << sinHijos << endl;
        cout << "Personas con hijos: " << (registro.size() - sinHijos) << endl;
    }
};

int main() {
    system("chcp 65001 > nul");
    system("cls");
    
    ArbolGenealogicoABB arbolGenealogico;
    
    cout << "===============================================\n";
    cout << "   SISTEMA DE ÁRBOL GENEALÓGICO CON ABB\n";
    cout << "   (Solo años para fecha de nacimiento)\n";
    cout << "===============================================\n\n";
    
    int opcion;
    do {
        cout << "\n=== MENÚ PRINCIPAL ===\n";
        cout << "1. Insertar persona\n";
        cout << "2. Buscar persona\n";
        cout << "3. Mostrar árbol completo\n";
        cout << "4. Mostrar ascendencia\n";
        cout << "5. Mostrar descendencia\n";
        cout << "6. Mostrar ancestros (lista)\n";
        cout << "7. Mostrar descendientes (lista)\n";
        cout << "8. Determinar relación\n";
        cout << "9. Mostrar estadísticas\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1: {
                int id, idPadre, idMadre, anio;  // Cambiado a anio
                string nombre;
                
                cout << "ID: ";
                cin >> id;
                cout << "Nombre: ";
                cin.ignore();
                getline(cin, nombre);
                cout << "Año de nacimiento (ej: 1990): ";  // Cambiado
                cin >> anio;
                cout << "ID del padre (0 si no se conoce): ";  // Cambiado a 0
                cin >> idPadre;
                cout << "ID de la madre (0 si no se conoce): ";  // Cambiado a 0
                cin >> idMadre;
                
                // Convertir 0 a -1 para el sistema interno
                if (idPadre == 0) idPadre = -1;
                if (idMadre == 0) idMadre = -1;
                
                if (arbolGenealogico.insertarPersona(id, nombre, anio, idPadre, idMadre)) {
                    cout << "Persona insertada correctamente.\n";
                }
                break;
            }
            
            case 2: {
                int id;
                cout << "ID a buscar: ";
                cin >> id;
                
                Persona* p = arbolGenealogico.buscarPersona(id);
                if (p != NULL) {
                    cout << "\nPERSONA ENCONTRADA:\n";
                    cout << "Nombre: " << p->nombre << endl;
                    cout << "ID: " << p->id << endl;
                    cout << "Año de nacimiento: " << p->anioNacimiento << endl;  // Cambiado
                    cout << "Padre: " << (p->padre ? p->padre->nombre : "Desconocido") << endl;
                    cout << "Madre: " << (p->madre ? p->madre->nombre : "Desconocido") << endl;
                    cout << "Hijos: " << p->hijos.size() << endl;
                } else {
                    cout << "Persona no encontrada.\n";
                }
                break;
            }
            
            case 3: {
                int id;
                cout << "ID de la persona central: ";
                cin >> id;
                arbolGenealogico.mostrarArbolCompleto(id);
                break;
            }
            
            case 4: {
                int id;
                cout << "ID de la persona: ";
                cin >> id;
                arbolGenealogico.mostrarAscendenciaCompleta(id);
                break;
            }
            
            case 5: {
                int id;
                cout << "ID de la persona: ";
                cin >> id;
                arbolGenealogico.mostrarDescendenciaCompleta(id);
                break;
            }
            
            case 6: {
                int id;
                cout << "ID de la persona: ";
                cin >> id;
                
                vector<Persona*> ancestros = arbolGenealogico.obtenerAncestros(id);
                if (!ancestros.empty()) {
                    cout << "\nANCESTROS:\n";
                    for (int i = 0; i < ancestros.size(); i++) {
                        cout << i+1 << ". " << ancestros[i]->nombre 
                             << " (ID: " << ancestros[i]->id << ")\n";
                    }
                } else {
                    cout << "No se encontraron ancestros.\n";
                }
                break;
            }
            
            case 7: {
                int id;
                cout << "ID de la persona: ";
                cin >> id;
                
                vector<Persona*> descendientes = arbolGenealogico.obtenerDescendientes(id);
                if (!descendientes.empty()) {
                    cout << "\nDESCENDIENTES:\n";
                    for (int i = 0; i < descendientes.size(); i++) {
                        cout << i+1 << ". " << descendientes[i]->nombre 
                             << " (ID: " << descendientes[i]->id << ")\n";
                    }
                } else {
                    cout << "No se encontraron descendientes.\n";
                }
                break;
            }
            
            case 8: {
                int id1, id2;
                cout << "ID primera persona: ";
                cin >> id1;
                cout << "ID segunda persona: ";
                cin >> id2;
                
                string relacion = arbolGenealogico.determinarRelacion(id1, id2);
                cout << "\nRELACIÓN: " << relacion << endl;
                break;
            }
            
            case 9: {
                arbolGenealogico.mostrarEstadisticas();
                break;
            }
            
            case 0: {
                cout << "Saliendo...\n";
                break;
            }
            
            default: {
                cout << "Opción no válida.\n";
                break;
            }
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
            system("cls");
        }
        
    } while (opcion != 0);
    
    return 0;
}
