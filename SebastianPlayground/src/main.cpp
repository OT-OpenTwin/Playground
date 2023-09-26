#include "AppBase.h"
#include <QtWidgets/QApplication>
#include "OpenTwinCore/Logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ot::LogDispatcher::initialize("SebastianPlayground");
   
    AppBase::getInstance()->ot::AbstractLogNotifier::deleteLater(true);
    ot::LogDispatcher::instance().addReceiver(AppBase::getInstance());
    ot::LogDispatcher::instance().setLogFlags(ot::ALL_LOG_FLAGS);
    
    AppBase::getInstance()->show();
    return a.exec();
}
