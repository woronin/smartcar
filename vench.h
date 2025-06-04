#ifndef VENCH_H
#define VENCH_H

#include "define.h"

#include <QDomNode>
#include <QObject>
#include <QMap>

#ifdef QSERIAL
#include <QSerialPort>
#include <QSerialPortInfo>
#endif
#ifdef WIN_D
#include <windows.h>
#endif
#ifdef QSERIAL
class QSerialPort;
#endif

struct xml_command
{
public:
    int imp1;
    int imp2;
    int napr;
    xml_command(int _napr)
    {
        imp1 = 10;
        imp2 = 10;
        napr = _napr;
    }

    xml_command(int _imp1, int _imp2, int _napr)
    {
        imp1 = _imp1;
        imp2 = _imp2;
        napr = _napr;
    }
};

enum Command
{
    Up,
    Down,
    Left,
    Right,
    Bip,
    Light,
    Stop,
    None,
};

const static QMap<Command, int> CommandsCode
{
    {Up, 0x5},
    {Down, 0xA},
    {Left, 0x9},
    {Right, 0x6},
    {Bip, 0xC1},
    {Light, 0xB1},
};

const static QMap<Command, QList<int>> BluetoothCommands
{
    {Up, {0x0, 0x0, 0x81, 0x0, 0x4, 0x0, 0x0}},
    {Down, {0x0, 0x0, 0x7F, 0x0, 0x4, 0x0, 0x0}},
    {Left, {0x0, 0x81, 0x0, 0x0, 0x4, 0x0, 0x0}},
    {Right, {0x0, 0x7F, 0x0, 0x0, 0x4, 0x0, 0x0}},
    {Bip, {0x8e, 0x7, 0x8f}},
    {Light, {0x8e, 0xF, 0x8f}},
    {Stop, {0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0}}
};

class Vench : public QObject
{
    Q_OBJECT

public:
    explicit Vench(QString kat, QString prt, QObject* parent = nullptr);
    ~Vench();
    QString AnsStatus();
    QColor AnsStatusColor();
    void BPressPlay();
    void BRec_pressed_n();
    void RuchnComm(QString text);

protected:
    QList <xml_command> mass_comm_n;
    int fl_play;
    int fl_rec;
    int fl_kat;
    int fl_port_s;
    int flcomport;
    int com_port;
    bool fl_print_log;
    int ansStatus;

    QString f_name_xml;
    QString addr;
    QString serverDir;
    QString katalog;

#ifdef QSERIAL
    QSerialPort com_port_qts;
#endif
#ifdef WIN_D
    DCB dcb;
    COMMTIMEOUTS rCommTimeouts;
    HANDLE hCOM;
#endif
    void SetKatalog(QString);
    void SetPort(QString);
    void mSleep(int duration);
    void init();
    void WriteProg1( int k );
    void ReadProg1();
    void ReadIni();
    bool readIniLine(QTextStream& tt, QString& t, QString debug_message = "");
    int Write(int port, QList<int> bt_com);
    int COMClose();
    int COMInit(const char *port); //funkcia inicilizacii COM porta - parametr nomer porta
    int print_time(FILE *fln);
    int print_log(unsigned char *buf,int len,int pr);

    void DoCommand(Command com);

public slots:
    void on_checkLog_n_B_clicked(bool checked);
    void on_BUp_B_pressed();
    void on_BDown_B_pressed();
    void on_BLeft_B_pressed();
    void on_BRight_B_pressed();
    void on_BBip_B_clicked();
    void on_BLight_B_clicked();
    void on_BStop_B_clicked();
};

#endif
