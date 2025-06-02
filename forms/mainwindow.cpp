#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bluetoothmode.h"
#include "zigbeemode.h"
#include "basemode.h"
#include "supermode.h"

MainWindow::MainWindow(QString kat, QString prt, bool isBluetoothMode, int fKon, QString nf, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setAcceptDrops(false);
    setFocusPolicy(Qt::ClickFocus);

    m_vench = new Vench(kat, prt, isBluetoothMode, fKon, nf);

    if (isBluetoothMode)
        ui->centralwidget->layout()->addWidget(new BluetoothMode(m_vench, this));
    else
        ui->centralwidget->layout()->addWidget(new ZigBeeMode(m_vench, this));
        //ui->centralwidget->layout()->addWidget(new SuperMode(m_vench, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
