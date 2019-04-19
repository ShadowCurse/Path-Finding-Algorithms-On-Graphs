#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graphWidget): graph(graphWidget)
{
    Pressed = false;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    weight = level = 0;
    show_CHP = show_Arc = false;
}

QRectF Node::boundingRect() const
{
    return QRectF( -30 , -30 , 60 , 60);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    //painter->fillRect(boundingRect(), Qt::red);
    painter->setPen(Qt::PenStyle::NoPen);
    this->setZValue(10);
    switch (Start_End_None) {
    case 0:
        painter->setBrush(QColor("#A33643"));
        painter->drawEllipse(-10, -10, 20, 20);
        break;
    case 1:
        painter->setBrush(QColor("#00CA73"));
        //painter->drawEllipse(boundingRect());
        painter->drawEllipse(-30, -30, 60, 60);
        break;
    case 2:
        painter->setBrush(QColor("#F4002B"));
        //painter->drawEllipse(boundingRect());
        painter->drawEllipse(-30, -30, 60, 60);
        break;
    case 3:
        painter->setBrush(QColor("#02c0ef"));
        //painter->drawEllipse(boundingRect());
        painter->drawEllipse(-30, -30, 60, 60);
        break;
    }
    painter->setPen(Qt::PenStyle::DashLine);
    painter->setPen(Qt::white);
    if (show_CHP)
        painter->drawText(boundingRect(),Qt::AlignCenter,"L: " + QString::number(level) + "\nW: " + QString::number(weight));
    if (show_Arc)
        painter->drawText(boundingRect(),Qt::AlignCenter,"ArcFlag: " + QString::number(Arc_Flag_color));
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, Edges)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
