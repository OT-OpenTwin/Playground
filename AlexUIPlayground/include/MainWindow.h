#pragma once

// OpenTwin header
#include "OpenTwinCore/Logger.h"

#include <QtWidgets/qmainwindow.h>

class QDockWidget;
class QTextEdit;
class QTabWidget;
class QMenuBar;

class MainWindow : public QMainWindow, public ot::AbstractLogNotifier
{
    Q_OBJECT
public:
	void createOwnWidgets(void);

public slots:
	void test(void);

	// #########################################################################################################################################

	// #########################################################################################################################################

	// #########################################################################################################################################

public:
	static MainWindow * instance(void);
	virtual void closeEvent(QCloseEvent * _event) override;

	void queueAppendOutput(const QString& _text);

	//! @brief Called when the a log message was created
	virtual void log(const ot::LogMessage& _message) override;

public slots:
	void appendOutput(const QString& _text);

private slots:
	void slotInitialize(void);

private:
	MainWindow();

	QDockWidget *		m_outputDock;
	QTextEdit *			m_output;

	QDockWidget* m_logDock;
	QTextEdit* m_log;

	QTabWidget *		m_tabWidget;

	QMenuBar *			m_menuBar;
};

#define AK_LOG(___message) MainWindow::instance()->appendOutput(___message)
#define AK_LOG_E(___message) MainWindow::instance()->appendOutput(QString("[ERROR] ") + ___message)