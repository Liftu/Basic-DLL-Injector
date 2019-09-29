#ifndef FRAMEPRINCIPALE_H
#define FRAMEPRINCIPALE_H

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include <QStatusBar>
#include <QLabel>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>

#include "processlistdialog.h"

class FramePrincipale : public QWidget
{
    Q_OBJECT
public:
    FramePrincipale();

public slots:
    void OnLineEditSearchInProcessListEdition(QString text);
    void OnButtonnSelectProcessClick();
    void OnButtonnSelectDllClick();
    void OnButtonInjectClick();
    void OnProcessSelection();

signals:

private:
    bool isProcessSelected;
    bool isDllSelected;

    QVBoxLayout *m_layoutPrincipale;
    QHBoxLayout *m_layoutProcess;
    QHBoxLayout *m_layoutDll;
    QHBoxLayout *m_layoutButtons;
    QPushButton *m_buttonSelectProcess;
    QPushButton *m_buttonSelectDll;
    QPushButton *m_bouttonInjectDll;
    QLabel *m_labelProcess;
    QLabel *m_labelDll;
    QPushButton *m_buttonInject;
    QPushButton *m_buttonExit;
    QStatusBar *m_statusBarPrincipale;
    QLabel *m_labelStatusBarPrincipale;
    QString m_DllPath;
    QString m_ProcessName;
    DWORD m_ProcessID;
    std::vector<QString> m_ProcessNameList;
    std::vector<DWORD> m_ProcessIDList;

    QDialog *m_DialogProcessSelection;
    QVBoxLayout *m_layoutPrincipaleDialogProcessSelection;
    QHBoxLayout *m_layoutSearchInProcessListDialogProcessSelection;
    QHBoxLayout *m_layoutButtonsDialogProcessSelection;
    QPushButton *m_buttonCancelDialogProcessSelection;
    QPushButton *m_buttonSelectDialogProcessSelection;
    QListWidget *m_listWidgetProcessListDialogProcessSelection;
    QLabel *m_labelSearchInProcessListDialogProcessSelection;
    QLineEdit *m_lineEditSearchInProcessListDialogProcessSelection;
};

#endif // FRAMEPRINCIPALE_H
