#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <vector>

class Edge;
class QGraphicsSceneMouseEvent;

class Node {
 public:
  explicit Node(size_t node_id);

  size_t id;
  double x;
  double y;
  bool show_CHP;
  bool show_Arc;
  int weight;
  int level;
  int arc_flag_color;
  std::vector<size_t> edges;
};

class GraphicalNode : public QGraphicsItem {
 public:
  explicit GraphicalNode(Node* node);

  enum class Color {
    def, green, red
  };

  Color color;
 private:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  bool pressed_;
  Node* node_;
};

struct Node_Length {
  Node *node;
  qreal length;
  bool operator<(const Node_Length &NL) const { return length < NL.length; }
};

#endif // NODE_H
