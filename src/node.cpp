#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(size_t node_id) {
  id = node_id;
  x = 0;
  y = 0;
  show_CHP = false;
  show_Arc = false;
  weight = 0;
  level = 0;
  arc_flag_color = -1;
}

GraphicalNode::GraphicalNode(Node *node) : node_(node), pressed_(false), color(Color::def) {
  setPos(node_->x, node_->y);
}

QRectF GraphicalNode::boundingRect() const {
  return QRectF(-30, -30, 60, 60);
}

void GraphicalNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
  painter->setPen(Qt::PenStyle::NoPen);
  this->setZValue(10);
  switch (color) {
    case Color::def:painter->setBrush(QColor("#A33643"));
      painter->drawEllipse(-10, -10, 20, 20);
      break;
    case Color::green:painter->setBrush(QColor("#00CA73"));
      painter->drawEllipse(-30, -30, 60, 60);
      break;
    case Color::red:painter->setBrush(QColor("#F4002B"));
      painter->drawEllipse(-30, -30, 60, 60);
      break;
  }
  painter->setPen(Qt::PenStyle::DashLine);
  painter->setPen(Qt::white);
  if (node_->show_CHP)
    painter->drawText(boundingRect(),
                      Qt::AlignCenter,
                      "L: " + QString::number(node_->level) + "\nW: " + QString::number(node_->weight));
  if (node_->show_Arc)
    painter->drawText(boundingRect(), Qt::AlignCenter, "ArcFlag: " + QString::number(node_->arc_flag_color));
}

