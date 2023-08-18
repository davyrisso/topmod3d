/********************************************************************************
** Form generated from reading UI file 'shortcutdialog.ui'
**
** Created: Tue May 1 14:49:40 2018
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHORTCUTDIALOG_H
#define UI_SHORTCUTDIALOG_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShortcutDialog
{
public:
    QVBoxLayout *vboxLayout;
    QTreeWidget *twShortcuts;

    void setupUi(QWidget *ShortcutDialog)
    {
        if (ShortcutDialog->objectName().isEmpty())
            ShortcutDialog->setObjectName(QString::fromUtf8("ShortcutDialog"));
        ShortcutDialog->resize(549, 349);
        vboxLayout = new QVBoxLayout(ShortcutDialog);
        vboxLayout->setSpacing(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        twShortcuts = new QTreeWidget(ShortcutDialog);
        twShortcuts->setObjectName(QString::fromUtf8("twShortcuts"));

        vboxLayout->addWidget(twShortcuts);


        retranslateUi(ShortcutDialog);

        QMetaObject::connectSlotsByName(ShortcutDialog);
    } // setupUi

    void retranslateUi(QWidget *ShortcutDialog)
    {
        ShortcutDialog->setWindowTitle("Set shortcuts");
        QTreeWidgetItem *___qtreewidgetitem = twShortcuts->headerItem();
        ___qtreewidgetitem->setText(1, "Shortcut");
        ___qtreewidgetitem->setText(0, "Action");
    } // retranslateUi

};

namespace Ui {
    class ShortcutDialog: public Ui_ShortcutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHORTCUTDIALOG_H
