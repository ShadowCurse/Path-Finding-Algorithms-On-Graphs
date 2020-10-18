#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge {
 public:
  Edge(Node *sourceNode, Node *destNode, size_t edge_id);

  friend class Node;

  size_t id;
  double length;
  bool is_shortcut;
  bool show_arc;
  Node *source;
  Node *dest;
  std::array<size_t, 2> shortcut_edges_ids;
  std::array<bool, 4> arc_flags;
};

class GraphicalEdge : public QGraphicsItem {
 public:
  explicit GraphicalEdge(const std::vector<Edge> &edges, size_t edge_id);

  enum class Color {
    def, green, red, orange, blue
  };

  enum Color color;

  qreal adjust();
 private:
  [[nodiscard]] QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  const std::vector<Edge> &edges_;
  const size_t edge_id_;
  QPointF sourcePoint_;
  QPointF destPoint_;
  qreal arrowSize_;
};
#endif // EDGE_H
