#include<QApplication>
#include <qnamespace.h>
#include "vench.h"
#include <unistd.h>
#include <stdio.h>
#include <QtCore>


#include "forms/mainwindow.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    a.setWindowIcon(QIcon(":/image/image/UMKI_logo.svg"));
    QString katalog;
    QString port_s;

    // parse argv
    int i = 1;
    while (i < argc)
    {
        QString arg1 = argv[i];
        ++i;

        if (arg1 == "-k")
            if (i < argc)
            {
                katalog = argv[i];
                ++i;
            } else
                printf("ERROR: katalog\n");

        if (arg1 == "-d")
            if (i < argc)
            {
                port_s = argv[i];
                ++i;
            } else
                printf("ERROR:port\n");
    }

    MainWindow *w = new MainWindow(katalog, port_s);
    w->show();

    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
