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


void jsonExample(QString filename);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL,"Spanish");
    fichas = new Cola();
    diccionario = new ListaDoble();
    jugadores = new ArbolJugadores();
    fichasPlayer1 = new FichasJugador();
    fichasPlayer2 = new FichasJugador();
    punteosMejoresJugadores = new ListaSimple();

    jsonExample("/home/juanpa/EDD_PY1_201800709/entrada.json");

    //diccionario->createGraph();
    matrix->colocarFicha((new Ficha("H", 1)),6,10);
    matrix->graficarMatriz();
    return a.exec();
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
    qDebug()<< "Dimension:";
    qDebug()<< jsonObject.value(QStringLiteral("dimension")).toInt();

    matrix = new MatrizDispersa(jsonObject.value(QStringLiteral("dimension")).toInt());

    QJsonObject artista = jsonObject["casillas"].toObject();
    QJsonArray jsonArray3 = artista["dobles"].toArray();
    QJsonArray jsonArray4 = artista["triples"].toArray();

    foreach (const QJsonValue & value, jsonArray3) {
        QJsonObject obj = value.toObject();
        QJsonArray arrayEvento = obj["dobles"].toArray();
        qDebug()<< "Coordenada doble x y ";
        qDebug()<< obj.value(QStringLiteral("x")).toInt();
        qDebug()<< obj.value(QStringLiteral("y")).toInt();
        int x = obj.value(QStringLiteral("x")).toInt();
        int y = obj.value(QStringLiteral("y")).toInt();
        matrix->colocarMultiplicador(x, y, 2);
    }
    foreach (const QJsonValue & value, jsonArray4) {
        QJsonObject obj = value.toObject();
        QJsonArray arrayEvento = obj["triples"].toArray();
        qDebug()<< "Coordenada trple x y ";
        qDebug()<< obj.value(QStringLiteral("x")).toInt();
        qDebug()<< obj.value(QStringLiteral("y")).toInt();
        int x = obj.value(QStringLiteral("x")).toInt();
        int y = obj.value(QStringLiteral("y")).toInt();
        matrix->colocarMultiplicador(x, y, 3);
    }
    QJsonArray jsonArray5 = jsonObject["diccionario"].toArray();
    qDebug()<< "DICCIONARIO";
    foreach (const QJsonValue & value, jsonArray5) {
        QJsonObject obj = value.toObject();
        QJsonObject palabra = obj["palabra"].toObject();
        qDebug()<< "Palabra de diccionario";
        qDebug() << obj.value(QStringLiteral("palabra")).toString();
        QString word = obj.value(QStringLiteral("palabra")).toString();
        string nueva = word.toStdString();
        diccionario->insertWord(nueva);
    }

}
