#include "frameprincipale.h"

using namespace std;

FramePrincipale::FramePrincipale() : QWidget()
{
    isProcessSelected = false;
    isDllSelected = false;
    m_ProcessID = 0;

    setWindowTitle("DLL Injector");
    setWindowIcon(QIcon("injector_icon.png"));

    resize(300, 120);

    m_layoutPrincipale = new QVBoxLayout;

    m_layoutProcess = new QHBoxLayout;
    m_buttonSelectProcess = new QPushButton("Select process");
    m_labelProcess = new QLabel("No process selected.");
    m_labelProcess->setStyleSheet("color: red");
    m_layoutProcess->addWidget(m_buttonSelectProcess);
    m_layoutProcess->addWidget(m_labelProcess);

    m_layoutDll = new QHBoxLayout;
    m_buttonSelectDll = new QPushButton("Select DLL");
    m_labelDll = new QLabel("No DLL selected.");
    m_labelDll->setStyleSheet("color: red");
    m_layoutDll->addWidget(m_buttonSelectDll);
    m_layoutDll->addWidget(m_labelDll);

    m_layoutButtons = new QHBoxLayout;
    m_buttonExit = new QPushButton("Exit");
    m_buttonInject = new QPushButton("Inject");
    m_buttonInject->setEnabled(false);
    m_layoutButtons->addWidget(m_buttonExit);
    m_layoutButtons->addWidget(m_buttonInject);

    m_layoutPrincipale->addLayout(m_layoutProcess);
    m_layoutPrincipale->addLayout(m_layoutDll);
    m_layoutPrincipale->addLayout(m_layoutButtons);

    setLayout(m_layoutPrincipale);

    m_buttonSelectProcess->setDefault(true);

    connect(m_buttonSelectProcess, SIGNAL(clicked()), this, SLOT(OnButtonnSelectProcessClick()));
    connect(m_buttonSelectDll, SIGNAL(clicked()), this, SLOT(OnButtonnSelectDllClick()));
    connect(m_buttonInject, SIGNAL(clicked()), this, SLOT(OnButtonInjectClick()));
    connect(m_buttonExit, SIGNAL(clicked()), qApp, SLOT(quit()));

    /*m_labelStatusBarPrincipale = new QLabel("Waiting...");
    m_statusBarPrincipale = new QStatusBar(this);
    m_statusBarPrincipale->addWidget(m_labelStatusBarPrincipale);*/
}

void FramePrincipale::OnButtonnSelectProcessClick()
{
    HANDLE hProcessSnap;
    //HANDLE hProcess;
    PROCESSENTRY32 pe32;
    //DWORD dwPriorityClass;
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        QMessageBox::critical(this, "Error", "Error : CreateToolhelp32Snapshot (of processes)");
    }
    else
    {
        pe32.dwSize = sizeof( PROCESSENTRY32 );
        if( !Process32First( hProcessSnap, &pe32 ) )
        {
            QMessageBox::critical(this, "Error", "Error : Process32First"); // show cause of failure
            CloseHandle( hProcessSnap );          // clean the snapshot object
        }
        else
        {
            do
            {
                char a[255];
                wcstombs(a, pe32.szExeFile, 255);
                m_ProcessNameList.push_back(a);
                m_ProcessIDList.push_back(pe32.th32ProcessID);
            }
            while( Process32Next( hProcessSnap, &pe32 ) );

            m_DialogProcessSelection = new QDialog(this);
            m_DialogProcessSelection->setWindowTitle("Process list");
            m_DialogProcessSelection->resize(300, 360);

            m_layoutPrincipaleDialogProcessSelection = new QVBoxLayout;
            m_layoutSearchInProcessListDialogProcessSelection = new QHBoxLayout;
            m_listWidgetProcessListDialogProcessSelection = new QListWidget;

            m_labelSearchInProcessListDialogProcessSelection = new QLabel("Search : ");
            m_lineEditSearchInProcessListDialogProcessSelection = new QLineEdit;
            m_layoutSearchInProcessListDialogProcessSelection->addWidget(m_labelSearchInProcessListDialogProcessSelection);
            m_layoutSearchInProcessListDialogProcessSelection->addWidget(m_lineEditSearchInProcessListDialogProcessSelection);

            m_layoutButtonsDialogProcessSelection = new QHBoxLayout;
            m_buttonCancelDialogProcessSelection = new QPushButton("Cancel");
            m_buttonSelectDialogProcessSelection = new QPushButton("Select");
            m_layoutButtonsDialogProcessSelection->addWidget(m_buttonCancelDialogProcessSelection);
            m_layoutButtonsDialogProcessSelection->addWidget(m_buttonSelectDialogProcessSelection);

            m_layoutPrincipaleDialogProcessSelection->addWidget(m_listWidgetProcessListDialogProcessSelection);
            m_layoutPrincipaleDialogProcessSelection->addLayout(m_layoutSearchInProcessListDialogProcessSelection);
            m_layoutPrincipaleDialogProcessSelection->addLayout(m_layoutButtonsDialogProcessSelection);
            m_DialogProcessSelection->setLayout(m_layoutPrincipaleDialogProcessSelection);

            for (size_t i = 1; i < m_ProcessNameList.size(); i++)
            {
                m_listWidgetProcessListDialogProcessSelection->addItem(m_ProcessNameList[i]);
            }
            m_listWidgetProcessListDialogProcessSelection->setCurrentRow(m_listWidgetProcessListDialogProcessSelection->count()-1);

            m_buttonSelectDialogProcessSelection->setDefault(true);
            m_lineEditSearchInProcessListDialogProcessSelection->setFocus();

            connect(m_lineEditSearchInProcessListDialogProcessSelection, SIGNAL(textChanged(QString)), this, SLOT(OnLineEditSearchInProcessListEdition(QString)));
            connect(m_listWidgetProcessListDialogProcessSelection, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnProcessSelection()));
            connect(m_buttonCancelDialogProcessSelection, SIGNAL(clicked()), m_DialogProcessSelection, SLOT(close()));
            connect(m_buttonSelectDialogProcessSelection, SIGNAL(clicked()), this, SLOT(OnProcessSelection()));

            m_DialogProcessSelection->exec();
        }
    }
}

void FramePrincipale::OnLineEditSearchInProcessListEdition(QString text)
{
    if (!m_listWidgetProcessListDialogProcessSelection->findItems(text, Qt::MatchContains).isEmpty())
        m_listWidgetProcessListDialogProcessSelection->setCurrentItem(m_listWidgetProcessListDialogProcessSelection->findItems(text, Qt::MatchContains).first());
}

void FramePrincipale::OnProcessSelection()
{
    m_DialogProcessSelection->close();
    m_ProcessName = m_ProcessNameList[m_listWidgetProcessListDialogProcessSelection->currentRow()+1];
    m_ProcessID = m_ProcessIDList[m_listWidgetProcessListDialogProcessSelection->currentRow()+1];
    if (m_ProcessName != QString("") && m_ProcessID != 0)
    {
        m_labelProcess->setText(m_ProcessName + " attached");
        m_labelProcess->setStyleSheet("color: blue");
        isProcessSelected = true;
        m_buttonSelectProcess->setDefault(false);
        m_buttonSelectDll->setDefault(true);
        m_buttonSelectDll->setFocus();
    }
    else
    {
        m_labelProcess->setText("No process selected");
        m_labelProcess->setStyleSheet("color: red");
        isProcessSelected = false;
    }
    if (isProcessSelected && isDllSelected)
    {
        m_buttonInject->setEnabled(true);
        m_buttonInject->setDefault(true);
        m_buttonInject->setFocus();
        m_buttonSelectProcess->setDefault(false);
        m_buttonSelectDll->setDefault(false);
    }
    else
    {
        m_buttonInject->setEnabled(false);
    }
}

void FramePrincipale::OnButtonnSelectDllClick()
{
    m_DllPath = QFileDialog::getOpenFileName(this, "Select the DLL to inject", QString(), "DLL (*.dll)");
    if (m_DllPath != QString(""))
    {
        QString dllName = m_DllPath.split("/").last();
        m_labelDll->setText(dllName + " charged");
        m_labelDll->setStyleSheet("color: blue");
        isDllSelected = true;
    }
    else
    {
        m_labelDll->setText("No DLL selected");
        m_labelDll->setStyleSheet("color: red");
        isDllSelected = false;
    }
    if (isProcessSelected && isDllSelected)
    {
        m_buttonInject->setEnabled(true);
        m_buttonInject->setDefault(true);
        m_buttonInject->setFocus();
        m_buttonSelectProcess->setDefault(false);
        m_buttonSelectDll->setDefault(false);
    }
    else
    {
        m_buttonInject->setEnabled(false);
        m_buttonInject->setDefault(false);
        m_buttonSelectProcess->setDefault(false);
        m_buttonSelectDll->setDefault(true);
        m_buttonSelectDll->setFocus();
    }
}

void FramePrincipale::OnButtonInjectClick()
{
    if (isProcessSelected && isDllSelected && (m_ProcessID != 0) && (m_DllPath != QString("")))
    {
        DWORD ProcessID = m_ProcessID;
        bool memoryWritten = false;
        int c = 0;
        while (!memoryWritten && (c <= 5))
        {
            c++;
            typedef HINSTANCE (*fpLoadLibrary)(char*);
            HANDLE hProc;
            LPVOID paramAddr;

            HINSTANCE hDll = LoadLibraryA("KERNEL32");

            fpLoadLibrary LoadLibraryAddr = (fpLoadLibrary)GetProcAddress(hDll, "LoadLibraryA");

            hProc = OpenProcess (PROCESS_ALL_ACCESS, false, ProcessID);

            char *dllPath = m_DllPath.toLocal8Bit().data();

            paramAddr = VirtualAllocEx(hProc, 0, strlen(dllPath)+1, MEM_COMMIT, PAGE_READWRITE);
            memoryWritten = WriteProcessMemory(hProc, paramAddr, dllPath, strlen(dllPath)+1, NULL);

            CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAddr, paramAddr, 0, 0);

            CloseHandle(hProc);

            if (!memoryWritten && c < 5)
            {
                QMessageBox::warning(this, "Warning", "Warning : DLL failed to inject to "+m_ProcessName+", retrying in 5 seconds...  attempt "+c+"/5");
                Sleep(5000);
            }
            else if (!memoryWritten && c == 5)
            {
                QMessageBox::warning(this, "Warning", "Warning : DLL failed to inject to "+m_ProcessName+" 5 times in a row.");
            }
            else
            {
                QMessageBox::information(this, "Success", "DLL successfully injected to "+m_ProcessName+".");
                isProcessSelected = false;
                isDllSelected = false;
                m_ProcessName = "";
                m_ProcessID = 0;
                m_labelProcess->setText("No process selected");
                m_labelProcess->setStyleSheet("color: red");
                m_labelDll->setText("No DLL selected");
                m_labelDll->setStyleSheet("color: red");
                m_buttonInject->setEnabled(false);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Warning : process or dll not selected.");
        m_buttonInject->setEnabled(false);
    }
}
