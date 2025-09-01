#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bluetoothmode.h"
#include <QMessageBox>

MainWindow::MainWindow(QString kat, QString prt, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setAcceptDrops(false);
    setFocusPolicy(Qt::ClickFocus);
    m_vench = new Vench(kat, prt);
    connect(m_vench, &Vench::informationMessage, this, [this](QString title, QString text, QString buttonText)
    {
        QMessageBox::information(this, title, text, buttonText);
    });
    ui->centralwidget->layout()->addWidget(new BluetoothMode(m_vench, this));
    m_vench->PortConnected();
}

MainWindow::~MainWindow()
{
    delete ui;
}
