#include "worklogicalmodul.h"
#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QUrl>
#include <QtCore>

WorkLogicalModul::WorkLogicalModul(Vench *vench, QWidget *parent)
    : QWidget(parent)
{
    m_vench = vench;
    connect(m_vench, &Vench::AnsStatusChanged, this, &WorkLogicalModul::LStatusChanged);
    connect(m_vench, &Vench::RegGrphTextChanged, this, &WorkLogicalModul::RegGrphTextChanged);
}

void WorkLogicalModul::OpenGuidDoc()
{
    QString helpFilePath = QCoreApplication::applicationDirPath() + "/umkiguide.pdf";
    if (QFileInfo(helpFilePath).exists())
        QDesktopServices::openUrl(QUrl(helpFilePath, QUrl::TolerantMode));
    else
        QDesktopServices::openUrl(QUrl("https://www.umkikit.ru/prog/umkiguide.pdf"));
}

void WorkLogicalModul::LStatusChanged()
{
}

void WorkLogicalModul::RegGrphTextChanged(QString text)
{
}
