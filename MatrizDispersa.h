#ifndef MATRIZDISPERSA_H
#define MATRIZDISPERSA_H

#include <locale>
#include <string>
#include <ColaFichas.h>

class NodoMatrix{
private:
    Ficha *ficha;
    NodoMatrix *up;
    NodoMatrix *down;
    NodoMatrix *right;
    NodoMatrix *left;
    int posX;
    int posY;
public:
    NodoMatrix(Ficha *ficha, int posX, int posY){
        this->ficha = ficha;
        up = NULL;
        down = NULL;
        right = NULL;
        left = NULL;
        this->posX = posX;
        this->posY = posY;
    }
    void setFicha(Ficha *ficha){
        this->ficha = ficha;
    }
    void setUp(NodoMatrix *up){
        this->up = up;
    }
    void setDown(NodoMatrix *down){
        this->down = down;
    }
    void setRight(NodoMatrix *right){
        this->right = right;
    }
    void setLeft(NodoMatrix *left){
        this->left = left;
    }
    void setX(int posX){
        this->posX = posX;
    }
    void setY(int posY){
        this->posY = posY;
    }
    Ficha *getFicha(){
        return ficha;
    }
    NodoMatrix *getUp(){
        return up;
    }
    NodoMatrix *getDown(){
        return down;
    }
    NodoMatrix *getRight(){
        return right;
    }
    NodoMatrix *getLeft(){
        return left;
    }
    int getX(){
        return posX;
    }
    int getY(){
        return posY;
    }
};

class MatrizDispersa{
private:
    NodoMatrix *root;
    //BUSCAR LA FILA
    NodoMatrix *searchY(int y){
        NodoMatrix *temp = root;
        while(temp != NULL){
            if(temp->getY() == y)
                return temp;
            temp = temp->getDown();
        }
        return NULL;
    }
    //BUSCAR COLUMNA
    NodoMatrix *searchX(int x){
        NodoMatrix *temp = root;
        while(temp != NULL){
            if(temp->getX() == x)
                return temp;
            temp = temp->getRight();
        }
        return NULL;
    }
    //INSETAR EN COLUMNA
    NodoMatrix *insertOrderColumn(NodoMatrix *nuevo, NodoMatrix *cabezaCol){
        NodoMatrix *temp = cabezaCol;
        bool flag = false;
        while(true){
            if(temp->getX() == nuevo->getX()){
                temp->setY(nuevo->getY());
                temp->setFicha(nuevo->getFicha());
                return temp;
            }
            else if (temp->getX() > nuevo->getX()){
                //HAY QUE INSERTAR ANTES DEL TEMP
                flag = true;
                break;
            }
            if (temp->getRight() != NULL) {
                temp = temp->getRight();
            }
            else{
                //INSERTAR DESPUES DE TEMP
                break;
            }
        }
        if(flag){
            nuevo->setRight(temp);
            temp->getLeft()->setRight(nuevo);
            nuevo->setLeft(temp->getLeft());
            temp->setLeft(nuevo);
        }
        else{
            temp->setRight(nuevo);
            nuevo->setLeft(temp);
        }
        return nuevo;
    }
    //INSERTAR EN FILA
    NodoMatrix *insertOrderRow(NodoMatrix *nuevo, NodoMatrix *cabezaFila){
        NodoMatrix *temp = cabezaFila;
        bool flag = false;
        while(true){
            if(temp->getY() == nuevo->getY()){
                temp->setX(nuevo->getX());
                temp->setFicha(nuevo->getFicha());
                return temp;
            }
            else if (temp->getY() > nuevo->getY()){
                //HAY QUE INSERTAR ANTES DEL TEMP
                flag = true;
                break;
            }
            if (temp->getDown() != NULL) {
                temp = temp->getDown();
            }
            else{
                //INSERTAR DESPUES DE TEMP
                break;
            }
        }
        if(flag){
            nuevo->setDown(temp);
            temp->getUp()->setDown(nuevo);
            nuevo->setUp(temp->getUp());
            temp->setUp(nuevo);
        }
        else{
            temp->setDown(nuevo);
            nuevo->setUp(temp);
        }
        return nuevo;
    }
    //CREAR COLUMNA
    NodoMatrix *createColumn(int posX){
        NodoMatrix *cabezaCol = root;
        NodoMatrix *columna = insertOrderColumn((new NodoMatrix((new Ficha("COL",0)), posX, -1)), cabezaCol);
        return columna;
    }
    //CREAR FILA
    NodoMatrix *createRow(int posY){
        NodoMatrix *cabezaFila = root;
        NodoMatrix *fila = insertOrderRow((new NodoMatrix((new Ficha("ROW",0)), -1, posY)), cabezaFila);
        return fila;
    }
    //INSERTAR FICHA
    void insertFicha(Ficha *ficha, int posX, int posY){
        NodoMatrix *nuevo = new NodoMatrix(ficha, posX, posY);
        NodoMatrix *columna = searchX(posX);
        NodoMatrix *fila = searchY(posY);
        //CASO 1: COLUMNA NO EXISTE Y FILA NO EXISTE
        if(columna == NULL && fila == NULL){
            cout << "Caso 1" << endl;
            //CREAR COLUMNA
            columna = createColumn(posX);
            //CRAR FILA
            fila = createRow(posY);
            //INSERTAR DE FORMA ORDENADA EN COLUMNA
            nuevo = insertOrderColumn(nuevo, fila);
            //INSERTAR DE FORMA ORDENADA EN FILA
            nuevo = insertOrderRow(nuevo, columna);
            return;
        }
        //CASO 2: COLUMNA NO EXISTE Y FILA SI EXISTE
        else if(columna == NULL && fila != NULL){
            cout << "Caso 2" << endl;
            //CREAR COLUMNA
            columna = createColumn(posX);
            //INSERTAR DE FORMA ORDENADA EN COLUMNA
            nuevo = insertOrderColumn(nuevo, fila);
            //INSERTAR DE FORMA ORDENADA EN FILA
            nuevo = insertOrderRow(nuevo, columna);
            return;
        }
        //CASO 3: COLUMNA SI EXISTE Y FILA NO EXISTE
        else if(columna != NULL && fila == NULL){
            cout << "Caso 3" << endl;
            //CRAR FILA
            fila = createRow(posY);
            //INSERTAR DE FORMA ORDENADA EN COLUMNA
            nuevo = insertOrderColumn(nuevo, fila);
            //INSERTAR DE FORMA ORDENADA EN FILA
            nuevo = insertOrderRow(nuevo, columna);
            return;
        }
        //CASO 4: COLUMNA SI EXISTE Y FILA SI EXISTE
        else if(columna != NULL && fila != NULL){
            cout << "Caso 4" << endl;
            //INSERTAR DE FORMA ORDENADA EN COLUMNA
            nuevo = insertOrderColumn(nuevo, fila);
            //INSERTAR DE FORMA ORDENADA EN FILA
            nuevo = insertOrderRow(nuevo, columna);
        }
    }

public:
    MatrizDispersa(){
        root = new NodoMatrix((new Ficha("Root", 0)),-1,-1);
    }
    //INSERTAR FICHA
    void colocarFicha(Ficha *ficha, int posX, int posY){
        insertFicha(ficha, posX, posY);
    }
};

#endif // MATRIZDISPERSA_H
