#include "MainWindow.h"

#include <QtWidgets/QApplication>

#include "OpenTwinCore/Logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ot::LogDispatcher::initialize("UIPlayground", false);
    ot::LogDispatcher::instance().setLogFlags(ot::ALL_LOG_FLAGS);

    MainWindow::instance();
    return a.exec();
}
