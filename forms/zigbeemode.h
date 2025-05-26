#ifndef ZIGBEEMODE_H
#define ZIGBEEMODE_H

#include <QWidget>
#include "worklogicalmodul.h"
#include "vench.h"

namespace Ui {
class ZigBeeMode;
}

class ZigBeeMode : public WorkLogicalModul
{

public:
    explicit ZigBeeMode(Vench *vench, QWidget *parent = nullptr);
    ~ZigBeeMode();

public slots:
    void Car1_ButtonClicked();
    void Car2_ButtonClicked();
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void keyReleaseEvent( QKeyEvent * e );

    virtual void LStatusChanged();

private:
    Ui::ZigBeeMode *ui;
};

#endif // ZIGBEEMODE_H
