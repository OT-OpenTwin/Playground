#include "MainWindow.h"
#include "testcode.h"

#include "OTCore/otAssert.h"
#include "OTCore/Size2D.h"
#include "OTCore/SimpleFactory.h"

#include "OTCommunication/ActionTypes.h"

// Qt header
#include <QtCore/qsettings.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgraphicswidget.h>
#include <QtWidgets/qgraphicsproxywidget.h>
#include <QtWidgets/qgraphicslinearlayout.h>

#include <thread>

static MainWindow * g_instance{ nullptr };

#include <list>
#include <sstream>

void MainWindow::createOwnWidgets(void) {
	
}

void MainWindow::test(void) {
	//g_editor->network()->update();
}

// ^ Add custom code above ^

// #########################################################################################################################################

// #########################################################################################################################################

// #########################################################################################################################################

MainWindow * MainWindow::instance(void) {
	if (g_instance == nullptr) g_instance = new MainWindow;
	return g_instance;
}

void MainWindow::closeEvent(QCloseEvent * _event) {
	QSettings s("OpenTwin", "AlexUIPlayground");
	s.setValue("WindowState", saveState());
	s.setValue("IsMaximized", isMaximized());
	s.setValue("PosX", pos().x());
	s.setValue("PosY", pos().y());
	s.setValue("SizeWidth", size().width());
	s.setValue("SizeHeight", size().height());

	QMainWindow::closeEvent(_event);
}

void MainWindow::queueAppendOutput(const QString& _text) {
	QMetaObject::invokeMethod(this, "appendOutput", Qt::QueuedConnection, Q_ARG(const QString&, _text));
}

void MainWindow::log(const ot::LogMessage& _message) {
	std::stringstream stream;
	stream << _message;
	m_log->append(QString::fromStdString(stream.str()));
}

void MainWindow::appendOutput(const QString& _text) {
	m_output->append(_text);
}

void MainWindow::slotInitialize(void) {
	try {
		createOwnWidgets();
	}
	catch (const std::exception& _e) {
		appendOutput("\n[ERROR] Orror while creating own widgets:\n" + QString(_e.what()) + "\n\n");
	}
	catch (...) {
		appendOutput("\n[ERROR] Orror while creating own widgets:\n[FATAL] Unknown error\n\n");
	}
}

MainWindow::MainWindow() : m_gview(nullptr), m_viewId(0)
{
	this->AbstractLogNotifier::deleteLater(true);
	ot::LogDispatcher::instance().addReceiver(this);

	// Create output
	m_output = new QTextEdit;
	m_log = new QTextEdit;

	// Create menu
	m_menuBar = new QMenuBar;
	setMenuBar(m_menuBar);

	QMenu * debug = m_menuBar->addMenu("Debug");
	connect(debug->addAction("Test"), &QAction::triggered, this, &MainWindow::test);

	// Restore State
	QSettings s("OpenTwin", "AlexUIPlayground");
	if (s.contains("IsMaximized")) {
		bool isMax = s.value("IsMaximized", true).toBool();
		if (isMax) {
			showMaximized();
		}
		else {
			move(s.value("PosX", 0).toInt(), s.value("PosY", 0).toInt());
			resize(s.value("SizeWidth", 800).toInt(), s.value("SizeHeight", 600).toInt());
			showNormal();
		}
	}
	else {
		// First start
		showMaximized();
	}

	showMaximized();

	if (s.contains("WindowState"))	restoreState(s.value("WindowState", QByteArray()).toByteArray());

	QMetaObject::invokeMethod(this, &MainWindow::slotInitialize, Qt::QueuedConnection);
}
