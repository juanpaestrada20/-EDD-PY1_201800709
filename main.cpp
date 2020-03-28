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

void jsonExample(QString filename);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL,"Spanish");

    jsonExample("/home/juanpa/[EDD]PY1_201800709/EDD_PY1_201800709/prueba.json");

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

    QJsonObject artista = jsonObject["casillas"].toObject();
    QJsonArray jsonArray3 = artista["dobles"].toArray();
    QJsonArray jsonArray4 = artista["triples"].toArray();

    foreach (const QJsonValue & value, jsonArray3) {
        QJsonObject obj = value.toObject();
        QJsonArray arrayEvento = obj["dobles"].toArray();
        //nombre.append(artista["Name"].toString());
        qDebug()<< "Coordenada doble x y ";
        qDebug()<< obj.value(QStringLiteral("x")).toInt();
        qDebug()<< obj.value(QStringLiteral("y")).toInt();
    }
    foreach (const QJsonValue & value, jsonArray4) {
        QJsonObject obj = value.toObject();
        QJsonArray arrayEvento = obj["triples"].toArray();
        //nombre.append(artista["Name"].toString());
        qDebug()<< "Coordenada trple x y ";
        qDebug()<< obj.value(QStringLiteral("x")).toInt();
        qDebug()<< obj.value(QStringLiteral("y")).toInt();
    }
    QJsonArray jsonArray5 = jsonObject["diccionario"].toArray();
    qDebug()<< "DICCIONARIO";
    foreach (const QJsonValue & value, jsonArray5) {
        QJsonObject obj = value.toObject();
        QJsonObject palabra = obj["palabra"].toObject();
        //nombre.append(artista["Name"].toString());
        qDebug()<< "Palabra de diccionario";
        qDebug() << obj.value(QStringLiteral("palabra")).toString();\
    }

}
