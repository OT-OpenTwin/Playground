#ifndef TESTCODE_H
#define TESTCODE_H

// Qt header
#include <QtCore/qsettings.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgraphicswidget.h>
#include <QtWidgets/qgraphicsproxywidget.h>
#include <QtWidgets/qgraphicslinearlayout.h>
#include <QtWidgets/qgraphicsgridlayout.h>

class Window : public QGraphicsWidget
{
    Q_OBJECT
public:
    Window(QGraphicsWidget* parent = nullptr);

};

class LayoutItem : public QGraphicsLayoutItem, public QGraphicsItem
{
public:
    LayoutItem(QGraphicsItem* parent = nullptr);

    // Inherited from QGraphicsLayoutItem
    void setGeometry(const QRectF& geom) override;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override;

    // Inherited from QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QPixmap m_pix;
};

#endif  //TESTCODE_H