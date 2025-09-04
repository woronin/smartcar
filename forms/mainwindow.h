#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vench.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString kat, QString prt, QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void OpenGuideDoc();
    void OpenReference();

private:
    Ui::MainWindow *ui;
    Vench* m_vench;
};

#endif // MAINWINDOW_H
