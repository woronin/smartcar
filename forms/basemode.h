#ifndef BASEMODE_H
#define BASEMODE_H

#include <QWidget>
#include "vench.h"
#include "worklogicalmodul.h"

namespace Ui {
class BaseMode;
}

class BaseMode : public WorkLogicalModul
{
    Q_OBJECT

public:
    explicit BaseMode(Vench *vench, QWidget *parent = nullptr);
    ~BaseMode();

protected slots:
    void RecClicked();
    void PlayClicked();
    void DiskrChanged();
    void AllDevicesChanged();
    void KumirChanged();
    void TempClicked();
    void GraphClicked();
    void RezultClicked();

    virtual void LStatusChanged();
    virtual void RegGrphTextChanged(QString text);

private:
    Ui::BaseMode *ui;

    bool fl_rec;
    bool fl_play;
};

#endif // BASEMODE_H
