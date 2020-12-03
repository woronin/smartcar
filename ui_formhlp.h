/********************************************************************************
** Form generated from reading UI file 'formhlp.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMHLP_H
#define UI_FORMHLP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_formhlp
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *formhlp)
    {
        if (formhlp->objectName().isEmpty())
            formhlp->setObjectName(QStringLiteral("formhlp"));
        formhlp->resize(823, 559);
        pushButton = new QPushButton(formhlp);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(720, 500, 99, 27));
        pushButton_2 = new QPushButton(formhlp);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(570, 260, 99, 27));

        retranslateUi(formhlp);

        QMetaObject::connectSlotsByName(formhlp);
    } // setupUi

    void retranslateUi(QDialog *formhlp)
    {
        formhlp->setWindowTitle(QApplication::translate("formhlp", "Dialog", 0));
        pushButton->setText(QApplication::translate("formhlp", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0));
        pushButton_2->setText(QApplication::translate("formhlp", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class formhlp: public Ui_formhlp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMHLP_H
