#include "MainWindow.h"

// Qt header
#include <QtCore/qsettings.h>
#include "OTGui/FillPainter2D.h"
#include "OTGui/LinearGradientPainter2D.h"
#include "OTGui/GraphicsItemCfg.h"
#include "OTGui/GraphicsLayoutItemCfg.h"
#include "OTWidgets/GraphicsView.h"
#include "OTWidgets/GraphicsScene.h"
#include "OTWidgets/GraphicsItem.h"
#include "OTWidgets/GraphicsLayoutItem.h"
#include "OTWidgets/GraphicsFactory.h"


#include "OpenTwinCore/otAssert.h"
#include "OpenTwinCore/SimpleFactory.h"
#include "OpenTwinCommunication/ActionTypes.h"
#include "OTGui/GraphicsItemCfg.h"
#include "OTGui/GraphicsCollectionCfg.h"
#include "OTGui/GraphicsLayoutItemCfg.h"
#include "OTGui/GraphicsEditorPackage.h"
#include "OTWidgets/GraphicsItem.h"
#include "OTWidgets/GraphicsLayoutItem.h"

#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qgraphicsproxywidget.h>
#include <QtWidgets/qmessagebox.h>

#include <thread>

static MainWindow * g_instance{ nullptr };

#include <list>
#include <sstream>

ot::GraphicsItemCfg* createTestBlock(const std::string& _name) {
	ot::GraphicsVBoxLayoutItemCfg* centralLayout = new ot::GraphicsVBoxLayoutItemCfg;

	ot::GraphicsTextItemCfg* title = new ot::GraphicsTextItemCfg;
	title->setName(_name);
	title->setText(_name);
	title->setBorder(ot::Border(ot::Color(rand() % 255, rand() % 255, rand() % 255), 2));

	ot::GraphicsHBoxLayoutItemCfg* midLayout = new ot::GraphicsHBoxLayoutItemCfg;

	ot::GraphicsRectangularItemCfg* leftRect = new ot::GraphicsRectangularItemCfg;
	leftRect->setSize(ot::Size2D(20, 20));
	ot::GraphicsRectangularItemCfg* rightRect = new ot::GraphicsRectangularItemCfg;
	rightRect->setSize(ot::Size2D(30, 30));

	midLayout->addChildItem(leftRect);
	midLayout->addStrech(1);
	midLayout->addChildItem(rightRect);

	centralLayout->addChildItem(title);
	centralLayout->addChildItem(midLayout, 1);

	return centralLayout;
}

ot::GraphicsItemCfg* createTestBlock2(const std::string& _name) {
	ot::GraphicsVBoxLayoutItemCfg* centralLayout = new ot::GraphicsVBoxLayoutItemCfg;

	ot::GraphicsTextItemCfg* title = new ot::GraphicsTextItemCfg;
	title->setName(_name);
	title->setText(_name);
	title->setBorder(ot::Border(ot::Color(rand() % 255, rand() % 255, rand() % 255), 2));

	ot::GraphicsHBoxLayoutItemCfg* midLayout = new ot::GraphicsHBoxLayoutItemCfg;

	ot::GraphicsRectangularItemCfg* leftRect = new ot::GraphicsRectangularItemCfg;
	ot::GraphicsRectangularItemCfg* rightRect = new ot::GraphicsRectangularItemCfg;

	midLayout->addChildItem(leftRect);
	midLayout->addStrech(1);
	midLayout->addChildItem(rightRect);

	centralLayout->addChildItem(title);
	centralLayout->addChildItem(midLayout, 1);

	return centralLayout;
}

void MainWindow::createOwnWidgets(void) {
	ot::GraphicsView* editor = new ot::GraphicsView;
	ot::GraphicsScene* sc = new ot::GraphicsScene;
	editor->setScene(sc);

	m_tabWidget->addTab(editor, "Editor");

	ot::GraphicsEditorPackage pckg("TestPackage", "Test title");
	ot::GraphicsCollectionCfg* a = new ot::GraphicsCollectionCfg("A", "A");
	ot::GraphicsCollectionCfg* a1 = new ot::GraphicsCollectionCfg("1", "1");
	//ot::GraphicsCollectionCfg* a2 = new ot::GraphicsCollectionCfg("2", "2");
	a->addChildCollection(a1);
	a1->addItem(createTestBlock("Alpha 1"));
	//a->addChildCollection(a2);
	//a2->addItem(createTestBlock2("Alpha 2"));
	pckg.addCollection(a);

	OT_rJSON_createDOC(doc);
	OT_rJSON_createValueObject(pckgObj);
	pckg.addToJsonObject(doc, pckgObj);

	ot::rJSON::add(doc, OT_ACTION_MEMBER, OT_ACTION_CMD_UI_GRAPHICSEDITOR_CreateEmptyGraphicsEditor);
	ot::rJSON::add(doc, OT_ACTION_PARAM_GRAPHICSEDITOR_Package, pckgObj);

	std::string str = ot::rJSON::toJSON(doc);
	OT_LOG_D("L1: " + std::to_string(str.length()));

	try {
		OT_rJSON_parseDOC(docI, str.c_str());

		ot::GraphicsEditorPackage pckg("", "");
		OT_rJSON_val o = docI.GetObject();
		OT_rJSON_val pckgObj = o[OT_ACTION_PARAM_GRAPHICSEDITOR_Package].GetObject();

		pckg.setFromJsonObject(pckgObj);

		OT_rJSON_createDOC(docTest);
		OT_rJSON_createValueObject(pckgObjTest);
		pckg.addToJsonObject(docTest, pckgObjTest);

		ot::rJSON::add(docTest, OT_ACTION_MEMBER, OT_ACTION_CMD_UI_GRAPHICSEDITOR_CreateEmptyGraphicsEditor);
		ot::rJSON::add(docTest, OT_ACTION_PARAM_GRAPHICSEDITOR_Package, pckgObjTest);

		std::string strTest = ot::rJSON::toJSON(docTest);
		OT_LOG_D("L2: " + std::to_string(strTest.length()));
	}
	catch (const std::exception& _e) {
		QMessageBox msg(QMessageBox::Warning, "Error", _e.what(), QMessageBox::Ok);
		msg.exec();
	}
	catch (...) {
		OT_LOG_E("Unknown error");
	}
	OT_LOG_D("Custom done");
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

MainWindow::MainWindow()
{
	this->AbstractLogNotifier::deleteLater(true);
	ot::LogDispatcher::instance().addReceiver(this);

	// Create output
	m_outputDock = new QDockWidget("Output");
	m_output = new QTextEdit;
	m_outputDock->setWidget(m_output);
	m_outputDock->setObjectName("UI_DOCK_Output");
	addDockWidget(Qt::BottomDockWidgetArea, m_outputDock);

	m_logDock = new QDockWidget("Log");
	m_log = new QTextEdit;
	m_logDock->setWidget(m_log);
	m_logDock->setObjectName("UI_DOCK_LOG");
	tabifyDockWidget(m_outputDock, m_logDock);

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
