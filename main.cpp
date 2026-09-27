/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Returns of Flashcards                                      #
#############################################################################
*
* Author information
* - Name: Manh Tran
* - Student number: 154123367
* - Gitlab user name: kkk243
* - Tuni email: manh.tran@tuni.fi
*
*/


#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
