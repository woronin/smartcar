#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bluetoothmode.h"

MainWindow::MainWindow(QString kat, QString prt, bool isBluetoothMode, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setAcceptDrops(false);
    setFocusPolicy(Qt::ClickFocus);

    m_vench = new Vench(kat, prt, isBluetoothMode);
    ui->centralwidget->layout()->addWidget(new BluetoothMode(m_vench, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
