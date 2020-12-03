#ifndef FORMHLP_H
#define FORMHLP_H

#include <QDialog>
//#include <QWidget/QWebView>

namespace Ui {
class formhlp;
}

class formhlp : public QDialog
{
    Q_OBJECT

public:
    explicit formhlp(QWidget *parent = 0);
    ~formhlp();
    void SetPathFileHlp(QString pf);
    QString urlm;

public slots:
    void showEvent(QShowEvent* event);

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::formhlp *ui;
};

#endif // FORMHLP_H
