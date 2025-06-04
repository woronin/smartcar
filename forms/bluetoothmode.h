#ifndef BLUETOOTHMODE_H
#define BLUETOOTHMODE_H

#include <QWidget>
#include "vench.h"

namespace Ui {
class BluetoothMode;
}

class BluetoothMode : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothMode(Vench *vench, QWidget *parent = nullptr);
    ~BluetoothMode();

protected slots:
    void RuchnCommClicked();
    void RecClicked();
    void PlayClicked();
    void OpenGuidDoc();
    void LStatusChanged();

private:
    Ui::BluetoothMode *ui;
    Vench *m_vench;
    bool fl_rec;
    bool fl_play;
};

#endif // BLUETOOTHMODE_H
