#include "formhlp.h"
#include "ui_formhlp.h"

#include <QtCore>




void formhlp::showEvent(QShowEvent* event) {

//    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

//    ui->webView->setUrl(QUrl(urlm));
    QDialog::showEvent(event);
}
formhlp::formhlp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formhlp)
{
    ui->setupUi(this);

    setWindowTitle( trUtf8( "Справка" ) );



    urlm = "";

//    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

    //ui->webView->setUrl(QUrl(urlm));
}

formhlp::~formhlp()
{
    delete ui;
}

void formhlp::SetPathFileHlp(QString pf)
{
    if (pf=="") return;
    urlm = "file:///"+pf;
    qDebug()<<urlm;
 //   ui->webView->setUrl(QUrl(urlm));
}

void formhlp::on_pushButton_clicked()
{
    this->close();
}


void formhlp::on_pushButton_2_clicked()
{

}
