#include "MainWindow.h"

// Qt header
#include <QtCore/qsettings.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>

#include <thread>

static MainWindow * g_instance{ nullptr };

#include "OTBlockEditor/BlockPickerDockWidget.h"
#include "OTBlockEditor/BlockPickerWidget.h"
#include "OTBlockEditor/BlockNetworkEditor.h"

#include "OTBlockEditorAPI/BlockCategoryConfiguration.h"
#include "OTBlockEditorAPI/BlockConfiguration.h"

#include <list>

void MainWindow::createOwnWidgets(void) {
	// Create widgets
	ot::BlockPickerDockWidget* blockPicker = new ot::BlockPickerDockWidget("Block Picker");
	blockPicker->setObjectName("BlockPicker");
	addDockWidget(Qt::LeftDockWidgetArea, blockPicker);

	ot::BlockNetworkEditor* networkEditor = new ot::BlockNetworkEditor;

	m_tabWidget->addTab(networkEditor, "Block Editor");

	// Fill data
	ot::BlockCategoryConfiguration* root1 = new ot::BlockCategoryConfiguration("r1", "Root 1");
	ot::BlockCategoryConfiguration* r1A = new ot::BlockCategoryConfiguration("A", "A");
	ot::BlockCategoryConfiguration* r1B = new ot::BlockCategoryConfiguration("B", "B");
	root1->addChild(r1A);
	root1->addChild(r1B);

	ot::BlockCategoryConfiguration* root2 = new ot::BlockCategoryConfiguration("r2", "Root 2");
	ot::BlockCategoryConfiguration* r2C = new ot::BlockCategoryConfiguration("C", "C");
	root2->addChild(r2C);

	ot::BlockCategoryConfiguration* root3 = new ot::BlockCategoryConfiguration("r3", "Root 3");
	ot::BlockCategoryConfiguration* r3D = new ot::BlockCategoryConfiguration("D", "D");
	root3->addChild(r3D);

	std::list<ot::BlockCategoryConfiguration*> rootItems;
	rootItems.push_back(root1);
	rootItems.push_back(root2);
	rootItems.push_back(root3);

	blockPicker->addTopLevelCategories(rootItems);
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
