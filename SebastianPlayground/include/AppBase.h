#pragma once

#include <QtWidgets/qmainwindow.h>
#include "OpenTwinCore/Logger.h"

class QVBoxLayout;
class QLabel;
class QPushButton;
class QWidget;
class QDockWidget;
class QPlainTextEdit;
class QTabWidget;
class QMenuBar;
class QAction;

class AppBase : public QMainWindow, public ot::AbstractLogNotifier
{
    Q_OBJECT

public:
    static AppBase* getInstance();
    ~AppBase();

    virtual void log(const ot::LogMessage& _message) override;
    QTabWidget* getTabView() {return m_TabView;}
    QMenuBar* getMenuBar() { return m_Menu; }
private:

    AppBase();

    QDockWidget* m_LogDock;
    QPlainTextEdit* m_LogEdit;
    QTabWidget* m_TabView;
    QMenuBar* m_Menu;
    QAction* m_exitButton;

private slots: 
    void slotExit();
    void slotInitialize();
    
    
};
