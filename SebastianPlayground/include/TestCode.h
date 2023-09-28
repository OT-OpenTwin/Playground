#pragma once
#include <QtCore/qobject.h>

namespace ot {
	class GraphicsPickerDockWidget;
	class GraphicsView;
	class GraphicsCollectionCfg;
	class GraphicsItemCfg;
}


class TestCode : public  QObject
{
	Q_OBJECT

public:
	TestCode();

private:
	int m_uid;
	ot::GraphicsPickerDockWidget* m_ItemPicker;
	ot::GraphicsView* m_View;

	ot::GraphicsCollectionCfg* createTestCollection();
	ot::GraphicsItemCfg* getItem();
	ot::GraphicsItemCfg* getItem2();

private slots:
	void slotItemDropped(const QString& _name, const QPointF& _pos);
};