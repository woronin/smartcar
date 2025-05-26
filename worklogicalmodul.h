#ifndef WORKLOGICALMODUL_H
#define WORKLOGICALMODUL_H

#include <QObject>
#include <QWidget>

#include "vench.h"

class WorkLogicalModul : public QWidget
{
    Q_OBJECT
public:
    explicit WorkLogicalModul(Vench *vench, QWidget*parent = nullptr);

protected:
    Vench *m_vench;
signals:

public slots:
    void OpenGuidDoc();
    virtual void LStatusChanged();
    virtual void RegGrphTextChanged(QString text);

};

#endif // WORKLOGICALMODUL_H
