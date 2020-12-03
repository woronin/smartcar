/****************************************************************************
** Form interface generated from reading ui file 'formview.ui'
**
** Created: 2011-06-10T22:45:59
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORMVIEW_H
#define FORMVIEW_H

#include <qvariant.h>
#include <qdialog.h>
#include <qtextcodec.h>
#include "qlabel.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QLineEdit;

class FormView : public QDialog
{
    Q_OBJECT

public:
    FormView( QWidget* parent = 0);
    ~FormView();

    QLineEdit* EImpRight;
    QLineEdit* EImpLeft;



    QPushButton* BExit;
    QPushButton* BOk;
    QLineEdit* EnewPanID;
    QLabel* ImpRLabel;
    QLabel* ImpLLabel;
    QLabel* TextLabel;
    QLabel* TextLabel1;
    
    int type_d;

    virtual int GetPanID();
    virtual int GetKolImp(int * impr,int *impl);
public slots:
    virtual void SetType( QString name_f ,int tp);
    virtual void init();
    virtual void destroy();
//    virtual void mouseReleaseEvent( QMouseEvent * event );
//    virtual void mousePressEvent( QMouseEvent * e );
//    virtual void mouseMoveEvent( QMouseEvent * e );

protected:
    int down;
    int koeff;
    QString name_F;
    QPoint bps;
    QPoint eps;


protected slots:
    virtual void languageChange();

};

#endif // FORMVIEW_H
