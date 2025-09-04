#include "bluetoothmode.h"
#include "ui_bluetoothmode.h"

#include <QKeyEvent>
#include <QShortcut>

BluetoothMode::BluetoothMode(Vench *vench, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BluetoothMode)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    m_vench = vench;
    fl_rec = false;
    fl_play = false;

    SetLStatus();

    // -------- Set Shortcuts (Hot keys) -------- //
    (new QShortcut(QKeySequence(Qt::Key_B), this, [this](){ m_vench->on_BBip_B_clicked(); }))->setAutoRepeat(false);
    (new QShortcut(QKeySequence(Qt::Key_L), this, [this](){ m_vench->on_BLight_B_clicked(); }))->setAutoRepeat(false);
    (new QShortcut(QKeySequence(Qt::Key_Space), this, [this](){ m_vench->on_BStop_B_clicked(); }))->setAutoRepeat(false);
    (new QShortcut(QKeySequence(Qt::Key_R), this, SLOT(RecClicked())))->setAutoRepeat(false);
    (new QShortcut(QKeySequence(Qt::Key_P), this, SLOT(PlayClicked())))->setAutoRepeat(false);
    //--------------------------------------------//

    connect(ui->BBell,  &QPushButton::clicked, m_vench, &Vench::on_BBip_B_clicked);
    connect(ui->BLight, &QPushButton::clicked, m_vench, &Vench::on_BLight_B_clicked);

    connect(ui->BUp, &QPushButton::pressed,  m_vench, &Vench::on_BUp_B_pressed);
    connect(ui->BUp, &QPushButton::released, m_vench, &Vench::on_BStop_B_clicked);
    connect(ui->BLeft, &QPushButton::pressed,  m_vench, &Vench::on_BLeft_B_pressed);
    connect(ui->BLeft, &QPushButton::released, m_vench, &Vench::on_BStop_B_clicked);
    connect(ui->BRight, &QPushButton::pressed,  m_vench, &Vench::on_BRight_B_pressed);
    connect(ui->BRight, &QPushButton::released, m_vench, &Vench::on_BStop_B_clicked);
    connect(ui->BDown, &QPushButton::pressed,  m_vench, &Vench::on_BDown_B_pressed);
    connect(ui->BDown, &QPushButton::released, m_vench, &Vench::on_BStop_B_clicked);
    connect(ui->BStop, &QPushButton::pressed,  m_vench, &Vench::on_BStop_B_clicked);

    connect(ui->CommTextEdit, &QLineEdit::returnPressed, this, &BluetoothMode::RuchnCommClicked);
    connect(ui->BRuchnComm, &QPushButton::clicked, this, &BluetoothMode::RuchnCommClicked);
    connect(ui->checkLog, &QCheckBox::clicked, m_vench, &Vench::on_checkLog_n_B_clicked);
    connect(ui->checkCicl, &QCheckBox::clicked, m_vench, &Vench::on_checkCicl_n_B_clicked);

    connect(ui->BRec, &QPushButton::clicked, this, &BluetoothMode::RecClicked);
    connect(ui->BPlay, &QPushButton::clicked, this, &BluetoothMode::PlayClicked);
    connect(m_vench, &Vench::playDone, this, &BluetoothMode::PlayClicked);

}

void BluetoothMode::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    int key = event->key();
    if ( key == Qt::Key_W || key == 1062)     // Клавиша W (код "Ц" - 1062)
        m_vench->on_BUp_B_pressed();
    else if (key == Qt::Key_A || key == 1060) // Клавиша A (код "Ф" - 1060)
        m_vench->on_BLeft_B_pressed();
    else if (key == Qt::Key_S || key == 1067) // Клавиша S (код "Ы" - 1067)
        m_vench->on_BDown_B_pressed();
    else if (key == Qt::Key_D || key == 1042) // Клавиша D (код "В" - 1042)
        m_vench->on_BRight_B_pressed();
    else
        QWidget::keyPressEvent(event);
}

void BluetoothMode::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    int key = event->key();
    if (key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D ||
            key == 1062 || key == 1060 || key == 1067 || key == 1042)  // Код русской раскладки
        m_vench->on_BStop_B_clicked();
}

void BluetoothMode::RuchnCommClicked()
{
    m_vench->RuchnComm(ui->CommTextEdit->text());
}

void BluetoothMode::RecClicked()
{
    if (fl_rec)
    {
        ui->BRec->setStyleSheet("");
        ui->BRec->setText("Запись");
    }
    else
    {
        ui->BRec->setStyleSheet("background: red");
        ui->BRec->setText("Стоп");
    }
    fl_rec = !fl_rec;
    m_vench->BRec_pressed_n();
}

void BluetoothMode::PlayClicked()
{
    if (fl_play)
    {
        ui->BPlay->setStyleSheet("");
        ui->BPlay->setText("Поехали");
    }
    else
    {
        ui->BPlay->setStyleSheet("background: green");
        ui->BPlay->setText("Стоп");
    }
    fl_play = !fl_play;
    m_vench->BPressPlay();
}

void BluetoothMode::SetLStatus()
{
    ui->LStatus->setText(m_vench->AnsStatus());
    ui->LStatus->setStyleSheet("background: " + m_vench->AnsStatusColor().name());
}

BluetoothMode::~BluetoothMode()
{
    delete ui;
}
