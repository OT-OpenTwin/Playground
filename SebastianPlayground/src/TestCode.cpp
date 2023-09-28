#include "TestCode.h"
#include "AppBase.h"

#include "OTWidgets/GraphicsPicker.h"
#include "OTWidgets/GraphicsView.h"
#include "OTGui/GraphicsPackage.h"
#include "OTGui/GraphicsCollectionCfg.h"
#include "OTGui/GraphicsItemCfg.h"
#include "OTGui/GraphicsLayoutItemCfg.h"
#include "OTWidgets/GraphicsFactory.h"
#include "OTWidgets/GraphicsItem.h"
#include "OTWidgets/GraphicsLayoutItem.h"
#include"OpenTwinCore/SimpleFactory.h"

#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qmenubar.h>

TestCode::TestCode()
	:m_uid(0)
{
	//Create Widgets

	m_ItemPicker=new ot::GraphicsPickerDockWidget;
	m_View = new ot::GraphicsView;

	AppBase::getInstance()->getTabView()->addTab(m_View, "NetworkElements");
	AppBase::getInstance()->addDockWidget(Qt::LeftDockWidgetArea, m_ItemPicker);

	//Create Item Collections
	ot::GraphicsCollectionPackage pckg;
	pckg.addCollection(this->createTestCollection());

	//Add Package to item picker
	m_ItemPicker->pickerWidget()->add(pckg);

	//Connect Signals
	m_View->setDropsEnabled(true);
	connect(m_View, &ot::GraphicsView::itemRequested, this, &TestCode::slotItemDropped); // Wer schreibt bei slotItemDropped den Namen rein?
	
	
	
	
}

ot::GraphicsCollectionCfg* TestCode::createTestCollection()
{
	//Creating Collections
	ot::GraphicsCollectionCfg* myCollection = new ot::GraphicsCollectionCfg("Elements","Elements");
	ot::GraphicsCollectionCfg* childCollection = new ot::GraphicsCollectionCfg("Electric Circuit Elements", "Electric Circuit Elements");
	myCollection->addChildCollection(childCollection);

	//Creating Items

	
	childCollection->addItem(this->getItem());
	childCollection->addItem(this->getItem2());

	return myCollection;
}

void TestCode::slotItemDropped(const QString& _name, const QPointF& _pos)
{
	ot::GraphicsItem* item = nullptr;
	
	if (_name == "resistor"){item = ot::GraphicsFactory::itemFromConfig(this->getItem());}
	
	else if (_name == "Test"){item = ot::GraphicsFactory::itemFromConfig(this->getItem2());}
	
	item->setGraphicsItemUid(std::to_string(++m_uid));
	item->getQGraphicsItem()->setPos(_pos);

	m_View->addItem(item);


}

ot::GraphicsItemCfg* TestCode::getItem()
{
	ot::GraphicsTextItemCfg* resistor = new ot::GraphicsTextItemCfg("Resistor", ot::Color(ot::Color::Blue));
	resistor->setName("resistor");
	resistor->setGraphicsItemFlags(ot::GraphicsItemCfg::ItemIsConnectable);
	resistor->setTitle("resistor");
	OT_rJSON_createDOC(doc);
	resistor->addToJsonObject(doc, doc);
	OT_LOG_I(ot::rJSON::toJSON(doc));
	ot::GraphicsItemCfg* config = ot::SimpleFactory::instance().createType<ot::GraphicsItemCfg>(doc);
	config->setFromJsonObject(doc);
	OT_LOG_I(config->name());

	return resistor;
}

ot::GraphicsItemCfg* TestCode::getItem2()
{
	ot::GraphicsImageItemCfg* image = new ot::GraphicsImageItemCfg();
	image->setName("Test");
	image->setTitle("Test");
	image->setImagePath("Default/Add");
	OT_rJSON_createDOC(doc);
	image->addToJsonObject(doc, doc);
	OT_LOG_I(ot::rJSON::toJSON(doc));
	ot::GraphicsItemCfg* config = ot::SimpleFactory::instance().createType<ot::GraphicsItemCfg>(doc);
	config->setFromJsonObject(doc);
	OT_LOG_I(config->name());

	return image;
}