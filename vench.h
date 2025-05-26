#ifndef VENCH_H
#define VENCH_H

#include "define.h"
#include "define_snap.h"

//periodicheskii send comm ne spat
#define SEND_COMM_ANTISLEEP

#ifdef QSERIAL
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include <QDialog>
#include <QThread>
#include <QComboBox>
#include <QListWidget>

#include <QDomNode>
#include <QDomDocument>

#include <stdio.h>

#ifdef LINUX_D

#ifndef QSERIAL
#include <sys/ioctl.h>
#include <termios.h>
#endif

#include <channel.h>
#endif

#ifdef WIN_D
#include <windows.h>
#endif



#include <qvariant.h>
#include <qdialog.h>
#include <qdatetime.h>


#include "formview.h"


//vrema zadergki dla posilki komandi ne spat v sec
#define PER_TIMENESPAT 260.0



const int PLOT_SIZE = 201;      // 0 to 200
#define MAX_DATA_MAS 8192
#define MAX_DATA_MAS_D 9192
#define MAX_USTR 50
#define NUM_COMM 100
#define KOL_ANSW 20
#define NMAS_KG 2000

//period oprosa com-port v sec
#define TIMER_INIT_PORT 10

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QComboBox;
class QCheckBox;
class QPushButton;
class QLineEdit;
class QTextEdit;

class QDomDocument;
class QDomElement;
class QDomNode;

#ifdef QSERIAL
class QSerialPort;
#endif

class MyThread : public QThread
{
    public:
	MyThread(void (*call_f)(void*),void *arg);
	void run();
    protected:
	void (*func)(void*);
	void* param;
	
};

struct Spisoc_ust
{
	unsigned char mac_addr[8];
	unsigned char set_addr[2];
	unsigned char parent_addr[2];
	double time_sleep;
	double p_sleep;
    double period_oprosa_t;
    double period_oprosa_d;
    double period_oprosa_o;
    double period_oprosa_e;
	double time_oprosa_t;
    double time_oprosa_d;
    double time_oprosa_e;
    double time_oprosa_o;
    int fl_izm_pt;
	int fl_izm_pd;
	int fl_izm_pe;
	int fl_oprosa;
	unsigned char pan1;
    unsigned char pan2;
    unsigned char fl_pan;
	
	int fl_antisleep;

	int impr;
	int impl;

	int vibr_sp;//priznak vibrana v spiske ili net
	
//skolko raz oprashivali
	int fl_temp;// chto sprashivaem temp ili en
	int fl_energ;// chto sprashivaem temp ili en
	double tar_kf;
	double tar_kfd;
	int fl_ans_sleep;// flag otveta na kodogrmmu usiplenia
	int fl_sleep;//priznak sna (bez sna ili spit periodicheski)
	int num_u;


};


struct Comannd{
    QString comm;
    QString comm16;
    int pos;
};

enum Dat
{
    None = -1,
    Right = 0,
    Left = 1,
};

namespace Ui {
class Vench;
}


class Vench:public QDialog//,public Ui::Vench
{
Q_OBJECT
private:
    Ui::Vench *ui;
public:

    explicit Vench(QWidget* parent, QString kat, QString prt, bool isBluetoothMode, int fKon, QString nf);

 ~Vench();

    //spisok tecushih comand s privazkoy k posicii
    struct Comannd sp_comm [100];
    int col_comm;
    Comannd JComm[NUM_COMM_SNAP];

    int nc_snap;// kol-vo komand dla snap

    QString progrPostrGRPH;
    QString progrShowGRPH;

    FormView *VvodPD;

    MyThread * thrRS;
#ifdef LINUX_D
    TChannel channel_drone; 
#endif
    void at_write (int8_t *buffer, int32_t len);
    void send_command(QString st);

    struct Spisoc_ust act_ust[100];

    bool fl_bluetooth;

	int fl_period_e;
	double t_oprosa_e;
	float period_oprosa_e;

	int fl_period;
	double t_oprosa;
	float period_oprosa_t;


	double t_oprosa_b;
	float period_oprosa_b;

        int fl_begin;

    int fl_repaint_panid;
	float period_oprosa_d;

    int def_diskr_impr;
    int def_diskr_impl;

    int mkol_ust;
    int kol_ust;
    int mas_vibr_ust[MAX_USTR];

    int mas_set_addr[50][2];
    int mas_mac_addr[50][8];

    int mac_addr_prg[8];//dla zapisi programmi

    FILE * flnd;
    int id;


    double TimeNeSpat;
    


    int mas_hid[MAX_USTR];
    int mas_fl_hid[MAX_USTR];
    int k_hid;

//1-left
//2-right
//3-fv
//4-rew
//5-light
//6-bip
//7-extr left
//8-extr-right


    int imp1;
    int imp2;
    int rimp1;
    int rimp2;
    int nD;
    int napr;

    unsigned char buf_zd[80];

    QString katalog;
    int fl_kat;
    int fl_port_s;
    int flag_com;
    int flcomport;


    int com_port;

#ifdef QSERIAL    
    QSerialPort com_port_qts;
#endif
    
#ifndef QSERIAL    
#ifdef LINUX_D
    struct termios options;
#endif
#endif


#ifdef WIN_D
    DCB dcb;
    COMMTIMEOUTS rCommTimeouts;
    HANDLE hCOM;
    OVERLAPPED overlapped; // (ReadThread)
    OVERLAPPED overlappedwr;

#endif
    int fl_potoc_rs; // flag raboti potoca
    bool fl_print_log;
    unsigned char buf_data_rs[MAX_DATA_MAS_D];// massiv dla hranenia dannih is rs (vseh)

    int kol_data;// kolichestvo dannih v massive
    char sys_com[90];

    unsigned char pbuf_data[3000],pbuf_mac[20],pbuf_set[2], pbuf_parent[2];
    int plen;
    double ptemp, penerg,posv,pdim,pbar,pvlg;
    int pi,pkk;

    int fl_clsp;//chobi ne sbrasivalsa viibranniy mashinka pri pererisovke
    int fl_vibr_ust;

//priznak zapisi data in specfile
    int pr_spec_file_t;
    int pr_spec_file_d;
    int pr_spec_file_e;

//dla opredelenia paceta
    int l;
    int fl_n;
    int l_k;

// dla algoritma usiplenia    
    float t_sn;
    int fl_al_sn;
    
    // dla upravlenia kardiogr
    int fl_krd;
    int fl_krd_start;

    FILE * ftrk;
    FILE * flnl;
    int il;

    struct tm *newtime;
    time_t ltime;
    int Y,M,D,h,m,s;
    int ret_napr;

    QString mEdit;

    int flzaprosbar;
    int flzaprosdim;
    int flzaprostemposv;

// kol-vo impulsov pri diskretnom dvigenii (iz fila)
    int imp_diskr;

    //speed
    int speed_car;

    int errte;
// startovay function potoca - obrabotchik prihoda inf po RS
    int fl_stop;
    int fl_pr_stop;
    int fl_bt_kr;
    int sh_mas1,sh_mas2;
    int fl_mas;
    int kf;
    int fl_datKas; // priznak srabat datchika kasania



    int mas_1[2000];
    int mas_2[2000];
    int mas_11[2000];
    int mas_21[2000];






    int num_comm;

    int pr_grph;
    
    int ind_tab;
    int ind_tab_n;
//закладки
    QTabWidget* tabWidget2;
    QWidget* tab_6;
    QWidget* tab_6_n;
    QListWidget * listV;
    QListWidget * list_PANID;
    QListWidget * list_vib;

    QString prog_for_read_win;
    QLineEdit* TLEEthKgd;
    QLineEdit* TLEEthKgdAT;
    QLineEdit* TLEEthKgdAT_CH;
    QLineEdit* TLEEthKgdAT_CH2;
    QPushButton* BSendEth;
    QPushButton* BSendEthAT;


    int fl_ans3_ust;

    QComboBox* SpActDatch;
    QLineEdit* CommText;

    QPushButton* BUp;
    QPushButton* BDown;
    QPushButton* BRight;
    QPushButton* BLeft;
    QPushButton* BStop;
    QPushButton* BLight;
    QPushButton* BBip;
    QPushButton* BAntiSleep;
    QPushButton* BRec;
    QCheckBox* checkVibrUst;
    QPushButton* BPlay;

    virtual double GetTime();

public slots:

    //vozvrashaet nomer ustr v massive po ego mac-adresu
    int GetNumberUst(unsigned char *mac);

    virtual void SetKatalog(QString);
    virtual void SetPort(QString);

    virtual void SbrosDatKas();

    virtual void DatKas(Dat dat);

    virtual QString PrivodStroki(QString st);
    
    virtual void SpVibr();
    virtual void SpVibr_n();
    virtual void ProcessTimer();
    virtual void ClearSpBeg();

    int mSleep(int zd);
    
    virtual void BPressLog();
    virtual void BPressParam();
    virtual void BPressBDiscrImp_Ever();
    void grph(int pr, QString fileName);
    virtual void BPressGrph();
    virtual void BPressGrphE();
    virtual void BPressGrphO();
    virtual void BPressGrphD();
    virtual void BPressGrphB();
    virtual void BPressGrphV();

    //ndt ==1 - print temp
    //ndt ==2 - print energ
    //ndt ==3 - print osv
    //ndt ==4 - print dim
    //ndt ==5 - print bar
    //ndt ==6 - print vlag
    virtual void PaintTxtData(int ndt,double dt);

    virtual void BPressTemp();
    virtual void BPressBar();
    virtual void BPressEnerg();
    virtual void BPressDim();
    virtual void BPressIzmPANID();
    virtual void BPressIzmPANID_KONTR();
    virtual void BPressZapPANID();

    virtual void BPressStop();
    virtual void BRealeseStop();
    virtual bool BPressRec(int listCount);
    virtual void BRealeseRec();
    virtual void BPressPlay();
    virtual void BPressLight();
    virtual void SendCommLight1();
    virtual void BPressProdol();

    virtual int SendVklNP();
    virtual int SendVklAPI();//regim api bez esc
    virtual int SendVklAPI(int api_e);//api_e==1 - vkl api bez esc? api_e==2 - vkl api s esc? api+e==0 - otkl esc

    virtual void BPressSendEth();
    virtual void BPressSendEthAT();

    virtual void SendCommAntiSon(int *,int *);
    virtual void BReleaseLight();
    virtual void SendCommStopLight1();
    virtual void BPressBip();
    virtual void SendCommBip1();
    virtual void BReleaseBip();
    virtual void SendCommStopBip1();
    virtual void BPressRight();
    virtual void BPressLeft();
    virtual void BReleaseLeft();
    virtual void BReleaseRight();
    virtual void BPressFw();
    virtual void BReleaseFw();
    virtual void BPressRv();
    virtual void BReleaseRv();

    void FormatAndSendComm(QString cm, int mk[10], QString postfix_cm = "");
    virtual void SendComm( QString  hcm );
    virtual void ObSpActUst();
    virtual void SendCommObnovit();
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void init();
    virtual void destroy();
    virtual void keyReleaseEvent( QKeyEvent * e );
    virtual void SendCommTempOff();
    virtual void SendCommTempOn();
    virtual void SendCommBarOn();
    virtual void SendCommEnergOff();
    virtual void SendCommEnergOn();
    virtual QString BaseOfCommForw1(QString cm, int mk[10]);
    virtual void SendCommForw1();
    virtual void SendCommBudil();
    virtual void SendCommStop1();
    virtual void SendCommLeft1();
    virtual void SendCommRight1();
    virtual void SendCommExtrLeft1();
    virtual void SendCommExtrRight1();
    virtual void SendCommRevers1();
    virtual void WriteProg1( int k );
    virtual void ReadProg1();
    virtual void ReadIni();
    bool readIniLine(QTextStream& tt, QString& t, QString debug_message = "");
    virtual void chLog(bool checked);
    virtual void ReadCum();

    virtual void ReadJson(QString &txt);
    virtual void chKum();
    virtual void SendCommDimOff();
    virtual void SendCommDimOn();

    virtual void ClearSp();
    virtual void SetSp(QString txt);

    int print_spec_file(QString liter, unsigned char *buf_mac_set, double data);
    int print_data(unsigned char *buf_mac_set,double data, int pr);
    int print_data_s(unsigned char *buf_mac_set,double data, int pr);

    int print_imp_sleep(unsigned char *buf_mac_set,int imp1,int imp2,int napr, int sleep);

    int print_time_s(FILE *fln);

    int Write(int port, unsigned char *buf, int len);

    int COMClose();

    int COMInit(const char *port); //funkcia inicilizacii COM porta - parametr nomer porta
    int try_read_port();
    int GetKadrN(unsigned char * buf_s,int *len);

    int GetDim(unsigned char *buf,int len, double *temp, double trk);
    int GetBar(unsigned char *buf,int len, double *temp,double *vlg, double trk);

    int print_time (FILE *fln);

    int TryDatKas(unsigned char *buf,int len, Dat &dt);
    int printKG(int flf);
    int RemoveUstr(int  k);

    int TryGetMash(unsigned char *buf, int len);
    int TryGetTemp(unsigned char *buf, int len);
    int TryGetPANID(unsigned char *buf, int len);
    int GetPANID(unsigned char *buf,int len, unsigned char *pan1,unsigned char *pan2);

    int SendZapisPanID(unsigned char *Mac,int pn);
    int SendZapisPanID_KONTR(int pn);

    int SetKolImpUstr(unsigned char *Mac,int impr,int impl);// zapominaet kol-vo impulsov dla kazdogo dvigatela mashinki zadannih polzavatelem
    int GetKolImpUstr(int *Mac,int *impr,int *impl);// vozvrashet kol-vo impulsov dla kazdogo dvigatela mashinki zadannih polzavatelem

    int SaveUstrINI(unsigned char *Mac);// sohranaet v file nastroiki dla kagdogo ustroistva
    int ReadUstrINI(unsigned char *Mac);

    int GetImp(unsigned char *buf,int len, int pr, int * imp1, int *imp2, int *napr);

    int SetParentUst();
    int RetParUst(unsigned char *pu,unsigned char *ppu);

    int SendSetAlarm(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendZaprosSleep(unsigned char *MacSet,double tm_slp);// zapros konkretnogo ustroistva energii
    int SendSetSleep(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendZaprosComm(unsigned char *MacSet,unsigned char *cm, int len);// zapros konkretnogo ustroistva

    int SendDriveStop(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendDriveStart(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendZaprosI();
    int NewActUst();
    int GetTemp(unsigned char *buf,int len, double *temp,double *osv, double trk);
    int GetEnerg(unsigned char *buf,int len, double *temp);
    int GetMacSetAddr(unsigned char *buf,unsigned char *mac,unsigned char *set, unsigned char *parent);
    int SendZaprosPANIDAll();
    int SendZaprosTAll();//shirokoveshat zapros temp
    int SendZaprosEAll();//shirokoveshat zapros en
    int SendZaprosDim(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendZaprosOtklDim(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendZaprosT(unsigned char *MacSet);// zapros konkretnogo ustroistva
    int SendZaprosE(unsigned char *MacSet);// zapros konkretnogo ustroistva energ
    int InitKdg(unsigned char *buf);
    int readbuf();// chtenie data is prota
    int print_log(unsigned char *buf,int len,int pr);
    int print_nastr_file(int pr=0);

    int SendCommStartKrd(unsigned char *MacSet);
    int SendCommStopKrd(unsigned char *MacSet);
    int TryGetKrdBegin(unsigned char *buf,int len);
    void SetActiveLog(unsigned char* mac_ad,unsigned char *mas_set);
    int SetAnswerUstr(int imp1, int imp2,int nD);

    void SetHiddenCar(int i, int fl);

    void ParseReadSnapNodes(QDomNode node, const QList<QString>& keyWords, int ind = 0);
    void ReadSnap(QString &txt);

    void AddCicklSnap(QDomNode node);

    int AddComm(QString cm,int pos);

    static void TestPotoc(void * param);

    int SendCommForRS(const char * str);

    void NewScriptPlot(int * mac, int pr);

    int GetNumbUst( int * mac );

    void BRec_pressed_n();
protected:
    int mass_comm_n[100][3];
    int fl_play;
    int fl_rec;
    bool fl_xml;
    bool fl_allDevices;
    bool fl_diskr;
    bool fl_temp;
    bool fl_unsleep;
    bool fl_energ;
    bool fl_dim;
    bool fl_bar;
    bool fl_grphB;
    bool fl_grphD;
    bool fl_grphE;
    bool fl_grph;
    bool fl_grphO;
    bool fl_grphVl;
    int PressBip;

    int PressRh;
    int PressLf;
    int PressRv;
    int PressFw;
    int tempPress;
    int osvPress;
    int dimPress;
    int energPress;
    int shiftPress;
    int fl_obn_spact;
    int fl_comm;
    QString txtAct;
    
    FILE* fl_logm;
    
    int PanIdKontr;
    int kp;
    int numc;
    int numc1;
    QString f_name_json;
    QString f_name_snap;
    QString f_name_kum;
    QString f_name_xml;
    QString addr,addr_win;
    int n_imp_kum;
    int n_imp_kum_p;
    QTime tmFile;
    int tm_usleep_com;//zadergka megdu komandami 
    QTimer * timer;
    double timer_init_port;

    QString name_file_console;
    int flConsole;
    bool fl_print_log_B;
    QString serverDir;



 
public slots:
    void on_listV_itemSelectionChanged();
    void ProgrClicked();
    void RezultClicked();
    void on_pBTemp_E_clicked();
    void on_pBTemp_D_clicked();
    void on_pBGraph_E_clicked();
    void on_pBGraph_O_clicked();
    void on_pBGraph_D_clicked();


    void SendCommBipBT();
    void on_BBip_B_clicked();
    void on_BLight_B_clicked();
    void SendCommLightBT();
    void SendCommForw_ave_BT();
    void SendCommRevers_ave_BT();
    void SendCommLeft_ave_BT();
    void SendCommRight_ave_BT();

    void SendCommStop_ave_BT();

    void RuchnComm(QString text);
    void ProgramClicked();
    void on_checkLog_n_B_clicked(bool checked);
    void on_BUp_B_pressed();
    void on_BDown_B_pressed();
    void on_BLeft_B_pressed();
    void on_BRight_B_pressed();

private:
    int ansStatus;
public:
    void CarChanged(int fl_car);
    void SetXMLFlag(bool value);
    void SetDiskrFlag(bool value);
    void SetAllDevicesFlag(bool value);
    void SetTempFlag(bool value);
    void SetUnsleepFlag(bool value);
    void SetEnergFlag(bool value);
    void SetDimFlag(bool value);
    void SetBarFlag(bool value);
    void SetGrphBFlag(bool value);
    void SetGrphDFlag(bool value);
    void SetGrphEFlag(bool value);
    void SetGrphFlag(bool value);
    void SetGrphOFlag(bool value);
    void SetGrphVlFlag(bool value);
    QString getTextReg();
    void SetSpeedCar(int value);

    QString ServerDir();
    QString AnsStatus();
    QColor AnsStatusColor();

signals:
    void ServerDirCganged();
    void AnsStatusChanged();
    void RegGrphTextChanged(QString text);
    void DatChanged(Dat dat);
    void LTempChanged(QString text);
    void LOsvChanged(QString text);
    void LEnChanged(QString text);
    void LDimChanged(QString text);
    void LBarChanged(QString text);
    void LVlChanged(QString text);
};

#endif
