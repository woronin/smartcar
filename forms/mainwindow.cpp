#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bluetoothmode.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QUrl>
#include <QShortcut>

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
        QMessageBox::critical(this, title, text, buttonText);
    });
    ui->centralwidget->layout()->addWidget(new BluetoothMode(m_vench, this));
    m_vench->PortConnected();
    connect(ui->actionGuide, &QAction::triggered, this, &MainWindow::OpenGuideDoc);
    connect(ui->actionReference, &QAction::triggered, this, &MainWindow::OpenReference);
    (new QShortcut(QKeySequence(Qt::Key_H), this, SLOT(OpenGuideDoc())))->setAutoRepeat(false);
}

void MainWindow::OpenGuideDoc()
{
    QString helpFilePath = QCoreApplication::applicationDirPath() + "/umkiguide.pdf";
    if (QFileInfo(helpFilePath).exists())
        QDesktopServices::openUrl(QUrl(helpFilePath, QUrl::TolerantMode));
    else
        QDesktopServices::openUrl(QUrl("https://www.umkikit.ru/prog/umkiguide.pdf"));
}

void MainWindow::OpenReference()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Справка");
    msgBox.setText("Горячие клавиши");
    QString informativeText =
              QString("H - Документация\n")
            + QString("W - Команда \"Вперед\"\n")
            + QString("A - Команда \"Влево\"\n")
            + QString("S - Команда \"Назад\"\n")
            + QString("D - Команда \"Вправо\"\n")
            + QString("B - Команда \"Сигнал (Бип)\"\n")
            + QString("L - Команда \"Свет\"\n")
            + QString("Space (Пробел) - Команда \"Стоп\"\n")
            + QString("R - Запись\n")
            + QString("P - Воспроизведение\n");

    msgBox.setInformativeText(informativeText);
    msgBox.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
