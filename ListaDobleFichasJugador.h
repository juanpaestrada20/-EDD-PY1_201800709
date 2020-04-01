#ifndef LISTADOBLEFICHASJUGADOR_H
#define LISTADOBLEFICHASJUGADOR_H

#include <string>
#include <iostream>
#include <fstream>

#include "ColaFichas.h"

class NodoDobleFichas{
private:
    Ficha *ficha;
    NodoDobleFichas *next;
    NodoDobleFichas *prev;
public:
    NodoDobleFichas(Ficha *ficha){
        this->ficha = ficha;
        next = NULL;
        prev = NULL;
    }
    Ficha *getFicha(){
        return ficha;
    }
    NodoDobleFichas *getNext(){
        return next;
    }
    NodoDobleFichas *getPrev(){
        return prev;
    }
    void setNext(NodoDobleFichas *next){
        this->next = next;
    }
    void setPrev(NodoDobleFichas *prev){
        this->prev = prev;
    }
};

class FichasJugador{
private:
    NodoDobleFichas *first;
    NodoDobleFichas *last;
    string name;
    int size;
    void insertFichas(Ficha *ficha){
        NodoDobleFichas *nuevo = new NodoDobleFichas(ficha);
        if(size == 0){
            first = nuevo;
            last = nuevo;
        }else{
            last->setNext(nuevo);
            nuevo->setPrev(last);
            last = nuevo;
        }
        size++;
    }
    Ficha *searchFicha(string letra){
        NodoDobleFichas *aux = first;
        bool found = false;
        while(aux != NULL){
            if(aux->getFicha()->letra == letra){
                found = true;
                break;
            }
            aux = aux->getNext();
        }
        if(found){
            return aux->getFicha();
        }else{
            return NULL;
        }
    }
    void eliminarPrimero(){
        if(first->getNext() == NULL){
            delete first;
            first = NULL;
            last = NULL;
        }else if(first != NULL){
            first = first->getNext();
            delete first->getPrev();
            first->setPrev(NULL);
        }
        size--;
    }
    void eliminarUltimo(){
        if(first->getNext() == NULL){
            delete first;
            first = NULL;
            last = NULL;
        }else if(last != NULL){
            last = last->getPrev();
            delete last->getNext();
            last->setNext(NULL);
        }
        size--;
    }
    void eliminarPos(int pos){
        if(pos < 0 || pos > size)
            return;
        else if(pos == 0)
            eliminarPrimero();
        else if(pos == size-1)
            eliminarUltimo();
        else if(first != NULL){
            NodoDobleFichas *aux = first;
            for(int i = 1; i <= pos; i++)
                aux = aux->getNext();
            aux->getPrev()->setNext(aux->getNext());
            aux->getNext()->setPrev(aux->getPrev());
            delete aux;
            size--;
        }
    }
    void deleteFicha(string letra){
        NodoDobleFichas *aux = first;
        int pos = 0;
        while (aux != NULL) {
            if(aux->getFicha()->letra == letra){
                eliminarPos(pos);
                break;
            }
            pos++;
            aux = aux->getNext();
        }
    }
    void graphic(){
        if(size > 0){
            int i = 0;
            int j = 0;
            NodoDobleFichas *temp = first;
            ofstream fs("playerHand.dot");
            fs << "digraph G{ " << endl;
            fs << "rankdir = LR;" << endl;
            fs << "label = \"Fichas de: " << name << "\";" << endl;
            fs << "node [margin=0 shape=box fillcolor=lightsalmon fontcolot=white style=filled];" << endl;
            fs << "B [label =\"null\"];"<< endl;
            fs << "C [label =\"null\"];"<< endl;
            while(i < size){
                fs << i << "[label=\" "<< temp->getFicha()->letra << ", " << temp->getFicha()->punteo <<  "pts\"];" << endl;
                i++;
                temp = temp->getNext();
            }
            fs << "B -> " << j << ";" << endl;
            fs << j << " -> "<< "B" << ";" << endl;
            temp = first;
            while(j<size-1){
                 fs << j << "->" << j+1 << ";" << endl;
                 fs << j+1 << "->" << j << ";" << endl;
                 j++;
                 temp = temp->getNext();
            }
            fs << j<< " -> "<< "C" << ";" << endl;
            fs << "C -> " << j << ";" << endl;
            fs << "}" << endl;
            fs.close();

            system("dot -Tpng playerHand.dot -o playerHand.png");
            system("display playerHand.png");
        }
    }
public:
    FichasJugador(){
        first = NULL;
        last = NULL;
        size = 0;
        name = "";
    }
    void setJugador(string name){
        this->name = name;
    }
    string getName(){
        return name;
    }
    //metodo para rellenar sus 7 fichas
    void getFichas(Ficha *ficha){
        insertFichas(ficha);
    }
    //para colocar ficha en el tablero
    Ficha *colocarFicha(string letra){
        return searchFicha(letra);
    }
    //eliminar
    void eliminarFicha(string letra){
        deleteFicha(letra);
    }
    void graphHand(){
        graphic();
    }
};


#endif // LISTADOBLEFICHASJUGADOR_H
