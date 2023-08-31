#include "MainWindow.h"

#include "OpenTwinCore/otAssert.h"
#include "OpenTwinCore/SimpleFactory.h"

#include "OpenTwinCommunication/ActionTypes.h"

#include "OTGui/GraphicsItemCfg.h"
#include "OTGui/GraphicsLayoutItemCfg.h"
#include "OTGui/GraphicsCollectionCfg.h"
#include "OTGui/GraphicsEditorPackage.h"

#include "OTWidgets/GraphicsView.h"
#include "OTWidgets/GraphicsScene.h"
#include "OTWidgets/GraphicsItem.h"
#include "OTWidgets/GraphicsLayoutItem.h"
#include "OTWidgets/GraphicsFactory.h"
#include "OTWidgets/GraphicsPicker.h"

// Qt header
#include <QtCore/qsettings.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qgraphicswidget.h>
#include <QtWidgets/qgraphicsproxywidget.h>
#include <QtWidgets/qgraphicslinearlayout.h>

#include <thread>

static MainWindow * g_instance{ nullptr };

#include <list>
#include <sstream>

ot::GraphicsItemCfg* createTestBlockCfg1(const std::string& _name) {
	ot::GraphicsImageItemCfg* bot = new ot::GraphicsImageItemCfg;
	bot->setSize(ot::Size2D(100, 60));
	bot->setName(_name + "i");
	bot->setTitle(_name + "i");
	bot->setImagePath("Default/BugGreen");
	bot->setGraphicsItemFlags(ot::GraphicsItemCfg::ItemIsConnectable);

	ot::GraphicsTextItemCfg* topA = new ot::GraphicsTextItemCfg("Senor");
	topA->setName(_name + "tA");
	topA->setTitle(_name + "tA");

	ot::GraphicsTextItemCfg* topB = new ot::GraphicsTextItemCfg("Buggo");
	topB->setName(_name + "tB");
	topB->setTitle(_name + "tB");

	ot::GraphicsVBoxLayoutItemCfg* top = new ot::GraphicsVBoxLayoutItemCfg;
	top->setName(_name + "t");
	top->setTitle(_name + "t");
	top->addChildItem(topA);
	top->addChildItem(topB);
	top->setSize(ot::Size2D(100, 50));

	ot::GraphicsRectangularItemCfg* backgr = new ot::GraphicsRectangularItemCfg;
	backgr->setName(_name + "b");
	backgr->setTitle(_name + "b");
	backgr->setCornerRadius(5);
	backgr->setSize(ot::Size2D(100, 50));

	ot::GraphicsStackItemCfg* cfgTop = new ot::GraphicsStackItemCfg;
	cfgTop->setName(_name + "s");
	cfgTop->setTitle(_name + "s");
	cfgTop->setSize(ot::Size2D(100, 50));
	cfgTop->addItemTop(bot, false);
	cfgTop->addItemTop(top, true);

	ot::GraphicsStackItemCfg* cfg = new ot::GraphicsStackItemCfg;
	cfg->setName(_name);
	cfg->setTitle(_name);
	cfg->setSize(ot::Size2D(100, 50));
	cfg->addItemTop(backgr, false);
	cfg->addItemTop(cfgTop, true);


	return cfg;
}

ot::GraphicsItemCfg* createTestBlockCfg2(const std::string& _name) {
	ot::GraphicsRectangularItemCfg* r1 = new ot::GraphicsRectangularItemCfg;
	r1->setSize(ot::Size2D(100, 50));

	ot::GraphicsTextItemCfg * txt = new ot::GraphicsTextItemCfg;
	txt->setText("Henlo");

	ot::GraphicsStackItemCfg* stack = new ot::GraphicsStackItemCfg;
	stack->setName(_name);
	stack->setTitle(_name);
	stack->addItemTop(r1, true);
	stack->addItemTop(txt, false);

	return stack;
}

void MainWindow::createOwnWidgets(void) {
	ot::GraphicsView* editor = new ot::GraphicsView;
	editor->setDropsEnabled(true);

	m_tabWidget->addTab(editor, "Editor");

	ot::GraphicsCollectionCfg* col = new ot::GraphicsCollectionCfg("Test", "Test");
	col->addItem(createTestBlockCfg1("T1"));
	col->addItem(createTestBlockCfg2("T2"));
	ot::GraphicsEditorPackage pckg("Test", "Test Editor");
	pckg.addCollection(col);

	
	ot::GraphicsPickerDockWidget* dock = new ot::GraphicsPickerDockWidget("Picker");
	dock->pickerWidget()->add(pckg);

	this->addDockWidget(Qt::LeftDockWidgetArea, dock);
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
