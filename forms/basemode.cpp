#include "basemode.h"
#include "ui_basemode.h"
#include "worklogicalmodul.h"

BaseMode::BaseMode(Vench *vench, QWidget *parent) :
    WorkLogicalModul(vench, parent),
    ui(new Ui::BaseMode)
{
    ui->setupUi(this);

    LStatusChanged();

    ui->listV->clearPropertyFlags();
    ui->listV->setSelectionMode(QAbstractItemView::MultiSelection);

    ui->listVibr->clearPropertyFlags();
    ui->listVibr->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(ui->BBell, &QPushButton::pressed, m_vench, &Vench::BPressBip);
    connect(ui->BBell, &QPushButton::released, m_vench, &Vench::BReleaseBip);

    connect(ui->BLight, &QPushButton::pressed, m_vench, &Vench::BPressLight);
    connect(ui->BLight, &QPushButton::released, m_vench, &Vench::BReleaseLight);
    /////////////////
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

    connect(ui->BRec, &QPushButton::clicked, this, &BaseMode::RecClicked);
    connect(ui->BPlay, &QPushButton::clicked, this, &BaseMode::PlayClicked);
    connect(ui->BProgr, &QPushButton::clicked, m_vench, &Vench::ProgrClicked);
    connect(ui->BHelp, &QPushButton::clicked, this, &WorkLogicalModul::OpenGuidDoc);
    connect(ui->checkDiskr, &QCheckBox::clicked, this, &BaseMode::DiskrChanged);
    connect(ui->checkAll, &QCheckBox::clicked, this, &BaseMode::AllDevicesChanged);
    connect(ui->checkKumir, &QCheckBox::clicked, this, &BaseMode::KumirChanged);

    connect(ui->BUpdate, &QPushButton::clicked, m_vench, &Vench::ObSpActUst);
    connect(ui->BWakeUp, &QPushButton::clicked, m_vench, &Vench::SendCommBudil);

    connect(ui->BPanIdContr, &QPushButton::clicked, m_vench, &Vench::BPressIzmPANID_KONTR);
    connect(ui->BPanId, &QPushButton::clicked, m_vench, &Vench::BPressIzmPANID);

    connect(ui->BTemp, &QPushButton::clicked, this, &BaseMode::TempClicked);
    connect(ui->BGraph, &QPushButton::clicked, this, &BaseMode::GraphClicked);
    connect(ui->BRezult, &QPushButton::clicked, this, &BaseMode::RezultClicked);

    ui->BGraph_O->hide();

    ui->BGraph_E->hide();
    ui->BTemp_E->hide();
    ui->BTemp_D->hide();
    ui->BGraph_D->hide();
}

BaseMode::~BaseMode()
{
    delete ui;
}

void BaseMode::RecClicked()
{
    if (fl_rec)
    {
        ui->BRec->setStyleSheet("");
        ui->BRec->setText("Запись");
        m_vench->BRealeseRec();
    }
    else
    {
        if (!m_vench->BPressRec(ui->listV->count()))
        {
            ui->BRec->setChecked(false);
            return;
        }
        ui->BRec->setStyleSheet("background: red");
        ui->BRec->setText("Стоп");
    }
    fl_rec = !fl_rec;
}

void BaseMode::PlayClicked()
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

void BaseMode::DiskrChanged()
{
    m_vench->SetDiskrFlag(ui->checkDiskr->isChecked());
}

void BaseMode::AllDevicesChanged()
{
    m_vench->SetAllDevicesFlag(ui->checkAll->isChecked());
}

void BaseMode::KumirChanged()
{
    m_vench->SetXMLFlag(!ui->checkKumir->isChecked());
}

void BaseMode::TempClicked()
{
    m_vench->SetTempFlag(ui->BTemp->isChecked());
    m_vench->BPressTemp();
}

void BaseMode::GraphClicked()
{
    m_vench->SetGrphFlag(ui->BGraph->isChecked());
    m_vench->BPressGrph();
}

void BaseMode::RezultClicked()
{
    m_vench->RezultClicked();
    m_vench->BPressParam();
}

void BaseMode::LStatusChanged()
{
    ui->LStatus->setText(m_vench->AnsStatus());
    ui->LStatus->setStyleSheet("background: " + m_vench->AnsStatusColor().name());
}

void BaseMode::RegGrphTextChanged(QString text)
{
    ui->LRegGrph->setText(text);
}
