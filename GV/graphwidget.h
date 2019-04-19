#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H
#include <QGraphicsView>
#include "rightmenu.h"
#include "node.h"
using namespace std;
class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    RightMenu *rm;
    int prev_Start;
    int prev_End;
    int max_right;
    int max_left;
    int max_top;
    int max_bottom;
signals:
    void Nodes_Created(int);
    void Nodes_Error();
public slots:
    void Generate_Graph();
    void zoomIn();
    void zoomOut();
    void Start_End_Change(int,int);
    void Unpack_ShortCut(Edge*);
    void Start();
    void CHP_state_change();
    void Arc_Flags_state_change();

private:
   void Create_Node();
   void Create_Edges();
   QList <Node*> Nodes;
   QList <Edge*> Edges;
   QList <Edge*> ShortCuts;
   QList <Edge*> Visited_Edges;
   QList <Edge*> Green_Edges;
   int **CHP;
   bool Shortcuts_Created;
   bool Arc_Flags_Created;
   QList <QGraphicsLineItem*> Arc_lines;
   int shortcuts_num;
   struct Node_Length{
       Node* node;
       qreal length;
       bool operator<(const Node_Length &NL) const {return length<NL.length;}
   };
   QList <Node_Length> Nodes_L;
   void Get_Lengths(Node*);
   qreal Get_Length(Node*, Node*);
   void scaleView(qreal scaleFactor);
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif

    //algirithms
    static bool Compare_Edges(Edge *,Edge *);
    void Kruskal();
    void Draw_Edge(int *parent_id,int, int);
    void Backward_Draw_Edge(int *parent_id,int, int);
    int* Dijkstra(int, int, bool R = 0);
    int* Reverce_Dijkstra(int, int, bool Return = 0);
    void DD_Draw_Edge(int *forward_parent_id,int *backward_parent_id,int, int, int);
    void Double_Dijkstra(int , int);
    void AStar(int,int);
    void Arc_Flags_Preprocessing(int squares_num = 4);
    void Arc_Flag_Dijkstra(int,int);
    void Constractional_Heirarchy_Prepricessing();
    void CHP_Double_Dijkstra(int,int);
};

#endif // GRAPHWIDGET_H
