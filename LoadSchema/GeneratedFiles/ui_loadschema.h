/********************************************************************************
** Form generated from reading UI file 'loadschema.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADSCHEMA_H
#define UI_LOADSCHEMA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "optimaview.h"

QT_BEGIN_NAMESPACE

class Ui_LoadSchemaClass
{
public:
    QAction *action;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    OptimaView *graphicsView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LoadSchemaClass)
    {
        if (LoadSchemaClass->objectName().isEmpty())
            LoadSchemaClass->setObjectName(QString::fromUtf8("LoadSchemaClass"));
        LoadSchemaClass->resize(600, 400);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoadSchemaClass->sizePolicy().hasHeightForWidth());
        LoadSchemaClass->setSizePolicy(sizePolicy);
        action = new QAction(LoadSchemaClass);
        action->setObjectName(QString::fromUtf8("action"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        action->setIcon(icon);
        centralWidget = new QWidget(LoadSchemaClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new OptimaView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        LoadSchemaClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LoadSchemaClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 18));
        LoadSchemaClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LoadSchemaClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        LoadSchemaClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LoadSchemaClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        LoadSchemaClass->setStatusBar(statusBar);

        mainToolBar->addAction(action);

        retranslateUi(LoadSchemaClass);
        QObject::connect(action, SIGNAL(triggered()), LoadSchemaClass, SLOT(loadXml()));

        QMetaObject::connectSlotsByName(LoadSchemaClass);
    } // setupUi

    void retranslateUi(QMainWindow *LoadSchemaClass)
    {
        LoadSchemaClass->setWindowTitle(QApplication::translate("LoadSchemaClass", "LoadSchema", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("LoadSchemaClass", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoadSchemaClass: public Ui_LoadSchemaClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADSCHEMA_H
