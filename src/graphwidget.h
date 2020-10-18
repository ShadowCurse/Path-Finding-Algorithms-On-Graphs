#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include "node.h"
#include "edge.h"
#include <vector>

class GraphWidget : public QGraphicsView {
 Q_OBJECT
 public:
  explicit GraphWidget(QWidget *parent = nullptr);

  enum class Methods {
    None,
    Kruskal,
    Dijkstra,
    Arc_Flag_Dijkstra,
    Double_Dijkstra,
    CHP_Double_Dijkstra,
    AStar
  };

 signals:
  void Nodes_Created(int);
  void Nodes_Error();
  void log_msg(QString msg);

 public slots:
  void generate_graph(int node_num, int edges_per_node, double edge_weight);
  void zoomIn();
  void zoomOut();
  void Start_End_Change(int, int);
  void start(int method, int start_id, int end_id);
  void CHP_state_change(bool state);
  void Arc_Flags_state_change(bool state);
 private:

  void Create_Nodes(int nodes_num, double weight);
  void Create_Edges(int edges_per_node, double weight);
  void scaleView(qreal scaleFactor);
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *event) override;
#endif
  void Unpack_ShortCut(Edge *);
  static bool Compare_Edges(Edge *, Edge *);
  void Draw_Edge(int *parent_id, int, int);
  void Backward_Draw_Edge(int *parent_id, int, int);
  void DD_Draw_Edge(int *forward_parent_id, int *backward_parent_id, int, int, int);

  int prev_Start;
  int prev_End;
  double right_bound;
  double left_bound;
  double top_bound;
  double bottom_bound;
  std::vector<Node> nodes_;
  std::vector<Edge> edges_;
  std::map<size_t, GraphicalNode* > graphical_nodes_;
  std::map<size_t, GraphicalEdge* > graphical_edges_;
  std::vector<Edge> ShortCuts;
  std::vector<Edge *> Visited_Edges;
  std::vector<Edge *> Green_Edges;
  bool Shortcuts_Created;
  bool Arc_Flags_Created;
  std::vector<QGraphicsLineItem *> Arc_lines;

};

#endif // GRAPHWIDGET_H
