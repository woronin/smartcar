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

#include <sys/time.h>


MyThread::MyThread(void(*call_f)(void*),void *arg):QThread()
{
    func = call_f;
    param = arg;
}

void MyThread::run()
{
    (*func)(param);
}

int Vench::mSleep(int zd)
{
    QEventLoop l;
    QTimer::singleShot(zd, &l, SLOT(quit()));
    l.exec();
    return zd;
}

void Vench::DatKas(Dat dat) //0 - right 1 - left
{
    // qWarning: задать цвет датчику красный
    fl_datKas = 1;
    emit DatChanged(dat);
}

Vench::Vench(QWidget* parent, QString kat, QString prt, bool isBluetoothMode, int fKon, QString nf):QDialog( parent,Qt::Window|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint),
    ui(new Ui::Vench)
{
    fl_bluetooth = isBluetoothMode;
    fl_allDevices = false;
    fl_diskr = true;
    fl_unsleep = false;
    fl_xml = false;
    fl_allDevices = false;
    fl_diskr = true;
    fl_temp = true;

    fl_kat = 0;
    fl_port_s = 0;

    ui->setupUi(this);
    mSleep(100);

    timer = new QTimer(this);

    fl_ans3_ust = 0;

    ansStatus = 0;

    // ?????????????????????? //
    //checkVibrUst->setChecked( false );

    connect (timer,SIGNAL(timeout()),this,SLOT(ProcessTimer()));
    timer->start(200);

    if (!kat.isEmpty())
        SetKatalog(kat);
    if (!prt.isEmpty())
        SetPort(prt);
    init();

    flConsole = fKon;
    name_file_console = nf;
/*
    connect( BSendEth , SIGNAL( clicked() ), this, SLOT(BPressSendEth() ) );
    connect( BSendEthAT , SIGNAL( clicked() ), this, SLOT(BPressSendEthAT() ) );

    connect( listV, SIGNAL( itemSelectionChanged() ), this, SLOT( SpVibr() ) );


     VvodPD = new FormView(this);



//for new panel
    connect( ui->listV_n, SIGNAL( itemSelectionChanged() ), this, SLOT( SpVibr_n() ) );

//end for new panel

*/
}

void Vench::SetXMLFlag(bool value)
{
    fl_xml = value;
    chKum();
}

void Vench::SetDiskrFlag(bool value)
{
    fl_diskr = value;
}

void Vench::SetAllDevicesFlag(bool value)
{
    fl_allDevices = value;
}

void Vench::SetTempFlag(bool value)
{
    fl_temp = value;
}

void Vench::SbrosDatKas()
{
    if (fl_datKas == 0)
        return;
    fl_datKas = 0;
    emit DatChanged(Dat::None);
}

int Vench::COMClose()
{
    if (flcomport)
    {
#ifdef QSERIAL
        com_port_qts.close();
#endif
#ifndef QSERIAL
#ifdef LINUX_D
        ::close(com_port);
        qDebug() << "Close  COM-port\n";
#endif
#ifdef WIN_D
        CloseHandle(hCOM);
#endif
#endif
    }
    fl_ans3_ust = 1;
    flcomport = 0;
    return 1;
}

Vench::~Vench()
{
    COMClose();
    destroy();
}

double Vench::GetTime()
{
    return (QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0);
}

void Vench::SpVibr()
{
    int k = listV->count();
    if (k > 0)
        checkVibrUst->setChecked(true);
    int fl_izmm = 0;
    fl_vibr_ust = 0;
    for(int i = 0; i < k; ++i)
    {
        if (listV->item(i)->isSelected())
        {
            if (act_ust[i].fl_antisleep != 2)
                continue;
            if (mas_vibr_ust[i] == 0)
                fl_izmm = 1;
            mas_vibr_ust[i] = 1;
            act_ust[i].vibr_sp = 1;
            fl_vibr_ust = 1;
        }
        else
        {
            if(fl_clsp == 1)
            {
                fl_clsp = 0;
                continue;
            }
            if (mas_vibr_ust[i] == 1)
                fl_izmm = 1;
            mas_vibr_ust[i] = 0;
            act_ust[i].vibr_sp = 0;
        }
    }
    if (fl_izmm == 1)
        fl_obn_spact = 4;
}

void Vench::SpVibr_n()
{
    int k = ui->listV_n->count();
    if (k > 0)
        checkVibrUst->setChecked(true);
    int fl_izmm = 0;
    fl_vibr_ust = 0;
    for(int i = 0; i < k; ++i)
    {
        if (ui->listV_n->item(i)->isSelected())
        {
            if (act_ust[i].fl_antisleep != 2)
                continue;
            if (mas_vibr_ust[i] == 0)
                fl_izmm = 1;
            mas_vibr_ust[i] = 1;
            act_ust[i].vibr_sp = 1;
            fl_vibr_ust = 1;
        }
        else
        {
            if(fl_clsp == 1)
            {
                fl_clsp = 0;
                continue;
            }
            if (mas_vibr_ust[i] == 1)
                fl_izmm = 1;
            mas_vibr_ust[i] = 0;
            act_ust[i].vibr_sp = 0;
        }
    }
    if (fl_izmm==1)
        fl_obn_spact = 4;
}

void Vench::ClearSpBeg()
{
    if(fl_begin == 1)
    {
        if (ui->BCar_1)
            ui->BCar_1->setEnabled(false);
        if (ui->BCar_2)
            ui->BCar_2->setEnabled(false);
    }
}

void Vench::ProcessTimer()
{
    if (flConsole)
    {
        qDebug() << serverDir;
        SetXMLFlag(true);
        ProgramClicked();
        disconnect (timer, SIGNAL(timeout()), this, SLOT(ProcessTimer()));
        COMClose();
        this->done(0);
        qDebug() << "program done";
    }

    for(int i = 0; i < k_hid; ++i)
    {
        if (mas_fl_hid[i] != 2)
        {
            /*listV->setRowHidden(mas_hid[i], true);
            ui->listV->setRowHidden(mas_hid[i], true);
            ui->listV_n->setRowHidden(mas_hid[i], true);

            list_PANID->setRowHidden(mas_hid[i], true);
            ui->list_PANID_n->setRowHidden(mas_hid[i], true);

            list_vib->setRowHidden(mas_hid[i], true);
            ui->listVibr->setRowHidden(mas_hid[i], true);
            ui->listVibr_n->setRowHidden(mas_hid[i], true);
            ///for new regim prostoy
            if ((mas_hid[i] == 0) && (ui->BCar_1))
                ui->BCar_1->setEnabled(false);
            if ((mas_hid[i] == 1) && (ui->BCar_2))
                ui->BCar_2->setEnabled(false);
            ///end for new regim prostoy*/
        }
        else
        {
            /*listV->setRowHidden(mas_hid[i], false);
            ui->listV->setRowHidden(mas_hid[i], false);
            ui->listV_n->setRowHidden(mas_hid[i], false);

            list_PANID->setRowHidden(mas_hid[i], false);
            ui->list_PANID_n->setRowHidden(mas_hid[i], false);

            list_vib->setRowHidden(mas_hid[i], false);
            ui->listVibr->setRowHidden(mas_hid[i], false);
            ui->listVibr_n->setRowHidden(mas_hid[i], false);
            ///for new regim prostoy
            if ((mas_hid[i] == 0) && (ui->BCar_1))
                ui->BCar_1->setEnabled(true);
            if ((mas_hid[i] == 1) && (ui->BCar_2))
                ui->BCar_2->setEnabled(true);
            ///end for new regim prostoy*/
        }
    }

    k_hid = 0;

    if (fl_begin == 3) //regim SUPER_N
    {
        /*ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(2);*/
    }
    if (fl_begin == 1) //regim START
    {
        //setMinimumSize(QSize( 600, 370));
        fl_diskr = false;
        /*checkVibrUst->setChecked(true); //chtoby rabotal po algoritmu vibrannih ustroistv
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(0);*/
    }
    if (fl_begin == 0) //regim SUPER
    {
        /*setMinimumSize(QSize(600, 670));
        ui->stackedWidget->hide();
        tabWidget2->show();*/
    }
    if (fl_begin == 2) //regim BASE
    {
        /*setMinimumSize(QSize(600, 370));
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(1);*/
    }

    if (fl_begin == 4)//regim BlueTooth
    {
        /*setMinimumSize(QSize(600, 370));
        ui->stackedWidget->show();
        ui->stackedWidget->setCurrentIndex(3);*/
    }

    /*if(fl_repaint_panid == 1)
    {
        list_PANID->clear();
        int k = listV->count();
        if (k > MAX_USTR)
            k = 0;
        for(int i = 0; i < k; ++i)
        {
            QString stt;
            if (act_ust[i].fl_pan == 1)
                stt = QString().setNum(act_ust[i].pan1, 16) + " ";
            else
                stt ="* ";

            list_PANID->addItem(stt);
            if (ui->list_PANID_n)
                ui->list_PANID_n->addItem(stt);
            SetHiddenCar(i, act_ust[i].fl_antisleep);
        }
        fl_repaint_panid = 0;
    }*/

    if(fl_ans3_ust == 1)
    {
        ansStatus = 2;
        emit AnsStatusChanged();

        if (flcomport == 0)
        {
            ansStatus = 0;
            emit AnsStatusChanged();
        }

        fl_ans3_ust = 0;
    }

    if (fl_obn_spact == 1)
    {
        /*SpActDatch->clear();
        listV->clear();
        list_PANID->clear();
        ui->list_PANID_n->clear();

        //for BASE
        ui->listV->clear();
        ui->listV_n->clear();*/

        if (flcomport != 0)
        {
            ansStatus = 1;
            emit AnsStatusChanged();
        }
    }
    else if (fl_obn_spact == 2)
    {
        /*SpActDatch->insertItem(MAX_USTR, txtAct);
        listV->addItem(txtAct);
        ui->listV->addItem(txtAct);
        ui->listV_n->addItem(txtAct);*/
        if (kol_ust > 0)
        {
            SetHiddenCar(kol_ust - 1, act_ust[kol_ust - 1].fl_antisleep); //chtoby ne pokazivalsa esli net otveta na komandu ne spat
            //SpActDatch->setCurrentIndex(0);
        }
    }
    else if (fl_obn_spact == 3)
    {
        /*SpActDatch->clear();
        listV->clear();
        ui->listV->clear();
        ui->listV_n->clear();*/

        ClearSpBeg();

        QString tt,txt;
        for (int i = 0; i < kol_ust; ++i)
        {
            if (act_ust[i].vibr_sp == 1)
                txt = " ";
            else
                txt = " ";
            tt.setNum(i + 1, 10);
            txt = txt + tt + " : ";
            for(int j = 0; j < 8; ++j)
            {
                tt.setNum(mas_mac_addr[i][j],16);
                txt = txt + tt+" ";
            }
            txt = txt + "; ";
            for(int j = 0; j < 2; ++j)
            {
                tt.setNum(mas_set_addr[i][j], 16);
                txt = txt + tt + " ";
            }

            if (act_ust[i].fl_sleep == 0)
                txt = txt + trUtf8("; без сна; ");
            if (act_ust[i].fl_sleep == 1)
                txt= txt + trUtf8("; может спать; ");

            tt.setNum(act_ust[i].impl, 16);
            txt = txt + tt + ", ";
            tt.setNum(act_ust[i].impr, 16);
            txt = txt + tt;

             /*listV->addItem(txt);
             ui->listV->addItem(txt);
             ui->listV_n->addItem(txt);

             ///for new regim prostoy
             if (i == 0)
                 ui->BCar_1->setEnabled(true);
             if (i == 1)
                 ui->BCar_2->setEnabled(true);
             ///end for new regim prostoy*/

             SetHiddenCar(i, act_ust[i].fl_antisleep);
        }
        //SpActDatch->insertItem(MAX_USTR, txt);
        fl_repaint_panid = 1;
    }
    else if (fl_obn_spact == 4)
    {
        /*list_vib->clear();
        ui->listVibr->clear();
        ui->listVibr_n->clear();

        int k = listV->count();
        if (k > MAX_USTR)
            k = 0;
        for(int i = 0; i < k; ++i)
        {
            QString stt;
            if (act_ust[i].vibr_sp == 1)
                stt = "*";
            else
                stt = " ";
            list_vib->addItem(stt);
            ui->listVibr->addItem(stt);
            ui->listVibr_n->addItem(stt);

            SetHiddenCar(i, act_ust[i].fl_antisleep);
        }*/
    }

    fl_obn_spact = 0;

    if (fl_play == 0)
    {
        // ToDo: проверить изменение текста
        /*if(ui->pBPlay->isChecked() == 1)
        {
            ui->pBPlay->setChecked(0);
            ui->pBPlay->setText(trUtf8("Поехали"));
            ui->pBPlay->setStyleSheet("background: lightgray");
        }*/
    }

    if(fl_unsleep)
    {
        if (GetTime() > (TimeNeSpat + PER_TIMENESPAT))
        {
#ifdef PRINT_DEBUG
            qDebug("Send Comm Ne Spat\n");
#endif
            TimeNeSpat = GetTime();
            BPressStop();
        }
    }
    // init com-port
    if (flcomport == 0)
    {
        if ((-timer_init_port + GetTime()) > TIMER_INIT_PORT)
        {
            QByteArray rr = addr.toLocal8Bit();
            qDebug("ustroistvo vvoda-vivoda:  %s\n", rr.constData()); // smotrim kakoe ustroistvo otkrilos
            if (COMInit(rr.constData()) != 1)
                qDebug("Error open port Exit\n");
            timer_init_port = GetTime();
        }
        else if (GetTime() > 3 * timer_init_port)
            timer_init_port   = GetTime();

        if (flcomport != 0)
        {
            qWarning("flcomport == 0  flcomport != 0");
            ansStatus = 1;
            emit AnsStatusChanged();
        }
        mSleep(3);
        if (!fl_bluetooth)
        {
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
    unsigned char macm[8];
    for(int j = 0; j < 8; ++j)
        macm[j] = 0;

    int k = listV->count();
    for(int i = 0; i < k; ++i)
        if (mas_vibr_ust[i] == 1)
        {
            QString name_f = "";
            for(int j = 0; j < 8; ++j)
            {
                macm[j] = (unsigned char)mas_mac_addr[i][j];
                name_f += QString().setNum(mas_mac_addr[i][j], 16) + " ";
            }
            VvodPD->SetType(name_f, 0);
            if (VvodPD->exec() == 1)
            {
                int pn = VvodPD->GetPanID();
                SendZapisPanID(macm, pn);
                int kk = GetNumberUst(macm);
                if ((PanIdKontr != -1) && (pn != PanIdKontr) && (kk >= 0))
                {
                    RemoveUstr(kk);
                    fl_repaint_panid = 1;
                    fl_obn_spact = 3;
                }
            }
        }
}
void Vench::BPressLog()
{
    QString ms = "xterm -e tail -f ~/.config/smartcar/";
    ms += LOGFILE;
    ms += " &";
#ifdef PRINT_DEBUG
    qDebug() << ms;
#endif
    system(ms.toLocal8Bit().constData());
}
void Vench::BPressParam()
{
    print_nastr_file();
#ifdef WIN_D
   WinExec(QString("tmp_smart.ini ").toLocal8Bit(), SW_RESTORE);
#endif
#ifdef LINUX_D
   system(QString(mEdit + QDir().homePath() + "/.config/smartcar/tmp_smart.ini" + " ").toLocal8Bit());
#endif
}

void Vench::BPressBDiscrImp_Ever()
{
    printf("Press Diskr imp\n");
    unsigned char macm[8];
    for(int j = 0; j < 8; ++j)
        macm[j]=0;

    int k = listV->count();
    for(int i = 0; i < k; ++i)
    {
        if (mas_vibr_ust[i] == 1)
        {
            QString name_f = "";
            for(int j = 0; j < 8; ++j)
            {
                macm[j] = (unsigned char)mas_mac_addr[i][j];
                name_f += QString().setNum(mas_mac_addr[i][j], 16) + " ";
            }
            VvodPD->SetType(name_f, 1);
            if (VvodPD->exec() == 1)
            {
                int impr, impl;
                if (VvodPD->GetKolImp(&impr, &impl) == 1)
                    SetKolImpUstr(macm, impr, impl);
            }
        }
    }

    fl_obn_spact = 3;
}

void Vench::BPressIzmPANID_KONTR()
{
    printf("Press IzmPANID_KONTR\n");
    VvodPD->SetType(trUtf8("ВНИМАНИЕ PanID контроллера должен \nсоответствовать остальным \nустройствам" ), 0);
    if (VvodPD->exec() == 1)
    {
        int pn = VvodPD->GetPanID();
        SendZapisPanID_KONTR(pn);
        ObSpActUst();
    }
}

QString Vench::PrivodStroki(QString st)
{
    QString text = st.toUpper();
    text.replace("  ", " ");
    for (int i = 0; i < 16; ++i)
    {
        QString ch = QString().setNum(i, 16).toUpper();
        text.replace(" 0" + ch, " " + ch);
    }
    return text;
}

void Vench::BPressStop()
{
    if(fl_play)
        fl_play=0;

//	BStop->setDown(1);
    PressRh=0;
    PressLf=0;
    PressRv=0;
    PressFw=0;
    SbrosDatKas();
    SendCommStop1();

}
void Vench::BRealeseStop() // QWarning: переименовать в BReleaseStop
{
    if(fl_play)fl_play=0;
    if(fl_rec){
        if (numc>(NUM_COMM-2)) return;
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


bool Vench::BPressRec(int listCount)
{
    int k = listCount;
    if (k > MAX_USTR)
        k = 0;
    int mkl = 0;
    for(int i = 0; i < k; ++i)
        if (act_ust[i].vibr_sp == 1)
            ++mkl;
    if (mkl == 0)

        if (k > 0)
        {
            for(int j = 0; j < 8; ++j)
                mac_addr_prg[j] = act_ust[0].mac_addr[j];
        }
        else
        {
            QMessageBox::information(this, trUtf8("Предупреждение"),
                                     trUtf8("Нет активных устройств\n"),
                                     trUtf8("Ok") );
            return false;
        }
    if (mkl > 1)
    {
        QMessageBox::information(this, trUtf8("Предупреждение"),
                                 trUtf8("Для записи программы выберите только одно устройство\n"),
                                 trUtf8("Ok") );
        return false;
    }
    if (fl_allDevices)
    {
        QMessageBox::information(this, trUtf8("Предупреждение"),
                                 trUtf8("Снимите признак \"Все устройства\" \n"),
                                 trUtf8("Ok") );
        return false;
    }

    if (mkl == 1)
        for(int i = 0; i < k; ++i)
            if (act_ust[i].vibr_sp == 1)
                for(int j = 0; j < 8; ++j)
                    mac_addr_prg[j] = act_ust[i].mac_addr[j];

    fl_play = 0;
    numc = 0;
    num_comm = 0;
    ret_napr = 0;
    rimp1 = 0;
    rimp2 = 0;

    fl_rec = 1;
    return true;
}

void Vench::BRealeseRec()
{
    if (fl_rec)
    {
#ifdef PRINT_DEBUG
        qDebug("nc=%d\n",num_comm);
#endif
        WriteProg1(num_comm);
        fl_rec = 0;
    }
}

void Vench::BPressProdol()
{
    printf("continue prog (don`t wait answer ustr)\n");
}

void Vench::BPressPlay()
{
    if(fl_play == 0)
    {
        if (fl_xml)
            ReadProg1();
        else
            ReadCum();

        //BRec->setDown(0);
        fl_rec = 0;
        fl_play = 1;

        //int k = listV->count();
        /*for(int i = 0; i < k; ++i)
            if (act_ust[i].vibr_sp == 1)
            {
                for(int j = 0; j < 8; ++j)
                    mac_addr_prg[j] = act_ust[i].mac_addr[j];
                break;
            }
*/
        for(int uu = 0; uu < num_comm; ++uu)
        {
#ifdef PRINT_DEBUG
            qDebug("m1 =%d\n", mass_comm_n[uu][0]);
            qDebug("m2 =%d\n", mass_comm_n[uu][1]);
            qDebug("m3 =%d\n", mass_comm_n[uu][2] );
#endif
        }
    }
    else
    {
        fl_play = 0;
        SendCommStop1();
    }
}

void Vench::BPressLight()
{
    if (fl_play)
        return;
    SendCommLight1();
}

void Vench::SendCommLight1()
{
    if (fl_comm > 0)
        return;
    fl_comm = 1;

    if (SpActDatch->count() == 0)
    {
#ifdef PRINT_DEBUG
        qDebug("Net active ustr\n");
#endif
        return;
    }

    int mk[10];

    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] = mas_set_addr[j][i];
                FormatAndSendComm("& b 3 44 37 5 ", mk);
            }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
        FormatAndSendComm("& b 3 44 37 5 ", mk);
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
        FormatAndSendComm("& b 3 44 37 5 ", mk);
    }
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 0;
        mass_comm_n[num_comm][1] = 0;
        mass_comm_n[num_comm][2] = 0xB1;
        num_comm++;
    }
}

void Vench::SendCommAntiSon(int * mak_a, int * mas_a)
{
    int mk[10];
    for(int i = 0; i < 8; ++i)
        mk[i] = mak_a[i];
    for(int i = 0; i < 2; ++i)
        mk[i+8] = mas_a[i];
    FormatAndSendComm("& b 3 53 4D 0 ", mk);
}

void Vench::BReleaseLight()
{
    if (fl_play)
        return;
    if (fl_rec)
    {
        if (numc > (NUM_COMM - 2))
            return;
        numc++;
    }
    SendCommStopLight1();
}

void Vench::SendCommStopLight1()
{
    if (fl_comm == 1)
        return;
    fl_comm = 1;

    if (SpActDatch->count() == 0)
    {
        qDebug("Net active ustr\n");
        return;
    }

    int mk[10];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] = mas_set_addr[j][i];
                FormatAndSendComm("& b 3 44 37 4 ", mk);
            }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
    }
    FormatAndSendComm("& b 3 44 37 4 ", mk);
}

void Vench::BPressBip()
{
    if (fl_vibr_ust == 0)
        return; // QWarning used for BBip_pressed but not used for BBip_2_pressed
    if (fl_play)
        return;

    if (PressBip == 0)
    {
        SendCommBip1();
        PressBip = 1;
    }
}


void Vench::SendCommBip1()
{
    if (SpActDatch->count() == 0)
    {
        qDebug("Net active ustr\n");
        return;
    }

    int mk[10];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j =0 ; j < numu; ++j)
        {
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] = mas_set_addr[j][i];
                FormatAndSendComm("& b 3 44 32 5 ", mk);
            }
        }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
        FormatAndSendComm("& b 3 44 32 5 ", mk);
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
        FormatAndSendComm("& b 3 44 32 5 ", mk);
    }
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 0;
        mass_comm_n[num_comm][1] = 0;
        mass_comm_n[num_comm][2] = 0xC1;
        num_comm++;
    }
}


void Vench::BReleaseBip()
{
    if (fl_vibr_ust == 0)
        return; // QWarning same for BPressBip
    if (fl_play)
        return;
    if (fl_rec)
    {
        if (numc > ( NUM_COMM - 2))
            return;
        numc++;
    }
    SendCommStopBip1();
    PressBip = 0;
}


void Vench::SendCommStopBip1()
{
    if (SpActDatch->count() == 0)
    {
        printf("Net active ustr\n");
        return;
    }
    int mk[10];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] =mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] =mas_set_addr[j][i];
                FormatAndSendComm("& b 3 44 32 4 ", mk);
            }
        return;
    }
    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] =0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
        FormatAndSendComm("& b 3 44 32 4 ", mk);
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
        FormatAndSendComm("& b 3 44 32 4 ", mk);
    }
}



void Vench::BPressRight()
{
    SbrosDatKas();
    if (fl_play)
        return;
    if (shiftPress == 0)
        SendCommRight1();
    else
        SendCommExtrRight1();
    PressRh = 1;
}

void Vench::BPressLeft()
{
    SbrosDatKas();
    if(fl_play)
        return;
    if (shiftPress == 0)
        SendCommLeft1();
    else
        SendCommExtrLeft1();
    PressLf = 1;
}

void Vench::BReleaseLeft()
{
    if (fl_play)
        return;
    if (fl_rec)
    {
        if (numc > (NUM_COMM - 2))
            return;
        numc++;
    }
    PressLf = 0;
    //!!!2015 07 23 dla proverki diskretnogo dvigenia!!!
    if (!fl_diskr)
        SendCommStop1();
}

void Vench::BReleaseRight()
{
    if (fl_play)
        return;
    if (fl_rec)
    {
        if (numc > (NUM_COMM - 2))
            return;
        numc++;
    }
    PressRh = 0;
    //!!!2015 07 23 dla proverki diskretnogo dvigenia!!!
    if (!fl_diskr)
        SendCommStop1();
}

void Vench::BPressFw()
{
    SbrosDatKas(); // qWarning : переделать потом
    if (fl_play)
        return;
    SendCommForw1();
    PressFw = 1;
}

void Vench::BReleaseFw()
{
    if (fl_play)
        return;
    if (fl_rec)
    {
        if (numc > (NUM_COMM - 2))
            return;
        numc++;
    }
    PressFw = 0;
    if (!fl_diskr)
        SendCommStop1();
}

void Vench::BPressRv()
{
    SbrosDatKas();
    if (fl_play)
        return;
    SendCommRevers1();
    PressRv = 1;
}

void Vench::BReleaseRv()
{
    if (fl_play)
        return;
    if (fl_rec)
    {
        if (numc > (NUM_COMM - 2))
            return;
        numc++;
    }
    PressRv = 0;
    if (!fl_diskr)
        SendCommStop1();
}

void Vench::SendCommTempOn()
{
    qDebug("SendCommTempOn\n");
    if (SpActDatch->count() == 0)
        return;

    int mk[20];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] =mas_set_addr[j][i];
                FormatAndSendComm("& 3 10 ", mk, " FF FF");
            }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] =0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
    }
    FormatAndSendComm("& 3 10 ", mk, " FF FF");
}

void Vench::SendCommTempOff()
{
    if (SpActDatch->count() == 0)
        return;
    int mk[20];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] = mas_set_addr[j][i];
                FormatAndSendComm("& 3 0 ", mk, " FF FF");
            }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
    }
    FormatAndSendComm("& 3 0 ", mk, " FF FF");
}

void Vench::SendCommBarOn()
{
    if (SpActDatch->count() == 0)
    {
        qDebug("Net active ustr\n");
        return;
    }
    int mk[10];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] = mas_set_addr[j][i];
                FormatAndSendComm("* 7E 0 F 10 11 ", mk, "FF FE 0 0 A5");
            }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
        FormatAndSendComm("* 7E 0 F 10 11 ", mk, "FF FE 0 0 A5");
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
        FormatAndSendComm("* 7E 0 F 10 11 ", mk, "FF FE 0 0 A5");
    }
}

void Vench::SendCommEnergOn()
{
    qDebug("SendCommEnergOn\n");
    if (SpActDatch->count() == 0)
        return;
    int mk[20];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] =mas_set_addr[j][i];
                FormatAndSendComm("& 4 10 ", mk, " FF FF");
            }
        return;
    }
    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
        FormatAndSendComm("& 4 10 ", mk, " FF FF");
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
        FormatAndSendComm("& 4 10 ", mk, " FF FF");
    }
}

void Vench::SendCommEnergOff()
{
    if (SpActDatch->count() == 0)
        return;
    int mk[20];
    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] = mas_set_addr[j][i];
                FormatAndSendComm("& 4 0 ", mk, " FF FF");
            }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mk[i] = 0;
        mk[6] = 0xff;
        mk[7] = 0xff;
        mk[8] = 0xff;
        mk[9] = 0xfe;
        FormatAndSendComm("& 4 0 ", mk, " FF FF");
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
        FormatAndSendComm("& 3 0 ", mk, " FF FF");
    }
}

void Vench::FormatAndSendComm(QString cm, int mk[10], QString postfix_cm)
{
    for (int i = 0; i < 10; ++i)
        cm += QString().setNum(mk[i], 16) + " ";
    cm += postfix_cm;
    SendComm(cm);
}

void Vench::SendComm( QString hcm )
{
    QString  cm = PrivodStroki(hcm) + "\n";

    SendCommForRS(cm.toLocal8Bit().constData());
    if (fl_comm == 1)
        fl_comm = 0;
    mSleep(tm_usleep_com);
}

void Vench::ObSpActUst()
{
    kol_ust = 0;
    mkol_ust = 0;
    fl_obn_spact = 1;

    mSleep(50);
    SendCommObnovit();
}

void Vench::SendCommObnovit()
{
    SendComm("& 0 1 ff ff");
}


void Vench::keyPressEvent( QKeyEvent *e )
{
    int k = e->key();
    if (tabWidget2->currentIndex() != 0)
        return;

    if ((fl_begin == 1) && (fl_vibr_ust == 0)) // chtoby ne srabativalo v nachalnem regime pri nevibrannih ustr-h
        return;

    // up    - 4115
    // down  - 4117
    // left  - 4114
    // right - 4116
    // shift - 4128
    if (e->isAutoRepeat())
        return;

    switch(k)
    {
    case 77://m
        SendCommBudil();
        BAntiSleep->setDown(1);
        break;
    case 80://p
        if (fl_play == 0)
        {
            fl_play = 1;
        }
        BPlay->setDown(1);
        break;
    case 82://r
        if (fl_play == 1)
            fl_play = 0;
        BPlay->setDown(0);
        BRec->setDown(1);
        if (fl_rec == 0)
        {
          numc = 0;
          num_comm = 0;
          fl_rec = 1;
        }
        break;
    case 87://up
        if (fl_bluetooth)
        {
            on_BUp_B_pressed();
            break;
        }
        BUp->setDown(1);
        if (fl_play)
            break;
        if (kp == 1)
            break;
        kp = 1;
        if (PressRh == 1)
        {
            PressRh = 0;
            SendCommStop1();
            PressRh = 1;
        }
        if (PressRv == 1)
        {
            PressRv = 0;
            SendCommStop1();
            PressRv = 1;
        }
        if (PressLf == 1)
        {
            PressLf = 0;
            SendCommStop1();
            PressLf = 1;
        }
        SbrosDatKas();
        SendCommForw1();
        PressFw = 1;
        qDebug("Send klav comm Forw\n");
        break;
    case 83://down
        if (fl_bluetooth)
        {
            qDebug("BT:down\n");
            on_BDown_B_pressed();
            break;
        }
        BDown->setDown(1);
        if (fl_play)
            break;
        if (PressRh == 1)
        {
            PressRh = 0;
            SendCommStop1();
            PressRh = 1;
        }
        if (PressFw == 1)
        {
            PressFw = 0;
            SendCommStop1();
            PressFw = 1;
        }
        if (PressLf == 1)
        {
            PressLf = 0;
            SendCommStop1();
            PressLf = 1;
        }
        SbrosDatKas();
        SendCommRevers1();
        PressRv=1;
        qDebug("Send klav comm revers\n");
        break;
    case 65://left
        if (fl_bluetooth)
        {
            on_BLeft_B_pressed();
            break;
        }
        BLeft->setDown(1);
        if (fl_play)
            break;
        if (PressRh == 1)
        {
            PressRh = 0;
            SendCommStop1();
            PressRh=1;
        }
        if (PressFw == 1)
        {
            PressFw = 0;
            SendCommStop1();
            PressFw = 1;
        }
        if (PressRv == 1)
        {
            PressRv = 0;
            SendCommStop1();
            PressRv = 1;
        }

        SbrosDatKas();

        if (shiftPress == 0)
            SendCommLeft1();
        if (shiftPress == 1)
            SendCommExtrLeft1();
        PressLf = 1;
        qDebug("Send klav comm left\n");
        break;
    case 68:// right
        BRight->setDown(1);
        if (fl_bluetooth)
        {
            on_BRight_B_pressed();
            break;
        }
        if (fl_play)
            break;
        if (PressRv == 1)
        {
            PressRv = 0;
            SendCommStop1();
            PressRv = 1;
        }
        if (PressFw == 1)
        {
            PressFw = 0;
            SendCommStop1();
            PressFw = 1;
        }
        if (PressLf == 1)
        {
            PressLf = 0;
            SendCommStop1();
            PressLf=1;
        }
        SbrosDatKas();
        if (shiftPress == 0)
            SendCommRight1();
        if (shiftPress == 1)
            SendCommExtrRight1();
        PressRh = 1;
        qDebug("Send klav comm right\n");
        break;
    case 16777248://shift
        shiftPress = 1;
        break;
    case 32://probel
        if (fl_play)
            fl_play = 0;
        SbrosDatKas();

        BStop->setDown(1);
        PressRh = 0;
        PressLf = 0;
        PressRv = 0;
        PressFw = 0;
        SendCommStop1();
        qDebug("Send klav comm stop\n");
        break;
    }
}


void Vench::SetKatalog(QString kat)
{
    katalog = kat;
    fl_kat = 1;
}

void Vench::SetPort(QString kat)
{
    addr = kat;
    fl_port_s = 1;
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
    char str[1024];
    sprintf(str, "%s", st.toLocal8Bit().constData());   //20201121

   //Send AT command
   at_write((int8_t*)str, strlen (str));

//end for dorne
#endif
}



void Vench::init()
{
    timer_init_port = GetTime();

    f_name_xml = NAME_FILE_VAR_NEW;
    f_name_json = NAME_FILE_VAR_NEW;
    f_name_snap = NAME_FILE_VAR_NEW;
    pr_grph = 0;
    rimp1 = 0;
    rimp2 = 0;
    k_hid = 0;

    flzaprosdim = 0;
    flzaprosbar = 0;
    flzaprostemposv = 0;
    progrPostrGRPH = "";
    progrShowGRPH = "";
    TimeNeSpat = 0;
    for(int k = 0; k < MAX_USTR; ++k)
        mas_vibr_ust[k] = 0;
    for(int k = 0; k < 8; ++k)
        mac_addr_prg[k] = 0;
    //listV->clear();
    t_oprosa_b = 0;
    period_oprosa_b = PERIOD_OPROSA_DAT;

    fl_begin = 0;
    fl_clsp = 0;
    PanIdKontr = -1;
    fl_logm = NULL;
    fl_comm = 0;
    ret_napr = 0;
    fl_obn_spact = 0;
    flcomport = 0;
    fl_n = 0;
    l = 0;
    l_k = 0;
    imp_diskr = 0;
    speed_car = 100;
    prog_for_read_win = "C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe";
    progrPostrGRPH = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe";
    progrShowGRPH = "C:\\Windows\\System32\\mspaint.exe";
    fl_repaint_panid = 0;

    tm_usleep_com = 500;
    pr_spec_file_t = 0;
    def_diskr_impr = DEF_DISKR_IMPR;
    def_diskr_impl = DEF_DISKR_IMPL;
    fl_print_log = false;
    fl_print_log_B = false;
    fl_al_sn = 0;
    t_oprosa = 0;
    t_oprosa_e = 0;

    fl_period = 0;
    period_oprosa_t = 0;
    period_oprosa_e = 0;
    fl_period_e = 0;

    fl_krd = 0;
    fl_krd_start = 0;
    fl_potoc_rs = 1;

    addr = "/dev/ttyS0";// po umolchaniu, esli ne naiden smart.ini
    shiftPress = 1;
    kp = 0;
    PressFw = 0;
    PressRv = 0;
    PressLf = 0;
    PressRh = 0;
    PressBip = 0;

    kol_ust = 0;
    kol_data = 0;
    mkol_ust = 0;

    char * var = getenv("VISUAL");
    if (var != NULL)
        mEdit.append(var);
    else
    {
        var = getenv("EDITOR");
        if (var != NULL)
            mEdit.append(var);
        else
            mEdit = "gedit";
    }
    mEdit += " ";

    QDir dr;
    serverDir = dr.homePath() + "/.config/smartcar/";
    dr.mkdir(serverDir);
    f_name_kum = "script.cum";
    n_imp_kum = 17;
    n_imp_kum_p = 16;
    def_diskr_impr = 17;
    def_diskr_impl = 16;
    tm_usleep_com = 500;
    imp_diskr = 12;
    addr_win = "COM3";
    fl_begin = 2;
    if (fl_bluetooth)
        fl_begin = 4;
    ReadIni();

    mSleep(100);//00000000
    fl_rec = 0;
    fl_play = 0;
    numc = 0;
    num_comm = 0;

    qDebug("ustroistvo vvoda-vivoda:  %s\n", addr.toLocal8Bit().constData()); // smotrim kakoe ustroistvo otkrilos

    if ((flcomport == 0) && (COMInit(addr.toLocal8Bit().constData()) != 1))
    {
        qDebug("Error open port Exit\n");
        QString st = trUtf8("Ошибка инициализации порта: ") + addr;
        QMessageBox::information( this, trUtf8("Ошибка"), st, trUtf8("Ok") );
    }

    if (flcomport != 0)
    {
        qWarning("flcomport != 0");
        ansStatus = 1;
        emit AnsStatusChanged();
    }
    mSleep(3);
    thrRS = new MyThread (TestPotoc, this);
    thrRS->start();

    if (!fl_bluetooth)
    {
        SendVklAPI();
        SendVklNP();
        SendZaprosI();
    }
    ReadProg1();

#ifdef LINUX_D
    channel_open(&channel_drone, PROTO_UDP, WRITE_MODE, PORT_DRONE, (char*)ADDR_SHIR, NULL, NULL, NULL, NULL);
#endif
    fl_datKas = 0;
    mSleep(10);
}


void Vench::destroy()
{
    this->releaseKeyboard();
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
        WriteProg1(num_comm);
        fl_rec=0;
    }
      BRec->setDown(0);
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
        if (numc>(NUM_COMM-2)) break;
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
        if (numc>(NUM_COMM-2)) break;
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
        if (numc>(NUM_COMM-2)) break;
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
        if (numc>(NUM_COMM-2)) break;
        numc++;
//	    break;
    }
    PressRh=0;
    SendCommStop1();
    break;
    case 16777248://shift
        shiftPress = 0;
    break;

    case 32://probel
        if (fl_bluetooth){
        SendCommStop_ave_BT();
            break;
        }


    BStop->setDown(0);
    if(fl_play)fl_play=0;
    if(fl_rec){
        if (numc>(NUM_COMM-2)) break;
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

int Vench::GetNumbUst(int *mac)
{
    int i,j;
    int fl_it;


    fl_it=0;
    for (i=0;i<kol_ust;i++){
        fl_it =1;
        for(j=0;j<8;j++){
            if(mas_mac_addr[i][j]!=mac[j]) fl_it =0;
        }
        if (fl_it==1) return i;
    }
    if (fl_it ==0) return -1;
    if (kol_ust ==0) return -1;
    return -1;
}


void Vench::SetHiddenCar(int i, int fl)
{
    mSleep(50);
    mas_hid[k_hid] = i;
    mas_fl_hid[k_hid] = fl;
    k_hid++;
}





int Vench::RemoveUstr(int k)
{
    if (k < mkol_ust)
    {
        if (mkol_ust > 0)
            --mkol_ust;
        for (int i = k; i < mkol_ust; ++i)
        {
            double tar_kfd = act_ust[i].tar_kfd;
            act_ust[i] = act_ust[i+1];
            act_ust[i].tar_kfd = tar_kfd;
        }

        if(kol_ust > 0)
            --kol_ust;
        for(int i = k; i < kol_ust; ++i)
        {
            for(int j = 0; j < 8; ++j)
                mas_mac_addr[i][j] = mas_mac_addr[i+1][j];

            for(int j = 0; j < 2; ++j)
                mas_set_addr[i][j] = mas_set_addr[i+1][j];
        }

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
            SendCommAntiSon(mas_mac_addr[nump],mas_set_addr[nump]);

#ifdef DVA95
        act_ust[nump].fl_antisleep =1;
#endif
#ifndef DVA95
        act_ust[nump].fl_antisleep =2;
#endif


    }


    }


//
//
////    if ((newp==0)&&(kol_ust==nkol)) {
//
    if ((newp==0)) {// esli nichego novogo net

        if ( SpActDatch->count()!=0) return;
        if ( kol_ust==0) return;

    }

    ClearSp();
    ClearSpBeg();

    if (kol_ust>0){

        ClearSp();
        while(   fl_obn_spact>0) mSleep(10);

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

void Vench::NewScriptPlot(int * mac, int pr)
{
    FILE* fl_com;
    QString fnamec = serverDir + NAME_SCRIPT_FILE;

    fl_com = fopen(fnamec.toLocal8Bit(), "w+");
    fprintf(fl_com, "%s", QString("set terminal png size 1200, 800").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fnamec = serverDir;
    if (pr == 0)
        fnamec += NAME_GRPH_FILE;
    if (pr == 1)
        fnamec += NAME_GRPH_FILE_EN;
    if (pr == 2)
        fnamec += NAME_GRPH_FILE_OS;
    if (pr == 3)
        fnamec += NAME_GRPH_FILE_DM;
    if (pr == 4)
        fnamec += NAME_GRPH_FILE_BAR;
    if (pr == 5)
        fnamec += NAME_GRPH_FILE_VLG;
#ifdef WIN_D
    fnamec.replace("\\","\\\\");
#endif
    fprintf(fl_com, "%s", QString("set output \"" + fnamec + "\"").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fprintf(fl_com, "%s", QString("set encoding koi8r").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fprintf(fl_com, "%s", QString("set xdata time").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fprintf(fl_com, "%s", QString("set datafile separator \";\"").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fprintf(fl_com, "%s", QString("set timefmt \"%H:%M:%S\"").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fprintf(fl_com, "%s", QString("set format x \"%H:%M\"").toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    if (pr == 0)
        fnamec = "set ylabel \"(C)\"";
    if (pr == 1)
        fnamec = "set ylabel \"(V)\"";
    if (pr == 2)
        fnamec = "set ylabel \"(O)\"";
    if (pr == 3)
        fnamec = "set ylabel \"(D)\"";
    if (pr == 4)
        fnamec = "set ylabel \"(kPa)\"";
    if (pr == 5)
        fnamec = "set ylabel \"(vlg)\"";
    fprintf(fl_com, "%s", fnamec.toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    QString mm = "";
    for (int i = 0; i < 8; ++i)
        mm += QString().setNum(mac[i], 16);
    fnamec = "plot \"" + serverDir + "DATA" + mm + ".txt";
    if (pr == 0)
        fnamec += "\" using 2:6  with lines lw 4  title \"Temperatura ";
    if (pr == 1)
        fnamec += "\" using 2:7  with lines  lw 4 title \"Volt ";
    if (pr == 2)
        fnamec += "\" using 2:8  with lines  lw 4 title \"Osv ";
    if (pr == 3)
        fnamec += "\" using 2:9  with lines  lw 4 title \"Dim ";
    if (pr == 4)
        fnamec += "\" using 2:9  with lines  lw 4 title \"Bar ";
    if (pr == 5)
        fnamec += "\" using 2:9  with lines  lw 4 title \"Vlg ";
    fnamec += mm + "\"";
#ifdef WIN_D
    fnamec.replace("\\","\\\\");
#endif
    fprintf(fl_com, "%s", fnamec.toLocal8Bit().constData());
    fprintf(fl_com, "\n");

    fprintf(fl_com, "refresh\n");
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

void Vench::grph(int pr, QString fileName)
{
    int mk[8];
    int numu = listV->count();
    int j = 0;
    if (checkVibrUst->isChecked())
    {
        for(j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] = mas_mac_addr[j][i];
                 break;
            }
         if (j >= numu)
             return;
    }
    else
    {
        if (numu > 0)
            for(int i = 0; i < 8; ++i)
                mk[i] = mas_mac_addr[j][i];
        else
            return;
    }

    NewScriptPlot(mk, pr);

    QString ms;
#ifdef LINUX_D
    ms = "gnuplot \"" + serverDir + NAME_SCRIPT_FILE + "\"";
    qDebug() << ms;
    system(ms.toLocal8Bit().constData());
    ms = "eog " + serverDir + fileName + " &";
    qDebug() << ms;
    system(ms.toLocal8Bit().constData());
#endif
#ifdef  WIN_D
    if (progrPostrGRPH == "")
        ms = "C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe " + serverDir + NAME_SCRIPT_FILE;
    else
    {
        ms = progrPostrGRPH + " " + serverDir + NAME_SCRIPT_FILE;
        ms.replace("\\", "\\\\");
    }
    qDebug() << ms.toLocal8Bit();
    WinExec(ms.toLocal8Bit(), SW_RESTORE);
    if (progrShowGRPH == "")
        ms = "C:\\Windows\\System32\\mspaint.exe " + serverDir + fileName;
    else
    {
        ms = progrShowGRPH + " " + serverDir + fileName;
        ms.replace("\\", "\\\\");
    }
    qDebug() << ms.toLocal8Bit();
    WinExec(ms.toLocal8Bit(), SW_RESTORE);
#endif
}

void Vench::BPressGrph()
{
    if (!fl_grph)
    {
        pr_grph = 0;
        return;
    }

    if (pr_grph > 1)
    {
        fl_grphE = false;
        fl_grphVl = false;
        fl_grphO = false;
        fl_grphD = false;
        fl_grphB = false;
    }
    return;

    // vremenno !!! ubrat to chto nige!!!
    grph(0, NAME_GRPH_FILE);
}

void Vench::BPressGrphE()
{
    if (!fl_grphE)
    {
        pr_grph = 0;
        return;
    }

    if ((pr_grph != 2) && (pr_grph > 0))
    {
        fl_grph = false;
        fl_grphVl = false;
        fl_grphO = false;
        fl_grphD = false;
        fl_grphB = false;
    }
    return;

    // vremenno !!! ubrat to chto nige!!!
    grph(1, NAME_GRPH_FILE_EN);
}

void Vench::BPressGrphD()
{
    if (!fl_grphD)
    {
        pr_grph = 0;
        return;
    }

    if ((pr_grph != 4) && (pr_grph > 0))
    {
        fl_grph = false;
        fl_grphVl = false;
        fl_grphO = false;
        fl_grphE = false;
        fl_grphB = false;
    }
    return;

    // vremenno !!! ubrat to chto nige!!!
    grph(3, NAME_GRPH_FILE_DM);
}

void Vench::BPressGrphB()
{
    if (!fl_grphB)
    {
        pr_grph = 0;
        return;
    }

    if ((pr_grph != 5) && (pr_grph > 0))
    {
        fl_grph = false;
        fl_grphVl = false;
        fl_grphO = false;
        fl_grphD = false;
        fl_grphE = false;
    }
    return;

    // vremenno !!! ubrat to chto nige!!!
    grph(4, NAME_GRPH_FILE_BAR);
}

void Vench::BPressGrphV()
{
    if (!fl_grphVl)
    {
        pr_grph = 0;
        return;
    }

    if ((pr_grph != 6) && (pr_grph > 0))
    {
        fl_grph = false;
        fl_grphO = false;
        fl_grphD = false;
        fl_grphE = false;
        fl_grphB = false;
    }
    return;

    // vremenno !!! ubrat to chto nige!!!
    grph(5, NAME_GRPH_FILE_VLG);
}

void Vench::BPressGrphO()
{
    if (!fl_grphO)
    {
        pr_grph = 0;
        return;
    }

    if ((pr_grph != 3) && (pr_grph > 0))
    {
        fl_grph = false;
        fl_grphE = false;
        fl_grphD = false;
        fl_grphB = false;
        fl_grphVl = false;
    }
    return;

    // vremenno !!! ubrat to chto nige!!!
    grph(2, NAME_GRPH_FILE_OS);
}

//ndt ==1 - print temp
//ndt ==2 - print energ
//ndt ==3 - print osv
//ndt ==4 - print dim
//ndt ==5 - print bar
//ndt ==6 - print vlag
void Vench::PaintTxtData(int ndt, double dt)
{
    QString sdt = QString().setNum(dt, 3, 2);
    if (ndt == 1)
        emit LTempChanged(sdt);
    if (ndt == 2)
        emit LEnChanged(sdt);
    if (ndt == 3)
        emit LOsvChanged(sdt);
    if (ndt == 4)
        emit LDimChanged(sdt);
    if (ndt == 5)
        emit LBarChanged(sdt);
    if (ndt == 6)
        emit LVlChanged(sdt);
}

QString Vench::getTextReg()
{
    QString text;
    text = tr("�����: ���� ��������� ������ 10 �. ");
    if (fl_temp)
    {
        text += tr("����������� � ������������ ");
        if (!fl_dim && !fl_energ)
            text += "\n ";
    }
    if (fl_energ)
    {
        if (fl_temp)
            text += "\n ";
        text += tr("������� ");
        if (!fl_dim)
            text += "\n ";
    }
    if (fl_dim)
    {
        if (!fl_energ && fl_temp)
            text += "\n ";
        text += tr("������������� \n");
    }
    if(!fl_temp && !fl_energ && !fl_dim)
        text = tr("�����:  \n");
    return text;
}

void Vench::BPressDim()
{
    if (fl_dim)
    {
        SendCommDimOn();
        dimPress = 1;
    }
    else
    {
        SendCommDimOff();
        dimPress = 0;
    }
    emit RegGrphTextChanged(getTextReg());
}

void Vench::BPressEnerg()
{
    if (fl_energ)
    {
        SendCommEnergOn();
        energPress = 1;
    }
    else
    {
        SendCommEnergOff();
        energPress = 0;
    }
    emit RegGrphTextChanged(getTextReg());
}

void Vench::BPressTemp()
{
    if (fl_temp)
    {
        SendCommTempOn();
        flzaprostemposv = 1;
        tempPress = 1;
    }
    if (!fl_temp)
    {
        SendCommTempOff();
        flzaprostemposv = 0;
        tempPress = 0;
    }
    qDebug("flzap = %d\n", flzaprostemposv);
    emit RegGrphTextChanged(getTextReg());
}

void Vench::BPressBar()
{
    if (fl_bar)
    {
        SendCommBarOn();
        flzaprosbar = 1;
    }
    else
        flzaprosbar = 0;
    emit RegGrphTextChanged(getTextReg());
}


int Vench::SetAnswerUstr(int imp1, int imp2,int nD)
{
    if (fl_rec && !fl_bluetooth)
    {
        if (num_comm < NUM_COMM)
        {
            if (!fl_diskr)
            {
                mass_comm_n[num_comm][0] = imp1;
                mass_comm_n[num_comm][1] = imp2;
                if (nD!=0)
                {
                    mass_comm_n[num_comm][2] = nD;
                    ++num_comm;
                }
            }
            else
            {
                if (nD == 0)
                {
                    if ((imp1 - rimp1) > 0)
                        mass_comm_n[num_comm][0] = imp1 - rimp1;
                    else
                        mass_comm_n[num_comm][0] = rimp1 - imp1;
                    if ((imp2 - rimp2) > 0)
                        mass_comm_n[num_comm][1] = imp2 - rimp2;
                    else
                        mass_comm_n[num_comm][1] = rimp2 - imp2;
                    mass_comm_n[num_comm][2] = ret_napr;
                    if ((imp1 != rimp1) && (imp2 != rimp2))
                        ++num_comm;
                    rimp1 = imp1;
                    rimp2 = imp2;
                }
                else
                {
                    ret_napr = nD;
                    rimp1 = imp1;
                    rimp2 = imp2;
                }
            }
        }
        else
        {
            printf("ERROR chislo comand ogran!!!\n");
            return 0;
        }
    }
    return 1;
}

void Vench::SendCommDimOff()
{
    unsigned char mkc[10];
    if (SpActDatch->count() == 0)
    {
        qDebug("Net active ustr\n");
        return;
    }

    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mkc[i] = mas_mac_addr[j][i];
                act_ust[GetNumberUst(mkc)].period_oprosa_d = 10.0;
                SendZaprosOtklDim(mkc);
            }
        return;
    }
    if (!fl_allDevices)
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mkc[i] = mas_mac_addr[numu][i];
        act_ust[GetNumberUst(mkc)].period_oprosa_o = 10.0;
    }

    SendZaprosOtklDim(mkc);
}

void Vench::SendCommDimOn()
{
    unsigned char mkc[10];
    if (SpActDatch->count() == 0)
    {
        qDebug("Net active ustr\n");
        return;
    }

    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mkc[i] = mas_mac_addr[j][i];
                act_ust[GetNumberUst(mkc)].period_oprosa_d = 10.0;
                SendZaprosDim(mkc);
            }
        return;
    }
    if (fl_allDevices)
    {
        for(int i = 0; i < 6; ++i)
            mkc[i] = 0;
        mkc[6]=0xff;
        mkc[7]=0xff;
        mkc[8]=0xff;
        mkc[9]=0xfe;
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mkc[i] = mas_mac_addr[numu][i];
        act_ust[GetNumberUst(mkc)].period_oprosa_o = 10.0;
    }
    SendZaprosDim(mkc);
}

QString Vench::BaseOfCommForw1(QString cm, int mk[10])
{
    for (int i = 0; i < 8; ++i)
        cm = cm + QString().setNum(mk[i], 16) + " "; // qWarning : убедиться что работает
#ifndef OLDD
    cm = cm + "FF FE 0 0 ";
#endif
#ifdef OLDD
    cm = cm + "FF FE 0 ";
#endif
     cm = cm + QString().setNum(speed_car, 16) + " ";

     if (!fl_diskr)
         cm = cm + "0 0 A";
     else
     {
         if (fl_allDevices)
             cm = cm + QString().setNum(imp_diskr, 10) + " " + QString().setNum(imp_diskr, 10) + " A";
         else
         {
             int impr, impl;
             GetKolImpUstr(mk, &impr, &impl);
             cm = cm + QString().setNum(impr, 16) + " " + QString().setNum(impl, 16) + " A";
             if (fl_rec == 1)
                 SetAnswerUstr(impr, impl, 0xa);
         }
     }
}

void Vench::SendCommForw1()
{
    int mk[10];
    if (SpActDatch->count() == 0)
    {
        qDebug("Net active ustr\n");
        return;//если список пуст
    }

    QString cm = "* 7E 0 12 10 11 ";
#ifdef OLDD
    cm = "* 7E 0 11 10 11 ";
#endif

    if (!fl_allDevices && checkVibrUst->isChecked())
    {
        int numu = listV->count();
        for(int j = 0; j < numu; ++j)
        {
            if (mas_vibr_ust[j] == 1)
            {
                for(int i = 0; i < 8; ++i)
                    mk[i] =mas_mac_addr[j][i];
                for(int i = 0; i < 2; ++i)
                    mk[i+8] =mas_set_addr[j][i];

                cm = BaseOfCommForw1(cm, mk);
                SendComm(cm);
            }
        }
        return;
    }

    if (fl_allDevices)
    {
        for(int i = 0; i < 10; ++i)
            mk[i] = 0;
        mk[6]=0xff;
        mk[7]=0xff;
        mk[8]=0xff;
        mk[9]=0xfe;
    }
    else
    {
        int numu = SpActDatch->currentIndex();
        for(int i = 0; i < 8; ++i)
            mk[i] = mas_mac_addr[numu][i];
        for(int i = 0; i < 2; ++i)
            mk[i+8] = mas_set_addr[numu][i];
    }
    cm = BaseOfCommForw1(cm, mk);
    SendComm(cm);
}


void Vench::SendCommBudil()
{
   int mk[10];
   qDebug("SendCommBudil\n");
   int  numu = SpActDatch->count();
   if (numu == 0)
   {
       qDebug("Net active ustr\n");
       return;
   }

   if (!fl_allDevices && checkVibrUst->isChecked())
   {
       numu = listV->count();
       for(int j = 0; j < numu; ++j)
           if (mas_vibr_ust[j] == 1)
           {
               for(int i = 0; i < 8; ++i)
                   mk[i] = mas_mac_addr[j][i];
               for(int i = 0; i < 2; ++i)
                   mk[i+8] = mas_set_addr[j][i];
               FormatAndSendComm("* 7E 0 10 17 11 ", mk, "2 44 34 4 ");
               mSleep(50);
               FormatAndSendComm("* 7E 0 10 17 11 ", mk, "2 44 34 5 ");
           }
       return;
   }

   if (fl_allDevices)
   {
       for(int i = 0; i < 6; ++i)
           mk[i] = 0;
       mk[6]=0xff;
       mk[7]=0xff;
       mk[8]=0xff;
       mk[9]=0xfe;
       FormatAndSendComm("* 7E 0 10 17 11 ", mk, "2 44 34 4 ");
       mSleep(100);
       FormatAndSendComm("* 7E 0 10 17 11 ", mk, "2 44 34 5 ");
   }
   else
   {
       numu = SpActDatch->currentIndex();
       for(int i = 0; i < 8; ++i)
           mk[i] = mas_mac_addr[numu][i];
       for(int i = 0; i < 2; ++i)
           mk[i+8] = mas_set_addr[numu][i];
       FormatAndSendComm("* 7E 0 10 17 11 ", mk, "2 44 34 4 ");
       mSleep(10);
       FormatAndSendComm("* 7E 0 10 17 11 ", mk, "2 44 34 5 ");
   }
}


void Vench::SendCommStop1()
{
    int mk[10];
    int  numu;
    int i,j;
   numu=0;

    //numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
    return;//если список пуст
    }
    if (!fl_allDevices &&(checkVibrUst->isChecked() ==  true)){

        numu = listV->count();

    for(j=0;j<numu;j++){
        if (mas_vibr_ust[j]==1){



         for(i=0;i<8;i++){
         mk[i] =mas_mac_addr[j][i];
         }
         for(i=0;i<2;i++){
         mk[i+8] =mas_set_addr[j][i];
         }

#ifndef OLDD
FormatAndSendComm("* 7E 0 12 10 11 ", mk,  "FF FE 0 0 0 0 0 0");
#endif
#ifdef OLDD
FormatAndSendComm("* 7E 0 11 10 11 ", mk,  "FF FE 0 0 0 0 0");
#endif
        }
    }
    return;
    }


     if (fl_allDevices) {
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


#ifndef OLDD
FormatAndSendComm("* 7E 0 12 10 11 ", mk,  "FF FE 0 0 0 0 0 0");
#endif
#ifdef OLDD
FormatAndSendComm("* 7E 0 11 10 11 ", mk,  "FF FE 0 0 0 0 0");
#endif

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


#ifndef OLDD
FormatAndSendComm("* 7E 0 12 10 11 ", mk,  "FF FE 0 0 0 0 0 0");
#endif
#ifdef OLDD
FormatAndSendComm("* 7E 0 11 10 11 ", mk,  "FF FE 0 0 0 0 0");
#endif
     }
}


void Vench::SendCommLeft1()
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

    if (!fl_allDevices &&(checkVibrUst->isChecked() ==  true)){

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
        if (!fl_diskr) {
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


    if (fl_allDevices) {
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

        if (!fl_diskr) {
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

        if (!fl_diskr) {
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

    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
    return;//если список пуст
    }

    if (!fl_allDevices &&(checkVibrUst->isChecked() ==  true)){

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

        if (!fl_diskr) {
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


    if (fl_allDevices) {
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


        if (!fl_diskr) {
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

        if (!fl_diskr) {
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

    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
    return;//если список пуст
    }

    if (!fl_allDevices &&(checkVibrUst->isChecked() ==  true)){

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

        if (!fl_diskr) {
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


    if (fl_allDevices) {
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

        if (!fl_diskr) {
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
        if (!fl_diskr) {
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

    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
    return;//если список пуст
    }

    if (!fl_allDevices &&(checkVibrUst->isChecked() ==  true)){

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
        if (!fl_diskr) {
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


    if (fl_allDevices) {
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
        if (!fl_diskr) {
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

        if (!fl_diskr) {
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

    numu = SpActDatch->count();
    if (numu==0) {
    qDebug("Net active ustr\n");
    return;//если список пуст
    }

    if (!fl_allDevices &&(checkVibrUst->isChecked() ==  true)){

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

        if (!fl_diskr) {
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


    if (fl_allDevices) {
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

        if (!fl_diskr) {
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

        if (!fl_diskr) {
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

void Vench::WriteProg1( int k )
{
    QDomDocument md;
    QDomElement root = md.createElement("bim_xml");
    QDomNode node = md.createElement("prog");
    for(int i = 0; i < k; ++i)
    {
        QDomElement node1 = md.createElement("pole");
        node1.setAttribute("imp1", QString().setNum(mass_comm_n[i][0], 10));
        node1.setAttribute("imp2", QString().setNum(mass_comm_n[i][1], 10));
        node1.setAttribute("napr", QString().setNum(mass_comm_n[i][2], 16));
        node.appendChild(node1);
    }
    root.appendChild(node);

    QDomDocument doc("MyML");
    doc.appendChild(root);
    QString fnamec = serverDir;
#ifdef LINUX_D
    fnamec += f_name_xml;
#endif
#ifdef WIN_D
    fnamec = f_name_xml;
#endif
    FILE *fff = fopen(fnamec.toLocal8Bit().constData(), "w+");
    QTextStream out(fff, QIODevice::WriteOnly);
    doc.save(out, 4);
    fclose(fff);
}

void Vench::ReadProg1()
{
    QString fnamec = serverDir;
#ifdef LINUX_D
    fnamec += f_name_xml;
#endif
#ifdef WIN_D
    fnamec = f_name_xml;
#endif

    QFile f(fnamec);
    QDomDocument doc;
    if (!doc.setContent(&f,  true))
    {
        WriteProg1(0);
        qDebug("Error setContent\n");
        if (!doc.setContent(&f,  true))
        {
            qDebug("Error setContent\n");
            return;
        }
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "bim_xml")
    {
        printf("Error format file! \n");
        return;
    }
    QDomNode node = root.firstChild();
    if (node.toElement().tagName() == "prog")
    {
        QDomNode node1 = node.firstChild();
        int i = 0;
        while(!node1.isNull())
        {
            if (node1.toElement().tagName() == "pole")
            {
                mass_comm_n[i][0] = node1.toElement().attribute("imp1").toInt();
                mass_comm_n[i][1] = node1.toElement().attribute("imp2").toInt();
                bool ok;
                mass_comm_n[i][2] = node1.toElement().attribute("napr").toInt(&ok, 16);
                ++i;
                if (i >= NUM_COMM)
                    break;
            }
            node1 = node1.nextSibling();
        }
        num_comm = i;
    }
}

bool Vench::readIniLine(QTextStream& tt, QString& t, QString debug_message)
{
    t = tt.readLine();
    int i = 0;
    while ((t == "") || (t.indexOf("#") == 0))
    {
        t = tt.readLine();
        ++i;
        if (i > NUM_STROK)
        {
#ifdef PRINT_DEBUG
            printf("Error read ini-file\n");
#endif
            return false;
        }
        mSleep(5);
    }
#ifdef PRINT_DEBUG
    qDebug() << debug_message;
    qDebug() << t;
#endif
    return true;
}

void Vench::ReadIni()
{
    QDir dr;
    QString flname;
    qDebug("readini\n");

    if (fl_kat == 0)
    {
#ifdef LINUX_D
        flname = dr.homePath() + "/.config/smartcar/" + NAME_FILE_INI;
        QFile mfl(flname);
        if (!mfl.exists())
            flname = QString("/etc/smartcar/").toLocal8Bit() + NAME_FILE_INI;
#endif
#ifdef WIN_D
        flname = NAME_FILE_INI;
#endif
    }
    else
        flname = katalog + NAME_FILE_INI;
#ifdef PRINT_DEBUG
    qDebug() << flname;
    qDebug("%s\n", katalog);
#endif


    QFile fl_com;
    fl_com.setFileName(flname);
    if (!fl_com.open(QIODevice::ReadOnly | QIODevice::Text ))
    {
#ifdef PRINT_DEBUG
        qDebug() << "Error open smart.ini!\n";
#endif
        return;
    }

    QTextStream tt( &fl_com);
    QString t;
    if (!readIniLine(tt, t, "Dir"))
            return;
    serverDir = t;
    emit ServerDirCganged();
#ifndef LINUX_D
    int num_sl = t.length();
    QString mtt = t.mid(num_sl-1,1);
    serverDir = t;
    emit serverDirCganged();
#endif
#ifdef LINUX_D
    if (fl_kat == 0)
    {
        serverDir = dr.homePath() + "/.config/smartcar/";
        emit ServerDirCganged();
#ifdef PRINT_DEBUG
        qDebug(" ");
        qDebug("WARNING: home dir = ");
        qDebug() << serverDir;
        qDebug(" ");
#endif
    }
#endif
    dr.mkpath(serverDir);

    if (!readIniLine(tt, t, "file name kumirr"))
        return;
    f_name_kum = t;

    if (!readIniLine(tt, t))
        return;
    f_name_xml = t;

    if (!readIniLine(tt, t))
        return;
    f_name_json = t;

    if (!readIniLine(tt, t))
        return;
    f_name_snap = t;

    if (!readIniLine(tt, t))
        return;
    bool ok;
    n_imp_kum = t.toInt(&ok, 16);
    def_diskr_impr = t.toInt(&ok, 16);

    if (!readIniLine(tt, t))
        return;
    n_imp_kum_p = t.toInt(&ok,16);
    def_diskr_impl = t.toInt(&ok,16);

    if (!readIniLine(tt, t, "Adress\n"))
        return;
    if(!fl_port_s)
        addr = t;

    if (!readIniLine(tt, t, "usleep\n"))
        return;
    tm_usleep_com = t.toInt();

    if (!readIniLine(tt, t, "imp discr"))
        return;
    imp_diskr = t.toInt();

    if (!readIniLine(tt, t))
        return;
    addr_win = t;
    #ifdef WIN_D
    if (!fl_port_s)
        addr = t;
    #endif

    if (!readIniLine(tt, t, "fomat prog:"))
        return;
    if (fl_bluetooth)
        fl_begin = 4;
    else
    {
        if (t == "START")
            fl_begin = 1;
        else if (t == "SUPER")
            fl_begin = 3;
        else if (t == "SUPER_N")
            fl_begin = 0;
        else if (t == "BASE")
            fl_begin = 2;
    }

    if (!readIniLine(tt, t, "read win\n"))
        return;
    prog_for_read_win = t; //programm dla chtenia file pod wind

    if (!readIniLine(tt, t, "read postr grph"))
        return;
    progrPostrGRPH = t; //programm dla postroenia graph file pod wind

    if (!readIniLine(tt, t, "read show grph"))
        return;
    progrShowGRPH = t; //programm dla postroenia graph file pod wind

    if (!readIniLine(tt, t, "QQQ"))
        return;
    speed_car = t.toInt(); //скорость движения машинки?

    fl_com.close();
}



void Vench::chLog(bool checked)
{
    fl_print_log = checked;
    if (checked)  SendComm("log");
    if (!checked) SendComm("ulog");
}

#include <qtextcodec.h>

void Vench::ReadCum()
{

 qDebug("read cum\n");
    QString fnamec;

    fnamec= serverDir;
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
    QString t;
#ifdef LINUX_D
       QString bc= "нц";
       QString ecc= "кц";
       QString ec= "раз";
       QString vpravo= "вправо";
       QString vlevo= "влево";
       QString vpered= "вверх";
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

    if (fll!= true){
        qDebug("Error open file kumir\n");
        qDebug("Create file kumir\n");
    print_nastr_file(2);

    }

    QTextStream tt( &fl_com);
    int i=0;
    int fl=0;
    tt.setCodec(QTextCodec::codecForName("UTF-16LE"));
    while(!tt.atEnd()){//for qt4
       t = tt.readLine();
       fl =0;

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

   }
    num_comm=i;
}



void Vench::chKum()
{
    if (!fl_xml)
        ReadCum();
}



///DOBAvLENIE  function for rabota s portom



int Vench::printKG(int flf)
{
    FILE * mmflnd = NULL;
    if (flf == 0)
        mmflnd = fopen("DAT1.txt", "a+") ;
    if (flf == 1)
        mmflnd = fopen("DAT2.txt", "a+") ;

    sprintf(sys_com, "chmod 666 %s", DATAFILE);

#ifdef LINUX_D
    system(sys_com);
#endif

    for(int mid=0; mid<10; ++mid)
            fprintf(flnd,";");
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
    if (com_port_qts.open(QIODevice::ReadWrite))
    {
        com_port_qts.setBaudRate(QSerialPort::Baud9600);
        com_port_qts.setDataBits(QSerialPort::Data8);
        com_port_qts.setFlowControl(QSerialPort::NoFlowControl);
        com_port_qts.setStopBits(QSerialPort::TwoStop);
        flcomport = 1 ;

    }
#endif

#ifndef QSERIAL
#ifdef LINUX_D
    if((com_port = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK,0)) < 0)
    {
        printf("ERROR Cannot open %s\n", port);
        flcomport = 0;
        return -1;
    }
    else // Set Parametr
    {
        flcomport = 1;
        tcgetattr(com_port, &options);
        cfsetispeed(&options, B9600); // skorost v baitax
        options.c_cflag |= (CLOCAL | CREAD);
        // PARODD - proverka na nechetnost
        options.c_cflag &= ~PARENB; // bez kontrola chetnosti
        options.c_cflag &= ~CSTOPB; // 1 stop bit
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cflag &= ~CRTSCTS;
        options.c_lflag &= ~(ICANON | ECHO | ISIG);
        options.c_cc[VTIME] = 0;
        options.c_cc[VMIN] = 1;
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
    return 1;
#endif

#ifdef WIN_D
    char size = 8;
    char * portname = new char[strlen(port) + 1];
    strcpy(portname, port);
    hCOM = CreateFileA(portname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (portname)
        delete [] portname;
    if (hCOM != INVALID_HANDLE_VALUE)
    {
        qDebug() << "Open com port\n";
        overlapped.hEvent = CreateEvent(NULL,  true,  true, NULL);
        SetCommMask(hCOM, EV_RXCHAR);
    }
    else
    {
        qDebug() << "Error open com port\n";
        return 0;
    }
    dcb.DCBlength = sizeof(dcb);
    if(!GetCommState(hCOM, &dcb))
        return 0;
    else
    {
        dcb.BaudRate = 9600;
        dcb.ByteSize = size;
        dcb.Parity = NOPARITY;
        dcb.StopBits = TWOSTOPBITS;
        if(!SetCommState(hCOM, &dcb))
            return 0;
    }
    if(!GetCommTimeouts (hCOM, &rCommTimeouts))
        return -1;
    else
    {
        rCommTimeouts.ReadTotalTimeoutConstant = 1000;
        rCommTimeouts.ReadTotalTimeoutMultiplier = 0;
        if (!SetCommTimeouts (hCOM, &rCommTimeouts))
            return -1;
    }
    flcomport = 1;
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

int Vench::Write(int port, unsigned char *buf, int len)
{
    if (fl_print_log_B)
        print_log(buf, len, 1);

#ifdef QSERIAL
    int r = port;
    if (!fl_print_log_B)
        print_log(buf, len, 1);
    r = com_port_qts.write((char *)buf, len);
    if (r < 0)
    {
        COMClose();
        fl_ans3_ust = 1;
    }
    return r;
#endif

#ifndef QSERIAL

#ifdef  LINUX_D
    ssize_t ret;
    if (!fl_print_log_B)
        print_log(buf, len, 1);
    ret = write(port, buf, len);
    int r = (int)ret;
    if (ret<0)
    {
        COMClose();
        fl_ans3_ust = 1;
    }
    printf("Write port = %d\n", r);
    return r;
#endif

#ifdef WIN_D
    if (!fl_print_log_B)
        print_log(buf, len, 1);
    int  ret = 0;
    unsigned long hh;
    DWORD signal; //temp - ����������-��������
    overlappedwr.hEvent = CreateEvent(NULL,  true,  true, NULL); //������� �������
    if(!WriteFile (hCOM,      // ����� �����
                   buf,              // ��������� �� ������������ ������
                   len,       // ������ ������������� �������
                   &hh,        // ��������� �� ���������� ������������ ����
                   &overlappedwr))// pointer to structure for overlapped I/O);
        return -1;
    signal = WaitForSingleObject(overlappedwr.hEvent, INFINITE); //������������� �����, ���� �� ���������� ������������� �������� WriteFile
    ret = hh;

    qDebug("Write: ret =%d len = %d\n", ret, len);

    if (ret != len)
    {
        if (ret < 0)
        {
            COMClose();
            fl_ans3_ust = 1;
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


int Vench::print_spec_file(QString liter, unsigned char *buf_mac_set,double data)
{
    FILE * mflnd;
    char nf[50];
    sprintf(nf,
            "%x%x%x%x%x%x%x%x" + liter.toLocal8Bit() + ".txt",
            buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);
#ifdef LINUX_D
        QString flname = QDir().homePath() + "/.config/smartcar/" + nf;
        mflnd = fopen(flname.toLocal8Bit().constData(), "w+") ;
#endif
#ifdef WIN_D
        mflnd = fopen(nf, "w+") ;
#endif


#ifdef LINUX_D
    sprintf(sys_com, "chmod 666 %s", flname.toLocal8Bit().constData());
    system(sys_com);
#endif
    time(&ltime);

    fprintf(mflnd, "%d %f", ltime, data);
    fclose(mflnd);
    return 1;
}

int Vench::print_nastr_file(int pr)
{
    FILE * mflnd;
    char nf[50];
    if (pr == 0)
        sprintf(nf, "%s", "tmp_smart.ini");
    if (pr==1)
        sprintf(nf, "%s", "smart.ini");
    if (pr == 2)
        sprintf(nf, "%s", f_name_kum.toLocal8Bit().constData());

#ifdef LINUX_D
    QString flname = QDir().homePath() + "/.config/smartcar/" + nf;
    mflnd = fopen(flname.toLocal8Bit().constData(), "w+");
#endif
#ifdef WIN_D
    mflnd = fopen(nf, "w+");
#endif

#ifdef LINUX_D
    sprintf(sys_com, "chmod 666 %s", flname.toLocal8Bit().constData());
    system(sys_com);
#endif
    if (pr==2)
    {
        fclose(mflnd);
        return 1;
    }
    fprintf(mflnd, "# HOME directory\n");

    fprintf(mflnd, "%s", serverDir.toLocal8Bit().constData());
    fprintf(mflnd, "\n");

    fprintf(mflnd, "# program KUMIR\n");

    fprintf(mflnd, "%s", f_name_kum.toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# program XML\n");

    fprintf(mflnd, "%s", f_name_xml.toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# kol-vo impulsov\n");

    fprintf(mflnd, "%s", QString().setNum(def_diskr_impr, 16).toLocal8Bit().constData());
    fprintf(mflnd, "\n");

    fprintf(mflnd, "%s", QString().setNum(def_diskr_impl,16).toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# port for Linux\n");

    fprintf(mflnd, "%s", addr.toLocal8Bit().constData());
    fprintf(mflnd, "\n");

    fprintf(mflnd, "%s", QString().setNum(tm_usleep_com,10).toLocal8Bit().constData());
    fprintf(mflnd, "\n");

    fprintf(mflnd, "%s", QString().setNum(imp_diskr,10).toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# port for windows\n");

    fprintf(mflnd, "%s", addr_win.toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# status : START | BASE | SUPER\n");

    if (fl_begin == 3)
        fprintf(mflnd, "%s", QString("SUPER_N").toLocal8Bit().constData());
    if (fl_begin == 0)
        fprintf(mflnd, "%s", QString("SUPER").toLocal8Bit().constData());
    if (fl_begin == 1)
        fprintf(mflnd, "%s", QString("START").toLocal8Bit().constData());
    if (fl_begin == 2)
        fprintf(mflnd, "%s", QString("BASE").toLocal8Bit().constData());

    fprintf(mflnd, "\n");
    fprintf(mflnd, "# program for read file win\n");

    fprintf(mflnd, "%s", prog_for_read_win.toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# program for postroenie graphikov pod win\n");

    fprintf(mflnd, "%s", progrPostrGRPH.toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# program for prosmotra graphikov pod win\n");

    fprintf(mflnd, "%s", progrShowGRPH.toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fprintf(mflnd, "# speed car\n");

    fprintf(mflnd, "%s", QString().setNum(speed_car,10).toLocal8Bit().constData());
    fprintf(mflnd, "\n");
    fclose(mflnd);
    return 1;
}

int Vench::print_data(unsigned char *buf_mac_set,double data, int pr)
{
    QString flname = DATAFILE;
#ifdef WIN_D
    flnd = fopen(DATAFILE, "a+") ;
#endif
#ifdef LINUX_D
    flname = QDir().homePath() + "/.config/smartcar/" + DATAFILE;
    flnd = fopen(flname.toLocal8Bit().constData(), "a+");
    sprintf(sys_com, "chmod 666 %s", flname.toLocal8Bit().constData());
    system(sys_com);
#endif
    print_time(flnd);

    for(id = 0; id < 10; ++id)
    {
        fprintf(flnd," %X", buf_mac_set[id]);
        if (id==7)
            fprintf(flnd, ";");
    }
    fprintf(flnd, ";");
    if (pr > 0 && pr < 6)
        fprintf(flnd, "%s", QString(" ;").repeated(pr).toLocal8Bit().constData());
    fprintf(flnd, " %0.2f", data);
    fprintf(flnd, ";");
    if (pr==0)
        fprintf(flnd, " t\n");
    if (pr==1)
        fprintf(flnd, " e\n");
    if (pr==2)
        fprintf(flnd, " o\n");
    if (pr==3)
        fprintf(flnd, " d\n");
    if (pr==4)
        fprintf(flnd, " b\n");
    if (pr==5)
        fprintf(flnd, " v\n");

    fflush(flnd);
    fclose(flnd);
    return 1;
}


int Vench::print_data_s(unsigned char *buf_mac_set,double data, int pr)
{
    FILE * mflnd;

    int cel, drob;
    char nf[50];
    sprintf(nf, "DATA%x%x%x%x%x%x%x%x.txt", buf_mac_set[0],buf_mac_set[1],buf_mac_set[2],buf_mac_set[3],buf_mac_set[4],buf_mac_set[5],buf_mac_set[6],buf_mac_set[7]);
    QString flname = nf;
#ifdef LINUX_D
    flname = QDir().homePath() + "/.config/smartcar/" + nf;
    mflnd = fopen(flname.toLocal8Bit().constData(), "a+");
    sprintf(sys_com, "chmod 666 %s", flname.toLocal8Bit().constData());
    cel = system(sys_com);
#endif
#ifdef WIN_D
    mflnd = fopen(nf, "a+") ;
#endif

    print_time(mflnd);
    print_time_s(mflnd);
    for(id = 0; id < 10; ++id)
    {
        fprintf(mflnd, " %X", buf_mac_set[id]);
        if (id==7)
            fprintf(mflnd, ";");
    }
    fprintf(mflnd, ";");
    if (pr > 0 && pr < 6)
        fprintf(mflnd, "%s", QString(" ;").repeated(pr).toLocal8Bit().constData());

    cel = (int)data;
    drob = (int)((data - cel) * 100.0);
    fprintf(mflnd, " %d.%d", cel, drob);
    fprintf(mflnd, ";");

    if (pr==0)
        fprintf(mflnd, " t\n");
    if (pr==1)
        fprintf(mflnd," e\n");
    if (pr==2)
        fprintf(mflnd," o\n");
    if (pr==3)
        fprintf(mflnd," d\n");
    if (pr==4)
        fprintf(mflnd," b\n");
    if (pr==5)
        fprintf(mflnd," vl\n");
    fflush(mflnd);
    fclose(mflnd);
    return 1;
}


int Vench::print_log(unsigned char *buf,int len,int pr)
{
//#ifdef PRINT_LOG
    if (!fl_print_log)
        return 0;
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


int Vench::print_time(FILE *fln) //pechataem vremia v poniatnom formate
{
    time(&ltime);
    newtime = localtime(&ltime); // podgotavlivaem vremia
    Y = newtime->tm_year + 1900;
    M = newtime->tm_mon + 1;
    D = newtime->tm_mday;
    h = newtime->tm_hour;
    m = newtime->tm_min;
    s = newtime->tm_sec;
    fprintf(fln, "%d:%.2d:%.2d; %.2d:%.2d:%.2d; ", Y, M, D, h, m, s);
    return 1;
}

int Vench::print_time_s(FILE *fln) //pechataem vremia v poniatnom formate
{
    time(&ltime);
    int kt = ltime;
    fprintf(fln, " %d; ", kt);
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
    DWORD temp, mask, signal; //���������� temp � ������� ������� �����
            {������������ � �������� ��������

                WaitCommEvent( hCOM, &mask, &overlapped); //������� ������� ������ ����� (��� � ���� ������������� ��������)
                signal = WaitForSingleObject(overlapped.hEvent, 500); //������������� ����� �� ������� �����
                if(signal == WAIT_OBJECT_0) //���� ������� ������� ����� ���������
                {
                        if(GetOverlappedResult( hCOM, &overlapped, &temp,  true)) //���������, ������� �� ����������� ������������� �������� WaitCommEvent
                        if((mask & EV_RXCHAR)!=0) //���� ��������� �����
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
            l++;
        }
        if (l==3){// viceplaem dlinu kadra
            buf_d[0]=buf_s[2];
            buf_d[1]=buf_s[1];
            memcpy(&l_k,buf_d,2);
        l_k = l_k+4;

        }
        if ((l>=l_k)&&(l>3)){
        fl_k=1;
        break;
        }
    }

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

int Vench::SetParentUst()
{
    int max_num = 0;
    unsigned char pu[2], ppu[2];
    for(int i = 0; i < mkol_ust; ++i)
    {
        printf(" %d pr[0] = %d pr[1] = %d\n", i, act_ust[i].parent_addr[0], act_ust[i].parent_addr[1]);
        if ((act_ust[i].parent_addr[0] == 0xff) && (act_ust[i].parent_addr[1] == 0xfe))
            act_ust[i].num_u = 0;
        else
        {
            pu[0] = act_ust[i].parent_addr[0];
            pu[1] = act_ust[i].parent_addr[1];
            int fl_c = 1;
            while (fl_c == 1)
            {
                fl_c = RetParUst(pu, ppu);
                printf("flc= %d\n", fl_c);
                pu[0] = ppu[0];
                pu[1] = ppu[1];
                if (fl_c > 0)
                    act_ust[i].num_u++;
                printf("act_ust[%d].num_u= %d\n", i, act_ust[i].num_u);
            }
        }
        if (act_ust[i].num_u > max_num)
            max_num = act_ust[i].num_u;
    }

    for(int i = 0; i < mkol_ust; ++i)
        printf("Ust %d  sloy = %d\n", i, act_ust[i].num_u);
    return max_num;
}

int Vench::NewActUst(){
    int i,j;
    QByteArray rr;
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

    if (fl_logm==NULL){
        printf("ERROR : open file ADD_LOG\n");
    return 0;
    }
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

        ret =0;
        }



    if (buf[3] == 0x95){
        if (pr==1){
        ret =-2;
        }
    }


    return ret;
}


int Vench::GetPANID(unsigned char *buf,int len, unsigned char *pan1,unsigned char *pan2)
{
    if (buf[3] != 0x97)
        return -1;

    if ((buf[15] != 0x49) || (buf[16] != 0x44))
        return -1; // esli ne temperature

    if (buf[2] != 0x17)
    {
        qDebug("GetPanID : Get panid kontroler in Kodogramm \n");
        PanIdKontr = buf[17];
        qDebug("GetPanID : pn = %x\n", PanIdKontr);
        return -2;
    }
    *pan1 = buf[25];
    *pan2 = 0;

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
    /*if (len == 0)
        return 0;*/
    if (buf[3] == 0x90)
        return 1;
    if (buf[3] == 0x95)
        return 1;
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
    if (buf[15]!=0xA5) return -1; // esli ne

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
    /*if (len == 0)
        return 0;*/
    if (buf[3] != 0x97)
        return -1;
    if ((buf[15]==0x49) && (buf[16]==0x53))
        return 1; // esli ne temperature
    else return -1;
    return 1;
}

int Vench::TryDatKas(unsigned char *buf, int len, Dat &dt)
{
    /*if (len == 0)
        return 0;*/
    dt = Dat::None;
    if (buf[3] != 0x90)
        return -1;
    if (buf[2] != 0x11)
        return -1; //opredelaem po dline ??? - nepravilno!!!

    if (buf[19] == 0x4A)
        dt = Dat::Right;
    else if (buf[19] == 0x8A)
        dt = Dat::Left;
    else
        return -1;

    return 1;
}

int Vench::TryGetPANID(unsigned char *buf,int len)
{
    /*if (len == 0)
        return 0;*/
    if (buf[3] != 0x97)
        return -1;
    if ((buf[15]==0x49) && (buf[16]==0x44))
        return 1;
    return -1;
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


int Vench::SendZaprosI() //shirokoveshat zapros temp
{
    unsigned char buf_d[20];
    if (flcomport != 1)
        return 0;
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
    unsigned char ktr =0;
    for(int i = 3; i < 7; ++i)
        ktr += buf_d[i];
    buf_d[7] = 0xFF-ktr;
    buf_d[2] = 4;
    qDebug("Send Zapros I\n");
    return Write(com_port, buf_d, 8);
}

int Vench::ReadUstrINI(unsigned char *Mac){
    int  k = GetNumberUst(Mac);


    QString fname;
    fname="";
    fname =  serverDir;

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
    fname =  serverDir;
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

int Vench::SendVklAPI(int api_e) //api_e==1 - vkl api bez esc? api_e==2 - vkl api s esc? api+e==0 - otkl esc
{
    unsigned char buf_d[20];
    unsigned char ktr;
    if (flcomport != 1)
        return 0;
    //nachalo kadra
    buf_d[0] = 0x7E;
    //dlina
    buf_d[1] = 0;
    buf_d[2] = 0;
    //komanda
    buf_d[3] = 0x8;
    //nomer freima
    buf_d[4] = 1;
    //mak-adres,set-adres
    buf_d[5] = 0x41;
    buf_d[6] = 0x50;
    buf_d[7] = api_e;
    buf_d[8] = 0;
    ktr = 0;
    for(int i = 3; i < 8; ++i)
        ktr += buf_d[i];
    buf_d[8] = 0xFF - ktr;
    buf_d[2] = 5;
    return Write(com_port, buf_d, 9);
}

int Vench::SendVklNP()
{
    unsigned char buf_d[20];
    unsigned char ktr;
    if (flcomport != 1)
        return 0;

    //nachalo kadra
    buf_d[0] = 0x7E;
    //dlina
    buf_d[1] = 0;
    buf_d[2] = 0;
    //komanda
    buf_d[3] = 0x9;
    //nomer freima
    buf_d[4] = 1;
    //mak-adres,set-adres
    buf_d[5] = 0x4E;
    buf_d[6] = 0x4F;
    buf_d[7] = 0; //2 - otvechaet normal 95 paketom, 0 - stalo toge+net 95 paketa ot koord - to est 0 - pravilno?
    buf_d[8] = 0;
    ktr = 0;
    for(int i = 3; i < 8; ++i)
        ktr += buf_d[i];
    buf_d[8] = 0xFF - ktr;
    buf_d[2] = 5;
    return Write(com_port, buf_d, 9);
}

int Vench::SetKolImpUstr(unsigned char *Mac,int impr,int impl){
    int  k = GetNumberUst(Mac);
    act_ust[k].impr = impr;
    act_ust[k].impl = impl;


    SaveUstrINI(Mac);

    return 1;
}

int Vench::GetKolImpUstr(int *Mac, int *impr, int *impl)
{
    unsigned char mmac[8];
    for (int i = 0; i < 8; ++i)
        mmac[i] = (unsigned char)Mac[i];
    int  k = GetNumberUst(mmac);
    *impr = act_ust[k].impr;
    *impl = act_ust[k].impl;

    return 1;
}

int Vench::SendZapisPanID_KONTR(int pn)
{
    unsigned char buf_d[40];

    //nachalo kadra
    buf_d[0] = 0x7E;
    //dlina
    buf_d[1] = 0;
    buf_d[2] = 0;
    //komanda
    buf_d[3] = 0x8;
    //nomer freima
    buf_d[4] = 0x01;
    buf_d[5] = 0x49;
    buf_d[6] = 0x44;
    buf_d[7] = (unsigned char)pn;
    unsigned char ktr = 0;

    for(int i = 3; i < 8; ++i)
        ktr += buf_d[i];
    buf_d[8] = 0xFF - ktr;
    buf_d[2] = 5;
    int res = Write(com_port, buf_d, 9);
    qDebug("Send comm smenit PANID kontr \n");

    return res;
}

int Vench::SendZapisPanID(unsigned char *Mac,int pn)
{
    qDebug("smenit Pan ID\n");
    if (flcomport != 1)
        return 0;
    unsigned char buf_d[40];
    //nachalo kadra
    buf_d[0] = 0x7E;
    //dlina
    buf_d[1] = 0;
    buf_d[2] = 0;
    //komanda
    buf_d[3] = 0x17;
    //nomer freima
    buf_d[4] = 0x01;
    //mak-adres,set-adres
    for(int i = 0; i < 8; ++i)
        buf_d[i + 5] = Mac[i];
    buf_d[13] = 0xff;
    buf_d[14] = 0xfe;
    buf_d[15] = 0x2;
    buf_d[16] = 0x49;
    buf_d[17] = 0x44;
    buf_d[18] = (unsigned char)pn;
    unsigned char ktr = 0;
    for(int i = 3; i < 19; ++i)
        ktr += buf_d[i];
    buf_d[19] = 0xFF - ktr;
    int len_d = 20;
    buf_d[2] = 16;

    qDebug("Send comm smenit PANID \n");
    return Write(com_port, buf_d, len_d);
}

int Vench::SendZaprosPANIDAll() //shirokoveshat zapros panID
{
    if (flcomport != 1)
        return 0;

    unsigned char buf_d[20];
    //nachalo kadra
    buf_d[0] = 0x7E;
    //dlina
    buf_d[1] = 0;
    buf_d[2] = 0;
    //komanda
    buf_d[3] = 0x17;
    //nomer freima
    buf_d[4] = 0x01;
    //mak-adres,set-adres
    for(int i = 0; i < 8; ++i)
        buf_d[i + 5] = 0;
    buf_d[11] = 0xFF;
    buf_d[12] = 0xFF;
    buf_d[13] = 0xfe;
    buf_d[14] = 0xff;
    buf_d[15] = 0x2;
    buf_d[16] = 0x49;
    buf_d[17] = 0x44;
    unsigned char ktr = 0;
    for(int i = 3; i < 18; ++i)
        ktr += buf_d[i];
    buf_d[18] = 0xFF - ktr;
    buf_d[2] = 15;
    printf("Send Zapros PANID ALL\n");
    return Write(com_port, buf_d, 19);
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
    buf_d[4] = 1;
    //mak-adres,set-adres
    for(i = 0; i < 8; i++)
        buf_d[i + 5] = 0;
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
    SendZaprosComm(MacSet, mbuf, 3);
    flzaprosdim = 1;
    if (flzaprostemposv == 0)
        SendCommTempOn();
    return 1;
}

int Vench::SendZaprosOtklDim(unsigned char *MacSet)// zapros konkretnogo ustroistva
{
    unsigned char mbuf[4];
    mbuf[0] = 0x44;
    mbuf[1] = 0x31;
    mbuf[2] = 0x05;
    SendZaprosComm(MacSet, mbuf, 3);
    flzaprosdim = 0;
    if (flzaprostemposv == 0)
        SendCommTempOff();
    return 1;
}


int Vench::SendZaprosComm(unsigned char *MacSet,unsigned char *cm, int len)// zapros konkretnogo ustroistva
{
    unsigned char buf_d[25];
    //nachalo kadra
    buf_d[0] = 0x7E;
    //dlina
    buf_d[1] = 0;
    buf_d[2] = 0x10; // dlina freima bez 4-x bait
    //komanda
    buf_d[3] = 0x17;
    //nomer freima
    buf_d[4] = 1;
    //mak-adres,set-adres
    for(int i = 0; i < 10; ++i)
        buf_d[i+5] = MacSet[i];
    buf_d[15] = 0x2;
    for(int i = 0; i < len; ++i)
        buf_d[16+i] = cm[i];

    unsigned char ktr = 0;
    len += 16;
    for(int i = 3; i < len; ++i)
        ktr += buf_d[i];
    buf_d[len] = 0xFF - ktr;
    buf_d[2] = len - 3;
    printf("\n");
    qDebug("Send Zapros Comm\n");
    return Write(com_port, buf_d, len + 1);
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





//vozvrashaet nomer ustr v massive po ego mac-adresu
int Vench::GetNumberUst(unsigned char *mac)
{
    int i;

    for(i = 0; i < mkol_ust; ++i)
    {
        int fl_itend = 1;
        for(int k = 0; k < 8; ++k)
            if( act_ust[i].mac_addr[k] != mac[k])
                fl_itend = 0;
        if (fl_itend == 1)
            break;
    }

    if (i == mkol_ust)
        i = -1;

    if (i == -1)
    {
        int fl_itend = 0;
        for(int k = 0; k < 8; ++k)
            if ((mac[k] != 0) && (mac[k] != 0xFF))
                fl_itend = 1;
        if (fl_itend == 0)
            i = -2; // dla opredelenia shirokovesh zaprosa
    }

    return i;
}


//pochemu-to ne rabotaet esli obyavit v tele main???????????????
//viletaet posle sled fscanf (flag_com == 0 ???????


// potoc read from port
void  Vench::TestPotoc(void * param)
{
    unsigned char buf_mac_set[20];
    Vench* md = (Vench*)param;

    md->mSleep(100);//chtobi ne zavisal??????????????

    md->ptemp = 0;
    md->pvlg = 0;
    md->pkk = -1;
    md->imp1 = 0;
    md->imp2 = 0;
    while(md->fl_potoc_rs)
    {
        md->mSleep(2);
        if (md->flcomport == 0)
            continue;
        if (md->fl_period && (md->GetTime() > md->t_oprosa))
        {
            md->SendZaprosTAll();
            md->t_oprosa = md->GetTime() + md->period_oprosa_t;
        }

        if (md->fl_period_e && (md->GetTime() > md->t_oprosa_e))
        {
            md->SendZaprosEAll();
            md->t_oprosa_e = md->GetTime() + md->period_oprosa_e;
        }

        //period zapros davl (poka dla vseh)
        if (md->flzaprosbar && (md->GetTime() > md->t_oprosa_b))
        {
            md->SendCommBarOn();
            md->t_oprosa_b = md->GetTime() + md->period_oprosa_b;
        }

        for(int ii = 0; ii < md->kol_ust; ++ii)
        {
            if (md->act_ust[ii].period_oprosa_t != 0) // esli neobhodimo oprosit ustr-vo
            {
                if ((md->act_ust[ii].fl_oprosa > KOL_ANSW) && (md->act_ust[ii].fl_izm_pt == 0)) // esli eto ustr-vo eshe ne oprashivali
                {
                    md->act_ust[ii].period_oprosa_t = md->act_ust[ii].period_oprosa_t * 10.0;
                    md->act_ust[ii].fl_izm_pt = 1;
                    printf("error temp\n");
                }
                if (md->act_ust[ii].fl_oprosa > (2 * KOL_ANSW)) // esli eto ustr-vo eshe ne oprashivali
                {
                    printf("error temp\n");
                    md->act_ust[ii].fl_oprosa = KOL_ANSW;
                }

                if (md->act_ust[ii].time_oprosa_t < md->GetTime()) // esli prishlo vrema oprosa
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = md->act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = md->act_ust[ii].set_addr[0];
                    buf_mac_set[9] = md->act_ust[ii].set_addr[1];
                    md->SendZaprosT(buf_mac_set);
                    md->act_ust[ii].fl_oprosa++;
                    md->act_ust[ii].time_oprosa_t = md->GetTime() + md->act_ust[ii].period_oprosa_t;
                    if (md->act_ust[ii].fl_izm_pt == 1)
                        printf("error temp\n");
                }
            }

            if (md->act_ust[ii].period_oprosa_e != 0) // esli neobhodimo oprosit ustr-vo
            {
                if ((md->act_ust[ii].fl_oprosa > KOL_ANSW) && (md->act_ust[ii].fl_izm_pe == 0)) // esli eto ustr-vo eshe ne oprashivali
                {
                    md->act_ust[ii].period_oprosa_e = md->act_ust[ii].period_oprosa_e * 10.0;
                    md->act_ust[ii].fl_izm_pe = 1;
                    printf("error energ\n");
                }
                if (md->act_ust[ii].fl_oprosa > (2 * KOL_ANSW)) // esli eto ustr-vo eshe ne oprashivali
                {
                    printf("error energ\n");
                    md->act_ust[ii].fl_oprosa = KOL_ANSW;// chtob ne bilo perepolnenia
                }
                if (md->act_ust[ii].time_oprosa_e < md->GetTime()) // esli prishlo vrema oprosa
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = md->act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = md->act_ust[ii].set_addr[0];
                    buf_mac_set[9] = md->act_ust[ii].set_addr[1];
                    md->SendZaprosE(buf_mac_set);
                    md->act_ust[ii].fl_oprosa++;
                    md->act_ust[ii].time_oprosa_e = md->GetTime() + md->act_ust[ii].period_oprosa_e;
                    if (md->act_ust[ii].fl_izm_pe == 1)
                        printf("error energ\n");
                }
            }

            if (( md->act_ust[ii].period_oprosa_o > 0) && (md->act_ust[ii].time_oprosa_o < md->GetTime())) // esli prishlo vrema oprosa
            {
                for(int k = 0; k < 8; ++k)
                    buf_mac_set[k] = md->act_ust[ii].mac_addr[k];
                buf_mac_set[8] = md->act_ust[ii].set_addr[0];
                buf_mac_set[9] = md->act_ust[ii].set_addr[1];
                md->act_ust[ii].time_oprosa_o = md->GetTime() + md->act_ust[ii].period_oprosa_o;
            }
        }
// end for analiz periodich komand

        md->readbuf(); // chtenie dannih
        if (md->GetKadrN(md->pbuf_data, &(md->plen)) > 0) // esli est dannie dla chenia
        {
            md->print_log(md->pbuf_data, md->plen, 0);
            if (md->InitKdg(md->pbuf_data) > 0) // esli eto kodogramma inicializacii
                md->NewActUst();
            else // obrabotka datchika kasania
            {
                Dat dt;
                if (md->TryDatKas(md->pbuf_data, md->plen, dt) > 0)
                {
                    if (dt == Dat::Right)
                        md->DatKas(Dat::Left);
                    if (dt == Dat::Left)
                        md->DatKas(Dat::Right);
                }
                if(md->TryGetMash(md->pbuf_data, md->plen) > 0)
                {
                    md->GetMacSetAddr(md->pbuf_data, md->pbuf_mac, md->pbuf_set, md->pbuf_parent);
                    md->pbuf_mac[8] = md->pbuf_set[0];
                    md->pbuf_mac[9] = md->pbuf_set[1];

                    // esli eto ustroistvo is spiska to sbrasivaem flag oprosa
                    //chtobi oprosil sleduyshii
                    md->pkk = md->GetNumberUst(md->pbuf_mac);
                    md->errte = md->GetImp(md->pbuf_data, md->plen, 1, &(md->imp1), &(md->imp2), &(md->napr));

                    if (md->errte == -2)
                        md->errte = 1;

                    int fl_sov = 1;
                    for(int mk = 0; mk < 8; ++mk)
                        if (md->pbuf_mac[mk] != md->mac_addr_prg[mk])
                            fl_sov = 0;
                    if (fl_sov == 1)
                    {
                        md->print_imp_sleep(md->pbuf_mac, md->imp1, md->imp2,md->napr, md->errte);
                        if (!md->fl_diskr)
                            md->SetAnswerUstr(md->imp1, md->imp2,md->napr);
                    }
                }

                //obrabotchik inf kardiografa
                if (md->fl_krd == 1)
                {
                    if (md->fl_krd_start == 0)
                    {
                        if (md->TryGetKrdBegin(md->pbuf_data, md->plen) > 0)
                        {
                            md->fl_krd_start = 1;
                            md->fl_stop = 0;
                            md->fl_pr_stop = 0;
                            md->fl_bt_kr = 0;
                            md->sh_mas1 = 0;
                            md->sh_mas2 = 0;
                            md->fl_mas = 0;
                        }
                    }
                    else
                    {
                        // idet inf s dannimi
                        for(md->kf = 0; md->kf < md->plen; ++md->kf)
                        {
                            // uslovie dla konca faila data
                            if ((md->fl_stop == 0) && (md->pbuf_data[md->kf] == 0xE6))
                                md->fl_stop++;
                            if (md->fl_stop == 1)
                            {
                                if (md->pbuf_data[md->kf] == 0x80)
                                    md->fl_stop++;
                                else
                                    md->fl_stop=0;
                            }
                            if (md->fl_stop == 2)
                            {
                                if (md->pbuf_data[md->kf] == 0xE6)
                                    md->fl_stop++;
                                else
                                    md->fl_stop = 0;
                            }
                            if (md->fl_stop == 3)
                            {
                                if (md->pbuf_data[md->kf] == 0x80)
                                    md->fl_stop++;
                                else
                                    md->fl_stop = 0;
                            }
                            if (md->fl_stop == 4)
                            {
                                md->fl_pr_stop = 1;
                                break;
                            }
                            // end uslovie dla konca faila data
                            if(md->fl_bt_kr == 0)
                            {
                                if (md->fl_mas == 0)
                                    md->mas_1[md->sh_mas1] = md->pbuf_data[md->kf];
                                md->sh_mas1++;
                                md->fl_bt_kr = 1;
                            }

                            if(md->fl_bt_kr == 1)
                            {
                                if (md->fl_mas == 0)
                                    md->mas_2[md->sh_mas2] = md->pbuf_data[md->kf];
                                md->sh_mas2++;
                                md->fl_bt_kr = 0;
                            }

                            if ((md->sh_mas1 == NMAS_KG - 1) || (md->sh_mas2 == NMAS_KG - 1))
                            {
                                if (md->fl_mas == 0)
                                {
                                    md->fl_mas = 1;
                                    md->sh_mas1 = 0;
                                    md->sh_mas2 = 0;
                                    md->printKG(0);
                                }
                                else
                                {
                                    md->fl_mas = 0;
                                    md->sh_mas1 = 0;
                                    md->sh_mas2 = 0;
                                    md->printKG(1);
                                }
                            }
                        }
                        if (md->fl_pr_stop == 1)
                        {
                            if (md->fl_mas == 0)
                                md->printKG(0);
                            if (md->fl_mas == 1)
                                md->printKG(1);
                            md->fl_krd_start = 0;
                        }
                    }
                }

                if (md->flzaprosbar && (md->GetBar(md->pbuf_data, md->plen, &(md->pbar), &(md->pvlg), md->act_ust[md->pkk].tar_kfd) > 0)) //esli bil zapros davl analiziruem estli otvet
                {
                    md->print_data(md->pbuf_mac, md->pbar, 4);
                    md->print_data_s(md->pbuf_mac, md->pbar, 4);

                    md->print_data(md->pbuf_mac, md->pvlg, 5);
                    md->print_data_s(md->pbuf_mac, md->pvlg, 5);

                    md->PaintTxtData(5, md->pbar);
                    md->PaintTxtData(6, md->pvlg);
                }

                if(md->TryGetTemp(md->pbuf_data, md->plen) > 0)
                {
                    md->GetMacSetAddr(md->pbuf_data, md->pbuf_mac, md->pbuf_set, md->pbuf_parent);
                    md->pbuf_mac[8] = md->pbuf_set[0];
                    md->pbuf_mac[9] = md->pbuf_set[1];

                    // esli eto ustroistvo is spiska to sbrasivaem flag oprosa chtobi oprosil sleduyshii
                    md->pkk = md->GetNumberUst(md->pbuf_mac);
                    if (md->pkk >= 0)
                    {
                        md->act_ust[md->pkk].fl_oprosa = 0;
                        md->errte = md->GetTemp(md->pbuf_data, md->plen, &(md->ptemp), &(md->posv), md->act_ust[md->pkk].tar_kf);
                        if ((md->errte > 0) && (md->flzaprosdim == 1))
                        {
                            md->GetDim(md->pbuf_data,md->plen,&(md->pdim),md->act_ust[md->pkk].tar_kfd);
                            md->print_data(md->pbuf_mac,md->pdim,3);
                            md->print_data_s(md->pbuf_mac,md->pdim,3);
                            md->PaintTxtData(4,md->pdim);

                            if (md->act_ust[md->pkk].fl_izm_pd==1)
                            {
                                md->act_ust[md->pkk].fl_izm_pd = 0;
                                md->act_ust[md->pkk].period_oprosa_d = md->act_ust[md->pkk].period_oprosa_d/10.0;
                            }
                            qDebug("get data dim0\n");
                        }
                        if (md->act_ust[md->pkk].fl_izm_pt == 1)
                        {
                            md->act_ust[md->pkk].fl_izm_pt = 0;
                            md->act_ust[md->pkk].period_oprosa_t = md->act_ust[md->pkk].period_oprosa_t/10.0;
                        }
                    }
                    else //esli ustroistvo ne is spiska
                    {
                        md->errte = md->GetTemp(md->pbuf_data, md->plen, &(md->ptemp), &(md->posv), 1);

                        if (md->flzaprosdim == 1)
                        {
                            md->GetDim(md->pbuf_data, md->plen, &(md->pdim), md->act_ust[md->pkk].tar_kfd);
                            md->print_data(md->pbuf_mac, md->pdim, 3);
                            qDebug("get data dim\n");
                        }
                    }
                    md->pkk = -1;

                    if (md->errte > 0)
                    {
                        if (md->flzaprostemposv)
                        {
                            md->print_data(md->pbuf_mac, md->ptemp, 0);
                            md->print_data(md->pbuf_mac, md->posv, 2);
                            md->print_data_s(md->pbuf_mac, md->ptemp, 0);
                            md->print_data_s(md->pbuf_mac, md->posv, 2);
                        }
                        md->PaintTxtData(1, md->ptemp);
                        md->PaintTxtData(3, md->posv);
                    }
                    if ((md->pr_spec_file_t == 1) && (md->flzaprostemposv))
                    {
                        md->print_spec_file("t", md->pbuf_mac, md->ptemp);
                        md->print_spec_file("o", md->pbuf_mac, md->posv);
                    }
                }
                else
                {
                    if(md->GetEnerg(md->pbuf_data,md->plen,&(md->penerg)) > 0)
                    {
                        md->GetMacSetAddr(md->pbuf_data, md->pbuf_mac, md->pbuf_set, md->pbuf_parent);
                        md->pbuf_mac[8] = md->pbuf_set[0];
                        md->pbuf_mac[9] = md->pbuf_set[1];
                        md->print_data(md->pbuf_mac, md->penerg,1);
                        md->print_data_s(md->pbuf_mac, md->penerg,1);
                        md->PaintTxtData(2, md->penerg);
                        if (md->pr_spec_file_e == 1)
                            md->print_spec_file("e", md->pbuf_mac, md->penerg);

                        // esli eto ustroistvo is spiska to sbrasivaem flag oprosa chtobi oprosil sleduyshii
                        md->pkk = md->GetNumberUst(md->pbuf_mac);
                        if (md->pkk >= 0)
                        {
                            md->act_ust[md->pkk].fl_oprosa = 0;
                            if (md->act_ust[md->pkk].fl_izm_pe == 1)
                            {
                                md->act_ust[md->pkk].fl_izm_pe = 0;
                                md->act_ust[md->pkk].period_oprosa_e = md->act_ust[md->pkk].period_oprosa_e / 10.0;
                            }
                        }
                        md->pkk = -1;
                    }
                    else
                    {
                        if(md->TryGetPANID(md->pbuf_data,md->plen) > 0)
                        {
                            md->GetMacSetAddr(md->pbuf_data,md->pbuf_mac,md->pbuf_set, md->pbuf_parent);
                            md->pbuf_mac[8] = md->pbuf_set[0];
                            md->pbuf_mac[9] = md->pbuf_set[1];
                            md->pkk = md->GetNumberUst(md->pbuf_mac);
                            if (md->pkk >= 0)
                            {
                                md->errte = md->GetPANID(md->pbuf_data, md->plen, &(md->act_ust[md->pkk].pan1), &(md->act_ust[md->pkk].pan2));
                                if (md->errte > 0)
                                {
                                    md->act_ust[md->pkk].fl_pan = 1;
                                    md->fl_repaint_panid = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printf("End test potoc rs\n");
}

int Vench::SendCommForRS(const char * str)
{
    int fs;
    char ss[10];
    const char * mstr = str;
    int len_st = 0;

    unsigned char  buf_mac_set[10];
    unsigned char  buf_com[40];
    unsigned char  mbuf_mac[8];
    unsigned char  comm[4];

    unsigned int mt,mmt;

    int it = 0;
    int fl_usp;//usiplat == 0 ili budit == 1
    int i = 0;
    int fl_end_c = 0;
    unsigned char ktr_m;
    int imm;
    float m_sleep;
    double tm_sleep;

    float p_sn;
    float mtf;

    flag_com=0;

    while(1)
    {
        qWarning() << mstr;
        if (flag_com == 0)
        {
            fs = sscanf(mstr, "%s", ss);
            mstr += (QString(ss).length());
            if (fs < 0)
                break;

            if (ss[0] == '*') // komanda dla peresilki dannih v port
                flag_com=1;
            if (ss[0] == '&')
                flag_com=2;
            if (((ss[0] == 'q')&&(ss[1]=='u')&&(ss[2]=='i')&&(ss[3]='t'))||((ss[0] == 'Q')&&(ss[1]=='U')&&(ss[2]=='I')&&(ss[3]='T')))
                break;

            if (((ss[0] == 'l')&&(ss[1]=='o')&&(ss[2]=='g'))||((ss[0] == 'L')&&(ss[1]=='O')&&(ss[2]=='G')))
            {
                qDebug("print log\n");
                fl_print_log = true;
                break;
            }

            if (((ss[0] == 'u')&&(ss[1]=='l')&&(ss[2]=='o')&&(ss[3]='g'))||((ss[0] == 'U')&&(ss[1]=='L')&&(ss[2]=='O')&&(ss[3]='G')))
            {
                qDebug("not print log\n");
                fl_print_log = false;
                break;
            }
        }
        if (flag_com == 1) //komanda tochoi peresilki v port
        {
            fs = sscanf(mstr, "%x", &mt);
            mstr += (QString().setNum(mt, 16).length() + 1);
            if (fs<0)
                break;

            if (mt == 0x7e)
                it = 1; // nachalo komandi
            if (it == 1)
            {
                buf_com[i] = (unsigned char)mt;
                if (i == 2)
                    len_st =(unsigned int)mt + 3;
            }
            if ((i == (len_st - 1)) && (len_st >= 4))
                fl_end_c = 1;

            if (i > 0)
            {
                if (fl_end_c == 1)
                {
                    if (it == 1)
                    {
                        ktr_m =0;
                        for(imm = 3; imm < (i +1 ); ++imm)
                            ktr_m = ktr_m + buf_com[imm];
                        buf_com[i + 1] = 0xFF - ktr_m;
                        int k = Write(com_port, buf_com, i + 2);
                        if (k != (i + 2))
                        {
                            qDebug("ERROR send comm! k = %d  i +2 = %d\n",k,i+2);
                            mstr += strlen(mstr);
                            break;
                        }
                        else
                        {
                            qDebug("Send comm!\n");
                            mstr += strlen(mstr);
                            break;
                        }
                        i = 0;
                        it = 0;
                        flag_com = 0;
                    }
                }
            }
            if (it == 1)
                ++i;
            if (i == 40)
            {
                qDebug("ERROR read comm!!!\n");
                i = 0;
                it = 0;
                flag_com = 0;
            }
        }

        if (flag_com == 2)
        {
            fs = sscanf(mstr,"%x",&mmt);
            mstr += (QString().setNum(mmt,16).length()+1);

            if (fs <= 0)
                break;
            if (mmt == 0) // obnovit file activnih ustristv
            {
                fs = sscanf(mstr,"%d",&mt);
                mstr += (QString().setNum(mmt,16).length() + 1);
                if (fs > 0)
                {
                    if (mt == 1) //chtobi perezapisivat file & data in program
                    {
                        printf("Obnovili fil act ustr-v && data in program\n");
                        mkol_ust = 0;
                        SendZaprosI();
                    }
                }
                else // tolko file
                    printf("Obnovili fil act ustr-v\n");
                NewActUst();
            }

            if (mmt == 2)
            {
                fs = sscanf(mstr,"%d",&mt);
                mstr += (QString().setNum(mmt,16).length() + 1);

                printf("Zapros energ period = %d\n",mt);
                if (fs > 0)
                {
                    if (mt > 0)
                    {
                        period_oprosa_e = mt;
                        fl_period_e = 1;
                    }

                    if (mt == 0)
                    {
                        SendZaprosEAll();
                        fl_period_e = 0;
                        period_oprosa_e = 0;
                    }
                }
            }

            if (mmt == 1)
            {
                fs = sscanf(mstr,"%d",&mt);
                mstr += (QString().setNum(mt, 10).length() + 1);
                qDebug("Zapros temp period = %d\n",mt);
                if (fs > 0)
                {
                    if (mt > 0)
                    {
                        period_oprosa_t = mt;
                        fl_period = 1;
                    }
                    if (mt == 0)
                    {
                        SendZaprosTAll();
                        fl_period = 0;
                        period_oprosa_t = 0;
                    }
                }
            }

            if (mmt == 3) // zapros temp koncretnogo ustr po mac adresu
            {
                fs = sscanf(mstr,"%d",&mt);
                mstr += (QString().setNum(mt).length() + 1);
                mtf = mt;
                if (fs < 0)
                    break;
                period_oprosa_t = mtf;
                qDebug("Zapros temp period mk = %f\n", mtf);
                pr_spec_file_t = 1;
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;

                    mbuf_mac[ii] = (unsigned char)mt;
                }
                if (fs < 0)
                    break;
                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;
                }

                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;
                }
                int ii = GetNumberUst(mbuf_mac);
                if (ii > -1)
                {
                    act_ust[ii].period_oprosa_t = period_oprosa_t;
                    act_ust[ii].fl_izm_pt = 0;
                    act_ust[ii].fl_oprosa = 0;
                    period_oprosa_t=0;
                    if (act_ust[ii].period_oprosa_t == 0)
                    {
                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = act_ust[ii].mac_addr[k];
                        buf_mac_set[8] = act_ust[ii].set_addr[0];
                        buf_mac_set[9] = act_ust[ii].set_addr[1];

                        SendZaprosT(buf_mac_set);
                    }
                    qDebug("Zapros temp period = %f\n", act_ust[ii].period_oprosa_t);
                }
                else
                    if (ii == -2)
                    {
                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = 0;
                        buf_mac_set[6] = 0xff;
                        buf_mac_set[7] = 0xff;
                        buf_mac_set[8] = 0xfe;
                        buf_mac_set[9] = 0xff;

                        SendZaprosT(buf_mac_set);
                    }
                    else
                        printf("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
            }

            if (mmt == 4) // zapros energ koncretnogo ustr po mac adresu
            {
                fs = sscanf(mstr, "%f", &mtf);
                mstr += (QString().setNum(mtf).length() + 1);

                if (fs < 0)
                    break;

                period_oprosa_e = mtf;
                pr_spec_file_e = 1;

                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);

                    if (fs < 0)
                        break;

                    mbuf_mac[ii] = (unsigned char)mt;
                }

                if (fs < 0)
                    break;

                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;
                }

                int ii = GetNumberUst(mbuf_mac);

                if (ii >= -1)
                {
                    act_ust[ii].period_oprosa_e = period_oprosa_e;
                    act_ust[ii].fl_izm_pe = 0;
                    period_oprosa_e = 0;

                    if (act_ust[ii].period_oprosa_e == 0)
                    {
                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = act_ust[ii].mac_addr[k];
                        buf_mac_set[8] = act_ust[ii].set_addr[0];
                        buf_mac_set[9] = act_ust[ii].set_addr[1];
                        SendZaprosE(buf_mac_set);
                    }

                    act_ust[ii].fl_oprosa = 0;
                    qDebug("Zapros energ period = %f\n",act_ust[ii].period_oprosa_e);
                }
                else
                    if (ii == -2)
                    {
                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = 0;
                        buf_mac_set[6] = 0xff;
                        buf_mac_set[7] = 0xff;
                        buf_mac_set[8] = 0xfe;
                        buf_mac_set[9] = 0xff;
                        SendZaprosE(buf_mac_set);
                    }
                    else
                        qDebug("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
            }

            if (mmt == 5)
            {
                fs = sscanf(mstr, "%f", &m_sleep);
                mstr += (QString().setNum(m_sleep).length() + 1);

                tm_sleep = (double)m_sleep;
                if (fs < 0)
                    break;
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;
                    mbuf_mac[ii] = (unsigned char)mt;
                }

                if (fs < 0)
                    break;

                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;
                }

                int ii = GetNumberUst(mbuf_mac);
                if (ii >= -1)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = act_ust[ii].set_addr[0];
                    buf_mac_set[9] = act_ust[ii].set_addr[1];
                    SendZaprosSleep(buf_mac_set,tm_sleep); // zapros konkretnogo ustroistva
                    mSleep(50);
                    SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
                }

                if (ii == -2)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = 0;
                    buf_mac_set[6] = 0xff;
                    buf_mac_set[7] = 0xff;
                    buf_mac_set[8] = 0xfe;
                    buf_mac_set[9] = 0xff;
                    SendZaprosSleep(buf_mac_set, tm_sleep);// zapros konkretnogo ustroistva
                    SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
                }
            }

            if (mmt == 6)
            {
                fs = sscanf(mstr,"%f",&mtf);
                mstr += (QString().setNum(mtf).length() + 1);
                if (fs < 0)
                    break;
                period_oprosa_d = mtf;
                qDebug("Zapros zadim period mk = %f\n", mtf);
                pr_spec_file_d = 1;
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);

                    if (fs < 0)
                        break;

                    mbuf_mac[ii] = (unsigned char)mt;
                }
                if (fs < 0)
                    break;

                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt,16).length() + 1);
                    if (fs < 0)
                        break;
                }

                int ii = GetNumberUst(mbuf_mac);
                if (ii > -1)
                {
                    act_ust[ii].period_oprosa_d = period_oprosa_d;
                    act_ust[ii].fl_izm_pd = 0;
                    act_ust[ii].fl_oprosa = 0;
                    period_oprosa_d = 0;
                    if (act_ust[ii].period_oprosa_d == 0)
                    {
                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = act_ust[ii].mac_addr[k];
                        buf_mac_set[8] = act_ust[ii].set_addr[0];
                        buf_mac_set[9] = act_ust[ii].set_addr[1];
                        SendZaprosDim(buf_mac_set);
                    }
                    qDebug("Zapros zadim period = %f\n", act_ust[ii].period_oprosa_d);
                }
                else
                    if (ii == -2)
                    {
                        for(int k = 0; k <8 ; ++k)
                            buf_mac_set[k] = 0;
                        buf_mac_set[6] = 0xff;
                        buf_mac_set[7] = 0xff;
                        buf_mac_set[8] = 0xfe;
                        buf_mac_set[9] = 0xff;
                        SendZaprosDim(buf_mac_set);
                    }
                    else
                        printf("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
            }

            if (mmt == 0xA)
            {
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);

                    if (fs < 0)
                        break;
                    mbuf_mac[ii] = (unsigned char)mt;
                }
                if (fs < 0)
                    break;
                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                }

                int ii = GetNumberUst(mbuf_mac);
                if (ii >= -1)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = act_ust[ii].set_addr[0];
                    buf_mac_set[9] = act_ust[ii].set_addr[1];
                    SendDriveStart(buf_mac_set);
                }

                if (ii == -2)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = 0;
                    buf_mac_set[6] = 0xff;
                    buf_mac_set[7] = 0xff;
                    buf_mac_set[8] = 0xfe;
                    buf_mac_set[9] = 0xff;
                    SendDriveStart(buf_mac_set);
                }
            }

            if (mmt == 0xD)
            {
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                    mbuf_mac[ii] = (unsigned char)mt;
                }
                if (fs < 0)
                    break;
                int ii = GetNumberUst(mbuf_mac);
                if (ii >= -1)
                {
                    for (ii = 0; ii < 2; ++ii)
                    {
                        fs = sscanf(mstr, "%x", &mt);
                        mstr += (QString().setNum(mt, 16).length() + 1);
                        if (fs < 0)
                            break;
                    }
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = act_ust[ii].set_addr[0];
                    buf_mac_set[9] = act_ust[ii].set_addr[1];
                    SendDriveStop(buf_mac_set);
                }

                if (ii == -2)
                {
                    for (ii = 0; ii < 2; ++ii)
                    {
                        fs = sscanf(mstr, "%x", &mt);
                        mstr += (QString().setNum(mt, 16).length() + 1);
                        if (fs < 0)
                            break;
                    }

                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = 0;
                    buf_mac_set[6] = 0xff;
                    buf_mac_set[7] = 0xff;
                    buf_mac_set[8] = 0xfe;
                    buf_mac_set[9] = 0xff;
                    SendDriveStop(buf_mac_set);
                }
            }

            if (mmt == 0xB)  // send comm po 17 paketu &addr
            {
                fs = sscanf(mstr, "%d", &mt);
                mstr += (QString().setNum(mt, 10).length() + 1);
                if (fs < 0)
                    break;
                period_oprosa_e = mt;
                for (int ii = 0; ii < period_oprosa_e; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                    comm[ii] = (unsigned char)mt;
                }

                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                    mbuf_mac[ii] = (unsigned char)mt;
                }

                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                }
                if (fs < 0)
                    break;
                int ii = GetNumberUst(mbuf_mac);

                if (ii > -1)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = act_ust[ii].set_addr[0];
                    buf_mac_set[9] = act_ust[ii].set_addr[1];

                    SendZaprosComm(buf_mac_set,comm, (int)period_oprosa_e);// zapros konkretnogo ustroistva
                }
                else
                    if (ii == -2)
                    {
                        for (ii = 0; ii < 2; ++ii)
                        {
                            fs = sscanf(mstr, "%x", &mt);
                            mstr += (QString().setNum(mt, 16).length() + 1);
                            if (fs < 0)
                                break;
                        }

                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = 0;
                        buf_mac_set[6] = 0xff;
                        buf_mac_set[7] = 0xff;
                        buf_mac_set[8] = 0xff;
                        buf_mac_set[9] = 0xfe;

                        SendZaprosComm(buf_mac_set,comm, (int)period_oprosa_e);// zapros konkretnogo ustroistva
                    }
                    else
                        printf("Error ustr-vo!!\n");
            }

            if (mmt == 0xC) // delaem algoritm usiplenia
            {
                if (fl_al_sn == 0)
                    fl_al_sn = 1;
                else if (fl_al_sn == 1)
                    fl_al_sn = 0;
                if (fl_usp == 1)
                {
                    fl_usp = 0;
                    fl_al_sn = 0;
                }

                if (fl_al_sn == 1)
                {
                    fs = sscanf(mstr, "%f", &t_sn);
                    mstr += (QString().setNum(t_sn).length() + 1);
                    if (fs < 0)
                        break;

                    fs = sscanf(mstr, "%f", &p_sn);
                    mstr += (QString().setNum(p_sn).length() + 1);
                    if (fs < 0)
                        break;

                    printf("Begin Algoritm usipl\n");
                    SetParentUst();

                    fl_usp = 0;
                }
                else
                    printf("End Algoritm usipl\n");
            }

            if (mmt == 9)
            {
                fs = sscanf(mstr, "%f", &m_sleep);
                mstr += (QString().setNum(m_sleep).length() + 1);

                tm_sleep = (double)m_sleep;
                if (fs < 0)
                    break;
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                    mbuf_mac[ii] = (unsigned char)mt;
                }

                if (fs < 0)
                    break;

                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                }

                int ii = GetNumberUst(mbuf_mac);
                if (ii >= -1)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = act_ust[ii].set_addr[0];
                    buf_mac_set[9] = act_ust[ii].set_addr[1];
                    SendSetAlarm(buf_mac_set);// zapros konkretnogo ustroistva
                    SendZaprosSleep(buf_mac_set,TM_UMOL);// zapros konkretnogo ustroistva
                    SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
                }

                if (ii == -2)
                {
                    for (ii = 0; ii < 2; ++ii)
                    {
                        fs = sscanf(mstr, "%x", &mt);
                        mstr += (QString().setNum(mt, 16).length() + 1);
                        if (fs < 0)
                            break;
                    }
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = 0;
                    buf_mac_set[6] = 0xff;
                    buf_mac_set[7] = 0xff;
                    buf_mac_set[8] = 0xfe;
                    buf_mac_set[9] = 0xff;
                    SendSetAlarm(buf_mac_set);// zapros konkretnogo ustroistva
                    SendZaprosSleep(buf_mac_set,TM_UMOL);// zapros konkretnogo ustroistva
                    SendSetSleep(buf_mac_set);// zapros konkretnogo ustroistva
                }


        }

            if (mmt == 8)
            {
                for (int ii = 0; ii < 8; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);

                    if (fs < 0)
                        break;
                    mbuf_mac[ii] = (unsigned char)mt;
                }
                if (fs < 0)
                    break;
                for (int ii = 0; ii < 2; ++ii)
                {
                    fs = sscanf(mstr, "%x", &mt);
                    mstr += (QString().setNum(mt, 16).length() + 1);
                    if (fs < 0)
                        break;
                }

                int ii = GetNumberUst(mbuf_mac);
                if (ii >= -1)
                {
                    for(int k = 0; k < 8; ++k)
                        buf_mac_set[k] = act_ust[ii].mac_addr[k];
                    buf_mac_set[8] = act_ust[ii].set_addr[0];
                    buf_mac_set[9] = act_ust[ii].set_addr[1];

                    if (fl_krd == 0)
                    {
                        if(fl_krd_start == 0)
                        {
                            SendCommStartKrd(buf_mac_set);
                            fl_krd = 1;
                            continue;
                        }
                    }
                    else
                    {
                        SendCommStopKrd(buf_mac_set);
                        fl_krd = 0;
                        continue;
                    }
                }
                else
                    if (ii == -2)
                    {
                        for (ii = 0; ii < 2; ++ii)
                        {
                            fs = sscanf(mstr, "%x", &mt);
                            mstr += (QString().setNum(mt, 16).length() + 1);
                            if (fs < 0)
                                break;
                        }
                        for(int k = 0; k < 8; ++k)
                            buf_mac_set[k] = 0;
                        buf_mac_set[6] = 0xff;
                        buf_mac_set[7] = 0xff;
                        buf_mac_set[8] = 0xfe;
                        buf_mac_set[9] = 0xff;

                        if (fl_krd == 0)
                        {
                            if(fl_krd_start == 0)
                            {
                                SendCommStartKrd(buf_mac_set);
                                fl_krd = 1;
                                continue;
                            }
                        }
                        else
                        {
                            SendCommStopKrd(buf_mac_set);
                            fl_krd=0;
                            continue;
                        }
                    }
                    else printf("ERROR: ustr s dannim mac adresom otsutstvuet!\n");
            }

            if (mmt == 7) // delaem dochernee ustroistvo
            {
                if (mkol_ust >= 2)
                {
                    act_ust[1].parent_addr[0] = act_ust[0].set_addr[0];
                    act_ust[1].parent_addr[1] = act_ust[0].set_addr[1];
                    printf("Ust: 1 - dochernee dla 0\n");
                }
            }
        }
    }

    return 1;
}

void Vench::CarChanged(int fl_car)
{
    int car_ind = fl_car - 1;

    for (int i = 0; i < kol_ust; ++i)
    {
        int fl_state = 0;
        if (car_ind == i)
            fl_state = 1;

        mas_vibr_ust[i] = fl_state;
        act_ust[i].vibr_sp = fl_state;
    }

    if (fl_car > 0)
        fl_vibr_ust = 1;
    else
        fl_vibr_ust = 0;
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
}

void Vench::BRec_pressed_n()
{
    if (fl_rec == 0)
    {
        fl_rec = 1;
        return;
    }
    if (fl_rec == 1)
    {
        fl_rec = 0;
        WriteProg1(num_comm);
        return;
    }
}

void Vench::ProgrClicked()
{
#ifdef WIN_D
    QString ms = prog_for_read_win + " ";
    if (fl_xml)
        ms += f_name_xml;
    else
        ms += f_name_kum;
    QByteArray rr = ms.toLocal8Bit();
    WinExec(rr, SW_RESTORE);
#endif
#ifdef LINUX_D
    QString ms = mEdit;
    QString fnamec = serverDir;
    if (fl_bluetooth)
        fnamec += f_name_snap;
    else
        fnamec += f_name_kum;

    if (fl_xml)
        ms += serverDir + f_name_xml + " &";
    else
         ms += fnamec + " &";

    qDebug() << ms;
    system(ms.toLocal8Bit());
#endif
}

void Vench::RezultClicked()
{
    int flv = 0;
    QString txt = serverDir;
#ifdef LINUX_D
    txt += "DATA";
 #endif
 #ifdef WIN_D
    txt += "/DATA";
 #endif
    for (int k = 0; k < kol_ust; ++k)
        if (mas_vibr_ust[k] == 1)
        {
            flv = 1;
            for(int i = 0; i < 8; ++i)
                txt += QString().setNum(mas_mac_addr[k][i], 16);
            txt += ".txt";
 #ifdef LINUX_D
            QString ms = mEdit + txt + " &";
            qDebug() << ms;
            system(ms.toLocal8Bit().constData());
 #endif
 #ifdef WIN_D
            QString ms = "notepad.exe ";
            ms += txt;
            WinExec(ms.toLocal8Bit().constData(), SW_RESTORE);
 #endif
        }
    if ((flv == 0) && (kol_ust > 0))
    {
        for(int i = 0; i < 8; ++i)
            txt += QString().setNum(mas_mac_addr[0][i], 16);
        txt += ".txt";
 #ifdef LINUX_D
        QString ms = mEdit + txt + " &";
        qDebug() << ms;
        system(ms.toLocal8Bit().constData());
 #endif
 #ifdef WIN_D
            QString ms = "notepad.exe ";
            ms += txt;
            WinExec(ms.toLocal8Bit().constData(), SW_RESTORE);
 #endif
    }
}

void Vench::on_pBTemp_E_clicked()
{
    fl_energ = ui->pBTemp_E->isChecked();
    BPressEnerg();
}

void Vench::on_pBTemp_D_clicked()
{
    fl_dim = ui->pBTemp_D->isChecked();
    BPressDim();
}

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

void Vench::SendCommBipBT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x8e; //begin packet
    buf[1] = 0x7;  //command packet
    buf[2] = 0x8f; //end packet

    Write(com_port, buf, 3);
}

void Vench::SendCommLightBT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x8e; //begin packet
    buf[1] = 0xF;  //command packet
    buf[2] = 0x8f; //end packet

    Write(com_port, buf, 3);
}

void Vench::SendCommForw_ave_BT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x0;  //begin packet
    buf[1] = 0x0;  //command packet
    buf[2] = 0x81; //end packet
    buf[3] = 0x0;  //begin packet
    buf[4] = 0x4;  //command packet
    buf[5] = 0x0;  //end packet
    buf[6] = 0x0;  //end packet

    Write(com_port, buf, 7);
}

void Vench::SendCommRevers_ave_BT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x0;  //begin packet
    buf[1] = 0x0;  //command packet
    buf[2] = 0x7F; //end packet
    buf[3] = 0x0;  //begin packet
    buf[4] = 0x4;  //command packet
    buf[5] = 0x0;  //end packet
    buf[6] = 0x0;  //end packet

    Write(com_port, buf, 7);
}

void Vench::SendCommLeft_ave_BT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x0;  //begin packet
    buf[1] = 0x81; //command packet
    buf[2] = 0x0;  //end packet
    buf[3] = 0x0;  //begin packet
    buf[4] = 0x4;  //command packet
    buf[5] = 0x0;  //end packet
    buf[6] = 0x0;  //end packet

    Write(com_port, buf, 7);
}

void Vench::SendCommRight_ave_BT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x0;  //begin packet
    buf[1] = 0x7F; //command packet
    buf[2] = 0x0;  //end packet
    buf[3] = 0x0;  //begin packet
    buf[4] = 0x4;  //command packet
    buf[5] = 0x0;  //end packet
    buf[6] = 0x0;  //end packet

    Write(com_port, buf, 7);
}



void Vench::SendCommStop_ave_BT()
{
    if (!fl_bluetooth)
        return;

    unsigned char buf[10];
    buf[0] = 0x0; //begin packet
    buf[1] = 0x0; //command packet
    buf[2] = 0x0; //end packet
    buf[3] = 0x0; //begin packet
    buf[4] = 0x4; //command packet
    buf[5] = 0x0; //end packet
    buf[6] = 0x0; //end packet

    Write(com_port, buf, 7);
}

void Vench::on_BBip_B_clicked() // bluetooth mode
{
    //       QString vpravo = "вправо"; //6
    //       QString vlevo  = "влево";  //9
    //       QString vpered = "вверх";  //0x5
    //       QString nazad  = "вниз";   //0xa
    //       QString svet   = "свет";   //0xb1
    //       QString signal = "сигнал"; //0xc1
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 10;
        mass_comm_n[num_comm][1] = 10;
        mass_comm_n[num_comm][2] = 0xC1;
        num_comm++;
    }
    SendCommBipBT();
}

void Vench::on_BLight_B_clicked() // bluetooth mode
{
    //       QString vpravo = "вправо"; //6
    //       QString vlevo  = "влево";  //9
    //       QString vpered = "вверх";  //0x5
    //       QString nazad  = "вниз";   //0xa
    //       QString svet   = "свет";   //0xb1
    //       QString signal = "сигнал"; //0xc1
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 10;
        mass_comm_n[num_comm][1] = 10;
        mass_comm_n[num_comm][2] = 0xB1;
        num_comm++;
    }
    SendCommLightBT();
}

void Vench::RuchnComm(QString text)
{
    text = PrivodStroki(text);
    if (fl_bluetooth)
        text += " ";
#ifdef PRINT_DEBUG
    qDebug() << text;
#endif

    if (fl_bluetooth)
    {
        unsigned char buf[1000];
        int i = 0;
        int l = text.indexOf(" ");
        while((i < 999) && (l >= 0))
        {
            bool ok;
            buf[i] = text.left(l).toInt(&ok, 16);
            if (!ok)
                --i;
            text = text.right(text.length() - l - 1);
            ++i;
            l = text.indexOf(" ");
        }

        Write(com_port, buf, i);
    }
    else
        SendCommForRS(text.toLocal8Bit().constData());


    QMessageBox::information( this,
                              trUtf8("Информация"),
                              trUtf8("Команда отправлена на выполнение\n"),
                              trUtf8("Ok") );
#ifdef PRINT_DEBUG
    qDebug("Send comm Ruchn vvod\n");
#endif
}

void Vench::on_checkLog_n_B_clicked(bool checked)
{
    fl_print_log = checked;
    fl_print_log_B = checked;
}

void Vench::on_BUp_B_pressed()
{
    //       QString vpravo = "вправо"; //6
    //       QString vlevo  = "влево";  //9
    //       QString vpered = "вверх";  //0x5
    //       QString nazad  = "вниз";   //0xa
    //       QString svet   = "свет";   //0xb1
    //       QString signal = "сигнал"; //0xc1
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 10;
        mass_comm_n[num_comm][1] = 10;
        mass_comm_n[num_comm][2] = 0x5;
        num_comm++;
    }
    SendCommForw_ave_BT();
}

void Vench::on_BDown_B_pressed()
{
    //       QString vpravo = "вправо"; //6
    //       QString vlevo  = "влево";  //9
    //       QString vpered = "вверх";  //0x5
    //       QString nazad  = "вниз";   //0xa
    //       QString svet   = "свет";   //0xb1
    //       QString signal = "сигнал"; //0xc1
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 10;
        mass_comm_n[num_comm][1] = 10;
        mass_comm_n[num_comm][2] = 0xA;
        num_comm++;
    }
    SendCommRevers_ave_BT();
}

void Vench::on_BLeft_B_pressed()
{
    //       QString vpravo = "вправо"; //6
    //       QString vlevo  = "влево";  //9
    //       QString vpered = "вверх";  //0x5
    //       QString nazad  = "вниз";   //0xa
    //       QString svet   = "свет";   //0xb1
    //       QString signal = "сигнал"; //0xc1
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 10;
        mass_comm_n[num_comm][1] = 10;
        mass_comm_n[num_comm][2] = 0x9;
        num_comm++;
    }
    SendCommLeft_ave_BT();
}

void Vench::on_BRight_B_pressed()
{
    //       QString vpravo = "вправо"; //6
    //       QString vlevo  = "влево";  //9
    //       QString vpered = "вверх";  //0x5
    //       QString nazad  = "вниз";   //0xa
    //       QString svet   = "свет";   //0xb1
    //       QString signal = "сигнал"; //0xc1
    if (fl_rec && (num_comm < NUM_COMM))
    {
        mass_comm_n[num_comm][0] = 10;
        mass_comm_n[num_comm][1] = 10;
        mass_comm_n[num_comm][2] = 0x6;
        num_comm++;
    }
    SendCommRight_ave_BT();
}

void Vench::ProgramClicked()
{
    QString txt;
    if (fl_xml)
        ReadSnap(txt);
    else
        ReadJson(txt);
    txt += " ";
    unsigned char buf[1000];
    int i = 0;
    int l = txt.indexOf(" ");
    while((i < 999) && (l >= 0))
    {
        bool ok;
        buf[i] = txt.left(l).toInt(&ok, 16);
        if (!ok)
            --i;
        txt = txt.right(txt.length() - l - 1);
        ++i;
        l = txt.indexOf(" ");
    }

     Write(com_port, buf, i);
}

int Vench::AddComm(QString cm, int pos)
{
    int min = 0;
    for(int i = 0; i < col_comm; ++i)
        if (sp_comm[i].pos < pos)
            ++min;
        else
            break;

    for(int i = col_comm; i > min; --i)
    {
        sp_comm[i].comm = sp_comm[i - 1].comm;
        sp_comm[i].comm16 = sp_comm[i - 1].comm16;
        sp_comm[i].pos = sp_comm[i - 1].pos;
    }
    sp_comm[min].comm = cm;
    sp_comm[min].comm16 = cm;
    sp_comm[min].pos = pos;
    ++col_comm;

    return min;
}

void Vench::AddCicklSnap(QDomNode node)
{
    int kk = node.toElement().text().toInt();
    for (int l = 0; l < kk - 1; ++l)
    {
        AddComm(JComm[2].comm16, nc_snap);
        ++nc_snap;
    }
    int icickl = kk;
    ParseReadSnapNodes(node, {"", "script", "s"}, 1);
    for (int t = 0; t < icickl; ++t)
    {
        AddComm(JComm[6].comm16, nc_snap);
        ++nc_snap;
    }
}

void Vench::ParseReadSnapNodes(QDomNode node, const QList<QString>& keyWords, int ind)
{
    if (ind >= keyWords.count())
        return;
    while(!node.isNull())
    {
        if (ind != 0)
            node.toElement().tagName();
        if (ind == keyWords.count() - 1)
        {
            QString attribute = node.toElement().attribute(keyWords.at(ind));
            for(int i = 0; i < NUM_COMM_SNAP; ++i)
                if (attribute == JComm[i].comm)
                {
                    AddComm(JComm[i].comm16, nc_snap);
                    ++nc_snap;
                    if (i == 2)
                        AddCicklSnap(node.firstChild());
                }
        }
        else
            if (node.toElement().tagName() == keyWords.at(ind))
                ParseReadSnapNodes(node.firstChild(), keyWords, ind+1);
        node = node.nextSibling();
    }
}

void Vench::ReadSnap(QString &txt)
{
    qDebug("read snap\n");

    QString fnamec = serverDir + f_name_snap;

    if (flConsole)
        fnamec = serverDir;
    if (flConsole == 1)
        fnamec += name_file_console;
    if (flConsole == 2)
        fnamec = name_file_console;

    qDebug() << "lll";
    qDebug() << fnamec;

    QFile f(fnamec);

    col_comm = 0;
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

    nc_snap = 0;

    QDomDocument doc;
    if (!doc.setContent(&f, true))
    {
        qDebug("Error read snap\n");
        return;
    }
    QDomNode node = doc.documentElement().firstChild();
    node.toElement().tagName();
    ParseReadSnapNodes(node, {"stage", "sprites", "sprite", "scripts", "script", "s"});
    txt = "";
    for(int i = 0; i < nc_snap; ++i)
        txt += sp_comm[i].comm16 + " ";
    if (col_comm > 0)
        txt += "8F";
    qDebug() << txt;
}

void Vench::ReadJson(QString &txt)
{
    qDebug("read json\n");
    QString fnamec = serverDir + f_name_json;
    qDebug() << f_name_json;

    QFile fl_com(fnamec);
    if (fl_com.open(QIODevice::ReadOnly) != true)
    {
        qDebug("Error open file json\n");
        return;
    }
    QTextStream tt( &fl_com);


    col_comm = 0;
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

    int fl_cicl = 0;
    int ncicl = 0;
    QString t = tt.readAll().toLocal8Bit();
    while(1)
    {
        int fl = 0;
        for (int i = 0; i < NUM_COMM_JSON; ++i)
        {
            int k = t.indexOf(JComm[i].comm);
            if (k >= 0)
            {
                ++fl;
                AddComm(JComm[i].comm16, k);
                t.remove(k, JComm[i].comm.length());
                for(int l = 0; l < JComm[i].comm.length(); ++l)
                    t.insert(k," ");
                if (JComm[i].comm == "repeat")
                {
                    QString h = t.right(t.length() - k - 7);
                    ncicl = h.left(h.indexOf(" ")).toInt();
                    qDebug("nc = %d\n", ncicl);
                    for(int j = 0; j < ncicl - 1; ++j)
                        AddComm(JComm[i].comm16, k + j);
                    fl_cicl = 1;
                }

                if (JComm[i].comm == "end")
                    if (fl_cicl > 0)
                    {
                        fl_cicl = 0;
                        for(int j = 0; j < ncicl - 1; ++j)
                            AddComm(JComm[i].comm16, k + j);
                        ncicl = 0;
                    }
            }
        }
        if (fl == 0)
            break;
    }
    txt = "";
    for(int i = 0; i < col_comm; ++i)
        txt += sp_comm[i].comm16 + " ";
    if (col_comm > 0)
        txt += "8F";
}

void Vench::SetUnsleepFlag(bool value)
{
    fl_unsleep = value;
}

void Vench::SetSpeedCar(int value)
{
    speed_car = value;
}

void Vench::SetEnergFlag(bool value)
{
    fl_energ = value;
}

void Vench::SetDimFlag(bool value)
{
    fl_dim = value;
}

void Vench::SetBarFlag(bool value)
{
    fl_bar = value;
}

void Vench::SetGrphBFlag(bool value)
{
    fl_grphB = value;
}

void Vench::SetGrphDFlag(bool value)
{
    fl_grphD = value;
}

void Vench::SetGrphEFlag(bool value)
{
    fl_grphE = value;
}

void Vench::SetGrphFlag(bool value)
{
    fl_grph = value;
}

void Vench::SetGrphOFlag(bool value)
{
    fl_grphO = value;
}

void Vench::SetGrphVlFlag(bool value)
{
    fl_grphVl = value;
}

QString Vench::ServerDir()
{
    return serverDir;
}

QString Vench::AnsStatus()
{
    qWarning("Hey!");
    if (ansStatus == 0)
        return "Канал обмена не инициализирован";
    if (ansStatus == 1)
        return "Канал обмена открыт успешно";
    if (ansStatus == 2)
        return "Устройство ответило";
    return "";
}

QColor Vench::AnsStatusColor()
{
    qWarning("Hey!2");
    if (ansStatus == 0)
        return Qt::red;
    if (ansStatus == 1)
        return Qt::yellow;
    if (ansStatus == 2)
        return Qt::green;
    return Qt::white;
}
