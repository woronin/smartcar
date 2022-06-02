
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>

#include "formview.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QLineEdit;
/*
 *  Constructs a FormView as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
FormView::FormView( QWidget* parent)
    : QDialog( parent)
{

//    #ifdef WIN_D
 //   QTextCodec::setCodecForTr(QTextCodec::codecForName ("Windows-1251"));
//    #endif


    EImpRight = new QLineEdit(  "",this );
    EImpLeft = new QLineEdit(  "",this );


//    EImpRight->setEnabled( true );
    EImpRight->setEnabled( true );
    EImpRight->setGeometry( QRect( 20, 130, 50, 22 ) );
    EImpRight->hide();


    EImpLeft->setEnabled( true );
    EImpLeft->setGeometry( QRect( 20, 100, 50, 22 ) );
    EImpLeft->hide();

    type_d=0;
    
    BExit = new QPushButton ("BExit" ,this);
    BExit->setGeometry( QRect( 180, 160, 104, 29 ) );

    BOk = new QPushButton ("BOk" ,this);
    BOk->setGeometry( QRect( 10, 160, 104, 29 ) );


    EnewPanID = new QLineEdit(  "",this );
    EnewPanID->setEnabled( true );
    EnewPanID->setGeometry( QRect( 20, 105, 50, 22 ) );

    ImpRLabel = new QLabel( trUtf8("Правый"),this );
    ImpRLabel->setGeometry( QRect( 75, 130, 60, 25 ) );
    ImpRLabel->hide();
    
    ImpLLabel = new QLabel( trUtf8("Левый"),this );
    ImpLLabel->setGeometry( QRect( 75, 100, 60, 25 ) );
    ImpLLabel->hide();

    TextLabel = new QLabel( trUtf8(""),this );
    TextLabel->setGeometry( QRect( 20, 20, 275, 30 ) );
    
    TextLabel1 = new QLabel( trUtf8("мак-адрес"),this );
    TextLabel1->setGeometry( QRect( 20, 55, 275, 40 ) );

//chtobi pravilno perehodil Tab-m
    setTabOrder(EImpLeft,EImpRight);
    setTabOrder(EImpRight,EnewPanID);
    setTabOrder(EnewPanID,BOk);
    setTabOrder(BOk,BExit);



    languageChange();
    resize( QSize(300, 200).expandedTo(minimumSizeHint()) );
    // signals and slots connections
    connect( BExit, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( BOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
FormView::~FormView()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FormView::languageChange()
{
//    setCaption( tr( "FormView" ) );

    setWindowTitle( trUtf8( "Ввод PanID" ) );
    TextLabel->setText( trUtf8( "Введите новый PanID для устройства" ) );
    BExit->setText( trUtf8( "Выход" ) );
    BOk->setText( trUtf8( "Применить" ) );
}



void FormView::SetType( QString name_f , int tp)
{
    TextLabel1->setText( name_f );
    type_d=tp;

    if (type_d==0){
        setWindowTitle( trUtf8( "Ввод PanID" ) );
        TextLabel->setText( trUtf8( "Введите новый PanID для устройства" ) );
        EnewPanID->show();
        EImpRight->hide();
	EImpLeft->hide();
        ImpRLabel->hide();
	ImpLLabel->hide();
    
    }
    if (type_d==1){
        setWindowTitle( trUtf8( "Ввод числа импульсов" ) );
        TextLabel->setText( trUtf8( "Введите число импульсов для\n каждого двигателя  устройства" ) );
        EnewPanID->hide();
        EImpRight->show();
	EImpLeft->show();
        ImpRLabel->show();
	ImpLLabel->show();
    
    }
}

int FormView::GetKolImp(int * impr,int *impl)
{
    bool ok;
//    qDebug("!!!!!!!!!!!!!!!1\n");
    *impr=0;
    *impl=0;
    QString ss = EImpRight->text();
    int hex = 0;
    hex=ss.toInt(&ok,16);
    if (ok==false){
            QMessageBox::information( this, trUtf8("Ошибка ввода"),
                                       trUtf8("Введено ошибочное значение"),
                                       trUtf8("Ok") );
	    return 0;
    }
    *impr=hex;

    ss = EImpLeft->text();
    hex = 0;
    hex=ss.toInt(&ok,16);
    if (ok==false){
            QMessageBox::information( this, trUtf8("Ошибка ввода"),
                                       trUtf8("Введено ошибочное значение"),
                                       trUtf8("Ok") );
	    return 0;
    }
    *impl=hex;
    
    return 1;
}


int FormView::GetPanID()
{
    bool ok;
//    qDebug("!!!!!!!!!!!!!!!1\n");
    QString ss = EnewPanID->text();
    int hex = 0;
    hex=ss.toInt(&ok,16);
    if (ok==false){
            QMessageBox::information( this, trUtf8("Ошибка ввода"),
                                       trUtf8("Введено ошибочное значение"),
                                       trUtf8("Ok") );

    }
    return hex;;
}


void FormView::init()
{
    
}


void FormView::destroy()
{
 //delete PixmapLabel;
}

////zapuskaetsa kogde bila otpushena knopka mishi
//void FormView::mouseReleaseEvent( QMouseEvent * event )
//{
//    
//}

//// vizivaetsa kogda bila nagataknopka mishi
//void FormView::mousePressEvent( QMouseEvent *e )
//{
//}     


//void FormView::mouseMoveEvent( QMouseEvent *e )
//{    
//}
