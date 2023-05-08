#include "MainWindow.h"

// OpenTwin header
#include <openTwin/BlockNetworkEditor.h>
#include <openTwin/BlockNetwork.h>
#include <openTwin/Block.h>
#include <openTwin/FlowBlock.h>
#include <openTwin/FlowConnector.h>
#include <openTwin/FlowConnection.h>

// Qt header
#include <QtCore/qsettings.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
MainWindow * g_instance{ nullptr };

#include <thread>

ot::BlockNetworkEditor* g_editor;

ot::FlowBlock* leftBlock;
ot::FlowBlock* rightBlock;

void MainWindow::createOwnWidgets(void) {
	g_editor = new ot::BlockNetworkEditor;

	// Left
	leftBlock = new ot::FlowBlock("Calculate");
	leftBlock->setTitleImage(QPixmap(QString(":/images/f.png")));

	ot::FlowConnector* leftOut0 = new ot::FlowConnector("Start", ot::OutputFlow, ot::Triangle);
	leftOut0->setFillColor(QColor(128, 128, 128));
	leftOut0->setBorderColor(QColor(255, 255, 255));

	ot::FlowConnector* leftOut1 = new ot::FlowConnector("Initial Data", ot::OutputFlow, ot::Circle);
	leftOut1->setFillColor(QColor(0, 255, 128));

	leftBlock->addConnector(leftOut0);
	leftBlock->addConnector(leftOut1);

	if (false) {
		for (int i = 0; i < 5; i++) {
			ot::FlowConnector* leftOut2 = new ot::FlowConnector("Some stuff " + QString::number(i), ot::OutputFlow, ot::Triangle);
			leftOut2->setFillColor(QColor(68, 255, 68));
			leftBlock->addConnector(leftOut2);
		}
	}

	// Right
	rightBlock = new ot::FlowBlock("Calculate stuff");
	rightBlock->setTitleColor(QColor(128, 0, 255));

	ot::FlowConnector* rightIn0 = new ot::FlowConnector("Run", ot::InputFlow, ot::Triangle);
	rightIn0->setFillColor(QColor(128, 128, 128));
	rightIn0->setBorderColor(QColor(255, 255, 255));

	ot::FlowConnector* rightIn1 = new ot::FlowConnector("Data", ot::InputFlow, ot::Circle);
	rightIn1->setFillColor(QColor(0, 255, 128));

	ot::FlowConnector* rightOut0 = new ot::FlowConnector("Success", ot::OutputFlow, ot::Triangle);
	rightOut0->setFillColor(QColor(128, 128, 128));
	rightOut0->setBorderColor(QColor(255, 255, 255));

	ot::FlowConnector* rightOut1 = new ot::FlowConnector("Fail", ot::OutputFlow, ot::Triangle);
	rightOut1->setFillColor(QColor(128, 128, 128));
	rightOut1->setBorderColor(QColor(255, 255, 255));

	ot::FlowConnector* rightOut2 = new ot::FlowConnector("ResultA", ot::OutputFlow, ot::Circle);
	rightOut2->setFillColor(QColor(255, 0, 128));

	ot::FlowConnector* rightOut3 = new ot::FlowConnector("ResultB", ot::OutputFlow, ot::Circle);
	rightOut3->setFillColor(QColor(255, 128, 0));

	rightBlock->addConnector(rightIn0);
	rightBlock->addConnector(rightIn1);

	rightBlock->addConnector(rightOut0);
	rightBlock->addConnector(rightOut1);
	rightBlock->addConnector(rightOut2);
	rightBlock->addConnector(rightOut3);

	// Fill network
	g_editor->network()->addBlock(leftBlock, QPoint(-80, 0));
	g_editor->network()->addBlock(rightBlock, QPoint(80, 0));

	// Create connections
	g_editor->network()->registerConnection(leftOut0->connectTo(rightIn0));
	g_editor->network()->registerConnection(leftOut1->connectTo(rightIn1));

	m_tabWidget->addTab(g_editor, "Block Editor");
}

void MainWindow::test(void) {
	g_editor->network()->update();
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
