#include "processlistdialog.h"

ProcessListDialog::ProcessListDialog(std::vector<QString> content, QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    setWindowTitle("Process list");
    resize(300, 360);
    m_layoutPrincipale = new QVBoxLayout;
    m_listWidgetProcessList = new QListWidget;
    m_layoutButtons = new QHBoxLayout;
    m_buttonCancel = new QPushButton("Cancel");
    m_buttonSelect = new QPushButton("Select");
    m_layoutButtons->addWidget(m_buttonCancel);
    m_layoutButtons->addWidget(m_buttonSelect);
    m_layoutPrincipale->addWidget(m_listWidgetProcessList);
    m_layoutPrincipale->addLayout(m_layoutButtons);
    setLayout(m_layoutPrincipale);

    for (int i = 1; i < content.size(); i++)
    {
        m_listWidgetProcessList->addItem(content[i]);
    }
    connect(m_listWidgetProcessList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnSelection()));
    connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_buttonSelect, SIGNAL(clicked()), this, SLOT(OnSelection()));
}

void ProcessListDialog::OnSelection()
{

}

