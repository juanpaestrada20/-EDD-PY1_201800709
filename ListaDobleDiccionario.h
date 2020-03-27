#ifndef LISTADOBLEDICCIONARIO_H
#define LISTADOBLEDICCIONARIO_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class NodoDoble{
private:
    string word;
    NodoDoble *next;
    NodoDoble *prev;
public:
    NodoDoble(string word){
        this->word = word;
        next = NULL;
        prev = NULL;
    }
    string getWord(){
        return word;
    }
    NodoDoble *getNext(){
        return next;
    }
    NodoDoble *getPrev(){
        return prev;
    }
    void setNext(NodoDoble *n){
        next = n;
    }
    void setPrev(NodoDoble *n){
        prev = n;
    }
};

class ListaDoble{
private:
    NodoDoble *first;
    NodoDoble *last;
    int size;
public:
    ListaDoble(){
        first = NULL;
        last = NULL;
        size = 0;
    }
    NodoDoble *getFirst(){
        return first;
    }
    NodoDoble *getLast(){
        return last;
    }
    bool isEmpty(){
        return (size==0);
    }
    void insertWord(string word){
        NodoDoble *nuevo = new NodoDoble(word);
        if(isEmpty()){
            first = nuevo;
            first->setNext(first);
            first->setPrev(first);
        }else if(first->getNext() == first){
            last = nuevo;
            first->setNext(last);
            first->setPrev(last);
            last->setNext(first);
            last->setPrev(first);
        }else{
            last->setNext(nuevo);
            nuevo->setPrev(last);
            nuevo->setNext(first);
            first->setPrev(nuevo);
            last = nuevo;
        }
        size++;
    }
    void createGraph(){
        if(!isEmpty()){
            int i, j;
            NodoDoble *temp = first;
            ofstream fs("dictionary.dot");
            fs << "digraph G{ " << endl;
            fs << "rankdir = LR;" << endl;
            fs << "node [margin=0 shape=egg fillcolor=green style=filled];" << endl;
            if(size == 1){
                fs << "1 [label=\""<< temp->getWord() <<"\"];" << endl;
                fs << "1 -> 1 [dir=both color=\"red:blue\"];" << endl;
            }else{
                for(i = 0; i < size; i++){
                    fs << i << "[label=\""<< temp->getWord() <<"\"];" << endl;
                    temp = temp->getNext();
                }
                fs << endl;
                temp = first;
                for(j = 0; j < size; j++){
                    fs << j << " -> " << j+1 << "[dir=both];" << endl;
                    temp = temp->getNext();
                }
                fs << "0 -> " << j << "[dir=both];"<< endl;
            }
            fs << "}" << endl;
            fs.close();

            system("dot -Tpng dictionary.dot -o dictionary.png");
            system("display dictionary.png");
        }
    }
};

#endif // LISTADOBLEDICCIONARIO_H
