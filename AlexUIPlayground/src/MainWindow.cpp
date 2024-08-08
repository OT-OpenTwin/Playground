// !                             ! \\
// !   Do not modify this file   ! \\ 
// !                             ! \\

// Playground header
#include "TestCode.h"
#include "MainWindow.h"

// OpenTwin Core
#include "OTCore/Size2D.h"
#include "OTCore/OTAssert.h"

// OpenTwin Gui
#include "OTGui/Property.h"
#include "OTGui/PropertyBool.h"
#include "OTGui/PropertyInt.h"
#include "OTGui/PropertyDouble.h"
#include "OTGui/PropertyColor.h"
#include "OTGui/PropertyString.h"
#include "OTGui/PropertyStringList.h"
#include "OTGui/PropertyFilePath.h"
#include "OTGui/PropertyDirectory.h"
#include "OTGui/PropertyGroup.h"
#include "OTGui/PropertyGridCfg.h"

// OpenTwin Widgets
#include "OTWidgets/PropertyGrid.h"
#include "OTWidgets/PropertyDialog.h"
#include "OTWidgets/PropertyGridItem.h"
#include "OTWidgets/PropertyGridGroup.h"
#include "OTWidgets/PlainTextEditView.h"
#include "OTWidgets/WidgetViewManager.h"
#include "OTWidgets/WidgetViewFactory.h"

// OpenTwin Communication
#include "OTCommunication/ActionTypes.h"

// Qt header
#include <QtCore/qthread.h>
#include <QtCore/qsettings.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmessagebox.h>
#include <QtOpenGLWidgets/qopenglwidget.h>

// std header
#include <list>
#include <sstream>

#define BUILD_INFO "Alex UI Playground - Build " + QString(__DATE__) + " - " + QString(__TIME__) + "\n\n"

MainWindow* MainWindow::instance(void) {
	static MainWindow* g_instance{ nullptr };
	if (!g_instance) g_instance = new MainWindow;
	return g_instance;
}

void MainWindow::initialize(void) {
	if (m_initialized) {
		OT_LOG_E("Already initialized");
		return;
	}

	this->setDeleteLogNotifierLater(true);
	ot::LogDispatcher::instance().addReceiver(this);

	// Initialize view management
	this->setCentralWidget(ot::WidgetViewManager::instance().getDockManager());

	// Create OpenGLWidget
	QOpenGLWidget* newGLWidget = new QOpenGLWidget;
	newGLWidget->setFixedSize(1, 1);

	// Create menu bar
	m_menuBar = new QMenuBar;
	m_menuBar->setCornerWidget(newGLWidget, Qt::TopLeftCorner);
	this->setMenuBar(m_menuBar);

	// Add dock toggle action
	QAction* viewsAction = ot::WidgetViewManager::instance().getDockToggleAction();
	m_menuBar->addAction(viewsAction);

	// Create output
	m_logView = new ot::PlainTextEditView;
	m_logView->setViewData(ot::WidgetViewBase("Output", "Output", ot::WidgetViewBase::Bottom, ot::WidgetViewBase::ViewIsSide));
	m_logView->setViewIsProtected(true);
	m_logView->getViewDockWidget()->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	m_logView->setReadOnly(true);
	m_logView->setAutoScrollToBottomEnabled(true);
	{
		QFont f = m_logView->font();
		f.setFamily("Courier");
		f.setFixedPitch(true);
		m_logView->setFont(f);
	}
	ot::WidgetViewManager::instance().addView(ot::BasicServiceInformation(), m_logView);

	// Restore State
	QSettings s("OpenTwin", "AlexUIPlayground");
	bool isMax = true;
	if (s.contains("IsMaximized")) {
		isMax = s.value("IsMaximized", true).toBool();
		if (!isMax) {
			this->move(s.value("PosX", 0).toInt(), s.value("PosY", 0).toInt());
			this->resize(s.value("SizeWidth", 800).toInt(), s.value("SizeHeight", 600).toInt());
		}
	}

	if (s.contains("WindowState")) ot::WidgetViewManager::instance().restoreState(s.value("WindowState", QByteArray()).toByteArray().toStdString());

	if (isMax) {
		this->showMaximized();
	}
	else {
		this->showNormal();
	}

	QMetaObject::invokeMethod(this, &MainWindow::slotInitialize, Qt::QueuedConnection);
}

void MainWindow::closeEvent(QCloseEvent * _event) {
	QSettings s("OpenTwin", "AlexUIPlayground");
	s.setValue("WindowState", QByteArray::fromStdString(ot::WidgetViewManager::instance().saveState()));
	s.setValue("IsMaximized", isMaximized());
	s.setValue("PosX", pos().x());
	s.setValue("PosY", pos().y());
	s.setValue("SizeWidth", size().width());
	s.setValue("SizeHeight", size().height());

	QMainWindow::closeEvent(_event);
}

void MainWindow::log(const ot::LogMessage& _message) {
	if (QThread::currentThread() == m_mainThread) {
		this->slotLog(_message);
	}
	else {
		QMetaObject::invokeMethod(this, "slotLog", Qt::QueuedConnection, Q_ARG(const ot::LogMessage&, _message));
	}
}

void MainWindow::slotInitialize(void) {
	try {
		m_testCode.initialize(this);
	}
	catch (const std::exception& _e) {
		QMessageBox msg(QMessageBox::Critical, "Error", "Orror while creating own widgets:\n" + QString(_e.what()), QMessageBox::Ok | QMessageBox::StandardButton::Close);
		QMessageBox::StandardButton result = (QMessageBox::StandardButton) msg.exec();
		if (result == QMessageBox::Cancel) {
			exit(0);
		}
	}
	catch (...) {
		QMessageBox msg(QMessageBox::Critical, "Error", "Orror while creating own widgets:\n [FATAL] Unknown error", QMessageBox::Ok);
		msg.exec();
	}
}

void MainWindow::slotLog(const ot::LogMessage& _message) {
	std::stringstream stream;
	stream << _message;
	m_logView->appendPlainText(QString::fromStdString(stream.str()));
}

MainWindow::MainWindow() 
	: m_mainThread(QThread::currentThread()), m_initialized(false), m_menuBar(nullptr), m_logView(nullptr)
{
	
}
