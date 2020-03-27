#ifndef LISTAPUNTEOS_H
#define LISTAPUNTEOS_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class NodoLista{
public:
    int punteo;
    string jugador;
    NodoLista *siguiente;
    NodoLista(int punteo){
        this->punteo = punteo;
        this->siguiente = NULL;
    }
    NodoLista(int punteo, string jugador){
        this->punteo = punteo;
        this->siguiente = NULL;
        this->jugador = jugador;
    }
};

class ListaSimple{
private:
    NodoLista *cabeza;
    NodoLista *cola;
    int size;
public:
    ListaSimple(){
        cabeza = NULL;
        cola = NULL;
        size = 0;
    }
    int getSize(){
        return size;
    }
    void insertPoints(int puntos){
        if(cabeza==NULL){
            cabeza = new NodoLista(puntos);
            cola = cabeza;
        }else if(cola->punteo >= puntos){
            NodoLista *nuevo = new NodoLista(puntos);
            cola->siguiente = nuevo;
            cola = nuevo;
        }else if(cabeza->punteo <= puntos){
            NodoLista *nuevo = new NodoLista(puntos);
            NodoLista *aux = cabeza;
            cabeza = nuevo;
            nuevo->siguiente = aux;
        }else{
            NodoLista *actual = cabeza->siguiente;
            NodoLista *anterior = cabeza;
            while(actual != NULL){
                if(actual->punteo <= puntos && anterior->punteo >= puntos){
                    NodoLista *nuevo = new NodoLista(puntos);
                    anterior->siguiente = nuevo;
                    nuevo->siguiente = actual;
                    break;
                }
                anterior = anterior->siguiente;
                actual = actual->siguiente;
            }
        }
        size++;
    }
    int getBestPoints(){
        return cabeza->punteo;
    }
    void clearList(){
        while(size>0){
            NodoLista *aux = cabeza;
            cabeza= aux->siguiente;
            delete aux;
            size--;
        }
    }
    void insertBestPoints(int puntos, string jugador){
        if(cabeza==NULL){
            cabeza = new NodoLista(puntos, jugador);
            cola = cabeza;
        }else if(cola->punteo >= puntos){
            NodoLista *nuevo = new NodoLista(puntos, jugador);
            cola->siguiente = nuevo;
            cola = nuevo;
        }else if(cabeza->punteo <= puntos){
            NodoLista *nuevo = new NodoLista(puntos, jugador);
            NodoLista *aux = cabeza;
            cabeza = nuevo;
            nuevo->siguiente = aux;
        }else{
            NodoLista *actual = cabeza->siguiente;
            NodoLista *anterior = cabeza;
            while(actual != NULL){
                if(actual->punteo <= puntos && anterior->punteo >= puntos){
                    NodoLista *nuevo = new NodoLista(puntos, jugador);
                    anterior->siguiente = nuevo;
                    nuevo->siguiente = actual;
                    break;
                }
                anterior = anterior->siguiente;
                actual = actual->siguiente;
            }
        }
        size++;
    }
    void playerPointsGraph(string name){
        NodoLista *temp = cabeza;
        ofstream fs("playerPoints.dot");
        fs << "digraph G{ " << endl;
        fs << "rankdir = LR;" << endl;
        fs << "node [margin=0 shape=Mcircle fontcolor=white fillcolor=dodgerblue style=filled];" << endl;
        fs << "label = \"Puntaje de: " << name << "\";" << endl;
        while(temp != NULL){
            if(temp->siguiente != 0){
                fs << temp->punteo << "->";
            }else{
                fs << temp->punteo << endl;
            }
            temp = temp->siguiente;
        }
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng playerPoints.dot -o playerPoints.png");
        system("display playerPoints.png");
    }
    void bestPointsGraph(){
        int i = 0;
        int j = 0;
        NodoLista *temp = cabeza;
        ofstream fs("bestPoints.dot");
        fs << "digraph G{ " << endl;
        fs << "rankdir = LR;" << endl;
        fs << "node [margin=0 shape=box fontcolor=white fillcolor=dodgerblue style=filled];" << endl;
        while(i<size){
            fs << i << "[label=\"" << temp->jugador << ", " << temp->punteo << "pts\"];" << endl;
            temp = temp->siguiente;
            i++;
        }
        temp = cabeza;
        while(j<size-1){
            fs << j << "->" << j+1 << endl;
            j++;
        }
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng bestPoints.dot -o bestPoints.png");
        system("display bestPoints.png");
    }
};

#endif // LISTAPUNTEOS_H
