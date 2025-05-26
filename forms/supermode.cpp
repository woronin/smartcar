#include "supermode.h"
#include "ui_supermode.h"

SuperMode::SuperMode(Vench *vench, QWidget *parent) :
    WorkLogicalModul(vench, parent),
    ui(new Ui::SuperMode)
{
    ui->setupUi(this);
    fl_rec = false;
    fl_play = false;

    ui->BDopParam->setChecked(true);
    DopParamClicked();

    LStatusChanged();

    ui->listV->clearPropertyFlags();
    ui->listV->setSelectionMode(QAbstractItemView::MultiSelection);

    ui->list_PANID->clearPropertyFlags();
    ui->list_PANID->setSelectionMode(QAbstractItemView::MultiSelection);

    ui->listVibr->clearPropertyFlags();
    ui->listVibr->setSelectionMode(QAbstractItemView::MultiSelection);

    ui->ImpS->setText(QString().setNum(m_vench->speed_car, 10));
    ui->BDiscrImp->setText(QString().setNum(m_vench->imp_diskr, 10));

    ui->ServerDir->setText(m_vench->ServerDir());
    connect(m_vench, &Vench::ServerDirCganged, this, &SuperMode::ServerDirChanged);

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
    connect(ui->checkDiskr, &QCheckBox::clicked, this, &SuperMode::DiskrChanged);
    connect(m_vench, &Vench::DatChanged, this, &SuperMode::DatChanged);

    connect(ui->BUpdate, &QPushButton::clicked, m_vench, &Vench::ObSpActUst);
    connect(ui->BDiscrImp, &QPushButton::clicked, m_vench, &Vench::BPressBDiscrImp_Ever);
    connect(ui->ZapPanId, &QPushButton::clicked, m_vench, &Vench::BPressZapPANID);
    connect(ui->IzmPanId, &QPushButton::clicked, m_vench, &Vench::BPressIzmPANID);
    connect(ui->IzmPanIdKONTR, &QPushButton::clicked, m_vench, &Vench::BPressIzmPANID_KONTR);

    connect(ui->BDopParam, &QPushButton::clicked, this, &SuperMode::DopParamClicked);
    connect(ui->BVibrSp, &QRadioButton::clicked, this, &SuperMode::AllDevicesFlagChanged);
    connect(ui->BAll, &QRadioButton::clicked, this, &SuperMode::AllDevicesFlagChanged);
    connect(ui->checkUnsleep, &QCheckBox::clicked, this, &SuperMode::UnsleepChanged);
    connect(ui->BAntiSleep, &QPushButton::clicked, m_vench, &Vench::SendCommBudil);
    connect(ui->BSpeed, &QPushButton::clicked, this, &SuperMode::SpeedClicked);
    connect(ui->BRuchnComm, &QPushButton::clicked, this, &SuperMode::RuchnCommClicked);
    connect(ui->checkLog, &QCheckBox::clicked, m_vench, &Vench::chLog);
    connect(ui->BXML, &QRadioButton::clicked, this, &SuperMode::ChangeVenchXMLFlag);
    connect(ui->BKumir, &QRadioButton::clicked, this, &SuperMode::ChangeVenchXMLFlag);
    connect(ui->BPokazProgr, &QPushButton::clicked, m_vench, &Vench::ProgramClicked);
    connect(ui->BRec, &QPushButton::clicked, this, &SuperMode::RecClicked);
    connect(ui->BPlay, &QPushButton::clicked, this, &SuperMode::PlayClicked);
    connect(ui->BContinue, &QPushButton::clicked, m_vench, &Vench::BPressProdol);
    connect(ui->BLog,  &QPushButton::clicked, m_vench, &Vench::BPressLog);
    connect(ui->BParam,  &QPushButton::clicked, m_vench, &Vench::BPressParam);
    connect(ui->BHelp, &QPushButton::clicked, this, &WorkLogicalModul::OpenGuidDoc);

    connect(ui->BTemp, &QPushButton::clicked, this, &SuperMode::TempClicked);
    connect(ui->BEnerg, &QPushButton::clicked, this, &SuperMode::EnergClicked);
    connect(ui->BDim, &QPushButton::clicked, this, &SuperMode::DimClicked);
    connect(ui->BBar, &QPushButton::clicked, this, &SuperMode::BarClicked);
    connect(ui->BRezult, &QPushButton::clicked, this, &SuperMode::RezultClicked);

    connect(ui->BGrph, &QPushButton::clicked, this, &SuperMode::BPressGrph);
    connect(ui->BGrphE, &QPushButton::clicked, this, &SuperMode::BPressGrphE);
    connect(ui->BGrphO, &QPushButton::clicked, this, &SuperMode::BPressGrphO);
    connect(ui->BGrphD, &QPushButton::clicked, this, &SuperMode::BPressGrphD);
    connect(ui->BGrphB, &QPushButton::clicked, this, &SuperMode::BPressGrphB);
    connect(ui->BGrphVl, &QPushButton::clicked, this, &SuperMode::BPressGrphV);

    connect(m_vench, &Vench::LTempChanged, this, &SuperMode::LTempTextChanged);
    connect(m_vench, &Vench::LOsvChanged, this, &SuperMode::LOsvTextChanged);
    connect(m_vench, &Vench::LEnChanged, this, &SuperMode::LEnTextChanged);
    connect(m_vench, &Vench::LDimChanged, this, &SuperMode::LDimTextChanged);
    connect(m_vench, &Vench::LBarChanged, this, &SuperMode::LBarTextChanged);
    connect(m_vench, &Vench::LVlChanged, this, &SuperMode::LVlTextChanged);
}

SuperMode::~SuperMode()
{
    delete ui;
}

void SuperMode::DiskrChanged()
{
    m_vench->SetDiskrFlag(ui->checkDiskr->isChecked());
}

void SuperMode::DopParamClicked()
{
    if (ui->BDopParam->isChecked())
    {
        ui->BDopParam->setText("Показать дополнительные параметры \\/ ");
        ui->GDopParam->hide();
    }
    else
    {
        ui->BDopParam->setText(trUtf8("Скрыть дополнительные параметры /\\ "));
        ui->GDopParam->show();
    }

}

void SuperMode::AllDevicesFlagChanged()
{
    m_vench->SetAllDevicesFlag(ui->BAll->isChecked());
}

void SuperMode::UnsleepChanged()
{
    m_vench->SetUnsleepFlag(ui->checkUnsleep);
}

void SuperMode::SpeedClicked()
{
    m_vench->SetSpeedCar(ui->ImpS->text().toInt());
}

void SuperMode::RuchnCommClicked()
{
    m_vench->RuchnComm(ui->CommTextEdit->text());
}

void SuperMode::ChangeVenchXMLFlag()
{
    m_vench->SetXMLFlag(ui->BXML->isChecked());
}

void SuperMode::RecClicked()
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

void SuperMode::PlayClicked()
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

void SuperMode::TempClicked()
{
    m_vench->SetTempFlag(ui->BTemp->isChecked());
    m_vench->BPressTemp();
}

void SuperMode::EnergClicked()
{
    m_vench->SetEnergFlag(ui->BEnerg->isChecked());
    m_vench->BPressEnerg();
}

void SuperMode::DimClicked()
{
    m_vench->SetDimFlag(ui->BDim->isChecked());
    m_vench->BPressDim();
}

void SuperMode::BarClicked()
{
    m_vench->SetBarFlag(ui->BBar->isChecked());
    m_vench->BPressBar();
}

void SuperMode::RezultClicked()
{
    m_vench->RezultClicked();
}

void SuperMode::ServerDirChanged()
{
    ui->ServerDir->setText(m_vench->ServerDir());
}

void SuperMode::BPressGrph()
{
    m_vench->SetGrphFlag(ui->BGrph->isChecked());
    m_vench->BPressGrph();
}

void SuperMode::BPressGrphE()
{
    m_vench->SetGrphEFlag(ui->BGrphE->isChecked());
    m_vench->BPressGrphE();
}

void SuperMode::BPressGrphO()
{
    m_vench->SetGrphOFlag(ui->BGrphO->isChecked());
    m_vench->BPressGrphO();
}

void SuperMode::BPressGrphD()
{
    m_vench->SetGrphDFlag(ui->BGrphD->isChecked());
    m_vench->BPressGrphD();
}

void SuperMode::BPressGrphB()
{
    m_vench->SetGrphBFlag(ui->BGrphB->isChecked());
    m_vench->BPressGrphB();
}

void SuperMode::BPressGrphV()
{
    m_vench->SetGrphVlFlag(ui->BGrphVl->isChecked());
    m_vench->BPressGrphV();
}

void SuperMode::DatChanged(Dat dat)
{
    if (dat == Dat::Right)
    {
        ui->LeftDat->setStyleSheet("");
        ui->RightDat->setStyleSheet("background: red");
    }
    else if (dat == Dat::Left)
    {
        ui->LeftDat->setStyleSheet("background: red");
        ui->RightDat->setStyleSheet("");
    }
    else // dat == Dat::None
    {
        ui->LeftDat->setStyleSheet("");
        ui->RightDat->setStyleSheet("");
    }

}

void SuperMode::LStatusChanged()
{
    ui->LStatus->setText(m_vench->AnsStatus());
    ui->LStatus->setStyleSheet("background: " + m_vench->AnsStatusColor().name());
}

void SuperMode::RegGrphTextChanged(QString text)
{
    ui->LRegGrph->setText(text);
}

void SuperMode::LTempTextChanged(QString text)
{
    ui->LTemp->setText(text);
}

void SuperMode::LOsvTextChanged(QString text)
{
    ui->LOsv->setText(text);
}

void SuperMode::LEnTextChanged(QString text)
{
    ui->LEn->setText(text);
}

void SuperMode::LDimTextChanged(QString text)
{
    ui->LDim->setText(text);
}

void SuperMode::LBarTextChanged(QString text)
{
    ui->LBar->setText(text);
}

void SuperMode::LVlTextChanged(QString text)
{
    ui->LVl->setText(text);
}
