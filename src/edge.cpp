#include "edge.h"
#include "node.h"
#include <qmath.h>
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destNode, size_t edge_id) {
  id = edge_id;
  source = sourceNode;
  dest = destNode;
  source->edges.push_back(id);
  dest->edges.push_back(id);
  length = std::pow(source->x - destNode->x, 2) + std::pow(source->y - destNode->y, 2);
  is_shortcut = false;
  show_arc = false;
  std::ranges::fill(shortcut_edges_ids, 0);
  std::ranges::fill(arc_flags, 0);
}

GraphicalEdge::GraphicalEdge(const std::vector<Edge> &edges, size_t edge_id)
    : edges_(edges), edge_id_(edge_id), color(Color::def) {}

qreal GraphicalEdge::adjust() {
  if (!edges_[edge_id_].source || !edges_[edge_id_].dest)
    return -1;

  QLineF
      line(edges_[edge_id_].source->x, edges_[edge_id_].source->y, edges_[edge_id_].dest->x, edges_[edge_id_].dest->y);
  qreal length = line.length();

  prepareGeometryChange();

  if (length > qreal(20.)) {
    QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
    sourcePoint_ = line.p1() + edgeOffset;
    destPoint_ = line.p2() - edgeOffset;
  } else {
    sourcePoint_ = destPoint_ = line.p1();
  }
  return abs(length);
}

QRectF GraphicalEdge::boundingRect() const {
  if (!edges_[edge_id_].source || !edges_[edge_id_].dest)
    return QRectF();
  return QRectF(sourcePoint_, destPoint_).normalized();
}

void GraphicalEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  if (!edges_[edge_id_].source || !edges_[edge_id_].dest)
    return;
  QLineF line(sourcePoint_, destPoint_);
  switch (color) {
    case Color::def: {
      arrowSize_ = 10;
      painter->setPen(QPen(QColor("#e0e0e0"), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      this->setZValue(0);
      break;
    }
    case Color::green: {
      arrowSize_ = 10;
      painter->setPen(QPen(QColor("#5D9F36"), 7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      this->setZValue(4);
      break;
    }
    case Color::red: {
      arrowSize_ = 10;
      painter->setPen(QPen(QColor("#f90487"), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      this->setZValue(3);
      break;
    }
    case Color::orange: {
      arrowSize_ = 10;
      painter->setPen(QPen(QColor("#fc6d07"), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      this->setZValue(1);
      break;
    }
    case Color::blue: {
      arrowSize_ = 10;
      painter->setPen(QPen(QColor("#0082fc"), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      this->setZValue(3);
      break;
    }
  }
  //arrows
  painter->drawLine(line);
  double angle = std::atan2(-line.dy(), line.dx());
  QPointF destArrowP1 = destPoint_ + QPointF(sin(angle - M_PI / 3) * arrowSize_,
                                             cos(angle - M_PI / 3) * arrowSize_);
  QPointF destArrowP2 = destPoint_ + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize_,
                                             cos(angle - M_PI + M_PI / 3) * arrowSize_);
  painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
  //drawing flags
  if (edges_[edge_id_].show_arc) {
    painter->setPen(QPen(QColor("#ffffff"), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawText(boundingRect(),
                      Qt::AlignCenter,
                      "Flags: " + QString::number(edges_[edge_id_].arc_flags[0])
                          + QString::number(edges_[edge_id_].arc_flags[1])
                          + QString::number(edges_[edge_id_].arc_flags[2])
                          + QString::number(edges_[edge_id_].arc_flags[3]));
  }
}
