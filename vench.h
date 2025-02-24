#ifndef VENCH_H
#define VENCH_H

#include "define.h"
#include "define_snap.h"

#include "formhlp.h"

//dva 95 ini odin
//#define DVA95

//kakay versia
//#define OLDD

//periodicheskii zapros panid
//#define ZAPROS_PANID_P


//periodicheskii send comm ne spat
#define SEND_COMM_ANTISLEEP

#ifdef QSERIAL
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

//#include <qapplication.h>
//#include <qnamespace.h>
//#include "ui_vench.h"
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
/* woronin
 #include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
*/

#endif

#ifdef WIN_D
#include <windows.h>
//#include <iostream.h>

//
//#include <c:\QtSDK\qwt-5.2.3\include\qwt_painter.h>
//#include <c:\QtSDK\qwt-5.2.3\include\qwt_plot_canvas.h>
//#include <c:\QtSDK\qwt-5.2.3\include\qwt_plot.h>
//#include <c:\QtSDK\qwt-5.2.3\include\qwt_plot_curve.h>

/* woronin
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
*/
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

struct Spisoc_ust{
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

namespace Ui {
class Vench;
}


class Vench:public QDialog//,public Ui::Vench
{
Q_OBJECT
private:
    Ui::Vench *ui;
public:


// Vench(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
// Vench(QWidget* parent = 0, const char* name = 0, bool modal = FALSE );

//    explicit Vench(QWidget* parent = 0,char * kat="",char * prt="",int fBT=0, int fKon=0,char * nf ="");

    explicit Vench(QWidget* parent, QString kat, QString prt, bool isBluetoothMode, int fKon, QString nf);

 ~Vench();

    //spisok tecushih comand s privazkoy k posicii
    struct Comannd sp_comm [100];
    int col_comm;
    Comannd JComm[NUM_COMM_SNAP];

    int nc_snap;// kol-vo komand dla snap


    QString fileHlp;
    QString fileBrs;// 20210426 for brouser
    int fl_altlin;//20210426 flag togo chto mi rabotaem v alt linux

    QString progrPostrGRPH;
    QString progrShowGRPH;

    FormView *VvodPD;
     
     
    MyThread * thrFile;
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
//	int fl_period_d;
//	double t_oprosa_d;
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
    int Plot_size_tecT;
    int Plot_size_tecE;
    int Plot_size_tecD;
    int Plot_size_tecV;


    double TimeNeSpat;
    


    int mas_hid[MAX_USTR];
    int mas_fl_hid[MAX_USTR];
    int k_hid;
    //28-01_14
//    int fl_hid;
//    int i_hid;
//    int fl_fl_hid;

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

    int rectH;
    int rectW;

    unsigned char buf_zd[80];

    QString katalog;
    int fl_kat;
    int fl_port_s;
    int flag_com;
    int flcomport;
    int err;

// dla automatic opredelenia com-port
    int fl_auto_succ;
    int fl_auto_wait;
    char addr_auto[40][40]; // ima ustroistva
    int sch_auto;
//end dla automatic opredelenia com-port




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
    int fl_print_log;
    unsigned char buf_data_rs[MAX_DATA_MAS_D];// massiv dla hranenia dannih is rs (vseh)
    
    double TecTimeWaitansPort;

    int fl_izm_dim;//flag izmerenia zadimlennosti
    int kol_data;// kolichestvo dannih v massive
    char sys_com[90];

    unsigned char pbuf_data[3000],pbuf_mac[20],pbuf_set[2], pbuf_parent[2];
    int plen;
//    FILE* pfl_data;
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
    int fl_datKas;/// priznak srabat datchika kasania



    int mas_1[2000];
    int mas_2[2000];
    int mas_11[2000];
    int mas_21[2000];






    int num_comm;


    formhlp formHlp;
//    int kol_ust;
//    int mas_set_addr[50][2];
//    int mas_mac_addr[50][8];

//    FormView formV;

/* woronin    QwtPlot *Mgrph;
    QwtPlot *Mgrph_n;
    //==0 - net grph
    //==1 - grph temp
    //==2 - grph energ
    //==3 - grph osv
    //==4 - grph dim
    //==5 - grph bar
    //==6 - grph vlag
    QwtPlotCurve *cRight;
    QwtPlotCurve *cRight_n; */
    int pr_grph;
        
    double d_x[PLOT_SIZE]; 
    double d_y[PLOT_SIZE]; 
    
    double d_yt[PLOT_SIZE]; 
    double d_ye[PLOT_SIZE]; 
    double d_yp[PLOT_SIZE]; 
    double d_yd[PLOT_SIZE]; 
    double d_yo[PLOT_SIZE]; 
    double d_yb[PLOT_SIZE];
    double d_yv[PLOT_SIZE];

    
    int ind_tab;
    int ind_tab_n;
//закладки
    QTabWidget* tabWidget2;
    QWidget* tab;
    QWidget* tab_2;
    QWidget* tab_3;
    QWidget* tab_4;
    QWidget* tab_5;
    QWidget* tab_6;
    QWidget* tab_6_n;
    QListWidget * listV;
    QListWidget * list_PANID;
    QListWidget * list_vib;

    QString prog_for_read_win;
    QLabel* TLabEthKgd;
    QLabel* TLabEthKgdAT;
    QLabel* TLabEthKgdAT_CH;
    QLabel* TLabEthKgdAT_CH2;
    QLineEdit* TLEEthKgd;
    QLineEdit* TLEEthKgdAT;
    QLineEdit* TLEEthKgdAT_CH;
    QLineEdit* TLEEthKgdAT_CH2;
    QPushButton* BSendEth;
    QPushButton* BSendEthAT;


    int fl_ans3_ust;
    QString ans1;
    QString ans2;
    QString ans3;
    QPalette *pl_ans_ust;
    QLabel* TLabAns_lab;
    QLabel* TLabReg;
    QLineEdit* TLabAns;

    QTextEdit* TLabLeftDat;
    QTextEdit* TLabRightDat;

    QLabel* TLabAns_labFirst;
    QLineEdit* TLabAnsFirst;
    QLabel* TextLabel2;
    QLabel* TextLabel2_3;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLabel* TextLabelSpeed;
    QComboBox* SpActDatch;
    QLabel* TextLabel2_2;
    QCheckBox* checkKumir;
    QCheckBox* checkLog;
    QCheckBox* checkDiskr;
    QCheckBox* checkCicl;
    QCheckBox* checkNeSpat;
    QPushButton* Obnovit;
    QPushButton* IzmPANID;
    QPushButton* IzmPANID_KONTR;
    QPushButton* ZapPANID;
    QLineEdit* ServerDir;
    QLineEdit* DiscrImpE;
    QLineEdit* ImpS;
    QLineEdit* CommText;
    QPushButton* BRuchnComm;
    QPushButton* BDiscrImp_Ever;
    QPushButton* BDiscrImp;
    QPushButton* BUp;
    QPushButton* BDown;
    QPushButton* BRight;
    QPushButton* BLeft;
    QPushButton* BShift;
    QPushButton* BStop;
    QPushButton* BLight;
    QPushButton* BBip;
    QPushButton* BAntiSleep;
    QCheckBox* checkAll;
    QCheckBox* checkVibrUst;
    QPushButton* BRec;
    QPushButton* BPokazProgr;
    QPushButton* BPlay;
    QPushButton* BTemp;
    QPushButton* BBar;
    QPushButton* BDim;
    QPushButton* BGrphO;
    QPushButton* BGrphVl;
    QLineEdit * LDim;
    QLabel * LbDim;
    QLineEdit * LBar;
    QLabel * LbBar;
    QLineEdit * LTemp;
    QLabel * LbTemp;
    QLineEdit * LOsv;
    QLabel * LbOsv;
    QLineEdit * LVl;
    QLabel * LbVl;
    QLineEdit * LEn;
    QLabel * LbEn;

    QPushButton* BGrphB;
    QPushButton* BGrphD;
    QPushButton* BEnerg;
    QPushButton* BGrphE;
    QPushButton* BGrph;
    QPushButton* BProdol;
    QPushButton* BRezult;
    QPushButton* BSpeed;
//    QPushButton* BRezultE;

    QPushButton* BLog;
    QPushButton* BParam;



    virtual double GetTime();
    virtual int ReadAnswer( int * imp1, int * imp2, int * naprD );

public slots:

    //vozvrashaet nomer ustr v massive po ego mac-adresu
    int GetNumberUst(unsigned char *mac);

    virtual void SetKatalog(QString);
    virtual void SetPort(QString);

    virtual void SbrosDatKas();

    virtual void DatKas(int num);//0 - right 1 - left

    virtual QString PrivodStroki(QString st);
    
    virtual void SpVibr();
    virtual void SpVibr_n();
    virtual void ChVibr();
    virtual void ProcessTimer();
    virtual void ClearSpBeg();

//    virtual void resizeGeometry();
    void resizeEvent(QResizeEvent * event);
//    void resizeEvent();
    
//for newpanel layout
    virtual void rBAll();
    virtual void RBKumir();
//end for newpanel layout

    int mSleep(int zd);
    
//    void qDebug_d(const char* format,...);
    
    virtual void BPressLog();
    virtual void BPressParam();
    virtual void BPressBDiscrImp_Ever();
    virtual void BPressDiscrImp();
    virtual void BPressSpeed();
    virtual void BPressSpeed_n();
    virtual void BPressRuchnComm();
    virtual void BPressRuchnComm_n();
    virtual void BPressGrph();
    virtual void BPressGrphE();
    virtual void BPressGrphO();
    virtual void BPressGrphD();
    virtual void BPressGrphB();
    virtual void BPressGrphV();

    virtual void chDiskr_n();
    virtual void BPressedDopParam();
    virtual void BPReleasedDopParam();

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
    
//    virtual void BRealeseTemp();
    virtual void BPressStop();
    virtual void BRealeseStop();
    virtual void BPressRec();
    virtual void BRealeseRec();
    virtual void BPressPlay();
    virtual void BPressPlay_n();
    virtual void BRealesePlay();
    virtual void BRealesePlay_n();
    virtual void BPressLight();
    virtual void SendCommLight1();
    virtual void SendCommSon();
    virtual void BPressProdol();

    virtual int SendVklNP();
    virtual int SendVklAPI();//regim api bez esc
    virtual int SendVklAPI(int api_e);//api_e==1 - vkl api bez esc? api_e==2 - vkl api s esc? api+e==0 - otkl esc

    virtual void BPressSendEth();
    virtual void BPressSendEthAT();


    virtual void SendZaprosSon(int * mak_a, int * mas_a);
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
    virtual void setTextReg();

    virtual void SendComm( QString  hcm );
    virtual void ObSpActUst();
    virtual void SendCommObnovit();
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void init();
    virtual void destroy();
    virtual void keyReleaseEvent( QKeyEvent * e );
    virtual void BShiftRel();
    virtual void ReadActiveLog();
    virtual void BShiftPress();
    static void  TestFile( void * param );
    virtual void ReadProg();
    virtual void WriteProg( int k );
    virtual void SendCommTempOff();
    virtual void SendCommTempOn();
    virtual void SendCommBarOff();
    virtual void SendCommBarOn();
    virtual void SendCommOsvOff();
    virtual void SendCommOsvOn();
//    virtual void SendCommDimOff();
//    virtual void SendCommDimOn();
    virtual void SendCommEnergOff();
    virtual void SendCommEnergOn();
    virtual void SendCommForw1();
    virtual void SendCommBudil();
    virtual void SendCommStop1();
    virtual void SendCommLeft1();
    virtual void SendCommRight1();
    virtual void SendCommExtrLeft1();
    virtual void SendCommExtrRight1();
    virtual void SendCommRevers1();
    virtual void SendComm1( int d1, int d2, int napr );
//    virtual void SendCommPrBT( int d1, int d2, int napr );
    virtual void SendCommPrBT( int d1,  int napr );
    virtual void WriteProg1( int k );
    virtual void ReadProg1();
    virtual void ReadIni();
    virtual void chLog();
    virtual void chLog_n();
    virtual void AntiSleep();
    virtual void ReadCum();

    virtual void ReadJson(QString &txt);
    virtual void chKum();
    virtual void BChCikl();
    virtual void SendCommDimOff();
    virtual void SendCommDimOn();

    virtual void ClearSp();
    virtual void SetSp(QString txt);



    void ActTab4(int index);


    int ClearMasGrph();

int print_spec_file_t(unsigned char *buf_mac_set,double data);
int print_spec_file_e(unsigned char *buf_mac_set,double data);
int print_spec_file_o(unsigned char *buf_mac_set,double data);
int print_spec_file_d(unsigned char *buf_mac_set,double data);
int print_data(unsigned char *buf_mac_set,double data, int pr);
int print_data_s(unsigned char *buf_mac_set,double data, int pr);

int print_imp_sleep(unsigned char *buf_mac_set,int imp1,int imp2,int napr, int sleep);

int print_time_s(FILE *fln);

int Write(int port, unsigned char *buf, int len);

int COMClose();

int COMInit(const char *port); //funkcia inicilizacii COM porta - parametr nomer porta
//int COMInit(QString port); //funkcia inicilizacii COM porta - parametr nomer porta
int try_read_port();
int GetKadrN(unsigned char * buf_s,int *len);

int GetDim(unsigned char *buf,int len, double *temp, double trk);
int GetBar(unsigned char *buf,int len, double *temp,double *vlg, double trk);

int print_time (FILE *fln);

int TryDatKas(unsigned char *buf,int len,int &dt);//dt==1 - pravii dt==2 - levii


int printKG(int flf);
int RemoveUstr(int  k);


int TryGetMash(unsigned char *buf, int len);

int TryGetTemp(unsigned char *buf, int len);
int TryGetPANID(unsigned char *buf, int len);
int GetPANID(unsigned char *buf,int len, unsigned char *pan1,unsigned char *pan2);

int SendZaprosPANID(unsigned char *Mac);

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
int SendSetSlpPZU(unsigned char *MacSet);// zapros
int SendSetSleep(unsigned char *MacSet);// zapros konkretnogo ustroistva
int SendZaprosComm(unsigned char *MacSet,unsigned char *cm, int len);// zapros konkretnogo ustroistva

int SendDriveStop(unsigned char *MacSet);// zapros konkretnogo ustroistva

int SendDriveStart(unsigned char *MacSet);// zapros konkretnogo ustroistva
int SendZaprosI();
int SendZaprosReset();
int NewActUst();
int GetTemp(unsigned char *buf,int len, double *temp,double *osv, double trk);

int GetOsv(unsigned char *buf, int len, double *temp,double trk);
int GetEnerg(unsigned char *buf,int len, double *temp);
int GetMacSetAddr(unsigned char *buf,unsigned char *mac,unsigned char *set, unsigned char *parent);
int SendZaprosPANIDAll();
int SendZaprosTAll();//shirokoveshat zapros temp
int SendZaprosEAll();//shirokoveshat zapros en
int SendZaprosDim(unsigned char *MacSet);// zapros konkretnogo ustroistva
int SendZaprosOtklDim(unsigned char *MacSet);// zapros konkretnogo ustroistva
int SendZaprosT(unsigned char *MacSet);// zapros konkretnogo ustroistva
int SendZaprosE(unsigned char *MacSet);// zapros konkretnogo ustroistva energ
int SendZaprosO(unsigned char *MacSet);// zapros konkretnogo ustroistva energ
int InitKdg(unsigned char *buf);
int readbuf();// chtenie data is prota
int print_log(unsigned char *buf,int len,int pr);
int print_nastr_file(int pr=0);
//int SendZaprosP(int i);

int SendCommStartKrd(unsigned char *MacSet);
int SendCommStopKrd(unsigned char *MacSet);
int TryGetKrdBegin(unsigned char *buf,int len); 
void SetActiveLog(unsigned char* mac_ad,unsigned char *mas_set);
int SetAnswerUstr(int imp1, int imp2,int nD);


void SetHiddenCar(int i, int fl);
int ReadTarFile();


void ReadSnap(QString &txt);

void AddCicklSnap(QDomNode node);

int AddComm(QString cm,int pos);


static void TestPotoc(void * param);
//void TestPotoc();

int SendCommForRS(const char * str);


void NewScriptPlot(int * mac, int pr);


int GetNumbUst( int * mac );

    void BRec_pressed_n();
    void BRec_released_n();


protected:
    int mass_comm_n[100][3];
    int fl_play;
    int PressLight;
    int PressBip;
    int fl_rec;
    int PressRh;
    int PressLf;
    int PressRv;
    int PressFw;
    int tempPress;
    int osvPress;
    int dimPress;
    int energPress;
    int shiftPress;
    int fl_al;
    int fl_obn_spact;
    int fl_comm;
    QString txtAct;
    
    FILE* fl_logm;
    
    int PanIdKontr;
    int kp;
    int fl_potoc;
    int* mas_comm;
    int numc;
    double TimeActiveLog;
    double *tim_comm;
    double b_tim_comm;
    double e_tim_comm;
    int fl_play_comm;
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
      int fl_print_log_B;
protected slots:
    virtual void languageChange();



 
private slots:
    void on_BCar_1_clicked();
    void on_BCar_2_clicked();
    void on_BLeft_pressed();
    void on_BLeft_released();
    void on_BRight_pressed();
    void on_BRight_released();
    void on_BUp_pressed();
    void on_BUp_released();
    void on_BDown_pressed();
    void on_BDown_released();
    void on_BStop_pressed();
    void on_BStop_released();
    void on_BBip_pressed();
    void on_BBip_released();
    void on_BLight_pressed();
    void on_BLight_released();
    void on_BAntiSleep_clicked();
    void on_Obnovit_clicked();
    void on_Obnovit_2_clicked();
    void on_BAntiSleep_2_clicked();
    void on_BLight_2_released();
    void on_BLight_2_pressed();
    void on_BBip_2_released();
    void on_BBip_2_pressed();
    void on_BStop_2_released();
    void on_BStop_2_pressed();
    void on_BDown_2_released();
    void on_BDown_2_pressed();
    void on_BUp_2_released();
    void on_BUp_2_pressed();
    void on_BRight_2_released();
    void on_BRight_2_pressed();
    void on_BLeft_2_released();
    void on_BLeft_2_pressed();
    void on_checkKumir_stateChanged(int arg1);
    void on_listV_itemSelectionChanged();
    void on_checDiskr_stateChanged(int arg1);
    void on_checkAll_stateChanged(int arg1);
    void on_pBPlay_pressed();
    void on_pBPlay_released();
    void on_pBRec_pressed();
    void on_pBRec_released();
    void on_pBProgr_clicked();
    void on_pBTemp_clicked();
    void on_pBGraph_clicked();
    void on_pBRezult_clicked();
    //void on_pBRezultE_Qclicked();
    void on_pBPanIdContr_clicked();
    void on_pBPanId_clicked();
    void on_pBTemp_pressed();
    void on_pBTemp_E_clicked();
    void on_pBTemp_D_clicked();
//    void on_pBTemp_O_clicked();
    void on_pBGraph_E_clicked();
    void on_pBGraph_O_clicked();
    void on_pBGraph_D_clicked();


    void SendCommBipBT();
    void on_BBip_B_clicked();
    void on_BLight_B_clicked();
    void SendCommLightBT();
    void on_BUp_B_clicked();
    void SendCommForw_ave_BT();
    void SendCommRevers_ave_BT();
    void SendCommLeft_ave_BT();
    void SendCommRight_ave_BT();

    void SendCommStop_ave_BT();

    void on_BLeft_B_clicked();
    void on_BRight_B_clicked();
    void on_BRuchnComm_n_B_clicked();
    void on_checkLog_n_B_clicked(bool checked);
    void on_CommText_n_B_returnPressed();
    void on_BUp_B_pressed();
    void on_BUp_B_released();
    void on_BDown_B_pressed();
    void on_BDown_B_released();
    void on_BDown_B_clicked();
    void on_BLeft_B_pressed();
    void on_BLeft_B_released();
    void on_BRight_B_pressed();
    void on_BRight_B_released();
    void on_BPlay_n_clicked();
    void on_BPokazProgr_n_B_clicked();
    void on_BPlay_n_B_pressed();
    void on_BProdol_n_B_clicked();
    void on_checkCicl_n_B_clicked();
    void on_BRec_n_B_pressed();
    void on_rBXML_n_2_clicked();
    void on_rBKumir_n_B_clicked();
    void on_BProgram_n_B_clicked();
    void on_BStop_B_released();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButtonHelp_clicked();
    void on_pushButton_3_clicked();
};

#endif
