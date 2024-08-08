// !                             ! \\
// !   Do not modify this file   ! \\ 
// !                             ! \\

#pragma once

// Playground header
#include "TestCodes.h"

// OpenTwin header
#include "OTCore/Logger.h"
#include "OTCore/OTClassHelper.h"

// Qt header
#include <QtWidgets/qmainwindow.h>

class TestCode;
class QThread;
class QMenuBar;
namespace ot { class PlainTextEditView; }

class MainWindow : public QMainWindow, public ot::AbstractLogNotifier
{
    Q_OBJECT
	OT_DECL_NOCOPY(MainWindow)
public:
	static MainWindow* instance(void);

	void initialize(void);

	// ###########################################################################################################################################################################################################################################################################################################################

	// Event handling

	virtual void closeEvent(QCloseEvent * _event) override;

	//! @brief Called when the a log message was created
	virtual void log(const ot::LogMessage& _message) override;

	// ###########################################################################################################################################################################################################################################################################################################################

private Q_SLOTS:
	void slotInitialize(void);
	void slotLog(const ot::LogMessage& _message);

	// ###########################################################################################################################################################################################################################################################################################################################

private:
	MainWindow();

	bool m_initialized;
	QThread* m_mainThread;
	TestCode m_testCode;
	QMenuBar* m_menuBar;
	ot::PlainTextEditView* m_logView;
};
