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

#include "OTBlockEditor/BlockNetworkEditor.h"

#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qgraphicsproxywidget.h>


#include <thread>

static MainWindow * g_instance{ nullptr };



#include <list>
#include <sstream>

ot::GraphicsItemCfg* optA(void) {
	ot::GraphicsVBoxLayoutItemCfg* root = new ot::GraphicsVBoxLayoutItemCfg;
	root->setSize(ot::Size2D(100, 100));
	ot::GraphicsHBoxLayoutItemCfg* mid = new ot::GraphicsHBoxLayoutItemCfg;

	ot::GraphicsTextItemCfg* title = new ot::GraphicsTextItemCfg;
	title->setText("Hey");
	title->setTextColor(ot::Color(255, 0, 0));
	title->setSize(ot::Size2D(200, 100));

	ot::GraphicsRectangularItemCfg* left = new ot::GraphicsRectangularItemCfg;
	left->setSize(ot::Size2D(20, 20));

	ot::GraphicsRectangularItemCfg* right = new ot::GraphicsRectangularItemCfg;
	right->setSize(ot::Size2D(20, 20));

	mid->addChildItem(left);
	mid->addStrech(1);
	mid->addChildItem(right);

	root->addChildItem(title);
	root->addChildItem(mid);
	
	return root;
}

ot::GraphicsItemCfg* optB() {
	return nullptr;
}

void addChildsToScene(QGraphicsScene* _scene, QGraphicsItemGroup * _group, QGraphicsLayout* _layout) {
	for (int i = 0; i < _layout->count(); i++) {
		auto itm = _layout->itemAt(i);
		if (itm) {
			QGraphicsItem* citm = dynamic_cast<QGraphicsItem*>(itm);
			QGraphicsLayout* clitm = dynamic_cast<QGraphicsLayout*>(itm);
			if (citm) {
				_scene->addItem(citm);
				_group->addToGroup(citm);
			}
			else if (clitm) addChildsToScene(_scene, _group, clitm);
			else {
				otAssert(0, "was weiss ich");
			}
		}
	}
}

void MainWindow::createOwnWidgets(void) {
	ot::BlockNetworkEditor* editor = new ot::BlockNetworkEditor;

	m_tabWidget->addTab(editor, "Editor");
	QGraphicsItemGroup* group = new QGraphicsItemGroup;
	group->setFlag(QGraphicsItem::ItemIsMovable, true);
	group->setFlag(QGraphicsItem::ItemIsSelectable, true);

	ot::GraphicsItem* itm = ot::GraphicsFactory::itemFromConfig(optA());

	if (itm) {
		QGraphicsItem* citm = dynamic_cast<QGraphicsItem*>(itm);
		QGraphicsLayout* clitm = dynamic_cast<QGraphicsLayout*>(itm);
		if (citm) {
			editor->scene()->addItem(citm);
		}
		else if (clitm) {
			QGraphicsWidget* container = new QGraphicsWidget;
			container->setLayout(clitm);
			addChildsToScene(editor->scene(), group, clitm);
			editor->scene()->addItem(group);
			container->setFlag(QGraphicsItem::ItemIsMovable, true);
			container->setFlag(QGraphicsItem::ItemIsSelectable, true);
			editor->scene()->addItem(container);
		}
		else {
			OT_LOG_EA("Cast fail");
		}
	}
	else {
		OT_LOG_EA("Factory fail");
	}

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

	if (s.contains("WindowState"))	restoreState(s.value("WindowState", QByteArray()).toByteArray());

	QMetaObject::invokeMethod(this, &MainWindow::slotInitialize, Qt::QueuedConnection);
}
