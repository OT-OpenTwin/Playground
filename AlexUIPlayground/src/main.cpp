#include "MainWindow.h"

#include <QtCore/qdir.h>
#include <QtCore/qlocale.h>
#include <QtGui/qopenglcontext.h>
#include <QtGui/qopenglfunctions.h>
#include <QtGui/qoffscreensurface.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qapplication.h>

#include "OTCore/Logger.h"
#include "OTWidgets/IconManager.h"
#include "OTWidgets/GlobalColorStyle.h"
#include "OTWidgets/WidgetViewManager.h"
#include "OTWidgets/GraphicsItemLoader.h"
#include "OTWidgets/ApplicationPropertiesManager.h"

void initializeLogging(void) {
	ot::LogDispatcher::initialize("UIPlayground", false);
	ot::LogDispatcher::instance().setLogFlags(ot::ALL_GENERAL_LOG_FLAGS);
}

QApplication* initializeQt(int _argc, char* _argv[]) {
	QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

	QLocale::setDefault(QLocale(QLocale::English));

	int argc = 0;
	QApplication* newApp = new QApplication(_argc, _argv);

	ot::GlobalColorStyle::instance().setApplication(newApp);

	return newApp;
}

void initializeMainWindow(void) {
	MainWindow::instance();
}

bool initializeOpenGL(void) {
	// Check for a sufficient OpenGL version
	QOffscreenSurface surf;
	surf.create();

	QOpenGLContext ctx;
	ctx.create();
	ctx.makeCurrent(&surf);

	std::string version = (const char*)ctx.functions()->glGetString(GL_VERSION);
	int major = std::atoi(version.c_str());
	if (major < 2)
	{
		std::string errorString = "The system supports OpenGL version " + version + " only.\n"
			"At least OpenGL version 2.0.0 is required.\n\n"
			"You may use software rendering by renaming the file opengl32sw.dll to opengl32.dll in the installation directory.";

		OT_LOG_E(errorString);
		QMessageBox msgBox(QMessageBox::Critical, "OpenGL error", QString::fromStdString(errorString), QMessageBox::Ok);
		msgBox.exec();
		return false;
	}
	else {
		return true;
	}
}

bool initializeAssets(void) {
	int iconPathCounter = 0;
	int stylePathCounter = 0;
	int graphicsPathCounter = 0;

#ifdef _DEBUG
	if (ot::IconManager::addSearchPath(QString(qgetenv("OPENTWIN_DEV_ROOT") + "/Assets/Icons/"))) {
		iconPathCounter++;
	}
	if (ot::GlobalColorStyle::instance().addStyleRootSearchPath(QString(qgetenv("OPENTWIN_DEV_ROOT") + "/Assets/ColorStyles/"))) {
		stylePathCounter++;
	}
	if (ot::GraphicsItemLoader::instance().addSearchPath(QString(qgetenv("OPENTWIN_DEV_ROOT") + "/Assets/GraphicsItems/"))) {
		graphicsPathCounter++;
	}
#else
	if (ot::IconManager::addSearchPath(QDir::currentPath() + "/icons/")) {
		iconPathCounter++;
	}
	if (ot::GlobalColorStyle::instance().addStyleRootSearchPath(QDir::currentPath() + "/ColorStyles/")) {
		stylePathCounter++;
	}
	if (ot::GraphicsItemLoader::instance().addSearchPath(QDir::currentPath() + "/GraphicsItems/")) {
		graphicsPathCounter++;
	}
#endif // _DEBUG

	// Check if at least one icon directory was found
	if (iconPathCounter == 0) {
		OTAssert(0, "No icon path was found!");
		OT_LOG_E("No icon path found");
		QMessageBox msgBox(QMessageBox::Critical, "Assets error", "No icon path was found. Try to reinstall the application", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}

	// Check if at least one style directory was found
	if (stylePathCounter == 0) {
		OT_LOG_EA("No color style path found");
		QMessageBox msgBox(QMessageBox::Critical, "Assets error", "No color style path was found. Try to reinstall the application", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}

	// Check if at least one graphics item directory was found
	if (stylePathCounter == 0) {
		OT_LOG_EA("No graphics item path found");
		QMessageBox msgBox(QMessageBox::Critical, "Assets error", "No graphics item path was found. Try to reinstall the application", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}

	// Initialize application icon
	ot::IconManager::setApplicationIcon(ot::IconManager::getIcon("Application/OpenTwin.ico"));

	// Initialize color style
	ot::GlobalColorStyle::instance().scanForStyleFiles();

	if (ot::GlobalColorStyle::instance().hasStyle(ot::toString(ot::ColorStyleName::DarkStyle))) {
		ot::GlobalColorStyle::instance().setCurrentStyle(ot::toString(ot::ColorStyleName::DarkStyle));
	}
	else {
		OT_LOG_EA("Dark style not found");
		QMessageBox msgBox(QMessageBox::Critical, "Assets error", "Default " + QString::fromStdString(ot::toString(ot::ColorStyleName::BrightStyle)) + " color style is missing. Try to reinstall the application", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}

	return true;
}

bool initializeComponents(void) {
	// Initialize Application Settings
	ot::ApplicationPropertiesManager::instance().setReplaceExistingPropertiesOnMerge(true);

	// Initialize Widget view manager
	ot::WidgetViewManager::instance().initialize();

	return true;
}

int main(int _argc, char * _argv[])
{
	// Initialize

	initializeLogging();

	QApplication* app = initializeQt(_argc, _argv);

	initializeMainWindow();

	if (!initializeOpenGL()) {
		return -1;
	}

	if (!initializeAssets()) {
		return -2;
	}

	if (!initializeComponents()) {
		return -3;
	}

	MainWindow::instance()->initialize();

	// Run Main Event Loop
	int status = app->exec();

	ot::WidgetViewManager::instance().closeViews();

    return status;
}
