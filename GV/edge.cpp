#include "edge.h"
#include "node.h"
#include <qmath.h>
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode)
    : color(0)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->Edges.append(this);
    dest->Edges.append(this);
    Length = adjust();
    Shortcut_Edges[0] = Shortcut_Edges[1] = nullptr;
    IsShortCut = false;
    memset(Arc_Flags, false, 4);
    show_Arc = false;
}
qreal Edge::adjust()
{
    if (!source || !dest)
        return -1;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
    return abs(length);
    //Length = abs(length);
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();
    //return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
    //                                  destPoint.y() - sourcePoint.y()))
    //        .normalized();
    return QRectF(sourcePoint,destPoint).normalized();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if (!source || !dest)
            return;
    QLineF line(sourcePoint, destPoint);
    //painter->fillRect(boundingRect(), Qt::yellow);
    //shoosing line color
    switch (color) {
        case 0:
            arrowSize = 10;
            painter->setPen(QPen(QColor("#e0e0e0"), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            this->setZValue(0);
            break;
        case 1:
            arrowSize = 50;
            painter->setPen(QPen(QColor("#5D9F36"), 45, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            this->setZValue(3);
            break;
        case 2:
            arrowSize = 10;
            painter->setPen(QPen(QColor("#f90487"), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            this->setZValue(3);
            break;
        case 3:
            arrowSize = 20;
            painter->setPen(QPen(QColor("#fc6d07"), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            this->setZValue(1);
            break;
        case 4:
            arrowSize = 20;
            painter->setPen(QPen(QColor("#0082fc"), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            this->setZValue(3);
            break;
    }
    //arrows
    painter->drawLine(line);
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                  cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                  cos(angle - M_PI + M_PI / 3) * arrowSize);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    //drawing flags
    if (show_Arc){
        painter->setPen(QPen(QColor("#ffffff"), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawText(boundingRect(),Qt::AlignCenter,"Flags: " + QString::number(Arc_Flags[0]) + QString::number(Arc_Flags[1]) + QString::number(Arc_Flags[2]) + QString::number(Arc_Flags[3]));
    }
}
