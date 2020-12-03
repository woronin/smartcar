#include <qapplication.h>
#include <qnamespace.h>
#include "vench.h"
#include "formzastav.h"
#include <unistd.h>
#include <qsplashscreen.h>
#include <qpixmap.h>
#include <stdio.h>



int main( int argc, char ** argv )
{
    
    qDebug("Version 20201119\n");
//    printf("start program\n");
    QApplication a( argc, argv );
//    Vench *w = new Vench(0,0,0,0x00000200);
//    Vench *w = new Vench(0,0,0);

//QPixmap  zt;
//zt.load("zst.jpg");
//    QSplashScreen *splash =
//            new QSplashScreen(zt);
//    splash->show();
//    splash->finish(w);
//    delete splash;
//
    int i;
    int flBlueTooth;// flag raboti s ustroistvom BT (==1) (esli raven 0 - to ustroistvo ZeegBee)
    int flKonsol;// rabota bez paneley chisto v konsole s ukazannim filom
    
    
    flKonsol=0;
    flBlueTooth=0;
//    if (argc>1) if ((argv[1][0] == '-')&&(argv[1][1] == 'b')) {
//	printf("Protokol BlueTooth\n");
//	flBlueTooth=1 ;
//    }
    
    

    char name_file[20]; // ima ustroistva
    for(i=0;i<20;i++) name_file[i]=0;

    char katalog[20]; // ima ustroistva
    for(i=0;i<20;i++) katalog[i]=0;

    char port_s[20]; // ima ustroistva
    for(i=0;i<20;i++) port_s[i]=0;
    
    int fl_port_s=0;

    int fl_kat=0;
// proveraem na vvedennie parametr    
    for (i=0;i<argc;i++){

	if (strcmp(argv[i],"-k")==0){
	    if (i<argc-1){
		strcpy(katalog,argv[i+1]);
		fl_kat =1;
		i++;
//		printf("%s\n",katalog);
	    }
	    else {
		printf("ERROR: katalog\n"); 
	    }
	    
	}
	

	if (strcmp(argv[i],"-d")==0){
	    if (i<argc-1){
		strcpy(port_s,argv[i+1]);
		fl_port_s =1;
		i++;
//		printf("%s\n",katalog);
	    }
	    else {
		printf("ERROR:port\n"); 
	    }
	}

	if (strcmp(argv[i],"-b")==0){
		printf("Protokol BlueTooth\n");
		flBlueTooth=1 ;
	}


	if (strcmp(argv[i],"-f")==0){
		flKonsol =1;
		printf("Consol mode\n");
	    if (i<argc-1){
		strcpy(name_file,argv[i+1]);
		fl_port_s =1;
		i++;
		printf("file : %s\n",name_file);
	    }
	    else {
		printf("ERROR:file\n"); 
	    }
	}

	if (strcmp(argv[i],"-F")==0){
		flKonsol =2;
		printf("Consol mode\n");
	    if (i<argc-1){
		strcpy(name_file,argv[i+1]);
		fl_port_s =1;
		i++;
		printf("file : %s\n",name_file);
	    }
	    else {
		printf("ERROR:file\n"); 
	    }
	}


    }






    if (!flKonsol){
//        FormZastav *Zastav;
//	Zastav = new FormZastav(0,katalog) ;
//        Zastav->exec();
//        delete Zastav;
    }
    
    Vench *w ;
    char * kt=0;
    char * prt=0;
    
    if (fl_port_s) prt = port_s; 
    if (fl_kat)   w= new Vench(0,katalog,prt,flBlueTooth,flKonsol,name_file);
    else	w= new Vench(0,kt,prt,flBlueTooth,flKonsol,name_file);
    
    
    if (!flKonsol) w->show();



    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
