#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <exception>
#include <time.h>
#include <unistd.h>

#include "ListaPunteos.h"
#include "ArbolJugadores.h"
#include "ColaFichas.h"
#include "ListaDobleDiccionario.h"
#include "ListaDobleFichasJugador.h"
#include "MatrizDispersa.h"

static MatrizDispersa *matrix;
static ArbolJugadores *jugadores;
static Cola *fichas;
static ListaDoble *diccionario;
static FichasJugador *fichasPlayer1;
static FichasJugador *fichasPlayer2;
static ListaSimple *punteosMejoresJugadores;
static bool configuraciones;
static Cola *TodasFichas;

static string player1;
static string player2;

void jsonExample(QString filename);
void menuOption();
void menuReportes();
void juego();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL,"Spanish");
    fichas = new Cola();
    TodasFichas = new Cola;
    diccionario = new ListaDoble();
    jugadores = new ArbolJugadores();
    fichasPlayer1 = new FichasJugador();
    fichasPlayer2 = new FichasJugador();
    punteosMejoresJugadores = new ListaSimple();

    fichas->crearFichas();
    TodasFichas->crearFichas();
    configuraciones = false;
    menuOption();

    return 0;
}


void jsonExample(QString filename){
    QFile file;
    QString val;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = d.object();
    //qDebug()<< "Dimension:";
    //qDebug()<< jsonObject.value(QStringLiteral("dimension")).toInt();

    matrix = new MatrizDispersa(jsonObject.value(QStringLiteral("dimension")).toInt());

    QJsonObject artista = jsonObject["casillas"].toObject();
    QJsonArray jsonArray3 = artista["dobles"].toArray();
    QJsonArray jsonArray4 = artista["triples"].toArray();

    foreach (const QJsonValue & value, jsonArray3) {
        QJsonObject obj = value.toObject();
        QJsonArray arrayEvento = obj["dobles"].toArray();
        //qDebug()<< "Coordenada doble x y ";
        //qDebug()<< obj.value(QStringLiteral("x")).toInt();
        //qDebug()<< obj.value(QStringLiteral("y")).toInt();
        int x = obj.value(QStringLiteral("x")).toInt();
        int y = obj.value(QStringLiteral("y")).toInt();
        matrix->colocarMultiplicador(x, y, 2);
    }
    foreach (const QJsonValue & value, jsonArray4) {
        QJsonObject obj = value.toObject();
        QJsonArray arrayEvento = obj["triples"].toArray();
        //qDebug()<< "Coordenada trple x y ";
        //qDebug()<< obj.value(QStringLiteral("x")).toInt();
        //qDebug()<< obj.value(QStringLiteral("y")).toInt();
        int x = obj.value(QStringLiteral("x")).toInt();
        int y = obj.value(QStringLiteral("y")).toInt();
        matrix->colocarMultiplicador(x, y, 3);
    }
    QJsonArray jsonArray5 = jsonObject["diccionario"].toArray();
    qDebug()<< "DICCIONARIO";
    foreach (const QJsonValue & value, jsonArray5) {
        QJsonObject obj = value.toObject();
        QJsonObject palabra = obj["palabra"].toObject();
        //qDebug()<< "Palabra de diccionario";
        //qDebug() << obj.value(QStringLiteral("palabra")).toString();
        QString word = obj.value(QStringLiteral("palabra")).toString();
        string nueva = word.toStdString();
        diccionario->insertWord(nueva);
    }
    configuraciones = true;
}

void menuOption(){
    int opcion;
    system("clear");
    do {
        /*
        OPCIONES PARA EL JUEGO:
        1. INICIAR JUEGO
        2. MENU DE REPORTES
        3. LEER CONFIGURACIONES DEL JUEGO
        4. SALIR
        */
        cout << "   _____    _____   _____               ____    ____    _        ______                   \n"
             << "  / ____|  / ____| |  __ \\      /\\     |  _ \\  |  _ \\  | |      |  ____|      _       _   \n"
             << " | (___   | |      | |__) |    /  \\    | |_) | | |_) | | |      | |__       _| |_   _| |_ \n"
             << "  \\___ \\  | |      |  _  /    / /\\ \\   |  _ <  |  _ <  | |      |  __|     |_   _| |_   _|\n"
             << "  ____) | | |____  | | \\ \\   / ____ \\  | |_) | | |_) | | |____  | |____      |_|     |_|  \n"
             << " |_____/   \\_____| |_|  \\_\\ /_/    \\_\\ |____/  |____/  |______| |______|                  \n" << endl;
        cout << "\n\n\n"
                "OPCIONES DEL JUEGO:\n"
                "1. INICIAR JUEGO\n"
                "2. MENU DE REPORTES\n"
                "3. ESTABLECER CONFIGURACIONES DEL JUEGO\n"
                "4. SALIR\n" << endl;
        cin >> opcion;
        switch (opcion) {
            case 1:
                if(!configuraciones){
                    system("clear");
                    cout << "No hay configuraciones cargadas" << endl;
                    sleep(3);
                    system("clear");
                }
                else {
                    system("clear");
                    cout << "INGRESE NOMBRE DEL JUGADOR 1" << endl;
                    cin >> player1;
                    cout << "INGRESE NOMBRE DEL JUGADOR 2" << endl;
                    cin >> player2;
                    jugadores->insertNewPlayer(player1);
                    jugadores->insertNewPlayer(player2);
                    fichasPlayer1->setJugador(player1);
                    fichasPlayer2->setJugador(player2);
                    for (int i = 0; i < 7; i++) {
                        fichasPlayer1->getFichas(fichas->desEnColar());
                        fichasPlayer2->getFichas(fichas->desEnColar());
                    }
                    system("clear");
                    juego();
                    system("clear");
                }
            break;
            case 2:
                system("clear");
                menuReportes();
            break;
        case 3:
            if(configuraciones){
                system("clear");
                cout << "Las configuraciones del juego ya han sido cargadas..." << endl;
                sleep(5);
                system("clear");
            }
            else{
                jsonExample("/home/juanpa/EDD_PY1_201800709/entrada.json");
                system("clear");
                cout << "Cargando configuraciones de juego, espere un momento..." << endl;
                sleep(3);
                system("clear");
            }
            break;
        default:
            system("clear");
            break;
        }
    } while (opcion != 4);
}

void menuReportes(){
    int reporte;
    string jugador;
    do{
        cout << " _____  ______ _____   ____  _____ _______ ______  _____ \n"
                "|  __ \\|  ____|  __ \\ / __ \\|  __ \\__   __|  ____|/ ____|\n"
                "| |__) | |__  | |__) | |  | | |__) | | |  | |__  | (___  \n"
                "|  _  /|  __| |  ___/| |  | |  _  /  | |  |  __|  \\___ \\ \n"
                "| | \\ \\| |____| |    | |__| | | \\ \\  | |  | |____ ____) |\n"
                "|_|  \\_\\______|_|     \\____/|_|  \\_\\ |_|  |______|_____/ \n" << endl;
        cout << "1. DICCIONARIO\n"
                "2. FICHAS DEL JUEGO (TODAS)\n"
                "3. ARBOL BINARIO JUGADORES\n"
                "4. ARBOL DE JUGADORES EN RECORRIDO PREORDER\n"
                "5. ARBOL DE JUGADORES EN RECORRIDO INORDER\n"
                "6. ARBOL DE JUGADORES EN RECORRIDO POSTORDER\n"
                "7. BUSCAR PUNTAJES DE JUGADOR\n"
                "8. MEJORES PUNTAJES DEL JUEGO\n"
                "9. ULTIMO TABLERO DE JUEGO\n"
                "10.FICHAS RESTANTES DEL ULTIMO JUEGO\n"
                "11.FICHAS DEL ULTIMOS JUGADORES\n"
                "12.SALIR" << endl;
        cin >> reporte;
        switch (reporte) {
        case 1:
            diccionario->createGraph();
            system("clear");
            break;
        case 2:
            TodasFichas->createGraph();
            system("clear");
            break;
        case 3:
            jugadores->createGraphTree();
            system("clear");
            break;
        case 4:
            jugadores->createGraphPreOrder();
            system("clear");
            break;
        case 5:
            jugadores->createGraphInOrder();
            system("clear");
            break;
        case 6:
            jugadores->createGraphPostOrder();
            system("clear");
            break;
        case 7:
            cout << "INGRESE EL NOMBRE DEL JUGADOR: ";
            cin >> jugador;
            jugadores->searchPlayer(jugador)->getLista();
            system("clear");
            break;
        case 8:
            jugadores->getBestPoints();
            system("clear");
            break;
        case 9:
            matrix->graficarMatriz();
            system("clear");
            break;
        case 10:
            fichas->createGraph();
            system("clear");
            break;
        case 11:
           fichasPlayer1->graphHand();
           fichasPlayer2->graphHand();
           system("clear");
            break;
        default:
            system("clear");
            break;
        }
    }while(reporte != 12);
}

void juego(){
    matrix->graficarMatriz();
    int opcionGlobal = 0;
    int option1;
    int option2;
    int cantidad;
    string letra;
    do{

        //TURNO DEL JUGADOR 1
        do{
            cout << "TURNO DEL JUGADOR 1:\n"
                    "1. VER FICHAS PROPIAS\n"
                    "2. COLOCAR FICHAS\n"
                    "3. INTERCAMBIAR FICHAS\n"
                    "4. VER DICCIONARIO\n"
                    "5. VER TABLERO\n"
                    "6. FINALIZAR JUEGO\n" << endl;
            cin >> option1;
            //SWITCH DEL PRIMER JUGADOR
            switch (option1) {
            case 1:
                system("clear");
                fichasPlayer1->graphHand();
                break;
            case 2:
                //PENDIENTE A COLOCAR FICHAS
                break;
            case 3:{
                system("clear");
                cout << "CUANTAS FICHAS DESEA INTERCAMBIAR" << endl;
                cin >> cantidad;
                if(cantidad >7)
                    break;
                else {
                    for(int i = 0; i < cantidad; i++){
                        fichasPlayer1->graphHand();
                        cout << "QUE FICHA CAMBIARA?" << endl;
                        cin >> letra;
                        Ficha *aux = fichasPlayer1->colocarFicha(letra);
                        if(aux != NULL){
                            fichasPlayer1->eliminarFicha(letra);
                            fichas->enColar(aux);
                            sleep(1);
                            Ficha *aux2 = fichas->desEnColar();
                            fichasPlayer1->getFichas(aux2);
                            sleep(1);
                        }else{
                            i--;
                            system("clear");
                        }
                    }
                    fichasPlayer1->graphHand();
                }
                system("clear");
                option1 = 7;
                break;

            }
            case 4:
                diccionario->createGraph();
                system("clear");
                break;
            case 5:
                matrix->graficarMatriz();
                system("clear");
                break;
            case 6:
                option1 = 7;
                opcionGlobal = 7;
                break;
            }
        }while(option1 != 7);
        if(option1 == 6){
            opcionGlobal = 7;
            break;
        }
        do{
            system("clear");
            cout << "TURNO DEL JUGADOR 2:\n"
                    "1. VER FICHAS PROPIAS\n"
                    "2. COLOCAR FICHAS\n"
                    "3. INTERCAMBIAR FICHAS\n"
                    "4. VER DICCIONARIO\n"
                    "5. VER TABLERO\n"
                    "6. FINALIZAR JUEGO\n" << endl;
            cin >> option2;
            //SWITCH DEL PRIMER JUGADOR
            switch (option2) {
            case 1:
                system("clear");
                fichasPlayer2->graphHand();
                break;
            case 2:
                //PENDIENTE A COLOCAR FICHAS
                break;
            case 3:{
                system("clear");
                cout << "CUANTAS FICHAS DESEA INTERCAMBIAR" << endl;
                cin >> cantidad;
                if(cantidad >7)
                    break;
                else {
                    for(int i = 0; i < cantidad; i++){
                        fichasPlayer2->graphHand();
                        cout << "QUE FICHA CAMBIARA?" << endl;
                        cin >> letra;
                        Ficha *aux = fichasPlayer2->colocarFicha(letra);
                        if(aux != NULL){
                            fichasPlayer2->eliminarFicha(letra);
                            fichas->enColar(aux);
                            sleep(1);
                            Ficha *aux2 = fichas->desEnColar();
                            fichasPlayer2->getFichas(aux2);
                            sleep(1);
                        }else{
                            i--;
                            system("clear");
                        }
                    }
                    fichasPlayer2->graphHand();
                }
                system("clear");
                option2 = 7;
                break;
            }
            case 4:
                diccionario->createGraph();
                system("clear");
                break;
            case 5:
                matrix->graficarMatriz();
                system("clear");
                break;

            case 6:
                option2 = 7;
                break;
            }
        }while(option2 != 7);
        if(option2 == 7){
            opcionGlobal = 7;
            break;
        }
    }while (opcionGlobal != 7);
}
