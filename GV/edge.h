#ifndef EDGE_H
#define EDGE_H
#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode);

    enum{
      defoult,green,red,orange,blue
    };
    qreal adjust();
    qreal Length;
    Edge* Shortcut_Edges[2];
    bool IsShortCut;
    int color;
    bool show_Arc;
    bool Arc_Flags[4];
    Node *source, *dest;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};
#endif // EDGE_H
