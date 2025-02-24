/** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#define TPeriodActive 5.0
#define TimeComandPr 1.0
#define TimeComandPov 1.0
#define TPeriodComand 1.0
#define TZaprosPanID 5.0
#define TZaprosANS 10.0

#define NAME_FILE_VAR "prog.xml"
#define NAME_FILE_INI "smart.ini"
#define NUM_STROK 500


#define NAME_SCRIPT_FILE "script.txt"
#define NAME_GRPH_FILE "graphik_tm.png"
#define NAME_GRPH_FILE_EN "graphik_en.png"
#define NAME_GRPH_FILE_OS "graphik_os.png"
#define NAME_GRPH_FILE_DM "graphik_dm.png"
#define NAME_GRPH_FILE_BAR "graphik_bar.png"
#define NAME_GRPH_FILE_VLG "graphik_vlg.png"

#define NAME_FILE_CUM "1.kum"
#define KOl_IMP 15

#define NAME_FILE_VAR_NEW "script.xml"
#define  DEF_DISKR_IMPR 12
#define  DEF_DISKR_IMPL 12



//period oprosa vseh datchikov v sec
#define PERIOD_OPROSA_DAT 10.0

//for rs
#define FOR_IGOR 1

//#define PRINT_LOG

//period shirokoveshat oprosa
#define PER_SHIR_ZAP 10
#define PER_OPROS_FILE 1
#define DEF_PERIOD_OPROSA 5
//vrema ogidania otveta ot porta v sec 
#define TIME_WAIT_ANS_PORT 5

#define PERIOD_OPROSA_TAR_FL 5
#define POROG_TIME_SN 0.5
#define TAR_KF 2.0

// vrema bordstvovania po umolchaniu
#define TM_UMOL 0.5


#define LOGFILE "LOG.txt"
#define LOGFILE_LINUX "/$HOME/.config/smartcar/LOG.txt"

#define DATAFILE "DATA_LOG.txt"
#define DATAFILE_LINUX "/$HOME/.config/smartcar/DATA_LOG.txt"

#define COMFILE "COM_F.txt"

#define ACTFILE "ACTIVE_LOG.txt"
#define ACTFILE_LINUX "/$HOME/.config/smartcar/ACTIVE_LOG.txt"

#define TARFILE "T_KOEFF.txt"
#define TARFILE_LINUX "/$HOME/.config/smartcar/T_KOEFF.txt"

//#define QWT_PLOT_LIB // woronin eto dla sborki v ubuntu nado zakommentrit


#include <qnamespace.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <QMessageBox>


//end for rs


#include "define_json.h"
#include "define_snap.h"

#include <QtGui>
#include <QtCore>
#include <QtXml>

#ifdef QSERIAL
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include <QStringList>

#include <vench.h>

#include "ui_vench.h"

#include <string.h>
#include <unistd.h>
#include <qdir.h>
#include <qmessagebox.h>
//#include <qdom.h>

#include <sys/time.h>

//#include <QDomNode>
//#include <QDomDocument>
//#include <QStandartItemModel>

//Vench::Vench(QWidget* parent, const char* name, bool modal, WFlags fl ):QDialog( parent, name, modal, fl )
//Vench::Vench(QWidget* parent, const char* name, bool modal ):QDialog( parent )




MyThread::MyThread(void(*call_f)(void*),void *arg):QThread()
{
    func = call_f;
    param = arg;
}

void MyThread::run()
{
    (*func)(param);
}

int Vench::mSleep(int zd){
    QEventLoop l;
    QTimer::singleShot(zd,&l,SLOT(quit()));
    l.exec();
    
//    usleep(zd*10000);
    return zd;
}

void Vench::DatKas(int num){//0 - right 1 - left
    QPalette pl(Qt::red,Qt::red);
    pl.setColor(QPalette::Base,Qt::red);
//	qDebug("datkas dt = %d\n",num);

    if (num==0) {
//m        TLabLeftDat->setPalette(pl);

//        TLabLeftDat->setTextBackgroundColor( QColor(Qt::red,Qt::red,Qt::red) );
       // QColor cl(255,0,0);
       // TLabLeftDat->setTextBackgroundColor(&cl );

 //m       if (ui->TLabLeftDat_n)	ui->TLabLeftDat_n->setPalette(pl);
        fl_datKas=1;
    }

    //mif (num==1) {TLabRightDat->setPalette(pl); if (ui->TLabRightDat_n) ui->TLabRightDat_n->setPalette(pl); fl_datKas=1;}


}


//Vench::Vench(QWidget* parent):QDialog( parent,Qt::WindowMinMaxButtonsHint|Qt::WindowSystemMenuHint )

Vench::Vench(QWidget* parent, QString kat, QString prt, bool isBluetoothMode, int fKon, QString nf):QDialog( parent,Qt::Window|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint),
    ui(new Ui::Vench)
{
    fl_bluetooth = isBluetoothMode;

    fl_altlin=0;

#ifdef WIN_D
    QTextCodec::setCodecForTr(QTextCodec::codecForName ("Windows-1251"));
#endif
  ui->setupUi(this);

  ui->stackedWidget->hide();
  mSleep(100);

    QFont ft("MS Shell Dlg 2",8,50,false);
    ft.setBold(false);
    ui->BCar_2->setFont(ft);
    ui->BCar_1->setFont(ft);

    setAcceptDrops( false );
    setSizeGripEnabled( false );
    setModal( false );

    timer = new QTimer(this);

    tabWidget2 = new QTabWidget( this );
    tabWidget2->hide();



    tab = new QWidget( tabWidget2 );
    tab_3 = new QWidget( tabWidget2 );
    tab_5 = new QWidget( tabWidget2 );
    tab_6 = new QWidget( tabWidget2 );
    tab_6_n = new QWidget( ui->tabWidget_2 );
    
#ifdef    QWT_PLOT_LIB
    Mgrph = new QwtPlot(tab_6);
    Mgrph_n = new QwtPlot(tab_6_n); 
    
    //setCentralWidget(Mgrph);
//    Mgrph->setGeometry( QRect( 25, 10, 550, 300 ) );//vremenno
//    Mgrph_n->setGeometry( QRect( 25, 10, 550, 300 ) );//vremenno

    // Insert new curves
    cRight = new QwtPlotCurve("Data ");
    cRight->attach(Mgrph);

    cRight_n = new QwtPlotCurve("Data ");
    cRight_n->attach(Mgrph_n); 

    // Set curve styles
    cRight->setPen(QPen(Qt::red));
    // Attach (don't copy) data. Both curves use the same x array.
    cRight->setRawData(d_x, d_y, 1);

    cRight_n->setPen(QPen(Qt::red));
    // Attach (don't copy) data. Both curves use the same x array.
    cRight_n->setRawData(d_x, d_y, 1);


    // Axis 
    Mgrph->setAxisTitle(QwtPlot::xBottom, trUtf8("Время, сек"));
    Mgrph_n->setAxisTitle(QwtPlot::xBottom, trUtf8("Время, сек"));
//    Mgrph->setAxisScale(QwtPlot::xBottom, 0, 100);

    Mgrph->setAxisTitle(QwtPlot::yLeft, "Values");
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, "Values");
//    Mgrph->setAxisScale(QwtPlot::yLeft, -50, 90);
    
    QwtPainter::setDeviceClipping(false);
         Mgrph->canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
	 Mgrph->canvas()->setLineWidth(1);
         Mgrph_n->canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
	 Mgrph_n->canvas()->setLineWidth(1); 
#endif // QWT_PLOT_LIB  - woronin












    QPalette pl(Qt::lightGray,Qt::lightGray);
    pl.setColor(QPalette::Base,Qt::lightGray);

    TextLabel2_3 = new QLabel( trUtf8("Команда для устройства") ,tab);
    TextLabel2_3->setGeometry( QRect( 40, 530, 200, 20 ) );

    CommText = new QLineEdit(  "",tab );
    CommText->setEnabled(   true );
    CommText->setGeometry( QRect( 40, 550, 400, 22 ) );

    BRuchnComm = new QPushButton( trUtf8("Отправить") ,tab);
    BRuchnComm->setGeometry( QRect( 40, 580, 90, 30 ) );
//end ������ ����



    TLabAns_labFirst = new QLabel( trUtf8("Статус устройства") ,tab);
    TLabAns_labFirst->setGeometry( QRect(40, 55, 300, 20 ) );
    
    ans1 = trUtf8("Канал обмена не инициализирован") ;
    ans2 = trUtf8("Канал обмена открыт успешно") ;
    ans3 = trUtf8("Устройство ответило") ;
    fl_ans3_ust=0;
    TLabAnsFirst = new QLineEdit( ans1,tab);
    TLabAnsFirst->setGeometry( QRect(40, 75, 300, 20 ) );
    pl_ans_ust = new QPalette(Qt::red,Qt::red);
    pl.setColor(QPalette::Base,Qt::red);
    TLabAnsFirst->setPalette(pl);
    TLabAnsFirst->setReadOnly(  true );

    if (ui->TLabAnsFirst_n) {
        	ui->TLabAnsFirst_n->setPalette(pl);
        	ui->TLabAnsFirst_n->setText(ans1);
    }

    TextLabel2 = new QLabel( "",tab );
    TextLabel2->setGeometry( QRect( 40, 10, 200, 20 ) );
    TextLabel2->setVisible(false);

    SpActDatch = new QComboBox(tab);
    SpActDatch->setGeometry( QRect( 40, 30, 250, 22 ) );
    SpActDatch->setVisible(false);
    TextLabel2_2 = new QLabel( "" ,tab);
    TextLabel2_2->setGeometry( QRect( 40, 10, 200, 20 ) );

    checkKumir = new QCheckBox(  "",tab );
    checkKumir->setGeometry( QRect( 460, 520, 180, 22 ) );
    checkKumir->setChecked(  true );

    checkLog = new QCheckBox( "",tab );
    checkLog->setGeometry( QRect( 460, 540, 160, 22 ) );
    checkLog->setChecked( false );

    checkCicl = new QCheckBox("",tab );
    checkCicl->setGeometry( QRect( 460, 560, 160, 22 ) );
    checkCicl->setChecked( false );

    checkNeSpat = new QCheckBox(trUtf8("Не спать"),tab );
    checkNeSpat->setGeometry( QRect( 355, 200,160, 22 ) );
    checkNeSpat->setChecked( false );

    Obnovit = new QPushButton( "Obnovit",tab );
    Obnovit->setGeometry( QRect( 50, 480, 82, 26 ) );


    BDiscrImp_Ever = new QPushButton( "BDiscrImp_Ever",tab );
    BDiscrImp_Ever->setGeometry( QRect( 150, 480, 115, 40 ) );

    IzmPANID = new QPushButton( "IzmPANID",tab );
    IzmPANID->setGeometry( QRect( 450, 460, 82, 26 ) );

    IzmPANID_KONTR = new QPushButton( "IzmPANID_KONTR",tab );
    IzmPANID_KONTR->setGeometry( QRect( 290, 480, 115, 40 ) );

    ZapPANID = new QPushButton( "ZapPANID",tab );
    ZapPANID->setGeometry( QRect( 450, 494, 82, 26 ) );

    ServerDir = new QLineEdit(  "ServerDir",tab );
    ServerDir->setEnabled(  true );
    ServerDir->setGeometry( QRect( 40, 30, 300, 22 ) );
    ServerDir->setReadOnly(  true );


    BDiscrImp = new QPushButton( trUtf8("Сменить") ,tab);
    BDiscrImp->setGeometry( QRect( 460, 160, 90, 30 ) );
    
    DiscrImpE = new QLineEdit(  "10",tab );
    DiscrImpE->setEnabled(  true );
    DiscrImpE->setGeometry( QRect( 400, 165, 50, 22 ) );

    TextLabel6 = new QLabel( trUtf8("Имп"),tab );
    TextLabel6->setGeometry( QRect( 355, 165, 30, 20 ) );

    TextLabelSpeed = new QLabel( trUtf8("Скорость"),tab );
    TextLabelSpeed->setGeometry( QRect( 355, 235, 70, 20 ) );

    ImpS = new QLineEdit(  "10",tab );
    ImpS->setEnabled(  true );
    ImpS->setGeometry( QRect( 435, 235, 50, 22 ) );

    BSpeed = new QPushButton( trUtf8("Сменить") ,tab);
    BSpeed->setGeometry( QRect( 490, 230, 90, 30 ) );


    BUp = new QPushButton(  "BUp" ,tab);
    BUp->setGeometry( QRect( 180, 110, 70, 60 ) );

    BDown = new QPushButton(  "BDown",tab );
    BDown->setGeometry( QRect( 180, 260, 70, 60 ) );

    BRight = new QPushButton(  "BRigh" ,tab);
    BRight->setGeometry( QRect( 260, 180, 70, 80 ) );

    BLeft = new QPushButton( "BLeft" ,tab);
    BLeft->setGeometry( QRect( 110, 180, 70, 80 ) );

    BShift = new QPushButton( "BShift" ,tab);
    BShift->setGeometry( QRect( 10, 300, 90, 29 ) );

    BShift->setCheckable(  true );
    BShift->setVisible( false );

    BStop = new QPushButton(  "BStop" ,tab);
    BStop->setGeometry( QRect( 360, 300, 60, 29 ) );

    BLight = new QPushButton( "BLight" ,tab);
    BLight->setGeometry( QRect( 20, 150, 50, 29 ) );

    BBip = new QPushButton( "BBip" ,tab);
    BBip->setGeometry( QRect( 20, 110, 50, 29 ) );

    BAntiSleep = new QPushButton( "BAntiSleep" ,tab);
    BAntiSleep->setGeometry( QRect( 430, 70, 104, 29 ) );

    checkAll = new QCheckBox( "" ,tab);
    checkAll->setGeometry( QRect( 355, 110, 140, 22 ) );
    checkAll->setChecked( false );

    checkDiskr = new QCheckBox( "" ,tab);
    checkDiskr->setGeometry( QRect( 355, 140, 180, 22 ) );
    checkDiskr->setChecked(  true );


    BRec = new QPushButton( trUtf8("Запись") ,tab);
    BRec->setGeometry( QRect( 20, 615, 95, 29 ) );
    BRec->setCheckable(  true );


    BPokazProgr = new QPushButton( trUtf8("Программа") ,tab);
    BPokazProgr->setGeometry( QRect( 120, 615, 95, 29 ) );

    BPlay = new QPushButton( trUtf8("Поехали") ,tab);
    BPlay->setGeometry( QRect( 230, 615, 95, 29 ) );
    BPlay->setCheckable(  true );

    BProdol = new QPushButton( trUtf8("Продолжить") ,tab);
    BProdol->setGeometry( QRect( 335, 615, 95, 29 ) );


    BLog = new QPushButton( trUtf8("LOG-файл") ,tab);
    BLog->setGeometry( QRect( 460, 615, 95, 29 ) );

    BParam = new QPushButton( trUtf8("Параметры") ,tab);
    BParam->setGeometry( QRect( 460, 585, 95, 29 ) );


    BDim = new QPushButton( tr("�������������") ,tab_3);
    BDim->setCheckable(  true );
    BDim->setGeometry( QRect( 320, 60, 190, 46 ) );
    BGrphD = new QPushButton( tr("������ �������������") ,tab_3);
    BGrphD->setGeometry( QRect( 320, 110, 190, 26 ) );
    BGrphD->setCheckable(  true );

    LDim = new QLineEdit("0",tab_3);
    LDim->setGeometry( QRect( 520, 60, 55, 46 ) );
    LbDim = new QLabel("?",tab_3);
    LbDim->setGeometry( QRect( 580, 60, 10, 46 ) );

    BGrphO = new QPushButton( tr("������ ������������") ,tab_3);
    BGrphO->setGeometry( QRect( 20, 140, 190, 26 ) );
    BGrphO->setCheckable(  true );


    BBar = new QPushButton( tr("��������,\n���������") ,tab_3);
    BBar->setCheckable(  true );
    BBar->setGeometry( QRect( 320, 180, 190, 46 ) );

    LBar = new QLineEdit("0",tab_3);
    LBar->setGeometry( QRect( 520, 180, 55, 46 ) );
    LbBar = new QLabel(tr("��"),tab_3);
    LbBar->setGeometry( QRect( 577, 180, 20, 46 ) );

    LVl = new QLineEdit("0",tab_3);
    LVl->setGeometry( QRect( 520, 230, 55, 46 ) );
    LbVl = new QLabel(tr("%"),tab_3);
    LbVl->setGeometry( QRect( 580, 230, 20, 46 ) );

    BEnerg = new QPushButton( trUtf8("Энергия") ,tab_3);
    BEnerg->setCheckable(  true );
    BEnerg->setGeometry( QRect( 20, 180, 190, 46 ) );

    LEn = new QLineEdit("0",tab_3);
    LEn->setGeometry( QRect( 220, 180, 55, 46 ) );
    LbEn = new QLabel(tr("V"),tab_3);
    LbEn->setGeometry( QRect( 280, 180, 20, 46 ) );

    BTemp = new QPushButton( tr("�����������,\n ������������") ,tab_3);
    BTemp->setCheckable(  true );
    BTemp->setGeometry( QRect( 20, 60, 190, 46 ) );


    LTemp = new QLineEdit("0",tab_3);
    LTemp->setGeometry( QRect( 220, 60, 55, 46 ) );
    LbTemp = new QLabel(tr("��"),tab_3);
    LbTemp->setGeometry( QRect( 280, 60, 20, 46 ) );

    LOsv = new QLineEdit("0",tab_3);
    LOsv->setGeometry( QRect( 220, 120, 55, 46 ) );
    LbOsv = new QLabel(tr("��"),tab_3);
    LbOsv->setGeometry( QRect( 280, 120, 20, 46 ) );


    BGrph = new QPushButton( trUtf8("График температуры") ,tab_3);
    BGrph->setCheckable(  true );
    BGrph->setGeometry( QRect( 20, 110, 190, 26 ) );

    BGrphE = new QPushButton( trUtf8("График энергии") ,tab_3);
    BGrphE->setCheckable(  true );
    BGrphE->setGeometry( QRect( 20, 230, 190, 26 ) );

    BGrphB = new QPushButton( trUtf8("График давления") ,tab_3);
    BGrphB->setGeometry( QRect( 320, 230, 190, 26 ) );
    BGrphB->setCheckable(  true );

    BGrphVl = new QPushButton( tr("������ ���������") ,tab_3);
    BGrphVl->setGeometry( QRect( 320, 260, 190, 26 ) );
    BGrphVl->setCheckable(  true );



    BRezult = new QPushButton( trUtf8("\xd0\xa0езультаты ") ,tab_3);
    BRezult->setGeometry( QRect( 20, 310, 190, 26 ) );

    checkVibrUst = new QCheckBox( "" ,tab);
    checkVibrUst->setGeometry( QRect( 50, 450, 260, 22 ) );
    checkVibrUst->setChecked( false );


    listV = new QListWidget( tab);
    listV->setGeometry( QRect( 45, 360, 400, 90 ) );
    listV->clearPropertyFlags();
    listV->setSelectionMode(QAbstractItemView::MultiSelection);

    list_PANID = new QListWidget( tab);
    list_PANID->setGeometry( QRect( 450, 360, 100, 90 ) );
    list_PANID->clearPropertyFlags();
    list_PANID->setSelectionMode(QAbstractItemView::MultiSelection);


    list_vib = new QListWidget( tab);
    list_vib->setGeometry( QRect( 20, 360, 20, 90 ) );
    list_vib->clearPropertyFlags();
    list_vib->setSelectionMode(QAbstractItemView::MultiSelection);




    TextLabel4 = new QLabel(trUtf8("Список активных устройств"),tab );
    TextLabel4->setGeometry( QRect( 25, 335, 200, 20 ) );

    TextLabel5 = new QLabel(trUtf8("Группа"),tab );
    TextLabel5->setGeometry( QRect( 455, 335, 200, 20 ) );


    TLabReg = new QLabel( trUtf8("\xd0\xa0ежим:\n") ,tab_3 );
    TLabReg->setGeometry( QRect(30, 10, 480, 30 ) );

    TLabEthKgd = new QLabel( trUtf8("Кодограмма для eth:") ,tab_5 );
    TLabEthKgd->setGeometry( QRect(30, 10, 480, 30 ) );

    TLEEthKgd = new QLineEdit( "",tab_5 );
    TLEEthKgd->setGeometry( QRect(30, 40, 300, 20 ) );


    BSendEth = new QPushButton( trUtf8("Отправить") ,tab_5);
    //    BGrph->setCheckable(  true );
    BSendEth->setGeometry( QRect( 30, 70, 190, 26 ) );
    //    BShift->setToggleButton(  true );


   TLabEthKgdAT = new QLabel( trUtf8("AT*") ,tab_5 );
   TLabEthKgdAT->setGeometry( QRect(20, 145, 45, 30 ) );

    TLEEthKgdAT = new QLineEdit( "",tab_5 );
    TLEEthKgdAT->setGeometry( QRect(50, 150, 100, 20 ) );


    TLabEthKgdAT_CH = new QLabel( trUtf8("=") ,tab_5 );
    TLabEthKgdAT_CH->setGeometry( QRect(160, 145, 45, 30 ) );

    TLEEthKgdAT_CH = new QLineEdit( "",tab_5 );
    TLEEthKgdAT_CH->setGeometry( QRect(200, 150, 50, 20 ) );

    TLabEthKgdAT_CH2 = new QLabel( trUtf8(",") ,tab_5 );
    TLabEthKgdAT_CH2->setGeometry( QRect(260, 145, 45, 30 ) );

    TLEEthKgdAT_CH2 = new QLineEdit( "",tab_5 );
    TLEEthKgdAT_CH2->setGeometry( QRect(290, 150, 100, 20 ) );

    BSendEthAT = new QPushButton( trUtf8("Отправить") ,tab_5);
    //    BGrph->setCheckable(  true );
    BSendEthAT->setGeometry( QRect( 30, 180, 190, 26 ) );
    //    BShift->setToggleButton(  true );

//    tabWidget2->addTab( tab, QString::fromLatin1(trUtf8( "qqqq")) );
    tabWidget2->addTab( tab, (trUtf8( "Управление")) );
//    tabWidget2->addTab( tab_4, (trUtf8( "Устройства")) );
    tabWidget2->addTab( tab_3, (trUtf8( " Телеметрия ")) );
//    tabWidget2->addTab( tab_2, (trUtf8( "технологические режимы")) );
    ind_tab = tabWidget2->addTab( tab_6, (trUtf8( "График")) );
    ind_tab_n = ui->tabWidget_2->addTab( tab_6_n, (trUtf8( "График")) );
#ifdef WIN_D
    tab_5->hide();
#endif
#ifdef LINUX_D
    tabWidget2->addTab( tab_5, (trUtf8( "Вертолет")) );
#endif
    tabWidget2->removeTab(ind_tab);
    ui->tabWidget_2->removeTab(ind_tab_n);

    languageChange();
//    resize( QSize(600, 370).expandedTo(minimumSizeHint()) );
//    clearWState( WState_Polished );

    // signals and slots connections

    
    connect (timer,SIGNAL(timeout()),this,SLOT(ProcessTimer()));

//    connect( tabWidget2,SIGNAL(resizeEvent()),this,SLOT(resizeGeometry()));

    connect( BSendEth , SIGNAL( clicked() ), this, SLOT(BPressSendEth() ) );
    connect( BSendEthAT , SIGNAL( clicked() ), this, SLOT(BPressSendEthAT() ) );
    connect( BRezult, SIGNAL( clicked() ), this, SLOT(on_pBRezult_clicked() ) );
//    connect( BRezultE, SIGNAL( clicked() ), this, SLOT(on_pBRezultE_clicked() ) );
    connect( Obnovit, SIGNAL( clicked() ), this, SLOT( ObSpActUst() ) );
    connect( BDiscrImp_Ever, SIGNAL( clicked() ), this, SLOT( BPressBDiscrImp_Ever() ) );

    connect( IzmPANID, SIGNAL( clicked() ), this, SLOT( BPressIzmPANID() ) );
    connect( IzmPANID_KONTR, SIGNAL( clicked() ), this, SLOT( BPressIzmPANID_KONTR() ) );
    connect( ZapPANID, SIGNAL( clicked() ), this, SLOT( BPressZapPANID() ) );

    connect( BLeft, SIGNAL( pressed() ), this, SLOT( BPressLeft() ) );
    connect( BLeft, SIGNAL( released() ), this, SLOT( BReleaseLeft() ) );
    connect( BRight, SIGNAL( released() ), this, SLOT( BReleaseRight() ) );
    connect( BDown, SIGNAL( released() ), this, SLOT( BReleaseRv() ) );
    connect( BUp, SIGNAL( released() ), this, SLOT( BReleaseFw() ) );
    connect( BRight, SIGNAL( pressed() ), this, SLOT( BPressRight() ) );
    connect( BUp, SIGNAL( pressed() ), this, SLOT( BPressFw() ) );
    connect( BDown, SIGNAL( pressed() ), this, SLOT( BPressRv() ) );
    connect( BShift, SIGNAL( clicked() ), this, SLOT( BShiftPress() ) );
    connect( BDim, SIGNAL( clicked() ), this, SLOT( BPressDim() ) );
    connect( BEnerg, SIGNAL( clicked() ), this, SLOT( BPressEnerg() ) );
    connect( BTemp, SIGNAL( clicked() ), this, SLOT( BPressTemp() ) );
    connect( BBar, SIGNAL( clicked() ), this, SLOT( BPressBar() ) );
    connect( BLog, SIGNAL( clicked() ), this, SLOT( BPressLog() ) );
    connect( BParam, SIGNAL( clicked() ), this, SLOT( BPressParam() ) );
    connect( BGrph, SIGNAL( clicked() ), this, SLOT( BPressGrph() ) );
    connect( BGrphE, SIGNAL( clicked() ), this, SLOT( BPressGrphE() ) );
    connect( BGrphO, SIGNAL( clicked() ), this, SLOT( BPressGrphO() ) );
    connect( BGrphD, SIGNAL( clicked() ), this, SLOT( BPressGrphD() ) );
    connect( BGrphB, SIGNAL( clicked() ), this, SLOT( BPressGrphB() ) );
    connect( BGrphVl, SIGNAL( clicked() ), this, SLOT( BPressGrphV() ) );
    connect( BRuchnComm, SIGNAL( clicked() ), this, SLOT( BPressRuchnComm() ) );
    connect( BDiscrImp, SIGNAL( clicked() ), this, SLOT( BPressDiscrImp() ) );
    connect( BSpeed, SIGNAL( clicked() ), this, SLOT( BPressSpeed() ) );
    connect( BBip, SIGNAL( pressed() ), this, SLOT( BPressBip() ) );
    connect( BBip, SIGNAL( released() ), this, SLOT( BReleaseBip() ) );
    connect( BLight, SIGNAL( pressed() ), this, SLOT( BPressLight() ) );
    connect( BLight, SIGNAL( released() ), this, SLOT( BReleaseLight() ) );

//    connect( BRec, SIGNAL( clicked() ), this, SLOT( BPressRec() ) );//20150107
    connect( BRec, SIGNAL( pressed() ), this, SLOT( BPressRec() ) );
    connect( BRec, SIGNAL( released() ), this, SLOT( BRealeseRec() ) );


    connect( BPokazProgr, SIGNAL( clicked() ), this, SLOT( on_pBProgr_clicked() ) );
    
    connect( BPlay, SIGNAL( pressed() ), this, SLOT( BPressPlay() ) );
    connect( BPlay, SIGNAL( released() ), this, SLOT( BRealesePlay() ) );
    
    connect( BProdol, SIGNAL( clicked() ), this, SLOT( BPressProdol() ) );
    connect( BStop, SIGNAL( clicked() ), this, SLOT( BPressStop() ) );
    connect( BStop, SIGNAL( released() ), this, SLOT( BRealeseStop() ) );
    connect( checkLog, SIGNAL( stateChanged(int) ), this, SLOT( chLog() ) );
//    connect( BAntiSleep, SIGNAL( clicked() ), this, SLOT( AntiSleep() ) );
    connect( BAntiSleep, SIGNAL( pressed() ), this, SLOT( AntiSleep() ) );
    connect( checkKumir, SIGNAL( stateChanged(int) ), this, SLOT( chKum() ) );

//    connect( listV, SIGNAL( clicked() ), this, SLOT( SpVibr() ) );
    connect( listV, SIGNAL( itemSelectionChanged() ), this, SLOT( SpVibr() ) );

    connect( checkVibrUst, SIGNAL( clicked() ), this, SLOT( ChVibr() ) );



    connect( tabWidget2, SIGNAL( currentChanged(int)), this, SLOT( ActTab4(int) ) );





    setFocusPolicy(Qt::ClickFocus);
//w    setFocusPolicy(Qt::TabFocus);




    setFocus();

     VvodPD = new FormView(this) ;





    timer->start(200);

    fl_kat=0;
    fl_port_s=0;

    if (!kat.isEmpty()){
        SetKatalog(kat);
    //	qDebug("EEEEEEEEE\n");
    }
    if (!prt.isEmpty()){
        SetPort(prt);
    //	qDebug("EEEEEEEEE\n");
    }
    
    init();



    //skrivay knopki v regime BASE po prosbe Igora 

    ui->pBGraph_O->hide();

    ui->pBGraph_E->hide();
    ui->pBTemp_E->hide();
//    ui->pBRezult->hide();
    ui->pBTemp_D->hide();
    ui->pBGraph_D->hide();

    if (ui->TLabLeftDat_n) ui->TLabLeftDat_n->setPalette(pl);
    if (ui->TLabRightDat_n) ui->TLabLeftDat_n->setPalette(pl);


    connect( ui->pBRezult, SIGNAL( clicked() ), this, SLOT( BPressParam() ) );

    // end skrivay knopki v regime BASE po prosbe Igora 

//20201119
    ui->BProgram_n_B->hide();

//20201119
    ui->BProgram_n_B->hide();

//for new panel

    connect( ui->BObnovit_n, SIGNAL( clicked() ), this, SLOT( ObSpActUst() ) );
    connect( ui->BDiscrImp_Ever_n, SIGNAL( clicked() ), this, SLOT( BPressBDiscrImp_Ever() ) );
    connect( ui->IzmPanId_n, SIGNAL( clicked() ), this, SLOT( BPressIzmPANID() ) );
    connect( ui->IzmPanId_KONTR_n, SIGNAL( clicked() ), this, SLOT( BPressIzmPANID_KONTR() ) );
    connect( ui->Zap_PanId_n, SIGNAL( clicked() ), this, SLOT( BPressZapPANID() ) );
    connect(  ui->BRuchnComm_n, SIGNAL( clicked() ), this, SLOT( BPressRuchnComm_n() ) );
    connect( ui->BRezult_n, SIGNAL( clicked() ), this, SLOT(on_pBRezult_clicked() ) );

    connect( ui->BLeft_n, SIGNAL( pressed() ), this, SLOT( BPressLeft() ) );
    connect( ui->BLeft_n, SIGNAL( released() ), this, SLOT( BReleaseLeft() ) );
    connect( ui->BRight_n, SIGNAL( released() ), this, SLOT( BReleaseRight() ) );
    connect( ui->BDown_n, SIGNAL( released() ), this, SLOT( BReleaseRv() ) );
    connect( ui->BUp_n, SIGNAL( released() ), this, SLOT( BReleaseFw() ) );
    connect( ui->BRight_n, SIGNAL( pressed() ), this, SLOT( BPressRight() ) );
    connect( ui->BUp_n, SIGNAL( pressed() ), this, SLOT( BPressFw() ) );
    connect( ui->BDown_n, SIGNAL( pressed() ), this, SLOT( BPressRv() ) );
    connect( ui->BStop_n, SIGNAL( clicked() ), this, SLOT( BPressStop() ) );
    connect( ui->BStop_n, SIGNAL( released() ), this, SLOT( BRealeseStop() ) );

    connect(  ui->BBip_n, SIGNAL( pressed() ), this, SLOT( BPressBip() ) );
    connect(  ui->BBip_n, SIGNAL( released() ), this, SLOT( BReleaseBip() ) );
    connect(  ui->BLight_n, SIGNAL( pressed() ), this, SLOT( BPressLight() ) );
    connect(  ui->BLight_n, SIGNAL( released() ), this, SLOT( BReleaseLight() ) );

    connect( ui->BDim_n, SIGNAL( clicked() ), this, SLOT( BPressDim() ) );
    connect( ui->BEnerg_n, SIGNAL( clicked() ), this, SLOT( BPressEnerg() ) );
    connect( ui->BTemp_n, SIGNAL( clicked() ), this, SLOT( BPressTemp() ) );
    connect( ui->BBar_n, SIGNAL( clicked() ), this, SLOT( BPressBar() ) );
    connect( ui->BLog_n, SIGNAL( clicked() ), this, SLOT( BPressLog() ) );
    connect( ui->BParam_n, SIGNAL( clicked() ), this, SLOT( BPressParam() ) );
    connect( ui->BGrph_n, SIGNAL( clicked() ), this, SLOT( BPressGrph() ) );
    connect( ui->BGrphE_n, SIGNAL( clicked() ), this, SLOT( BPressGrphE() ) );
    connect( ui->BGrphO_n, SIGNAL( clicked() ), this, SLOT( BPressGrphO() ) );
    connect( ui->BGrphD_n, SIGNAL( clicked() ), this, SLOT( BPressGrphD() ) );
    connect( ui->BGrphB_n, SIGNAL( clicked() ), this, SLOT( BPressGrphB() ) );
    connect( ui->BGrphVl_n, SIGNAL( clicked() ), this, SLOT( BPressGrphV() ) );

    connect( ui->BRec_n, SIGNAL( pressed() ), this, SLOT( BRec_pressed_n() ) );
//    connect( ui->BRec_n, SIGNAL( released() ), this, SLOT( BRec_released_n() ) );

    connect( ui->BAntiSleep_n, SIGNAL( clicked() ), this, SLOT( AntiSleep() ) );

    connect( ui->BPokazProgr_n, SIGNAL( clicked() ), this, SLOT( on_pBProgr_clicked() ) );

    connect( ui->BPlay_n, SIGNAL( pressed() ), this, SLOT( BPressPlay_n() ) );
//    connect( ui->BPlay_n, SIGNAL( clicked() ), this, SLOT( BPressPlay_n() ) );
//    connect( ui->BPlay_n, SIGNAL( released() ), this, SLOT( BRealesePlay_n() ) );


    connect( ui->BProdol_n, SIGNAL( clicked() ), this, SLOT( BPressProdol() ) );

    connect( ui->listV_n, SIGNAL( itemSelectionChanged() ), this, SLOT( SpVibr_n() ) );

    connect( ui->rB_all_n, SIGNAL( clicked() ), this, SLOT( rBAll() ) );
    
    connect( ui->rB_vibr_sp_n, SIGNAL( clicked() ), this, SLOT( rBAll() ) );
    connect( ui->rBKumir_n, SIGNAL( clicked() ), this, SLOT( RBKumir() ) );
    connect( ui->rBXML_n, SIGNAL( clicked() ), this, SLOT( RBKumir() ) );
    connect( ui->checkDiskr_n, SIGNAL( clicked() ), this, SLOT( chDiskr_n() ) );

    connect( ui->checkLog_n, SIGNAL( stateChanged(int) ), this, SLOT( chLog_n() ) );


    connect( ui->BSpeed_n, SIGNAL( clicked() ), this, SLOT( BPressSpeed_n() ) );

    connect( ui->checkCicl_n, SIGNAL( clicked() ), this, SLOT( BChCikl() ) );

    connect( ui->BDopParam, SIGNAL( clicked() ), this, SLOT( BPressedDopParam() ) );
//    connect( ui->BDopParam, SIGNAL( released() ), this, SLOT( BPReleasedDopParam() ) );

//end for new panel

    
	
    flConsole =0;
    flConsole = fKon;
    name_file_console = nf;



//20201119  vstavkalogotip

    QPixmap zs;
    zs.load(":image/image/umki_logo_s.png");


    ui->label_21->setPixmap(zs);

    zs.load(":image/image/umki_logo_m.png");
//SUPER
    ui->label_36->setText("");
    ui->label_36->setPixmap(zs);
//BT
    ui->label_logotip_b->setText("");
    ui->label_logotip_b->setPixmap(zs);

    //Start
    zs.load(":image/image/umki_logo_m.png");
    ui->label_logo_start->setText("");
    ui->label_logo_start->setPixmap(zs);

    QPalette plm(Qt::white,Qt::white);
    plm.setColor(QPalette::Base,Qt::white);
    this->setPalette(plm);



//20210426 proveraem altlinux ili net
	bool fla =  QFile::exists("/etc/altlinux-release");
//	bool fla =  QFile::exists("smartcar1");
	if ( fla== true)	{ 
	    fl_altlin=1;
	    qDebug(" OS - AltLinux!!!\n");
	}
	else {
	    fl_altlin=0;
	    qDebug(" OS - NO AltLinux!!!\n");

	}
    }

//void Vench::qDebug_d(const char* format,...){
//    #ifdef PRINT_DEBUG
//    
//	va_list  va;
// 	char String[256];
//	va_start(va,format);   
//	vsprintf(String,format,va);
//	va_end(va);
//	qDebug("%s",String);    
//    #endif
//
//}

void Vench::BChCikl(){
    if (ui->checkCicl_n->isChecked() ==  true) checkCicl->setChecked( true);
    if (ui->checkCicl_n->isChecked() == false) checkCicl->setChecked(false);
}
void Vench::BPressedDopParam(){
    if (ui->BDopParam->isChecked()){
        ui->BDopParam->setText(trUtf8("Показать дополнительные параметры \\/ "));
        ui->GDopParam_n->hide();
        QRect rt;

        rt=ui->tabWidget_2->rect();

        rectW = rt.height();

        rt.setHeight(rt.height()-rectW/2.1);
        ui->tabWidget_2->resize(rt.width(),rt.height());


        rt=this->rect();
        rectH = rt.height();

        rt.setHeight(rt.height()-rectH/2.1);
        this->resize(rt.width(),rt.height());
	
    }else{
        ui->BDopParam->setText(trUtf8("Скрыть дополнительные параметры /\\ "));
        ui->GDopParam_n->show();
        QRect rt;
        rt=this->rect();
    //    rt.setHeight(rt.height()+200);
        rt.setHeight(rectH);
        this->resize(rt.width(),rt.height());

        rt=ui->tabWidget_2->rect();
        rt.setHeight(rectW);
        ui->tabWidget_2->resize(rt.width(),rt.height());

    }

}

void Vench::BPReleasedDopParam(){
}

void Vench::chDiskr_n(){
    if  ( ui->checkDiskr_n->isChecked() ){
    checkDiskr->setChecked( true);
    }else{
    checkDiskr->setChecked(false);
    }
}

void Vench::SbrosDatKas(){
    if (fl_datKas==0) return;
//    qDebug("sbros dat kas\n");
    QPalette pl(Qt::lightGray,Qt::lightGray);
    pl.setColor(QPalette::Base,Qt::lightGray);

    //mif (TLabLeftDat) TLabLeftDat->setPalette(pl);
   //m if (TLabRightDat) TLabRightDat->setPalette(pl);

    if (ui->TLabLeftDat_n) ui->TLabLeftDat_n->setPalette(pl);
    if (ui->TLabRightDat_n) ui->TLabRightDat_n->setPalette(pl);
    fl_datKas=0;
}


int Vench::COMClose(){
//	qDebug()<<"ComClose \n";
    if (flcomport){

#ifdef QSERIAL
    
	com_port_qts.close();
#endif
#ifndef QSERIAL
#ifdef LINUX_D
    //    com_port = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK,0)) <0)
	::close(com_port);
	qDebug()<<"Close  COM-port\n";
#endif
#ifdef WIN_D
      CloseHandle(hCOM);
#endif

#endif

	
    }
    fl_ans3_ust=1;
    flcomport = 0;
    return 1;
}

Vench::~Vench(){
    COMClose();
    destroy();
}

int Vench::ClearMasGrph(){
    for (int i = 0; i< PLOT_SIZE; i++)
    {
        d_x[i] = 0.5 * i;     // time axis
//        d_x[i] =  period_oprosa_b*i;     // time axis
        d_y[i] = 0;
        d_yt[i] = 0;
        d_ye[i] = 0;
        d_yo[i] = 0;
        d_yd[i] = 0;
        d_yb[i] = 0;
        d_yv[i] = 0;
        d_yp[i] = 0;
    }
    return 1;
}


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Vench::languageChange()
{

//    setCaption( trUtf8( "\xd0\xbf\xd1\x83\xd0\xbb\xd1\x8c\xd1\x82\x20\xd0\xa3\xd0\x9c\xd0\x9a\xd0\x98" ) );
    //setWindowTitle( tr( "1" ) );
    setWindowTitle( trUtf8( "\xd0\xbf\xd1\x83\xd0\xbb\xd1\x8c\xd1\x82\x20\xd0\xa3\xd0\x9c\xd0\x9a\xd0\x98" ) );
    TextLabel2->setText( trUtf8( "\xd0\xa1\xd0\xbf\xd0\xb8\xd1\x81\xd0\xbe\xd0\xba\x20\xd0\xb0\xd0\xba\xd1\x82\xd0\xb8"
    "\xd0\xb2\xd0\xbd\xd1\x8b\xd1\x85\x20\xd0\xb4\xd0\xb0\xd1\x82\xd1\x87\xd0\xb8\xd0"
    "\xba\xd0\xbe\xd0\xb2" ) );
    TextLabel2_2->setText( trUtf8( "\xd0\xa0\xd0\xb0\xd0\xb1\xd0\xbe\xd1\x87\xd0\xb8\xd0\xb9\x20\xd0\xba\xd0\xb0\xd1\x82"
    "\xd0\xb0\xd0\xbb\xd0\xbe\xd0\xb3\x20\xd1\x81\xd0\xb5\xd1\x80\xd0\xb2\xd0\xb5\xd1"
    "\x80\xd0\xb0" ) );
    checkKumir->setText( trUtf8( "По программе Кумир" ) );
    checkLog->setText( trUtf8( "печатать LOG-файл" ) );
    checkCicl->setText( trUtf8( "Цикл" ) );
    BDiscrImp_Ever->setText( trUtf8( "Сменить кол-во\n импульсов" ) );
    IzmPANID->setText( trUtf8( "Сменить" ) );
    IzmPANID_KONTR->setText( trUtf8( "Сменить группу\n контроллера" ) );
    ZapPANID->setText( trUtf8( "Запросить" ) );
    Obnovit->setText( trUtf8( "\xd0\x9e\xd0\xb1\xd0\xbd\xd0\xbe\xd0\xb2\xd0\xb8\xd1\x82\xd1\x8c" ) );
//    ServerDir->setText( tr( "/home/user/server/23_02_12" ) );
    ServerDir->setText( tr( "" ) );
    BUp->setText( trUtf8( "\xd0\x92\xd0\xbf\xd0\xb5\xd1\x80\xd0\xb5\xd0\xb4" ) );
    BDown->setText( trUtf8( "\xd0\x9d\xd0\xb0\xd0\xb7\xd0\xb0\xd0\xb4" ) );
    BRight->setText( trUtf8( "\xd0\x92\xd0\xbf\xd1\x80\xd0\xb0\xd0\xb2\xd0\xbe" ) );
    BLeft->setText( trUtf8( "\xd0\x92\xd0\xbb\xd0\xb5\xd0\xb2\xd0\xbe" ) );
    BShift->setText( trUtf8( "\xd1\x83\xd1\x81\xd0\xba\xd0\xbe\xd1\x80\xd0\xb5\xd0\xbd\xd0\xb8\xd0\xb5" ) );
    BStop->setText( trUtf8( "\xd0\xa1\xd1\x82\xd0\xbe\xd0\xbf" ) );
    BLight->setText( trUtf8( "\xd0\xa1\xd0\xb2\xd0\xb5\xd1\x82" ) );
    BBip->setText( trUtf8( "\xd0\x91\xd0\xb8\xd0\xbf" ) );
    BAntiSleep->setText( trUtf8( "\xd0\xa0\xd0\xb0\xd0\xb7\xd0\xb1\xd1\x83\xd0\xb4\xd0\xb8\xd1\x82\xd1\x8c" ) );
    checkAll->setText( trUtf8( "\xd0\x92\xd1\x81\xd0\xb5\x20\xd1\x83\xd1\x81\xd1\x82\xd1\x80\xd0\xbe\xd0\xb9\xd1\x81"
    "\xd1\x82\xd0\xb2\xd0\xb0" ) );
    BRec->setText( trUtf8( "\xd0\x97\xd0\xb0\xd0\xbf\xd0\xb8\xd1\x81\xd1\x8c" ) );
    BPlay->setText( trUtf8( "\xd0\x9f\xd0\xbe\xd0\xb5\xd1\x85\xd0\xb0\xd0\xbb\xd0\xb8" ) );
    checkVibrUst->setText(trUtf8("Использовать выбранные уст-ва"));
    checkDiskr->setText(trUtf8("Дискретное движение"));
}


double Vench::GetTime(){
//    time_t ltime;
//    struct timeval tv;
//    gettimeofday(&tv,NULL);
//    return tv.tv_sec+tv.tv_usec/1000000.0;
    QDateTime now = QDateTime::currentDateTime();
    return ( now.toMSecsSinceEpoch()/1000.0);
     
}


void Vench::resizeEvent(QResizeEvent * event)
{

    event->ignore();//20201121 ?????? antiwarning
    QRect rt = this->rect();

//    QRect rt = tabWidget2->rect();

//    tabWidget2->setGeometry (rt);
    
    rt.setLeft(rt.left()+15);
    rt.setRight(rt.right()+10);
    rt.setWidth(rt.width()-30);
    rt.setHeight(rt.height()-70);
    

 /*woronin   Mgrph->setGeometry( rt );//vremenno
    Mgrph_n->setGeometry( rt );//vremenno */
    

}

void Vench::ActTab4(int index)
{
    if(index==3){
        listV->setFocus();
    }
}

void Vench::ChVibr()
{
//    listV->repaint();
}

void Vench::SpVibr()
{
    int i,k;
    QListWidgetItem *itm;
//        qDebug()<<"SpVibr";
        
    k = listV->count();
    if (k>0){
        checkVibrUst->setChecked(  true );
    }
    int fl_izmm=0;
    fl_vibr_ust=0;
    for(i=0;i<k;i++){
        itm = listV->item(i);
        if (itm->isSelected()== true){
            if (act_ust[i].fl_antisleep!=2) continue;
            if (mas_vibr_ust[i] ==0) fl_izmm=1;
            mas_vibr_ust[i] = 1;
    	    act_ust[i].vibr_sp = 1;
            fl_vibr_ust =1;
        }else{
	    if(fl_clsp==1){
            fl_clsp=0;
            continue;
	    }
            if (mas_vibr_ust[i] ==1) fl_izmm=1;
            mas_vibr_ust[i] = 0;
            act_ust[i].vibr_sp = 0;
        }
    }
    if (fl_izmm==1){
        fl_obn_spact =4;
    }
}

void Vench::SpVibr_n()
{
    int i,k;
    QListWidgetItem *itm;
//        qDebug()<<"SpVibr";
        
    k = ui->listV_n->count();
    if (k>0){
        checkVibrUst->setChecked(  true );
    }
    int fl_izmm=0;
    fl_vibr_ust=0;
    for(i=0;i<k;i++){
        itm = ui->listV_n->item(i);
        if (itm->isSelected()== true){
            if (act_ust[i].fl_antisleep!=2) continue;
            if (mas_vibr_ust[i] ==0) fl_izmm=1;
            mas_vibr_ust[i] = 1;
    	    act_ust[i].vibr_sp = 1;
            fl_vibr_ust =1;
        }else{
	    if(fl_clsp==1){
            fl_clsp=0;
            continue;
	    }
            if (mas_vibr_ust[i] ==1) fl_izmm=1;
            mas_vibr_ust[i] = 0;
            act_ust[i].vibr_sp = 0;
        }
    }
    if (fl_izmm==1){
        fl_obn_spact =4;
    }
}



void Vench::ClearSpBeg()
{
    if(fl_begin==1){
        if (ui->BCar_1) ui->BCar_1->setEnabled(false);
        if (ui->BCar_2) ui->BCar_2->setEnabled(false);
    }
}

void Vench::ProcessTimer()
{
    int i,k;
    QString st,stt;

    if (flConsole){
    
//	qDebug()<<"constructor";
	if (flConsole){// esli ustanovlen flag raboti v konsole
	    //ServerDir->setText(name_file_console);
	qDebug()<<ServerDir->text();
	    on_BProgram_n_B_clicked();
	    //this->reject();
	    //~Vench();
	    disconnect (timer,SIGNAL(timeout()),this,SLOT(ProcessTimer()));
//    timer->stop();
    	    COMClose();
//	this->finished(0);
		this->done(0);
	//    QApplication::exit();

	}

	qDebug()<<"program done";
    
    }
    

//28_01_14 - ubrano potomu chto zavisaet v cicle
//    if(fl_hid){
//        if (fl_fl_hid!=2){
//            listV->setRowHidden(i_hid, true);
//            ui->listV->setRowHidden(i_hid, true);
//            list_PANID->setRowHidden(i_hid, true);
//            list_vib->setRowHidden(i_hid, true);
//            ui->listVibr->setRowHidden(i_hid, true);
//            ///for new regim prostoy
//                             if (i_hid==0){
//                                 if (ui->BCar_1){
//                                    ui->BCar_1->setEnabled(false);
//                                   }
//                             }
//                             if (i_hid==1){
//                                 if(ui->BCar_2){
//                                    ui->BCar_2->setEnabled(false);
//                                 }
//                             }
//            ///end for new regim prostoy
//
//        }
//        else{
//            listV->setRowHidden(i_hid,false);
//            ui->listV->setRowHidden(i_hid,false);
//            list_PANID->setRowHidden(i_hid,false);
//            list_vib->setRowHidden(i_hid,false);
//            ui->listVibr->setRowHidden(i_hid,false);
//            ///for new regim prostoy
//                             if (i_hid==0){
//                                 if(ui->BCar_1){
//                                    ui->BCar_1->setEnabled( true);
//
//                                 }
//                             }
//                             if (i_hid==1){
//                                 if(ui->BCar_2){
//                                    ui->BCar_2->setEnabled( true);
//                                 }
//                             }
//            ///end for new regim prostoy
//        }
//        fl_hid=0;
//    }
// end 28_01_14 - ubrano potomu chto zavisaet v cicle

    for(i=0;i<k_hid;i++)    {
                if (mas_fl_hid[i]!=2){
                    listV->setRowHidden(mas_hid[i], true);
                    ui->listV->setRowHidden(mas_hid[i], true);
                    ui->listV_n->setRowHidden(mas_hid[i], true);
                    
                    list_PANID->setRowHidden(mas_hid[i], true);
                    ui->list_PANID_n->setRowHidden(mas_hid[i], true);
                    
                    list_vib->setRowHidden(mas_hid[i], true);
                    ui->listVibr->setRowHidden(mas_hid[i], true);
                    ui->listVibr_n->setRowHidden(mas_hid[i], true);
                    ///for new regim prostoy
                                     if (mas_hid[i]==0){
                                         if (ui->BCar_1){
                                            ui->BCar_1->setEnabled(false);
                                           }
                                     }
                                     if (mas_hid[i]==1){
                                         if(ui->BCar_2){
                                            ui->BCar_2->setEnabled(false);
                                         }
                                     }
                    ///end for new regim prostoy

                }
                else{
                    listV->setRowHidden(mas_hid[i],false);
                    ui->listV->setRowHidden(mas_hid[i],false);
                    ui->listV_n->setRowHidden(mas_hid[i],false);
                    
                    list_PANID->setRowHidden(mas_hid[i],false);
                    ui->list_PANID_n->setRowHidden(mas_hid[i],false);
                    
                    list_vib->setRowHidden(mas_hid[i],false);
                    ui->listVibr->setRowHidden(mas_hid[i],false);
                    ui->listVibr_n->setRowHidden(mas_hid[i],false);
                    ///for new regim prostoy
                                     if (mas_hid[i]==0){
                                         if(ui->BCar_1){
                                            ui->BCar_1->setEnabled( true);

                                         }
                                     }
                                     if (mas_hid[i]==1){
                                         if(ui->BCar_2){
                                            ui->BCar_2->setEnabled( true);
                                         }
                                     }
                    ///end for new regim prostoy
                }

    }
    k_hid=0;

    if (fl_begin==3){//regim SUPER_N
        tabWidget2->hide();
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(2);
    }
    if (fl_begin==1){//regim START
        setMinimumSize( QSize( 600, 370 ) );
        tabWidget2->hide();
        checkDiskr->setChecked( false );// chtoby pri nachalnom regime ne bilo diskretnogo dvigenia (vsegda postoynnoe, poka nagata knopka
        checkVibrUst->setChecked(  true );// chtoby rabotal po algoritmu vibrannih ustroistv
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(0);
    }
    if (fl_begin==0){//regim SUPER
        setMinimumSize( QSize( 600, 670 ) );
        ui->stackedWidget->hide();
        tabWidget2->show();
    }
    if (fl_begin==2){//regim BASE
        tabWidget2->hide();
        setMinimumSize( QSize( 600, 370 ) );
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(1);
    }

    if (fl_begin==4){//regim BlueTooth
	 tabWidget2->hide();
        setMinimumSize( QSize( 600, 370 ) );
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(3);
    }



    if(fl_repaint_panid==1){
	
    	list_PANID->clear();
        k = listV->count();
        if (k<0) k=0;
        if (k>MAX_USTR) k=0;
        for(i=0;i<k;i++){
            if (act_ust[i].fl_pan==1){
                st.setNum(act_ust[i].pan1,16);
            }
            else st="*";
            stt= st +" ";


    //		st.setNum(act_ust[i].pan2,16);
    //		stt= stt+st;
		
            list_PANID->addItem(stt);
            if (ui->list_PANID_n) ui->list_PANID_n->addItem(stt);
//            if (act_ust[i].fl_antisleep!=2) list_PANID->setRowHidden(i, true);
//            else list_PANID->setRowHidden(i,false);
            SetHiddenCar(i,act_ust[i].fl_antisleep);

        }

        fl_repaint_panid=0;
    }
    if(fl_ans3_ust==1){
//	qDebug()<<"DDDDD\n";
        pl_ans_ust->setColor(QPalette::Base,Qt::green);
//        TLabAns->setPalette(*pl_ans_ust);
//        TLabAns->setText(ans3);
        
        if (ui->LStatus){
//	qDebug()<<"LLLL\n";
    	    ui->LStatus->setPalette(*pl_ans_ust);
    	    ui->LStatus->setText(ans3);
        }
        if (ui->LStatus_2){
//	qDebug()<<"LLLL\n";
            ui->LStatus_2->setPalette(*pl_ans_ust);
            ui->LStatus_2->setText(ans3);
        }

        if (ui->LStatus_B){
//	qDebug()<<"LLLL\n";
            ui->LStatus_B->setPalette(*pl_ans_ust);
            ui->LStatus_B->setText(ans3);
        }


        TLabAnsFirst->setPalette(*pl_ans_ust);
        TLabAnsFirst->setText(ans3);
    
        if (ui->TLabAnsFirst_n) {
        	ui->TLabAnsFirst_n->setPalette(*pl_ans_ust);
        	ui->TLabAnsFirst_n->setText(ans3);
        }
    
    
	if (flcomport==0){
    	    pl_ans_ust->setColor(QPalette::Base,Qt::red);
//    	    TLabAns->setPalette(*pl_ans_ust);
//    	    TLabAns->setText(ans1);
    	    TLabAnsFirst->setPalette(*pl_ans_ust);
    	    TLabAnsFirst->setText(ans1);

            if (ui->TLabAnsFirst_n) {
        	ui->TLabAnsFirst_n->setPalette(*pl_ans_ust);
        	ui->TLabAnsFirst_n->setText(ans1);
            }
            
            if (ui->LStatus){
                ui->LStatus->setPalette(*pl_ans_ust);
                ui->LStatus->setText(ans1);
            }
            if (ui->LStatus_2){
                ui->LStatus_2->setPalette(*pl_ans_ust);
                ui->LStatus_2->setText(ans1);
            }

            if (ui->LStatus_B){
    //	qDebug()<<"LLLL\n";
                ui->LStatus_B->setPalette(*pl_ans_ust);
                ui->LStatus_B->setText(ans1);
            }

    }
    
        fl_ans3_ust = 0;
    }
    
    if (fl_obn_spact!=0){
        if (fl_obn_spact==1){
         SpActDatch->clear();
         listV->clear();
         list_PANID->clear();
         ui->list_PANID_n->clear();
         
         //for BASE
         ui->listV->clear();
         ui->listV_n->clear();

         if (flcomport!=0){
             QPalette mpl(Qt::red,Qt::red);
             mpl.setColor(QPalette::Base,Qt::yellow);

//             if (TLabAns){
//                 TLabAns->setPalette(mpl);
//                 TLabAns->setText(ans2);
//             }

             if (TLabAnsFirst){
                 TLabAnsFirst->setPalette(mpl);
                 TLabAnsFirst->setText(ans2);
             }
            if (ui->TLabAnsFirst_n) {
        	ui->TLabAnsFirst_n->setPalette(mpl);
        	ui->TLabAnsFirst_n->setText(ans2);
//        	        	qDebug("1ans2\n");

            }

             if (ui->LStatus){
                 ui->LStatus->setPalette(mpl);
                 ui->LStatus->setText(ans2);
             }
             if (ui->LStatus_2){
                 ui->LStatus_2->setPalette(mpl);
                 ui->LStatus_2->setText(ans2);
             }

            if (ui->LStatus_B){
    //	qDebug()<<"LLLL\n";
                ui->LStatus_B->setPalette(mpl);
                ui->LStatus_B->setText(ans2);
            }


         }


        }
        if (fl_obn_spact==2){
//    	    SpActDatch->insertItem(1,txtAct);
    	    SpActDatch->insertItem(MAX_USTR,txtAct);
            listV->addItem(txtAct);
            ui->listV->addItem(txtAct);
            ui->listV_n->addItem(txtAct);
            if (kol_ust>0) {
                SetHiddenCar(kol_ust-1,act_ust[kol_ust-1].fl_antisleep);//chtoby ne pokazivalsa esli net otveta na komandu ne spat
                SpActDatch->setCurrentIndex(0);
            }

        }

        if (fl_obn_spact==3){
            SpActDatch->clear();
            listV->clear();
            ui->listV->clear();
            ui->listV_n->clear();



            ClearSpBeg();


            QString tt,txt;
            for (i=0;i<kol_ust;i++){
                if (act_ust[i].vibr_sp==1) txt =" ";
                else txt = " ";
                tt.setNum(i+1,10);
                txt = txt+tt+" : ";
                for(int j=0;j<8;j++){
                    tt.setNum(mas_mac_addr[i][j],16);
                    txt = txt + tt+" ";
                }
                txt = txt + "; ";
                for(int j=0;j<2;j++){
                    tt.setNum(mas_set_addr[i][j],16);
                    txt = txt + tt+" ";
                }

                if (act_ust[i].fl_sleep==0) txt=txt+trUtf8("; без сна; ");
                if (act_ust[i].fl_sleep==1) txt=txt+trUtf8("; может спать; ");

                tt.setNum(act_ust[i].impl,16);
                txt = txt + tt+", ";
                tt.setNum(act_ust[i].impr,16);
                txt = txt + tt;





                 listV->addItem(txt);
                 ui->listV->addItem(txt);
                 ui->listV_n->addItem(txt);


///for new regim prostoy
                 if (i==0){
                     ui->BCar_1->setEnabled( true);
                 }
                 if (i==1){
                     ui->BCar_2->setEnabled( true);
                 }
///end for new regim prostoy


                 SetHiddenCar(i,act_ust[i].fl_antisleep);


            }
            SpActDatch->insertItem(MAX_USTR,txt);
            fl_repaint_panid=1;
        }
        if (fl_obn_spact==4){

            list_vib->clear();
            ui->listVibr->clear();
            ui->listVibr_n->clear();

            k = listV->count();
            if (k<0) k=0;
            if (k>MAX_USTR) k=0;
            for(i=0;i<k;i++){
                if (act_ust[i].vibr_sp==1){
                    st="*";
                }
                else st=" ";
                list_vib->addItem(st);
                ui->listVibr->addItem(st);
                ui->listVibr_n->addItem(st);

                SetHiddenCar(i,act_ust[i].fl_antisleep);
//                if (act_ust[i].fl_antisleep!=2) list_vib->setRowHidden(i, true);
//                else list_vib->setRowHidden(i,false);

            }

        }

        fl_obn_spact=0;
    }
//	printf("OOOOOOOOO  fl_play=%d\n",fl_play);
    
    if (fl_play==0){
//	printf("GGGGGGGGGGGG\n");
        if(BPlay->isChecked()==1) {
            BPlay->setChecked(0);
            BPlay->setText(trUtf8("Поехали"));
#ifdef WIN_D            
            BPlay->setStyleSheet("background: lightgray");
#endif
#ifdef LINUX_D            
            BPlay->setStyleSheet("background: white");
#endif
        }

        if(ui->BPlay_n_B->isChecked()==1) {
            ui->BPlay_n_B->setChecked(0);
            ui->BPlay_n_B->setText(trUtf8("Поехали"));
            ui->BPlay_n_B ->setStyleSheet("background: lightgray");
        }


        if(ui->BPlay_n->isChecked()==1) {
            ui->BPlay_n->setChecked(0);
            ui->BPlay_n->setText(trUtf8("Поехали"));
            ui->BPlay_n->setStyleSheet("background: lightgray");
        }

        if(ui->pBPlay->isChecked()==1) {
            ui->pBPlay->setChecked(0);
            ui->pBPlay->setText(trUtf8("Поехали"));
#ifdef WIN_D
            ui->pBPlay->setStyleSheet("background: lightgray");
#endif

#ifdef LINUX_D
//            ui->pBPlay->setStyleSheet("background: white");
            ui->pBPlay->setStyleSheet("background: lightgray");
#endif

        }

    }
    
    if(checkNeSpat->isChecked()){
        if (GetTime()>(TimeNeSpat+PER_TIMENESPAT)){
	    #ifdef PRINT_DEBUG
	    qDebug("Send Comm Ne Spat\n");
	    #endif
	    
    	    TimeNeSpat=GetTime();
	    BPressStop();
	}
    }
    
    // otrisovka graphika
/*woronin    
    if ((pr_grph !=0)) {
    	if (pr_grph==1) {
//    	    cRight->setRawData(d_x, d_yt, PLOT_SIZE);
    	    cRight->setRawData(d_x, d_yt, Plot_size_tecT);
    	    cRight_n->setRawData(d_x, d_yt, Plot_size_tecT);
    //	    qDebug("%d\n",Plot_size_tecT);
    //	    for (int u=0;u<Plot_size_tecT;u++)qDebug("%f ",d_yt[u]);
    	    }
    	if (pr_grph==2) cRight->setRawData(d_x, d_ye, Plot_size_tecE);
    	if (pr_grph==3) cRight->setRawData(d_x, d_yo, Plot_size_tecT);
    	if (pr_grph==4) cRight->setRawData(d_x, d_yd, Plot_size_tecD);
        if (pr_grph==5) cRight->setRawData(d_x, d_yb, Plot_size_tecV);
        if (pr_grph==6) cRight->setRawData(d_x, d_yv, Plot_size_tecV);

    	if (pr_grph==2) cRight_n->setRawData(d_x, d_ye, Plot_size_tecE);
    	if (pr_grph==3) cRight_n->setRawData(d_x, d_yo, Plot_size_tecT);
    	if (pr_grph==4) cRight_n->setRawData(d_x, d_yd, Plot_size_tecD);
        if (pr_grph==5) cRight_n->setRawData(d_x, d_yb, Plot_size_tecV);
        if (pr_grph==6) cRight_n->setRawData(d_x, d_yv, Plot_size_tecV);

    Mgrph->replot();
    Mgrph_n->replot();
    }
*/
// init com-port
    if (flcomport==0){
//qDebug() << (timer_init_port - GetTime());
//qDebug()<<GetTime();
        if ((-timer_init_port+GetTime())>TIMER_INIT_PORT){
            QByteArray rr;
    	    rr = addr.toLocal8Bit();
	    qDebug("ustroistvo vvoda-vivoda:  %s\n",rr.constData()); // smotrim kakoe ustroistvo otkrilos
            if (COMInit(rr.constData())!=1){
	        qDebug("Error open port Exit\n");

	    }
	    timer_init_port = GetTime();
	}else {
	    if (GetTime()> 3*timer_init_port) timer_init_port   = GetTime();
	}

        if (flcomport!=0){
	    QPalette pl(Qt::red,Qt::red);
	    pl.setColor(QPalette::Base,Qt::yellow);
            TLabAnsFirst->setPalette(pl);
    	    TLabAnsFirst->setText(ans2);
        
    	    if (ui->TLabAnsFirst_n) {
        	ui->TLabAnsFirst_n->setPalette(pl);
        	ui->TLabAnsFirst_n->setText(ans2);
        	qDebug("ans2\n");
    	    }

    	    if (ui->LStatus){
    		ui->LStatus->setPalette(pl);
    		ui->LStatus->setText(ans2);
    	    }
    	    if (ui->LStatus_2){
        	ui->LStatus_2->setPalette(pl);
        	ui->LStatus_2->setText(ans2);
    	    }
    	    

//    	qDebug()<<"LLLL0\n";
            if (ui->LStatus_B){
//    	qDebug()<<"LLLL1\n";
                ui->LStatus_B->setPalette(pl);
                ui->LStatus_B->setText(ans2);
            }
    	    

	}
	mSleep(3);
    if (!fl_bluetooth){
    	    SendVklAPI();
	    SendVklNP();
    	    SendZaprosI();
	}

    }
// end init com-port
    
}// end processtimer

void Vench::BPressZapPANID()
{
    #ifdef PRINT_DEBUG
    qDebug("Press ZapPANID\n");
    #endif
	    SendZaprosPANIDAll();

}

void Vench::BPressIzmPANID()
{
    #ifdef PRINT_DEBUG
    qDebug("Press IzmPANID\n");
    #endif
    
    int i,k,j;
    unsigned char macm[8];
    QString name_f,tt;
    k = listV->count();
    for(j=0;j<8;j++){
	macm[j]=0;
    }
    for(i=0;i<k;i++){
        if (mas_vibr_ust[i] == 1){
    	    name_f= "";
            for(j=0;j<8;j++){
                macm[j] = (unsigned char)mas_mac_addr[i][j];
                tt.setNum(mas_mac_addr[i][j],16);
                name_f=name_f+tt+" ";
               }
                VvodPD->SetType( name_f, 0 );
                if (VvodPD->exec()==1){
                    int pn = VvodPD->GetPanID();
                    SendZapisPanID(macm,pn);

                    int     kk = GetNumberUst(macm);

//                act_ust[kk].fl_pan=0;
//                fl_repaint_panid = 1;

                if (PanIdKontr!=-1){
                    if (pn!=PanIdKontr){
                        if (kk>=0){
                            RemoveUstr(kk);
                            fl_repaint_panid = 1;
                            fl_obn_spact=3;
                        }
                    }
                }

	    }
	}
    }
    
}
void Vench::BPressLog()
{


//    QString ms = "gnome-terminal \"tail -f ~/.config/smartcar/";
//    ms=ms + LOGFILE+"\"";
    QString ms = "xterm -e tail -f ~/.config/smartcar/";
    ms=ms + LOGFILE;
    ms = ms + " &";

    #ifdef PRINT_DEBUG
    qDebug()<<ms;
    #endif
    QByteArray rr = ms.toLocal8Bit();
    int i = system(rr.constData());
    i++;

}
void Vench::BPressParam()
{

    print_nastr_file();


#ifdef WIN_D
   QString ms="";
   ms=ms+ "tmp_smart.ini";
   ms = ms+" ";
   QByteArray rr = ms.toLocal8Bit();
   WinExec(rr,SW_RESTORE)   ;
#endif


#ifdef LINUX_D
    
   QString     ms = mEdit;


    QDir dr;
    QString flname = dr.homePath();
    flname =flname+"/.config/smartcar/tmp_smart.ini";


    ms = ms+flname + " ";

   
    QByteArray rr = ms.toLocal8Bit();
    
    rr = ms.toLocal8Bit();
    int i = system(rr);
    i=i+1;
    
#endif

}

void Vench::BPressBDiscrImp_Ever()
{
    printf("Press Diskr imp\n");



    int i,k,j;
    unsigned char macm[8];
    QString name_f,tt;
    k = listV->count();
    for(j=0;j<8;j++){
	macm[j]=0;
    }
    for(i=0;i<k;i++){
        if (mas_vibr_ust[i] == 1){
    	    name_f= "";
            for(j=0;j<8;j++){
                macm[j] = (unsigned char)mas_mac_addr[i][j];
                tt.setNum(mas_mac_addr[i][j],16);
                name_f=name_f+tt+" ";
            }
            VvodPD->SetType( name_f, 1 );
            if (VvodPD->exec()==1){
                int impr,impl;
                int pn = VvodPD->GetKolImp(&impr,&impl);
                if (pn==1){
                    SetKolImpUstr(macm,impr,impl);
                }
            }
        }
    }

    fl_obn_spact=3;

}

void Vench::BPressIzmPANID_KONTR()
{
    printf("Press IzmPANID_KONTR\n");

    VvodPD->SetType(trUtf8(  "ВНИМАНИЕ PanID контроллера должен \nсоответствовать остальным \nустройствам" ),0);
    if (VvodPD->exec()==1){

        int pn = VvodPD->GetPanID();
				
		SendZapisPanID_KONTR(pn);	

        ObSpActUst();
    }
}

void Vench::BPressDiscrImp()
{
    QString t= DiscrImpE->text();
    imp_diskr = t.toInt(); 




}

void Vench::BPressSpeed()
{
    QString t= ImpS->text();
    speed_car = t.toInt(); 




}

void Vench::BPressSpeed_n()
{
    QString t= ui->ImpS_n->text();
    speed_car = t.toInt(); 




}


QString Vench::PrivodStroki(QString st){
    QString txt =st;
    txt = txt.toUpper();

    QString txtt = "  ";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" "));

    txtt = " 00";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 0"));
    txtt = " 01";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 1"));
    txtt = " 02";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 2"));
    txtt = " 03";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 3"));

    txtt = " 04";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 4"));

    txtt = " 05";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 5"));

    txtt = " 06";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 6"));

    txtt = " 07";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 7"));

    txtt = " 08";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 8"));

    txtt = " 09";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 9"));

    txtt = " 0A";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" A"));

    txtt = " 0B";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" B"));

    txtt = " 0C";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" C"));

    txtt = " 0D";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" D"));

    txtt = " 0E";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" E"));

    txtt = " 0F";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" F"));
    
    
//    qDebug("Privod\n");
//    qDebug()<<txt;
    return txt;
}
void Vench::BPressRuchnComm()
{
    QString txt;
    txt = CommText->text();
    txt = txt.toUpper();

    QString txtt = "  ";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" "));

    txtt = " 00";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 0"));
    txtt = " 01";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 1"));
    txtt = " 02";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 2"));
    txtt = " 03";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 3"));

    txtt = " 04";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 4"));

    txtt = " 05";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 5"));

    txtt = " 06";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 6"));

    txtt = " 07";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 7"));

    txtt = " 08";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 8"));

    txtt = " 09";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 9"));

    txtt = " 0A";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" A"));

    txtt = " 0B";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" B"));

    txtt = " 0C";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" C"));

    txtt = " 0D";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" D"));

    txtt = " 0E";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" E"));

    txtt = " 0F";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" F"));

    #ifdef PRINT_DEBUG
    qDebug()<<txt;
    #endif
    QByteArray rr = txt.toLocal8Bit();
    SendCommForRS(rr.constData());
    #ifdef PRINT_DEBUG
    qDebug("Send comm Ruchn vvod\n");
    #endif
}
void Vench::BPressRuchnComm_n()
{
    QString txt;
    txt = ui->CommText_n->text();
    txt = txt.toUpper();

    QString txtt = "  ";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" "));

    txtt = " 00";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 0"));
    txtt = " 01";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 1"));
    txtt = " 02";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 2"));
    txtt = " 03";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 3"));

    txtt = " 04";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 4"));

    txtt = " 05";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 5"));

    txtt = " 06";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 6"));

    txtt = " 07";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 7"));

    txtt = " 08";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 8"));

    txtt = " 09";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 9"));

    txtt = " 0A";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" A"));

    txtt = " 0B";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" B"));

    txtt = " 0C";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" C"));

    txtt = " 0D";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" D"));

    txtt = " 0E";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" E"));

    txtt = " 0F";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" F"));

    #ifdef PRINT_DEBUG
    qDebug()<<txt;
    #endif
    QByteArray rr = txt.toLocal8Bit();
    SendCommForRS(rr.constData());

    QMessageBox::information( this, trUtf8("Информация"),
                                               trUtf8("Команда отправлена на выполнение\n"),
                                               trUtf8("Ok") );
    
    #ifdef PRINT_DEBUG
    qDebug("Send comm Ruchn vvod\n");
    #endif
}

void Vench::BPressStop()
{
	if(fl_play) fl_play=0;
	if(fl_rec){
	    b_tim_comm = GetTime();
//	    break;

	}
	
//	BStop->setDown(1);
	PressRh=0;
	PressLf=0;
	PressRv=0;
	PressFw=0;
	SbrosDatKas();
     SendCommStop1();

}
void Vench::BRealeseStop()
{
	if(fl_play)fl_play=0;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) return;
	    mas_comm[numc]=9;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
	    
//	    break;
	}
	PressRh=0;
	PressLf=0;
	PressRv=0;
	PressFw=0;
	SbrosDatKas();
	SendCommStop1();
}


void Vench::BPressRec()
{
    if(BRec->isChecked()==1){

        // �������� ��� ������ ���� ���������� �������
        int k = listV->count();
        if (k<0) k=0;
        if (k>MAX_USTR) k=0;
        int mkl=0;
        for(int i=0;i<k;i++){
            if (act_ust[i].vibr_sp==1){
                mkl++;
            }
        }
        if (mkl==0){
            if (k>0){
                for(int j=0;j<8;j++){
                    mac_addr_prg[j] = act_ust[0].mac_addr[j];
                }
            }
            else{
                QMessageBox::information( this, trUtf8("Предупреждение"),
                                               trUtf8("Нет активных устройств\n"),
                                               trUtf8("Ok") );
                if (fl_begin==0) {
                        BRec->setChecked(0);
                }
                if (fl_begin==2) ui->pBRec->setChecked(0);
                return;
            }
        }
        if ((mkl>1)){
            QMessageBox::information( this, trUtf8("Предупреждение"),
                                           trUtf8("Для записи программы выберите только одно устройство\n"),
                                           trUtf8("Ok") );
            if (fl_begin==0) {
                    BRec->setChecked(0);
            }
            if (fl_begin==2) ui->pBRec->setChecked(0);
            return;

        }
        if (checkAll->isChecked()==1){
            QMessageBox::information( this, trUtf8("Предупреждение"),
                                           trUtf8("Снимите признак \"Все устройства\" \n"),
                                           trUtf8("Ok") );
            if (fl_begin==0) {
                    BRec->setChecked(0);
            }
            if (fl_begin==2) ui->pBRec->setChecked(0);
            return;
         }

        if (mkl==1){
            for(int i=0;i<k;i++){
                if (act_ust[i].vibr_sp==1){
                    for(int j=0;j<8;j++){
                        mac_addr_prg[j] = act_ust[i].mac_addr[j];
                    }
                }
            }
        }

        fl_play=0;
        numc=0;
//        qDebug("IIIIIIIIIIIII0\n");
        num_comm=0;
	ret_napr=0;
	rimp1=0;
	rimp2=0;
        
        fl_rec=1;
        if ((fl_begin==0)||(fl_begin==3)){
            BRec->setStyleSheet("background: red");
            BRec->setText(trUtf8("Стоп"));
            ui->BRec_n->setStyleSheet("background: red");
            ui->BRec_n->setText(trUtf8("Стоп"));
            ui->BRec_n_B->setStyleSheet("background: red");
            ui->BRec_n_B->setText(trUtf8("Стоп"));

        }
        if(fl_begin==2){
            ui->pBRec->setStyleSheet("background: red");
            ui->pBRec->setText(trUtf8("Стоп"));
        }
        //  printf("iii\n");
    }



}
void Vench::BRealeseRec()
{
//        qDebug("1nc=%d\n",num_comm);
    if(BRec->isChecked()==1) return;

    if (fl_rec) {
//      printf("ppp  = %d\n",mass_comm_n[0][0]);
//      WriteProg(numc);
    #ifdef PRINT_DEBUG
        qDebug("nc=%d\n",num_comm);
    #endif
        WriteProg1(num_comm);
//      WriteProg1(numc);
        fl_rec=0;
        if ((fl_begin==0)||(fl_begin==3)){
//            BRec->setStyleSheet("background: white");
            BRec->setStyleSheet("background: lightgray");
            ui->BRec_n->setStyleSheet("background: lightgray");
            ui->BRec_n_B->setStyleSheet("background: lightgray");
            BRec->setText(trUtf8("Запись"));
            ui->BRec_n->setText(trUtf8("Запись"));
            ui->BRec_n_B->setText(trUtf8("Запись"));
        }
        if(fl_begin==2){
#ifdef LINUX_D
//            ui->pBRec->setStyleSheet("background: white");
            ui->pBRec->setStyleSheet("background: lightgray");
#endif
#ifdef WIN_D
            ui->pBRec->setStyleSheet("background: lightgray");
#endif
            ui->pBRec->setText(trUtf8("Запись"));
        }

    }
}

void Vench::BPressProdol()
{
	fl_play_comm=0;
	printf("continue prog (don`t wait answer ustr)\n");
}

void Vench::BPressPlay_n()// sdelay po cliced!!!!
{
//    if (ui->BPlay_n->isChecked()==0) {
    if (fl_play==0) {
        BPlay->setChecked( true);
        BPressPlay();
        return;
    }
    if (fl_play==1) {
        BPlay->setChecked(false);
        BRealesePlay();
    }

}


//ne ispolzuetsa!!!!
void Vench::BRealesePlay_n()
{
//    if (ui->BPlay_n->isChecked()==0) qDebug("pl 0\n");
//    if (ui->BPlay_n->isChecked()==1) qDebug("pl 1\n");
//    qDebug("stop\n");
//    if (ui->BPlay_n->isChecked()==1) return;
    if (fl_play==1) {
    BPlay->setChecked(false);
    BRealesePlay();
    }
}

void Vench::BPressPlay()
{
    if(BPlay->isChecked()==1) {
//	qDebug("RRRRRRRRRRRr\n");
        if (checkKumir->isChecked() ==  true) {
            ReadCum();
        }else{
            ReadProg1();
        }
	
        BRec->setDown(0);
        fl_rec=0;
        fl_play=1;
        fl_play_comm=0;
        BPlay->setStyleSheet("background: green");
        BPlay->setText(trUtf8("Стоп"));

        ui->BPlay_n->setStyleSheet("background: green");
        ui->BPlay_n->setText(trUtf8("Стоп"));

        ui->BPlay_n_B->setStyleSheet("background: green");
        ui->BPlay_n_B->setText(trUtf8("Стоп"));

	
	int k =listV->count();
        for(int i=0;i<k;i++){
                if (act_ust[i].vibr_sp==1){
                    for(int j=0;j<8;j++){
                        mac_addr_prg[j] = act_ust[i].mac_addr[j];
                    }
                    break;
                }
        }

        
        if(fl_begin==2){
    	    ui->pBPlay->setStyleSheet("background: green");
    	    ui->pBPlay->setText(trUtf8("Стоп"));
	}
    
         for(int uu=0;uu<num_comm;uu++){
    #ifdef PRINT_DEBUG
	    qDebug("m1 =%d\n",		    mass_comm_n[uu][0] );
	    qDebug("m2 =%d\n",				    mass_comm_n[uu][1]);
	    qDebug("m3 =%d\n",			    mass_comm_n[uu][2] );
    #endif
	 }

    
    //    printf("n m = %d\n",numc);
 }else{
        fl_play=0;
        if ( BPlay) BPlay->setText(trUtf8("Поехали"));
        if (ui->BPlay_n) ui->BPlay_n->setText(trUtf8("Поехали"));
	if (ui->pBPlay) ui->pBPlay->setText(trUtf8("Поехали"));
        if (ui->BPlay_n_B)    ui->BPlay_n_B->setText(trUtf8("Поехали"));
	

#ifdef LINUX_D
//        BPlay->setStyleSheet("background: white");
        if ( BPlay) BPlay->setStyleSheet("background: lightgray");
        if (ui->BPlay_n) ui->BPlay_n->setStyleSheet("background: lightgray");
	if (ui->pBPlay) ui->pBPlay->setStyleSheet("background: lightgray");
	if (ui->BPlay_n_B)	ui->BPlay_n_B->setStyleSheet("background: lightgray");
#endif
#ifdef WIN_D
        if ( BPlay) BPlay->setStyleSheet("background: lightgray");
        if (ui->BPlay_n) ui->BPlay_n->setStyleSheet("background: lightgray");
	if (ui->pBPlay) ui->pBPlay->setStyleSheet("background: lightgray");
	if (ui->BPlay_n_B)	ui->BPlay_n_B->setStyleSheet("background: lightgray");
#endif
        SendCommStop1();

    }

}
void Vench::BRealesePlay()
{
    if(BPlay->isChecked()==1){
          return;
    }


    fl_play=0;
    if (BPlay) BPlay->setText(trUtf8("Поехали"));
    
    if (ui->BPlay_n) {
//	qDebug("UUUUUUUUUUUUuuu\n");
	ui->BPlay_n->setText(trUtf8("Поехали"));
    }
    if (ui->pBPlay) ui->pBPlay->setText(trUtf8("Поехали"));
    
#ifdef LINUX_D
//        BPlay->setStyleSheet("background: white");
        BPlay->setStyleSheet("background: lightgray");
        if (ui->BPlay_n) ui->BPlay_n->setStyleSheet("background: lightgray");
	if (ui->pBPlay) ui->pBPlay->setStyleSheet("background: lightgray");
#endif
#ifdef WIN_D
        BPlay->setStyleSheet("background: lightgray");
        if (ui->BPlay_n) ui->BPlay_n->setStyleSheet("background: lightgray");
	if (ui->pBPlay) ui->pBPlay->setStyleSheet("background: lightgray");
#endif

#ifdef WIN_D
    ui->pBPlay->setStyleSheet("background: lightgray");
//    ui->pBPlay->setStyleSheet("background: white");
#endif
#ifdef LINUX_D
    ui->pBPlay->setStyleSheet("background: lightgray");
//    ui->pBPlay->setStyleSheet("background: white");
#endif
    ui->pBPlay->setText(trUtf8("Поехали"));
    SendCommStop1();
//    qDebug("fl p = %d\n",fl_play);
}

void Vench::BPressLight()
{
    if(fl_play)return;
    if(fl_rec){
//	if (numc>(NUM_COMM-2)) return;
//	mas_comm[numc]=5;
//	numc++;
//	return;
	b_tim_comm = GetTime();
    }
    PressLight=1;
    


    SendCommLight1();
    
}

void Vench::SendCommLight1()
{
    int mk[10];
    int  numu;
    int i,j;
    
    
//    printf("fl_c = %d\n",fl_comm);
    if (fl_comm>0) return;
    fl_comm=1;
    
    
   numu=0;
 //   printf("SendCommorw\n");
    numu = SpActDatch->count();
    if (numu==0) {
    #ifdef PRINT_DEBUG
    qDebug("Net active ustr\n");
    #endif
//	ObSpActUst(); 
	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){



	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
        QString cm,ss;
        cm = "& b 3 44 37 5 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	SendComm(cm);

	    
	    
	    }
	}
	return;
    }

    if (checkAll->isChecked() ==  true) {//delaem otpravku odnoi komandi
	//for(j=0;j<numu;j++)
	{
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
        QString cm,ss;
        cm = "& b 3 44 37 5 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	SendComm(cm);
	    
	}	    
    }else{

    
    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
	mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
    
    QString cm,ss;
    cm = "& b 3 44 37 5 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
//    printf(cm);
    
	SendComm(cm);
    }
    
    if (fl_rec){
	if (num_comm<NUM_COMM){
            mass_comm_n[num_comm][0] = 0;
	    mass_comm_n[num_comm][1] = 0;
    	    mass_comm_n[num_comm][2] = 0xB1;
    	    num_comm++;
    	}
    }
    
}

void Vench::SendCommSon()
{
    int mk[10];
    int  numu;
    int i;
   numu=0;
 //   printf("SendCommorw\n");
    numu = SpActDatch->count();
    if (numu==0) {
    #ifdef PRINT_DEBUG
    qDebug("Net active ustr\n");
    #endif
//	ObSpActUst(); 
	return;//если список пуст
    }
    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
        mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
     QString cm,ss;
  
    cm = "& b 3 53 50 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
//    printf(cm);
    
    SendComm(cm);
    
   
    cm = "& b 3 53 4D 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
//    printf(cm);
    
    SendComm(cm);
    
}

void Vench::SendCommAntiSon(int * mak_a, int * mas_a)
{
    int mk[10];
    int i;

    for(i=0;i<8;i++){
	mk[i] =mak_a[i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_a[i];
    }
     
    
     QString cm,ss;

   
    cm = "& b 3 53 4D 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
//    printf(cm);
    
    SendComm(cm);
    
}


void Vench::SendZaprosSon(int * mak_a, int * mas_a)
{
    int mk[10];
    int i;

    for(i=0;i<8;i++){
    mk[i] =mak_a[i];
    }
    for(i=0;i<2;i++){
    mk[i+8] =mas_a[i];
    }


     QString cm,ss;


    cm = "& b 2 53 50 ";
//    printf(cm);
    for (int i=0;i<10;i++){
    ss="";
    ss.setNum(mk[i],16);
    cm = cm+ss+" ";
    }
//    printf(cm);

    SendComm(cm);

}


void Vench::BReleaseLight()
{
    if(fl_play)return;
    if(fl_rec){
        e_tim_comm = GetTime();

        if (numc>(NUM_COMM-2)) return;
        mas_comm[numc]=5;
        tim_comm[numc]= (e_tim_comm - b_tim_comm);
        numc++;
    //	return;
	
    }
    PressLight=0;
    SendCommStopLight1();
}


void Vench::SendCommStopLight1()
{
    int mk[10];
    int  numu;
    int i,j;

    if (fl_comm==1) return;
    fl_comm = 1;
   numu=0;
 //   printf("SendCommorw\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");

	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){



	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
        QString cm,ss;
        cm = "& b 3 44 37 4 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	SendComm(cm);

	    
	    
	    }
	}
	return;
    }



    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] = 0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] = 0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
        QString cm,ss;
        cm = "& b 3 44 37 4 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	SendComm(cm);
	    
	}	    
    }else{


    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
	mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
    
    QString cm,ss;
    cm = "& b 3 44 37 4 ";
//    printf(cm);
	for (int i=0;i<10;i++){
		ss="";
	    ss.setNum(mk[i],16);
		cm = cm+ss+" ";
        }
//    printf(cm);
    
	SendComm(cm);
    }
}




void Vench::BPressBip()
{
    if(fl_play)return;
    if(fl_rec){
//	if (numc>(NUM_COMM-2)) return;
//	mas_comm[numc]=6;
//	numc++;
//	return;
	b_tim_comm = GetTime();
    }
    PressBip=1;

	SendCommBip1();

}


void Vench::SendCommBip1()
{
    int mk[10];
    int  numu;
    int i,j;
   numu=0;
 //   printf("SendCommorw\n");
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");

	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){

	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
        QString cm,ss;
        cm = "& b 3 44 32 5 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	SendComm(cm);



	    
	    
	    }
	}
	return;
    }


    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] = 0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] = 0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
        QString cm,ss;
        cm = "& b 3 44 32 5 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	SendComm(cm);
	    
	}	    
    }else{
    
	numu = SpActDatch->currentIndex();
//        printf("num = %d\n",numu);    
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
    	    mk[i+8] =mas_set_addr[numu][i];
        }
        QString cm,ss;
	cm = "& b 3 44 32 5 ";
//    printf(cm);
        for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}
//    printf(cm);
    
        SendComm(cm);
    }
    if (fl_rec){
	if (num_comm<NUM_COMM){
            mass_comm_n[num_comm][0] = 0;
	    mass_comm_n[num_comm][1] = 0;
    	    mass_comm_n[num_comm][2] = 0xC1;
    	    num_comm++;
    	}
    }
}


void Vench::BReleaseBip()
{
    if(fl_play)return;
    if(fl_rec){
        e_tim_comm = GetTime();
        if (numc>(NUM_COMM-2)) return;
        mas_comm[numc]=6;
        tim_comm[numc]=  (e_tim_comm-b_tim_comm);
        numc++;
    //	return;
    }
    SendCommStopBip1();
    PressBip=0;
}


void Vench::SendCommStopBip1()
{
    int mk[10];
    int  numu;
    int i,j;
   numu=0;
 //   printf("SendCommorw\n");
    numu = SpActDatch->count();
    if (numu==0) {
        printf("Net active ustr\n");
        return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();
        for(j=0;j<numu;j++){
            if (mas_vibr_ust[j]==1){

                for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                }
                for(i=0;i<2;i++){
                mk[i+8] =mas_set_addr[j][i];
                }

                QString cm,ss;
                cm = "& b 3 44 32 4 ";
            //    printf(cm);
                for (int i=0;i<10;i++){
                    ss="";
                    ss.setNum(mk[i],16);
                        cm = cm+ss+" ";
                }
            //    printf(cm);

                SendComm(cm);
            }
        }
        return;
    }

    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
        QString cm,ss;
        cm = "& b 3 44 32 4 ";
    //    printf(cm);
	for (int i=0;i<10;i++){
	    ss="";
	    ss.setNum(mk[i],16);
    	    cm = cm+ss+" ";
	}
//    printf(cm);
    
	    SendComm(cm);
	    
	}	    
    }else{

    
        numu = SpActDatch->currentIndex();
    
        for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[numu][i];
        }
        for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[numu][i];
        }
     
    
	QString cm,ss;
        cm = "& b 3 44 32 4 ";
//    printf(cm);
        for (int i=0;i<10;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
        }
//    printf(cm);
    
	SendComm(cm);
    }
}



void Vench::BPressRight()
{
    SbrosDatKas();

    if(fl_play)return;
    if(fl_rec){
//	    if (numc>(NUM_COMM-2)) return;
//	mas_comm[numc]=2;
//	if (shiftPress==1) mas_comm[numc]=8;
//	numc++;
//	return;
	b_tim_comm=GetTime();
	
    }
    
//    sleep(3);
//    printf("sh = %d\n",shiftPress);
	if (shiftPress==0) SendCommRight1();
	else SendCommExtrRight1();
    PressRh=1;
}

void Vench::BPressLeft()
{


    SbrosDatKas();
    if(fl_play)return;
    if(fl_rec){
	b_tim_comm=GetTime();
//	    if (numc>(NUM_COMM-2)) return;
//	mas_comm[numc]=1;
//	if (shiftPress==1) mas_comm[numc]=7;
//  	numc++;
//	return;
    }

    if (shiftPress==0) SendCommLeft1();
    else SendCommExtrLeft1();
    PressLf=1;
}

void Vench::BReleaseLeft()
{
    if(fl_play)return;
    if(fl_rec){
	e_tim_comm=GetTime();
	if (numc>(NUM_COMM-2)) return;
	mas_comm[numc]=1;
	tim_comm[numc]= (e_tim_comm-b_tim_comm);
	if (shiftPress==1) mas_comm[numc]=7;
  	numc++;
//	return;
    }
    PressLf=0;
    //!!!2015 07 23 dla proverki diskretnogo dvigenia!!!
    if (checkDiskr->isChecked() == false) SendCommStop1();
}

void Vench::BReleaseRight()
{
    if(fl_play)return;
    if(fl_rec){
        e_tim_comm=GetTime();
	    if (numc>(NUM_COMM-2)) return;
        mas_comm[numc]=2;
        tim_comm[numc]= (e_tim_comm-b_tim_comm);
        if (shiftPress==1) mas_comm[numc]=8;
        numc++;
	//	return;
	//	return;
    }
    PressRh=0;
    //!!!2015 07 23 dla proverki diskretnogo dvigenia!!!
    if (checkDiskr->isChecked() == false) SendCommStop1();
}

void Vench::BPressFw()
{	
    SbrosDatKas();
    
    if(fl_play)return;
    if(fl_rec){
        b_tim_comm=GetTime();
//	    if (numc>(NUM_COMM-2)) return;
//	mas_comm[numc]=3;
//  	numc++;
//	return;
    }
    SendCommForw1();
    PressFw=1;
}

void Vench::BReleaseFw()
{

    if(fl_play)return;
    if(fl_rec){
        e_tim_comm=GetTime();
	    if (numc>(NUM_COMM-2)) return;
        mas_comm[numc]=3;
        tim_comm[numc]= (e_tim_comm-b_tim_comm);
        numc++;
//	return;
    }
    PressFw=0;
    //!!!2015 07 23 dla proverki diskretnogo dvigenia!!!
    if (checkDiskr->isChecked() == false) SendCommStop1();
}

void Vench::BPressRv()
{
    SbrosDatKas();
    if(fl_play)return;
    if(fl_rec){
        b_tim_comm=GetTime();
//	    if (numc>(NUM_COMM-2)) return;
//	mas_comm[numc]=4;
//  	numc++;
//	return;
    }
    SendCommRevers1();
    PressRv=1;
}

void Vench::BReleaseRv()
{
    if(fl_play)return;
    if(fl_rec){
        e_tim_comm=GetTime();
	    if (numc>(NUM_COMM-2)) return;
        mas_comm[numc]=4;
        tim_comm[numc]= (e_tim_comm-b_tim_comm);
        numc++;
    }
    PressRv=0;
    //!!!2015 07 23 dla proverki diskretnogo dvigenia!!!
    if (checkDiskr->isChecked() == false) SendCommStop1();
}








void Vench::SendCommTempOn()
{
    int mk[20];
    int  numu;
    int i,j;
    
    
    numu=0;
    qDebug("SendCommTempOn\n");
    numu = SpActDatch->count();
    if (numu==0) {

	
	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();
qDebug("TTTTTTTTTT\n");
	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){


	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
    
    QString cm,ss;

    cm = "& 3 10 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);


	    
	    
	    }
	}
	return;
    }



    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
    
    QString cm,ss;

    cm = "& 3 10 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);

	}
    }else
    {

    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
	mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
    
    QString cm,ss;
    cm = "& 3 10 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);
    }
}

void Vench::SendCommTempOff()
{
    int mk[20];
    int  numu;
    int i,j;
    
    
    numu=0;
  //  printf("SendCommLeft\n");
    numu = SpActDatch->count();
    if (numu==0) {

	
	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){

	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
    
    QString cm,ss;
    cm = "& 3 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);



	    
	    
	    }
	}
	return;
    }



    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] = 0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] = 0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
    
    QString cm,ss;
    cm = "& 3 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);
	}
}
	else{

    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
	mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
    
    QString cm,ss;
    cm = "& 3 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);

    SendComm(cm);
	}
}



void Vench::SendCommBarOn()
{

    int mk[10];
    int  numu;
    int i,j;
   numu=0;
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");
        return;//если список пуст
    }
    
        if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

            numu = listV->count();

    	    for(j=0;j<numu;j++){
		if (mas_vibr_ust[j]==1){
	            for(i=0;i<8;i++){
    		        mk[i] =mas_mac_addr[j][i];
        	    }
        	    for(i=0;i<2;i++){
            		mk[i+8] =mas_set_addr[j][i];
        	    }
        	    QString cm,ss;
        	    cm = "* 7E 0 F 10 11 ";

		    //    printf(cm);
        	    for (int i=0;i<8;i++){
            		ss="";
            		ss.setNum(mk[i],16);
            		cm = cm+ss+" ";
        	    }
	            cm = cm+ "FF FE 0 0 A5";
	            qDebug() << cm;
        	    SendComm(cm);
		}
	    }
	    return;
	}

    
    if (checkAll->isChecked() ==  true) {
	//for(j=0;j<numu;j++)
	{
	    for(i=0;i<8;i++){
        	mk[i] =0;
	    }
	    for(i=0;i<2;i++){
        	mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
	    QString cm,ss;
	    cm = "* 7E 0 F 10 11 ";
	    //    printf(cm);
	    for (i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
            cm = cm+ "FF FE 0 0 A5";
	    SendComm(cm);
	}
    }
    else{
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	QString cm,ss;
	cm = "* 7E 0 F 10 11 ";
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}
	cm = cm+ "FF FE 0 0 A5";
//	qDebug()<<cm;
    
	
	SendComm(cm);
    }
}//endvoid Vench::SendCommBarOn()


void Vench::SendCommBarOff()
{
    
}//end void Vench::SendCommBarOff()




void Vench::SendCommEnergOn()
{
    int mk[20];
    int  numu;
    int i,j;
    
    
    numu=0;
    qDebug("SendCommEnergOn\n");
    numu = SpActDatch->count();
    if (numu==0) {

	
	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){


	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
    
    QString cm,ss;

    cm = "& 4 10 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);


	    
	    
	    }
	}
	return;
    }



    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
    
    QString cm,ss;

    cm = "& 4 10 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);

	}
    }else
    {

    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
	mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
    
    QString cm,ss;
    cm = "& 4 10 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);
    }
}

void Vench::SendCommEnergOff()
{
    int mk[20];
    int  numu;
    int i,j;
    
    
    numu=0;
  //  printf("SendCommLeft\n");
    numu = SpActDatch->count();
    if (numu==0) {

	
	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){

	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
    
    QString cm,ss;
    cm = "& 4 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);



	    
	    
	    }
	}
	return;
    }



    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] = 0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] = 0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
    
    QString cm,ss;
    cm = "& 4 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);
    
    SendComm(cm);
	}
}
	else{

    numu = SpActDatch->currentIndex();
    
    for(i=0;i<8;i++){
	mk[i] =mas_mac_addr[numu][i];
    }
    for(i=0;i<2;i++){
	mk[i+8] =mas_set_addr[numu][i];
    }
     
    
    
    QString cm,ss;
    cm = "& 3 0 ";
//    printf(cm);
    for (int i=0;i<10;i++){
	ss="";
	ss.setNum(mk[i],16);
	cm = cm+ss+" ";
    }
    cm = cm+" FF FF";
//    printf(cm);

    SendComm(cm);
	}
}





void Vench::SendComm( QString hcm )
{
    
//    FILE* fl_com;
    QString txt,cm;

    cm = hcm;
    
//    int i;
    txt ="";
    txt= ServerDir->text();
    txt = txt + "/COM_F.txt";

//            qDebug() << cm;
//            qDebug("SSSS\n"); 	
    cm = PrivodStroki(cm);
//            qDebug() << cm;
//            qDebug("SSSS1\n"); 	


    
//        QFile fl_c(txt);
//	bool q = fl_c.open(QIODevice::Append|QIODevice::ReadWrite);
//	if (q==false) {
	    
//	    bool qq = fl_c.open(QIODevice::ReadWrite);
//	    	printf("ppppp\n");

//	    if (qq==false){
//		printf("Error writer file COM_F\n");
//		return;
//	    }
//	}
    
    cm = cm +"\n";
    
    QByteArray rr = cm.toLocal8Bit();
    
//    qDebug("ttt ssend comm\n");
//    qDebug("%s\n",rr.constData());
    
    SendCommForRS(rr.constData());
    if (fl_comm==1) fl_comm=0;
//    if (fl_comm==2) fl_comm =0;
//    printf("fl_comm=%d\n",fl_comm);

//zadergka dla novoy versii, elsi nado vsegda to ubrat uslovie
//	mSleep(tm_usleep_com*10);
	mSleep(tm_usleep_com);

// fl_c.write( cm.toLocal8Bit(),cm.length());//for qt4

// fl_c.flush();
// fl_c.close();
}

void Vench::ObSpActUst()
{
//for test
//    if (kol_ust>0) {act_ust[0].fl_antisleep = 2;
//        SetHiddenCar(0,2);
///        }
//     return;
    // end for test

    kol_ust=0;
    mkol_ust =0;


    fl_obn_spact=1;
//    SpActDatch->clear();

    mSleep(50);
    SendCommObnovit();

    //    sleep(1);
   //ReadctiveLog();
  //  IzmNumbDatch();
//  kol_ust=0;
}

void Vench::SendCommObnovit()
{
//  qDebug("SendCommObnovit\n");
    
    SendComm("& 0 1 ff ff");
}


void Vench::keyPressEvent( QKeyEvent *e )
{
    int k;

    k = e->key();
    if (tabWidget2->currentIndex()!=0) return;

    if (fl_begin==1) if (fl_vibr_ust==0) return;// chtoby ne srabativalo v nachalnem regime pri nevibrannih ustr-h

//    QString str;
//    str.setNum(k);
//   this->setCaption(str);
//qDebug(str.toLocal8Bit());
    
// up-    4115
//    down 4117
    //left 4114
    //right - 4116
    // shift - 4128
    if (e->isAutoRepeat()== true) return;
    
    switch(k){
	
    case 77://m
   //   fl_rec=0;
      SendCommBudil();
      BAntiSleep->setDown(1);
      break;
    case 80://p
   //   fl_rec=0;
    //  SendCommStopBip();
	if (fl_play==0){
	    fl_play=1;
	    fl_play_comm=0;
	}
      BPlay->setDown(1);
      break;
    case 82://r
      //SendCommStopBip();
      if (fl_play==1)fl_play=0;
      BPlay->setDown(0);
      BRec->setDown(1);
      if (fl_rec==0) {
          numc=0;
//        qDebug("IIIIIIIIIIIII1\n");
          num_comm=0;
          fl_rec=1;
      }
      break;
	
	
	
    case 66://B
	BBip->setDown(1);
	if(fl_play)break;
	if(fl_rec){
	    b_tim_comm = GetTime();
//	    if (numc>(NUM_COMM-2)) break;
//	    mas_comm[numc]=6;
//	    numc++;
//	    break;
	}
//	if (kp==1) break;
//	kp=1;
	
	if (PressBip==0){
//	    PressBip=0;
	    SendCommBip1();
	    PressBip=1;
	}
//	SendCommBip();
	break;
	
    case 76://L
	BLight->setDown(1);
	if(fl_play)break;
	if(fl_rec){
	    b_tim_comm = GetTime();
//	    if (numc>(NUM_COMM-2)) break;
//	    mas_comm[numc]=5;
//	    numc++;
//	    break;
	}
	if (PressLight==0){
//	    PressLight=0;
    	    SendCommLight1();
	    PressLight=1;
	}
	
//	SendCommLight();
	break;
//    case 16777235://up
    case 87://up
//	printf("Kp=%d\n",kp);

        if (fl_bluetooth){
            on_BUp_B_pressed();
            break;
        }

	BUp->setDown(1);
	if(fl_play)break;
    if(fl_rec){
	    b_tim_comm = GetTime();
//	    if (numc>(NUM_COMM-2)) break;
//	    mas_comm[numc]=3;
//	    numc++;
//	    break;
	}
	if (kp==1) break;
	kp=1;
	if (PressRh==1){
	    PressRh=0;
	    SendCommStop1();
	    PressRh=1;
	}
	if (PressRv==1){
	    PressRv=0;
	    SendCommStop1();
	    PressRv=1;
	}
	if (PressLf==1){
	    PressLf=0;
	    SendCommStop1();
	    PressLf=1;
	}
	    SbrosDatKas();
	
        SendCommForw1();
        PressFw=1;
    qDebug("Send klav comm Forw\n");
	break;
//    case 16777237://down
    case 83://down
        if (fl_bluetooth){
            qDebug("BT:down\n");
            on_BDown_B_pressed();
            break;
        }

	BDown->setDown(1);
	if(fl_play)break;
	if(fl_rec){
	    b_tim_comm = GetTime();
//	    if (numc>(NUM_COMM-2)) break;
//	    mas_comm[numc]=4;
//	    numc++;
//	    printf("numc = %d\n",numc);
//	    break;
	}
	if (PressRh==1){
	    PressRh=0;
	    SendCommStop1();
	    PressRh=1;
	}
	if (PressFw==1){
	    PressFw=0;
	    SendCommStop1();
	    PressFw=1;
	}
	if (PressLf==1){
	    PressLf=0;
	    SendCommStop1();
	    PressLf=1;
	}
	
        SbrosDatKas();
	
	SendCommRevers1();
	PressRv=1;

    qDebug("Send klav comm revers\n");
    break;
//    case 16777234://left
    case 65://left
        if (fl_bluetooth){
            on_BLeft_B_pressed();
            break;
        }
	
	BLeft->setDown(1);
	if(fl_play)break;
	if(fl_rec){
	    b_tim_comm = GetTime();
//	    if (numc>(NUM_COMM-2)) break;
//	    mas_comm[numc]=1;
//	    if (shiftPress==1) mas_comm[numc]=7;
//	    numc++;
//	    break;
	}
	
	if (PressRh==1){
	    PressRh=0;
    	    SendCommStop1();
	    PressRh=1;
	}
	if (PressFw==1){
	    PressFw=0;
    	    SendCommStop1();
	    PressFw=1;
	}
	if (PressRv==1){
	    PressRv=0;
    	    SendCommStop1();
	    PressRv=1;
	}

        SbrosDatKas();
	
    if (shiftPress==0) SendCommLeft1();
    if (shiftPress==1) SendCommExtrLeft1();
	PressLf=1;
    qDebug("Send klav comm left\n");
    break;
//    case 16777236:// right
    case 68:// right



	BRight->setDown(1);

        if (fl_bluetooth){
            on_BRight_B_pressed();
            break;
        }
	if(fl_play)break;
	if(fl_rec){
	    b_tim_comm = GetTime();
//	    if (numc>(NUM_COMM-2)) break;
//	    mas_comm[numc]=2;
//	    if (shiftPress==1) mas_comm[numc]=8;
//	    numc++;
//	    break;
	}
	if (PressRv==1){
	    PressRv=0;
    	    SendCommStop1();
	    PressRv=1;
	}
	if (PressFw==1){
	    PressFw=0;
    	    SendCommStop1();
	    PressFw=1;
	}
	if (PressLf==1){
	    PressLf=0;
    	    SendCommStop1();
	    PressLf=1;
	}
    
        SbrosDatKas();
	
    if (shiftPress==0) SendCommRight1();
    if (shiftPress==1) SendCommExtrRight1();
	PressRh=1;
    qDebug("Send klav comm right\n");
    break;
    case 16777248://shift
        shiftPress =1;
        BShift->setDown(1);
	break;
    
    case 32://probel
        if(fl_play) fl_play=0;
        if(fl_rec){
            b_tim_comm = GetTime();
    //	    break;
        }
        SbrosDatKas();
	
        BStop->setDown(1);
        PressRh=0;
        PressLf=0;
        PressRv=0;
        PressFw=0;
        SendCommStop1();
        qDebug("Send klav comm stop\n");
        break;
    }
    
}


void Vench::SetKatalog(QString kat)
{
    katalog = kat;
    fl_kat=1;
}

void Vench::SetPort(QString kat)
{
    addr = kat;
    fl_port_s=1;
}



void Vench::at_write (int8_t *buffer, int32_t len)
{
#ifdef LINUX_D
//for dorne
//	int32_t flags;


	channel_stdput(&channel_drone, (void*) buffer, (int)len);
	

//end for dorne
#endif

}


void Vench::send_command(QString st)
{
#ifdef LINUX_D
//for dorne
//	unsigned long current;
 	char str[1024];

	
   // send command to drone
//	current = get_time_ms();
    QByteArray rr = st.toLocal8Bit();
    
//    snprintf(str, 1024, rr.data());
    sprintf(str, "%s", rr.constData());   //20201121

   //Send AT command	
   at_write((int8_t*)str, strlen (str));

//end for dorne
#endif
}



void Vench::init()
{
    
    int k;
    
    if (ui->BRec_n_B) ui->BRec_n_B->setEnabled(1);

    
    timer_init_port = GetTime();
    
    f_name_xml = NAME_FILE_VAR_NEW;
    f_name_json = NAME_FILE_VAR_NEW;
    f_name_snap = NAME_FILE_VAR_NEW;
    pr_grph =0;
    rimp1=0;
    rimp2=0;
    fl_datKas =0;
    k_hid=0;
    
    Plot_size_tecT=0;
    Plot_size_tecE=0;
    Plot_size_tecD=0;
    Plot_size_tecV=0;
    flzaprosdim=0;
    flzaprosbar=0;
    flzaprostemposv=0;
    progrPostrGRPH="";
    progrShowGRPH="";
    TimeNeSpat=0;
    for(k=0;k<MAX_USTR;k++){
        mas_vibr_ust[k]=0;
    }
    for(k=0;k<8;k++){
        mac_addr_prg[k]=0;
    }
    listV->clear();
    
    

	t_oprosa_b=0;
	period_oprosa_b = PERIOD_OPROSA_DAT;
    
    //for rs
    
//	kol_port=20;
	sch_auto = 0;
	fl_auto_succ=0;
//	fl_auto_port=0;
	fl_auto_wait=0;

    //28_01_14
//    fl_hid =0;
//    i_hid =0;
//    fl_fl_hid =0;
//end 28-01-14

    fl_begin = 0;
    fl_clsp=0;
    PanIdKontr =-1;
     TecTimeWaitansPort = GetTime();
    fl_logm=NULL;
    fl_comm=0;
    ret_napr =0;
    fl_al=0;
    fl_obn_spact=0;
    flcomport=0;
    err=0;
    fl_n=0;
    l=0;
    l_k=0;
    imp_diskr=0;
    speed_car=100;
    prog_for_read_win = "C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe";
    progrPostrGRPH  =  "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe";
    progrShowGRPH = "C:\\Windows\\System32\\mspaint.exe";
    fl_izm_dim=0;
    fl_repaint_panid = 0;
	
    tm_usleep_com=500;
    pr_spec_file_t =0;
    def_diskr_impr = DEF_DISKR_IMPR;
    def_diskr_impl = DEF_DISKR_IMPL;
    fl_print_log = 0;
    fl_print_log_B = 0;
    fl_al_sn =0;
    t_oprosa =0;
    

    t_oprosa_e =0;
	
    fl_period =0;
    period_oprosa_t =0;
    period_oprosa_e =0;
    fl_period_e =0;
    period_oprosa_e =0;

	
    fl_krd=0;
    fl_krd_start=0;
    fl_potoc_rs=1;

//    fl_vyvod = 0;
//	strcpy(addr,"/dev/ttyUSB0");
//    strcpy(addr,"/dev/ttyS0");
    addr = "/dev/ttyS0";// po umolchaniu, esli ne naiden smart.ini


//for automatic opredelenia com port
    strcpy(addr_auto[0],"/dev/ttyS0");
    strcpy(addr_auto[1],"/dev/ttyS1");
    strcpy(addr_auto[2],"/dev/ttyS2");
    strcpy(addr_auto[3],"/dev/ttyS3");
    strcpy(addr_auto[4],"/dev/ttyS4");
    strcpy(addr_auto[5],"/dev/ttyS5");
    strcpy(addr_auto[6],"/dev/ttyS6");
    strcpy(addr_auto[7],"/dev/ttyS7");
    strcpy(addr_auto[8],"/dev/ttyS8");
    strcpy(addr_auto[9],"/dev/ttyS9");
    strcpy(addr_auto[10],"/dev/ttyUSB0");
    strcpy(addr_auto[11],"/dev/ttyUSB1");
    strcpy(addr_auto[12],"/dev/ttyUSB2");
    strcpy(addr_auto[13],"/dev/ttyUSB3");
    strcpy(addr_auto[14],"/dev/ttyUSB4");
    strcpy(addr_auto[15],"/dev/ttyUSB5");
    strcpy(addr_auto[16],"/dev/ttyUSB6");
    strcpy(addr_auto[17],"/dev/ttyUSB7");
    strcpy(addr_auto[18],"/dev/ttyUSB8");
    strcpy(addr_auto[19],"/dev/ttyUSB9");
//end for automatic opredelenia com port
    
    
    
    //end for rs
    
    
    
    TimeActiveLog = GetTime()+TPeriodActive;
    shiftPress=1;
    kp=0;
    PressFw =0;
    PressRv=0;
    PressLf=0;
    PressRh=0;
    PressBip=0;
    PressLight=0;

    kol_ust =0;
    kol_data =0;
    mkol_ust =0;


//chitaem redactor dla linux
    char * var = getenv("VISUAL");
    if (var!=NULL) mEdit.append(var);
    else {
	var = getenv("EDITOR");
        if (var!=NULL) mEdit.append(var);
	else {
	    mEdit = "gedit";
	}
    }
    mEdit = mEdit+" ";



    QDir dr;
    QString md = dr.homePath();
	    md = md+"/.config/smartcar/";
//esli dirnne sushestvuet - sozdaem
    dr.mkdir(md);
    	    
    ServerDir->setText(md);
    ui->ServerDir_n->setText(md);
    f_name_kum = "script.cum";
    n_imp_kum = 17;
    n_imp_kum_p = 16;
    def_diskr_impr = 17;
    def_diskr_impl = 16;
    tm_usleep_com =500;
    imp_diskr= 12;
    addr_win = "COM3";
    fl_begin=2;
    if (fl_bluetooth) fl_begin =4;

    ReadIni();



// chtobi pokazivalsa iz ini fila
    QString mtd;
    mtd.setNum(imp_diskr,10);
    DiscrImpE->setText(mtd);
    mtd.setNum(speed_car,10);
    ImpS->setText(mtd);
    ui->ImpS_n->setText(mtd);
    
    mSleep(100);//00000000
    fl_potoc=1;
    fl_rec=0;
    fl_play=0;
    fl_play_comm=0;
    numc=0;
//        qDebug("IIIIIIIIIIIII2\n");
    num_comm=0;
    mas_comm=new int[NUM_COMM];
    tim_comm=new double[NUM_COMM];
    b_tim_comm = 0;
    e_tim_comm = 0;

    fl_potoc =1;


    thrFile = new MyThread (TestFile,this);
    thrFile->start();




    QByteArray rr;
    rr = addr.toLocal8Bit();

    qDebug("ustroistvo vvoda-vivoda:  %s\n",rr.constData()); // smotrim kakoe ustroistvo otkrilos
    
    if (flcomport==0){
        if (COMInit(rr.constData())!=1){
            qDebug("Error open port Exit\n");
            QString st = trUtf8("Ошибка инициализации порта: ")+addr;

            QMessageBox::information( this, trUtf8("Ошибка"),
                                       st,
                                       trUtf8("Ok") );
        }
    }
    if (flcomport!=0){
        QPalette pl(Qt::red,Qt::red);
        pl.setColor(QPalette::Base,Qt::yellow);
//        TLabAns->setPalette(pl);
//        TLabAns->setText(ans2);
        TLabAnsFirst->setPalette(pl);
        TLabAnsFirst->setText(ans2);
        
        if (ui->TLabAnsFirst_n) {
        	ui->TLabAnsFirst_n->setPalette(pl);
        	ui->TLabAnsFirst_n->setText(ans2);
//        	qDebug("ans2\n");
        }

        if (ui->LStatus){
    	    ui->LStatus->setPalette(pl);
    	    ui->LStatus->setText(ans2);
        }
        if (ui->LStatus_2){
            ui->LStatus_2->setPalette(pl);
            ui->LStatus_2->setText(ans2);
        }
        if (ui->LStatus_B){
    //	qDebug()<<"LLLL\n";
                ui->LStatus_B->setPalette(pl);
                ui->LStatus_B->setText(ans2);
        }


    }
    mSleep(3);
//esli potoc sozfdaetsa s pomoshiy QThread
    thrRS = new MyThread (TestPotoc,this);

    thrRS->start();

    if (!fl_bluetooth){
	SendVklAPI();
	SendVklNP();
	SendZaprosI();
    }


    QString sd;
    sd.setNum(imp_diskr,10);
    DiscrImpE->setText(sd);

    ReadProg1();
    
//    SendComm("log");
//qDebug()<<"LLLLLLLLLLLLLLLLLL";


// cvet nadpisi BT
	    QPalette plbt(Qt::yellow,Qt::yellow);
	    plbt.setColor(QPalette::Base,Qt::yellow);
        
    	    if (ui->label_31) {
    		ui->label_31->setPalette(plbt);
    	    }
//end cvet nadpisi BT



#ifdef LINUX_D
//for drone
    channel_open(&channel_drone, PROTO_UDP, WRITE_MODE, PORT_DRONE,(char*)ADDR_SHIR, NULL, NULL, NULL, NULL);
#endif




    //  Initialize data for grph
    for (int i = 0; i< PLOT_SIZE; i++)
    {
//        d_x[i] = 0.5 * i;     // time axis
        d_x[i] =  period_oprosa_b*i;     // time axis
        d_y[i] = 0;
        d_yt[i] = 0;
        d_ye[i] = 0;
        d_yo[i] = 0;
        d_yd[i] = 0;
        d_yb[i] = 0;
        d_yv[i] = 0;
        d_yp[i] = 0;
    }


//    d_yt[50] = 44;
//    d_ye[90] = 4;
//    d_yo[44] = -44;
//    d_yp[25] = 20;
//    d_yb[10] = -20;
//    d_yd[70] = -10;


    if (!fl_bluetooth) {
        ui->BDopParam->setChecked(1);
        BPressedDopParam();
    }

//choby byli odnogo cveta???
    DatKas(1);

    DatKas(0);
    SbrosDatKas();


    if(ui->BStop_B) ui->BStop_B->hide();

    mSleep(10);
    
//    ReadActiveLog();//chtobi pri pervom zap


}


void Vench::destroy()
{
    if (mas_comm) delete [] mas_comm;
    if (tim_comm) delete [] tim_comm;
    this->releaseKeyboard();
    fl_potoc=0;
}


void Vench::keyReleaseEvent( QKeyEvent *e )
{
    int k;

    k = e->key();
    if (tabWidget2->currentIndex()!=0) return;
    if (fl_begin==1) if (fl_vibr_ust==0) return;// chtoby ne srabativalo v nachalnem regime pri nevibrannih ustr-h

// up-    4115
//    down 4117
    //left 4114
    //right - 4116
    // shift - 4128
//    	printf("Kp1=%d\n",k);

    
    if (e->isAutoRepeat()== true) return;
    
    switch(k){

    case 77://m
   //   fl_rec=0;
//      SendCommBudil();
      BAntiSleep->setDown(0);
      break;


  case 80://p
   //   fl_rec=0;
      SendCommStopBip1();
      if (fl_play==1)fl_play=0;
      BPlay->setDown(0);
      break;
  case 82://
      if (fl_rec) {
//        WriteProg(numc);
        WriteProg1(num_comm);
//        WriteProg1(numc);
        fl_rec=0;
	}
      BRec->setDown(0);
      break;
	
    case 66://b
	BBip->setDown(0);
	if(fl_play)break;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=6;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
//	    break;
	}


	  SendCommStopBip1();
	

	PressBip=0;
	break;
	
    case 76://L
	BLight->setDown(0);
	if(fl_play)break;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=5;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
	}

    	    SendCommStopLight1();

	PressLight=0;
	break;
	
//    case 16777235://up
    case 87://up

//	printf("Kp1=%d\n",kp);

        if (fl_bluetooth){
	    SendCommStop_ave_BT();
            break;
        }

	BUp->setDown(0);
	if(fl_play)break;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=3;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
//	    break;
	}
    
	if (kp==0) break;
	kp=0;
	PressFw=0;

	SendCommStop1();

	break;
//    case 16777237://down
    case 83://down
        if (fl_bluetooth){
	    SendCommStop_ave_BT();
            break;
        }


	BDown->setDown(0);
	if(fl_play)break;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=4;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
//	    break;
	}
	PressRv=0;
	SendCommStop1();
	break;
//    case 16777234://left
    case 65://left
//	printf("terfdsvdcvc\n");
        if (fl_bluetooth){
	    SendCommStop_ave_BT();
            break;
        }


	BLeft->setDown(0);
	if(fl_play)break;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=1;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
//	    printf("numc = %d\n",numc);
//	    break;
	}
	PressLf=0;	
	SendCommStop1();
	break;
//    case 16777236:// right
    case 68:// right
        if (fl_bluetooth){
	    SendCommStop_ave_BT();
            break;
        }


	BRight->setDown(0);
	if(fl_play)break;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=2;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    if (shiftPress==1) mas_comm[numc]=8;
	    numc++;
//	    break;
	}
	PressRh=0;
	SendCommStop1();
	break;
    case 16777248://shift
        shiftPress =0;
        BShift->setDown(0);
	break;
	
    case 32://probel
        if (fl_bluetooth){
	    SendCommStop_ave_BT();
            break;
        }


	BStop->setDown(0);
	if(fl_play)fl_play=0;
	if(fl_rec){
	    e_tim_comm = GetTime();
	    if (numc>(NUM_COMM-2)) break;
	    mas_comm[numc]=9;
	    tim_comm[numc] = (e_tim_comm-b_tim_comm);
	    numc++;
	    
//	    break;
	}
	PressRh=0;
	PressLf=0;
	PressRv=0;
	PressFw=0;
	SendCommStop1();
	break;

    }


    
}

void Vench::BShiftRel()
{
	if (BShift->isChecked()==0) shiftPress =0;
//	printf("sh = %d\n",shiftPress);
}


int Vench::GetNumbUst(int *mac)
{
    int i,j;
    int fl_it;


    fl_it=0;
///    qDebug("GetNumbUst: kol_ust=%d\n",kol_ust);
    for (i=0;i<kol_ust;i++){
        fl_it =1;
        for(j=0;j<8;j++){
            if(mas_mac_addr[i][j]!=mac[j]) fl_it =0;
        }
        if (fl_it==1) return i;
    }
//    qDebug("GetNumbUst: fl_it=%d\n",fl_it);
    if (fl_it ==0) return -1;
    if (kol_ust ==0) return -1;
    return -1;
}


void Vench::SetHiddenCar(int i, int fl){

////28_01_14
//    int k;
//    while (fl_hid){
//        k++;
//        if (k>300) ProcessTimer();
//        mSleep(50);
//    }
//    i_hid=i;
//    fl_fl_hid=fl;
//    fl_hid=1;
// end  28_01_14

    mSleep(50);
    mas_hid[k_hid]=i;
    mas_fl_hid[k_hid]=fl;
    k_hid++;


}

//int rr=-1;
void Vench::ReadActiveLog()
{
    FILE* mfl_com;
    QString txt,tt;
    int i;
    int fs;
    char ss;
    int dt;
    int mac_a[8];
//    int set_a[2];
    int newp;
  //  int nump;
    int nkol;
    int nump=-1;
    
//    if(rr>-1) return;
//    rr++;
    dt=0;
    fs=0;
    ss=0;
    newp =0;
    nkol  =0;
    txt= ServerDir->text();
#ifdef LINUX_D
    txt = txt + "/ACTIVE_LOG.txt";
#endif
    
#ifdef WIN_D
    txt = txt + "ACTIVE_LOG.txt";
#endif

    QFileInfo fll(txt);
    QDateTime mtmFile = fll.lastModified() ;
    if (mtmFile.time()==tmFile){
//      printf("ActiveLog ne izmenalsa!!!, return\n");
	
        return;
    }
//    printf("read active log\n");
//    qDebug("read active log\n");
    tmFile = mtmFile.time();
    
    qDebug()<<txt;
    mfl_com = fopen(txt.toLocal8Bit(),"r+");
//    mfl_com = fopen(flname,"r+");
    
    
    
    
    if (!mfl_com) {
        qDebug("Error open Active LOG\n");
        return;
    }
  //  	   printf("www\n");
    
    
//    fclose(fl_com);
  //  return;
    
    while(1)
	//  for(int ii;ii<1;ii++)
    {
	for (i=0;i<8;i++){
	    dt =0;
	    fs = fscanf(mfl_com,"%x",&dt);
//	    	    printf("%x  %d\n",dt,fs);
	    if (fs>0) mac_a[i] = dt;
	    //	    if (fs>0) mas_mac_addr[kol_ust][i] = dt;
	    if (fs<=0) {
		break;
	    }
	}
	if (i<8) break;
	if(fs>0){
//	  printf("uiblb\n");
	    nump = GetNumbUst(mac_a);
//	    printf("%d\n",nump);
	}
	else break;
	
	if (nump<0) {
//	    printf("kol_ust = %d\n",kol_ust);
	    for(i=0;i<8;i++){
		 mas_mac_addr[kol_ust][i] = mac_a[i];
		
//		qDebug ("mm0 = %x\n",mac_a[i]);
	    
	    }
	    nump= kol_ust;
	    kol_ust++;
		newp=1;
	}
//    qDebug("@@@@@@@@@@@@@@@\n");

if (newp==1){

//26_09_13

		    if (mkol_ust<MAX_USTR){
			for(i=0;i<8;i++){
			    act_ust[mkol_ust].mac_addr[i] = (unsigned char)(mas_mac_addr[kol_ust-1][i]);	
			}	
			act_ust[mkol_ust].set_addr[0] = 0;
			act_ust[mkol_ust].set_addr[1] = 0;

			act_ust[mkol_ust].parent_addr[0] = 0;
			act_ust[mkol_ust].parent_addr[1] = 0;

            act_ust[mkol_ust].period_oprosa_d = 0;
            act_ust[mkol_ust].period_oprosa_o = 0;
			act_ust[mkol_ust].fl_izm_pd = 0;
			act_ust[mkol_ust].period_oprosa_t = 0;
			act_ust[mkol_ust].fl_izm_pt = 0;
			act_ust[mkol_ust].period_oprosa_e = 0;
			act_ust[mkol_ust].fl_izm_pe = 0;
			act_ust[mkol_ust].tar_kf = TAR_KF;
			act_ust[mkol_ust].pan1 = 0;
			act_ust[mkol_ust].pan2 = 0;
        		act_ust[mkol_ust].fl_pan = 0;

#ifdef DVA95        		
        		act_ust[mkol_ust].fl_antisleep = 0;
#endif
#ifndef DVA95        		
        		act_ust[mkol_ust].fl_antisleep = 2;
#endif
        		
    		
            		if (mkol_ust==0)act_ust[mkol_ust].fl_antisleep = 2;
            		if (mkol_ust==1)act_ust[mkol_ust].fl_antisleep = 2;




        		act_ust[mkol_ust].impr = def_diskr_impr;
			act_ust[mkol_ust].impl = def_diskr_impl;
    
			
				
			act_ust[mkol_ust].fl_ans_sleep=0;
			act_ust[mkol_ust].fl_sleep=1;

        		act_ust[mkol_ust].vibr_sp=0;

			act_ust[mkol_ust].time_sleep=0;
			act_ust[mkol_ust].p_sleep=0;
			act_ust[mkol_ust].time_oprosa_t=0;
			act_ust[mkol_ust].time_oprosa_e=0;
			act_ust[mkol_ust].fl_oprosa =0;
			act_ust[mkol_ust].fl_temp =0;
			act_ust[mkol_ust].fl_energ =0;
			act_ust[mkol_ust].num_u =0;


			mkol_ust++;
//			SetActiveLog(act_ust[mkol_ust-1].mac_addr, act_ust[mkol_ust-1].set_addr);
			ReadUstrINI(act_ust[mkol_ust-1].mac_addr);// pitaemsa chitati iz file zapomnenie ranee nastroiki (v chastnosti kol imp diskretnogo dvig)

		    }


}

	if (fs<=0) {
	    break;
	    
	}
	fs = fscanf(mfl_com,"%s",&ss);
	if (fs<=0) break;
	
	for (i=0;i<2;i++){
	    fs = fscanf(mfl_com,"%x",&dt);
	    if (fs>0){ 
		if (mas_set_addr[nump][i] != dt)
		{
		    mas_set_addr[nump][i] = dt;
		    newp=1;
		}
//		if (newp==1){
//			SendCommAntiSon(mas_mac_addr[kol_ust],mas_set_addr[nump]);	    
//		}	
	    }
	    if (fs<=0) break;
	}
	
	if (fs<=0) break;
//	printf("tt1\n");
	fs = fscanf(mfl_com,"%s",&ss);
	if (fs<=0) break;
//	printf("tt2\n");
	fs = fscanf(mfl_com,"%x",&dt);
	if (fs<=0) break;
//	printf("tt3\n");
	
	fs = fscanf(mfl_com,"%x",&dt);
	if (fs<=0) break;
//	printf("tt4\n");

	fs = fscanf(mfl_com,"%s",&ss);
	if (fs<=0) break;
//	printf("tt5\n");

	nkol++;
		
    }   
// printf("Fclose\n");
    fclose(mfl_com);
    mfl_com = NULL;
  //    printf("kol_ust = %d\n",kol_ust);
//       printf("nkol = %d\n",nkol);
       
       
       
    if ((newp==0)&&(kol_ust==nkol)) {
	if ( SpActDatch->count()!=0) return;
	if ( kol_ust==0) return;
	
    }
   
     if (kol_ust!=nkol) kol_ust=0;
     SpActDatch->clear();
      listV->clear();
    if (kol_ust>0){
	for (i=0;i<kol_ust;i++){

        if (act_ust[i].vibr_sp==1) txt =" ";
        else txt = " ";

	    tt.setNum(i+1,10);
        txt = txt+tt+" : ";
	    for(int j=0;j<8;j++){
		tt.setNum(mas_mac_addr[i][j],16);
		txt = txt + tt+" ";
	    }
	    txt = txt + "; ";
	    for(int j=0;j<2;j++){
		tt.setNum(mas_set_addr[i][j],16);
		txt = txt + tt+" ";
	    }
	    txt=txt+trUtf8("; может спать; ");
	    
	    
	    tt.setNum(act_ust[i].impl,10);
	    txt = txt + tt+", ";
	    tt.setNum(act_ust[i].impr,10);
	    txt = txt + tt;
	    
	    SpActDatch->insertItem(MAX_USTR,txt);
        listV->addItem(txt);
        ui->listV->addItem(txt);


        SetHiddenCar(i,act_ust[i].fl_antisleep);


	}		
	if (kol_ust>0)SpActDatch->setCurrentIndex(0);
    }               

    //zachem&&&&&&&&????????????????
//  SendCommSon();
    
}



int Vench::RemoveUstr(int  k)
{
    if (k<mkol_ust){
        for (int i=k;i<mkol_ust-1;i++){



            for(int ii=0;ii<8;ii++){
                act_ust[i].mac_addr[ii] = act_ust[i+1].mac_addr[ii];
            }
            act_ust[i].set_addr[0] = act_ust[i+1].set_addr[0];
            act_ust[i].set_addr[1] = act_ust[i+1].set_addr[1];

            act_ust[i].parent_addr[0] = act_ust[i+1].parent_addr[0];
            act_ust[i].parent_addr[1] = act_ust[i+1].parent_addr[1];

            act_ust[i].period_oprosa_d = act_ust[i+1].period_oprosa_d ;
            act_ust[i].fl_izm_pd = act_ust[i+1].fl_izm_pd;
            act_ust[i].period_oprosa_t = act_ust[i+1].period_oprosa_t ;
            act_ust[i].fl_izm_pt = act_ust[i+1].fl_izm_pt;
            act_ust[i].period_oprosa_e = act_ust[i+1].period_oprosa_e;
            act_ust[i].period_oprosa_o = act_ust[i+1].period_oprosa_o;
            act_ust[i].fl_izm_pe = act_ust[i+1].fl_izm_pe;
            act_ust[i].tar_kf = act_ust[i+1].tar_kf;
            act_ust[i].pan1 = act_ust[i+1].pan1;
            act_ust[i].pan2 = act_ust[i+1].pan2;
            act_ust[i].fl_pan = act_ust[i+1].fl_pan;
            act_ust[i].impr = act_ust[i+1].impr;
            act_ust[i].impl = act_ust[i+1].impl;

	    act_ust[i].fl_antisleep =  act_ust[i+1].fl_antisleep;



            act_ust[i].fl_ans_sleep=act_ust[i+1].fl_ans_sleep;
            act_ust[i].fl_sleep=act_ust[i+1].fl_sleep;

            act_ust[i].vibr_sp=act_ust[i+1].vibr_sp;

            act_ust[i].time_sleep=act_ust[i+1].time_sleep;
            act_ust[i].p_sleep=act_ust[i+1].p_sleep;
            act_ust[i].time_oprosa_t=act_ust[i+1].time_oprosa_t;
            act_ust[i].time_oprosa_e=act_ust[i+1].time_oprosa_e;
            act_ust[i].fl_oprosa =act_ust[i+1].fl_oprosa;
            act_ust[i].fl_temp =act_ust[i+1].fl_temp;
            act_ust[i].fl_energ =act_ust[i+1].fl_energ;
            act_ust[i].num_u =act_ust[i+1].num_u;




        }

        if (mkol_ust>0){
            mkol_ust--;
        }

        for(int i=k;i<kol_ust-1;i++){
            for(int j=0;j<8;j++){
                mas_mac_addr[i][j] = mas_mac_addr[i+1][j];
            }
            for(int j=0;j<2;j++){
                mas_set_addr[i][j] = mas_set_addr[i+1][j];
            }
        }

        if(kol_ust>0) kol_ust--;
        return 1;
    }
    return 0;
}


void Vench::ClearSp()
{
    //this->SpActDatch->clear();
    if ((kol_ust>0)&&(fl_vibr_ust==1)) fl_clsp=1;
    fl_obn_spact=1;
}

void Vench::SetSp(QString txt)
{
    txtAct = txt;
//        this->SpActDatch->insertItem(1,txt);
//	if (kol_ust>0) this->SpActDatch->setCurrentIndex(0);
    fl_obn_spact=2;
}

void Vench::SetActiveLog(unsigned char *mac_ad, unsigned char*  mas_set)
{

    QString txt,tt;
    int i;
////    int fs;
//    char ss;
//    int dt;
    int newp;
//    int nkol;
    int nump;
    int mac_a[8];
//    unsigned char mac_am[8];

//        
//    dt=0;
//    ss=0;
    newp =0;
//    nkol  =0;
//    
    nump=-1;
    for(i=0;i<8;i++){
	mac_a[i] = mac_ad[i];
    }
    nump = GetNumbUst(mac_a);
	
    if (nump<0) {
/////	    printf("kol_ust = %d\n",kol_ust);
	    for(i=0;i<8;i++) {
		mas_mac_addr[kol_ust][i] = mac_a[i];
//		mac_am[i] = mac_a[i];
	    }
	    nump= kol_ust;
	    kol_ust++;
	    newp=1;
    }
    if (newp==0){
    
//	    for(i=0;i<8;i++) {
//        	mac_am[i] = mas_mac_addr[nump][i];
//	    }
	
	
    }

    for(i=0;i<2;i++){
        if (mas_set_addr[nump][i] != mas_set[i])
        {
		    mas_set_addr[nump][i] = mas_set[i];
		    newp=1;
        }
    }
    
    //qDebug("setactivelog :newp = %d\n",newp);
    if (newp==1){
//moget ne budet cherez 5 sec zasipat???	    

        
	if (act_ust[nump].fl_antisleep ==0) {
    	    qDebug() << "Send comm anti son";
           // SendZaprosSon(mas_mac_addr[nump],mas_set_addr[nump]);
            SendCommAntiSon(mas_mac_addr[nump],mas_set_addr[nump]);

#ifdef DVA95        		
	    act_ust[nump].fl_antisleep =1;
#endif
#ifndef DVA95        		
	    act_ust[nump].fl_antisleep =2;
#endif

            
	}
        //mSleep(50);
	    
        //20_12_13
        // ubrana zapros pan id ustr,  vremenno???
        //SendZaprosPANID(mac_am);


    
    }


//		
//       
////    if ((newp==0)&&(kol_ust==nkol)) {
//
    if ((newp==0)) {// esli nichego novogo net
////	printf("break\n");
////	printf("Sp - c = %d\n",SpActDatch->count());
        if (kol_ust>0){

//    	    QPalette pl(Qt::red,Qt::red);
//	    pl.setColor(QPalette::Base,Qt::green);
//	    TLabAns->setPalette(pl);
//	    TLabAns->setText(ans3);
            //fl_ans3_ust=1;
        }

        if ( SpActDatch->count()!=0) return;
        if ( kol_ust==0) return;
	
    }

    ClearSp();
    ClearSpBeg();

    if (kol_ust>0){
	
        ClearSp();
	    while(   fl_obn_spact>0) mSleep(10);
    
        //fl_ans3_ust=1;
	for (i=0;i<kol_ust;i++){
	    if(act_ust[i].fl_antisleep==2){
		if (fl_ans3_ust!=1)fl_ans3_ust=1;
	    }

    	    if (act_ust[i].vibr_sp==1) txt =" ";
    	    else txt = " ";

    	    tt.setNum(i+1,10);
    	    txt = txt+ tt+" : ";
	    for(int j=0;j<8;j++){
		tt.setNum(mas_mac_addr[i][j],16);
		txt = txt + tt+" ";
	    }
	    txt = txt + "; ";
	    for(int j=0;j<2;j++){
		tt.setNum(mas_set_addr[i][j],16);
		txt = txt + tt+" ";
	    }
	    
	    if (act_ust[i].fl_sleep==0) txt=txt+trUtf8("; без сна; ");
	    if (act_ust[i].fl_sleep==1) txt=txt+trUtf8("; может спать; ");

	    
//	    qDebug("qQQQQQQQQQQQQQQQQQQQQq = %d\n",act_ust[i].vibr_sp);
	    
	    tt.setNum(act_ust[i].impl,10);
	    txt = txt + tt+", ";
	    tt.setNum(act_ust[i].impr,16);
	    txt = txt + tt;
	    
	    
//	    SpActDatch->insertItem(0,txt,Qt::UserRole);
//	    SpActDatch->insertItem(txt,Qt::UserRole);


	    
//	    QByteArray rr = txt.toLocal8Bit();
//	    printf(rr.constData());
//	    printf("\n");
	    
	    SetSp(txt);




	    while(   fl_obn_spact>0) mSleep(6);
	}
	fl_obn_spact=4;		
    }               
    
}


void Vench::BShiftPress()
{
    if (BShift->isChecked()== true) shiftPress =1;
	
    if (BShift->isChecked()==false) shiftPress =0;
//	printf("sh pr = %d\n",shiftPress);
//	printf("d = %d\n",BShift->isDown());

}


void Vench::NewScriptPlot(int * mac, int pr)
{
    FILE* fl_com;
    QString txt;
    QString flm,mm,tt;
    int i;
    
    QString fnamec;

    fnamec= ServerDir->text();
    #ifdef LINUX_D
//    fnamec= fnamec+"/";
    fnamec= fnamec+NAME_SCRIPT_FILE;
    #endif
    #ifdef WIN_D
//    fnamec= fnamec+"\\";
    fnamec= fnamec+NAME_SCRIPT_FILE;
    #endif
    
    QByteArray rr =  fnamec.toLocal8Bit();
//	    printf(rr.constData());
    
    
    fl_com = fopen(rr.constData(),"w+");

    flm= ServerDir->text()+"DATA";
    
    mm ="";
    //preobrazuem mac addr v text
    for (i=0;i<8;i++){
        tt.setNum(mac[i],16);
        mm = mm+tt;
    }
//    printf(mm);
    
    flm = flm+mm+".txt";
    
	    
    txt = "set terminal png size 1200, 800";
    
     rr = txt.toLocal8Bit();
    
    
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
//    tt = NAME_GRPH_FILE;

    fnamec= ServerDir->text();
    #ifdef LINUX_D
//    fnamec= fnamec+"/";
        if (pr==0) fnamec= fnamec+NAME_GRPH_FILE;
        if (pr==1) fnamec= fnamec+NAME_GRPH_FILE_EN;
        if (pr==2) fnamec= fnamec+NAME_GRPH_FILE_OS;
        if (pr==3) fnamec= fnamec+NAME_GRPH_FILE_DM;
        if (pr==4) fnamec= fnamec+NAME_GRPH_FILE_BAR;
        if (pr==5) fnamec= fnamec+NAME_GRPH_FILE_VLG;

    #endif
    #ifdef WIN_D
//    fnamec= fnamec+"\\";
        if (pr==0) fnamec= fnamec+NAME_GRPH_FILE;
        if (pr==1) fnamec= fnamec+NAME_GRPH_FILE_EN;
        if (pr==2) fnamec= fnamec+NAME_GRPH_FILE_OS;
        if (pr==3) fnamec= fnamec+NAME_GRPH_FILE_DM;
        if (pr==4) fnamec= fnamec+NAME_GRPH_FILE_BAR;
        if (pr==5) fnamec= fnamec+NAME_GRPH_FILE_VLG;
    fnamec.replace("\\","\\\\");
    #endif
    
    tt = fnamec;


    txt = "set output \"" + tt+ "\"";
    rr = txt.toLocal8Bit();
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    txt = "set encoding koi8r";
    rr = txt.toLocal8Bit();
    
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    txt = "set xdata time";
    rr = txt.toLocal8Bit();
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    txt = "set datafile separator \";\""; 
    rr = txt.toLocal8Bit();
    
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    txt = "set timefmt \"%H:%M:%S\"";
    rr = txt.toLocal8Bit();
    
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    txt = "set format x \"%H:%M\"";
    rr = txt.toLocal8Bit();
    
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n")
    ;
    if (pr==0) txt = "set ylabel \"(C)\"";
    if (pr==1) txt = "set ylabel \"(V)\"";
    if (pr==2) txt = "set ylabel \"(O)\"";
    if (pr==3) txt = "set ylabel \"(D)\"";
    if (pr==4) txt = "set ylabel \"(kPa)\"";
    if (pr==5) txt = "set ylabel \"(vlg)\"";
    rr = txt.toLocal8Bit();
    
    
//    fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    

//bez mac-adresa
//    if (pr==0)    txt = "plot \""+flm+"\" using 2:6  with lines lw 4  title \"Temperatura\"";
//  if (pr==1)    txt = "plot \""+flm+"\" using 2:7  with lines  lw 4 title \"Volt\"";

    //dobavlaem mac adres v zagolovok

//    qDebug()<<flm;

    if (pr==0)    txt = "plot \""+flm+"\" using 2:6  with lines lw 4  title \"Temperatura " +mm+"\"";
    if (pr==1)    txt = "plot \""+flm+"\" using 2:7  with lines  lw 4 title \"Volt "+mm+"\"";
    if (pr==2)    txt = "plot \""+flm+"\" using 2:8  with lines  lw 4 title \"Osv "+mm+"\"";
    if (pr==3)    txt = "plot \""+flm+"\" using 2:9  with lines  lw 4 title \"Dim "+mm+"\"";
    if (pr==4)    txt = "plot \""+flm+"\" using 2:9  with lines  lw 4 title \"Bar "+mm+"\"";
    if (pr==5)    txt = "plot \""+flm+"\" using 2:9  with lines  lw 4 title \"Vlg "+mm+"\"";

#ifdef WIN_D
    txt.replace("\\","\\\\");
#endif

    rr = txt.toLocal8Bit();
   
//   fprintf(fl_com,rr.constData());
    fprintf(fl_com,"%s",rr.constData());
    fprintf(fl_com,"\n");
    fprintf(fl_com,"refresh\n");

    fclose(fl_com);
}


void Vench::BPressSendEth()
{
    unsigned char buf[1024];
    const char *mb;
    char ss[5];
    int len;
    QString st   = TLEEthKgd->text()+" ";
    len = 0;
    QByteArray rr =  st.toLocal8Bit();
    mb = rr.constData();

    int fs =0;
    QString   ttSS;

    while(1){
	fs = sscanf(mb,"%s",ss);
	if (fs<0) break;
	ttSS =ss;
	bool ok;
	int k = ttSS.toInt(&ok,16);
        if (ok== true){
    	    buf[len] = k;
    	    len++;
        }
        mb +=(ttSS.length()+1);
//	qDebug("l = %d",ttSS.length());
    }
    
    #ifdef LINUX_D
//    qDebug("Send Eth len = %d\n",len);
//    for(int i=0;i<len;i++) qDebug("%x ",buf[i]);
    channel_stdput(&channel_drone, (void*) buf, len);
    #endif
}


void Vench::BPressSendEthAT()
{
    #ifdef LINUX_D

    QString st;
    
    st = "AT*"+TLEEthKgdAT->text() +"="   ;
    
    QString st1,st2;
    
    st2 = TLEEthKgdAT_CH->text();
    st1.setNum (st2.toInt(),10);//??????????? - moget 2
//    qDebug ()<<st1;
    st = st+st1 + ",";

    st2 = TLEEthKgdAT_CH2->text();
    st1.setNum (st2.toInt(),10);//
    qDebug ()<<st1;
    st = st+st1+"\n";
    send_command(st);
    #endif
}


void Vench::BPressGrph()
{
    QString mac,ms,tt;
    int mk[8];
    int i,j;
    
    if ((BGrph->isChecked()==false)&&(ui->BGrph_n->isChecked()==false)) {
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        pr_grph =0;
        return;
    }

    if ((pr_grph!=1)&&(pr_grph>0)){
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
	BGrphE->setChecked(0);
        BGrphVl->setChecked(0);
        BGrphO->setChecked(0);
	BGrphD->setChecked(0);
	BGrphB->setChecked(0);

	ui->BGrphE_n->setChecked(0);
        ui->BGrphVl_n->setChecked(0);
        ui->BGrphO_n->setChecked(0);
	ui->BGrphD_n->setChecked(0);
	ui->BGrphB_n->setChecked(0);
	Plot_size_tecT=0;
	Plot_size_tecV=0;
	Plot_size_tecE=0;
	Plot_size_tecD=0;
    }
/* woronin    cRight->setRawData(d_x, d_yt, Plot_size_tecT);
    cRight->setPen(QPen(Qt::red));

    cRight_n->setRawData(d_x, d_yt, Plot_size_tecT);
    cRight_n->setPen(QPen(Qt::red));
    Mgrph->setAxisTitle(QwtPlot::yLeft, (trUtf8( "градус")));
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, (trUtf8( "градус")));

    ind_tab = tabWidget2->insertTab(4, tab_6, (trUtf8( "График температуры")) );
    ind_tab_n = ui->tabWidget_2->insertTab(4, tab_6_n, (trUtf8( "График температуры")) );
    pr_grph =1;
    Mgrph->replot();
    Mgrph_n->replot(); */


    return ;
    // vremenno !!! ubrat to chto nige!!!
    
    
    j=0;
    mac =  SpActDatch->currentText();
    int numu = listV->count();
     if ((checkVibrUst->isChecked() ==  true)){//���� ���������� ������� ��������� ����������

         for(j=0;j<numu;j++){
             if (mas_vibr_ust[j]==1){
                 for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                 }
                 break;
            }
         }
         if (j>=numu){
             return;
         }
     }else{//���� �� ���������� ������� ��������� ����������
         if (numu>0){
             for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
             }
         }else return;
     }

     
    NewScriptPlot(mk,0);

    tt= ServerDir->text();
    tt = tt+NAME_SCRIPT_FILE;
#ifdef LINUX_D
    ms = "gnuplot \"" + tt+"\"";

    qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    i = system(rr.constData());

//    sleep(1);

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE;
    ms = "eog " + tt + " &";
    qDebug()<<ms;
    rr = ms.toLocal8Bit();
    i =system(rr.constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH=="") ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " +tt;
    else {
        ms = progrPostrGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }


    QByteArray rr = ms.toLocal8Bit();
    qDebug()<<rr;

    WinExec(rr,SW_RESTORE)   ;// zapusk gnuplot

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE;

    //tt.replace("\\","\\\\");

    if (progrShowGRPH=="") ms = "C:\\Windows\\System32\\mspaint.exe " +tt;
    else {
        ms = progrShowGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }
    rr = ms.toLocal8Bit();

    qDebug()<<rr;
    WinExec(rr,SW_RESTORE);// zapusk file


#endif
}

void Vench::BPressGrphE()
{
    QString mac,ms,tt;
    int mk[8];
    int i,j;


    if ((BGrphE->isChecked()==false)&&(ui->BGrphE_n->isChecked()==false)) {
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        pr_grph =0;
        return;
    }

    if ((pr_grph!=2)&&(pr_grph>0)){
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
	BGrph->setChecked(0);
        BGrphVl->setChecked(0);
	BGrphO->setChecked(0);
	BGrphD->setChecked(0);
	BGrphB->setChecked(0);
	
	ui->BGrph_n->setChecked(0);
        ui->BGrphVl_n->setChecked(0);
	ui->BGrphO_n->setChecked(0);
	ui->BGrphD_n->setChecked(0);
	ui->BGrphB_n->setChecked(0);
	
	Plot_size_tecT=0;
	Plot_size_tecD=0;
	Plot_size_tecV=0;
	Plot_size_tecE=0;
    }
/*woronin    cRight->setRawData(d_x, d_ye, Plot_size_tecE);
    cRight_n->setRawData(d_x, d_ye, Plot_size_tecE);
//    cRight->setPen(QPen(Qt::yellow));
    cRight->setPen(QPen(Qt::red));
    cRight_n->setPen(QPen(Qt::red));
    Mgrph->setAxisTitle(QwtPlot::yLeft, (trUtf8( "Volt")));
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, (trUtf8( "Volt")));
    ind_tab = tabWidget2->insertTab(4, tab_6, (trUtf8( "График энергии")) );
    ind_tab_n = ui->tabWidget_2->insertTab(4, tab_6_n, (trUtf8( "График энергии")) );
    pr_grph =2;
    Mgrph->replot();
    Mgrph_n->replot(); */

    return ;
    // vremenno !!! ubrat to chto nige!!!

    j=0;
    mac =  SpActDatch->currentText();
    int numu = listV->count();
     if ((checkVibrUst->isChecked() ==  true)){

         for(j=0;j<numu;j++){
             if (mas_vibr_ust[j]==1){
                 for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                 }
                 break;
            }
         }
         if (j>=numu){
             return;
         }
     }else{
         if (numu>0){
             for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
             }
         }else return;
     }

     
    NewScriptPlot(mk,1);

    tt= ServerDir->text();
    tt = tt+NAME_SCRIPT_FILE;
#ifdef LINUX_D
    ms = "gnuplot \"" + tt+"\"";

    qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    i = system(rr.constData());

//    sleep(1);

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_EN;
    ms = "eog " + tt + " &";
    qDebug()<<ms;
    rr = ms.toLocal8Bit();
    i =system(rr.constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH=="") ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " +tt;
    else {
        ms = progrPostrGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }


    QByteArray rr = ms.toLocal8Bit();
    qDebug()<<rr;

    WinExec(rr,SW_RESTORE)   ;// zapusk gnuplot

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_EN;

    //tt.replace("\\","\\\\");

    if (progrShowGRPH=="") ms = "C:\\Windows\\System32\\mspaint.exe " +tt;
    else {
        ms = progrShowGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }
    rr = ms.toLocal8Bit();

    qDebug()<<rr;
    WinExec(rr,SW_RESTORE);// zapusk file


#endif
}
void Vench::BPressGrphD()
{
    QString mac,ms,tt;
    int mk[8];
    int i,j;

    if ((BGrphD->isChecked()==false)&&(ui->BGrphD_n->isChecked()==false)) {
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        pr_grph =0;
        return;
    }

    if ((pr_grph!=4)&&(pr_grph>0)){
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        BGrph->setChecked(0);
        BGrphVl->setChecked(0);
        BGrphO->setChecked(0);
	BGrphE->setChecked(0);
	BGrphB->setChecked(0);
	
        ui->BGrph_n->setChecked(0);
        ui->BGrphVl_n->setChecked(0);
        ui->BGrphO_n->setChecked(0);
	ui->BGrphE_n->setChecked(0);
	ui->BGrphB_n->setChecked(0);
	
	Plot_size_tecT=0;
	
	Plot_size_tecD=0;
	Plot_size_tecE=0;
	Plot_size_tecV=0;
    }
/*    cRight->setRawData(d_x, d_yd, Plot_size_tecD);
    cRight_n->setRawData(d_x, d_yd, Plot_size_tecD);
//    cRight->setPen(QPen(Qt::magenta));
    cRight->setPen(QPen(Qt::red));
    cRight_n->setPen(QPen(Qt::red));
    Mgrph->setAxisTitle(QwtPlot::yLeft, (trUtf8( "Values, ..")));
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, (trUtf8( "Values, ..")));
    ind_tab = tabWidget2->insertTab(4, tab_6, (trUtf8( "График задымленности")) );
    ind_tab_n = ui->tabWidget_2->insertTab(4, tab_6_n, (trUtf8( "График задымленности")) );
    pr_grph =4;
    Mgrph->replot();
    Mgrph_n->replot(); */

    return ;
    // vremenno !!! ubrat to chto nige!!!

    j=0;
    mac =  SpActDatch->currentText();
    int numu = listV->count();
     if ((checkVibrUst->isChecked() ==  true)){

         for(j=0;j<numu;j++){
             if (mas_vibr_ust[j]==1){
                 for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                 }
                 break;
            }
         }
         if (j>=numu){
             return;
         }
     }else{
         if (numu>0){
             for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
             }
         }else return;
     }


    NewScriptPlot(mk,3);

    tt= ServerDir->text();
    tt = tt+NAME_SCRIPT_FILE;
#ifdef LINUX_D
    ms = "gnuplot \"" + tt+"\"";

    qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    i = system(rr.constData());

//    sleep(1);

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_DM;
    ms = "eog " + tt + " &";
    qDebug()<<ms;
    rr = ms.toLocal8Bit();
    i =system(rr.constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH=="") ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " +tt;
    else {
        ms = progrPostrGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }


    QByteArray rr = ms.toLocal8Bit();
    qDebug()<<rr;

    WinExec(rr,SW_RESTORE)   ;// zapusk gnuplot

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_DM;

    //tt.replace("\\","\\\\");

    if (progrShowGRPH=="") ms = "C:\\Windows\\System32\\mspaint.exe " +tt;
    else {
        ms = progrShowGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }
    rr = ms.toLocal8Bit();

    qDebug()<<rr;
    WinExec(rr,SW_RESTORE);// zapusk file


#endif
}
void Vench::BPressGrphB()
{
    QString mac,ms,tt;
    int mk[8];
    int i,j;

    if ((BGrphB->isChecked()==false)&&(ui->BGrphB_n->isChecked()==false)) {
        tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        pr_grph =0;
        return;
    }

    if ((pr_grph!=5)&&(pr_grph>0)){
        tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        BGrph->setChecked(0);
        BGrphVl->setChecked(0);
        BGrphO->setChecked(0);
        BGrphD->setChecked(0);
        BGrphE->setChecked(0);

        ui->BGrph_n->setChecked(0);
        ui->BGrphVl_n->setChecked(0);
        ui->BGrphO_n->setChecked(0);
        ui->BGrphD_n->setChecked(0);
        ui->BGrphE_n->setChecked(0);
        
        Plot_size_tecT=0;
        Plot_size_tecV=0;
        Plot_size_tecD=0;
        Plot_size_tecE=0;
    }
 /*   cRight->setRawData(d_x, d_yb, Plot_size_tecV);
    cRight_n->setRawData(d_x, d_yb, Plot_size_tecV);
//    cRight->setPen(QPen(Qt::gray));
    cRight->setPen(QPen(Qt::red));
    cRight_n->setPen(QPen(Qt::red));
    Mgrph->setAxisTitle(QwtPlot::yLeft, (trUtf8( "кПа")));
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, (trUtf8( "кПа")));
    ind_tab = tabWidget2->insertTab(4, tab_6, (trUtf8( "График давления")) );
    ind_tab_n = ui->tabWidget_2->insertTab(4, tab_6_n, (trUtf8( "График давления")) );
    pr_grph =5;
    Mgrph->replot();
    Mgrph_n->replot(); */
    return ;
    // vremenno !!! ubrat to chto nige!!!

    j=0;
    mac =  SpActDatch->currentText();
    int numu = listV->count();
    if ((checkVibrUst->isChecked() ==  true)){

    for(j=0;j<numu;j++){
             if (mas_vibr_ust[j]==1){
                 for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                 }
                 break;
            }
         }
         if (j>=numu){
             return;
         }
    }else{
         if (numu>0){
             for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
             }
         }else return;
    }


    NewScriptPlot(mk,4);

    tt= ServerDir->text();
    tt = tt+NAME_SCRIPT_FILE;
#ifdef LINUX_D
    ms = "gnuplot \"" + tt+"\"";

    qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    i = system(rr.constData());

//    sleep(1);

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_BAR;
    ms = "eog " + tt + " &";
    qDebug()<<ms;
    rr = ms.toLocal8Bit();
    i =system(rr.constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH=="") ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " +tt;
    else {
        ms = progrPostrGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }


    QByteArray rr = ms.toLocal8Bit();
    qDebug()<<rr;

    WinExec(rr,SW_RESTORE)   ;// zapusk gnuplot

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_BAR;

    //tt.replace("\\","\\\\");

    if (progrShowGRPH=="") ms = "C:\\Windows\\System32\\mspaint.exe " +tt;
    else {
        ms = progrShowGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }
    rr = ms.toLocal8Bit();

    qDebug()<<rr;
    WinExec(rr,SW_RESTORE);// zapusk file
#endif
}



void Vench::BPressGrphV()
{
    QString mac,ms,tt;
    int mk[8];
    int i,j;

    if ((BGrphVl->isChecked()==false)&&(ui->BGrphVl_n->isChecked()==false)) {
        tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        pr_grph =0;
        return;
    }

    if ((pr_grph!=6)&&(pr_grph>0)){
        tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);

        BGrph->setChecked(0);
        BGrphO->setChecked(0);
        BGrphD->setChecked(0);
        BGrphE->setChecked(0);
        BGrphB->setChecked(0);

        ui->BGrph_n->setChecked(0);
        ui->BGrphO_n->setChecked(0);
        ui->BGrphD_n->setChecked(0);
        ui->BGrphE_n->setChecked(0);
        ui->BGrphB_n->setChecked(0);

        Plot_size_tecT=0;

        Plot_size_tecV=0;

        Plot_size_tecE=0;

        Plot_size_tecD=0;
    }
/*    cRight->setRawData(d_x, d_yv, Plot_size_tecV);
    cRight_n->setRawData(d_x, d_yv, Plot_size_tecV);
//    cRight->setPen(QPen(Qt::gray));
    cRight->setPen(QPen(Qt::red));
    cRight_n->setPen(QPen(Qt::red));
    Mgrph->setAxisTitle(QwtPlot::yLeft, (tr( "���������,%")));
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, (tr( "���������,%")));
    ind_tab = tabWidget2->insertTab(4, tab_6, (trUtf8( "График влажности")) );
    ind_tab_n = ui->tabWidget_2->insertTab(4, tab_6_n, (trUtf8( "График влажности")) );
    pr_grph =6;
    Mgrph->replot();
    Mgrph_n->replot(); */
    return ;
    // vremenno !!! ubrat to chto nige!!!

    j=0;
    mac =  SpActDatch->currentText();
    int numu = listV->count();
    if ((checkVibrUst->isChecked() ==  true)){

    for(j=0;j<numu;j++){
             if (mas_vibr_ust[j]==1){
                 for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                 }
                 break;
            }
         }
         if (j>=numu){
             return;
         }
    }else{
         if (numu>0){
             for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
             }
         }else return;
    }


    NewScriptPlot(mk,5);

    tt= ServerDir->text();
    tt = tt+NAME_SCRIPT_FILE;
#ifdef LINUX_D
    ms = "gnuplot \"" + tt+"\"";

    qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    i = system(rr.constData());

//    sleep(1);

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_VLG;
    ms = "eog " + tt + " &";
    qDebug()<<ms;
    rr = ms.toLocal8Bit();
    i =system(rr.constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH=="") ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " +tt;
    else {
        ms = progrPostrGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }


    QByteArray rr = ms.toLocal8Bit();
    qDebug()<<rr;

    WinExec(rr,SW_RESTORE)   ;// zapusk gnuplot

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_VLG;

    //tt.replace("\\","\\\\");

    if (progrShowGRPH=="") ms = "C:\\Windows\\System32\\mspaint.exe " +tt;
    else {
        ms = progrShowGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }
    rr = ms.toLocal8Bit();

    qDebug()<<rr;
    WinExec(rr,SW_RESTORE);// zapusk file
#endif
}//BPressGrphV()




void Vench::BPressGrphO()
{
    QString mac,ms,tt;
    int mk[8];
    int i,j;

    if ((BGrphO->isChecked()==false)&&(ui->BGrphO_n->isChecked()==false)) {
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);
        pr_grph =0;
        return;
    }

    if ((pr_grph!=3)&&(pr_grph>0)){
	tabWidget2->removeTab(ind_tab);
	ui->tabWidget_2->removeTab(ind_tab_n);

	BGrph->setChecked(0);
	BGrphE->setChecked(0);
	BGrphD->setChecked(0);
	BGrphB->setChecked(0);
        BGrphVl->setChecked(0);

        ui->BGrphVl_n->setChecked(0);
	ui->BGrph_n->setChecked(0);
	ui->BGrphE_n->setChecked(0);
	ui->BGrphD_n->setChecked(0);
	ui->BGrphB_n->setChecked(0);

	Plot_size_tecT=0;
	Plot_size_tecV=0;
	Plot_size_tecD=0;
	Plot_size_tecE=0;
    }
/* woronin   cRight->setRawData(d_x, d_yo, Plot_size_tecT);
    cRight_n->setRawData(d_x, d_yo, Plot_size_tecT);
//    cRight->setPen(QPen(Qt::green));
    cRight->setPen(QPen(Qt::red));
    cRight_n->setPen(QPen(Qt::red));
    Mgrph->setAxisTitle(QwtPlot::yLeft, (trUtf8("Values, Л")));
    Mgrph_n->setAxisTitle(QwtPlot::yLeft, (trUtf8("Values, Л")));
    ind_tab = tabWidget2->insertTab(4, tab_6, (trUtf8( "График освещенности")) );
    ind_tab_n = ui->tabWidget_2->insertTab(4, tab_6_n, (trUtf8( "График освещенности")) );
    pr_grph =3;
    Mgrph->replot();
    Mgrph_n->replot();*/

    return ;
    // vremenno !!! ubrat to chto nige!!!


    j=0;
    mac =  SpActDatch->currentText();
    int numu = listV->count();
     if ((checkVibrUst->isChecked() ==  true)){

         for(j=0;j<numu;j++){
             if (mas_vibr_ust[j]==1){
                 for(i=0;i<8;i++){
                    mk[i] =mas_mac_addr[j][i];
                 }
                 break;
            }
         }
         if (j>=numu){
             return;
         }
     }else{
         if (numu>0){
             for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
             }
         }else return;
     }


    NewScriptPlot(mk,2);

    tt= ServerDir->text();
    tt = tt+NAME_SCRIPT_FILE;
#ifdef LINUX_D
    ms = "gnuplot \"" + tt+"\"";

    qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    i = system(rr.constData());

//    sleep(1);

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_OS;
    ms = "eog " + tt + " &";
    qDebug()<<ms;
    rr = ms.toLocal8Bit();
    i =system(rr.constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH=="") ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " +tt;
    else {
        ms = progrPostrGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }


    QByteArray rr = ms.toLocal8Bit();
    qDebug()<<rr;

    WinExec(rr,SW_RESTORE)   ;// zapusk gnuplot

    tt = ServerDir->text();
    tt = tt+NAME_GRPH_FILE_OS;

    //tt.replace("\\","\\\\");

    if (progrShowGRPH=="") ms = "C:\\Windows\\System32\\mspaint.exe " +tt;
    else {
        ms = progrShowGRPH+" " + tt;
        ms.replace("\\","\\\\");

    }
    rr = ms.toLocal8Bit();

    qDebug()<<rr;
    WinExec(rr,SW_RESTORE);// zapusk file


#endif
}

//ndt ==1 - print temp
//ndt ==2 - print energ
//ndt ==3 - print osv
//ndt ==4 - print dim
//ndt ==5 - print bar
//ndt ==6 - print vlag
void Vench::PaintTxtData(int ndt,double dt){
    QString sdt;
    sdt.setNum(dt,3,2);
    if (ndt==1){
        if (LTemp) LTemp->setText(sdt);
        if (ui->LTemp_n) ui->LTemp_n->setText(sdt);
    }
    if (ndt==2) if (LEn) LEn->setText(sdt);
    if (ndt==2) if (ui->LEn_n) ui->LEn_n->setText(sdt);
    if (ndt==3) if (LOsv) LOsv->setText(sdt);
    if (ndt==3) if (ui->LOsv_n) ui->LOsv_n->setText(sdt);
    if (ndt==4) if (LDim) LDim->setText(sdt);
    if (ndt==4) if (ui->LDim_n) ui->LDim_n->setText(sdt);
    if (ndt==5) if (LBar) LBar->setText(sdt);
    if (ndt==5) if (ui->LBar_n) ui->LBar_n->setText(sdt);
    if (ndt==6) if (LVl) LVl->setText(sdt);
    if (ndt==6) if (ui->LVl_n) ui->LVl_n->setText(sdt);


}//end painttxtdata

void Vench::setTextReg(){
    QString text;
    text = tr("�����: ���� ��������� ������ 10 �. ");
    if ((BTemp->isChecked()== true)){
        text=text+tr("����������� � ������������ ");
    if ((BDim->isChecked()==false)&&(BEnerg->isChecked()==false)) text=text+"\n ";
    }
    if ((BEnerg->isChecked()== true)){
        if ((BTemp->isChecked()== true)) text=text+"\n ";
        text=text+tr("������� ");
    if ((BDim->isChecked()==false)) text=text+"\n ";
    }
    if ((BDim->isChecked()== true)){
        if ((BEnerg->isChecked()==false)&&(BTemp->isChecked()== true)) text=text+"\n ";
        text=text+tr("������������� \n");
    }
    if((BTemp->isChecked()==false)&&(BEnerg->isChecked()==false)&&(BDim->isChecked()==false)){
        text = tr("�����:  \n");
    }
    TLabReg->setText( text) ;
    ui->LRegGrph->setText(text) ;}

void Vench::BPressDim()
{
    if (ui->BDim_n)if (ui->BDim_n->isChecked()== true)if (BDim) BDim->setChecked( true);
    if (ui->BDim_n) if (ui->BDim_n->isChecked()==false)if (BDim) BDim->setChecked(false);

    if (BDim->isChecked()== true) {

        SendCommDimOn();
        dimPress =1;
    }
    if (BDim->isChecked()==false) {
        SendCommDimOff();
        dimPress =0;
    }
     setTextReg();
}

void Vench::BPressEnerg()
{
    if (ui->BEnerg_n)if (ui->BEnerg_n->isChecked()== true) if (BEnerg)BEnerg->setChecked( true);
    if (ui->BEnerg_n)if (ui->BEnerg_n->isChecked()==false) if (BEnerg)BEnerg->setChecked(false);
	
    if (BEnerg->isChecked()== true) {
	    SendCommEnergOn();
        energPress =1;
	}
    if (BEnerg->isChecked()==false) {
	    SendCommEnergOff();
	    energPress =0;
	}
    setTextReg();
}

void Vench::BPressTemp()
{
    if (ui->BTemp_n)if (ui->BTemp_n->isChecked()== true) if (BTemp)BTemp->setChecked( true);
    if (ui->BTemp_n)if (ui->BTemp_n->isChecked()==false) if (BTemp)BTemp->setChecked(false);
	
    if (BTemp->isChecked()== true) {
	//    qDebug("UUUUUUUUUUU\n");
	    SendCommTempOn();
        flzaprostemposv=1;
	    tempPress =1;
	}
    if (BTemp->isChecked()==false) {
	    SendCommTempOff();
        flzaprostemposv=0;
        tempPress =0;
	}
	qDebug("flzap = %d\n",flzaprostemposv);
    setTextReg();
}
 


void Vench::BPressBar()
{
    if (ui->BBar_n)if (ui->BBar_n->isChecked()== true) if (BBar)BBar->setChecked( true);
    if (ui->BBar_n)if (ui->BBar_n->isChecked()==false) if (BBar)BBar->setChecked(false);

    if (BBar->isChecked()== true) {
	    SendCommBarOn();
        flzaprosbar=1;
	}
    if (BBar->isChecked()==false) {
	    SendCommBarOff();
        flzaprosbar=0;
	}
    setTextReg();
}
 

int Vench::SetAnswerUstr(int imp1, int imp2,int nD){
    

    if ((fl_rec)&&(!fl_bluetooth)){
        if(num_comm<NUM_COMM){
            if (checkDiskr->isChecked() ==  true){
                mass_comm_n[num_comm][0] = imp1;
    	        mass_comm_n[num_comm][1] = imp2;
                if (nD!=0) {
                    mass_comm_n[num_comm][2] = nD;
        //        	qDebug("DDDDDDDDDDDD\n");
                    num_comm++;
                }
    	    }
    	    else{
	        if (nD==0){
//qDebug("1=%d\n",imp1);
	    	    if ((imp1-rimp1)>0) mass_comm_n[num_comm][0] = imp1-rimp1;
	    	    else mass_comm_n[num_comm][0] = rimp1-imp1;
    	    	    if ((imp2-rimp2)>0) mass_comm_n[num_comm][1] = imp2 -rimp2;
    	    	    else mass_comm_n[num_comm][1] = rimp2-imp2;
            	    mass_comm_n[num_comm][2] = ret_napr;
//            	    ret_napr=0;
            	    if (((imp1-rimp1)!=0)&&((imp2-rimp2)!=0)) num_comm++;
		    rimp1=imp1;
		    rimp2=imp2;
                }
                else{
//qDebug("0=%d\n",imp1);
		    ret_napr=nD;
		    rimp1=imp1;
		    rimp2=imp2;
                }
    	    }
	}
//    	    qDebug("FFFFFFFFFFFFFFF =%d\n",num_comm);
        else {
            printf("ERROR chislo comand ogran!!!\n");
            return 0;
        }
    }
    return 1;
}

void Vench::TestFile(void * param)
{
//   double TimeIspComm;
//   double TPerComm;

    


    //int i;
   int ntec=0;
     Vench*md = (Vench*)param;

#ifdef     ZAPROS_PANID_P
    double TimeZaprosPanID = 0;
    TimeZaprosPanID = md->GetTime()+ TPerPID;
    double TPerPID;
     TPerPID = TZaprosPanID;
#endif

#ifdef     SEND_COMM_ANTISLEEP
#ifdef     DVA95
    double TimeSendCommAntisleep;
    double TPerANS;
     TimeSendCommAntisleep = 0;
     TPerANS = TZaprosANS;
     TimeSendCommAntisleep = md->GetTime()+ TPerANS;
#endif
#endif

//    TPerComm = TPeriodComand;
//    TimeIspComm = md->GetTime()+TPerComm;
    ntec=0;	

    while (md->fl_potoc){
        md->mSleep(40);
#ifdef     ZAPROS_PANID_P
        if  (TimeZaprosPanID < md->GetTime()){
//            qDebug("TestFile: TimeZa =%f\n",TimeZaprosPanID);
            TimeZaprosPanID = md->GetTime()+ TPerPID;
            for(i=0;i < md->mkol_ust;i++ ){
                if (md->act_ust[i].fl_pan==0){
//                    qDebug("TestFile: zaprosPanID\n");
                    md->SendZaprosPANID(md->act_ust[i].mac_addr);
//                    qDebug("TestFile: zaprosPanID11\n");
                }
            }

        }
        if  (TimeZaprosPanID < md->GetTime()- 5*TPerPID){
            TimeZaprosPanID = md->GetTime()+ TPerPID;
        }
#endif


#ifdef     SEND_COMM_ANTISLEEP

#ifdef     DVA95
        if  (TimeSendCommAntisleep < md->GetTime()){
//            qDebug("TestFile: TimeZa =%f\n",TimeZaprosPanID);
            TimeSendCommAntisleep = md->GetTime()+ TPerANS;
            for(i=0;i < md->mkol_ust;i++ ){
                if (md->act_ust[i].fl_antisleep!=2){
//                    qDebug("TestFile: zaprosPanID\n");
                        md->SendCommAntiSon(md->mas_mac_addr[i],md->mas_set_addr[i]);

//                    qDebug("TestFile: zaprosPanID11\n");
                }
            }

        }
        if  (TimeSendCommAntisleep < md->GetTime()- 5*TPerANS){
            TimeSendCommAntisleep = md->GetTime()+ TPerANS;
        }
#endif
#endif

        if (md->fl_play){
            if (ntec<md->num_comm){
                if (!md->fl_bluetooth){
                    if ((md->fl_play_comm==0)||(md->checkAll->isChecked()== true)){//poprobovat moget kogda shirokoveshat budet luchshe ezdit
                        md->fl_play_comm=1;
                        if (md->checkAll->isChecked()== true) {
                            md->fl_play_comm=0;
                            md->mSleep(50);
                        }else   md-> mSleep(50); // pause megdu komandami iz faila
                        md->SendComm1(md->mass_comm_n[ntec][0],md->mass_comm_n[ntec][1],md->mass_comm_n[ntec][2]);
                        ntec++;
                    }
                }//if (!flBlueTooth)
                else{
            	    if (md->fl_play_comm==0){
                	md->fl_play_comm=1;
//                    md->SendCommPrBT(md->mass_comm_n[ntec][0],d->mass_comm_n[ntec][1],md->mass_comm_n[ntec][2]);
                    md->SendCommPrBT(md->mass_comm_n[ntec][0],md->mass_comm_n[ntec][2]);
                    ntec++;
            	    }

	        }
            }//    end if (ntec<md->num_comm)

            else{
                        if     ((md->checkCicl->isChecked() ==  true)||(md->ui->checkCicl_n->isChecked() ==  true)||(md->ui->checkCicl_n_B->isChecked() ==  true))   ntec=0;
                        else md->fl_play =0;
            }
        }else {

         if (ntec!=0)  ntec=0;
        }

    }
}

void Vench::ReadProg(){
    int i;
      QDomDocument doc;
      QDomElement root;
      QDomNode node; 
      QDomNode node1; 

      QFile f(NAME_FILE_VAR);
      QString errorStr;
      int errorLine;
      int errorColumn;
      if (!doc.setContent(&f,  true, &errorStr, &errorLine,&errorColumn)) {
//	qWarning("Line %d, column %d: %s", errorLine, errorColumn, errorStr.toAscii()); 
//	printf("Line %d, column %d: %s", errorLine, errorColumn, errorStr); 
    
// create new file
    
	WriteProg1( 0 );
    
        qDebug("Error setContent\n");
          if (!doc.setContent(&f,  true, &errorStr, &errorLine,&errorColumn)) {
	    qDebug("Error setContent\n");
    	    return;
    	} 
      } 
      root = doc.documentElement(); 
      if (root.tagName() != "bim_xml") { 
	 printf("Error format file! \n");
	  return; 
      } 
      node = root.firstChild(); 
    if (node.toElement().tagName() == "prog") {
	node1 = node.firstChild(); 
	i=0;
	 while(!node1.isNull()){
	    if (node1.toElement().tagName() == "left") {
		mas_comm[i] = 1;
		
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		
		
		i++;
	    }
	    if (node1.toElement().tagName() == "right") {
		mas_comm[i] = 2;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
        tim_comm[i] = numpi;
		i++;
	    }
	    if (node1.toElement().tagName() == "forward") {
		mas_comm[i] = 3;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
//		printf("vvv=%f\n",numpi);
		tim_comm[i] = numpi;
		i++;
	    }
	    if (node1.toElement().tagName() == "revers") {
		mas_comm[i] = 4;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		i++;
	    }
	    
	    if (node1.toElement().tagName() == "light") {
		mas_comm[i] = 5;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		i++;
	    }
	    if (node1.toElement().tagName() == "bip") {
		mas_comm[i] = 6;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		i++;
	    }
	    if (node1.toElement().tagName() == "extrleft") {
		mas_comm[i] = 7;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		i++;
	    }
	    if (node1.toElement().tagName() == "extrright") {
		mas_comm[i] = 8;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		i++;
	    }
	
	    if (node1.toElement().tagName() == "stop") {
		mas_comm[i] = 9;
		QString rrr1;
		rrr1 = node1.toElement().attribute("tm");
		double numpi=rrr1.toDouble();
		tim_comm[i] = numpi;
		i++;
	    }
	    
	    
	    
	    node1 = node1.nextSibling(); 
	}
	numc=i;
    }
}

void Vench::WriteProg(int k){
int i;
      QDomDocument doc;
      QDomDocument md;
      QDomElement root;
      root = md.createElement("bim_xml");
      QDomNode node; 
      node = md.createElement("prog");
      QDomElement node1; 
	
      for(i=0;i<k;i++){
        if (mas_comm[i]==1) node1 = md.createElement("left");
        if (mas_comm[i]==2) node1 = md.createElement("right");
        if (mas_comm[i]==3) node1 = md.createElement("forward");
        if (mas_comm[i]==4) node1 = md.createElement("revers");

        if (mas_comm[i]==5) node1 = md.createElement("light");
        if (mas_comm[i]==6) node1 = md.createElement("bip");
        if (mas_comm[i]==7) node1 = md.createElement("extrleft");
        if (mas_comm[i]==8) node1 = md.createElement("extrright");
        if (mas_comm[i]==9) node1 = md.createElement("stop");

	QString t;
	t.setNum(tim_comm[i],'g',3);
	node1.setAttribute("tm1",t);
          node.appendChild(node1);

      }
      root.appendChild(node);
      doc.appendChild(root);
      
      
      
  
  FILE *fff;
  fff = fopen(NAME_FILE_VAR,"w+");
  QTextStream out(fff,QIODevice::WriteOnly); 
  doc.save(out, 4);
  fclose(fff);
//printf("www\n");

}



void Vench::SendCommOsvOff()
{
//    int mk[10];
    unsigned char mkc[10];
    int  numu;
    int i,j;
    numu=0;
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");

        return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

        for(j=0;j<numu;j++){
            if (mas_vibr_ust[j]==1){

                for(i=0;i<8;i++){
//                    mk[i] =  mas_mac_addr[j][i];
                    mkc[i] = mas_mac_addr[j][i];
                }

                QString cm,ss;

                int numust = GetNumberUst(mkc);
                act_ust[numust].period_oprosa_o=0.0;

            }
        }
	return;
    }




    if (checkAll->isChecked() ==  true) {
        {
            ;
        }
    }
    else{
        numu = SpActDatch->currentIndex();
        for(i=0;i<8;i++){
//          mk[i] =mas_mac_addr[numu][i];
          mkc[i] =mas_mac_addr[numu][i];

        }
//        for(i=0;i<2;i++){
//            mk[i+8] =mas_set_addr[numu][i];
//        }

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_o=0.0;


    }
}



void Vench::SendCommDimOff()
{

//    int mk[10];
    unsigned char mkc[10];
    int  numu;
    int i,j;
   numu=0;
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");

        return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

    for(j=0;j<numu;j++){
        if (mas_vibr_ust[j]==1){


        for(i=0;i<8;i++){
//            mk[i] =  mas_mac_addr[j][i];
            mkc[i] = mas_mac_addr[j][i];
        }
//        for(i=0;i<2;i++){
//            mk[i+8] =mas_set_addr[j][i];
//        }

        //mk[8]=0xFF;
        //mk[9]=0xFE;

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_d=10.0;

        SendZaprosOtklDim(mkc);// zapros konkretnogo ustroistva

        }
    }
    return;
    }




    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
        {

        }
    }
    else{
        numu = SpActDatch->currentIndex();
        for(i=0;i<8;i++){
//          mk[i] =mas_mac_addr[numu][i];
          mkc[i] =mas_mac_addr[numu][i];

        }
//        for(i=0;i<2;i++){
//            mk[i+8] =mas_set_addr[numu][i];
//        }

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_o=10.0;

        SendZaprosOtklDim(mkc);// zapros konkretnogo ustroistva
    }
}

void Vench::SendCommDimOn()
{

//    int mk[10];
    unsigned char mkc[10];
    int  numu;
    int i,j;
   numu=0;
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");

        return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

    for(j=0;j<numu;j++){
        if (mas_vibr_ust[j]==1){


        for(i=0;i<8;i++){
//            mk[i] =  mas_mac_addr[j][i];
            mkc[i] = mas_mac_addr[j][i];
        }
//        for(i=0;i<2;i++){
//            mk[i+8] =mas_set_addr[j][i];
//        }

        //mk[8]=0xFF;
        //mk[9]=0xFE;

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_d=10.0;

        SendZaprosDim(mkc);// zapros konkretnogo ustroistva

        }
    }
    return;
    }




    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
        {
            for(i=0;i<8;i++){

//                mk[i]=0;
                mkc[i]=0;
            }
            for(i=0;i<2;i++){

//                mk[i+8]=0;
                mkc[i+8]=0;
            }
//            mk[6]=0xff;
//            mk[7]=0xff;
//            mk[8]=0xff;
//            mk[9]=0xfe;
            mkc[6]=0xff;
            mkc[7]=0xff;
            mkc[8]=0xff;
            mkc[9]=0xfe;
            SendZaprosDim(mkc);// zapros konkretnogo ustroistva

        }
    }
    else{
        numu = SpActDatch->currentIndex();
        for(i=0;i<8;i++){
//          mk[i] =mas_mac_addr[numu][i];
          mkc[i] =mas_mac_addr[numu][i];

        }
//        for(i=0;i<2;i++){
//            mk[i+8] =mas_set_addr[numu][i];
//        }

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_o=10.0;

        SendZaprosDim(mkc);// zapros konkretnogo ustroistva
    }
}

void Vench::SendCommOsvOn()
{

    int mk[10];
    unsigned char mkc[10];
    int  numu;
    int i,j;
   numu=0;
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");

        return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

    for(j=0;j<numu;j++){
        if (mas_vibr_ust[j]==1){


        for(i=0;i<8;i++){
            mk[i] =  mas_mac_addr[j][i];
            mkc[i] = mas_mac_addr[j][i];
        }
        for(i=0;i<2;i++){
            mk[i+8] =mas_set_addr[j][i];
        }

        //mk[8]=0xFF;
        //mk[9]=0xFE;

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_o=10.0;


        cm = "* 7E 0 10 17 11 ";
        for (int i=0;i<8;i++){
            ss="";
            ss.setNum(mk[i],16);
            cm = cm+ss+" ";
        }
        cm = cm+ "2 49 53 ";
//        qDebug()<<cm;
        SendComm(cm);


        }
    }
    return;
    }




    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
        {
            for(i=0;i<8;i++){

                mk[i]=0;
            }
            for(i=0;i<2;i++){

                mk[i+8]=0;
            }
            mk[6]=0xff;
            mk[7]=0xff;
            mk[8]=0xff;
            mk[9]=0xfe;
            QString cm,ss;
            cm = "* 7E 0 10 17 11 ";
        //    printf(cm);
            for (int i=0;i<10;i++){
                ss="";
                ss.setNum(mk[i],16);
                cm = cm+ss+" ";
            }
            cm = cm+ "2 49 53 ";
            SendComm(cm);

        }
    }
    else{
        numu = SpActDatch->currentIndex();
        for(i=0;i<8;i++){
          mk[i] =mas_mac_addr[numu][i];
          mkc[i] =mas_mac_addr[numu][i];

        }
        for(i=0;i<2;i++){
            mk[i+8] =mas_set_addr[numu][i];
        }

        QString cm,ss;

        int numust = GetNumberUst(mkc);
        act_ust[numust].period_oprosa_o=10.0;


        cm = "* 7E 0 10 17 11 ";
    //    printf(cm);
        for (int i=0;i<10;i++){
            ss="";
            ss.setNum(mk[i],16);
            cm = cm+ss+" ";
        }
        cm = cm+ "2 49 53 ";
        SendComm(cm);
    }
}


void Vench::SendCommForw1()
{
    int mk[10];
    int  numu;
    int i,j;
   numu=0;
//   SendCommBudil();
  // sleep(1);
   
//    printf("SendCommorw\n");
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");
    //	ObSpActUst();
        return;//если список пуст
    }
    
        if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){


            numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){

            for(i=0;i<8;i++){
                mk[i] =mas_mac_addr[j][i];
            }
            for(i=0;i<2;i++){
                mk[i+8] =mas_set_addr[j][i];
            }
	    
	    
	    
            QString cm,ss;

            cm = "* 7E 0 12 10 11 ";
	    
	    #ifdef OLDD
            cm = "* 7E 0 11 10 11 ";
	    #endif
	    
	    //    printf(cm);
            for (int i=0;i<8;i++){
                ss="";
                ss.setNum(mk[i],16);
                cm = cm+ss+" ";
            }

            if (checkDiskr->isChecked() == false) {
            #ifndef OLDD
                cm = cm+ "FF FE 0 0 ";
            #endif
            #ifdef OLDD
                cm = cm+ "FF FE 0 ";
            #endif

                QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
                cm = cm+ "0 0 A";
            }
            else {
	    
	    #ifndef OLDD
                cm = cm+ "FF FE 0 0  ";
	    #endif
	    #ifdef OLDD
                cm = cm+ "FF FE 0 ";
	    #endif
	    
                QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
	    
//		ss.setNum(imp_diskr,16); old version
//		cm = cm+ss+" "+ss+" A";

		//26_10_13 chtobi dla kagdoi mashinki
                int impr,impl;
                GetKolImpUstr(mk,&impr,&impl);
		
//		qDebug("imr = %d\niml = %d\n",impr,impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" A";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0xa);
	    }
	    
//            qDebug() << cm;
//            qDebug("QQQQQQQQQQQQQ\n"); 	

            SendComm(cm);
	    //    
	    //    cm = "& b 3 44 34 5 ";
	    ////    printf(cm);
	    //    for (int i=0;i<10;i++){
	    //	ss="";
	    //	ss.setNum(mk[i],16);
	    //	cm = cm+ss+" ";
	    //    }
	    ////    printf(cm);
	    //    
	    //    SendComm(cm);



	    
	    
	    }
	}
	return;
    }

    
    if (checkAll->isChecked() ==  true) {
	//for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
            mk[i] =0;
	    }
	    for(i=0;i<2;i++){
            mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;

	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
            cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
            cm = cm+ "FF FE 0 ";
	    #endif
	    QString ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
    
        	cm = cm+ "0 0 A";
	    
	    
	    }
	    else {
	    
	    #ifndef OLDD
            cm = cm+ "FF FE 0 0 ";
	    #endif
	    #ifdef OLDD
            cm = cm+ "FF FE 0 ";
	    #endif
		QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";


		ss.setNum(imp_diskr,10);
		cm = cm+ss+" "+ss+" A";

//		//26_10_13 chtobi dla kagdoi mashinki
//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
//		int impr,impl;
//		GetKolImpUstr(mk,&impr,&impl);
//		ss.setNum(impr,16); 
//		cm = cm+ss+" ";
//		ss.setNum(impl,16); 
//		cm = cm+ss+" A";
//		//end 26_10_13 chtobi dla kagdoi mashinki
	    }
	    
	    //    printf(cm);
	    
	    SendComm(cm);
	    
	}
    }
    else{
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	//    printf(cm);
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 A";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" A";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
//		qDebug("imr = %d\niml = %d\n",impr,impl);
		
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" A";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) {
        	    SetAnswerUstr(impr, impl,0xa);
        	}
	    }
	
	//    printf(cm);
	SendComm(cm);
	//    
	//    cm = "& b 3 44 34 5 ";
	////    printf(cm);
	//    for (int i=0;i<10;i++){
	//	ss="";
	//	ss.setNum(mk[i],16);
	//	cm = cm+ss+" ";
	//    }
	////    printf(cm);
	//    
	//    SendComm(cm);
    }
}


void Vench::SendCommBudil()
{
   int mk[10];
    int  numu;
    int i,j;
   numu=0;
   qDebug("SendCommBudil\n");
   numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }
    
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){


	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }

//        mk[8]=0xFF;
//        mk[9]=0xFE;

	    QString cm,ss;
 
 
 
        cm = "* 7E 0 10 17 11 ";
        for (int i=0;i<10;i++){
            ss="";
            ss.setNum(mk[i],16);
            cm = cm+ss+" ";
	    }
	    cm = cm+ "2 44 34 4 ";
	    SendComm(cm);
        qDebug()<<cm;
//        sleep(1);//zadergka
//        mSleep(100);//zadergka
        mSleep(50);
        cm = "* 7E 0 10 17 11 ";
	//    printf(cm);
        for (int i=0;i<10;i++){
            ss="";
            ss.setNum(mk[i],16);
            cm = cm+ss+" ";
	    }
	    cm = cm+ "2 44 34 5 ";
	    SendComm(cm);
        qDebug()<<cm;

	    
	    }
	}
	return;
    }


    
    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	    for(i=0;i<8;i++){
//		mk[i] = mas_mac_addr[j][i];
		mk[i]=0;
	    }
	    for(i=0;i<2;i++){
//		mk[i+8] =mas_set_addr[j][i];
		mk[i+8]=0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
	    
	    
	    QString cm,ss;
  
//	    cm = "& b 4 2 44 34 4 ";
//	    for (int i=0;i<10;i++){
//		ss="";
//		ss.setNum(mk[i],16);
//		cm = cm+ss+" ";
//	    }
//    
//	    SendComm(cm);
  // 
//	    cm = "& b 4 2 44 34 5 ";
//	    for (int i=0;i<10;i++){
//		ss="";
//		ss.setNum(mk[i],16);
//		cm = cm+ss+" ";
//	    }
//    
//	    SendComm(cm);

	    cm = "* 7E 0 10 17 11 ";


	//    printf(cm);
	    for (int i=0;i<10;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
	    cm = cm+ "2 44 34 4 ";
	    SendComm(cm);
        mSleep(100);//zadergka
//        sleep(1);//zadergka
        cm = "* 7E 0 10 17 11 ";
	//    printf(cm);
	    for (int i=0;i<10;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
	    cm = cm+ "2 44 34 5 ";
	    SendComm(cm);
	    
	}
    }
    else{
	numu = SpActDatch->currentIndex();
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	
	    QString cm,ss;
  
//	    cm = "& b 4 2 44 34 4 ";
//	    for (int i=0;i<10;i++){
//		ss="";
//		ss.setNum(mk[i],16);
//		cm = cm+ss+" ";
//	    }
//    
//	    SendComm(cm);
  // 
//	    cm = "& b 4 2 44 34 5 ";
//	    for (int i=0;i<10;i++){
//		ss="";
//		ss.setNum(mk[i],16);
//		cm = cm+ss+" ";
//	    }
//    
//	    SendComm(cm);

	    cm = "* 7E 0 10 17 11 ";
	//    printf(cm);
	    for (int i=0;i<10;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
	    cm = cm+ "2 44 34 4 ";
	    SendComm(cm);
//        sleep(1);//zadergka
        mSleep(10);//zadergka
        cm = "* 7E 0 10 17 11 ";
	//    printf(cm);
	    for (int i=0;i<10;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
	    cm = cm+ "2 44 34 5 ";
	    SendComm(cm);

    }  
}


void Vench::SendCommStop1()
{
    int mk[10];
    int  numu;
    int i,j;
   numu=0;
//   SendCommBudil();
//   sleep(1);
   
  //  printf("SendComm Stop1\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){



	     for(i=0;i<8;i++){
		 mk[i] =mas_mac_addr[j][i];
	     }
	     for(i=0;i<2;i++){
		 mk[i+8] =mas_set_addr[j][i];
	     }
	     
	     
	     
	     QString cm,ss;

	    cm = "* 7E 0 12 10 11 ";
	    
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif


	     //    printf(cm);
	     for (int i=0;i<8;i++){
		 ss="";
		 ss.setNum(mk[i],16);
		 cm = cm+ss+" ";
	     }

	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 0 0 0 0";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 0 0 0 0";
	    #endif


	     
	     //    printf(cm);
	     
	     SendComm(cm);

	    
	    
	    }
	}
	return;
    }
    
    
     if (checkAll->isChecked() ==  true) {
//	 for(j=0;j<numu;j++)
	 {
	     for(i=0;i<8;i++){
		 mk[i] =0;
	     }
	     for(i=0;i<2;i++){
		 mk[i+8] =0;
	     }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	     
	     
	     
	     QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif

	     //    printf(cm);
	     for (int i=0;i<8;i++){
		 ss="";
		 ss.setNum(mk[i],16);
		 cm = cm+ss+" ";
	     }
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 0 0 0 0";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 0 0 0 0";
	    #endif


	     
	     //    printf(cm);
	     
	     SendComm(cm);
	     
	 }
     }
     else{
	 numu = SpActDatch->currentIndex();
	 
	 for(i=0;i<8;i++){
	     mk[i] =mas_mac_addr[numu][i];
	 }
	 for(i=0;i<2;i++){
	     mk[i+8] =mas_set_addr[numu][i];
	 }
	 
	 
	 
	 QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	 //    printf(cm);
	 for (int i=0;i<8;i++){
	     ss="";
	     ss.setNum(mk[i],16);
	     cm = cm+ss+" ";
	 }
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 0 0 0 0";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 0 0 0 0";
	    #endif
	 
	 //    printf(cm);
	 
	 SendComm(cm);
     }
}


void Vench::SendCommLeft1()
{
    int mk[10];
    int  numu;
    int i,j;
   numu=0;
//   SendCommBudil();
//   sleep(1);
   
//    printf("SendComm Left1\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }
    
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){



	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 8";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		
		QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";

//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 8";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 8";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x8);
	    }
	    //    printf(cm);
	    
	    SendComm(cm);
	    
	    

	    
	    
	    }
	}
	return;
    }
    
    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 8";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif


		QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";

		ss.setNum(imp_diskr,10);
		cm = cm+ss+" "+ss+" 8";

//		//26_10_13 chtobi dla kagdoi mashinki
//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
//		int impr,impl;
//		GetKolImpUstr(mk,&impr,&impl);
//		ss.setNum(impr,16); 
//		cm = cm+ss+" ";
//		ss.setNum(impl,16); 
//		cm = cm+ss+" 8";
//		//end 26_10_13 chtobi dla kagdoi mashinki
	    }


	    
	    //    printf(cm);
	    
	    SendComm(cm);
	    
	    
	}
	
    }
    else{
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	//    printf(cm);
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 8";
	    
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif


		QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";

//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 8";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 8";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x8);
	    }

	
	//    printf(cm);
	
	SendComm(cm);
    }
}


void Vench::SendCommRight1()
{
    int mk[10];
    int  numu;
    int i;
    int j;
   numu=0;
//   SendCommBudil();
//   sleep(1);
   
//    printf("SendComm Right1\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }
    
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){

	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 2";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif


		QString    ss="";

		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";


//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 2";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 2";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x2);
	    }



	    
	    //    printf(cm);
	    
	    SendComm(cm);
    	    



	    
	    
	    }
	}
	return;
    }

    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	    
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }


        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 2";
	    
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif


		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		ss.setNum(imp_diskr,10);
		cm = cm+ss+" "+ss+" 2";

//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
//		//26_10_13 chtobi dla kagdoi mashinki
//		int impr,impl;
///		GetKolImpUstr(mk,&impr,&impl);
//		ss.setNum(impr,16); 
//		cm = cm+ss+" ";
//		ss.setNum(impl,16); 
//		cm = cm+ss+" 2";
//		//end 26_10_13 chtobi dla kagdoi mashinki
	    }
	    
	    //    printf(cm);
	    
	    SendComm(cm);
    	    
	}
    }
    else{
    
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	//    printf(cm);
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 2";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif


		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 2";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 2";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x2);
	    }
	
	//    printf(cm);
	
	SendComm(cm);
    }
}


void Vench::SendCommExtrLeft1()
{
    int mk[10];
    int  numu;
    int i;
    int j;
    numu=0;
//   SendCommBudil();
//   sleep(1);
   
 //   printf("SendComm Extr Left 1\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }
    
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){



	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 9";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 9";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 9";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x9);
	    }
	    
	    //    printf(cm);
	    
	    SendComm(cm);

	    
	    
	    }
	}
	return;
    }
    
    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
		
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 9";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		ss.setNum(imp_diskr,10);
		cm = cm+ss+" "+ss+" 9";


//		//26_10_13 chtobi dla kagdoi mashinki
//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
//		int impr,impl;
//		GetKolImpUstr(mk,&impr,&impl);
//		ss.setNum(impr,16); 
//		cm = cm+ss+" ";
//		ss.setNum(impl,16); 
//		cm = cm+ss+" 9";
//		//end 26_10_13 chtobi dla kagdoi mashinki
	    }
	    
	    //    printf(cm);
	    
	    SendComm(cm);
	    
	}
    }
    else{
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	//    printf(cm);
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 9";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 9";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 9";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x9);
	    }
	
	//    printf(cm);
	
	SendComm(cm);
   }
}


void Vench::SendCommExtrRight1()
{
    int mk[10];
    int  numu;
    int i;
    int j;
    numu=0;
//   SendCommBudil();
//   sleep(1);
   
 //   printf("SendComm Extr Right 1\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }
    
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){



	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
	    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 6";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 6";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 6";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x6);
	    }
	    
	    //    printf(cm);
	    
	    SendComm(cm);
	    

	    
	    
	    }
	}
	return;
    }
    
    
    if (checkAll->isChecked() ==  true) {
//	for(j=0;j<numu;j++)
	{
	    
	    for(i=0;i<8;i++){
		mk[i] =0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;
    
	    
	    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 6";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(imp_diskr,10);
		cm = cm+ss+" "+ss+" 6";


//		//26_10_13 chtobi dla kagdoi mashinki
//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
//		int impr,impl;
//		GetKolImpUstr(mk,&impr,&impl);
//		ss.setNum(impr,16); 
//		cm = cm+ss+" ";
//		ss.setNum(impl,16); 
//		cm = cm+ss+" 6";
//		//end 26_10_13 chtobi dla kagdoi mashinki
	    }
	    
	    //    printf(cm);
	    
	    SendComm(cm);
	    
	    
	}
	
    }
    else{
    
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	//    printf(cm);
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}
		
        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 6";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 6";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 6";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x6);
	    }
	
	//    printf(cm);
	
	SendComm(cm);
    }
}


void Vench::SendCommRevers1()
{
    int mk[10];
    int  numu;
    int i;
    int j;
   numu=0;
//   SendCommBudil();
//   sleep(1);
   
 //   printf("SendComm Revers1\n");
    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
//	ObSpActUst(); 
	return;//если список пуст
    }

    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){


	    for(i=0;i<8;i++){
		mk[i] =mas_mac_addr[j][i];
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =mas_set_addr[j][i];
	    }
     
    
    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }

        if (checkDiskr->isChecked() == false) {
            #ifndef OLDD
            cm = cm+ "FF FE 0 0 ";
            #endif
            #ifdef OLDD
            cm = cm+ "FF FE 0 ";
            #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
        	cm = cm+ "0 0 5";
	    }
	    else {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 5";

		//26_10_13 chtobi dla kagdoi mashinki
//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 5";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x5);
	    }
	
	    //   	 printf(cm);
	    
	    SendComm(cm);
	    


	    
	    
	    }
	}
	return;
    }


    if (checkAll->isChecked() ==  true) {
	//for(j=0;j<numu;j++)
	{
	    
	    for(i=0;i<8;i++){
		mk[i] = 0;
	    }
	    for(i=0;i<2;i++){
		mk[i+8] =0;
	    }
     	    mk[6]=0xff;
	    mk[7]=0xff;
	    mk[8]=0xff;
	    mk[9]=0xfe;

    
    
	    QString cm,ss;
	    cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	    //    printf(cm);
	    for (int i=0;i<8;i++){
		ss="";
		ss.setNum(mk[i],16);
		cm = cm+ss+" ";
	    }

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 5";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		ss.setNum(imp_diskr,10);
		cm = cm+ss+" "+ss+" 5";


//		//26_10_13 chtobi dla kagdoi mashinki
//		//NE NADO tk dla kagdoi mashinki svoi A komanda DLA VSEH!!!!
//		int impr,impl;
//		GetKolImpUstr(mk,&impr,&impl);
//		ss.setNum(impr,16); 
//		cm = cm+ss+" ";
//		ss.setNum(impl,16); 
//		cm = cm+ss+" 5";
//		//end 26_10_13 chtobi dla kagdoi mashinki
	    }
	
	    //   	 printf(cm);
	    
	    SendComm(cm);
	    
	    
	}
    }
    else{
    
	numu = SpActDatch->currentIndex();
    
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
     
    
    
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
	    #ifdef OLDD
	    cm = "* 7E 0 11 10 11 ";
	    #endif
	//    printf(cm);
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}

        if (checkDiskr->isChecked() == false) {
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 ";
	    #endif	
	    #ifdef OLDD
		cm = cm+ "FF FE 0 ";
	    #endif
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
		cm = cm+ "0 0 5";
	    }
	    else {
	    
	    #ifndef OLDD
		cm = cm+ "FF FE 0 0 "; 
	    #endif
	    #ifdef OLDD
		cm = cm+ "FF FE 0 "; 
	    #endif
		QString    ss="";
		ss.setNum(speed_car,16); 
		cm = cm+ss+" ";
//		ss.setNum(imp_diskr,16);
//		cm = cm+ss+" "+ss+" 5";
		//26_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
		GetKolImpUstr(mk,&impr,&impl);
		ss.setNum(impr,16); 
		cm = cm+ss+" ";
		ss.setNum(impl,16); 
		cm = cm+ss+" 5";
		//end 26_10_13 chtobi dla kagdoi mashinki
        	if (fl_rec==1) SetAnswerUstr(impr, impl,0x5);
	    }
	
	//    printf(cm);
	
	SendComm(cm);
    }
}


int Vench::ReadAnswer(int *imp1, int *imp2,int *naprD)
{
    
    FILE* fl_com;
    QString txt,tt,dds;
    int i;
    int fs;
    QString ss;
    int dt;
    int mac_a[8];
//        int mk[10];

//    int set_a[2];
//    int newp;
//    int nump;
//    int nkol;



    int ret;
    char ssr[10];
    int flr;
    flr=0;
    
    
    ret =0;
    
    dt=0;
    fs=0;
    ss="";
//    newp =0;
//    nkol  =0;
    txt= ServerDir->text();
    txt = txt + "ANS_";
    qDebug()<<txt;
//    int numu;
//    numu = SpActDatch->currentIndex();
    for(i=0;i<8;i++){
        mac_a[i] =mac_addr_prg[i];
    }
    
    
    
    for (i=0;i<8;i++){
        ss="";
        ss.setNum(mac_a[i],16);
        txt = txt+ss;
    }
    
    
    txt = txt + ".txt";
    
    
    	

    fl_com = fopen(txt.toLocal8Bit(),"r+");
    
    
//    printf(txt);
//    QByteArray rr = txt.toLocal8Bit();
//    printf("fl name = %s\n",rr.constData());
    
//    if (fl_com<=0) {
    if (fl_com==NULL) {
    if (fl_rec==1) qDebug(" Error open file answer \n");
	return -3;
    }
    ret=0;
    while(1)
	//  for(int ii;ii<1;ii++)
    {
	    fs = fscanf(fl_com,"%s",ssr);
	    dds = ssr;
	    if (fs>0){
		if (dds == "sleep") {
		    SendCommBudil();
		    printf("sleep\n");
		    flr=1;
		    break;
		}
		if (dds == "imp"){
//		       printf("www\n");
		    fs = fscanf(fl_com,"%d",&dt);
		    if (fs>0) {
			*imp1 = dt;
			
			fs = fscanf(fl_com,"%d",&dt);
			if (fs>0){
			    *imp2=dt;
			
			    fs = fscanf(fl_com,"%x",&dt);
			    if (fs>0){
				*naprD=dt;
						    flr=1;

			    }else ret=-1;
			}else ret=-1;
		    
		    }
		    else ret =-1;
//		    printf("imp1 = %d\n",*imp1);
//		    printf("imp2 = %d\n",*imp2);
//		    printf("flr==%d\n",flr);
		}
	    }
	    if (fs<=0) break;
    }
    if (fl_com) fclose(fl_com);
    fl_com = fopen((txt.toLocal8Bit()),"w+");
    if (fl_com) fclose(fl_com);
  //  printf("flr==%d\n",flr);
    if (flr==1) ret =1;
    return ret;  
}


void Vench::SendComm1( int d1, int d2, int napr )
{
    int mk[10];
    int  numu;
    int i;
    int md1,md2;
//    int j;
    numu=0;
    md1=0;
    md2=md1+0;
    md1=md2+0;
//   SendCommBudil();
//   sleep(1);
   
 //   printf("SendComm  1\n");
    numu = SpActDatch->count();
    if (numu==0) {
        qDebug("Net active ustr\n");
    //	ObSpActUst();
        return;//если список пуст
    }


    if (napr==0xB1){// svet
    	    SendCommLight1();
//    sleep(2);
        mSleep(50);

    	    SendCommStopLight1();
        fl_play_comm=0;
        return ;
    }
    else{
      if (napr==0xC1){//bip

    		SendCommBip1();
//        sleep(2);
        mSleep(200);

    		SendCommStopBip1();
	    fl_play_comm=0;
    	    return ;
     }
    }
    
    if ((checkAll->isChecked() == false)&&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

	for(int j=0;j<numu;j++){
	    if (mas_vibr_ust[j]==1){

		for(i=0;i<8;i++){
		    mk[i] =mas_mac_addr[j][i];
		}
		for(i=0;i<2;i++){
		    mk[i+8] =mas_set_addr[j][i];
		}
	    
		//POCHEMU&?????????????????????
		if (napr==0){
		    md1=0;
		    md2=0;
		}
		else{
		    md1=d1;
		    md2=d2;
		}
    
		QString cm,ss;
		cm = "* 7E 0 12 10 11 ";
#ifdef OLDD
		cm = "* 7E 0 11 10 11 ";
#endif
		for (int i=0;i<8;i++){
		    ss="";
		    ss.setNum(mk[i],16);
		    cm = cm+ss+" ";
		}
		
#ifndef OLDD
		cm = cm+ "FF FE 0 0 0 "; 
#endif
#ifdef OLDD
		cm = cm+ "FF FE 0 0 "; 
#endif
//			qDebug("CCCCCCCCCCCCCCCC\n");
		
            if (checkKumir->isChecked() == false) {
			    ss="";
			    ss.setNum(md1,16);
			    cm = cm+ss+" ";
			    ss="";
			    ss.setNum(md2,16);
			    cm = cm+ss+" ";
			

		}
		else{
			//30_10_13 chtobi dla kagdoi mashinki
			int impr,impl;
			GetKolImpUstr(mk,&impr,&impl);
		    
			if (napr==0) impr=0;
			ss.setNum(impr,16); 
			cm = cm+ss+" ";
			if (napr==0) impl=0;
			ss.setNum(impl,16); 
	    		cm = cm+ss+" ";
		}


		ss="";
		ss.setNum(napr,16);
       
		cm = cm+ss+"";
		SendComm(cm);
	    }
    }
	return;
    }

    
    
  
    if (checkAll->isChecked() ==  true) {
	    
	    for(i=0;i<8;i++){
            mk[i] =0;
	    }
	    for(i=0;i<2;i++){
	    	mk[i+8] =0;
	    }
        mk[6]=0xFF;
        mk[7]=0xFF;
        mk[8]=0xFF;
        mk[9]=0xFE;
	    
	    
	    //POCHEMU&?????????????????????
	if (napr==0){
	    md1=0;
	    md2=0;
	}
	else{
	    md1=d1;
	    md2=d2;
	}
    
	QString cm,ss;
		cm = "* 7E 0 12 10 11 ";
#ifdef OLDD
		cm = "* 7E 0 11 10 11 ";
#endif
		for (int i=0;i<8;i++){
		    ss="";
		    ss.setNum(mk[i],16);
		    cm = cm+ss+" ";
		}
		
#ifndef OLDD
		cm = cm+ "FF FE 0 0 0 "; 
#endif
#ifdef OLDD
		cm = cm+ "FF FE 0 0 "; 
#endif
		
		
		
		//	    ss="";
		//	    ss.setNum(md1,16);
		//	    cm = cm+ss+" ";
		//	    ss="";
		//	    ss.setNum(md2,16);
		//	    cm = cm+ss+" ";

            if (checkKumir->isChecked() == false) {
			    ss="";
			    ss.setNum(md1,16);
			    cm = cm+ss+" ";
			    ss="";
			    ss.setNum(md2,16);
			    cm = cm+ss+" ";
			

		}
		else{

		//30_10_13 chtobi dla kagdoi mashinki
		int impr,impl;
        impr = n_imp_kum;
        impl = n_imp_kum_p;
		
		
		if (napr==0) impr=0;
            ss.setNum(impr,16);
            cm = cm+ss+" ";
            if (napr==0) impl=0;
            ss.setNum(impl,16);
	    	cm = cm+ss+" ";
		}


		ss="";
		ss.setNum(napr,16);
       
		cm = cm+ss+"";
		SendComm(cm);
  //      qDebug("sssssssssssssssssss\n");
    

    }
    else{//esli ne vibrano ustr-vo v spiske && net priznaka chto vse
    
	numu = SpActDatch->currentIndex();
	
	for(i=0;i<8;i++){
	    mk[i] =mas_mac_addr[numu][i];
	}
	for(i=0;i<2;i++){
	    mk[i+8] =mas_set_addr[numu][i];
	}
	
	
	//POCHEMU&?????????????????????
	if (napr==0){
	    md1=0;
	    md2=0;
	}
	else{
	    md1=d1;
	    md2=d2;
	}
	
	
	QString cm,ss;
	cm = "* 7E 0 12 10 11 ";
#ifdef OLDD
	cm = "* 7E 0 11 10 11 ";
#endif
	for (int i=0;i<8;i++){
	    ss="";
	    ss.setNum(mk[i],16);
	    cm = cm+ss+" ";
	}
	
#ifndef OLDD
	cm = cm+ "FF FE 0 0 0 "; 
#endif
#ifdef OLDD
	cm = cm+ "FF FE 0 0 "; 
#endif
	
	
	
	//    ss="";
	//    ss.setNum(md1,16);
	//    cm = cm+ss+" ";
	//    ss="";
	//    ss.setNum(md2,16);
	//    cm = cm+ss+" ";
	
            if (checkKumir->isChecked() == false) {
			    ss="";
			    ss.setNum(md1,16);
			    cm = cm+ss+" ";
			    ss="";
			    ss.setNum(md2,16);
			    cm = cm+ss+" ";
			

		}
		else{
	
	//30_10_13 chtobi dla kagdoi mashinki
	int impr,impl;
	GetKolImpUstr(mk,&impr,&impl);
	if (napr==0) impr=0;
	ss.setNum(impr,16); 
	cm = cm+ss+" ";
	if (napr==0) impl=0;
	ss.setNum(impl,16); 
	cm = cm+ss+" ";
	}	
	
	ss="";
	ss.setNum(napr,16);
	
	cm = cm+ss+"";
	
	//    printf(cm);
	
        SendComm(cm);
    }
}


void Vench::WriteProg1( int k )
{
    int i;
      QDomDocument doc("MyML");
//      QDomDocument doc();
      QDomDocument md;
      QDomElement root;
      root = md.createElement("bim_xml");

      QDomNode node; 
      node = md.createElement("prog");
      QDomElement node1; 
	
//      printf("kk=%d\n",k);
  //    printf("ppp  = %d\n",mass_comm_n[0][0]);

      for(i=0;i<k;i++){
	  
	 node1 = md.createElement("pole");

	QString t;
	t.setNum(mass_comm_n[i][0],10);
	node1.setAttribute("imp1",t);
//	printf(t);
//	printf("\n");
	t.setNum(mass_comm_n[i][1],10);
	node1.setAttribute("imp2",t);
	t.setNum(mass_comm_n[i][2],16);
	node1.setAttribute("napr",t);
          node.appendChild(node1);

      }
      root.appendChild(node);
//    printf("qqqqqq\n");

      doc.appendChild(root);
      
      
      

    QString fnamec;

    fnamec= ServerDir->text();
    #ifdef LINUX_D
//    fnamec= fnamec+"/";
    fnamec = fnamec+f_name_xml;
    #endif
    #ifdef WIN_D
//    fnamec= fnamec+"\\";
    fnamec= f_name_xml;
    #endif


//    qDebug("RRRR\n");

//    qDebug()<<fnamec;

    
    QByteArray rr = fnamec.toLocal8Bit();

  FILE *fff;
  fff = fopen(rr.constData(),"w+");
  QTextStream out(fff,QIODevice::WriteOnly); 
  doc.save(out, 4);
  fclose(fff);

}


void Vench::ReadProg1()
{
int i;
//printf("read prog\n");
      QDomDocument doc;
      QDomElement root;
      QDomNode node; 
      QDomNode node1; 



    QString fnamec;

    fnamec= ServerDir->text();
    #ifdef LINUX_D
//    fnamec= fnamec+"/";
    fnamec = fnamec+f_name_xml;
    #endif
    #ifdef WIN_D
//    fnamec= fnamec+"\\";
    fnamec= f_name_xml;
    #endif


//    qDebug("RRRR\n");

//    qDebug()<<fnamec;

      QFile f(fnamec);
      
      
      
      
      QString errorStr;
      int errorLine;
      int errorColumn;
      if (!doc.setContent(&f,  true, &errorStr, &errorLine,&errorColumn)) {
//	qWarning("Line %d, column %d: %s", errorLine, errorColumn, errorStr.toAscii()); 

// create new file
    
	WriteProg1( 0 );
    
        qDebug("Error setContent\n");
          if (!doc.setContent(&f,  true, &errorStr, &errorLine,&errorColumn)) {
	    qDebug("Error setContent\n");
    	    return;
    	} 

      } 
      root = doc.documentElement(); 
      if (root.tagName() != "bim_xml") { 
	 printf("Error format file! \n");
	  return; 
      } 
      node = root.firstChild(); 
    if (node.toElement().tagName() == "prog") {
	node1 = node.firstChild(); 
	i=0;
	 while(!node1.isNull()){
	    if (node1.toElement().tagName() == "pole") {
	
		
		QString rrr1;
		bool ok;
		rrr1 = node1.toElement().attribute("imp1");
		int numpi=rrr1.toInt();
		mass_comm_n[i][0] = numpi;
		rrr1 = node1.toElement().attribute("imp2");
		numpi=rrr1.toInt();
		mass_comm_n[i][1] = numpi;
		rrr1 = node1.toElement().attribute("napr");
		numpi=rrr1.toInt(&ok,16);
		mass_comm_n[i][2] = numpi;
		i++;
		if (i>=NUM_COMM) break;
	    }
	    
	    node1 = node1.nextSibling(); 
	}
//        qDebug("IIIIIIIIIIIII3\n");
	num_comm=i;
    }
//    qDebug("pp = %x\n",mass_comm_n[0][0]);
}


void Vench::ReadIni()
{
    QString t;
    int i;
    QFile fl_com;
    QString flname;

    qDebug("readini\n");


    if (fl_kat==0){
    #ifdef LINUX_D
        {
    	    QDir dr;
    	    flname = dr.homePath();
	    flname =flname+"/.config/smartcar/";
    	    flname =flname +NAME_FILE_INI;
//    qDebug()<<flname;
    	    
    	    QFile mfl(flname);
	    if (!mfl.exists()){
    		flname ="/etc/smartcar/";
    		flname =flname +NAME_FILE_INI;
    	    }
        }
    #endif
    #ifdef WIN_D
            flname =NAME_FILE_INI;
    #endif
    }
    else{
	flname =katalog;
	flname = flname+NAME_FILE_INI;
    }

    #ifdef PRINT_DEBUG
    qDebug()<<flname;
    qDebug("%s\n",katalog);
    #endif

    fl_com.setFileName(flname);
    
    if (fl_com.open(QIODevice::ReadOnly | QIODevice::Text )==false){
    #ifdef PRINT_DEBUG
        qDebug()<<"Error open smart.ini!\n";
    #endif
//	print_nastr_file(1);
//        qDebug()<<"Create new  smart.ini\n";
	return ;
    }
    
    
    QTextStream tt( &fl_com);

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
        t= tt.readLine();
        i++;
        if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
              printf("Error read ini-file\n");
    #endif
              return;
        }
    }
    #ifdef PRINT_DEBUG
    qDebug("Dir");
    qDebug()<<t;
    #endif
    	QDir dr;

        ServerDir->setText(t);
        ui->ServerDir_n->setText(t);

        fileHlp = t; //chtoby potom ne zapomint put...
    #ifndef LINUX_D
        int num_sl = t.length();
        QString mtt = t.mid(num_sl-1,1);
//        if (mtt!="\\") t = t+"\\";


        ServerDir->setText(t);
        ui->ServerDir_n->setText(t);
        dr.mkpath(ServerDir->text());
    #endif


    #ifdef LINUX_D
    if (fl_kat==0){
    	flname = dr.homePath();
        flname =flname+"/.config/smartcar/";
        ServerDir->setText(flname);
        ui->ServerDir_n->setText(flname);
        dr.mkpath(flname);
        
    #ifdef PRINT_DEBUG
        qDebug(" ");
        qDebug("WARNING: home dir = ");
        qDebug()<<flname;
        qDebug(" ");
    #endif        

    }
    else{
	dr.mkpath(ServerDir->text());
    }

    #endif

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
         t= tt.readLine();
         i++;
         if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
                printf("Error read ini-file\n");
    #endif
                return;
         }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("file name kumirr");
    qDebug()<<t;
    #endif

//    f_name_kum = ServerDir->text();
//    f_name_kum =f_name_kum+ t;
	f_name_kum = t;
    

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
         t= tt.readLine();
         i++;
         if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
                printf("Error read ini-file\n");
    #endif
                return;
         }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug()<<t;
    #endif

	f_name_xml = t;

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
         t= tt.readLine();
         i++;
         if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
                printf("Error read ini-file\n");
    #endif
                return;
         }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug()<<t;
    #endif

	f_name_json = t;

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
         t= tt.readLine();
         i++;
         if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
                printf("Error read ini-file\n");
    #endif
                return;
         }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug()<<t;
    #endif

    f_name_snap = t;


    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
          t= tt.readLine();
          i++;
          if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
            printf("Error read ini-file\n");
    #endif
            return;
          }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug()<<t;
    #endif
    
    bool ok;
    
        n_imp_kum = t.toInt(&ok,16);
        def_diskr_impr = t.toInt(&ok,16);
      
    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
        t= tt.readLine();
        i++;
        if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          printf("Error read ini-file\n");
    #endif
          return;
        }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug()<<t;
    #endif
        
        n_imp_kum_p = t.toInt(&ok,16);
        def_diskr_impl = t.toInt(&ok,16);


    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
        t= tt.readLine();
        i++;
        if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
            printf("Error read ini-file\n");
    #endif
            return;
        }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("Adress\n");
    qDebug()<<t;
    #endif
    
    if(!fl_port_s)    addr = t;



    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
        #ifdef PRINT_DEBUG
          printf("Error read ini-file\n");
	#endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("usleep\n");
    qDebug()<<t;
    #endif
        tm_usleep_com = t.toInt();


    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          printf("Error read ini-file\n");
    #endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("imp discr");
    qDebug()<<t;
    #endif
        imp_diskr = t.toInt();



    t= tt.readLine();

    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          qDebug("Error read ini-file\n");
    #endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug()<<t;
    #endif
    addr_win = t;
    #ifdef WIN_D
	if (!fl_port_s)    addr = t;
    #endif


        t= tt.readLine();

        i=0;
        while ((t=="")||(t.indexOf("#")==0)){
          t= tt.readLine();
          i++;
          if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
              qDebug("Error read ini-file\n");
    #endif
              return;
          }
            mSleep(5);
        }
    #ifdef PRINT_DEBUG
        qDebug("fomat prog:");
        qDebug()<<t;
    #endif
            if (t=="START")        fl_begin=1;
            if (t=="SUPER")        fl_begin=3;
            if (t=="SUPER_N")      fl_begin=0;
            if (t=="BASE")         fl_begin=2;

            if (fl_bluetooth) fl_begin =4;

        t= tt.readLine();
        i=0;
        while ((t=="")||(t.indexOf("#")==0)){
          t= tt.readLine();
          i++;
          if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
              qDebug("Error read ini-file\n");
    #endif
              return;
          }
            mSleep(5);
        }
    #ifdef PRINT_DEBUG
        qDebug("read win\n");
        qDebug()<<t;
    #endif
    prog_for_read_win = t;//programm dla chtenia file pod wind

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          qDebug("Error read ini-file\n");
    #endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("read postr grph");
    qDebug()<<t;
    #endif
    progrPostrGRPH = t;//programm dla postroenia graph file pod wind

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          qDebug("Error read ini-file\n");
    #endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("read show grph");
    qDebug()<<t;
    #endif
    progrShowGRPH = t;//programm dla postroenia graph file pod wind

    t= tt.readLine();
    i=0;
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
          #ifdef PRINT_DEBUG
          qDebug("Error read ini-file_\n");
	    #endif          
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG
    qDebug("QQQ");
    qDebug()<<t;
    #endif
    speed_car = t.toInt();//скорость движения машинки?

//////20201123 chtenie file HLP

    t= tt.readLine();
    i=0;
#ifdef PRINT_DEBUG

qDebug("EEE");
qDebug()<<t;
#endif
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          qDebug("Error read ini-file % \n");
    #endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG

    qDebug()<<t;
    #endif

    #ifdef LINUX_D
//    fileHlp = "/.config/smartcar/"+t;//ima file HLP;
    fileHlp = t;//ima file HLP;
    #endif
    #ifdef WIN_D
        fileHlp +="\\"+ t;//ima file HLP;  put ugezadan ranee
    #endif

    #ifdef PRINT_DEBUG

        qDebug()<<fileHlp;
    #endif


//////20210426 chtenie file brouser

    t= tt.readLine();
    i=0;
#ifdef PRINT_DEBUG

//qDebug("EEE");
//qDebug()<<t;
#endif
    while ((t=="")||(t.indexOf("#")==0)){
      t= tt.readLine();
      i++;
      if (i>NUM_STROK) {
    #ifdef PRINT_DEBUG
          qDebug("Error read ini-file % \n");
    #endif
          return;
      }
        mSleep(5);
    }
    #ifdef PRINT_DEBUG

    qDebug()<<t;
    #endif

    #ifdef LINUX_D
//    fileHlp = "/.config/smartcar/"+t;//ima file HLP;
    fileBrs = t;//ima file HLP;
    #endif
    #ifdef WIN_D
//        fileBrs +="\\"+ t;//ima file HLP;  put ugezadan ranee
    #endif

    #ifdef PRINT_DEBUG

        qDebug()<<fileBrs;
    #endif

    fl_com.close();
}



void Vench::chLog()
{
    if (checkLog->isChecked() ==  true) SendComm("log");
    if (checkLog->isChecked() == false) SendComm("ulog");
}

void Vench::chLog_n()
{
//    if (ui->checkLog_n->isChecked() ==  true) checkLog->setChecked( true);
//    if (ui->checkLog_n->isChecked() == false) checkLog->setChecked(false);
    if (ui->checkLog_n->isChecked() ==  true)  SendComm("log");
    if (ui->checkLog_n->isChecked() == false) SendComm("ulog");
}


void Vench::AntiSleep()
{
    SendCommBudil();
//    printf("razbudit\n");
}

#include <qtextcodec.h>

void Vench::ReadCum()
{

 qDebug("read cum\n");
    QString fnamec;

    fnamec= ServerDir->text();
//    qDebug()<<fnamec;
    #ifdef LINUX_D
//    fnamec= fnamec+"/"+f_name_kum;
    fnamec= fnamec+f_name_kum;
    #endif
    #ifdef WIN_D
//    fnamec= fnamec+"\\"+f_name_kum;
    fnamec= fnamec+f_name_kum;
    #endif
    qDebug()<<f_name_kum;

    
    QFile fl_com(fnamec);

    QByteArray rr = fnamec.toLocal8Bit();
    
//    qDebug("fn cum %s\n",rr.constData());

//printf(f_name_kum);
//printf("\n");
//   QFile fl_com("12.kum");
    QString t;
//    char mm[1000];
#ifdef LINUX_D
       QString bc= "нц";
       QString ecc= "кц";
       QString ec= "раз";
       QString vpravo= "вправо";
       QString vlevo= "влево";
       QString vpered= "вверх";
///       QString nazad= "����";
       QString nazad= "вниз";
       QString svet= "свет";
       QString signal= "сигнал";
#endif

#ifdef WIN_D
       QString bc= "��";
       QString ecc= "��";
       QString ec= "���";
       QString vpravo= "������";
       QString vlevo= "�����";
       QString vpered= "�����";
       QString nazad= "����";
       QString svet= "����";
       QString signal= "������";
#endif

       int cicl =0;
       int com_cicl=0;
       int iter=0;	
	
	bool fll=    fl_com.open(QIODevice::ReadOnly);
	
//   fl_com.readLine(t,1000);
 //  fl_com.readLine(t,1000);
    if (fll!= true){
        qDebug("Error open file kumir\n");
        qDebug("Create file kumir\n");
	print_nastr_file(2);
//        QMessageBox::information( this, trUtf8("Error"),
//                                   trUtf8("Error read file*.kum\n"),
//                                   trUtf8("Ok") );

//	return;

	    
    }
    
    QTextStream tt( &fl_com);     
    int i=0;
    int fl=0;
    tt.setCodec(QTextCodec::codecForName("UTF-16LE"));
//    tt.setAutoDetectUnicode( true); //for qt4
//    while(!tt.eof()){
    while(!tt.atEnd()){//for qt4
       t = tt.readLine();
       fl =0;
//     QSString g  = t.local8Bit();

//    QByteArray rr = t.toLocal8Bit();
    
    //qDebug("str %s\n",rr.constData());




       t = t.toLocal8Bit();
       int k = t.indexOf(vpravo);
       if (k>=0)      {
            mass_comm_n[i][0] =  n_imp_kum;
            mass_comm_n[i][1] =  n_imp_kum_p;
            mass_comm_n[i][2] = 6;
            if (cicl==1) com_cicl++;
            fl=1;
		
	    }else{
		int k = t.indexOf(vlevo);
		if (k>=0)      {
		    mass_comm_n[i][0] = n_imp_kum;
		    mass_comm_n[i][1] =  n_imp_kum_p;
		    mass_comm_n[i][2] = 9;
		if (cicl==1) com_cicl++;
		fl=1;
//		    printf("vlevo\n");
		}
		else{
		    int k = t.indexOf(vpered);
		    if (k>=0)      {
			mass_comm_n[i][0] =  n_imp_kum;
			mass_comm_n[i][1] =  n_imp_kum_p;
			mass_comm_n[i][2] = 0x5;
			if (cicl==1) com_cicl++;
			fl=1;
//		    printf("vpered\n");
			
			
		    }
		    else{
			int k = t.indexOf(nazad);
			if (k>=0)      {
			    mass_comm_n[i][0] =  n_imp_kum;
			    mass_comm_n[i][1] = n_imp_kum_p;
			    mass_comm_n[i][2] = 0xA;
			    if (cicl==1) com_cicl++;
			    fl=1;
//                qDebug("nazad\n");
			}

            else{

				int k = t.indexOf(svet);
				if (k>=0)      {
				    mass_comm_n[i][0] =  n_imp_kum;
				    mass_comm_n[i][1] = n_imp_kum_p;
				    mass_comm_n[i][2] = 0xB1;
				    if (cicl==1) com_cicl++;
				    fl=1;
	//			    printf("svet\n");
				}
				else{

                    int k = t.indexOf(signal);
					if (k>=0)      {
					    mass_comm_n[i][0] =  n_imp_kum;
					    mass_comm_n[i][1] = n_imp_kum_p;
					    mass_comm_n[i][2] = 0xC1;
					    if (cicl==1) com_cicl++;
					    fl=1;
		//			    printf("signal\n");
			    		}
					else{
			int k = t.indexOf(bc);
			if (k>=0)      {
			    int f = t.indexOf(ec);
			    if (f>0){

#ifdef LINUX_D
                    QString mrr = t.mid(k+5,f-k-5);
#endif
#ifdef WIN_D
                    QString mrr = t.mid(k+3,f-k-4);

//                    QByteArray rrr = mrr.toLocal8Bit();

//                    qDebug("sssss %s\n",rrr.constData());

#endif
                //			       printf("wwww\n");
				//			       printf(rr);
                int ii = mrr.toInt();
//                qDebug("\n k=%d\n",k);
  //              qDebug("\n f=%d\n",f);
    //            qDebug("\n ii=%d\n",ii);
                com_cicl=0;
				cicl = 1;
				iter =ii;
			    }
			}
			else{
			    int k = t.indexOf(ecc);
			    if (k>=0)      {
				if (cicl==1){
				    cicl=0;
				    int ind = i;
				    

				    for(int y=1;y<iter;y++){
					for(int jjj=0;jjj<com_cicl;jjj++){
					    mass_comm_n[i][0] =  mass_comm_n[ind-com_cicl+jjj][0];
					    mass_comm_n[i][1] = mass_comm_n[ind-com_cicl+jjj][1];
					    mass_comm_n[i][2] = mass_comm_n[ind-com_cicl+jjj][2];
					    i++;
					    if (i>=NUM_COMM) break;
					    
					}
					    if (i>=NUM_COMM) break;
				    }
				    com_cicl=0;
				}
			    }
			    
			}
		    }
		    }
		}
	    }
	}
	}
//	    }
	    
    if (fl==1){
	fl=0;
	i++;
	if (i>=NUM_COMM) break;
   }
       
     //   printf("i = %d\n",i);
  //         printf( "%s\n",  (const char *)t );
       
   }
//        qDebug("IIIIIIIIIIIII4\n");
    num_comm=i;


//    qDebug("num_comm= %d\n",i);
 
//     for(int uu=0;uu<num_comm;uu++){
//        qDebug("m1 =%d\n",		    mass_comm_n[uu][0] );
//        qDebug("m2 =%d\n",		  mass_comm_n[uu][1]);
//        qDebug("m3 =%x\n",			    mass_comm_n[uu][2] );
//    }
     
  //  int kk =fl_com.readLine(mm,1000);
    //printf("kk=%d\n",kk);
  //  fl_com.readLine(t,1000);
//    t = QString::fromUtf8(mm,kk);
   
//	printf("weqw=");
//	for(int k=0;k<300;k++){
//	    printf("%x\n",mm[k]);
//	}
//	printf(t);
//printf("\n1111\n");
}



void Vench::chKum()
{
    //qDebug("RRRRRRRRRRRRRRRRRR\n");
    if (checkKumir->isChecked() ==  true) {
	ReadCum();
    }
//    if (checkLog->isChecked() == false) SendComm("ulog");
}



///DOBAvLENIE  function for rabota s portom



int Vench::printKG(int flf){
    int mid;
    FILE * mmflnd ;
    
    mmflnd =NULL;
    if (flf==0) mmflnd = fopen("DAT1.txt","a+") ;
    if (flf==1) mmflnd = fopen("DAT2.txt","a+") ;
    
    sprintf(sys_com,"chmod 666 %s",DATAFILE);

#ifdef LINUX_D
    mid = system(sys_com);
#endif

    for(mid=0;mid<10;mid++){
    	    fprintf(flnd,";");
    }
    fflush(mmflnd);
    fclose(mmflnd);
    return 1;
}

    //function for open port
int Vench::COMInit(const char* port)
{

    QString pn(port);

#ifdef QSERIAL

    com_port_qts.setPortName(pn);
    if (com_port_qts.open(QIODevice::ReadWrite)){
	com_port_qts.setBaudRate(QSerialPort::Baud9600);
	com_port_qts.setDataBits(QSerialPort::Data8);
	com_port_qts.setFlowControl(QSerialPort::NoFlowControl);
	com_port_qts.setStopBits(QSerialPort::TwoStop);
	flcomport=1;
    
    }
#endif


#ifndef QSERIAL

#ifdef LINUX_D
    if( (
    com_port = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK,0)) <0)
      {
          printf("ERROR Cannot open %s\n",port);
          flcomport =0;
   return -1;
      }
    else // Set Parametr
    {
    flcomport=1;
    tcgetattr(com_port, &options);
    cfsetispeed(&options, B9600); // skorost v baitax
    options.c_cflag |= (CLOCAL | CREAD);
       // PARODD - proverka na nechetnost
    options.c_cflag &= ~PARENB; // bez kontrola chetnosti
    // options.c_cflag |= (PARENB|PARODD); // s kontrolem chetnosti
    options.c_cflag &= ~CSTOPB; // 1 stop bit
// options.c_cflag |= CSTOPB; // 2 stop bita
            options.c_cflag &= ~CSIZE;
     options.c_cflag |= CS8;
     options.c_cflag &= ~CRTSCTS;
     options.c_lflag &= ~(ICANON | ECHO | ISIG);
     options.c_cc[VTIME]=0;
     options.c_cc[VMIN]=1;
       // zapretit preobrazovanie PC -- VK
       options.c_iflag &= ~INLCR;
     options.c_iflag &= ~ICRNL;
     options.c_oflag &= ~ONLCR;
     options.c_oflag &= ~OCRNL;
       // zapretit programmnoe upravlenie peredachei
       options.c_iflag &= ~IXON;
     options.c_iflag &= ~IXOFF;
       tcsetattr(com_port, TCSANOW, &options);
             fcntl(com_port, F_SETFL, O_NONBLOCK);
  }
      //printf("***** COM initialization sucsessfull! *****\n" ;
    return 1;
#endif

#ifdef WIN_D

    char size = 8;
    char * portname = new char[strlen(port)+1];
      strcpy(portname,port);
//      printf("com = %s\n",portname);
//      hCOM=CreateFile((WCHAR*)portname,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
      hCOM=CreateFileA(portname,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
//        hCOM=CreateFile(portname,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (portname) delete [] portname;
    if (hCOM!=INVALID_HANDLE_VALUE)
{
    qDebug()<<"Open com port\n";

   //         cout << " Open COM1" << endl;

//       Application->MessageBox("���� ������ �������","���",MB_OK|MB_ICONSTOP);

    overlapped.hEvent = CreateEvent(NULL,  true,  true, NULL); //������� ���������� ������-������� ��� ����������� ��������
    SetCommMask(hCOM, EV_RXCHAR); //���������� ����� �� ������������ �� �������                   ������ ����� � ����


}
    else {
//        Application->MessageBox("���������� ������� ����","������",MB_OK|MB_ICONSTOP);
    qDebug()<<"Error open com port\n";
            return 0;
//        cout << "Error Open COM1" << endl;

    }


    dcb.DCBlength = sizeof(dcb);
    if(!GetCommState(hCOM, &dcb))   {
        return 0;
    //      Application->MessageBox("������ �������� ����������","������",MB_OK|MB_ICONSTOP);
    }
    else{
        dcb.BaudRate = 9600;
        dcb.ByteSize = size;
        dcb.Parity = NOPARITY;
        dcb.StopBits = TWOSTOPBITS;
            if(!SetCommState (hCOM, &dcb))
            {
                return 0;
    //              Application->MessageBox("������ �������� ����������","������",MB_OK|MB_ICONSTOP);
            }
      }

      // ������������� ����-��� �� �������� ������ ���������� ������� �� COM-�����
      if(!GetCommTimeouts (hCOM, &rCommTimeouts))
      {
        return-1;
      }
      else{
        // ��������������� ����-��� - 4 ���. (� �������)
          rCommTimeouts.ReadTotalTimeoutConstant = 1000;
          rCommTimeouts.ReadTotalTimeoutMultiplier = 0;
          if (!SetCommTimeouts (hCOM, &rCommTimeouts))
          {
                return-1;
          }
      }

      flcomport =1;
      //reader = NULL;

#endif

#endif



return 1;
}
      //----------------------------------------------------------------
// proveriaem est li na vhode neprochitannie baiti i vozvrashaem ix kolichestvo
int Vench::try_read_port(){
#ifndef QSERIAL
#ifdef LINUX_D
    int retval;
    fd_set ReadFds;
    struct  timeval tv;
    
    tv.tv_sec=1;
    tv.tv_usec=1000;
    FD_ZERO(&ReadFds);// 
    FD_SET(com_port,&ReadFds);
//    retval=select(FD_SETSIZE,&ReadFds,NULL,NULL,NULL);// esli net dannih budet gdat vechno
    retval=select(FD_SETSIZE,&ReadFds,NULL,NULL,&tv);// esli net dannih budet gdat vrema tv
//    printf("ret=%d\n",retval);
    if(retval>0){
	return 1;
    }
    if (retval<0)         COMClose();

//    int cbt;
//
//    if (ioctl (com_port,FIONREAD,&cbt)==0){
//      return cbt;
//    }


//    return 1;
    return -1;
#endif
    return 1;
#endif
}

int Vench::Write(int port, unsigned char *buf, int len){

    if (fl_print_log_B){
        print_log(buf,len,1);
    }

#ifdef QSERIAL
int r;
    r = port;//chtob ne rugalsa ob neispolzavanii
    if (!fl_print_log_B) print_log(buf,len,1);
    r = com_port_qts.write((char *)buf,len);
    if (r<0) {
        COMClose();
	fl_ans3_ust=1;
    }

return r;

#endif

#ifndef QSERIAL

#ifdef  LINUX_D
    ssize_t ret;
    int r;
    if (!fl_print_log_B)     print_log(buf,len,1);
    ret= write(port,buf,len);
    r = (int)ret;
    if (ret<0) {
        COMClose();
	fl_ans3_ust=1;
    }
    printf("Write port = %d\n",r);
    return r;
#endif

#ifdef WIN_D
    if (!fl_print_log_B)     print_log(buf,len,1);
    int  ret =0;
    unsigned long hh;
               DWORD signal; //temp - ����������-��������
//        SuspendThread( reader);
    overlappedwr.hEvent = CreateEvent(NULL,  true,  true, NULL); //������� �������
    if(!WriteFile (hCOM,      // ����� �����
         buf,              // ��������� �� ������������ ������
//             sizeof(dt),       // ������ ������������� �������
         len,       // ������ ������������� �������
         &hh,        // ��������� �� ���������� ������������ ����
         &overlappedwr)             )// pointer to structure for overlapped I/O);
    {
        //qDebug("Write: hh =%d len = %d\n",hh,len);
            return -1;
    }
    signal = WaitForSingleObject(overlappedwr.hEvent, INFINITE); //������������� �����, ���� �� ���������� ������������� �������� WriteFile

//      ResumeThread( reader);

    ret =hh;

    qDebug("Write: ret =%d len = %d\n",ret,len);

    if (ret !=len) {
        if (ret<0) {
            COMClose();
            fl_ans3_ust=1;
        }
    
        return -1;
    }
    return ret;

#endif
#endif

return 0;
}

int Vench::print_imp_sleep(unsigned char *buf_mac_set,int imp1,int imp2,int napr, int sleep)
{

    FILE * mflnd;
//    int k;
    char nf[50];
//    k=0;
    QByteArray rr;
    sprintf(nf,"ANS_%x%x%x%x%x%x%x%x.txt",buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);
#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	rr = flname.toLocal8Bit();

    
        //flnl = fopen(rr.constData(),"w+") ;


	mflnd = fopen(rr.constData(),"w+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"w+") ;
#endif
//printf("mlfnd=%d\n",mflnd);
    
    if (sleep==1 )fprintf(mflnd,"sleep");
    else fprintf(mflnd,"imp %d %d %x",imp1,imp2,napr);
    
    fclose(mflnd);

#ifdef LINUX_D
    sprintf(sys_com,"chmod 666 %s",rr.constData());
    int k=system(sys_com);
//printf("oo3\n");
    k++;
    k=0;
#endif
//    system(sys_com);
    return 1;
}



int Vench::print_spec_file_t(unsigned char *buf_mac_set,double data)
{

    FILE * mflnd;
    int k;
    char nf[50];
    k=0;
    sprintf(nf,"%x%x%x%x%x%x%x%xt.txt",buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);

#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	QByteArray rr = flname.toLocal8Bit();

    
//        flnl = fopen(rr.constData(),"w+") ;


    mflnd = fopen(rr.constData(),"w+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"w+") ;
#endif


#ifdef LINUX_D

    sprintf(sys_com,"chmod 666 %s",rr.constData());
    k = system(sys_com);
#endif
	time(&ltime); 
    k = ltime;

    fprintf(mflnd,"%d %f",k,data);
    
    fclose(mflnd);
    return 1;
}


int Vench::print_spec_file_e(unsigned char *buf_mac_set,double data)
{

    FILE * mflnd;
    int k;
    char nf[50];
    k=0;
    sprintf(nf,"%x%x%x%x%x%x%x%xe.txt",buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);


#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	QByteArray rr = flname.toLocal8Bit();

    
//        flnl = fopen(rr.constData(),"w+") ;


    mflnd = fopen(rr.constData(),"w+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"w+") ;
#endif




#ifdef LINUX_D

    sprintf(sys_com,"chmod 666 %s",rr.constData());
    k=system(sys_com);
#endif
    time(&ltime); 
    k=ltime;
    fprintf(mflnd,"%d %f",k,data);
    
    fclose(mflnd);
    return 1;
}

int Vench::print_spec_file_o(unsigned char *buf_mac_set,double data)
{

    FILE * mflnd;
    int k;
    char nf[50];
    k=0;
    sprintf(nf,"%x%x%x%x%x%x%x%xo.txt",buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);


#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	QByteArray rr = flname.toLocal8Bit();

    
//        flnl = fopen(rr.constData(),"w+") ;


    mflnd = fopen(rr.constData(),"w+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"w+") ;
#endif


#ifdef LINUX_D

    sprintf(sys_com,"chmod 666 %s",rr.constData());
    k=system(sys_com);
#endif
    time(&ltime); 
    k=ltime;
    fprintf(mflnd,"%d %f",k,data);
    
    fclose(mflnd);
    return 1;
}

int Vench::print_spec_file_d(unsigned char *buf_mac_set,double data)
{

    FILE * mflnd;
    int k;
    char nf[50];
    k=0;
    sprintf(nf,"%x%x%x%x%x%x%x%xd.txt",buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);


#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	QByteArray rr = flname.toLocal8Bit();

    
//        flnl = fopen(rr.constData(),"w+") ;


    mflnd = fopen(rr.constData(),"w+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"w+") ;
#endif


#ifdef LINUX_D

    sprintf(sys_com,"chmod 666 %s",rr.constData());
    k=system(sys_com);
#endif
    time(&ltime); 
    k=ltime;
    fprintf(mflnd,"%d %f",k,data);
    
    fclose(mflnd);
    return 1;
}


int Vench::print_nastr_file(int pr)
{

    FILE * mflnd;
    int k;
    char nf[50];
    k=0;
    if (pr==0)sprintf(nf,"%s","tmp_smart.ini");
    if (pr==1)sprintf(nf,"%s","smart.ini");

    QByteArray rr;
    rr = f_name_kum.toLocal8Bit();
    if (pr==2)sprintf(nf,"%s",(rr.constData()));

#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	rr = flname.toLocal8Bit();

    
//        flnl = fopen(rr.constData(),"w+") ;


    mflnd = fopen(rr.constData(),"w+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"w+") ;
#endif


#ifdef LINUX_D

    sprintf(sys_com,"chmod 666 %s",rr.constData());
    k=system(sys_com);
#endif
    if (pr==2){
	fclose(mflnd);
        return 1;
    }
    k++;
    	QString    date;
    	
        fprintf(mflnd,"# HOME directory\n");
    	date = ServerDir->text();
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");



        fprintf(mflnd,"# program KUMIR\n");

	date = f_name_kum;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

        fprintf(mflnd,"# program XML\n");

	date = f_name_xml;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");


        fprintf(mflnd,"# kol-vo impulsov\n");

//        n_imp_kum = t.toInt(&ok,16);
//        def_diskr_impr = t.toInt(&ok,16);
	date.setNum(def_diskr_impr,16);
	rr = date.toLocal8Bit();
        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

	date.setNum(def_diskr_impl,16);
	rr = date.toLocal8Bit();
        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

        fprintf(mflnd,"# port for Linux\n");
	date = addr;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");
//	qDebug()<<date;


	date.setNum(tm_usleep_com,10);
	rr = date.toLocal8Bit();
        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");
	

	date.setNum(imp_diskr,10);
	rr = date.toLocal8Bit();
        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

        fprintf(mflnd,"# port for windows\n");
	date = addr_win;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");


        fprintf(mflnd,"# status : START | BASE | SUPER\n");
	if (fl_begin==3) date="SUPER_N";
	if (fl_begin==0) date="SUPER";
	if (fl_begin==1) date="START";
	if (fl_begin==2) date="BASE";

	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");
        
        fprintf(mflnd,"# program for read file win\n");
	date = prog_for_read_win;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

        fprintf(mflnd,"# program for postroenie graphikov pod win\n");
	date = progrPostrGRPH;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

        fprintf(mflnd,"# program for prosmotra graphikov pod win\n");
	date = progrShowGRPH;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");

        fprintf(mflnd,"# speed car\n");
	date.setNum(speed_car,10);;
	rr = date.toLocal8Bit();

        fprintf(mflnd,"%s",rr.constData());
        fprintf(mflnd,"\n");



    
    fclose(mflnd);
    return 1;
}





int Vench::print_data(unsigned char *buf_mac_set,double data, int pr){

//return t1;
#ifdef WIN_D
    flnd = fopen(DATAFILE,"a+") ;
#endif  
QByteArray rr;  
#ifdef LINUX_D

    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +DATAFILE;
	
	
	rr = flname.toLocal8Bit();

    
        flnd = fopen(rr.constData(),"a+") ;

#endif

//    sprintf(sys_com,"chmod 666 %s",DATAFILE);
    sprintf(sys_com,"chmod 666 %s",rr.constData());
//    system(sys_com);
#ifdef LINUX_D
    int k=system(sys_com);
    k++;
    k=0;
#endif
    print_time(flnd);

    for(id=0;id<10;id++){
	fprintf(flnd," %X",buf_mac_set[id]);
	if (id==7){
    	    fprintf(flnd,";");
	}
    }
    fprintf(flnd,";");
    if (pr==1){
        fprintf(flnd," ;");
    }
    if (pr==2){
        fprintf(flnd," ; ;");
    }
    if (pr==3){
        fprintf(flnd," ; ; ;");
    }
    if (pr==4){
        fprintf(flnd," ; ; ; ;");
    }
    if (pr==5){
        fprintf(flnd," ; ; ; ; ;");
    }
    fprintf(flnd," %0.2f",data);
    fprintf(flnd,";");
    if (pr==0){
        fprintf(flnd," t\n");

    }

    if (pr==1){
        fprintf(flnd," e\n");
    }
    if (pr==2){
        fprintf(flnd," o\n");

    }
    if (pr==3){
        fprintf(flnd," d\n");
    }
    if (pr==4){
        fprintf(flnd," b\n");
//	qDebug("Bar \n");
    }
    if (pr==5){
        fprintf(flnd," v\n");
//	qDebug("Bar \n");
    }
    fflush(flnd);
    fclose(flnd);
    return 1;
}


int Vench::print_data_s(unsigned char *buf_mac_set,double data, int pr){
    FILE * mflnd;

    int cel, drob;
    char nf[50];
    sprintf(nf,"DATA%x%x%x%x%x%x%x%x.txt",buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);

#ifdef LINUX_D
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +nf;
	QByteArray rr = flname.toLocal8Bit();

    
//        flnl = fopen(rr.constData(),"w+") ;


    mflnd = fopen(rr.constData(),"a+") ;
#endif
#ifdef WIN_D
    mflnd = fopen(nf,"a+") ;
#endif

#ifdef LINUX_D

//    sprintf(sys_com,"chmod 666 %s",nf);

    sprintf(sys_com,"chmod 666 %s",rr.constData());
    cel = system(sys_com);
#endif
    print_time(mflnd);

    print_time_s(mflnd);

    for(id=0;id<10;id++){
	fprintf(mflnd," %X",buf_mac_set[id]);
	if (id==7){
    	    fprintf(mflnd,";");
	}
    }
    fprintf(mflnd,";");



    if (pr==1){
        fprintf(mflnd," ;");
    }

    if (pr==2){
        fprintf(mflnd," ; ;");
    }

    if (pr==3){
        fprintf(mflnd," ; ; ;");
    }
    if (pr==4){
        fprintf(mflnd," ; ; ; ;");
    }
    if (pr==5){
        fprintf(mflnd," ; ; ; ; ;");
    }

    
//    fprintf(mflnd," %0.1f",data); // tak vivodit v file s razdel celoi & drobnoi chasti - ','

//budem delat tochku
    cel = (int)data;
    drob = (int)((data - cel)*100.0);
    fprintf(mflnd," %d.%d",cel,drob);
    
    fprintf(mflnd,";");
    if (pr==0){
        fprintf(mflnd," t\n");
    }

    if (pr==1){
        fprintf(mflnd," e\n");
    }
    if (pr==2){
        fprintf(mflnd," o\n");
    }
    if (pr==3){
        fprintf(mflnd," d\n");
    }
    if (pr==4){
        fprintf(mflnd," b\n");
    }
    if (pr==5){
        fprintf(mflnd," vl\n");
    }
    fflush(mflnd);
    fclose(mflnd);
    return 1;
}


int Vench::print_log(unsigned char *buf,int len,int pr)
{
//#ifdef PRINT_LOG
    if (fl_print_log!=1) return 0;
//return 1;

#ifdef WIN_D
    flnl = fopen(LOGFILE,"a+") ;
//    qDebug("print log");
//    qDebug()<<LOGFILE;
#endif

#ifdef LINUX_D

    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +LOGFILE;
	QByteArray rr = flname.toLocal8Bit();

    
    flnl = fopen(rr.constData(),"a+") ;
#endif
//    sprintf(sys_com,"chmod 666 %s",LOGFILE);
//    system(sys_com);

    print_time(flnl);
    if (pr==0){
	fprintf(flnl,"Read :");
    }
    if (pr==1){
	fprintf(flnl,"Write:");
    }
    for(il=0;il<len;il++){
	if (buf[il]>0xF) fprintf(flnl,"%X ",buf[il]);
	else fprintf(flnl,"0%X ",buf[il]);
    }
    fprintf(flnl,"\n");

    fflush(flnl);
    fclose(flnl);
//#endif    
    return 1;
}


int Vench::print_time(FILE *fln)
{ //pechataem vremia v poniatnom formate
//    return 1;
//	time(&ltime); 
//	newtime=localtime(&ltime); /*vremia*/
	time(&ltime); 
	newtime=localtime(&ltime); // podgotavlivaem vremia
	Y = newtime->tm_year+1900;
	M = newtime->tm_mon+1;
	D = newtime->tm_mday;
	h = newtime->tm_hour;
	m = newtime->tm_min;
	s = newtime->tm_sec;
//	fprintf(fln,"\n%d:%.2d:%.2d; %.2d:%.2d:%.2d; ",Y,M,D,h,m,s); 
	fprintf(fln,"%d:%.2d:%.2d; %.2d:%.2d:%.2d; ",Y,M,D,h,m,s); 
    return 1;
}

int Vench::print_time_s(FILE *fln)

{ //pechataem vremia v poniatnom formate
    int kt;
//    return 1;
//	time(&ltime); 
//	newtime=localtime(&ltime); /*vremia*/
	time(&ltime); 
//	newtime=localtime(&ltime); // podgotavlivaem vremia
//	M = newtime->tm_mon+1;
//	D = newtime->tm_mday;
//	h = newtime->tm_hour;
//	m = newtime->tm_min;
//	s = newtime->tm_sec;
//	kt = s +m*60 + h*60*60 + D*24*60*60;

//	fprintf(fln," %d; ",kt); 
	kt=ltime;
	fprintf(fln," %d; ",kt); 
    return 1;
}






// function chitaet vse data is porta i scladivaet v massiv buf_data_rs;

int Vench::readbuf(){

    int BytesRead;
    int i;
    char buf[501];
    


#ifdef QSERIAL
forever {
    
    BytesRead = 0;
    BytesRead = com_port_qts.read(buf,400);

    if (BytesRead>0){
	if ((BytesRead+kol_data)<MAX_DATA_MAS){
	    for(i=kol_data;i<BytesRead+kol_data;i++){
		buf_data_rs[i] = (unsigned char)( buf[i-kol_data]);
	    }
	    kol_data = kol_data+BytesRead;
//	    printf("kol_data=%d\n",kol_data);
	}
	else { // peredelet!!! esli perepolnenie ochishaem massiv
	    kol_data = 0;
	}
    }
    if (BytesRead <= 0 && !com_port_qts.waitForReadyRead(50)) break;
}  
#endif  
    

#ifndef QSERIAL
#ifdef LINUX_D

//    printf("trybread port\n");
    if (try_read_port()>0){
    
//    printf("read port\n");
    BytesRead = read(com_port,buf,400);
//	    for (int j=0;j <BytesRead;j++) qDebug(" %x ",buf[j]);
//	    qDebug("\n");
//BytesRead =10;
//buf[0]= 0x7e;
//buf[1]=0x0;
//buf[2]=10;
//buf[3]=0x88;
//buf[4]=10;
//buf[5]=10;
//buf[6]=10;
//buf[7]=10;


    if (BytesRead>0){
	if ((BytesRead+kol_data)<MAX_DATA_MAS){
	    for(i=kol_data;i<BytesRead+kol_data;i++){
		buf_data_rs[i] = buf[i-kol_data];
	    }
	    kol_data = kol_data+BytesRead;
//	    printf("kol_data=%d\n",kol_data);
	}
	else { // peredelet!!! esli perepolnenie ochishaem massiv
	    kol_data = 0;
	}
    }
    if(BytesRead<0){
	COMClose();
	
    }
	return 1;
    }

#endif
#ifdef WIN_D

    COMSTAT comstat; //��������� �������� ��������� �����, � ������ ��������� ������������ ��� ����������� ���������� �������� � ���� ������
    DWORD temp, mask, signal; //���������� temp ������������ � �������� ��������

    WaitCommEvent( hCOM, &mask, &overlapped); //������� ������� ������ ����� (��� � ���� ������������� ��������)
    signal = WaitForSingleObject(overlapped.hEvent, 500); //������������� ����� �� ������� �����
    if(signal == WAIT_OBJECT_0) //���� ������� ������� ����� ���������
    {
            if(GetOverlappedResult( hCOM, &overlapped, &temp,  true)) //���������, ������� �� ����������� ������������� �������� WaitCommEvent
            if((mask & EV_RXCHAR)!=0) //���� ��������� ������ ������� ������� �����
            {
                    ClearCommError( hCOM, &temp, &comstat); //����� ��������� ��������� COMSTAT
                        BytesRead = comstat.cbInQue; //� �������� �� ��� ���������� �������� ������
                    if(    BytesRead) //���� ������������� ���� ����� ��� ������
                    {
                           ReadFile( hCOM, buf, BytesRead, &temp, &overlapped); //��������� ����� �� ����� � ����� ���������
                           if ((BytesRead+kol_data)<MAX_DATA_MAS){
                               for(i=kol_data;i<BytesRead+kol_data;i++){
                               buf_data_rs[i] = buf[i-kol_data];
                               }
                               kol_data = kol_data+BytesRead;
                       //	    printf("kol_data=%d\n",kol_data);
                           }
                           else { // peredelet!!! esli perepolnenie ochishaem massiv
                               kol_data = 0;
                           }
                    }
                    if(BytesRead<0){
			COMClose();
	
		    }

                    return 1;
            }
    }

#endif
#endif

    return 1;
}


// function rashifrovivaet dannie// videlaya odin kadr
int Vench::GetKadrN(unsigned char * buf_s,int *len){
    int i;
    int fl_k;
    int ii;
    unsigned char buf_d[2];
    fl_k=0;
//    printf(" GetKadrN kol_data =%d\n",kol_data);
//    printf(" GetKadrN lk =%d\n",l_k);
    if (kol_data<=0){
	*len = 0;
	l =0;
	fl_n=0;
	l_k=0;
	fl_k = *len;
	return fl_k;
    }

    if (kol_data>0){
	for (i=0;i<kol_data;i++){
	    if (buf_data_rs[i] == 0x7E){
            fl_n=1;
	    }
	    if (fl_n==1){
            buf_s[l] = buf_data_rs[i];
//		qDebug(" %X",buf_s[l]); 
            l++;
	    }
	    if (l==3){// viceplaem dlinu kadra
            buf_d[0]=buf_s[2];
            buf_d[1]=buf_s[1];
            memcpy(&l_k,buf_d,2);
		l_k = l_k+4;
//        qDebug("GetKadrN len =%d\n",l_k);

	    }
	    if ((l>=l_k)&&(l>3)){
		fl_k=1;
		break;
	    }
	}
//	printf("\n");
//	printf("GetKadrN i =%d\n",i);


        // sdvigaem ishodnii massiv
	
	if (i>0){
    	    ii=i;
	    for (i=ii;i<kol_data;i++){
		buf_data_rs[i-ii] = buf_data_rs[i];
	    }
	    kol_data = kol_data -ii;
	}
	//end sdvigaem ishodnii massiv
    }
    if (fl_k){
	*len = l_k;
	l =0;
	fl_n=0;
	l_k=0;
	fl_k = *len;
	return fl_k;
    }
    return 0;
}

int Vench::RetParUst(unsigned char *pu,unsigned char *ppu){
    int i;
    int ret;
    ret =0;
    if ((pu[0] == 0xff)&&(pu[1]==0xfe)) return ret;
    ppu[0] = 0xFF;
    ppu[1] = 0xFE;
    for(i=0;i<mkol_ust;i++){
	if ((pu[0] == act_ust[i].set_addr[0])&&(pu[1] == act_ust[i].set_addr[1])){
	    ppu[0] = act_ust[i].parent_addr[0];
	    ppu[1] = act_ust[i].parent_addr[1];
	    ret =1;    
	    break;
	}
    }
    return ret;
}

int Vench::SetParentUst(){
    int i;
    int fl_c;
    int max_num;
    unsigned char pu[2],ppu[2];
    
    max_num=0;
    for(i=0;i<mkol_ust;i++){
	printf(" %d pr[0] = %d pr[1] = %d\n",i,act_ust[i].parent_addr[0], act_ust[i].parent_addr[1]);
        if ((act_ust[i].parent_addr[0]==0xff)&&(act_ust[i].parent_addr[1]==0xfe)){
		act_ust[i].num_u=0;
	}
	else{
	    pu[0] = act_ust[i].parent_addr[0];
	    pu[1] = act_ust[i].parent_addr[1];
    	    fl_c=1;
	    while (fl_c==1){
		fl_c = RetParUst(pu,ppu);
		printf("flc= %d\n",fl_c);
		pu[0] = ppu[0];
		pu[1] = ppu[1];
		if (fl_c>0) act_ust[i].num_u++;
		printf("act_ust[%d].num_u= %d\n",i,act_ust[i].num_u);
	    }
	    
	}
	if (act_ust[i].num_u>max_num){
	    max_num = act_ust[i].num_u;
	}
    }
    
    for(i=0;i<mkol_ust;i++){
	printf("Ust %d  sloy = %d\n",i,act_ust[i].num_u);
    }
    return max_num;
}

int Vench::NewActUst(){
    int i,j;
    QByteArray rr;
//    if (fl_al!=0) {
//	printf("Error active log file (otkritv!!!!)\n");
//	return 0;
//    }
//    printf("FL += %d\n",fl_logm); 
    
//    fl_logm=NULL;
#ifdef WIN_D
    fl_logm = fopen(ACTFILE,"w+");
#endif
#ifdef LINUX_D
    if (fl_kat==0){
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +ACTFILE;
	 rr = flname.toLocal8Bit();

    
        fl_logm = fopen(rr.constData(),"w+") ;
    }
    else{
        fl_logm = fopen(ACTFILE,"w+");
    }
#endif
    
    
//    printf("FL1 += %d\n",fl_logm); 
    fl_al=1;
//    if (fl_logm<=0){
    if (fl_logm==NULL){
    	printf("ERROR : open file ADD_LOG\n");
        fl_al=0;
	return 0;
    }
//    fl_al=0;
//    fclose(fl_logm);
//    return 0;

    if (fl_kat==1)   sprintf(sys_com,"chmod 666 %s",ACTFILE);
    else sprintf(sys_com,"chmod 666 %s",rr.constData());

#ifdef LINUX_D
    i = system(sys_com);
#endif

    for(i=0;i<mkol_ust;i++){
	for (j=0;j<8;j++) fprintf(fl_logm," %X",act_ust[i].mac_addr[j]);
	fprintf(fl_logm,";");
	fprintf(fl_logm," %X",act_ust[i].set_addr[0]);
	fprintf(fl_logm," %X",act_ust[i].set_addr[1]);
	fprintf(fl_logm,";");
	fprintf(fl_logm," %X",act_ust[i].parent_addr[0]);
	fprintf(fl_logm," %X",act_ust[i].parent_addr[1]);
	fprintf(fl_logm,";\n");
    }
    
    fclose(fl_logm);
    fl_logm=NULL;
    fl_al=0;
    return 1;
}




//------------------------------------------InitKdg
int Vench::InitKdg(unsigned char *buf)
{
	unsigned char buf_mac[8],buf_set[2], buf_parent[2];
	int fl_itend;
	unsigned char mk;
	int im;
	int k;
	if (buf[3] == 0x95) {
            qDebug("resieve 0x95\n");


	    GetMacSetAddr(buf,buf_mac,buf_set, buf_parent);


	}
	else{
	    if ((buf[3] == 0x88)&&(buf[2]==0x19)){
		if ((buf[5] == 0x4e)&&(buf[6]==0x44)){
		    for (im=0;im<8;im++){
			buf_mac[im] = buf[im+10];
		    }
		    buf_set[0] = buf[8];
		    buf_set[1] = buf[9];
		    buf_parent[0] = buf[20];
		    buf_parent[1] = buf[21];
		
            qDebug("resieve 0x88\n");
		    
		    
		}
		else return -1;
	    }
	    else return -1;
	}
	fl_itend=0;
	for(k=0;k<mkol_ust;k++){
		fl_itend =1;
	        for(im=0;im<8;im++){//sravnenie po mac-adresu
		    mk = (unsigned char)(buf_mac[im]);
		    if( act_ust[k].mac_addr[im] != mk) fl_itend=0;	

		}
		if(fl_itend==1){
			    break;// esli proizoshlo uspeshnoe sravnivanie
		}
	}
//printf("initkdg mkol ust =%d\n",mkol_ust);
//printf("initkdg kol ust =%d\n",kol_ust);
//qDebug("InitKdg: fk_itend=%d\n",fl_itend);
		
	if (fl_itend ==1){
	    act_ust[k].set_addr[0] = buf_set[0];
	    act_ust[k].set_addr[1] = buf_set[1];
//	    act_ust[mkol_ust].fl_sleep=1;//pochmy mkol_ust/????????
	    act_ust[k].fl_sleep=1;//pochmy mkol_ust/????????

        if (act_ust[k].fl_antisleep ==1) {
            act_ust[k].fl_antisleep =2;
            if (fl_ans3_ust!=1) fl_ans3_ust=1;
//            qDebug()<<"KKKKKKKK";
            SetHiddenCar(k,act_ust[k].fl_antisleep);

        }

	    
	    SetActiveLog(act_ust[k].mac_addr, act_ust[k].set_addr);

//vr	    act_ust[k].parent_addr[0] = buf_parent[0];
//vr	    act_ust[k].parent_addr[1] = buf_parent[1];
	    
//		    printf("Per oporosa = %d\n",act_ust[k].period_oprosa);
	}
	else{
		//dobavlenie novogo ust
		//qDebug()<<"NEWUSTR";
		    if (mkol_ust<MAX_USTR){
			for(im=0;im<8;im++){
			    act_ust[mkol_ust].mac_addr[im] = (unsigned char)(buf_mac[im]);	
			}	
			act_ust[mkol_ust].set_addr[0] = buf_set[0];
			act_ust[mkol_ust].set_addr[1] = buf_set[1];

			act_ust[mkol_ust].parent_addr[0] = buf_parent[0];
			act_ust[mkol_ust].parent_addr[1] = buf_parent[1];

			act_ust[mkol_ust].period_oprosa_d = 0;
			act_ust[mkol_ust].fl_izm_pd = 0;
			act_ust[mkol_ust].period_oprosa_t = 0;
			act_ust[mkol_ust].fl_izm_pt = 0;
        		act_ust[mkol_ust].period_oprosa_e = 0;
	                act_ust[mkol_ust].period_oprosa_o = 0;
	                act_ust[mkol_ust].fl_izm_pe = 0;
			act_ust[mkol_ust].tar_kf = TAR_KF;
			act_ust[mkol_ust].pan1 = 0;
	        	act_ust[mkol_ust].pan2 = 0;
        		act_ust[mkol_ust].fl_pan = 0;


			act_ust[mkol_ust].fl_antisleep =  0;


			act_ust[mkol_ust].impr =  def_diskr_impr; 
			act_ust[mkol_ust].impl =  def_diskr_impl;
			
				
			act_ust[mkol_ust].fl_ans_sleep=0;
			act_ust[mkol_ust].fl_sleep=1;

        		act_ust[mkol_ust].vibr_sp=0;
			
			act_ust[mkol_ust].time_sleep=0;
			act_ust[mkol_ust].p_sleep=0;
			act_ust[mkol_ust].time_oprosa_t=0;
			act_ust[mkol_ust].time_oprosa_e=0;
			act_ust[mkol_ust].fl_oprosa =0;
			act_ust[mkol_ust].fl_temp =0;
			act_ust[mkol_ust].fl_energ =0;
			act_ust[mkol_ust].num_u =0;


			mkol_ust++;
  //          qDebug("pppp1\n");

			ReadUstrINI(buf_mac);// pitaemsa chitati iz file zapomnenie ranee nastroiki (v chastnosti kol imp diskretnogo dvig)

			SetActiveLog(act_ust[mkol_ust-1].mac_addr, act_ust[mkol_ust-1].set_addr);
//printf("pppp2\n");
		    }
	}

//printf("kol ust =%d\n",mkol_ust);

		
	return 1;
}

//pr - priznak raboti nado analiz zasip ili net
int Vench::GetImp(unsigned char *buf,int len, int pr, int * imp1, int *imp2, int *napr)
{
	int ret;
	if (len==0)ret =0;
	if ((buf[3] != 0x90)&&(buf[3] != 0x95))  {
	    return -1;
	}
	ret =-1;
	*imp1=0;
	*imp2=0;
	
//	unsigned char per;
//	per = buf[15];
//	buf[15] = buf[16];
//	buf[16]=per;
//	per = buf[17];
//	buf[17] = buf[18];
//	buf[18]=per;

	if (buf[3] == 0x90){
	    short mmm;
	    memcpy(&mmm,buf+15,2);
    	    *imp1 = mmm;
	    memcpy(&mmm,buf+17,2);
    	    *imp2 = mmm;
    	    *napr = buf[19];

//        *napr = ret_napr;
//		ret_napr = buf[17];
//		qDebug("napr=%x\n",*napr);
//		qDebug("imp1=%d\n",*imp1);
//		qDebug("imp2=%d\n",*imp2);
	    ret =0;
	    if(checkAll->isChecked()==0){
		if (fl_play){
    		    if (fl_play_comm==1){
        		fl_play_comm=0;
    		    }
    		}
    	    }
    	}
    


	if (buf[3] == 0x95){
	    if (pr==1){
		ret =-2;
//		printf("GetImp: sleep\n");
	    }
	}


	return ret;
}


int Vench::GetPANID(unsigned char *buf,int len, unsigned char *pan1,unsigned char *pan2)
{
//	printf("GePanID0 \n");
	if (buf[3] != 0x97) return -1;
	if (len==0) {
	//    return -1;
	}
			
	if ((buf[15]!=0x49)||(buf[16]!=0x44)) return -1; // esli ne temperature
	
	if (buf[2]!=0x17) {
        qDebug("GetPanID : Get panid kontroler in Kodogramm \n");
        PanIdKontr = buf[17];
        qDebug("GetPanID : pn = %x\n",PanIdKontr);

        return -2;
	}
	
//	    printf("GePanID \n");
	
//	    *pan1 = buf[17];
	    *pan1 = buf[25];
	    *pan2 = 0;
//	    *pan2 = buf[23];
	return 1;
}

int Vench::GetTemp(unsigned char *buf,int len, double *temp,double *osv, double trk)
{
	short vdac_c;
	unsigned char bper;
	double vdac_m, tempc;
	unsigned char mbuf[2];
	if (buf[3] != 0x97) return -1;

	if (len==0) bper=0;
			
	tempc =0;
	vdac_m=0;
	vdac_c=0;
	
	if ((buf[15]!=0x49)||(buf[16]!=0x53)) return -1; // esli ne temperature
	
	if (buf[17]!=0) {
	    printf("GetTemp : Error Temp in Kodogramm\n");
	    return -1;
	}
	
	#ifndef FOR_IGOR
	    mbuf[0] = buf[22];
	    mbuf[1] = buf[23];
	#endif
	
	#ifdef FOR_IGOR
	    mbuf[0] = buf[24];
	    mbuf[1] = buf[25];
	#endif
	    bper=mbuf[1];
	    mbuf[1]=mbuf[0];
	    mbuf[0]=bper; // perevorachivaem 2 baita
	    memcpy(&vdac_c,mbuf+0,2);
	
	vdac_m = (float)vdac_c/1023.0*1.2; // v milivoltax
	tempc = -1481.96 + sqrt(2.1962E+6+(1.8639- trk*vdac_m)/3.88E-6); // sam raschet temperaturi v C
	
	*temp = tempc;

    // viceplaem osveshennost
#ifdef FOR_IGOR
    mbuf[0] = buf[26];
    mbuf[1] = buf[27];
#endif

    bper=mbuf[1];
    mbuf[1]=mbuf[0];
    mbuf[0]=bper; // perevorachivaem 2 baita
    memcpy(&vdac_c,mbuf+0,2);


    vdac_m = (float)vdac_c/1023.0*1.2*1000; // v milivoltax
//	tempc = -1481.96 + sqrt(2.1962E+6+(1.8639- trk*vdac_m)/3.88E-6); // sam raschet osvesh v C
    tempc = vdac_m; // sam ras
    *osv = tempc;





    return 1;
}

int Vench::GetOsv(unsigned char *buf,int len, double *temp, double trk)
{
	short vdac_c;
	unsigned char bper;
	double vdac_m, tempc;
	unsigned char mbuf[2];

	if (buf[3] != 0x97) return -1;

	if (len==0)trk=trk;
			
	tempc =0;
	vdac_m=0;
	vdac_c=0;
	
	if ((buf[15]!=0x49)||(buf[16]!=0x53)) return -1; // esli ne temperature
	
	if (buf[17]!=0) {
        qDebug("GetOsv : Error Osv in Kodogramm\n");
	    return -1;
	}
	#ifndef FOR_IGOR
	    mbuf[0] = buf[25];
	    mbuf[1] = buf[26];
	#endif
	
	#ifdef FOR_IGOR
	    mbuf[0] = buf[26];
	    mbuf[1] = buf[27];
	#endif
	    bper=mbuf[1];
	    mbuf[1]=mbuf[0];
	    mbuf[0]=bper; // perevorachivaem 2 baita
	    memcpy(&vdac_c,mbuf+0,2);
	
	
	vdac_m = (float)vdac_c/1023.0*1.2; // v milivoltax
//	tempc = -1481.96 + sqrt(2.1962E+6+(1.8639- trk*vdac_m)/3.88E-6); // sam raschet temperaturi v C
	tempc = vdac_m; // sam raschet temperaturi v C
	
	*temp = tempc;
	return 1;
}

int Vench::GetDim(unsigned char *buf,int len, double *temp, double trk)
{
	short vdac_c;
	unsigned char bper;
	double vdac_m, tempc;
	unsigned char mbuf[2];
	
	
	if (len==0) bper=0;
	if (trk==0) bper=0;
		
	if (buf[3] != 0x97) return -1;


			
	tempc =0;
	vdac_m=0;
	vdac_c=0;
	
	if ((buf[15]!=0x49)||(buf[16]!=0x53)) return -1; // esli ne temperature
	
	if (buf[17]!=0) {
	    printf("Getdim : Error dim in Kodogramm\n");
	    return -1;
	}
	#ifndef FOR_IGOR
	    mbuf[0] = buf[27];
	    mbuf[1] = buf[28];
	#endif
	
	#ifdef FOR_IGOR
	    mbuf[0] = buf[28];
	    mbuf[1] = buf[29];
	#endif
	    bper=mbuf[1];
	    mbuf[1]=mbuf[0];
	    mbuf[0]=bper; // perevorachivaem 2 baita
	    memcpy(&vdac_c,mbuf+0,2);
	
	
	vdac_m = (float)vdac_c/1023.0*1.2; // v milivoltax
//	tempc = -1481.96 + sqrt(2.1962E+6+(1.8639- trk*vdac_m)/3.88E-6); // sam raschet temperaturi v C
	tempc = vdac_m; // sam raschet temperaturi v C
	
	*temp = tempc;
	return 1;
}


int Vench::TryGetMash(unsigned char *buf,int len)
{
	int ret=0;
	if (len==0) ret=0;
	ret=1;
//	if ((buf[3] != 0x90)||(buf[3] != 0x95)) return -1;
	if (buf[3] == 0x90) return ret; 
	if (buf[3] == 0x95) return ret;
	//else return -1;

	return -1;
}

int Vench::GetBar(unsigned char *buf,int len, double *temp,double *vlg, double trk)
{
	int ret;
	if (len==0) return -1;
	ret=1;
	if (buf[3] != 0x90) return -1;
	if (len != 26) return -1;
	short vdac_c;
	unsigned char mbuf[2];
	
	
	vdac_c=0;
//    qDebug("GetBar: len == 26\n");
    if (buf[15]!=0xA5) return -1; // esli ne

//    qDebug("GetBar buf[16] = %x\nDolgen byt = 0xA5   !!!!!!!!",buf[16]);

//	mbuf[1]=buf[24];
//	mbuf[0]=buf[25]; // perevorachivaem 2 baita
	mbuf[0]=buf[24];
	mbuf[1]=buf[23]; 
	memcpy(&vdac_c,mbuf+0,2);

	
//	*temp = (double)vdac_c; //v pask 
	
	*temp = (double)vdac_c/100.0;// v kPa

//    mbuf[0]=buf[22];
//    mbuf[1]=buf[21];

    mbuf[0]=buf[18];
    mbuf[1]=buf[17];
    memcpy(&vdac_c,mbuf+0,2);
    
    double t0 =trk;//ctob ne rugalsa o neispolzovanii
    t0 = 0.01*(double)vdac_c-39.7;
     
    mbuf[0]=buf[20];
    mbuf[1]=buf[19];
    memcpy(&vdac_c,mbuf+0,2);
    
//    double rh_izm = -2.0468 + 0.0367*(double)vdac_c - pow(1.5955,-6)*(double)vdac_c*(double)vdac_c;
    double rh_izm = -2.0468 + 0.0367*(double)vdac_c - 1.5955*pow(10.9,-6)*pow((double)vdac_c,0.5);

//    *vlg = -2.0468 + 0.0367*(double)vdac_c - pow(1.5955,-6)*(double)vdac_c*(double)vdac_c;
    *vlg = (t0-25)*(0.01+0.00008*(double)vdac_c)+rh_izm;


    return ret;
}
int Vench::TryGetTemp(unsigned char *buf,int len)
{
	int ret;
	if (len==0)ret=0;
	ret=1;
	if (buf[3] != 0x97) return -1;
//	printf("trygettemp\n");
//	printf("bg = %x  bg = %x\n",buf[15],buf[16]);

	if ((buf[15]==0x49)&&(buf[16]==0x53)) return ret; // esli ne temperature
	else return -1;
//	printf("trygettemp2\n");
	
	return ret;
}

int Vench::TryDatKas(unsigned char *buf,int len,int &dt)//dt==1 - pravii dt==2 - levii
{
	int ret;
	if (len==0)ret=0;
	ret=1;
	dt=0;
	if (buf[3] != 0x90) return -1;
	if (buf[2] != 0x11) return -1; //opredelaem po dline ??? - nepravilno!!!
//	qDebug("trydatkas\n");
//	printf("bg = %x  bg = %x\n",buf[15],buf[16]);

	if (buf[19]==0x4A){
	    dt=1;
	}
	else	if (buf[19]==0x8A){
		    dt=2;
		}
		else return -1;
//	printf("trygettemp2\n");
	
	return ret;
}


int Vench::TryGetPANID(unsigned char *buf,int len)
{
	int ret;
	if (len==0)ret=0;
	ret=1;
	if (buf[3] != 0x97) return -1;
	
//	printf("TryGetPanID\n");
//	printf("bg = %x  bg = %x\n",buf[15],buf[16]);
	if ((buf[15]==0x49)&&(buf[16]==0x44)) ret=1; // esli ne зфт шв
	else ret= -1;
//	printf("ret =%d\n",ret);
	return ret;
}






int Vench::TryGetKrdBegin(unsigned char *buf,int len)
{
	int ret;
	if (len==0)ret =0;
	ret=1;
	if (buf[3] != 0x90) return -1;

//	if ((buf[15]!=0x49)&&(buf[16]!=0x53)) return -1; // esli ne temperature
	return ret;
}

int Vench::GetEnerg(unsigned char *buf,int len, double *temp)
{
	short vdac_c;
	unsigned char bper;
	double vdac_m;
//	double tempc;
	
	if (len==0) bper=0;
	if (buf[3] != 0x97) return -1;
	
//	printf("222\n");
	
	vdac_m=0;
	vdac_c=0;
//	tempc =0;

	if ((buf[15]!=0x25)||(buf[16]!=0x56)) return -1; // esli ne energ

	if (buf[17]!=0) {
	    printf("GetEnerg : Error Energ in Kodogramm\n");
	    return -1;
	}
//	printf("333\n");
	

	bper=buf[19];
	buf[19]=buf[18];
	buf[18]=bper; // perevorachivaem 2 baita

//	bper=buf[20];
//	buf[20]=buf[19];
//	buf[19]=bper; // perevorachivaem 2 baita

//	printf("GetEn: b1 = %X   b2= %X  \n",buf[18],buf[19]);
	memcpy(&vdac_c,buf+18,2);
//	printf("GetEn: vdsc = %x  \n",vdac_c);

///	printf("444\n");
	
	vdac_m = (float)vdac_c/1023.0*1.2; // v milivoltax
//	printf("GetEn: vdsc = %f  \n",vdac_m);
	
	*temp = vdac_m;

	return 1;
}



int Vench::GetMacSetAddr(unsigned char *buf,unsigned char *mac,unsigned char *set, unsigned char *parent)
{
	int i;
//	int fl_m;
	if (buf[3] == 0x95){
		for (i=4;i<12;i++){ //8 bait MAK adres mote
			mac[i-4] = buf[i];
	
		}
		for (i=12;i<14;i++){ // 2 baita net adres
			set[i-12] = buf[i];
		}
		
		for (i=27;i<29; i++){ // 2 baita  adres roditela
			parent[i-27] = buf[i];
		}
	}

	if (buf[3] == 0x88){
	    for (i=0;i<8;i++){
		mac[i] = buf[i+10];
	    }
	    set[0] = buf[8];
	    set[1] = buf[9];
	
	    parent[0] = buf[20];
	    parent[1] = buf[21];
	}


	if (buf[3] == 0x97){
		for (i=5;i<13;i++){ //8 bait MAK adres mote
			mac[i-5] = buf[i];
	
		}
		for (i=13;i<15;i++){ // 2 baita netadres
			set[i-13] = buf[i];
		}
	}
	if (buf[3] == 0x90){
		for (i=4;i<12;i++){ //8 bait MAK adres mote
			mac[i-4] = buf[i];
	
		}
		for (i=12;i<14;i++){ // 2 baita netadres
			set[i-12] = buf[i];
		}
	}
//v etoi procedure ne nado!!
//	fl_m=0;
//	for(i=0;i<8;i++){
//	    if (mac[i]!=0) fl_m=1;
//	}
//	if (fl_m==0) return -2;// dla opredelenia shirokovesh zaprosa
		
	return 1;
}

int Vench::SendZaprosSleep(unsigned char *MacSet,double tm_slp)//
{
	int len_d;
	int i;
	unsigned char buf_d[30];
	unsigned char ktr;
	short mtm;

	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres
 
//    printf("SendZaprosE : MacAddr = ");
	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//    printf("\n");	

	buf_d[15] = 0x2;
	buf_d[16] = 0x53;
	buf_d[17] = 0x50;
	
	mtm = (short)(tm_slp*100.0);
	memcpy(buf_d+18,&mtm,2);
	ktr = buf_d[18];
	buf_d[18]=buf_d[19];
	buf_d[19] = ktr;
//	printf("bf 18 = %x\nbf 19 = %x\n",buf_d[18],buf_d[19]);	
//	buf_d[18] = 0x01; // 500 mlsek
//	buf_d[19] = 0xF4; // 500 mlsek

	ktr =0;
	
	for(i=3;i<20;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[20] = 0xFF-ktr;
	len_d = 21; //chislo bait
	buf_d[2] = 17;//15; //
 //otprablaem kadr v port
	i= Write(com_port,buf_d,len_d);
	return i;
} //end SendZapros

int Vench::SendSetSlpPZU(unsigned char *MacSet)// zapros
{
	int mlen_d;
	int mi;
	unsigned char mbuf_d[40];
	unsigned char mktr;

	
	mlen_d=0;
       //nachalo kadra
	mbuf_d[0] = 0x7E;
    //dlina
	mbuf_d[1]=0;
	mbuf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	mbuf_d[3] = 0x17;
    //nomer freima
	mbuf_d[4]=1;
    //mak-adres

    printf("SendSetSlpTm : MacAddr = ");
	for(mi=0;mi<10;mi++){
		mbuf_d[mi+5] = MacSet[mi];
//		printf("%X ",MacSet[mi]);
	}
    printf("\n");	

	

/*
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
	buf_d[11] = 0xFF;
	buf_d[12] = 0xFF;
	buf_d[13] = 0xfe;
	buf_d[14] = 0xff;

*/
	mbuf_d[15] = 0x2;
	mbuf_d[16] = 0x57;
	mbuf_d[17] = 0x52;
	
	
	mktr =0;
	
	for(mi=3;mi<18;mi++){
		mktr=mktr+mbuf_d[mi];
	}
	mbuf_d[18] = 0xFF-mktr;
	mlen_d = 19;// obshay dlina kadra
	mbuf_d[2] = 15;//dlina bez 4
 //otprablaem kadr v port
	mi= Write(com_port,mbuf_d,mlen_d);
	return mi;
} //end SendSetSlpTPZU


int Vench::SendSetSleep(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int mlen_d;
	int mi;
	unsigned char mbuf_d[40];
	unsigned char mktr;

	
	mlen_d=0;
       //nachalo kadra
	mbuf_d[0] = 0x7E;
    //dlina
	mbuf_d[1]=0;
	mbuf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	mbuf_d[3] = 0x17;
    //nomer freima
	mbuf_d[4]=1;
    //mak-adres

    printf("SendSetSlpTm : MacAddr = ");
	for(mi=0;mi<10;mi++){
		mbuf_d[mi+5] = MacSet[mi];
//		printf("%X ",MacSet[mi]);
	}
    printf("\n");	
	

/*
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
	buf_d[11] = 0xFF;
	buf_d[12] = 0xFF;
	buf_d[13] = 0xfe;
	buf_d[14] = 0xff;

*/
	mbuf_d[15] = 0x2;
	mbuf_d[16] = 0x53;
	mbuf_d[17] = 0x4D;
	mbuf_d[18] = 0x4;
	
	mktr =0;
	
	for(mi=3;mi<19;mi++){
		mktr=mktr+mbuf_d[mi];
	}
	mbuf_d[19] = 0xFF-mktr;
	mlen_d = 20;// obshay dlina kadra
	mbuf_d[2] = 16;//dlina bez 4
 //otprablaem kadr v port
	mi= Write(com_port,mbuf_d,mlen_d);
	return mi;
} //end SendSetSleep

int Vench::SendSetAlarm(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int mlen_d;
	int mi;
	unsigned char mbuf_d[40];
	unsigned char mktr;

	
	mlen_d=0;
       //nachalo kadra
	mbuf_d[0] = 0x7E;
    //dlina
	mbuf_d[1]=0;
	mbuf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	mbuf_d[3] = 0x17;
    //nomer freima
	mbuf_d[4]=1;
    //mak-adres

    printf("SendSetAlarm : MacAddr = ");
	for(mi=0;mi<10;mi++){
		mbuf_d[mi+5] = MacSet[mi];
//		printf("%X ",MacSet[mi]);
	}
    printf("\n");



/*
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
	buf_d[11] = 0xFF;
	buf_d[12] = 0xFF;
	buf_d[13] = 0xfe;
	buf_d[14] = 0xff;

*/
	mbuf_d[15] = 0x2;
	mbuf_d[16] = 0x53;
	mbuf_d[17] = 0x4D;
	mbuf_d[18] = 0x0;
	
	mktr =0;
	
	for(mi=3;mi<19;mi++){
		mktr=mktr+mbuf_d[mi];
	}
	mbuf_d[19] = 0xFF-mktr;
	mlen_d = 20;// obshay dlina kadra
	mbuf_d[2] = 16;//dlina bez 4
 //otprablaem kadr v port
	mi= Write(com_port,mbuf_d,mlen_d);
	return mi;
} //end SendSetSleep


int Vench::SendZaprosEAll()

{//shirokoveshat zapros temp

	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;
	
	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
	buf_d[11] = 0xFF;
	buf_d[12] = 0xFF;
	buf_d[13] = 0xfe;
	buf_d[14] = 0xff;
	buf_d[15] = 0x2;
	buf_d[16] = 0x25;
	buf_d[17] = 0x56;
	ktr =0;
	for(i=3;i<18;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[18] = 0xFF-ktr;
	len_d = 19;
	buf_d[2] = 15;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

	printf("Send Zapros Energ ALL\n");

	return i;
}


int Vench::SendZaprosI()
{//shirokoveshat zapros temp

	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;
	
	if (flcomport!=1) return 0;
	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x8;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	buf_d[5] = 0x4e;
	buf_d[6] = 0x44;
	buf_d[7] = 0;
	ktr =0;
	for(i=3;i<7;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[7] = 0xFF-ktr;
	len_d = 8;
	buf_d[2] = 4;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);
	
//	mkol_ust =0;//w
    qDebug("Send Zapros I\n");
	return i;
}

int Vench::SendZaprosReset()
{//shirokoveshat zapros temp

	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;
	
	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x8;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	buf_d[5] = 0x4e;
	buf_d[6] = 0x52;
	buf_d[7] = 1;
	ktr =0;
	for(i=3;i<8;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[8] = 0xFF-ktr;
	len_d = 9;
	buf_d[2] = 5;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

	printf("Send Zapros Reset\n");
	return i;
}

int Vench::ReadUstrINI(unsigned char *Mac){
    int  k = GetNumberUst(Mac);
//    qDebug("readini k=%d\n",k);


    QString fname;
    fname="";
    fname =  ServerDir->text();

    for (int i=0;i<8;i++){
        QString st;
        st.setNum(Mac[i],16);
        fname = fname+st;
    }
    fname = fname+".ini";

//    qDebug() << fname;
    
    QFile fl_com(fname);
    QString t;
    int i;


    if (fl_com.open(QIODevice::ReadOnly | QIODevice::Text )==false){
        qDebug("ReadUstrIni: error open file data\n");
        return 0;
    }
    

    QTextStream tt( &fl_com);

    t= tt.readLine();
    i=0;
    while(t==""){
        t= tt.readLine();
        i++;
        if (i>NUM_STROK) {
              printf("Error read ini-file\n");
              return 0;
        }
        mSleep(5);
    }
//    qDebug()<<t;

    act_ust[k].impr = t.toInt();
//    qDebug("ReadUstrINI: fff\n");
//    qDebug()<<    act_ust[k].impr;
    
    t= tt.readLine();
    i=0;
    while(t==""){
        t= tt.readLine();
        i++;
        if (i>NUM_STROK) {
              printf("Error read ini-file\n");
              return 0;
        }
        mSleep(5);
    }
    
    
    act_ust[k].impl = t.toInt();

    return 1;
}

int Vench::SaveUstrINI(unsigned char *Mac){
    int  k = GetNumberUst(Mac);
    QString fname;
    fname="";
    fname =  ServerDir->text();
    for (int i=0;i<8;i++){
	QString st;
	st.setNum(Mac[i],16);
	fname = fname+st;
    }
    fname = fname+".ini";
    
    QByteArray rr = fname.toLocal8Bit();
//    qDebug("qqq   %s\n",rr.constData());

    
    FILE* fff;
    fff = fopen(fname.toLocal8Bit(),"w+");

    fprintf(fff,"%d\n",act_ust[k].impr);
    fprintf(fff,"%d\n",act_ust[k].impl);
    fflush(fff);
    fclose(fff);
    return 1;
}

int Vench::SendVklAPI()//regim api bez esc
{
    return SendVklAPI(1);
}

int Vench::SendVklAPI(int api_e)//api_e==1 - vkl api bez esc? api_e==2 - vkl api s esc? api+e==0 - otkl esc
{
    int len_d;
    int i;
    unsigned char buf_d[20];
    unsigned char ktr;

    if (flcomport!=1) return 0;



    len_d=0;
       //nachalo kadra

    buf_d[0] = 0x7E;
    //dlina
    buf_d[1]=0;
    buf_d[2]=0;
    //komanda
    buf_d[3] = 0x8;
    //nomer freima
    buf_d[4]=1;
    //mak-adres,set-adres
    buf_d[5] = 0x41;
    buf_d[6] = 0x50;
    buf_d[7] = api_e;
    buf_d[8] = 0;
    ktr =0;
    for(i=3;i<8;i++){
        ktr=ktr+buf_d[i];
    }
    buf_d[8] = 0xFF-ktr;
    len_d = 9;
    buf_d[2] = 5;
    i= Write(com_port,buf_d,len_d);
    return i;
}

int Vench::SendVklNP()
{
    int len_d;
    int i;
    unsigned char buf_d[20];
    unsigned char ktr;

    if (flcomport!=1) return 0;



    //S EToi KoMANDOI IDET PRAV PAKeT!!!!
    len_d=0;
       //nachalo kadra

    buf_d[0] = 0x7E;
    //dlina
    buf_d[1]=0;
    buf_d[2]=0;
    //komanda
    buf_d[3] = 0x9;
    //nomer freima
    buf_d[4]=1;
    //mak-adres,set-adres
    buf_d[5] = 0x4E;
    buf_d[6] = 0x4F;
    buf_d[7] = 0;//2 - otvechaet normal 95 paketom, 0 - stalo toge+net 95 paketa ot koord - to est 0 - pravilno?
    buf_d[8] = 0;
    ktr =0;
    for(i=3;i<8;i++){
        ktr=ktr+buf_d[i];
    }
    buf_d[8] = 0xFF-ktr;
    len_d = 9;
    buf_d[2] = 5;
    i= Write(com_port,buf_d,len_d);
    return i;
}

int Vench::SetKolImpUstr(unsigned char *Mac,int impr,int impl){
    int  k = GetNumberUst(Mac);
    act_ust[k].impr = impr;
    act_ust[k].impl = impl;

    
    SaveUstrINI(Mac);

    return 1;
}

int Vench::GetKolImpUstr(int *Mac,int *impr,int *impl){
    unsigned char mmac[8];
//    qDebug("qqqqqq\n");
    for(int i=0;i<8;i++){
	mmac[i] = (unsigned char)Mac[i];
//    qDebug("mm = %d\n",mmac[i]);
    }
    int  k = GetNumberUst(mmac);
//    qDebug ()<< k;
    *impr = act_ust[k].impr;
    *impl = act_ust[k].impl;

    return 1;
}

int Vench::SendZapisPanID_KONTR(int pn)

{

	int len_d;
	int i;
	unsigned char buf_d[40];
	unsigned char ktr;
	
	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x8;
    //nomer freima
	buf_d[4]=0x01;
	buf_d[5] = 0x49;
	buf_d[6] = 0x44;
	buf_d[7] = (unsigned char)pn;
	ktr =0;
	for(i=3;i<8;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[8] = 0xFF-ktr;
	len_d = 9;
	buf_d[2] = 5;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

    qDebug("Send comm smenit PANID kontr \n");
///
///	mSleep(30);
///
///	printf("Send comm sohranit PANID \n");
	return i;
}

int Vench::SendZapisPanID(unsigned char *Mac,int pn)

{

	int len_d;
	int i;
	unsigned char buf_d[40];
	unsigned char ktr;
	
    qDebug("smenit Pan ID\n");

    if (flcomport!=1)return 0;



	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=0x01;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = Mac[i];
	}
    buf_d[13] = 0xff;
	buf_d[14] = 0xfe;
	buf_d[15] = 0x2;
	buf_d[16] = 0x49;
	buf_d[17] = 0x44;
    //buf_d[18] = 0x00;
    buf_d[18] = (unsigned char)pn;
	ktr =0;
    for(i=3;i<19;i++){
		ktr=ktr+buf_d[i];
	}
    buf_d[19] = 0xFF-ktr;
    len_d = 20;
	buf_d[2] = 16;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

    qDebug("Send comm smenit PANID \n");

//	mSleep(30);
//	printf("Send comm sohranit PANID \n");
	return i;
}

int Vench::SendZaprosPANID(unsigned char *Mac)

{

	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;
	

    if (flcomport!=1)return 0;

	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=0x01;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = Mac[i];
	}
	buf_d[13] = 0xff;
	buf_d[14] = 0xfe;
	buf_d[15] = 0x2;
	buf_d[16] = 0x49;
	buf_d[17] = 0x44;
	ktr =0;
	for(i=3;i<18;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[18] = 0xFF-ktr;
	len_d = 19;
	buf_d[2] = 15;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

    qDebug("Send Zapros PANID \n");
	return i;
}

int Vench::SendZaprosPANIDAll()

{//shirokoveshat zapros panID

	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;
	
    if (flcomport!=1)return 0;

	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=0x01;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
	buf_d[11] = 0xFF;
	buf_d[12] = 0xFF;
	buf_d[13] = 0xfe;
	buf_d[14] = 0xff;
	buf_d[15] = 0x2;
	buf_d[16] = 0x49;
	buf_d[17] = 0x44;
	ktr =0;
	for(i=3;i<18;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[18] = 0xFF-ktr;
	len_d = 19;
	buf_d[2] = 15;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

	printf("Send Zapros PANID ALL\n");
	return i;
}

int Vench::SendZaprosTAll()

{//shirokoveshat zapros temp

	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;
	
    if (flcomport!=1)return 0;

	len_d=0;
       //nachalo kadra
	
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0;
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
	buf_d[11] = 0xFF;
	buf_d[12] = 0xFF;
	buf_d[13] = 0xfe;
	buf_d[14] = 0xff;
	buf_d[15] = 0x2;
	buf_d[16] = 0x49;
	buf_d[17] = 0x53;
	ktr =0;
	for(i=3;i<18;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[18] = 0xFF-ktr;
	len_d = 19;
	buf_d[2] = 15;
	i= Write(com_port,buf_d,len_d);
//	mSleep(50);

    qDebug("Send Zapros Temp ALL\n");
	return i;
}


int Vench::SendZaprosT(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int len_d;
	int i;
	unsigned char buf_d[40];
	unsigned char ktr;

	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
//	buf_d[11] = 0xFF;
//	buf_d[12] = 0xFF;
//	buf_d[13] = 0xfe;
//	buf_d[14] = 0xff;

	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//	printf("\n");

	buf_d[15] = 0x2;
	buf_d[16] = 0x49;
	buf_d[17] = 0x53;
	ktr =0;
	for(i=3;i<18;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[18] = 0xFF-ktr;
	len_d = 19;
	buf_d[2] = 15;
	
//	for(i=0;i<len_d;i++){
//        qDebug(" %X",buf_d[i]);
//	}
    qDebug("SendZaprosT  \n");

	i= Write(com_port,buf_d,len_d);
	return i;
}

int Vench::SendZaprosDim(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
    unsigned char mbuf[4];
    mbuf[0] = 0x44;
    mbuf[1] = 0x31;
    mbuf[2] = 0x04;
    SendZaprosComm(MacSet,mbuf,3);
    flzaprosdim =1;
//    sleep(1);
//    mSleep(100);
//    SendZaprosT(MacSet);// zapros konkretnogo ustroistva
    if (flzaprostemposv==0) SendCommTempOn();
    //    printf("dddd\n");
	return 1;
}

int Vench::SendZaprosOtklDim(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
    unsigned char mbuf[4];
    mbuf[0] = 0x44;
    mbuf[1] = 0x31;
    mbuf[2] = 0x05;
    SendZaprosComm(MacSet,mbuf,3);
    flzaprosdim =0;
    if (flzaprostemposv==0) SendCommTempOff();
//    sleep(1);
    
	return 1;
}


int Vench::SendZaprosComm(unsigned char *MacSet,unsigned char *cm, int len)// zapros konkretnogo ustroistva
{
	int len_d;
	int i;
	unsigned char buf_d[25];
	unsigned char ktr;

	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
//	buf_d[11] = 0xFF;
//	buf_d[12] = 0xFF;
//	buf_d[13] = 0xfe;
//	buf_d[14] = 0xff;

	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//	printf("\n");

	buf_d[15] = 0x2;

        for(i=0;i<len;i++){
	    buf_d[16+i] = cm[i];
	}
	ktr =0;
	for(i=3;i<16+len;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[16+len] = 0xFF-ktr;
	len_d = 16+len+1;
	buf_d[2] = len_d-4;
	
//	for(i=0;i<len_d;i++){
//	    printf(" %X",buf_d[i]);
//	}
	printf("\n");
    qDebug("Send Zapros Comm\n");
	i= Write(com_port,buf_d,len_d);
	return i;
}



int Vench::SendCommStopKrd(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int len_d;
	int i;
	unsigned char buf_d[25];
	unsigned char ktr;
	int lenm;
	lenm=1;
	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x10;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
//	buf_d[11] = 0xFF;
//	buf_d[12] = 0xFF;
//	buf_d[13] = 0xfe;
//	buf_d[14] = 0xff;

	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//	printf("\n");

	buf_d[15] = 0x2;

        for(i=0;i<lenm;i++){
	    buf_d[16+i] = 0x52;
	}
	ktr =0;
	for(i=3;i<16+lenm;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[16+lenm] = 0xFF-ktr;
	len_d = 16+lenm+1;
	buf_d[2] = len_d-4;
	
//	for(i=0;i<len_d;i++){
//	    printf(" %X",buf_d[i]);
//
//}
//	printf("\n");
	printf("Send Comm Stop Krd\n");
	i= Write(com_port,buf_d,len_d);
	return i;
}


int Vench::SendCommStartKrd(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int len_d;
	int i;
	unsigned char buf_d[25];
	unsigned char ktr;
	int lenm;
	lenm=1;
	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x10;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
//	buf_d[11] = 0xFF;
//	buf_d[12] = 0xFF;
//	buf_d[13] = 0xfe;
//	buf_d[14] = 0xff;

	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//	printf("\n");

	buf_d[15] = 0x2;

        for(i=0;i<lenm;i++){
	    buf_d[16+i] = 0x51;
	}
	ktr =0;
	for(i=3;i<16+lenm;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[16+lenm] = 0xFF-ktr;
	len_d = 16+lenm+1;
	buf_d[2] = len_d-4;
	
//	for(i=0;i<len_d;i++){
//	    printf(" %X",buf_d[i]);
//	}
//	printf("\n");
	printf("Send Comm Start Krd\n");
	i= Write(com_port,buf_d,len_d);
	return i;
}


int Vench::SendDriveStart(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;

	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
//	buf_d[11] = 0xFF;
//	buf_d[12] = 0xFF;
//	buf_d[13] = 0xfe;
//	buf_d[14] = 0xff;

	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//	printf("\n");

	buf_d[15] = 0x2;
	buf_d[16] = 0x44;
	buf_d[17] = 0x31;
	buf_d[18] = 0x05;
	
	ktr =0;
	for(i=3;i<19;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[19] = 0xFF-ktr;
	len_d = 20;
	buf_d[2] = 16;
	
	
	printf("SendDriveStart\n");
	for(i=0;i<len_d;i++){
	    printf(" %X",buf_d[i]);
	}
	printf("\n");

	i= Write(com_port,buf_d,len_d);
	return i;
}


int Vench::SendDriveStop(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
	int len_d;
	int i;
	unsigned char buf_d[20];
	unsigned char ktr;

	
	len_d=0;
       //nachalo kadra
	buf_d[0] = 0x7E;
    //dlina
	buf_d[1]=0;
	buf_d[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_d[3] = 0x17;
    //nomer freima
	buf_d[4]=1;
    //mak-adres,set-adres
	for(i=0;i<8;i++){
		buf_d[i+5] = 0;
	}
//	buf_d[11] = 0xFF;
//	buf_d[12] = 0xFF;
//	buf_d[13] = 0xfe;
//	buf_d[14] = 0xff;

	for(i=0;i<10;i++){
		buf_d[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//	printf("\n");

	buf_d[15] = 0x2;
	buf_d[16] = 0x44;
	buf_d[17] = 0x31;
	buf_d[18] = 0x04;
	
	ktr =0;
	for(i=3;i<19;i++){
		ktr=ktr+buf_d[i];
	}
	buf_d[19] = 0xFF-ktr;
	len_d = 20;
	buf_d[2] = 16;
	
	printf("SendDriveStop\n");
	for(i=0;i<len_d;i++){
	    printf(" %X",buf_d[i]);
	}
	printf("\n");

	i= Write(com_port,buf_d,len_d);
	return i;
}



int Vench::SendZaprosE(unsigned char *MacSet)// zapros konkretnogo ustroistva energii
{
	int len_d;
	int i,rr;
	unsigned char ktr;

	
	len_d=0;
       //nachalo kadra
	buf_zd[0] = 0x7E;
    //dlina
	buf_zd[1]=0;
	buf_zd[2]=0x10; // dlina freima bez 4-x bait
    //komanda
	buf_zd[3] = 0x17;
    //nomer freima
	buf_zd[4]=1;
    //mak-adres
 
//    printf("SendZaprosE : MacAddr = ");
	for(i=0;i<10;i++){
		buf_zd[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
	}
//        printf("\n");	

	buf_zd[15] = 0x2;
	buf_zd[16] = 0x25;
	buf_zd[17] = 0x56;

	ktr =0;
	for(i=3;i<18;i++){
		ktr=ktr+buf_zd[i];
	}
	buf_zd[18] = 0xFF-ktr;
	len_d = 19;
	buf_zd[2] = 15;



//	for(i=0;i<len_d;i++){
//	    qDebug(" %X",buf_zd[i]);
//	}
//	qDebug("\n");

 //otprablaem kadr v port
	rr= Write(com_port,buf_zd,len_d);
	return rr;
} //end SendZaprosE



int Vench::SendZaprosO(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
    int len_d;
    int i,rr;
    unsigned char ktr;


    len_d=0;
       //nachalo kadra
    buf_zd[0] = 0x7E;
    //dlina
    buf_zd[1]=0;
    buf_zd[2]=0x10; // dlina freima bez 4-x bait
    //komanda
    buf_zd[3] = 0x17;
    //nomer freima
    buf_zd[4]=1;
    //mak-adres

    qDebug("SendZaprosO \n");
    for(i=0;i<10;i++){
        buf_zd[i+5] = MacSet[i];
//		printf("%X ",MacSet[i]);
    }
//        printf("\n");

    buf_zd[15] = 0x2;
    buf_zd[16] = 0x49;//25
    buf_zd[17] = 0x53;//56

    ktr =0;
    for(i=3;i<18;i++){
        ktr=ktr+buf_zd[i];
    }
    buf_zd[18] = 0xFF-ktr;
    len_d = 19;
    buf_zd[2] = 15;



//	for(i=0;i<len_d;i++){
//	    qDebug(" %X",buf_zd[i]);
//	}
//	qDebug("\n");

 //otprablaem kadr v port
    rr= Write(com_port,buf_zd,len_d);
    return rr;
} //end SendZaprosO


//vozvrashaet nomer ustr v massive po ego mac-adresu
int Vench::GetNumberUst(unsigned char *mac){
    int i;
    int k;
    int fl_itend;
    

    for(i=0;i<mkol_ust;i++){
        fl_itend =1;
        for(k=0;k<8;k++){
		    if( act_ust[i].mac_addr[k] != mac[k]) fl_itend=0;
//	    printf("%x  == %x \n",act_ust[i].mac_addr[k], mac[k]);
        }
//	printf("fl_i=%d\n",fl_itend);
        if (fl_itend==1) break;
    }
    
    if (i==mkol_ust) i=-1;

//    qDebug("getnumberust i =%d\n",i);
    
    if (i==-1){
        fl_itend=0;
        for(k=0;k<8;k++){
            if ((mac[k]!=0)&&(mac[k]!=0xFF)) fl_itend=1;
//	    qDebug("mk == %x \n", mac[k]);
	}
	if (fl_itend==0) i=-2;// dla opredelenia shirokovesh zaprosa
//	qDebug("fl_i=%d\n",fl_itend);
    }

//    qDebug("1getnumberust i =%d\n",i);
    
    return i;
}



int Vench::ReadTarFile(){
    int fs;
    unsigned char ss[2];
    unsigned char macb[8];
    int mt;
    int ii;
    float trk;
#ifdef LINUX_D   
    	QDir dr;
    	QString    flname = dr.homePath();
	flname =flname+"/.config/smartcar/";
    	
    	flname =flname +TARFILE;
	QByteArray rr = flname.toLocal8Bit();

    
        ftrk = fopen(rr.constData(),"r+") ;
#endif
#ifdef WIN_D
    ftrk = fopen(TARFILE,"r+");
#endif
    if(!ftrk) return 1;
    
    while(1){
	fs = fscanf(ftrk,"%s",ss);
	if (fs<0) break;
	if (ss[0]=='&'){
	    fs = fscanf(ftrk,"%f",&trk);
    	    for (ii=0;ii<8;ii++){
		fs = fscanf(ftrk,"%x",&mt);
		if (fs<0) break;
		macb[ii] = (unsigned char)mt;
	    }


	    ii = GetNumberUst(macb);
	    if (ii>=-1){ 
		act_ust[ii].tar_kf = (double)trk;
	    }
	}
    }
    fclose(ftrk);
    return 1;
}


//pochemu-to ne rabotaet esli obyavit v tele main???????????????
//viletaet posle sled fscanf (flag_com == 0 ???????


// potoc read from port
void  Vench::TestPotoc(void * param){
    
    double tm_tek;
    int ii,k,mk,fl_sov,dt;
    unsigned char buf_mac_set[20];
    
    Vench*md = (Vench*)param;

    md->mSleep(100);//chtobi ne zavisal??????????????
            
    //printf("Start potoc =%d\n",md->fl_potoc_rs);
    md->ptemp =0;
    md->pvlg =0;
    md->pkk=-1;
    md->imp1=0;
    md->imp2=0;
    
//    md->SendVklAPI();
//    md->SendVklNP();
//    md->SendZaprosI();

    while(md->fl_potoc_rs){
//		    if (md->flzaprostemposv) {
//
//			    for ( int ig = md->Plot_size_tec - 1; ig > 0; ig-- ) {
//			       md->d_yt[ig] = md->d_yt[ig-1];
//			       md->d_yo[ig] = md->d_yo[ig-1];
//			    }
//				if (md->Plot_size_tec<PLOT_SIZE){
//				    md->Plot_size_tec++;
//				}
//			    md->d_yt[0] = md->Plot_size_tec+1;
//		        qDebug(" potoc \n");
//		        qDebug(" %d \n",md->Plot_size_tec);
//		        qDebug(" %f \n",md->d_yt[1]);
//
//
//		    }
//		    sleep(2);
//		    continue;

        md->mSleep(2);
//    qDebug(" potoc \n");
    if (md->flcomport==0) continue;
//    qDebug("Rab potoc \n");
//for analiz periodich komand
	if (md->fl_period){
	    tm_tek = md->GetTime();
	    if (tm_tek> md->t_oprosa){
		    md->SendZaprosTAll();
		    md->t_oprosa = md->GetTime()+md->period_oprosa_t;
	    }
	}

	if (md->fl_period_e){
	    tm_tek = md->GetTime();
	    if (tm_tek> md->t_oprosa_e){
		    md->SendZaprosEAll();
		    md->t_oprosa_e = md->GetTime()+md->period_oprosa_e;
	    }
	}
	
	
	//period zapros davl (poka dla vseh)
	if (md->flzaprosbar){
	    tm_tek = md->GetTime();
	    if (tm_tek> md->t_oprosa_b){
		    md->SendCommBarOn();
		    md->t_oprosa_b = md->GetTime()+md->period_oprosa_b;
	    }
	
	}
	
	for(ii=0;ii<md->kol_ust;ii++){
	    if (md->act_ust[ii].period_oprosa_t!=0){// esli neobhodimo oprosit ustr-vo
            if (md->act_ust[ii].fl_oprosa>KOL_ANSW){// esli eto ustr-vo eshe ne oprashivali
                if(md->act_ust[ii].fl_izm_pt==0){
                md->act_ust[ii].period_oprosa_t = md->act_ust[ii].period_oprosa_t*10.0;
                md->act_ust[ii].fl_izm_pt=1;
                printf("error temp\n");
                }
    //			printf("error temp\n");

            }
            if (md->act_ust[ii].fl_oprosa>(2*KOL_ANSW)){// esli eto ustr-vo eshe ne oprashivali
                printf("error temp\n");
                md->act_ust[ii].fl_oprosa=KOL_ANSW;// chtob ne bilo perepolnenia
            }
            tm_tek = md->GetTime();
            if (md->act_ust[ii].time_oprosa_t<tm_tek){ // esli prishlo vrema oprosa
                for(k=0;k<8;k++){
                    buf_mac_set[k] = md->act_ust[ii].mac_addr[k];
                }
                buf_mac_set[8] = md->act_ust[ii].set_addr[0];
                buf_mac_set[9] = md->act_ust[ii].set_addr[1];
                md->SendZaprosT(buf_mac_set);
                md->act_ust[ii].fl_oprosa++;
                md->act_ust[ii].time_oprosa_t = md->GetTime()+md->act_ust[ii].period_oprosa_t;
                if (md->act_ust[ii].fl_izm_pt==1){
                    printf("error temp\n");
                }
            }
	    }
	    
	    if (md->act_ust[ii].period_oprosa_e!=0){// esli neobhodimo oprosit ustr-vo
            if (md->act_ust[ii].fl_oprosa>KOL_ANSW){// esli eto ustr-vo eshe ne oprashivali
                if(md->act_ust[ii].fl_izm_pe==0){
                md->act_ust[ii].period_oprosa_e = md->act_ust[ii].period_oprosa_e*10.0;
                md->act_ust[ii].fl_izm_pe=1;
                printf("error energ\n");
                }

            }
            if (md->act_ust[ii].fl_oprosa>(2*KOL_ANSW)){// esli eto ustr-vo eshe ne oprashivali
                printf("error energ\n");
                md->act_ust[ii].fl_oprosa=KOL_ANSW;// chtob ne bilo perepolnenia
            }
            tm_tek = md->GetTime();
            if (md->act_ust[ii].time_oprosa_e<tm_tek){ // esli prishlo vrema oprosa
                for(k=0;k<8;k++){
                    buf_mac_set[k] = md->act_ust[ii].mac_addr[k];
                }
                buf_mac_set[8] = md->act_ust[ii].set_addr[0];
                buf_mac_set[9] = md->act_ust[ii].set_addr[1];
                md->SendZaprosE(buf_mac_set);
                md->act_ust[ii].fl_oprosa++;
                md->act_ust[ii].time_oprosa_e = md->GetTime()+md->act_ust[ii].period_oprosa_e;
                if (md->act_ust[ii].fl_izm_pe==1){
                    printf("error energ\n");
                }
            }
        }

        if ( md->act_ust[ii].period_oprosa_o>0){
            tm_tek = md->GetTime();
            if ((md->act_ust[ii].time_oprosa_o<tm_tek)){ // esli prishlo vrema oprosa

                for(k=0;k<8;k++){
                    buf_mac_set[k] = md->act_ust[ii].mac_addr[k];
                }
                buf_mac_set[8] = md->act_ust[ii].set_addr[0];
                buf_mac_set[9] = md->act_ust[ii].set_addr[1];

//                md->SendZaprosO(buf_mac_set);// - ne nado tk zaprashivaem temp!!!
    //            md->act_ust[ii].fl_oprosa++;
                md->act_ust[ii].time_oprosa_o = md->GetTime()+md->act_ust[ii].period_oprosa_o;
            }
        }

	    
	}
// end for analiz periodich komand


	if (md->err==1){
//	    printf("kol_data = %d\n",kol_data);
	}
    md->readbuf(); // chtenie dannih
/*

    //7E 0 20 95 0 13 A2 0 40 5C EB 9E 7F 8C 2 7F 8C 0 13 A2 0 40 5C EB 9E 20 0 FF FE 1 3 C1 5 10 1E 89
    md->buf_data_rs[0] = 0x7E;
    md->buf_data_rs[1] = 0x0;
    md->buf_data_rs[2] = 0x20;
    md->buf_data_rs[3] = 0x95;
    md->buf_data_rs[4] = 0x0;
    md->buf_data_rs[5] = 0x13;
    md->buf_data_rs[6] = 0xa2;
    md->buf_data_rs[7] = 0x0;
    md->buf_data_rs[8] = 0x40;
    md->buf_data_rs[9] = 0x5C;
    md->buf_data_rs[10] = 0xEB;
    md->buf_data_rs[11] = 0x9e;
    md->buf_data_rs[12] = 0x7f;
    md->buf_data_rs[13] = 0x8c;
    md->buf_data_rs[14] = 0x2;
    md->buf_data_rs[15] = 0x7f;
    md->buf_data_rs[16] = 0x8c;
    md->buf_data_rs[17] = 0x0;
    md->buf_data_rs[18] = 0x13;
    md->buf_data_rs[19] = 0xa2;
    md->buf_data_rs[20] = 0x0;
    md->buf_data_rs[21] = 0x40;
    md->buf_data_rs[22] = 0x5c;
    md->buf_data_rs[23] = 0xeb;
    md->buf_data_rs[24] = 0x9e;
    md->buf_data_rs[25] = 0x20;
    md->buf_data_rs[26] = 0x0;
    md->buf_data_rs[27] = 0xff;
    md->buf_data_rs[28] = 0xfe;
    md->buf_data_rs[29] = 0x1;
    md->buf_data_rs[30] = 0x3;
    md->buf_data_rs[31] = 0xc1;
    md->buf_data_rs[32] = 0x5;
    md->buf_data_rs[33] = 0x10;
    md->buf_data_rs[34] = 0x1E;
    md->buf_data_rs[35] = 0x89;

    md->plen=36;
    md->kol_data =36;

*/
	if (md->GetKadrN(md->pbuf_data,&(md->plen))>0){// esli est dannie dla chenia
//        qDebug("Kadr est\n");




	    md->print_log(md->pbuf_data,md->plen,0);
//        qDebug("000Init new ust\n");
	    if (md->InitKdg(md->pbuf_data)>0){// esli eto kodogramma inicializacii
		
				;//neobhodiomo opisat! zapis v fail
      //      printf("Init new ust\n");
//            qDebug("Init new ust\n");
    		md->NewActUst();
		
		
		if (md->fl_auto_wait==1){
		    md->fl_auto_succ=1;
//		    printf("Success open port mashinka !\n");
		    printf("%s\n",md->addr_auto[md->sch_auto]); // smotrim kakoe ustroistvo otkrilos

		}
//	    return NULL;
	    }	 
	    else{
		    // obrabotchik mashinki
//		    if (GetImp(unsigned char *buf,int len, int pr, int * imp1, int *imp2)
//	    	printf("fffff\n");

		// obrabotka datchika kasania
		if (md->TryDatKas(md->pbuf_data,md->plen,dt)>0){//dt==1 - pravii dt==2 - levii
		    
		    if (dt==1) md->DatKas(1);
		    if (dt==2) md->DatKas(0);
    
		}



            if(md->TryGetMash(md->pbuf_data,md->plen)>0){
                md->GetMacSetAddr(md->pbuf_data,md->pbuf_mac,md->pbuf_set, md->pbuf_parent);
                md->pbuf_mac[8] = md->pbuf_set[0];
                md->pbuf_mac[9] = md->pbuf_set[1];

                // esli eto ustroistvo is spiska to sbrasivaem flag oprosa
                //chtobi oprosil sleduyshii
                md->pkk = md->GetNumberUst(md->pbuf_mac);
                md->errte = md->GetImp(md->pbuf_data,md->plen,1,&(md->imp1),&(md->imp2),&(md->napr));

                if (md->errte==-2) md->errte =1;
			

                fl_sov=1;
                for(mk=0;mk<8;mk++){
                    if (md->pbuf_mac[mk]!=md->mac_addr_prg[mk]){
                        fl_sov=0;
                    }

                }
                if (fl_sov==1){
                    md->print_imp_sleep(md->pbuf_mac,md->imp1, md->imp2,md->napr, md->errte);
//			    qDebug("ooo1=%x\n",md->napr);
                    
                    if (md->checkDiskr->isChecked()==0) md->SetAnswerUstr(md->imp1, md->imp2,md->napr);
                }
			if (md->errte == 1) {
			    //printf("Ansver Mash sleep\n\n");
			}
			else {
//			    printf("Ansver Mash imp  err==%d\n\n",errte);
//			    printf("mac 2  err==%d\n\n",pbuf_mac[1]);
			    
			}
		    }
	    
		    //obrabotchik inf kardiografa
		    if (md->fl_krd==1){
			if (md->fl_krd_start==0){
			    if (md->TryGetKrdBegin(md->pbuf_data,md->plen)>0){
				md->fl_krd_start=1;
				md->fl_stop=0;
				md->fl_pr_stop =0;
				md->fl_bt_kr=0;
				md->sh_mas1=0;
				md->sh_mas2=0;
				md->fl_mas=0;
				//continue;
			    }
			}
			else{
			    // idet inf s dannimi
			    for(md->kf=0;md->kf<md->plen;md->kf++){
			    // uslovie dla konca faila data
				if (md->fl_stop==0){ 
				    if (md->pbuf_data[md->kf] ==0xE6) md->fl_stop++;
				}
				if (md->fl_stop==1){ 
				    if (md->pbuf_data[md->kf] ==0x80) md->fl_stop++;
				    else md->fl_stop=0;
				}
				if (md->fl_stop==2){ 
				    if (md->pbuf_data[md->kf] ==0xE6) md->fl_stop++;
				    else md->fl_stop=0;
				}
				if (md->fl_stop==3){ 
				    if (md->pbuf_data[md->kf] ==0x80) {
					md->fl_stop++;
				    }
				    else md->fl_stop=0;
				}
				if (md->fl_stop==4){
				    md->fl_pr_stop=1;
				    break;
				}
				
			    // end uslovie dla konca faila data
				if(md->fl_bt_kr==0){
				    if (md->fl_mas==0) md->mas_1[md->sh_mas1] = md->pbuf_data[md->kf];
				    md->sh_mas1++;
				    md->fl_bt_kr=1;
				}

				if(md->fl_bt_kr==1){
				    if (md->fl_mas==0) md->mas_2[md->sh_mas2] = md->pbuf_data[md->kf];
				    md->sh_mas2++;
				    md->fl_bt_kr=0;
				}
				
				if ((md->sh_mas1==NMAS_KG-1)||(md->sh_mas2==NMAS_KG-1)){
				    if (md->fl_mas==0){
					md->fl_mas=1;
					md->sh_mas1=0;
					md->sh_mas2=0;
					md->printKG(0);
					
				    }
				    else{
					md->fl_mas=0;
					md->sh_mas1=0;
					md->sh_mas2=0;
					md->printKG(1);
				    }
				}
			    
				
			    }//			    for(kf=0;kf<plen;kf++){
			    if (md->fl_pr_stop==1){
				if (md->fl_mas==0) md->printKG(0);
				if (md->fl_mas==1) md->printKG(1);
				md->fl_krd_start=0;
			    }

			}//else fl_krd_start
			

		    }
		    //end obrabotchik inf kardiografa
		    
		    if (md->flzaprosbar){//esli bil zapros davl analiziruem estli otvet
            if (md->GetBar(md->pbuf_data,md->plen,&(md->pbar),&(md->pvlg),md->act_ust[md->pkk].tar_kfd)>0){
//			    qDebug("Bar = %f\n",md->pbar);

                md->print_data(md->pbuf_mac,md->pbar,4);
                md->print_data_s(md->pbuf_mac,md->pbar,4);

                md->print_data(md->pbuf_mac,md->pvlg,5);
                md->print_data_s(md->pbuf_mac,md->pvlg,5);

                if (md->Plot_size_tecV<PLOT_SIZE){
				md->Plot_size_tecV++;
			    }else{
                		for ( int ig = md->Plot_size_tecV - 1; ig > 0; ig-- ) {
                            md->d_yb[ig-1] = md->d_yb[ig];
                            md->d_yv[ig-1] = md->d_yv[ig];
                }
			    }
                md->d_yb[md->Plot_size_tecV-1] = md->pbar;
                md->d_yv[md->Plot_size_tecV-1] = md->pvlg;

                md->PaintTxtData(5,md->pbar);
                md->PaintTxtData(6,md->pvlg);

//				if (md->pr_grph ==5)   md->Mgrph->replot();

//				if (md->pr_spec_file_e==1){
////				    pr_spec_file_e =0;
//				    md->print_spec_file_e(md->pbuf_mac,md->penerg);
//				}

			}
            }
		    
	    	    if(md->TryGetTemp(md->pbuf_data,md->plen)>0){
//			printf("GetTemp\n\n");
                    md->GetMacSetAddr(md->pbuf_data,md->pbuf_mac,md->pbuf_set, md->pbuf_parent);
                    md->pbuf_mac[8] = md->pbuf_set[0];
                    md->pbuf_mac[9] = md->pbuf_set[1];

			// esli eto ustroistvo is spiska to sbrasivaem flag oprosa
			//chtobi oprosil sleduyshii
                md->pkk = md->GetNumberUst(md->pbuf_mac);
            if (md->pkk>=0) {
			    md->act_ust[md->pkk].fl_oprosa =0;
        		    md->errte = md->GetTemp(md->pbuf_data,md->plen,&(md->ptemp),&(md->posv),md->act_ust[md->pkk].tar_kf);
			    if (md->errte>0){
                		;
                		if (md->flzaprosdim==1){
//                        if (md->fl_izm_dim==1){
                        	    md->GetDim(md->pbuf_data,md->plen,&(md->pdim),md->act_ust[md->pkk].tar_kfd);
        //				    md->SendZaprosOtklDim(md->pbuf_mac);
        //				    md->fl_izm_dim =0;

                        	    md->print_data(md->pbuf_mac,md->pdim,3);
                        	    md->print_data_s(md->pbuf_mac,md->pdim,3);


			    if (md->Plot_size_tecD<PLOT_SIZE){
				md->Plot_size_tecD++;
			    }else{
                		for ( int ig = md->Plot_size_tecD - 1; ig > 0; ig-- ) {
				    md->d_yd[ig-1] = md->d_yd[ig];
				}
			    }
			    md->d_yd[md->Plot_size_tecD-1] = md->pdim;
                md->PaintTxtData(4,md->pdim);
//				if ((md->pr_grph ==4))   md->Mgrph->replot();

//				if (md->pr_spec_file_e==1){
////				    pr_spec_file_e =0;
//				    md->print_spec_file_e(md->pbuf_mac,md->penerg);
//				}


	    	                    if (md->act_ust[md->pkk].fl_izm_pd==1){
            	                	md->act_ust[md->pkk].fl_izm_pd=0;
                	        	md->act_ust[md->pkk].period_oprosa_d = md->act_ust[md->pkk].period_oprosa_d/10.0;
                        	    }

                        	    qDebug("get data dim0\n");

				    
				}
			    }
			    if (md->act_ust[md->pkk].fl_izm_pt==1){
			        md->act_ust[md->pkk].fl_izm_pt=0;
                		md->act_ust[md->pkk].period_oprosa_t = md->act_ust[md->pkk].period_oprosa_t/10.0;
			    }
			    //dla algoritma usiplenia
//esli pri algoritme usiplenia ustr-vo ne otvetilo, to ne budem zadavat period sna - oprashivat budem postoyanno!!!!			    
//				if (fl_usp) act_ust[pkk].time_sleep = GetTime()+act_ust[pkk].p_sleep; 
			    
			    //enddla algoritma usiplenia
			    
			}
			else {//esli ustroistvo ne is spiska
            		    md->errte = md->GetTemp(md->pbuf_data,md->plen,&(md->ptemp),&(md->posv),1);

	                    if (md->flzaprosdim==1){
//		                    if (md->fl_izm_dim==1){
	    		        md->GetDim(md->pbuf_data,md->plen,&(md->pdim),md->act_ust[md->pkk].tar_kfd);
//					    md->SendZaprosOtklDim(md->pbuf_mac);
	                        md->print_data(md->pbuf_mac,md->pdim,3);
//				    md->fl_izm_dim =0;
	                	qDebug("get data dim\n");
				    
				}
			    
			    }
			    md->pkk=-1;

			    if (md->errte>0) {
	                	if (md->flzaprostemposv)  md->print_data(md->pbuf_mac,md->ptemp,0);
        			if (md->flzaprostemposv)  md->print_data(md->pbuf_mac,md->posv,2);
		            if (md->flzaprostemposv)  md->print_data_s(md->pbuf_mac,md->ptemp,0);
	                    if (md->flzaprostemposv)  md->print_data_s(md->pbuf_mac,md->posv,2);

			    if (md->Plot_size_tecT<PLOT_SIZE){
				md->Plot_size_tecT++;
			    }else{
                		for ( int ig = md->Plot_size_tecT - 1; ig > 0; ig-- ) {
				    md->d_yt[ig-1] = md->d_yt[ig];
				    md->d_yo[ig-1] = md->d_yo[ig];
				}
                }
			    md->d_yt[md->Plot_size_tecT - 1] = md->ptemp;
			    md->d_yo[md->Plot_size_tecT - 1] = md->posv;
                md->PaintTxtData(1,md->ptemp);
                md->PaintTxtData(3,md->posv);
//			    if ((md->pr_grph ==1)||(md->pr_grph ==3))   md->Mgrph->replot();



            //for m//    print_data(pbuf_mac,posv,2);
			//for m//    print_data_s(pbuf_mac,posv,2);
			}
			if (md->pr_spec_file_t==1){
//			    pr_spec_file_t =0;
	
        		    if (md->flzaprostemposv)  md->print_spec_file_t(md->pbuf_mac,md->ptemp);
	                    if (md->flzaprostemposv)  md->print_spec_file_o(md->pbuf_mac,md->posv);
			}
		    }//if(md->TryGetTemp(md->pbuf_data,md->plen)>0){
		    else {
		    
		    	    if(md->GetEnerg(md->pbuf_data,md->plen,&(md->penerg))>0){
//				printf("111\n");
				md->GetMacSetAddr(md->pbuf_data,md->pbuf_mac,md->pbuf_set, md->pbuf_parent);
				md->pbuf_mac[8] = md->pbuf_set[0];
				md->pbuf_mac[9] = md->pbuf_set[1];
	    			md->print_data(md->pbuf_mac,md->penerg,1);
	    			md->print_data_s(md->pbuf_mac,md->penerg,1);

			    if (md->Plot_size_tecE<PLOT_SIZE){
				md->Plot_size_tecE++;
			    }else{
                		for ( int ig = md->Plot_size_tecE - 1; ig > 0; ig-- ) {
				    md->d_ye[ig-1] = md->d_ye[ig];
				}
			    }
				md->d_ye[md->Plot_size_tecE] = md->penerg;
                md->PaintTxtData(2,md->penerg);
//				if ((md->pr_grph ==2))   md->Mgrph->replot();

				if (md->pr_spec_file_e==1){
//				    pr_spec_file_e =0;
				    md->print_spec_file_e(md->pbuf_mac,md->penerg);
				}

			// esli eto ustroistvo is spiska to sbrasivaem flag oprosa
			//chtobi oprosil sleduyshii
				md->pkk = md->GetNumberUst(md->pbuf_mac);
				if (md->pkk>=0) {
				    md->act_ust[md->pkk].fl_oprosa =0;
				    if (md->act_ust[md->pkk].fl_izm_pe==1){
					md->act_ust[md->pkk].fl_izm_pe=0;
					md->act_ust[md->pkk].period_oprosa_e = md->act_ust[md->pkk].period_oprosa_e/10.0;
				    }
				}
				md->pkk=-1;
			
			    }//if(md->GetEnerg(md->pbuf_data,md->plen,&(md->penerg))>0){
			    else{

				if(md->TryGetPANID(md->pbuf_data,md->plen)>0){
				
					md->GetMacSetAddr(md->pbuf_data,md->pbuf_mac,md->pbuf_set, md->pbuf_parent);
					md->pbuf_mac[8] = md->pbuf_set[0];
					md->pbuf_mac[9] = md->pbuf_set[1];

					md->pkk = md->GetNumberUst(md->pbuf_mac);
					if (md->pkk>=0) {

                        md->errte = md->GetPANID(md->pbuf_data,md->plen,&(md->act_ust[md->pkk].pan1), &(md->act_ust[md->pkk].pan2));
					    if (md->errte>0){

                            md->act_ust[md->pkk].fl_pan=1;
                            md->fl_repaint_panid=1;
					    }
					}
				
                   }//if(md->TryGetPANID(md->pbuf_data,md->plen)>0){
			    }
		    }	
	    }	

      }
    }
    
    printf("End test potoc rs\n");
//    return NULL;
}// end TestSVRA


int Vench::SendCommForRS(const char * str){
	int fs;
//	int fl0;
	char ss[10];
	const char * mstr;
//	int fl_file=0;
//	int fl_end=0;
	int len_st;
//	FILE* fl_com;
	int ii;
	
	unsigned char  buf_mac_set[10];
	unsigned char  buf_com[40];
	unsigned char  mbuf_mac[8];
//	unsigned char  mbuf_set[2];
	unsigned char  comm[4];
	
	unsigned int mt,mmt;	

	int it;
	int k;
	int fl_usp;//usiplat == 0 ili budit == 1
	int i;
	int fl_end_c;
	unsigned char ktr_m;
	int imm;
	float m_sleep;
	double tm_sleep;

//	int max_dt;// maximalnoe  dochernee ustroistvo
//    	int tec_dt;// tekusee ustr (po stepeni dhernego)
	float p_sn;
	float mtf;


	QString ttSS;


	it =0;
	mstr=str;
     TecTimeWaitansPort = GetTime();

/*    p_sn=0;
    fl_n=0;
    

    max_dt=0;
    tec_dt =0;
    fl_usp=0;
    pr_spec_file_t =0;
    
    fl_print_log = 0;
    fl_end=0;
    fl_al_sn =0;
    tm_sleep=0;
    m_sleep = 0;
    
	

*/

    i=0;
    flag_com=0;
    fl_end_c=0;

//    fl0=0;
    len_st=0;

	while(1){
//	printf("fl = %d\n",flag_com);
	    if (flag_com == 0) {
            fs = sscanf(mstr,"%s",ss);
            ttSS =ss;
            mstr +=(ttSS.length());
    //	printf("fs = %d\n ss = %s\n",ttSS.length(),ss);
            if (fs<0) break;

            if (ss[0] == '*'){flag_com=1;}// komanda dla peresilki dannih v port
            if (ss[0] == '&'){flag_com=2;}
            if (((ss[0] == 'q')&&(ss[1]=='u')&&(ss[2]=='i')&&(ss[3]='t'))||((ss[0] == 'Q')&&(ss[1]=='U')&&(ss[2]=='I')&&(ss[3]='T'))){
                printf("exit now\n");
    //		    fl_end=1;
    //		    fl_file=1;
                break;
            }

            if (((ss[0] == 'l')&&(ss[1]=='o')&&(ss[2]=='g'))||((ss[0] == 'L')&&(ss[1]=='O')&&(ss[2]=='G'))){
                qDebug("print log\n");
                fl_print_log=1;
    //		    fl_file=1;
                break;
            }

            if (((ss[0] == 'u')&&(ss[1]=='l')&&(ss[2]=='o')&&(ss[3]='g'))||((ss[0] == 'U')&&(ss[1]=='L')&&(ss[2]=='O')&&(ss[3]='G'))){
                qDebug("not print log\n");
                fl_print_log=0;
    //		    fl_file=1;
                break;
            }

//            qDebug("flag_com=%d\n",flag_com);
	    }
//	    fl_file =1;
//        qDebug(" w flag_com=%d\n",flag_com);
//        qDebug("qQQ\n\n");
        if (flag_com==1){//komanda tochoi peresilki v port
//            qDebug("yyyyyy\n");
//	   printf("%s\n",mstr);
    		fs = sscanf(mstr,"%x",&mt);
            ttSS.setNum(mt,16);
            mstr +=(ttSS.length()+1);
//		mstr +=(fs+2);
	
            if (fs<0) {
                break;
            }
//		printf("eee  %x\n fs = %d\n",mt,fs);

		
            if (mt==0x7e) it =1;// nachalo komandi
            if (it==1){

                buf_com[i] = (unsigned char)mt;
                if (i==2) {
                    len_st =(unsigned int)mt +3;
//                    qDebug("ll = %d\n",len_st);
                }
            }
//            qDebug("i = %d\n",i);
//            qDebug("ll = %d\n",len_st);
            if ((i==(len_st-1))&&(len_st>=4)) {
//                qDebug("ii = %d\n",i);
                fl_end_c=1;
            }
	    
            if (i>0){

                // esli eto vosprinimat kak konec komandy to ne rabotaet shirokoveshat peredacha !!!! (dva byte mac-adresa - 0xff 0xff)
    //		    if ((buf_com[i]==0xff)&&(buf_com[i-1]==0xff)) // konec komandi
                if (fl_end_c==1)
                {
                if (it==1){
                    ktr_m =0;
    //			for(imm=3;imm<(i-1);imm++){//kogda end ff ff
                    for(imm=3;imm<(i+1);imm++){
                        ktr_m=ktr_m+buf_com[imm];
                    }
    //			printf("kk = %x\n",ktr_m);
                    buf_com[i+1] = 0xFF-ktr_m;

                //pechat komandi
    //			printf("\n");
    //			for(imm=0;imm<(i+2);imm++){
    //			    printf("%x ",buf_com[imm]);
    //			}
    //			printf("\n");

    //			printf("i=%d\n",i);
//                qDebug("ERRRRRR\n");
                    k = Write(com_port,buf_com,i+2);







                if (k!=(i+2)){
                    qDebug("ERROR send comm! k = %d  i +2 = %d\n",k,i+2);
                    mstr += strlen(mstr);
                    break;
                }
                else {
                    qDebug("Send comm!\n");
                    mstr += strlen(mstr);
                    break;
                }
                i =0;
                it=0;
                flag_com=0;

               }
          }
		}

		if (it==1) i++;
		if (i==40){
            qDebug("ERROR read comm!!!\n");
		    i=0;
		    it=0;
		    flag_com=0;
		}

	    }// if (flag_com==1
	    if (flag_com==2){
    

    		fs = sscanf(mstr,"%x",&mmt);
            ttSS.setNum(mmt,16);
            mstr +=(ttSS.length()+1);

            if (fs<=0){
    //		printf("Error read com fs = %d\n",fs);
    //    printf("%s\n",mstr);
            break;
    //		continue;
            }
//		printf(" read com fs = %x\n",mmt);
		if (mmt == 0){// obnovit file activnih ustristv
		    fs = sscanf(mstr,"%d",&mt);
//		    mstr +=(fs+2);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);

		    if (fs>0){
			if (mt==1){//chtobi perezapisivat file & data in program
			    printf("Obnovili fil act ustr-v && data in program\n");
			    mkol_ust =0;
			    SendZaprosI();
			}
		    }else{// tolko file
			printf("Obnovili fil act ustr-v\n");
		    }
		    NewActUst();
		}

		if (mmt==2) {
            fs = sscanf(mstr,"%d",&mt);
            ttSS.setNum(mt,16);
            mstr +=(ttSS.length()+1);
    		    
//    		    mstr +=(fs+2);

		    printf("Zapros energ period = %d\n",mt);
		    if (fs>0) {
			if (mt>0) {
			    period_oprosa_e = mt;
			    fl_period_e =1;
			}
    			if (mt==0) {
			    SendZaprosEAll();
			    fl_period_e = 0;
			    period_oprosa_e =0;
			}
		    }

		}

		if (mmt==1) {
    		    fs = sscanf(mstr,"%d",&mt);
//    		    mstr +=(fs+2);
		ttSS.setNum(mt,10);
		mstr +=(ttSS.length()+1);
            qDebug("Zapros temp period = %d\n",mt);
		    if (fs>0) {
			if (mt>0) {
			    period_oprosa_t = mt;
			    fl_period =1;
			}
    			if (mt==0) {
			    SendZaprosTAll();
			    fl_period = 0;
			    period_oprosa_t =0;
			}
		    }
		}

		if (mmt==3) {// zapros temp koncretnogo ustr po mac adresu
//    		    fs = sscanf(mstr,"%f",&mtf);
    		    fs = sscanf(mstr,"%d",&mt);
//    		    mstr +=(fs+1);
            ttSS.setNum(mt);
		
//		printf("mtf = %f\n",mtf);
//		printf("fs = %d\n",fs);
//		printf("tt 3  = %s\n",mstr);

		mstr +=(ttSS.length()+1);

//		    printf("Zapr = %s\n",mstr);
		mtf = mt;
		    if (fs<0) break;
		    period_oprosa_t = mtf;
            qDebug("Zapros temp period mk = %f\n",mtf);
//		    period_oprosa_t = 2;
		    //if (mt==0){
			pr_spec_file_t =1;
		    //}
    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
//			mstr +=(fs+2);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);

			if (fs<0) break;

			mbuf_mac[ii] = (unsigned char)mt;
//			printf("%x  ",mbuf_mac[ii]);
		    }
//		    printf("\n");
		    if (fs<0) break;
    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
		    }
	
    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
		    }
		    ii = GetNumberUst(mbuf_mac);
		    if (ii>-1){ 
			act_ust[ii].period_oprosa_t = period_oprosa_t;
			act_ust[ii].fl_izm_pt=0;
			act_ust[ii].fl_oprosa = 0;
			period_oprosa_t=0;
			if (act_ust[ii].period_oprosa_t==0) {

			    for(k=0;k<8;k++){
				    buf_mac_set[k] = act_ust[ii].mac_addr[k];
			    }
			    buf_mac_set[8] = act_ust[ii].set_addr[0];
			    buf_mac_set[9] = act_ust[ii].set_addr[1];

			    SendZaprosT(buf_mac_set);
			}
            qDebug("Zapros temp period = %f\n",act_ust[ii].period_oprosa_t);
		    }
		    else{

			if (ii==-2){
			    for(k=0;k<8;k++){
				buf_mac_set[k] = 0;
			    }
			    buf_mac_set[6] = 0xff;
			    buf_mac_set[7] = 0xff;
			    buf_mac_set[8] = 0xfe;
			    buf_mac_set[9] = 0xff;

			    SendZaprosT(buf_mac_set);
			}
			else	printf("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
		    }
		}

		if (mmt==4) {// zapros energ koncretnogo ustr po mac adresu
    		    fs = sscanf(mstr,"%f",&mtf);
//    		    mstr +=(fs+1);
			ttSS.setNum(mtf);
			mstr +=(ttSS.length()+1);

		    if (fs<0) break;
		    period_oprosa_e = mtf;
		    //if (mt==0){
			pr_spec_file_e =1;
		    //}

    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);

			if (fs<0) break;
			mbuf_mac[ii] = (unsigned char)mt;
		    }
		    if (fs<0) break;
    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
		    }

		    ii = GetNumberUst(mbuf_mac);
		    if (ii>=-1){
			    act_ust[ii].period_oprosa_e = period_oprosa_e;
			    act_ust[ii].fl_izm_pe=0;
			    period_oprosa_e=0;
			    if (act_ust[ii].period_oprosa_e==0) {

				for(k=0;k<8;k++){
				    buf_mac_set[k] = act_ust[ii].mac_addr[k];
				}
				buf_mac_set[8] = act_ust[ii].set_addr[0];
				buf_mac_set[9] = act_ust[ii].set_addr[1];

				SendZaprosE(buf_mac_set);
			    }
			    act_ust[ii].fl_oprosa = 0;
			    qDebug("Zapros energ period = %f\n",act_ust[ii].period_oprosa_e);
		    }
		    else{

			if (ii==-2){
			    for(k=0;k<8;k++){
				buf_mac_set[k] = 0;
			    }
			    buf_mac_set[6] = 0xff;
			    buf_mac_set[7] = 0xff;

			    buf_mac_set[8] = 0xfe;
			    buf_mac_set[9] = 0xff;

			    SendZaprosE(buf_mac_set);
			}
			else qDebug("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
		    }
		}
		if (mmt==5){
    		    fs = sscanf(mstr,"%f",&m_sleep);
//			mstr +=(fs+1);
		ttSS.setNum(m_sleep);
		mstr +=(ttSS.length()+1);
    		    
		    tm_sleep = (double)m_sleep;
		    if (fs<0) break;
    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);
			
			if (fs<0) break;
			mbuf_mac[ii] = (unsigned char)mt;
		    }
		    if (fs<0) break;
    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
		    }

		    ii = GetNumberUst(mbuf_mac);
		    if (ii>=-1){
			for(k=0;k<8;k++){
			    buf_mac_set[k] = act_ust[ii].mac_addr[k];
			}
			buf_mac_set[8] = act_ust[ii].set_addr[0];
			buf_mac_set[9] = act_ust[ii].set_addr[1];
			SendZaprosSleep(buf_mac_set,tm_sleep);// zapros konkretnogo ustroistva 
//            sleep(1);
            mSleep(50);
//			SendSetSlpPZU(buf_mac_set);// zapros
			SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
		    }		

		    if (ii==-2){
			for(k=0;k<8;k++){
			    buf_mac_set[k] = 0;
			}
			buf_mac_set[6] = 0xff;
			buf_mac_set[7] = 0xff;

			buf_mac_set[8] = 0xfe;
			buf_mac_set[9] = 0xff;
			SendZaprosSleep(buf_mac_set,tm_sleep);// zapros konkretnogo ustroistva 
//			SendSetSlpPZU(buf_mac_set);// zapros
			SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
		    }		
		}

		if (mmt==6){
    		    fs = sscanf(mstr,"%f",&mtf);
//			mstr +=(fs+1);
		ttSS.setNum(mtf);
		mstr +=(ttSS.length()+1);

		    if (fs<0) break;
		    period_oprosa_d = mtf;
		    qDebug("Zapros zadim period mk = %f\n",mtf);
//		    period_oprosa_t = 2;
		    //if (mt==0){
			pr_spec_file_d =1;
		    //}
    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);
			
			if (fs<0) break;

			mbuf_mac[ii] = (unsigned char)mt;
		    }
		    if (fs<0) break;
    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
		    }

		    ii = GetNumberUst(mbuf_mac);
//		    printf("ii=%d\n",ii);
		    if (ii>-1){ 
			act_ust[ii].period_oprosa_d = period_oprosa_d;
//			printf("po = %f\n",act_ust[ii].period_oprosa_d);
			act_ust[ii].fl_izm_pd=0;
			act_ust[ii].fl_oprosa = 0;
			period_oprosa_d=0;
			if (act_ust[ii].period_oprosa_d==0) {

			    for(k=0;k<8;k++){
				    buf_mac_set[k] = act_ust[ii].mac_addr[k];
			    }
			    buf_mac_set[8] = act_ust[ii].set_addr[0];
			    buf_mac_set[9] = act_ust[ii].set_addr[1];

			    SendZaprosDim(buf_mac_set);
//			    printf("llll\n");
			}
			qDebug("Zapros zadim period = %f\n",act_ust[ii].period_oprosa_d);
		    }
		    else{

			if (ii==-2){
			    for(k=0;k<8;k++){
				buf_mac_set[k] = 0;
			    }
			    buf_mac_set[6] = 0xff;
			    buf_mac_set[7] = 0xff;
			    buf_mac_set[8] = 0xfe;
			    buf_mac_set[9] = 0xff;

			    SendZaprosDim(buf_mac_set);
			}
			else	printf("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
		    }
		    fl_izm_dim=1;
		}

		if (mmt==0xA){
    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
//			mstr +=(fs+2);
		ttSS.setNum(mt,16);
		mstr +=(ttSS.length()+1);
			
			if (fs<0) break;
			mbuf_mac[ii] = (unsigned char)mt;
		    }
		    if (fs<0) break;
	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }

		    ii = GetNumberUst(mbuf_mac);
		    if (ii>=-1){
			for(k=0;k<8;k++){
			    buf_mac_set[k] = act_ust[ii].mac_addr[k];
			}
			buf_mac_set[8] = act_ust[ii].set_addr[0];
			buf_mac_set[9] = act_ust[ii].set_addr[1];
			SendDriveStart(buf_mac_set);
		    }		

		    if (ii==-2){
			for(k=0;k<8;k++){
			    buf_mac_set[k] = 0;
			}

			buf_mac_set[6] = 0xff;
			buf_mac_set[7] = 0xff;
			buf_mac_set[8] = 0xfe;
			buf_mac_set[9] = 0xff;
			SendDriveStart(buf_mac_set);
		    }		
		}

		if (mmt==0xD){
		    
    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
			
//			mstr +=(fs+2);
		ttSS.setNum(mt,16);
		mstr +=(ttSS.length()+1);
			
			if (fs<0) break;
			mbuf_mac[ii] = (unsigned char)mt;
		    }
		    if (fs<0) break;
		    ii = GetNumberUst(mbuf_mac);
		    if (ii>=-1){

	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }

			for(k=0;k<8;k++){


			    buf_mac_set[k] = act_ust[ii].mac_addr[k];
			}
			buf_mac_set[8] = act_ust[ii].set_addr[0];
			buf_mac_set[9] = act_ust[ii].set_addr[1];
			SendDriveStop(buf_mac_set);
		    }		

		    if (ii==-2){
	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }

			for(k=0;k<8;k++){
			    buf_mac_set[k] = 0;
			}

			buf_mac_set[6] = 0xff;
			buf_mac_set[7] = 0xff;
			buf_mac_set[8] = 0xfe;
			buf_mac_set[9] = 0xff;
			SendDriveStop(buf_mac_set);
		    }	
		}

		if(mmt==0xB){ // send comm po 17 paketu &addr
    		    fs = sscanf(mstr,"%d",&mt);
//			mstr +=(fs+1);
		ttSS.setNum(mt,10);
		mstr +=(ttSS.length()+1);
//    	printf	("qwe = %s\n", mstr);    
		    if (fs<0) break;
		    period_oprosa_e = mt;
		    for (ii=0;ii<period_oprosa_e;ii++){
			fs = sscanf(mstr,"%x",&mt);
//			mstr +=(fs+2);
		ttSS.setNum(mt,16);
		mstr +=(ttSS.length()+1);
			
			if (fs<0) break;
			comm[ii] = (unsigned char)mt;
		    }
    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
//			mstr +=(fs+2);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);
//		printf	("qwe1 = %s\n", mstr);	
			if (fs<0) break;
			mbuf_mac[ii] = (unsigned char)mt;
		    }
    		    for (ii=0;ii<2;ii++){
			fs = sscanf(mstr,"%x",&mt);
//			mstr +=(fs+2);
			ttSS.setNum(mt,16);
			mstr +=(ttSS.length()+1);
//		printf	("qwe1 = %s\n", mstr);	
			if (fs<0) break;
//			mbuf_set[ii] = (unsigned char)mt;
		    }
    		    if (fs<0) break;
		    ii = GetNumberUst(mbuf_mac);
//		    printf("ii = %d\n",ii);
		    if (ii>-1){
			for(k=0;k<8;k++){
			    buf_mac_set[k] = act_ust[ii].mac_addr[k];
			}
			
			buf_mac_set[8] = act_ust[ii].set_addr[0];
			buf_mac_set[9] = act_ust[ii].set_addr[1];
			
//            qDebug("EEEEEEEEEEEEEEEEEEE\n");

            SendZaprosComm(buf_mac_set,comm, (int)period_oprosa_e);// zapros konkretnogo ustroistva
		    }else{
			if (ii==-2){
	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }

			    for(k=0;k<8;k++){
				buf_mac_set[k] = 0;
			    }
			    buf_mac_set[6] = 0xff;
			    buf_mac_set[7] = 0xff;
			    buf_mac_set[8] = 0xff;
			    buf_mac_set[9] = 0xfe;
			
//			    printf("EEEEEEEEEEEEEEEEEEE111111\n");

			    SendZaprosComm(buf_mac_set,comm, (int)period_oprosa_e);// zapros konkretnogo ustroistva
			}else{
			    printf("Error ustr-vo!!\n");
			}
			
		    }
		    

		}
    		if (mmt==0xC){ // delaem algoritm usiplenia
		    if (fl_al_sn==0)  fl_al_sn= 1;
		    else if (fl_al_sn==1)  fl_al_sn= 0;
		    if (fl_usp == 1) {
			fl_usp =0;
			fl_al_sn = 0;
		    }
		    if (fl_al_sn==1){
    			fs = sscanf(mstr,"%f",&t_sn);
//			mstr +=(fs+1);
			ttSS.setNum(t_sn);
			mstr +=(ttSS.length()+1);
    			if (fs<0) break;

    			fs = sscanf(mstr,"%f",&p_sn);
//			mstr +=(fs+1);
			ttSS.setNum(p_sn);
			mstr +=(ttSS.length()+1);
    			if (fs<0) break;

			printf("Begin Algoritm usipl\n");
			SetParentUst();
//			max_dt = SetParentUst();
//			tec_dt = max_dt;
			fl_usp = 0;
		    }else
			printf("End Algoritm usipl\n");
		    
		}
		
		if (mmt==9){
    			fs = sscanf(mstr,"%f",&m_sleep);
			ttSS.setNum(m_sleep);
			mstr +=(ttSS.length()+1);
    		    
		    	tm_sleep = (double)m_sleep;
		    	if (fs<0) break;
    		   	 for (ii=0;ii<8;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
			
				if (fs<0) break;
				mbuf_mac[ii] = (unsigned char)mt;
			    }
			    if (fs<0) break;
	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }

			    ii = GetNumberUst(mbuf_mac);
			    if (ii>=-1){
				for(k=0;k<8;k++){
				    buf_mac_set[k] = act_ust[ii].mac_addr[k];
				}
				buf_mac_set[8] = act_ust[ii].set_addr[0];
				buf_mac_set[9] = act_ust[ii].set_addr[1];
				SendSetAlarm(buf_mac_set);// zapros konkretnogo ustroistva
				SendZaprosSleep(buf_mac_set,TM_UMOL);// zapros konkretnogo ustroistva 
				SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
			    }		

			    if (ii==-2){
                    for (ii=0;ii<2;ii++){
                        fs = sscanf(mstr,"%x",&mt);
                        ttSS.setNum(mt,16);
                        mstr +=(ttSS.length()+1);
                        if (fs<0) break;
    //					mbuf_set[ii] = (unsigned char)mt;
                    }
                    for(k=0;k<8;k++){
                        buf_mac_set[k] = 0;
                    }
                    buf_mac_set[6] = 0xff;
                    buf_mac_set[7] = 0xff;

                    buf_mac_set[8] = 0xfe;
                    buf_mac_set[9] = 0xff;
                    SendSetAlarm(buf_mac_set);// zapros konkretnogo ustroistva
                    SendZaprosSleep(buf_mac_set,TM_UMOL);// zapros konkretnogo ustroistva
    //			SendSetSlpPZU(buf_mac_set);// zapros
                    SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
			    }		


		}

		if (mmt==8){

    		    for (ii=0;ii<8;ii++){
			fs = sscanf(mstr,"%x",&mt);
//			mstr +=(fs+1);
		ttSS.setNum(mt,16);
		mstr +=(ttSS.length()+1);
			
			if (fs<0) break;
			mbuf_mac[ii] = (unsigned char)mt;
		    }
		    if (fs<0) break;
	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }

		    ii = GetNumberUst(mbuf_mac);
		    if (ii>=-1){
				for(k=0;k<8;k++){
				    buf_mac_set[k] = act_ust[ii].mac_addr[k];
				}
				buf_mac_set[8] = act_ust[ii].set_addr[0];
				buf_mac_set[9] = act_ust[ii].set_addr[1];

			        if (fl_krd==0){
			    	    if(fl_krd_start==0){
					SendCommStartKrd(buf_mac_set);
					fl_krd=1;
					continue;

				    }
				}else{
				    SendCommStopKrd(buf_mac_set);
				    fl_krd=0;
				    continue;

				}
		    }
		    else{
			if (ii==-2){
	    		    for (ii=0;ii<2;ii++){
				fs = sscanf(mstr,"%x",&mt);
				ttSS.setNum(mt,16);
				mstr +=(ttSS.length()+1);
				if (fs<0) break;
//				mbuf_set[ii] = (unsigned char)mt;
			    }
			    for(k=0;k<8;k++){
				buf_mac_set[k] = 0;
			    }
			    buf_mac_set[6] = 0xff;
			    buf_mac_set[7] = 0xff;

			    buf_mac_set[8] = 0xfe;
			    buf_mac_set[9] = 0xff;

			        if (fl_krd==0){
			    	    if(fl_krd_start==0){
					SendCommStartKrd(buf_mac_set);
					fl_krd=1;
					continue;

				    }
				}else{
				    SendCommStopKrd(buf_mac_set);
				    fl_krd=0;
					continue;
				}


			}
			else printf("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
		    }



		}
		if (mmt==7){ // delaem dochernee ustroistvo
		    if (mkol_ust>=2){
			act_ust[1].parent_addr[0] = act_ust[0].set_addr[0];
			act_ust[1].parent_addr[1] = act_ust[0].set_addr[1];
			printf("Ust: 1 - dochernee dla 0\n");
		    }    
//			max_dt = SetParentUst();
		}
		

		
//		flag_com =0;
	    }


    }
    
    return 1;
}

void Vench::on_BCar_1_clicked()
{
    int i,k;


    if (ui->BCar_1->isChecked()){



        k = kol_ust;

        fl_vibr_ust=0;
        for(i=0;i<k;i++){
                mas_vibr_ust[i] = 0;
                act_ust[i].vibr_sp = 0;
        }

        QFont ft("MS Shell Dlg 2",8,50,false);
        ft.setBold( true);
        ui->BCar_1->setFont(ft);
        mas_vibr_ust[0] = 1;
        act_ust[0].vibr_sp = 1;
        fl_vibr_ust =1;

    }
    else{
        QFont ft("MS Shell Dlg 2",8,50,false);
        ft.setBold(false);
        ui->BCar_1->setFont(ft);
        if (ui->BCar_2->isChecked()){

            k = kol_ust;
            fl_vibr_ust=0;
            for(i=0;i<k;i++){
                    mas_vibr_ust[i] = 0;
                    act_ust[i].vibr_sp = 0;
            }

            mas_vibr_ust[1] = 1;
            act_ust[1].vibr_sp = 1;
            fl_vibr_ust =1;
            QFont ft("MS Shell Dlg 2",8,50,false);
            ft.setBold( true);
            ui->BCar_2->setFont(ft);

        }
        else{
            QFont ft("MS Shell Dlg 2",8,50,false);
            ft.setBold(false);
            ui->BCar_2->setFont(ft);
               k = kol_ust;
            for(i=0;i<k;i++){
                    mas_vibr_ust[i] = 0;
                    act_ust[i].vibr_sp = 0;
                    fl_vibr_ust =0;
            }

        }


    }
}

void Vench::on_BCar_2_clicked()
{
    on_BCar_1_clicked();
}

void Vench::on_BLeft_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressLeft();
}

void Vench::on_BLeft_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BReleaseLeft();
}

void Vench::on_BRight_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressRight();
}

void Vench::on_BRight_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BReleaseRight();
}

void Vench::on_BUp_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressFw();
}

void Vench::on_BUp_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BReleaseFw();
}

void Vench::on_BDown_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressRv();
}

void Vench::on_BDown_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BReleaseRv();
}

void Vench::on_BStop_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressStop();
}

void Vench::on_BStop_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BRealeseStop();
}

void Vench::on_BBip_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressBip();
}

void Vench::on_BBip_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BReleaseBip();
}

void Vench::on_BLight_pressed()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BPressLight();
}

void Vench::on_BLight_released()
{
    if(fl_vibr_ust==0) return; // esli ne vibrano ustroistv
    BReleaseLight();
}

void Vench::on_BAntiSleep_clicked()
{
    AntiSleep();
}

void Vench::on_Obnovit_clicked()
{
    ObSpActUst();
}

void Vench::on_Obnovit_2_clicked()
{
    ObSpActUst();
}

void Vench::on_BAntiSleep_2_clicked()
{
    AntiSleep();

}

void Vench::on_BLight_2_released()
{
    BReleaseLight();

}

void Vench::on_BLight_2_pressed()
{
    BPressLight();

}

void Vench::on_BBip_2_released()
{
    BReleaseBip();

}

void Vench::on_BBip_2_pressed()
{
    BPressBip();

}

void Vench::on_BStop_2_released()
{
    BRealeseStop();

}

void Vench::on_BStop_2_pressed()
{
    BPressStop();

}

void Vench::on_BDown_2_released()
{
    BReleaseRv();

}

void Vench::on_BDown_2_pressed()
{
    BPressRv();

}

void Vench::on_BUp_2_released()
{
    BReleaseFw();

}

void Vench::on_BUp_2_pressed()
{
    BPressFw();

}

void Vench::on_BRight_2_released()
{
    BReleaseRight();

}

void Vench::on_BRight_2_pressed()
{
    BPressRight();

}

void Vench::on_BLeft_2_released()
{
    BReleaseLeft();

}

void Vench::on_BLeft_2_pressed()
{
    BPressLeft();

}

void Vench::on_checkKumir_stateChanged(int arg1)
{
    checkKumir->setChecked(arg1);
//    chKum(); //dolgno samo ????????????
}

void Vench::on_listV_itemSelectionChanged()
{
    int i,k;
    QListWidgetItem *itm;
//        qDebug()<<"TTTT0";

    k = ui->listV->count();
    for(i=0;i<k;i++){
        itm = ui->listV->item(i);
        if (itm->isSelected()== true){
            listV->setCurrentRow(i,QItemSelectionModel::Select);
        }
        else{
            listV->setCurrentRow(i,QItemSelectionModel::Deselect);
        }
    }

//    SpVibr();//dolgno samo????????
}

void Vench::on_checDiskr_stateChanged(int arg1)
{
    qDebug()<< arg1;
    checkDiskr->setChecked( arg1 );
    qDebug()<<checkDiskr->isChecked();
}

void Vench::on_checkAll_stateChanged(int arg1)
{
    checkAll->setChecked(arg1);
}

void Vench::on_pBPlay_pressed()
{
//    BPlay->setChecked(ui->pBPlay->isChecked());
    BPlay->setChecked( true);
    BPressPlay();
}

void Vench::on_pBPlay_released()
{
//    BPlay->setChecked(ui->pBPlay->isChecked());
    if (ui->pBPlay->isChecked()==1) return;
    BPlay->setChecked(false);
    BRealesePlay();
}

void Vench::on_pBRec_pressed()
{
    if (ui->pBRec->isChecked()==1) return;

    if (fl_rec==1) return;
    
//    qDebug("LLLLLLL\n");
    BRec->setChecked( true);
    BPressRec();
}

void Vench::on_pBRec_released()
{
    if (ui->pBRec->isChecked()==1) return;
    BRec->setChecked(false);
    BRealeseRec();
}


void Vench::BRec_pressed_n()
{
    //if (ui->BRec_n->isChecked()==1) return;

    if (fl_rec==0) {    
        if (!fl_bluetooth) {
            BRec->setChecked( true);
            BPressRec();
        }
        else{
            ui->BRec_n_B->setStyleSheet("background: red");
            ui->BRec_n_B->setText(trUtf8("Стоп"));
            fl_rec=1;

        }
        return;
    }
    
    if (fl_rec==1) {    
        if (!fl_bluetooth) {
            BRec->setChecked(false);
            BRealeseRec();
        }
        else{
            ui->BRec_n_B->setStyleSheet("background: lightgray");
            ui->BRec_n_B->setText(trUtf8("Запись"));
            fl_rec=0;
            WriteProg1(num_comm);

            return;
        }
    }
}


//ne ispolzuetsa
void Vench::BRec_released_n()
{
    if (ui->BRec_n->isChecked()==1) return;
    if (ui->BRec_n_B->isChecked()==1) return;
    BRec->setChecked(false);
    BRealeseRec();
}

void Vench::on_pBProgr_clicked()
{
//    QObject *parent;
//    QString program = "calc.exe";
//    QStringList arguments;
//  ///      arguments << "-style" << "motif";
//    QProcess *myProcess = new QProcess(parent);
////    myProcess->start(program, arguments);
//    myProcess->start(program);

//rabotaet no s cmd
//   QString ms = "C:\\Windows\\System32\\notepad.exe";
//   QByteArray rr = ms.toLocal8Bit();
//   int i = system(rr);

#ifdef WIN_D
   QString ms="";
   ms=ms+ prog_for_read_win;
   ms = ms+" ";
   if (checkKumir->isChecked()== true){
        ms =ms +f_name_kum;
   }else{
       ms = ms + f_name_xml;
   }
   QByteArray rr = ms.toLocal8Bit();
   WinExec(rr,SW_RESTORE)   ;
#endif


#ifdef LINUX_D
    
   QString     ms = mEdit;// + tt + " &";
//   QString     ms = "mcedit ./";// + tt + " &";

    QString fnamec;

    fnamec= ServerDir->text();
    #ifdef LINUX_D
//    fnamec= fnamec+"/"+f_name_kum;

    if (!fl_bluetooth) fnamec= fnamec+f_name_kum;
    if (fl_bluetooth) fnamec= fnamec+f_name_snap;

    #endif


   if (checkKumir->isChecked()== true){
        ms =ms +fnamec + " &";;
   }else{
       QString tt;
       tt= ServerDir->text();
       tt =tt+ f_name_xml;
       ms = ms+tt + " &";
   }
   
   qDebug()<<ms;
    QByteArray rr = ms.toLocal8Bit();
    
    rr = ms.toLocal8Bit();
    int i = system(rr);
    i=i+1;
    
#endif

}

void Vench::on_pBTemp_clicked()
{
    if (ui->pBTemp->isChecked() == true) BTemp->setChecked( true);
    if (ui->pBTemp->isChecked() ==false) BTemp->setChecked(false);

    BPressTemp();
}

void Vench::on_pBGraph_clicked()
{
    BPressGrph();
}

void Vench::on_pBRezult_clicked()
{

#ifdef WIN_D
    int flv;
    flv=0;
    for (int k=0;k<kol_ust;k++){
        if (mas_vibr_ust[k]==1){
	flv=1;
         QString txt= ServerDir->text();
         txt = txt + "/DATA";
         for(int i=0;i<8;i++){
             QString ss="";
             ss.setNum(mas_mac_addr[k][i],16);
             txt = txt+ss;
         }
        txt = txt + ".txt";

        QString ms="notepad.exe ";
           ms = ms + txt;
       QByteArray rr = ms.toLocal8Bit();
       WinExec(rr.constData(),SW_RESTORE)   ;
	
     }
   }
   if (flv==0){
	if(kol_ust>0){
         QString txt= ServerDir->text();
         txt = txt + "/DATA";
         for(int i=0;i<8;i++){
             QString ss="";
             ss.setNum(mas_mac_addr[0][i],16);
             txt = txt+ss;
         }
        txt = txt + ".txt";

        QString ms="notepad.exe ";
           ms = ms + txt;
       QByteArray rr = ms.toLocal8Bit();
       WinExec(rr.constData(),SW_RESTORE)   ;
	
	}
   }
#endif



#ifdef LINUX_D

    int flv;
    flv=0;
   for (int k=0;k<kol_ust;k++){
       if (mas_vibr_ust[k]==1){
	flv=1;

        QString txt= ServerDir->text();
        txt = txt + "DATA";
        for(int i=0;i<8;i++){
            QString ss="";
            ss.setNum(mas_mac_addr[k][i],16);
            txt = txt+ss;
        }
       txt = txt + ".txt";

       QString ms = mEdit + txt + " &";
	qDebug()<<ms;
       QByteArray  rr = ms.toLocal8Bit();
       int ii = system(rr.constData());
       ii=ii+1;
    }
  }

   if (flv==0){
	if(kol_ust>0){
            QString txt= ServerDir->text();
	    txt = txt + "DATA";
    	    for(int i=0;i<8;i++){
        	QString ss="";
    	        ss.setNum(mas_mac_addr[0][i],16);
        	txt = txt+ss;
    	    }
    	    txt = txt + ".txt";

    	    QString ms = mEdit + txt + " &";
	    qDebug()<<ms;
    	    QByteArray  rr = ms.toLocal8Bit();
    	    int iii = system(rr.constData());
    	    iii=iii+1;
	
	}
   }

#endif


}

//void Vench::on_pBRezultE_Qclicked()
//{
//
//#ifdef WIN_D
//    int flv;
//    flv=0;
//    for (int k=0;k<kol_ust;k++){
//        if (mas_vibr_ust[k]==1){
//	flv=1;
//         QString txt= ServerDir->text();
//         txt = txt + "/DATA";
//         for(int i=0;i<8;i++){
//             QString ss="";
//             ss.setNum(mas_mac_addr[k][i],16);
//             txt = txt+ss;
//         }
//        txt = txt + ".txt";
//
//        QString ms="notepad.exe ";
//           ms = ms + txt;
//       QByteArray rr = ms.toLocal8Bit();
//       WinExec(rr.constData(),SW_RESTORE)   ;
//
//     }
//   }
//   if (flv==0){
//	if(kol_ust>0){
//       QString txt= ServerDir->text();
//         txt = txt + "/DATA";
//         for(int i=0;i<8;i++){
//             QString ss="";
//             ss.setNum(mas_mac_addr[0][i],16);
//             txt = txt+ss;
//         }
//        txt = txt + ".txt";
//
//        QString ms="notepad.exe ";
//           ms = ms + txt;
//       QByteArray rr = ms.toLocal8Bit();
//       WinExec(rr.constData(),SW_RESTORE)   ;
	
//	}
//   }
//#endif



//#ifdef LINUX_D
//
//    int flv;
//    flv=0;
//   for (int k=0;k<kol_ust;k++){
//       if (mas_vibr_ust[k]==1){
//	flv=1;
//
//        QString txt= ServerDir->text();
//        txt = txt + "DATA";
//        for(int i=0;i<8;i++){
//            QString ss="";
//            ss.setNum(mas_mac_addr[k][i],16);
//            txt = txt+ss;
//        }
//       txt = txt + ".txt";
//
//       QString ms = "gedit " + txt + " &";
//	qDebug()<<ms;
//       QByteArray  rr = ms.toLocal8Bit();
//       int ii = system(rr.constData());
//       ii=ii+1;
//    }
//  }
//
//   if (flv==0){
//	if(kol_ust>0){
//            QString txt= ServerDir->text();
//	    txt = txt + "DATA";
//    	    for(int i=0;i<8;i++){
//        	QString ss="";
//    	        ss.setNum(mas_mac_addr[0][i],16);
//        	txt = txt+ss;
//    	    }
//    	    txt = txt + ".txt";
//
//    	    QString ms = "gedit " + txt + " &";
//	    qDebug()<<ms;
//    	    QByteArray  rr = ms.toLocal8Bit();
//    	    int iii = system(rr.constData());
//    	    iii=iii+1;
//
//	}
//   }
//
//#endif
//
//
//}

void Vench::on_pBPanIdContr_clicked()
{
    BPressIzmPANID_KONTR();
}

void Vench::on_pBPanId_clicked()
{
    BPressIzmPANID();
}

void Vench::on_pBTemp_pressed()
{

}

void Vench::on_pBTemp_E_clicked()
{
    if (ui->pBTemp_E->isChecked() == true) BEnerg->setChecked( true);
    if (ui->pBTemp_E->isChecked() ==false) BEnerg->setChecked(false);

    BPressEnerg();

}

void Vench::on_pBTemp_D_clicked()
{
    if (ui->pBTemp_D->isChecked() == true) BDim->setChecked( true);
    if (ui->pBTemp_D->isChecked() ==false) BDim->setChecked(false);

    BPressDim();

}

//void Vench::on_pBTemp_O_clicked()
//{
//}

void Vench::on_pBGraph_E_clicked()
{
    BPressGrphE();
}

void Vench::on_pBGraph_O_clicked()
{
    BPressGrphO();
}

void Vench::on_pBGraph_D_clicked()
{
    BPressGrphD();
}


void Vench::RBKumir()
{
    if (ui->rBKumir_n->isChecked()){
        if (!(checkKumir->isChecked())){
            checkKumir->setChecked(  true );
    	}
        
    }else{
        if ((checkKumir->isChecked())){
            checkKumir->setChecked( false );
    	}
    }
}


void Vench::rBAll()
{
    if (ui->rB_all_n->isChecked()){
        checkAll->setChecked(  true );
    }else{
        checkAll->setChecked( false );
    }
}
void Vench::SendCommBipBT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x8e; //begin packet

    buf[1]=0x7;// command packet

    buf[2]=0x8f;//end packet

    Write(com_port,buf,3);


}

void Vench::SendCommLightBT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x8e; //begin packet

    buf[1]=0xF;// command packet

    buf[2]=0x8f;//end packet

    Write(com_port,buf,3);


}

void Vench::SendCommForw_ave_BT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x0; //begin packet

    buf[1]=0x0;// command packet

    buf[2]=0x81;//end packet

    buf[3]=0x0; //begin packet

    buf[4]=0x4;// command packet

    buf[5]=0x0;//end packet

    buf[6]=0x0;//end packet

    Write(com_port,buf,7);


}


void Vench::SendCommRevers_ave_BT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x0; //begin packet

    buf[1]=0x0;// command packet

    buf[2]=0x7F;//end packet

    buf[3]=0x0; //begin packet

    buf[4]=0x4;// command packet

    buf[5]=0x0;//end packet

    buf[6]=0x0;//end packet

    Write(com_port,buf,7);


}

void Vench::SendCommLeft_ave_BT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x0; //begin packet

    buf[1]=0x81;// command packet

    buf[2]=0x0;//end packet

    buf[3]=0x0; //begin packet

    buf[4]=0x4;// command packet

    buf[5]=0x0;//end packet

    buf[6]=0x0;//end packet

    Write(com_port,buf,7);


}

void Vench::SendCommRight_ave_BT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x0; //begin packet

    buf[1]=0x7F;// command packet
    buf[2]=0x0;//end packet

    buf[3]=0x0; //begin packet

    buf[4]=0x4;// command packet

    buf[5]=0x0;//end packet

    buf[6]=0x0;//end packet

    Write(com_port,buf,7);


}



void Vench::SendCommStop_ave_BT()
{
    if (!fl_bluetooth) return;



    unsigned char buf[10];
    buf[0]=0x0; //begin packet

    buf[1]=0x0;// command packet
    buf[2]=0x0;//end packet

    buf[3]=0x0; //begin packet

    buf[4]=0x4;// command packet

    buf[5]=0x0;//end packet

    buf[6]=0x0;//end packet

    Write(com_port,buf,7);


}

void Vench::on_BBip_B_clicked()
{

    //       QString vpravo= "вправо";//6
    //       QString vlevo= "влево";//9
    //       QString vpered= "вверх";0x5
    //       QString nazad= "вниз";0xa
    //       QString svet= "свет";//0xb1
    //       QString signal= "сигнал";//0xc1
    if (fl_rec){
       if(num_comm<NUM_COMM){

            mass_comm_n[num_comm][0] = 10;
            mass_comm_n[num_comm][1] = 10;
            mass_comm_n[num_comm][2] = 0xC1;
            num_comm++;


       }
   }


    SendCommBipBT();
}

void Vench::on_BLight_B_clicked()
{

    //       QString vpravo= "вправо";//6
    //       QString vlevo= "влево";//9
    //       QString vpered= "вверх";0x5
    //       QString nazad= "вниз";0xa
    //       QString svet= "свет";//0xb1
    //       QString signal= "сигнал";//0xc1
    if (fl_rec){
       if(num_comm<NUM_COMM){

            mass_comm_n[num_comm][0] = 10;
            mass_comm_n[num_comm][1] = 10;
            mass_comm_n[num_comm][2] = 0xB1;
            num_comm++;


       }
   }



    SendCommLightBT();
}

void Vench::on_BUp_B_clicked()
{


}

void Vench::on_BLeft_B_clicked()
{

}

void Vench::on_BRight_B_clicked()
{

}

void Vench::on_BRuchnComm_n_B_clicked()
{
    QString txt;
    txt = ui->CommText_n_B->text();
    txt = txt.toUpper();

    QString txtt = "  ";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" "));

    txtt = " 00";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 0"));
    txtt = " 01";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 1"));
    txtt = " 02";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 2"));
    txtt = " 03";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 3"));

    txtt = " 04";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 4"));

    txtt = " 05";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 5"));

    txtt = " 06";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 6"));

    txtt = " 07";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 7"));

    txtt = " 08";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 8"));

    txtt = " 09";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" 9"));

    txtt = " 0A";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" A"));

    txtt = " 0B";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" B"));

    txtt = " 0C";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" C"));

    txtt = " 0D";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" D"));

    txtt = " 0E";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" E"));

    txtt = " 0F";
    while (txt.indexOf(txtt)>=0) txt.replace(txtt,QString(" F"));

    txt =txt +" ";
    #ifdef PRINT_DEBUG
    qDebug()<<txt;
    #endif
    unsigned char buf[1000];
//    int err =0;
    int i=0;
    while(1){

        if (i>998) break;
        int l = txt.indexOf(" ");
        if (l<0)break;

        QString mt = txt.left(l);
        bool ok;
        buf[i] = mt.toInt(&ok,16);
        if (!ok) {
            err=1;
            i--;
        }



            mt = txt.right(txt.length()-l-1);
            txt = mt;

            i++;

    }



    Write(com_port,buf,i);

//    if ((err)||(i>997)){
//        QMessageBox::information( this, trUtf8("Информация"),
//                                                   trUtf8("Выражение в строке преобразовано не полностью\n"),
//                                                   trUtf8("Ok") );
//    }

    QMessageBox::information( this, trUtf8("Информация"),
                                               trUtf8("Команда отправлена на выполнение\n"),
                                               trUtf8("Ok") );

    #ifdef PRINT_DEBUG
        qDebug("Send comm Ruchn vvod\n");
    #endif

}

void Vench::on_checkLog_n_B_clicked(bool checked)
{
    if (checked){
        fl_print_log=1;
        fl_print_log_B=1;
    }
    else {
        fl_print_log_B = 0;
        fl_print_log = 0;
    }
}

void Vench::on_CommText_n_B_returnPressed()
{
   on_BRuchnComm_n_B_clicked();
}

void Vench::on_BUp_B_pressed()
{

    //       QString vpravo= "вправо";//6
    //       QString vlevo= "влево";//9
    //       QString vpered= "вверх";0x5
    //       QString nazad= "вниз";0xa
    //       QString svet= "свет";//0xb1
    //       QString signal= "сигнал";//0xc1
    if (fl_rec){
       if(num_comm<NUM_COMM){

            mass_comm_n[num_comm][0] = 10;
            mass_comm_n[num_comm][1] = 10;
            mass_comm_n[num_comm][2] = 0x5;
            num_comm++;


       }
   }


     SendCommForw_ave_BT();

}

void Vench::on_BUp_B_released()
{
    SendCommStop_ave_BT();
}

void Vench::on_BDown_B_pressed()
{

    //       QString vpravo= "вправо";//6
    //       QString vlevo= "влево";//9
    //       QString vpered= "вверх";0x5
    //       QString nazad= "вниз";0xa
    //       QString svet= "свет";//0xb1
    //       QString signal= "сигнал";//0xc1
    if (fl_rec){
       if(num_comm<NUM_COMM){

            mass_comm_n[num_comm][0] = 10;
            mass_comm_n[num_comm][1] = 10;
            mass_comm_n[num_comm][2] = 0xA;
            num_comm++;


       }
   }


    SendCommRevers_ave_BT();
}

void Vench::on_BDown_B_released()
{
    SendCommStop_ave_BT();

}

void Vench::on_BDown_B_clicked()
{

}

void Vench::on_BLeft_B_pressed()
{

    //       QString vpravo= "вправо";//6
    //       QString vlevo= "влево";//9
    //       QString vpered= "вверх";0x5
    //       QString nazad= "вниз";0xa
    //       QString svet= "свет";//0xb1
    //       QString signal= "сигнал";//0xc1
    if (fl_rec){
       if(num_comm<NUM_COMM){

            mass_comm_n[num_comm][0] = 10;
            mass_comm_n[num_comm][1] = 10;
            mass_comm_n[num_comm][2] = 0x9;
            num_comm++;


       }
   }


    SendCommLeft_ave_BT();
}

void Vench::on_BLeft_B_released()
{
    SendCommStop_ave_BT();

}

void Vench::on_BRight_B_pressed()
{

    //       QString vpravo= "вправо";//6
    //       QString vlevo= "влево";//9
    //       QString vpered= "вверх";0x5
    //       QString nazad= "вниз";0xa
    //       QString svet= "свет";//0xb1
    //       QString signal= "сигнал";//0xc1
    if (fl_rec){
       if(num_comm<NUM_COMM){

            mass_comm_n[num_comm][0] = 10;
            mass_comm_n[num_comm][1] = 10;
            mass_comm_n[num_comm][2] = 0x6;
            num_comm++;


       }
   }


    SendCommRight_ave_BT();
}

void Vench::on_BRight_B_released()
{
    SendCommStop_ave_BT();

}

void Vench::on_BPlay_n_clicked()
{

}


//void Vench::SendCommPrBT( int d1, int d2, int napr ){
void Vench::SendCommPrBT( int d1, int napr ){

     //       QString vpravo= "вправо";//6
     //       QString vlevo= "влево";//9
     //       QString vpered= "вверх";0x5
     //       QString nazad= "вниз";0xa
     //       QString svet= "свет";//0xb1
     //       QString signal= "сигнал";//0xc1


     if (napr==0xB1){// svet
             SendCommLightBT();
             fl_play_comm=0;
             return ;
     }

     if (napr==0xC1){//bip

             SendCommBipBT();
             fl_play_comm=0;
             return ;
     }


     if (napr==0x9){//vlevo
//printf("LLL\n");

         SendCommLeft_ave_BT();
//         mSleep((d1-d2)*10);
         mSleep((d1)*10);
         SendCommStop_ave_BT();

         fl_play_comm=0;

         return ;
     }

     if (napr==0x6){//vpravo

         SendCommRight_ave_BT();
//         mSleep((d1-d2)*10);
         mSleep((d1)*10);
         SendCommStop_ave_BT();
//         printf("d1 = %d\n",d1);
//         printf("d2 = %d\n",d2);

         fl_play_comm=0;

         return ;
     }

     if (napr==0x5){//vverh

         SendCommForw_ave_BT();
         mSleep(d1*10);
         SendCommStop_ave_BT();

         fl_play_comm=0;

         return ;
     }

     if (napr==0xA){//vniz

         SendCommRevers_ave_BT();
         mSleep(d1*10);
         SendCommStop_ave_BT();

         fl_play_comm=0;

         return ;
     }
}

void Vench::on_BPokazProgr_n_B_clicked()
{
    on_pBProgr_clicked();
}

void Vench::on_BPlay_n_B_pressed()
{
    BPressPlay_n();
}

void Vench::on_BProdol_n_B_clicked()
{
    BPressProdol();
}

void Vench::on_checkCicl_n_B_clicked()
{
    if (ui->checkCicl_n_B->isChecked() ==  true) ui->checkCicl_n->setChecked( true);
    if (ui->checkCicl_n_B->isChecked() == false) ui->checkCicl_n->setChecked(false);

    BChCikl();
    printf("cicl = %d\n",checkCicl->isChecked());
}

void Vench::on_BRec_n_B_pressed()
{
    BRec_pressed_n();
}

void Vench::on_rBXML_n_2_clicked()
{
//    ui->BPlay_n_B->setEnabled(0);
//    ui->BRec_n_B->setEnabled(1);
}

void Vench::on_rBKumir_n_B_clicked()
{
//    ui->BPlay_n_B->setEnabled(1);
//    ui->BRec_n_B->setEnabled(0);

}

void Vench::on_BProgram_n_B_clicked()
{
    QString txt;

    if (ui->rBXML_n_2->isChecked()) ReadSnap(txt);
    else ReadJson(txt);

//    qDebug("WWWWWWWWW");
//    qDebug()<<txt;

    unsigned char buf[1000];
    int i=0;
    txt =txt + " ";
    while(1){

        if (i>998) break;
        int l = txt.indexOf(" ");
        if (l<0)break;

        QString mt = txt.left(l);
        bool ok;
        buf[i] = mt.toInt(&ok,16);
        if (!ok) {
            err=1;
            i--;
        }



            mt = txt.right(txt.length()-l-1);
            txt = mt;

            i++;

    }
//qDebug("i=%d\n",i);
//    int err =0;
//    for (err=0;err<i;err++) qDebug("%x",buf[err]);

//    if (i>0)
     Write(com_port,buf,i);
//    qDebug("en\n");

}


int Vench::AddComm(QString cm,int pos){

    int min=0;
    int i;
    
    for(i=0;i<col_comm;i++){
	if (sp_comm[i].pos<pos)min++;
	else break;
    }
    
    for(i=col_comm;i>min;i--){
	sp_comm[i].comm = sp_comm[i-1].comm;
	sp_comm[i].comm16 = sp_comm[i-1].comm16;
	sp_comm[i].pos = sp_comm[i-1].pos;
    }
    
    sp_comm[min].comm = cm;
    sp_comm[min].comm16 = cm;
    sp_comm[min].pos = pos;
    col_comm++;
    return min;
}

void Vench::AddCicklSnap(QDomNode node){
    QDomNode node1,node5;
    QString rrr1;

    node1 = node.firstChild();


    rrr1=node1.toElement().text();
//    qDebug()<<rrr1;

    int kk = rrr1.toInt();
//    qDebug("cicl ka = %d\n",kk);


    //otkrivaem cicl
    for (int l=0;l<kk-1;l++){
               AddComm(JComm[2].comm16,nc_snap);
               nc_snap++;
    }
    int icickl =kk;

    while(!node1.isNull()){
        rrr1=node1.toElement().tagName();
//        qDebug()<<rrr1;


        if (node1.toElement().tagName() == "script") {
//            qDebug()<<"VMM\n";
            node5 = node1.firstChild();
            while(!node5.isNull()){
                rrr1=node5.toElement().tagName();
//                qDebug()<<rrr1;





                rrr1 = node5.toElement().attribute("s");
//                                int numpi=rrr1.toInt();
                for(int i=0;i<NUM_COMM_SNAP;i++){
                    if (rrr1==JComm[i].comm){
                        AddComm(JComm[i].comm16,nc_snap);
                        nc_snap++;

                        if (i==2){
                            AddCicklSnap(node5);
                        }
                    }
                }


                node5 = node5.nextSibling();
            }
        }
        node1 = node1.nextSibling();
    }

// zakrivaem obratno cikl
    for (int t=0;t<icickl;t++){
        AddComm(JComm[6].comm16,nc_snap);
        nc_snap++;

    }



}

void Vench::ReadSnap(QString &txt)
{
    qDebug("read snap\n");
  //      int i;
        QDomDocument doc;
        QDomElement root;
        QDomNode node;
        QDomNode node1,node2,node3,node4,node5,node6;

        QString fnamec;
	
        fnamec= ServerDir->text();

        #ifdef LINUX_D

            fnamec= fnamec+f_name_snap;
        #endif
        #ifdef WIN_D

            fnamec= fnamec+f_name_snap;
        #endif

	
	if (flConsole) fnamec= ServerDir->text();
    if (flConsole==1)  fnamec= fnamec + name_file_console;
    if (flConsole==2)  fnamec= name_file_console;
 
	qDebug()<<"lll";
        qDebug()<<fnamec;


        QFile f(fnamec);


        col_comm=0;// obnulaem spisok comand
    //spisok vozmognih command
    JComm[0].comm = SM1;
    JComm[0].comm16 = SM1_16;
    JComm[1].comm = SM2;
    JComm[1].comm16 = SM2_16;
    JComm[2].comm = SM3;
    JComm[2].comm16 = SM3_16;
    JComm[3].comm = SM4;
    JComm[3].comm16 = SM4_16;
    JComm[4].comm = SM5;
    JComm[4].comm16 = SM5_16;
    JComm[5].comm = SM6;
    JComm[5].comm16 = SM6_16;
    JComm[6].comm = SM7;
    JComm[6].comm16 = SM7_16;
    JComm[7].comm = SM8;
    JComm[7].comm16 = SM8_16;
    //end spisok command


    nc_snap=0;// kol-vo komand dla snap
   // int icickl =0;


        QString errorStr;
        int errorLine;
        int errorColumn;
        if (!doc.setContent(&f,  true, &errorStr, &errorLine,&errorColumn)) {

           qDebug("Error read snap\n");
           return;

        }
        root = doc.documentElement();
//        if (root.tagName() != "bim_xml") {
  //          printf("Error format file! \n");
    //        return;
      //  }


        QString rrr1;

        node = root.firstChild();

        rrr1=node.toElement().tagName();
 //       qDebug()<<rrr1;


         while(!node.isNull()){
//             qDebug()<<"qqq";
//             rrr1=node.toElement().tagName();
//             qDebug()<<rrr1;
            if (node.toElement().tagName() == "stage") {
                node1 = node.firstChild();
                while(!node1.isNull()){
                    rrr1=node1.toElement().tagName();
//                    qDebug()<<rrr1;
                    if (node1.toElement().tagName() == "sprites") {

                        node2 = node1.firstChild();
                        while(!node2.isNull()){
                            rrr1=node2.toElement().tagName();
//                            qDebug()<<rrr1;

                            if (node2.toElement().tagName() == "sprite") {

                                node3 = node2.firstChild();
                                while(!node3.isNull()){
                                    rrr1=node3.toElement().tagName();
//                                    qDebug()<<rrr1;


                                    if (node3.toElement().tagName() == "scripts") {

                                        node4 = node3.firstChild();
                                        while(!node4.isNull()){
                                            rrr1=node4.toElement().tagName();
//                                            qDebug()<<rrr1;


                                            if (node4.toElement().tagName() == "script") {
                                                node5 = node4.firstChild();
                                                while(!node5.isNull()){
                                                    rrr1=node5.toElement().tagName();
//                                                    qDebug()<<rrr1;





                                                    rrr1 = node5.toElement().attribute("s");
                    //                                int numpi=rrr1.toInt();
                                                    for(int i=0;i<NUM_COMM_SNAP;i++){
                                                        if (rrr1==JComm[i].comm){
                                                            AddComm(JComm[i].comm16,nc_snap);
                                                            nc_snap++;

                                                            if (i==2){
//                                                                qDebug()<<"MM\n";

                                                                AddCicklSnap(node5);

                                                            }
                                                        }
                                                    }


                                                    node5 = node5.nextSibling();
                                                }
                                            }
                                            node4 = node4.nextSibling();
                                        }
                                    }

                                    node3 = node3.nextSibling();
                                }
                            }


                            node2 = node2.nextSibling();
                        }
                    }
                    node1 = node1.nextSibling();
                }

             }
             node = node.nextSibling();
         }


  QString ret="";
for(int ccl=0;ccl<nc_snap;ccl++){
    ret = ret + sp_comm[ccl].comm16 +" ";
}
if (col_comm>0) ret = ret + "8F";

    qDebug()<<ret;
txt =ret;


}

void Vench::ReadJson(QString &txt)
{

 qDebug("read json\n");
    QString fnamec;

    fnamec= ServerDir->text();
//    qDebug()<<fnamec;
    #ifdef LINUX_D
//    fnamec= fnamec+"/"+f_name_kum;
    fnamec= fnamec+f_name_json;
    #endif
    #ifdef WIN_D
//    fnamec= fnamec+"\\"+f_name_kum;
    fnamec= fnamec+f_name_json;
    #endif
    qDebug()<<f_name_json;


    QFile fl_com(fnamec);

    QByteArray rr = fnamec.toLocal8Bit();

    QString t;


       int cicl =0;
//       int com_cicl=0;
//       int iter=0;

    bool fll=    fl_com.open(QIODevice::ReadOnly);

    if (fll!= true){
        qDebug("Error open file json\n");
  //      qDebug("Create file kumir\n");
 //   print_nastr_file(2);
        return;
    }
    QTextStream tt( &fl_com);
    int i=0;
    int fl=0;
    int k=0;
    QString ret;
//    tt.setCodec(QTextCodec::codecForName("UTF-16LE"));

    ret = "";

    col_comm=0;// obnulaem spisok comand
//spisok vozmognih command
Comannd JComm[NUM_COMM_JSON];
JComm[0].comm = CM1;
JComm[0].comm16 = CM1_16;
JComm[1].comm = CM2;
JComm[1].comm16 = CM2_16;
JComm[2].comm = CM3;
JComm[2].comm16 = CM3_16;
JComm[3].comm = CM4;
JComm[3].comm16 = CM4_16;
JComm[4].comm = CM5;
JComm[4].comm16 = CM5_16;
JComm[5].comm = CM6;
JComm[5].comm16 = CM6_16;
JComm[6].comm = CM7;
JComm[6].comm16 = CM7_16;
JComm[7].comm = CM8;
JComm[7].comm16 = CM8_16;
JComm[8].comm = CM9;
JComm[8].comm16 = CM9_16;
JComm[9].comm = CM10;
JComm[9].comm16 = CM10_16;
JComm[10].comm = CM11;
JComm[10].comm16 = CM11_16;
JComm[11].comm = CM12;
JComm[11].comm16 = CM12_16;
JComm[12].comm = CM13;
JComm[12].comm16 = CM13_16;
JComm[13].comm = CM14;
JComm[13].comm16 = CM14_16;
JComm[14].comm = CM15;
JComm[14].comm16 = CM15_16;
JComm[15].comm = CM16;
JComm[15].comm16 = CM16_16;
JComm[16].comm = CM17;
JComm[16].comm16 = CM17_16;
//end spisok command

int fl_cicl =0;
int ncicl = 0;
        t = tt.readAll();
	t = t.toLocal8Bit();
	cicl=0;
        while(1){

		fl=0;
	    
		for (i=0;i<NUM_COMM_JSON;i++){

		    k = t.indexOf(JComm[i].comm);
	    	    if (k>=0)  {
    			fl++;
    			AddComm(JComm[i].comm16,k);

    			t.remove(k,JComm[i].comm.length());
    			
    			for(int l=0;l<JComm[i].comm.length();l++){
    			    t.insert(k," ");
    			}
    			//qDebug("text = \n");
    			//qDebug()<<t;
    		    
    		    //razbor repeat
    			if (JComm[i].comm=="repeat"){
    			    
			    QString h = t.right(t.length()-k-1-6);
			    QString Cl = h.left(h.indexOf(" "));
			    ncicl = Cl.toInt();
			    qDebug("nc = %d\n",ncicl);
			    for(int j=0;j<ncicl-1;j++){
    				AddComm(JComm[i].comm16,k+j);
				
			    }
			    fl_cicl = 1;
    			}

    			if (JComm[i].comm=="end"){
    			    if (fl_cicl>0){
    				fl_cicl=0;
				for(int j=0;j<ncicl-1;j++){
    				    AddComm(JComm[i].comm16,k+j);
				
				}
				ncicl=0;
    			    
    			    }
    			}
    		    }


		}
		
		if (fl==0) break;

    }



//    qDebug("col_comm = %d\n",col_comm);

    ret="";
    for(cicl=0;cicl<col_comm;cicl++){
        ret = ret + sp_comm[cicl].comm16 +" ";
    }
    if (col_comm>0) ret = ret + "8F";
    
//    qDebug()<<ret;
    txt =ret;


}// end   void Vench::ReadJson(QString &txt)


void Vench::on_BStop_B_released()
{

}

//BASE
void Vench::on_pushButton_clicked()
{
     //formHlp.SetPathFileHlp(fileHlp);
    //qDebug() << fileHlp;
      //formHlp.exec();
      on_pushButton_3_clicked();
}

void Vench::on_pushButton_2_clicked()
{
//    formHlp.SetPathFileHlp(fileHlp);
//    formHlp.exec();
    on_pushButton_3_clicked();
}

void Vench::on_pushButtonHelp_clicked()
{

    //formHlp.SetPathFileHlp(fileHlp);
    //formHlp.exec();
    on_pushButton_3_clicked();
}

void Vench::on_pushButton_3_clicked()
{
    QString helpFilePath = QCoreApplication::applicationDirPath() + "/umkiguide.pdf";
    if (QFileInfo(helpFilePath).exists())
        QDesktopServices::openUrl(QUrl(helpFilePath, QUrl::TolerantMode));
    else
        QDesktopServices::openUrl(QUrl("https://www.umkikit.ru/prog/umkiguide.pdf"));

#ifdef LINUX_D


//   QString     ms = "firefox ";// + tt + " &";
//20210426  berem ima brousera iz ini-file
	QProcess verStream;

	QString     ms = fileBrs;
    
        ms=ms+" ";
	

//fl_altlin=1;
    if (fl_altlin==0){//20210426 lubaya oper system krome altlinux
       QString tt;
       tt= ServerDir->text();
       tt =tt+ fileHlp;
	ms = ms+tt;

	
   qDebug()<<ms;

        QByteArray rr = ms.toLocal8Bit();

        rr = ms.toLocal8Bit();


//    	verStream.start(rr);
    	verStream.start(ms);
	verStream.waitForFinished();
	QString err = verStream.readAllStandardOutput();
	
//qDebug()<<err;
    
//20210426	int i = system(rr);
//20210426        i=i+1;
    }
    else{// 20210426 altlinux

	QString commands("rpm -q --queryformat \"%{VERSION}\" smartcar ");    


    	verStream.start(commands);
	verStream.waitForFinished(2000);
	QString verS = verStream.readAllStandardOutput();

	QString pf("/usr/share/doc/smartcar-");

	pf = pf+verS;
	pf = pf+"/"+fileHlp;

	ms =ms+pf;
    	verStream.start(ms);
	verStream.waitForFinished();

	//qDebug("Nomer Versii");
	//qDebug() << verS;
	//qDebug("File help");
	//qDebug() << pf;
        QByteArray rr = verS.toLocal8Bit();

	printf("Nomer Versii\n");
	printf("%s \n", rr.constData());

	
	rr = pf.toLocal8Bit();
	printf("File help\n");
	printf("%s \n", rr.constData());
    }

#endif

}
