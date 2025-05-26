#include "zigbeemode.h"
#include "ui_zigbeemode.h"

#include "worklogicalmodul.h"
#include <QKeyEvent>

ZigBeeMode::ZigBeeMode(Vench *vench, QWidget *parent) :
    WorkLogicalModul(vench, parent),
    ui(new Ui::ZigBeeMode)
{
    ui->setupUi(this);

    LStatusChanged();

    connect(ui->BHelp, &QPushButton::clicked, this, &WorkLogicalModul::OpenGuidDoc);
    connect(ui->BCar_1, &QPushButton::clicked, this, &ZigBeeMode::Car1_ButtonClicked);
    connect(ui->BCar_2, &QPushButton::clicked, this, &ZigBeeMode::Car2_ButtonClicked);

    connect(ui->BBell, &QPushButton::pressed, m_vench, &Vench::BPressBip);
    connect(ui->BBell, &QPushButton::released, m_vench, &Vench::BReleaseBip);

    connect(ui->BLight, &QPushButton::pressed, m_vench, &Vench::BPressLight);
    connect(ui->BLight, &QPushButton::released, m_vench, &Vench::BReleaseLight);

    connect(ui->BUp, &QPushButton::pressed, m_vench, &Vench::BPressFw);
    connect(ui->BUp, &QPushButton::released, m_vench, &Vench::BReleaseFw);

    connect(ui->BLeft, &QPushButton::pressed, m_vench, &Vench::BPressLeft);
    connect(ui->BLeft, &QPushButton::released, m_vench, &Vench::BReleaseLeft);

    connect(ui->BRight, &QPushButton::pressed, m_vench, &Vench::BPressRight);
    connect(ui->BRight, &QPushButton::released, m_vench, &Vench::BReleaseRight);

    connect(ui->BDown, &QPushButton::pressed, m_vench, &Vench::BPressRv);
    connect(ui->BDown, &QPushButton::released, m_vench, &Vench::BReleaseRv);

    connect(ui->BStop, &QPushButton::pressed, m_vench, &Vench::BPressStop);
    connect(ui->BStop, &QPushButton::released, m_vench, &Vench::BRealeseStop);

    connect(ui->BUpdate, &QPushButton::clicked, m_vench, &Vench::ObSpActUst);
    connect(ui->BWakeUp, &QPushButton::clicked, m_vench, &Vench::SendCommBudil);
}

void ZigBeeMode::keyPressEvent( QKeyEvent *e )
{
    /*switch (e->key()) {
    case Qt::Key_B:
        ui->BBell->pressed();
        break;
    }*/
}

void ZigBeeMode::keyReleaseEvent( QKeyEvent *e )
{
    /*switch (e->key()) {
    case Qt::Key_B:
        ui->BBell->released();
        break;
        // L - BLight
    }*/
}

void ZigBeeMode::Car1_ButtonClicked()
{
    m_vench->CarChanged(1);
    ui->BCar_2->setChecked(false);
    ui->BCar_1->setChecked(true);
}

void ZigBeeMode::Car2_ButtonClicked()
{
    m_vench->CarChanged(2);
    ui->BCar_1->setChecked(false);
    ui->BCar_2->setChecked(true);
}

void ZigBeeMode::LStatusChanged()
{
    ui->LStatus->setText(m_vench->AnsStatus());
    ui->LStatus->setStyleSheet("background: " + m_vench->AnsStatusColor().name());
}

ZigBeeMode::~ZigBeeMode()
{
    delete ui;
}
