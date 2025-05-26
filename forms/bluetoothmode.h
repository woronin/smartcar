#ifndef BLUETOOTHMODE_H
#define BLUETOOTHMODE_H

#include <QWidget>
#include "worklogicalmodul.h"
#include "vench.h"

namespace Ui {
class BluetoothMode;
}

class BluetoothMode : public WorkLogicalModul
{
    Q_OBJECT

public:
    explicit BluetoothMode(Vench *vench, QWidget *parent = nullptr);
    ~BluetoothMode();

protected slots:
    void RuchnCommClicked();
    void ChangeVenchXMLFlag();
    void RecClicked();
    void PlayClicked();

    virtual void LStatusChanged();

private:
    Ui::BluetoothMode *ui;

    bool fl_rec;
    bool fl_play;
};

#endif // BLUETOOTHMODE_H
