#include <qapplication.h>
#include <qnamespace.h>
#include "vench.h"
#include "formzastav.h"
#include <unistd.h>
#include <qsplashscreen.h>
#include <qpixmap.h>
#include <stdio.h>
#include <QtCore>


#include "forms/mainwindow.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    bool isBluetoothMode = false;
    int flKonsol = 0;

    QString name_file;
    QString katalog;
    QString port_s;

    // parse argv
    int i = 1;
    while (i < argc)
    {
        QString arg1 = argv[i];
        ++i;

        if (arg1 == "-b")
        {
            printf("BlueTooth Mode\n");
            isBluetoothMode = true;
        }

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

        if (arg1 == "-f" || arg1 == "-F")
        {
            if (arg1 == "-f")
                flKonsol = 1;
            else
                flKonsol = 2;

            printf("Consol mode\n");

            if (i < argc)
            {
                name_file = argv[i];

                printf("file : %s\n",qPrintable(name_file));
                ++i;
            } else
                printf("ERROR:file\n");
        }
    }

    //Vench *w = new Vench(0, katalog, port_s, isBluetoothMode, flKonsol, name_file);
    MainWindow* w = new MainWindow();
    if (!flKonsol) w->show();

    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
