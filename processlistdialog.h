#ifndef PROCESSLISTDIALOG_H
#define PROCESSLISTDIALOG_H

#include <vector>

#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>

#include <QMessageBox>

class ProcessListDialog : public QDialog
{
    Q_OBJECT
public:
    ProcessListDialog(std::vector<QString> content, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

public slots:
    void OnSelection();

private:
    QVBoxLayout *m_layoutPrincipale;
    QHBoxLayout *m_layoutButtons;
    QPushButton *m_buttonCancel;
    QPushButton *m_buttonSelect;
    QListWidget *m_listWidgetProcessList;
    QString m_ProcessSelected;
};

#endif // PROCESSLISTDIALOG_H
