#include "AppBase.h"
#include "TestCode.h"
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlayout.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qplaintextedit.h>
#include <QtWidgets/qdockwidget.h>
#include <QtCore/qdatetime.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qaction.h>



AppBase* AppBase::getInstance()
{
    static AppBase* g_instance{nullptr};

    if (g_instance == nullptr)
    {
        g_instance = new AppBase;
    }

    return g_instance;
}

AppBase::AppBase()
    
{
    


    //Create Layouts
    


    

    //Create Controls
    m_LogDock = new QDockWidget;
    m_LogEdit= new QPlainTextEdit;
    m_TabView = new QTabWidget;
    m_Menu = new QMenuBar;
   
    //Setup Controls
    m_LogDock->setWidget(m_LogEdit);
    m_LogEdit->setReadOnly(true);
    
    
    QMenu* fileMenu = m_Menu->addMenu("File");
    m_exitButton = fileMenu->addAction("Exit");
    //Setup Layouts
    

    
    //Setup Window
    this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_LogDock);
    this->setMenuBar(m_Menu);
    this->setCentralWidget(m_TabView);
    //Connect Signals
    connect(m_exitButton, &QAction::triggered, this, &AppBase::slotExit);

    QMetaObject::invokeMethod(this, &AppBase::slotInitialize, Qt::QueuedConnection);
}

AppBase::~AppBase()
{}

void AppBase:: log(const ot::LogMessage& _message)
{
    

    QTextCursor cursor = m_LogEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();
    QTextCharFormat formatTime = format;
    QTextCharFormat formatSender = format;
    QTextCharFormat formatWarning = format;
    QTextCharFormat formatError = format;

    formatTime.setForeground(QBrush(QColor(192, 128, 255)));
    formatSender.setForeground(QBrush(QColor(128, 192, 255)));
    formatWarning.setForeground(QBrush(QColor(255, 242, 0)));
    formatError.setForeground(QBrush(QColor(255, 0, 0)));

    cursor.insertText("[");
    cursor.setCharFormat(formatTime);
    cursor.insertText(QString::fromStdString(_message.localSystemTime()));
    cursor.setCharFormat(format);
    cursor.insertText("] ");

    if (!_message.functionName().empty())
    {
        cursor.insertText("[");
        cursor.setCharFormat(formatSender);
        cursor.insertText(QString::fromStdString(_message.functionName()));
        cursor.setCharFormat(format);
        cursor.insertText("] ");
    }

    if (_message.flags() & ot::WARNING_LOG)
    {
        cursor.insertText("[");
        cursor.setCharFormat(formatWarning);
        cursor.insertText("Warning");
        cursor.setCharFormat(format);
        cursor.insertText("] ");

    }
    else if (_message.flags() & ot::ERROR_LOG)
    {
        cursor.insertText("[");
        cursor.setCharFormat(formatError);
        cursor.insertText("Error");
        cursor.setCharFormat(format);
        cursor.insertText("] ");

    }

    cursor.insertText(QString::fromStdString(_message.text()));

    m_LogEdit->setTextCursor(cursor);
    m_LogEdit->appendPlainText("");


}

void AppBase::slotExit()
{
    this->close();
}

void AppBase::slotInitialize()
{
    new TestCode;
}


