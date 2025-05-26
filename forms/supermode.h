#ifndef SUPERMODE_H
#define SUPERMODE_H

#include <QWidget>
#include "vench.h"
#include "worklogicalmodul.h"

namespace Ui {
class SuperMode;
}

class SuperMode : public WorkLogicalModul
{
    Q_OBJECT

public:
    explicit SuperMode(Vench *vench, QWidget *parent = nullptr);
    ~SuperMode();

protected slots:
    void DiskrChanged();
    void DopParamClicked();
    void AllDevicesFlagChanged();
    void UnsleepChanged();
    void SpeedClicked();
    void RuchnCommClicked();
    void ChangeVenchXMLFlag();
    void RecClicked();
    void PlayClicked();
    void TempClicked();
    void EnergClicked();
    void DimClicked();
    void BarClicked();
    void RezultClicked();
    void ServerDirChanged();
    void BPressGrph();
    void BPressGrphE();
    void BPressGrphO();
    void BPressGrphD();
    void BPressGrphB();
    void BPressGrphV();
    void DatChanged(Dat dat);
    void LTempTextChanged(QString text);
    void LOsvTextChanged(QString text);
    void LEnTextChanged(QString text);
    void LDimTextChanged(QString text);
    void LBarTextChanged(QString text);
    void LVlTextChanged(QString text);

    virtual void LStatusChanged();
    virtual void RegGrphTextChanged(QString text);

private:
    Ui::SuperMode *ui;
    bool fl_rec;
    bool fl_play;
};

#endif // SUPERMODE_H
