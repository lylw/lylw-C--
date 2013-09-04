#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])    
{    
    QApplication app(argc, argv);    
    IrregularWidget *widget = new IrregularWidget;    
    widget->show();    
    return app.exec();    
}    
