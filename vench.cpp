#define NAME_FILE_INI "smart.ini"
#define NAME_FILE_VAR_NEW "script.xml"
#define LOGFILE "LOG.txt"

#define NUM_STROK 500

#include "vench.h"
#include "channel.h"

#include <fcntl.h>
#include <unistd.h>
#include <QtCore>
#include <QColor>

#ifdef LINUX_D
#ifndef QSERIAL
#include <termios.h>
#endif
#endif

Vench::Vench(QString kat, QString prt, QObject* parent) :
    QObject(parent)
{
    fl_kat = 0;
    fl_port_s = 0;

    mSleep(100);

    if (!kat.isEmpty())
        SetKatalog(kat);
    if (!prt.isEmpty())
        SetPort(prt);
    init();
}

Vench::~Vench()
{
    COMClose();
}

void Vench::init()
{
    f_name_xml = NAME_FILE_VAR_NEW;
    flcomport = 0;
    fl_print_log = false;
    addr = "/dev/ttyS0";
    ansStatus = 0;

    QString mEdit;
    char* var = getenv("VISUAL");
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
    ReadIni();

    mSleep(100);
    fl_rec = 0;
    fl_play = 0;
    mass_comm_n.clear();

    qDebug("ustroistvo vvoda-vivoda:  %s\n", addr.toLocal8Bit().constData()); // smotrim kakoe ustroistvo otkrilos

    if ((flcomport == 0) && (COMInit(addr.toLocal8Bit().constData()) != 1))
    {
        qDebug("Error open port Exit\n");
        QString st = trUtf8("Ошибка инициализации порта: ") + addr;
        /*!!!!
         *QMessageBox::information( this, trUtf8("Ошибка"), st, trUtf8("Ok") );*/
    }

    if (flcomport != 0)
        ansStatus = 1;
    mSleep(3);
    ReadProg1();

#ifdef LINUX_D
    TChannel channel_drone;
    channel_open(&channel_drone, PROTO_UDP, WRITE_MODE, PORT_DRONE, (char*)ADDR_SHIR, NULL, NULL, NULL, NULL);
#endif
    mSleep(10);
}

void Vench::mSleep(int duration)
{
    QEventLoop loopEvent;
    QTimer::singleShot(duration, &loopEvent, SLOT(quit()));
    loopEvent.exec();
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
    flcomport = 0;
    return 1;
}

void Vench::BPressPlay()
{
    if(fl_play == 0)
    {
        ReadProg1();
        fl_rec = 0;
        fl_play = 1;

        for(int uu = 0; uu < mass_comm_n.count(); ++uu)
        {
            xml_command xml_c = mass_comm_n.at(uu);
#ifdef PRINT_DEBUG
            qDebug("m1 =%d\n", xml_c.imp1);
            qDebug("m2 =%d\n", xml_c.imp2);
            qDebug("m3 =%d\n", xml_c.napr);
#endif
            DoCommand(CommandsCode.key(xml_c.napr, None));
            mSleep(1000);
        }
    }
    else
    {
        fl_play = 0;
         on_BStop_B_clicked();
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

void Vench::WriteProg1( int k )
{
    QDomDocument md;
    QDomElement root = md.createElement("bim_xml");
    QDomNode node = md.createElement("prog");
    for(int i = 0; i < k; ++i)
    {
        QDomElement node1 = md.createElement("pole");
        xml_command xml_c = mass_comm_n.at(i);
        node1.setAttribute("imp1", QString().setNum(xml_c.imp1, 10));
        node1.setAttribute("imp2", QString().setNum(xml_c.imp2, 10));
        node1.setAttribute("napr", QString().setNum(xml_c.napr, 16));
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
    mass_comm_n.clear();
    QString fnamec = serverDir + f_name_xml;
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
        while(!node1.isNull())
        {
            if (node1.toElement().tagName() == "pole")
            {
                xml_command xml_c(node1.toElement().attribute("imp1").toInt(),
                                  node1.toElement().attribute("imp2").toInt(),
                                  node1.toElement().attribute("napr").toInt(nullptr, 16));
                mass_comm_n.append(xml_c);
            }
            node1 = node1.nextSibling();
        }
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
#ifndef LINUX_D
    int num_sl = t.length();
    QString mtt = t.mid(num_sl-1,1);
    serverDir = t;
#endif
#ifdef LINUX_D
    if (fl_kat == 0)
    {
        serverDir = dr.homePath() + "/.config/smartcar/";
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

    if (!readIniLine(tt, t))
        return;
    f_name_xml = t;

    for (int i = 0; i < 4; ++i)
        if (!readIniLine(tt, t))
            return;
    if (!readIniLine(tt, t, "Adress\n"))
        return;
    if(!fl_port_s)
        addr = t;

    for (int i = 0; i < 3; ++i)
        if (!readIniLine(tt, t))
            return;
    #ifdef WIN_D
    if (!fl_port_s)
        addr = t;
    #endif
    fl_com.close();
}

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
        struct termios options;
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
        CreateEvent(NULL,  true,  true, NULL);
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

int Vench::Write(int port, QList<int> bt_com)
{
    qWarning() << bt_com;
    int len = bt_com.count();
    unsigned char buf[10];
    for (int i = 0; i < len; ++i)
        buf[i] = bt_com.at(i);

    print_log(buf, len, 1);

#ifdef QSERIAL
    int r = port;
    r = com_port_qts.write((char *)buf, len);
    if (r < 0)
        COMClose();
    return r;
#endif

#ifndef QSERIAL

#ifdef  LINUX_D
    ssize_t ret;
    ret = write(port, buf, len);
    int r = (int)ret;
    if (ret < 0)
        COMClose();
    printf("Write port = %d\n", r);
    return r;
#endif

#ifdef WIN_D
    int  ret = 0;
    unsigned long hh;
    DWORD signal; //temp - ����������-��������
    OVERLAPPED overlappedwr;
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
            COMClose();
        return -1;
    }
    return ret;
#endif
#endif
    return 0;
}

int Vench::print_log(unsigned char *buf, int len, int pr)
{
    if (!fl_print_log)
        return 0;

    FILE * flnl;
#ifdef WIN_D
    flnl = fopen(LOGFILE, "a+");
#endif
#ifdef LINUX_D
    QString flname = QDir().homePath() + "/.config/smartcar/" + LOGFILE;
    flnl = fopen(flname.toLocal8Bit().constData(), "a+");
#endif

    print_time(flnl);
    if (pr == 0)
        fprintf(flnl, "Read :");
    else if (pr == 1)
        fprintf(flnl, "Write:");

    for(int i = 0; i < len; ++i)
    {
        if (buf[i] > 0xF)
            fprintf(flnl, "%X ", buf[i]);
        else
            fprintf(flnl, "0%X ", buf[i]);
    }
    fprintf(flnl, "\n");
    fflush(flnl);
    fclose(flnl);
    return 1;
}


int Vench::print_time(FILE *fln)
{
    time_t ltime;
    time(&ltime);
    tm *newtime = localtime(&ltime);
    fprintf(fln, "%d:%.2d:%.2d; %.2d:%.2d:%.2d; ",
            newtime->tm_year + 1900, newtime->tm_mon + 1, newtime->tm_mday, // Day
            newtime->tm_hour, newtime->tm_min, newtime->tm_sec);            // Time
    return 1;
}

void Vench::BRec_pressed_n()
{
    if (fl_rec == 0)
    {
        fl_rec = 1;
        mass_comm_n.clear();
        return;
    }
    if (fl_rec == 1)
    {
        fl_rec = 0;
        WriteProg1(mass_comm_n.count());
        return;
    }
}

void Vench::DoCommand(Command com)
{
    qWarning() << com;
    if (fl_rec && CommandsCode.contains(com))
        mass_comm_n.append(xml_command(CommandsCode.value(com)));

    if (BluetoothCommands.contains(com))
        Write(com_port, BluetoothCommands.value(com));
}


void Vench::on_BStop_B_clicked()
{
    DoCommand(Stop);
}

void Vench::on_BBip_B_clicked()
{
    DoCommand(Bip);
}

void Vench::on_BLight_B_clicked()
{
    DoCommand(Light);
}

void Vench::on_BUp_B_pressed()
{
    DoCommand(Up);
}

void Vench::on_BDown_B_pressed()
{
    DoCommand(Down);
}

void Vench::on_BLeft_B_pressed()
{
    DoCommand(Left);
}

void Vench::on_BRight_B_pressed()
{
    DoCommand(Right);
}

void Vench::on_checkLog_n_B_clicked(bool checked)
{
    fl_print_log = checked;
}

void Vench::RuchnComm(QString text)
{
    text = text.toUpper().replace("  ", " ");
    for (int i = 0; i < 16; ++i)
    {
        QString ch = QString().setNum(i, 16).toUpper();
        text.replace(" 0" + ch, " " + ch);
    }
    text += " ";
#ifdef PRINT_DEBUG
    qDebug() << text;
#endif
    QList<int> buf;
    int l = text.indexOf(" ");
    while((buf.count() < 999) && (l >= 0))
    {
        bool ok;
        buf.append(text.left(l).toInt(&ok, 16));
        if (!ok)
            buf.removeLast();
        text = text.right(text.length() - l - 1);
        l = text.indexOf(" ");
    }

    Write(com_port, buf);

    /*!!!!
     *QMessageBox::information( this,
                              trUtf8("Информация"),
                              trUtf8("Команда отправлена на выполнение\n"),
                              trUtf8("Ok") );*/
#ifdef PRINT_DEBUG
    qDebug("Send comm Ruchn vvod\n");
#endif
}

QString Vench::AnsStatus()
{
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
    if (ansStatus == 0)
        return Qt::red;
    if (ansStatus == 1)
        return Qt::yellow;
    if (ansStatus == 2)
        return Qt::green;
    return Qt::white;
}
