#include "MainWindow.h"

#include <QtCore/qdir.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>

#include "OpenTwinCore/Logger.h"
#include "OTWidgets/IconManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ot::LogDispatcher::initialize("UIPlayground", false);
    ot::LogDispatcher::instance().setLogFlags(ot::ALL_LOG_FLAGS);

	// Setup icon manager
	QByteArray env = qgetenv("OPENTWIN_DEV_ROOT");
	if (env.isEmpty()) {
		QMessageBox msg(QMessageBox::Warning, "Warning", "Can not start playground without the OpenTwin dev environment set.", QMessageBox::Ok);
		msg.exec();
		return 1;
	}

	QString path = QString::fromStdString(env.toStdString());
	path.replace("\\", "/");
	if (!path.endsWith("/")) {
		path.append("/");
	}
	path.append("Assets/Icons");
	
	QDir dir(path);
	if (!dir.exists()) {
		QMessageBox msg(QMessageBox::Warning, "Warning", "Can not start playground without the OpenTwin deployment created (missing: Assets/Icons).", QMessageBox::Ok);
		msg.exec();
		return 1;
	}

	ot::IconManager::instance().addSearchPath(path);

    MainWindow::instance();
    return a.exec();
}
