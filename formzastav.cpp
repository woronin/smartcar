
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextcodec.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "formzastav.h"
#include "vench.h"
#include <QFile>
#include <QtDebug>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QLineEdit;
class QImage;
/*
 *  Constructs a FormView as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FormZastav::FormZastav( QWidget* parent, char * kat)
    : QDialog( parent)
{

    setMinimumSize( QSize( 600, 370 ) );
    setMaximumSize( QSize( 600, 370 ) );
    setAcceptDrops( false );
    setSizeGripEnabled( false );
    setModal( false );
    //QTextCodec::setCodecForTr(QTextCodec::codecForName ("Windows-1251"));

    QPixmap zs;
    QString ktl(kat);
    
    if (ktl=="") {
#ifdef LINUX_D
	ktl = "/usr/share/smartcar/";
	
        ktl =ktl + "umki_logo.png";
        QFile mfl(ktl);
	
        if (!mfl.exists()){
	
            ktl ="umki_logo.png";
        }
#endif
#ifdef WIN_D
            ktl ="umki_logo.png";
#endif        
	
    }else    {
	ktl = ktl+"umki_logo.png";
	
    }
//    qDebug()<<ktl;
//20201119    zs.load(ktl);
zs.load(":image/image/umki_logo.png");


    TextLabel = new QLabel( trUtf8(""),this );
    TextLabel->setGeometry( QRect( 0, 0, 600, 340 ) );
    TextLabel->setPixmap(zs);

    TextLabel1 = new QLabel( tr("            Н А Ж М И Т Е             Л Ю Б У Ю          К Л А В И Ш У"),this );
    TextLabel1->setGeometry( QRect( 100, 0, 600, 710 ) );

    languageChange();


    resize( QSize(300, 200).expandedTo(minimumSizeHint()) );
    // signals and slots connections
    init();
}


void FormZastav::SetKatalog(char*kat)
{
    strcpy(katalog,kat);
}

/*
 *  Destroys the object and frees any allocated resources
 */
FormZastav::~FormZastav()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FormZastav::languageChange()
{
//    setCaption( tr( "FormZastav" ) );

    setWindowTitle( trUtf8( "РџСѓР»СЊС‚ РЈРњР Р" ) );
//    TextLabel1->setText( tr( "Нажмите любую клавишу" ) );
}





void FormZastav::init()
{
    //im.setText("Presss any key");
}


void FormZastav::destroy()
{
 //delete PixmapLabel;
}

    

///void FormZastav::mousePressEvent(  )
//{
//    close() ;
//}



//void FormZastav::keyPressEvent(  )
//{
//    close() ;
//}


void FormZastav::mousePressEvent( QMouseEvent *  )
{

    close() ;
}



void FormZastav::keyPressEvent( QKeyEvent *e )
{
    Q_UNUSED (e);

    close() ;
}

