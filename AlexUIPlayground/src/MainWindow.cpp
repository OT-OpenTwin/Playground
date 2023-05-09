#include "MainWindow.h"

// OpenTwin header
#include "OTBlockEditor/BlockPickerWidget.h"
#include "OTBlockEditor/BlockNetworkEditor.h"

// Qt header
#include <QtCore/qsettings.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
static MainWindow * g_instance{ nullptr };

#include <thread>

void MainWindow::createOwnWidgets(void) {
	// Create widgets
	ot::BlockPicker* blockPicker = new ot::BlockPicker("Block Picker");
	blockPicker->setObjectName("BlockPicker");
	addDockWidget(Qt::LeftDockWidgetArea, blockPicker);

	ot::BlockNetworkEditor* networkEditor = new ot::BlockNetworkEditor;

	m_tabWidget->addTab(networkEditor, "Block Editor");

	// Fill data
	
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

void MainWindow::appendOutput(const QString& _text) {
	m_output->append(_text);
}

void MainWindow::slotInitialize(void) {
	createOwnWidgets();

	QSettings s("OpenTwin", "AlexUIPlayground");
	if (s.contains("WindowState"))	restoreState(s.value("WindowState", QByteArray()).toByteArray());
}

MainWindow::MainWindow()
{
	// Create output
	m_outputDock = new QDockWidget("Output");
	m_output = new QTextEdit;
	m_outputDock->setWidget(m_output);
	m_outputDock->setObjectName("UI_DOCK_Output");
	addDockWidget(Qt::BottomDockWidgetArea, m_outputDock);

	// Create tab widget
	m_tabWidget = new QTabWidget;
	setCentralWidget(m_tabWidget);

	// Create menu
	m_menuBar = new QMenuBar;
	setMenuBar(m_menuBar);

	QMenu * debug = m_menuBar->addMenu("Debug");
	connect(debug->addAction("Test"), &QAction::triggered, this, &MainWindow::test);

	// Restore State
	QSettings s("OpenTwin", "AlexUIPlayground");
	if (s.contains("IsMaximized")) {
		move(s.value("PosX", 0).toInt(), s.value("PosY", 0).toInt());
		bool isMax = s.value("IsMaximized", true).toBool();
		if (isMax) showMaximized();
		else {
			resize(s.value("SizeWidth", 800).toInt(), s.value("SizeHeight", 600).toInt());
			showNormal();
		}
	}
	else {
		// First start
		showMaximized();
	}

	QMetaObject::invokeMethod(this, &MainWindow::slotInitialize, Qt::QueuedConnection);
}
