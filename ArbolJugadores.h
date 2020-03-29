
#ifndef ARBOLJUGADORES_H
#define ARBOLJUGADORES_H
#include <string>
#include <iostream>
#include <fstream>

#include "ListaPunteos.h"

using namespace std;

class Player{
private:
    string nombre;
    ListaSimple *puntos;
public:
    Player(string nombre){
        this->nombre = nombre;
        puntos = new ListaSimple();
    }
    Player(){};
    string getNombre(){
        return  nombre;
    }
    ListaSimple *getLista(){
        return puntos;
    }
    void newPoints(int points){
        puntos->insertPoints(points);
    }
    int getPoint(){
        return puntos->getBestPoints();
    }
    int getSize(){
        return puntos->getSize();
    }
};

class NodoArbol{
private:
    Player *jugador;
    NodoArbol *izquierda;
    NodoArbol *derecha;
public:
    NodoArbol(Player *jugador){
        this->jugador = jugador;
        izquierda = NULL;
        derecha = NULL;
    }
    void setIzquierda(NodoArbol *n){
        izquierda = n;
    }
    void setDerecha(NodoArbol *n){
        derecha = n;
    }
    string getNombre(){
        return jugador->getNombre();
    }
    Player *getPlayer(){
        return jugador;
    }
    NodoArbol *getIzquierda(){
        return izquierda;
    }
    NodoArbol *getDerecha(){
        return derecha;
    }
};

class ArbolJugadores{
private:
    NodoArbol *raiz;
    ListaSimple *bestPoints;
    NodoArbol *insertPlayer(Player *jugador, NodoArbol *n){
        if(n == NULL){
            n = new NodoArbol(jugador);
            return n;
        }else {
            if(n->getNombre() > jugador->getNombre()){
                NodoArbol *temp = this->insertPlayer(jugador, n->getIzquierda());
                n->setIzquierda(temp);
            }else if(n->getNombre() < jugador->getNombre()){
                NodoArbol *temp = this->insertPlayer(jugador,n->getDerecha());
                n->setDerecha(temp);
            }else{
                cout << "Jugador ya registrado" << endl;
                return 0;
            }
            return n;
        }
    }
    Player *search(NodoArbol *n, string name){
        if(n == NULL){
            return NULL;
        }else if(n->getNombre() == name){
            return n->getPlayer();
        }else if(n->getNombre() > name){
            return search(n->getIzquierda(), name);
        }else{
            return search(n->getDerecha(), name);
        }
    }
    string createTree(NodoArbol *n, string escritura){
        if(n != NULL){
            if(n->getIzquierda() != NULL){
                escritura += n->getNombre() + " -> " + n->getIzquierda()->getNombre() + "\n";
                escritura = createTree(n->getIzquierda(), escritura);
            }
            if(n->getDerecha() != NULL){
                escritura += n->getNombre() + " -> " + n->getDerecha()->getNombre() + "\n";
                escritura = createTree(n->getDerecha(), escritura);
            }
        }
        return escritura;
    }
    string createTreeInOrder(NodoArbol *n, string escritura){
        if(n != NULL){
            escritura = createTreeInOrder(n->getIzquierda(), escritura);
            escritura += n->getNombre() + "->";
            escritura = createTreeInOrder(n->getDerecha(), escritura);
        }
        return escritura;
    }
    string createTreePreOrder(NodoArbol *n, string escritura){
        if(n != NULL){
            escritura += n->getNombre() + "->";
            escritura = createTreePreOrder(n->getIzquierda(), escritura);
            escritura = createTreePreOrder(n->getDerecha(), escritura);
        }
        return escritura;
    }
    string createTreePostOrder(NodoArbol *n, string escritura){
        if(n != NULL){
            escritura = createTreePostOrder(n->getIzquierda(), escritura);
            escritura = createTreePostOrder(n->getDerecha(), escritura);
            escritura += n->getNombre() + "->";
        }
        return escritura;
    }
    void insertBestPoint(NodoArbol *n){
        if(n != NULL){
            insertBestPoint(n->getIzquierda());
            if(n->getPlayer()->getLista()->getSize() > 0){
                bestPoints->insertBestPoints(n->getPlayer()->getPoint(), n->getNombre());
            }
            insertBestPoint(n->getDerecha());
        }
    }
    void clearList(){
        if(bestPoints->getSize()>0){
            bestPoints->clearList();
        }

    }
public:
    ArbolJugadores(){
        raiz = NULL;
        bestPoints = new ListaSimple();
    }
    void insertNewPlayer(string dato){
        Player *jugador = new Player(dato);
        raiz = insertPlayer(jugador, raiz);
    }
    void recorrerInorder(NodoArbol *n){
        if(n != NULL){
            recorrerInorder(n->getIzquierda());
            cout << n->getNombre() << endl;
            recorrerInorder(n->getDerecha());
        }
    }
    void recorrerPreorder(NodoArbol *n){
        if(n != NULL){
            cout << n->getNombre() << endl;
            recorrerPreorder(n->getIzquierda());
            recorrerPreorder(n->getDerecha());
        }
    }
    void recorrerPostorder(NodoArbol *n){
        if(n != NULL){
            recorrerPostorder(n->getIzquierda());
            recorrerPostorder(n->getDerecha());
            cout << n->getNombre() << endl;
        }
    }
    Player *searchPlayer(string name){
        Player *result = search(raiz, name);
        return result;
    }
    void createGraphTree(){
        string escritura = "";
        NodoArbol *temp = raiz;
        ofstream fs("jugadoresArbol.dot");
        fs << "digraph G{ " << endl;
        fs << "rankdir = DU;" << endl;
        fs << "node [margin=0 shape=circle width=1 fontcolor=white fillcolor=red style=filled];" << endl;
        escritura = createTree(temp, escritura);
        fs << escritura << endl;
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng jugadoresArbol.dot -o jugadoresArbol.png");
        system("display jugadoresArbol.png");
    }
    void createGraphInOrder(){
        string escritura = "";
        NodoArbol *temp = raiz;
        ofstream fs("jugadoresInOrder.dot");
        fs << "digraph G{ " << endl;
        fs << "rankdir = LR;" << endl;
        fs << "node [margin=0 shape=cds height=1 fontcolor=white fillcolor=turquoise style=filled];" << endl;
        escritura = createTreeInOrder(temp, escritura);
        escritura = escritura.substr(0, escritura.size()-2);
        fs << escritura << endl;
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng jugadoresInOrder.dot -o jugadoresInOrder.png");
        system("display jugadoresInOrder.png");
    }
    void createGraphPreOrder(){
        string escritura = "";
        NodoArbol *temp = raiz;
        ofstream fs("jugadoresPreOrder.dot");
        fs << "digraph G{ " << endl;
        fs << "rankdir = LR;" << endl;
        fs << "node [margin=0 shape=septagon height=1 fontcolor=white fillcolor=forestgreen style=filled];" << endl;
        escritura = createTreePreOrder(temp, escritura);
        escritura = escritura.substr(0, escritura.size()-2);
        fs << escritura << endl;
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng jugadoresPreOrder.dot -o jugadoresPreOrder.png");
        system("display jugadoresPreOrder.png");
    }
    void createGraphPostOrder(){
        string escritura = "";
        NodoArbol *temp = raiz;
        ofstream fs("jugadoresPostOrder.dot");
        fs << "digraph G{ " << endl;
        fs << "rankdir = LR;" << endl;
        fs << "node [margin=0 shape=oval width=1 fontcolor=white fillcolor=gold style=filled];" << endl;
        escritura = createTreePostOrder(temp, escritura);
        escritura = escritura.substr(0, escritura.size()-2);
        fs << escritura << endl;
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng jugadoresPostOrder.dot -o jugadoresPostOrder.png");
        system("display jugadoresPostOrder.png");
    }
    ListaSimple *getBestPoints(){
        clearList();
        insertBestPoint(raiz);
        return bestPoints;
    }
};


#endif // ARBOLJUGADORES_H
