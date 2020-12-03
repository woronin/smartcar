/****************************************************************************
** Form interface generated from reading ui file 'formview.ui'
**
** Created: 2011-06-10T22:45:59
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORMZASTAV_H
#define FORMZASTAV_H

#include <qvariant.h>
#include <qdialog.h>
#include "qlabel.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QLineEdit;
class QImage;

class FormZastav : public QDialog
{
    Q_OBJECT

public:
//    FormZastav (QWidget* parent = 0, char *kat="");
    FormZastav (QWidget* parent , char *kat);
    ~FormZastav();

    QLabel*TextLabel;
    QLabel*TextLabel1;
    char katalog[100];
    //QImage im;
public slots:
    virtual void init();
    virtual void destroy();
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );
//    virtual void keyPressEvent(  );
//    virtual void mousePressEvent(  );
    virtual void SetKatalog(char*);

protected:

protected slots:
    virtual void languageChange();

};

#endif // FORMZASTAV_H
