#include "bluetoothmode.h"
#include "ui_bluetoothmode.h"

BluetoothMode::BluetoothMode(Vench *vench, QWidget *parent) :
    WorkLogicalModul(vench, parent),
    ui(new Ui::BluetoothMode)
{
    ui->setupUi(this);

    fl_rec = false;
    fl_play = false;

    LStatusChanged();

    connect(ui->BHelp, &QPushButton::clicked, this, &WorkLogicalModul::OpenGuidDoc);

    connect(ui->BBell,  &QPushButton::clicked, m_vench, &Vench::on_BBip_B_clicked);
    connect(ui->BLight, &QPushButton::clicked, m_vench, &Vench::on_BLight_B_clicked);

    connect(ui->BUp, &QPushButton::pressed,  m_vench, &Vench::on_BUp_B_pressed);
    connect(ui->BUp, &QPushButton::released, m_vench, &Vench::SendCommStop_ave_BT);
    connect(ui->BLeft, &QPushButton::pressed,  m_vench, &Vench::on_BLeft_B_pressed);
    connect(ui->BLeft, &QPushButton::released, m_vench, &Vench::SendCommStop_ave_BT);
    connect(ui->BRight, &QPushButton::pressed,  m_vench, &Vench::on_BRight_B_pressed);
    connect(ui->BRight, &QPushButton::released, m_vench, &Vench::SendCommStop_ave_BT);
    connect(ui->BDown, &QPushButton::pressed,  m_vench, &Vench::on_BDown_B_pressed);
    connect(ui->BDown, &QPushButton::released, m_vench, &Vench::SendCommStop_ave_BT);

    connect(ui->CommTextEdit, &QLineEdit::returnPressed, this, &BluetoothMode::RuchnCommClicked);
    connect(ui->BRuchnComm, &QPushButton::clicked, this, &BluetoothMode::RuchnCommClicked);
    connect(ui->checkLog, &QCheckBox::clicked, m_vench, &Vench::on_checkLog_n_B_clicked);

    connect(ui->BXML, &QRadioButton::clicked, this, &BluetoothMode::ChangeVenchXMLFlag);
    connect(ui->BKumir, &QRadioButton::clicked, this, &BluetoothMode::ChangeVenchXMLFlag);
    connect(ui->BPokazProgr, &QPushButton::clicked, m_vench, &Vench::ProgrClicked);
    connect(ui->BRec, &QPushButton::clicked, this, &BluetoothMode::RecClicked);
    connect(ui->BPlay, &QPushButton::clicked, this, &BluetoothMode::PlayClicked);
}

void BluetoothMode::RuchnCommClicked()
{
    m_vench->RuchnComm(ui->CommTextEdit->text());
}

void BluetoothMode::ChangeVenchXMLFlag()
{
    m_vench->SetXMLFlag(ui->BXML->isChecked());
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

void BluetoothMode::LStatusChanged()
{
    ui->LStatus->setText(m_vench->AnsStatus());
    ui->LStatus->setStyleSheet("background: " + m_vench->AnsStatusColor().name());
}

BluetoothMode::~BluetoothMode()
{
    delete ui;
}
