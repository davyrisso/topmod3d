/********************************************************************************
** Form generated from reading UI file 'pathparameters.ui'
**
** Created: Fri Mar 2 17:24:46 2018
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATHPARAMETERS_H
#define UI_PATHPARAMETERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PathParameters
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *ScaleBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDoubleSpinBox *AngleBox;
    QGridLayout *gridLayout;
    QRadioButton *rotateAlternatively;
    QRadioButton *rotateAll;
    QRadioButton *rotateEdge;
    QRadioButton *rotateVertexFace;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *PathParameters)
    {
        if (PathParameters->objectName().isEmpty())
            PathParameters->setObjectName(QString::fromUtf8("PathParameters"));
        PathParameters->resize(308, 204);
        horizontalLayout_4 = new QHBoxLayout(PathParameters);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(PathParameters);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        ScaleBox = new QDoubleSpinBox(PathParameters);
        ScaleBox->setObjectName(QString::fromUtf8("ScaleBox"));
        ScaleBox->setMinimum(0.1);
        ScaleBox->setMaximum(5);
        ScaleBox->setSingleStep(0.1);
        ScaleBox->setValue(1.33);

        horizontalLayout->addWidget(ScaleBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(PathParameters);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        AngleBox = new QDoubleSpinBox(PathParameters);
        AngleBox->setObjectName(QString::fromUtf8("AngleBox"));
        AngleBox->setMinimum(-180);
        AngleBox->setMaximum(180);

        horizontalLayout_2->addWidget(AngleBox);


        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        rotateAlternatively = new QRadioButton(PathParameters);
        rotateAlternatively->setObjectName(QString::fromUtf8("rotateAlternatively"));
        rotateAlternatively->setChecked(true);

        gridLayout->addWidget(rotateAlternatively, 3, 0, 1, 1);

        rotateAll = new QRadioButton(PathParameters);
        rotateAll->setObjectName(QString::fromUtf8("rotateAll"));

        gridLayout->addWidget(rotateAll, 0, 0, 1, 1);

        rotateEdge = new QRadioButton(PathParameters);
        rotateEdge->setObjectName(QString::fromUtf8("rotateEdge"));

        gridLayout->addWidget(rotateEdge, 1, 0, 1, 1);

        rotateVertexFace = new QRadioButton(PathParameters);
        rotateVertexFace->setObjectName(QString::fromUtf8("rotateVertexFace"));

        gridLayout->addWidget(rotateVertexFace, 2, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton = new QPushButton(PathParameters);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_3->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout);


        retranslateUi(PathParameters);
        QObject::connect(pushButton, SIGNAL(clicked()), PathParameters, SLOT(close()));

        QMetaObject::connectSlotsByName(PathParameters);
    } // setupUi

    void retranslateUi(QDialog *PathParameters)
    {
        PathParameters->setWindowTitle(QApplication::translate("PathParameters", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PathParameters", "Scaling Factor", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PathParameters", "Rotation Angle", 0, QApplication::UnicodeUTF8));
        rotateAlternatively->setText(QApplication::translate("PathParameters", "Alternate Rotation Angles", 0, QApplication::UnicodeUTF8));
        rotateAll->setText(QApplication::translate("PathParameters", "Rotate All Control Points Uniformly", 0, QApplication::UnicodeUTF8));
        rotateEdge->setText(QApplication::translate("PathParameters", "Rotate Control-Points on Edge Corners", 0, QApplication::UnicodeUTF8));
        rotateVertexFace->setText(QApplication::translate("PathParameters", "Rotate Control Points on Vertex and Face-Center", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("PathParameters", "Set", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PathParameters: public Ui_PathParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATHPARAMETERS_H
