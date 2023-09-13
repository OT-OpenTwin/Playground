#include "MainWindow.h"
#include "testcode.h"

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
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgraphicswidget.h>
#include <QtWidgets/qgraphicsproxywidget.h>
#include <QtWidgets/qgraphicslinearlayout.h>

#include <thread>

static MainWindow * g_instance{ nullptr };

#include <list>
#include <sstream>

ot::GraphicsItemCfg* createTestBlockCfg1(const std::string& _name) {

	return nullptr;
}

ot::GraphicsItemCfg* createTestBlockCfg2(const std::string& _name) {

	return nullptr;
}

void MainWindow::createOwnWidgets(void) {
	ot::GraphicsView* editor = new ot::GraphicsView;

	m_tabWidget->addTab(editor, "Editor");

	ot::GraphicsPickerDockWidget* dock = new ot::GraphicsPickerDockWidget("Picker");

	// Proxy
	//{
	//	QPushButton* btn1 = new QPushButton("X");

	//	QWidget* w = new QWidget;
	//	QGridLayout* lay = new QGridLayout(w);
	//	lay->addWidget(btn1, 0, 0);

	//	//QGraphicsProxyWidget* proxy = editor->getGraphicsScene()->addWidget(w);
	//	//proxy->setFlag(QGraphicsItem::ItemIsMovable);
	//	//proxy->setFlag(QGraphicsItem::ItemIsSelectable);
	//}

	// Rect item
	/*{
		QGraphicsWidget* gw = new QGraphicsWidget;
		QGraphicsGridLayout* lay = new QGraphicsGridLayout;
		gw->setLayout(lay);

		ot::GraphicsRectangularItem* r1 = new ot::GraphicsRectangularItem;
		ot::GraphicsRectangularItem* r2 = new ot::GraphicsRectangularItem;
		r2->setRectangleSize(QSizeF(20., 20.));
		ot::GraphicsRectangularItem* r3 = new ot::GraphicsRectangularItem;
		ot::GraphicsRectangularItem* r4 = new ot::GraphicsRectangularItem;
		ot::GraphicsRectangularItem* r5 = new ot::GraphicsRectangularItem;
		ot::GraphicsRectangularItem* r6 = new ot::GraphicsRectangularItem;
		r6->setRectangleSize(QSizeF(20., 20.));

		ot::GraphicsTextItem* t1 = new ot::GraphicsTextItem;
		t1->setPlainText("T1");

		ot::GraphicsTextItem* t2 = new ot::GraphicsTextItem;
		t2->setPlainText("Tes 2");

		ot::GraphicsTextItem* t3 = new ot::GraphicsTextItem;
		t3->setPlainText("Testing 3");

		ot::GraphicsTextItem* t4 = new ot::GraphicsTextItem;
		t4->setPlainText("Very long test 4");

		ot::GraphicsEllipseItem* e1 = new ot::GraphicsEllipseItem;
		e1->setRadius(5., 7.5);

		ot::GraphicsEllipseItem* e2 = new ot::GraphicsEllipseItem;
		e2->setRadius(7.5, 5.0);

		lay->addItem(r1, 0, 0);
		lay->addItem(t1, 0, 1);
		lay->addItem(t2, 0, 2);
		lay->addItem(r3, 0, 3);

		lay->addItem(r4, 1, 0);
		lay->addItem(t3, 1, 1);
		lay->addItem(t4, 1, 2);
		lay->addItem(r6, 1, 3);

		gw->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

		editor->getGraphicsScene()->addItem(gw);
	}*/

	editor->getGraphicsScene()->addItem(new Window);

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
