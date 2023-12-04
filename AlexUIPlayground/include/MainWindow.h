#pragma once

// OpenTwin header
#include "OTCore/Logger.h"

#include <QtWidgets/qmainwindow.h>

class QDockWidget;
class QTextEdit;
class QTabWidget;
class QMenuBar;

namespace ot { class GraphicsView; };

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

	QTextEdit *			m_output;
	QTextEdit*          m_log;

	QMenuBar *			m_menuBar;

	ot::GraphicsView*   m_gview;
	int                 m_viewId;
};

#define AK_LOG(___message) MainWindow::instance()->appendOutput(___message)
#define AK_LOG_E(___message) MainWindow::instance()->appendOutput(QString("[ERROR] ") + ___message)