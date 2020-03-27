#include <QCoreApplication>
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
