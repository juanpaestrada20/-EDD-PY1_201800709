#ifndef MATRIZDISPERSA_H
#define MATRIZDISPERSA_H

#include <locale>
#include <string>
#include <fstream>
#include <ColaFichas.h>

class NodoMatrix{
private:
    Ficha *ficha;
    NodoMatrix *up;
    NodoMatrix *down;
    NodoMatrix *right;
    NodoMatrix *left;
    int multiplicador;
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
        multiplicador = 1;
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
    void setMultiplicador(int multiplicador){
        this->multiplicador = multiplicador;
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
    int getMultiplicador(){
        return multiplicador;
    }
};

class MatrizDispersa{
private:
    NodoMatrix *root;
    int max;
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
    void insertFicha(Ficha *ficha, int posX, int posY, int multiplicador){
        NodoMatrix *nuevo = new NodoMatrix(ficha, posX, posY);
        nuevo->setMultiplicador(multiplicador);
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
    void graphMatrix();
public:
    MatrizDispersa(int max){
        root = new NodoMatrix((new Ficha("Root", 0)),-1,-1);
        this->max = max;
    }
    //INSERTAR FICHA
    void colocarFicha(Ficha *ficha, int posX, int posY){
        if(posX <= max && posY <= max && posX > -1 && posY > -1)
            insertFicha(ficha, posX, posY, 1);
    }
    //INSERTAR MULTIPLICADORES
    void colocarMultiplicador(int posX, int posY, int multiplicador){
        if(posX <= max && posY <= max && posX > -1 && posY > -1)

            insertFicha(NULL, posX, posY, multiplicador);
    }
    //GRAFICAR MATRIZ
    void graficarMatriz(){
        graphMatrix();
    }
    NodoMatrix *buscarX(int x){
        return searchX(x);
    }
    NodoMatrix *buscarY(int y){
        return searchY(y);
    }
};

void MatrizDispersa::graphMatrix(){
    if(root->getDown() != NULL && root->getRight() != NULL){
        int x = 0;//CONTADOR PARA EJE X
        int y = 0;//CONTADOR PARA EJE Y
        NodoMatrix *auxRow = root->getDown(); //NODO AUXILIAR PARA LAS FILAS
        NodoMatrix *auxColumn = root->getRight(); //NODO AUXILIAR PARA LAS COLUMNAS
        ofstream fs("matrix.dot");
        fs << "digraph G{" << endl;
        fs << "node[shape=box width=1.5 style=filled];" << endl;
        //RAIZ
        fs << "M[label=\"(" << root->getX() << ", " << root->getY();
        fs << ")\" witdh=1.5, filled=crimson, group=1];" << endl;
        fs << endl;

        //CREACION DE LOS NODOS DE FILAS
        while(auxRow != NULL){
            fs << "F" << y << " [label=\"" << auxRow->getX() << ", " << auxRow->getY() << "\" fillcolor=burlywood1, group=1];" << endl;
            y++;
            auxRow = auxRow->getDown();
        }
        //CONECTAR NODOS DE FILAS
        y=0;
        auxRow = root->getDown();
        while(auxRow->getDown() != NULL){
            fs << "F" << y << " -> ";
            y++;
            auxRow = auxRow->getDown();
        }
        fs << "F" << y << "[dir=both];" << endl;
        //while(y > 0){
        //    fs << "F" << y << " -> ";
        //    y--;
        //}
        fs << "F0 ;" << endl;

        //CREACION DE NODOS DE COLUMNAS
        while(auxColumn != NULL){
            fs << "C" << x << " [label=\"" << auxColumn->getX() << ", " << auxColumn->getY() << "\", fillcolor=burlywood1, group=" << auxColumn->getX()+2 << "];" << endl;
            x++;
            auxColumn = auxColumn->getRight();
        }
        //CONECTAR NODOS DE COLUMNAS
        x = 0;
        auxColumn = root->getRight();
        while(auxColumn->getRight() != NULL){
            fs << "C" << x << " -> ";
            x++;
            auxColumn = auxColumn->getRight();
        }
        fs << "C" << x << "[dir=both];" << endl;
        /*while(x > 0){
            fs << "C" << x << " -> ";
            x--;
        }*/
        fs << "C0" << endl;

        //CONECTAR LAS COLUMNAS Y LAS FILAS A LA RAIZ
        fs << "M -> F0;" << endl;
        fs << "M -> C0;" << endl;

        fs << endl;

        //COLOCAR LAS COLUMAS EN LA MISMA LINEA
        x = 0;
        auxColumn = root->getRight();
        fs << "{rank = same; M;";
        while(auxColumn != NULL){
            fs << " C" << x << ";";
            x++;
            auxColumn = auxColumn->getRight();
        }
        fs << "}" << endl;

        fs << endl;

        //CREACION DE NODOS POR CADA FILA
        x = 0;
        y = 0;
        auxRow = root->getDown();
        string color = "";
        while(auxRow != NULL){
            auxColumn = auxRow->getRight();
            x = 0;
            while(auxColumn != NULL){
                //SI ES NODO CON FICHA
                if(auxColumn->getFicha() != NULL){
                    fs << "X" << auxColumn->getX() << "Y" << auxColumn->getY() << " [label=\""
                       << auxColumn->getFicha()->letra << "\", fontcolor=white fillcolor=lightcoral, group="
                       << auxColumn->getX()+2 << "];" << endl;
                }
                //SI ESTA VACIA PERO TIENE MULTIPLICADOR
                else{
                    if(auxColumn->getMultiplicador() == 2){
                        color = "mediumaquamarine";
                    }
                    else if(auxColumn->getMultiplicador() == 3){
                        color = "lightskyblue";
                    }
                    else{
                        color = "gold";
                    }
                    fs << "X" << auxColumn->getX() << "Y" << auxColumn->getY() << " [label=\"x" << auxColumn->getMultiplicador()
                       << "\", fontcolor=white fillcolor=" << color << ", group=" << auxColumn->getX()+2 << "];" << endl;
                }
                auxColumn = auxColumn->getRight();
                x++;
            }
            fs << endl;
            auxRow = auxRow->getDown();
            y++;
        }

        fs << endl;

        //CONECTAR LOS NODOS CREADOS
        x = 0;
        y = 0;
        auxRow = root->getDown();
        string alinearFila = "";
        while(auxRow != NULL){
            fs << "F" << y;
            alinearFila = "F" + to_string(y);
            auxColumn = auxRow->getRight();
            while(auxColumn != NULL){
                fs << " -> X" << auxColumn->getX() << "Y" << auxColumn->getY();
                alinearFila += "; X" + to_string(auxColumn->getX()) + "Y" + to_string(auxColumn->getY());
                auxColumn = auxColumn->getRight();
                x++;
            }
            fs << "[dir=both];" << endl;
            fs << "{rank=same; " << alinearFila << "}" << endl;
            fs << endl;
            y++;
            auxRow = auxRow->getDown();
            alinearFila.clear();
        }

        //CONECTAR LOS POR COLUMNAS
        x = 0;
        y = 0;
        auxColumn = root->getRight();
        while (auxColumn != NULL) {
            fs << "C" << x;
            auxRow = auxColumn->getDown();
            while(auxRow != NULL){
                fs << " -> X" << auxRow->getX() << "Y" << auxRow->getY();
                auxRow = auxRow->getDown();
            }
            fs << ";" << endl;
            auxColumn = auxColumn->getRight();
            x++;
        }
        //FIN DEL GRAFICO
        fs << "}" << endl;
        fs.close();

        system("dot -Tpng matrix.dot -o matrix.png");
        system("display matrix.png");

    }
}

#endif // MATRIZDISPERSA_H
