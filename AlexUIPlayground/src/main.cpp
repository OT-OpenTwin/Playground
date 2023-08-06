#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "OpenTwinCore/Logger.h"
#include "OpenTwinCore/otAssert.h"
#include "OpenTwinCore/SimpleFactory.h"
#include "OTGui/GraphicsItemCfg.h"
#include "OTGui/GraphicsLayoutItemCfg.h"
#include "OTWidgets/GraphicsItem.h"
#include "OTWidgets/GraphicsLayoutItem.h"

#define USE_TEST false
#if USE_TEST == true

class TesterA : public ot::SimpleFactoryObject {
public:
    virtual std::string simpleFactoryObjectKey(void) const { return "A"; };
};

class TesterB : public ot::SimpleFactoryObject {
public:
    virtual std::string simpleFactoryObjectKey(void) const { return "B"; };
};

void test(void) {
    //ot::SimpleFactoryRegistrar<TesterA> testerARegistrar("A");
    //ot::SimpleFactoryRegistrar<TesterB> testerBRegistrar("B");

    std::string expectedKey = OT_SimpleFactoryJsonKeyValue_GraphicsTextItemCfg;

    auto itm = ot::SimpleFactory::instance().create(expectedKey);
    if (itm) {
        std::string actualKey = itm->simpleFactoryObjectKey();
        if (actualKey != expectedKey) {
            otAssert(0, "KEY");
        }
    }
    else {
        otAssert(0, "NULLPTR");
    }
}

#endif // USE_TEST == true

int main(int argc, char *argv[])
{
#if USE_TEST == true
    ::test();
    return 0;
#endif

    QApplication a(argc, argv);
    ot::LogDispatcher::initialize("UIPlayground", false);
    ot::LogDispatcher::instance().setLogFlags(ot::ALL_LOG_FLAGS);

	MainWindow::instance();
    return a.exec();
}
