#include "AppBase.h"
#include "OpenTwinCore/Logger.h"
#include <QtWidgets/qapplication.h>
#include "OTWidgets/IconManager.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ot::LogDispatcher::initialize("SebastianPlayground");
    
    QString devenv = qgetenv("OPENTWIN_DEV_ROOT");
    ot::IconManager::instance().addSearchPath(devenv+"\\Assets\\Icons\\");

    AppBase::getInstance()->ot::AbstractLogNotifier::deleteLater(true);
    ot::LogDispatcher::instance().addReceiver(AppBase::getInstance());
    ot::LogDispatcher::instance().setLogFlags(ot::ALL_LOG_FLAGS);
    
    AppBase::getInstance()->show();
    return a.exec();
}
