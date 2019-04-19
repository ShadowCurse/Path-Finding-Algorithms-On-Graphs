#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include <algorithm>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <set>
#include <QTime>

using namespace std;
GraphWidget::GraphWidget(QWidget *parent) : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    //Created_nodes = 0;
    prev_Start = prev_End = -1;
    setStyleSheet("QGraphicsView{background-color:rgb(25,25,25);border: 3px solid gray;border-color: #A33643;border-radius: 15px; padding: 5px 5px 5px 5px;} QScrollBar:vertical {border:none;width: 15	px;} QScrollBar::handle {background-color: #A33643;border-radius: 5px;min-height: 0px; } QScrollBar::add-page:vertical {background-color:  rgb(30,30,30);height: 10px;subcontrol-position: bottom;subcontrol-origin: margin;} QScrollBar::sub-page:vertical {background-color:  rgb(30,30,30);height: 10 px;subcontrol-position: top;subcontrol-origin: margin;} QScrollBar::add-line:vertical {background-color:  rgb(30,30,30);height: 10 px;subcontrol-position: bottom;subcontrol-origin: margin;} QScrollBar::sub-line:vertical {background-color:  rgb(30,30,30);height: 10 px;subcontrol-position: top;subcontrol-origin: margin;} QScrollBar:horizontal {border:none;height: 15px;} QScrollBar::add-page:horizontal {background-color:  rgb(30,30,30);width: 10px;subcontrol-position: left;subcontrol-origin: margin;} QScrollBar::sub-page:horizontal {background-color:  rgb(30,30,30);width: 10 px;subcontrol-position: right;subcontrol-origin: margin;} QScrollBar::add-line:horizontal {background-color:  rgb(30,30,30);width: 10 px;subcontrol-position: left;subcontrol-origin: margin;} QScrollBar::sub-line:horizontal {background-color:  rgb(30,30,30);width: 10 px;subcontrol-position: right;subcontrol-origin: margin;}");
    setRenderHint(QPainter::Antialiasing);
}
//slots
void GraphWidget::Generate_Graph()
{
    if (rm->Node_num_value->value() > 0 && rm->Edges_Per_Node_value->value() < rm->Node_num_value->value()){
        max_right = max_left = max_bottom = max_top = 0;
        Shortcuts_Created = false;
        Arc_Flags_Created = false;
        rm->Arc_Flags_Prepricessing->setText("Arc Flags Preprocessing");
        rm->Constractional_Heirarchy_Prepricessing->setText("Constractional Heirarchy Preprocessing");
        rm->Arc_Flags_Prepricessing->setCheckState(Qt::CheckState::Unchecked);
        rm->Constractional_Heirarchy_Prepricessing->setCheckState(Qt::CheckState::Unchecked);
        items().clear();
        scene()->clear();
        Nodes.clear();
        Edges.clear();
        ShortCuts.clear();
        Visited_Edges.clear();
        Green_Edges.clear();
        Arc_lines.clear();
        shortcuts_num = 0;
        rm->Start_id_value->setValue(0);
        rm->End_id_value->setValue(0);
        prev_Start = prev_End = -1;
        for (int i(0); i < rm->Node_num_value->value(); ++i){
                    Create_Node();
        }
        if (rm->Node_num_value->value() > 1)
            Create_Edges();
        emit Nodes_Created(Edges.size());
    }
    else
    {
        emit Nodes_Error();
    }
}
void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}
void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
void GraphWidget::Start_End_Change(int Start, int End)
{
    if (Nodes.size()){
    if (prev_Start != -1 && prev_End != -1){
        Nodes[prev_Start]->Start_End_None = 0;
        Nodes[prev_Start]->update();
        Nodes[prev_End]->Start_End_None = 0;
        Nodes[prev_End]->update();
    }
    Nodes[Start]->Start_End_None = 1;
    Nodes[Start]->update();
    Nodes[End]->Start_End_None = 2;
    Nodes[End]->update();
    prev_Start = Start;
    prev_End = End;
    }
}

void GraphWidget::Unpack_ShortCut(Edge* edge){
    if (!edge->IsShortCut){
        edge->color = Edge::green;
        Green_Edges.append(edge);
        return;
    }
    else{
        Unpack_ShortCut(edge->Shortcut_Edges[0]);
        Unpack_ShortCut(edge->Shortcut_Edges[1]);
    }
}

void GraphWidget::Start()
{
    for (auto &edge: Edges){
        edge->color = Edge::defoult;//0;
        edge->update();
    }
    Visited_Edges.clear();
    Green_Edges.clear();
    QTime time;
    if (!Nodes.size()){
        rm->Logs->appendHtml("<font color = red>Warning: Graph should be created first</font>");
        return;
    }
    if (rm->Method_Select->currentIndex() == 0){
        rm->Logs->appendHtml("<font color = red>Warning: No algorithm is chosen</font>");
        return;
    }

    if (rm->Method_Select->currentIndex() == 1){
        time.start();
        Kruskal();
        rm->Logs->appendPlainText("Kruskal ends in: " + QString::number(time.elapsed()) + "ms");
    }
    else{
        if (rm->Start_id_value->value() != rm->End_id_value->value()){
            if (rm->Method_Select->currentIndex() == 2){
               time.start();
               Dijkstra(rm->Start_id_value->value(), rm->End_id_value->value());
               rm->Logs->appendPlainText("Dijkstra ends in: " + QString::number(time.elapsed()) + "ms");
            }
            else {
                if (rm->Method_Select->currentIndex() == 3){
                   time.start();
                   Arc_Flag_Dijkstra(rm->Start_id_value->value(), rm->End_id_value->value());
                   rm->Logs->appendPlainText("Arc_Flag_Dijkstra ends in: " + QString::number(time.elapsed()) + "ms");
                }
                else{
                    if (rm->Method_Select->currentIndex() == 4){
                       time.start();
                       Double_Dijkstra(rm->Start_id_value->value(), rm->End_id_value->value());
                       rm->Logs->appendPlainText("DDijkstra ends in: " + QString::number(time.elapsed()) + "ms");
                    }
                    else{
                        if (rm->Method_Select->currentIndex() == 5){
                            time.start();
                            CHP_Double_Dijkstra(rm->Start_id_value->value(), rm->End_id_value->value());
                            rm->Logs->appendPlainText("CHP DDijkstra ends in: " + QString::number(time.elapsed()) + "ms");
                        }
                        else{
                            if (rm->Method_Select->currentIndex() == 6){
                                time.start();
                                AStar(rm->Start_id_value->value(), rm->End_id_value->value());
                                rm->Logs->appendPlainText("AStar ends in: " + QString::number(time.elapsed()) + "ms");
                            }
                        }
                    }
                }
            }
        }
        else
            rm->Logs->appendHtml("<font color = red>Warning: start and end ids should be different</font>");
    }
    for (auto &edge : Visited_Edges)
        edge->update();
    for (auto &edge : Green_Edges)
        edge->update();
}

void GraphWidget::CHP_state_change()
{
    if (Nodes.size()){
        QTime time;
        if (rm->Constractional_Heirarchy_Prepricessing->isChecked()){
            if (!Shortcuts_Created){
                time.start();
                Constractional_Heirarchy_Prepricessing();
                rm->Logs->appendPlainText("Constractional_Heirarchy_Prepricessing ends in: " + QString::number(time.elapsed()) + "ms");
                rm->Constractional_Heirarchy_Prepricessing->setText("Constractional Heirarchy View");
                rm->Constractional_Heirarchy_Prepricessing->setCheckState(Qt::CheckState::Unchecked);
                return;
            }
            else{
                for (auto &edge: ShortCuts){
                    edge->color = Edge::orange;
                    edge->show();
                }
            }
            for (auto &node: Nodes){
                node->show_CHP = true;
                node->update();
            }
        }
        if (!rm->Constractional_Heirarchy_Prepricessing->isChecked() && Shortcuts_Created){
            for (auto &edge: ShortCuts){
                edge->color = Edge::orange;
                edge->hide();
            }
            for (auto &node: Nodes){
                node->show_CHP = false;
                node->update();
            }
        }
    }
}

void GraphWidget::Arc_Flags_state_change()
{
    if (Nodes.size()){
        QTime time;
        if (rm->Arc_Flags_Prepricessing->isChecked()){
            if (!Arc_Flags_Created){
                time.start();
                Arc_Flags_Preprocessing();
                rm->Logs->appendPlainText("Arc_Flags_Preprocessing ends in: " + QString::number(time.elapsed()) + "ms");
                rm->Arc_Flags_Prepricessing->setText("Arc Flags View");
                rm->Arc_Flags_Prepricessing->setCheckState(Qt::CheckState::Unchecked);
                return;
            }
            for (auto &node: Nodes){
                node->show_Arc = true;
                node->update();
            }
            for (auto &edge: Edges){
                edge->show_Arc = true;
                edge->update();
            }
            for (auto &line: Arc_lines){
                line->show();
            }
        }
        else{
            for (auto &node: Nodes){
                node->show_Arc = false;
                node->update();
            }
            for (auto &edge: Edges){
                edge->show_Arc = false;
                edge->update();
            }
            for (auto &line: Arc_lines){
                line->hide();
            }
        }
    }
}
//private f
void GraphWidget::Create_Node()
{
    Node* new_Node = new Node(this);
    new_Node->id = Nodes.size();
    if (Nodes.size() != 0){
        qreal weight_limit = 20*rm->Edge_weight_value->value();
        int random_node_id = QRandomGenerator::global()->bounded(Nodes.size());
        qreal node_x = Nodes[random_node_id]->x();
        qreal node_y = Nodes[random_node_id]->y();
        qreal new_x = node_x -(weight_limit/2) + QRandomGenerator::global()->bounded(weight_limit);
        qreal new_y = node_y -(weight_limit/2) + QRandomGenerator::global()->bounded(weight_limit);
        new_Node->setPos(new_x, new_y);
        if (new_x > max_right)
            max_right = new_x;
        else
            if (new_x < max_left)
                max_left = new_x;
        if (new_y > max_top)
            max_top = new_y;
        else
            if (new_y < max_bottom)
                max_bottom = new_y;
    }
    else {
        new_Node->setPos(-(this->width()/2) + QRandomGenerator::global()->bounded(this->width()), -(this->height()/2) + QRandomGenerator::global()->bounded(this->height()));
    }
    new_Node->Start_End_None = 0;
    this->scene()->addItem(new_Node);
    Nodes.append(new_Node);
}
void GraphWidget::Create_Edges()
{
    for (auto node: Nodes){
        Get_Lengths(node);
        int r = 1 + QRandomGenerator::global()->bounded(rm->Edges_Per_Node_value->value());
        for (int i(0); i < r; ++i){
            Edge *new_Edge = new Edge(node,Nodes_L[i].node);
            this->scene()->addItem(new_Edge);
            Edges.append(new_Edge);
            node->Edges.append(new_Edge);
            Nodes_L[i].node->Edges.append(new_Edge);
        }
    }
}
void GraphWidget::Get_Lengths(Node * sourse_node)
{
    Nodes_L.clear();
    for (auto node: Nodes){
        if (node != sourse_node){
            Node_Length NL;
            NL.length = Get_Length(node,sourse_node);
            NL.node = node;
            Nodes_L.append(NL);
        }
    }
    sort(Nodes_L.begin(),Nodes_L.end());
}
qreal GraphWidget::Get_Length(Node* node, Node* source_node)
{
    return sqrt(((node->pos().x() - source_node->pos().x())*(node->pos().x() - source_node->pos().x()))
            +((node->pos().y() - source_node->pos().y())*(node->pos().y() - source_node->pos().y())));
}
#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 240.0));
}
#endif
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.000001 || factor > 1000000)
        return;

    scale(scaleFactor, scaleFactor);
}


//algirithms
bool GraphWidget::Compare_Edges(Edge *E1, Edge *E2)
{
    return E1->Length<E2->Length;
}

void GraphWidget::Kruskal()
{
    int nodes_size = rm->Node_num_value->value();
    int *parent_nodes = new int[nodes_size];
    for (int i(0); i < nodes_size; ++i){
        parent_nodes[i] = i;
    }
    if (Edges.size()) {
            sort(Edges.begin(), Edges.end(),Compare_Edges);
            for (auto &edge: Edges){
                if (!edge->IsShortCut){
                    int source_id = edge->source->id;
                    int dest_id = edge->dest->id;
                    if (parent_nodes[source_id] != parent_nodes[dest_id]) {
                        edge->color = Edge::green;
                        Green_Edges.append(edge);
                        int old_parent = parent_nodes[dest_id];
                        for (int i(0); i < nodes_size; ++i){
                            if (parent_nodes[i] == old_parent)
                                parent_nodes[i] = parent_nodes[source_id];
                        }
                    }
                    else
                    {
                        edge->color = Edge::red;
                        Visited_Edges.append(edge);
                    }
                }
            }
       }
    delete[] parent_nodes;
}

void GraphWidget::Draw_Edge(int *parent_id, int source_id, int dest_id){
    for (auto &edge : Nodes[dest_id]->Edges){
        if (edge->dest->id == dest_id && edge->source->id == parent_id[dest_id]){
            if (edge->IsShortCut)
                Unpack_ShortCut(edge);
            else{
                edge->color = Edge::green;//1;
                Green_Edges.append(edge);
            }
        }
    }
    if (parent_id[dest_id] != source_id)
        Draw_Edge(parent_id, source_id, parent_id[dest_id]);
    else
        return;
}

void GraphWidget::Backward_Draw_Edge(int *parent_id, int source_id, int dest_id){
    for (auto &edge : Nodes[dest_id]->Edges){
        if (edge->source->id == dest_id && edge->dest->id == parent_id[dest_id]){
            if (edge->IsShortCut)
                Unpack_ShortCut(edge);
            else{
                edge->color = Edge::green;//1;
                Green_Edges.append(edge);
            }
        }
    }
    if (parent_id[dest_id] != source_id)
        Backward_Draw_Edge(parent_id, source_id, parent_id[dest_id]);
    else
        return;
}

int* GraphWidget::Dijkstra(int source_id, int dest_id, bool R)
{
    int source;
    bool *edge_run_number = new bool[Nodes.size()];
    memset(edge_run_number, 0, Nodes.size());
    int *parent_id = new int[Nodes.size()];
    memset(parent_id, -1, Nodes.size()*sizeof(int));
    int *min_distance = new int[Nodes.size()];
    min_distance[source_id] = 0;
    edge_run_number[source_id] = true;
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source_id} );
    while (!active_vertices.empty()) {
        source = active_vertices.begin()->second;
        if (source == dest_id){
            break;
        }
        active_vertices.erase(active_vertices.begin());
        for (auto &edge : Nodes[source]->Edges){
            if (source != edge->dest->id && !edge->IsShortCut){
                if (edge_run_number[edge->dest->id] != true){
                    min_distance[edge->dest->id] = min_distance[source] + edge->Length;
                    active_vertices.insert( { min_distance[edge->dest->id], edge->dest->id } );
                    edge_run_number[edge->dest->id] = true;
                    parent_id[edge->dest->id] = source;
                }
                else{
                    if (min_distance[edge->dest->id] > min_distance[source] + edge->Length){
                        active_vertices.erase( { min_distance[edge->dest->id], edge->dest->id } );
                        min_distance[edge->dest->id] = min_distance[source] + edge->Length;
                        active_vertices.insert( { min_distance[edge->dest->id], edge->dest->id } );
                        parent_id[edge->dest->id] = source;
                    }
                }
                if (!R){
                    edge->color = Edge::red;//2;
                    Visited_Edges.append(edge);
                }
            }
        }
    }
    if (!R)
        if (edge_run_number[dest_id] == true)
            Draw_Edge(parent_id, source_id, dest_id);
    delete[] edge_run_number;
    delete[] min_distance;
    if (!R)
        delete[] parent_id;
    return parent_id;
}

int* GraphWidget::Reverce_Dijkstra(int source_id, int dest_id, bool R)
{
    int source;
    bool *edge_run_number = new bool[Nodes.size()];
    memset(edge_run_number, 0, Nodes.size());
    int *parent_id = new int[Nodes.size()];
    memset(parent_id, -1, Nodes.size()*sizeof(int));
    int *min_distance = new int[Nodes.size()];
    min_distance[source_id] = 0;
    edge_run_number[source_id] = true;
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source_id} );
    while (!active_vertices.empty()) {
        source = active_vertices.begin()->second;
        if (source == dest_id){
            break;
        }
        active_vertices.erase(active_vertices.begin());
        for (auto &edge : Nodes[source]->Edges){
            if (source != edge->source->id && !edge->IsShortCut){
                if (edge_run_number[edge->source->id] != true){
                    min_distance[edge->source->id] = min_distance[source] + edge->Length;
                    active_vertices.insert( { min_distance[edge->source->id], edge->source->id } );
                    edge_run_number[edge->source->id] = true;
                    parent_id[edge->source->id] = source;
                }
                else{
                    if (min_distance[edge->source->id] > min_distance[source] + edge->Length){
                        active_vertices.erase( { min_distance[edge->source->id], edge->source->id } );
                        min_distance[edge->source->id] = min_distance[source] + edge->Length;
                        active_vertices.insert( { min_distance[edge->source->id], edge->source->id } );
                        parent_id[edge->source->id] = source;
                    }
                }
                if (!R){
                    edge->color = Edge::red;//2;
                    Visited_Edges.append(edge);
                }
            }
        }
    }
    if (!R)
        if (edge_run_number[dest_id] == true)
            Backward_Draw_Edge(parent_id, source_id, dest_id);
    delete[] edge_run_number;
    delete[] min_distance;
    if (!R)
        delete[] parent_id;
    return parent_id;
}

void GraphWidget::DD_Draw_Edge(int *forward_parent_id, int *backward_parent_id, int source_id, int dijkstra_meet_id, int dest_id)
{
    Draw_Edge(forward_parent_id, source_id, dijkstra_meet_id);
    Backward_Draw_Edge(backward_parent_id, dest_id, dijkstra_meet_id);
}

void GraphWidget::Double_Dijkstra(int source_id, int dest_id)
{
    int dijkstra_meet_id  = -1;
    int source;
    int dest;
    int min_forward;
    int min_backward;
    bool *forward_edge_run_number = new bool[Nodes.size()];
    memset(forward_edge_run_number, 0, Nodes.size());
    bool *backward_edge_run_number = new bool[Nodes.size()];
    memset(backward_edge_run_number, 0, Nodes.size());
    int *forward_parent_id = new int[Nodes.size()];
    int *backward_parent_id = new int[Nodes.size()];
    int *forward_min_distance = new int[Nodes.size()];
    int *backward_min_distance = new int[Nodes.size()];
    forward_min_distance[source_id] = 0;
    backward_min_distance[dest_id] = 0;
    set< pair<int,int> > forward_active_vertices;
    set< pair<int,int> > backward_active_vertices;
    forward_active_vertices.insert( {0,source_id} );
    backward_active_vertices.insert( {0,dest_id} );
    while (!forward_active_vertices.empty() && !backward_active_vertices.empty()) {
        source = forward_active_vertices.begin()->second;
        dest = backward_active_vertices.begin()->second;
        forward_active_vertices.erase(forward_active_vertices.begin());
        backward_active_vertices.erase(backward_active_vertices.begin());
        //forward dijkstra
        for (auto &edge : Nodes[source]->Edges){
            if (source != edge->dest->id && !edge->IsShortCut){
                if (forward_edge_run_number[edge->dest->id] != true){
                    forward_min_distance[edge->dest->id] = forward_min_distance[source] + edge->Length;
                    forward_active_vertices.insert( { forward_min_distance[edge->dest->id], edge->dest->id } );
                    forward_edge_run_number[edge->dest->id] = true;
                    forward_parent_id[edge->dest->id] = source;
                }
                else{
                    if (forward_min_distance[edge->dest->id] > forward_min_distance[source] + edge->Length){
                        forward_active_vertices.erase( { forward_min_distance[edge->dest->id], edge->dest->id } );
                        forward_min_distance[edge->dest->id] = forward_min_distance[source] + edge->Length;
                        forward_active_vertices.insert( { forward_min_distance[edge->dest->id], edge->dest->id } );
                        forward_parent_id[edge->dest->id] = source;
                    }
                }
                edge->color = Edge::red;//2;
                Visited_Edges.append(edge);
                if (backward_edge_run_number[edge->dest->id] != true)
                    continue;
                if (!forward_active_vertices.empty()){
                    min_forward = forward_active_vertices.begin()->first;
                }
                else
                {
                    min_forward = forward_min_distance[edge->dest->id];
                }
                if (!backward_active_vertices.empty()){
                    min_backward = backward_active_vertices.begin()->first;
                }
                else
                {
                    min_backward = backward_min_distance[edge->dest->id];
                }
                if (forward_min_distance[edge->dest->id] + backward_min_distance[edge->dest->id] <= min_forward+min_backward){
                    dijkstra_meet_id = edge->dest->id;
                    break;
                }
            }
        }
        if (dijkstra_meet_id != -1)
            break;
        //backward dijkstra
        for (auto &edge : Nodes[dest]->Edges){
            if (dest != edge->source->id && !edge->IsShortCut){
                if (backward_edge_run_number[edge->source->id] != true){
                    backward_min_distance[edge->source->id] = backward_min_distance[dest] + edge->Length;
                    backward_active_vertices.insert( { backward_min_distance[edge->source->id], edge->source->id } );
                    backward_edge_run_number[edge->source->id] = true;
                    backward_parent_id[edge->source->id] = dest;
                }
                else{
                    if (backward_min_distance[edge->source->id] > backward_min_distance[dest] + edge->Length){
                        backward_active_vertices.erase( { backward_min_distance[edge->source->id], edge->source->id } );
                        backward_min_distance[edge->source->id] = backward_min_distance[dest] + edge->Length;
                        backward_active_vertices.insert( { backward_min_distance[edge->source->id], edge->source->id } );
                        backward_parent_id[edge->source->id] = dest;
                    }
                }
                edge->color = Edge::blue;//2;
                Visited_Edges.append(edge);
                if (forward_edge_run_number[edge->source->id] != true)
                    continue;
                if (!forward_active_vertices.empty()){
                    min_forward = forward_active_vertices.begin()->first;
                }
                else
                {
                    min_forward = forward_min_distance[edge->dest->id];
                }
                if (!backward_active_vertices.empty()){
                    min_backward = backward_active_vertices.begin()->first;
                }
                else
                {
                    min_backward = backward_min_distance[edge->dest->id];
                }
                if (forward_min_distance[edge->source->id] + backward_min_distance[edge->source->id] <= min_forward+min_backward){
                    dijkstra_meet_id = edge->source->id;
                    break;
                }
            }
        }
        if (dijkstra_meet_id != -1)
            break;
    }
    if (dijkstra_meet_id != -1)
        DD_Draw_Edge(forward_parent_id, backward_parent_id, source_id, dijkstra_meet_id, dest_id);
    delete[] forward_edge_run_number;
    delete[] backward_edge_run_number;
    delete[] forward_parent_id;
    delete[] backward_parent_id;
    delete[] forward_min_distance;
    delete[] backward_min_distance;
}

void GraphWidget::AStar(int source_id, int dest_id)
{
    int source;
    int full_path;
    int *parent_id = new int[Nodes.size()];
    int *min_distance = new int[Nodes.size()];
    bool *closed_set = new bool[Nodes.size()];
    memset(closed_set, 0, Nodes.size());
    bool *open_set = new bool[Nodes.size()];
    memset(open_set, 0, Nodes.size());
    min_distance[source_id] = 0;
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source_id} );
    while (!active_vertices.empty()) {
        source = active_vertices.begin()->second;
        if (source == dest_id){
            closed_set[source] = true;
            break;
        }
        active_vertices.erase(active_vertices.begin());
        open_set[source] = false;
        closed_set[source] = true;
        for (auto &edge : Nodes[source]->Edges){
            if (source != edge->dest->id && !edge->IsShortCut){
                if (closed_set[edge->dest->id] == true)
                    continue;
                full_path = min_distance[source] + edge->Length;
                if (open_set[edge->dest->id] != true){
                    min_distance[edge->dest->id] = full_path + Get_Length(edge->dest, Nodes[dest_id]);
                    active_vertices.insert( { min_distance[edge->dest->id], edge->dest->id } );
                    open_set[edge->dest->id] = true;
                }
                else{
                    if (full_path >= min_distance[edge->dest->id])
                        continue;
                }
                parent_id[edge->dest->id] = source;
                min_distance[edge->dest->id] = full_path;
                edge->color = Edge::red;//2;
                Visited_Edges.append(edge);
            }
        }
    }
    if (closed_set[dest_id] == true)
        Draw_Edge(parent_id, source_id, dest_id);
    delete[] parent_id;
    delete[] min_distance;
    delete[] closed_set;
    delete[] open_set;
}

void GraphWidget::Arc_Flags_Preprocessing(int squares_num)
{
    QPen pen(QColor("#fc6d07"), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QGraphicsLineItem *horizontal_line = new QGraphicsLineItem(QLineF(max_left*6,(max_top + max_bottom)/2,max_right*6,(max_top + max_bottom)/2));
    QGraphicsLineItem *vertical_line = new QGraphicsLineItem(QLineF((max_left + max_top)/2,max_bottom*6, (max_left + max_top)/2,max_top*6));
    horizontal_line->setPen(pen);
    vertical_line->setPen(pen);
    Arc_lines.append(horizontal_line);
    Arc_lines.append(vertical_line);
    this->scene()->addItem(horizontal_line);
    this->scene()->addItem(vertical_line);

    qreal width_sep  = (max_left + max_top)/2;
    qreal height_sep = (max_top + max_bottom)/2;
    for (auto &node: Nodes){
        if (node->pos().x() < width_sep && node->pos().y() > height_sep)
            node->Arc_Flag_color = 0;
        else
            if (node->pos().x() > width_sep && node->pos().y() > height_sep)
                node->Arc_Flag_color = 1;
            else
                if (node->pos().x() < width_sep && node->pos().y() < height_sep)
                    node->Arc_Flag_color = 2;
                else
                    if (node->pos().x() > width_sep && node->pos().y() < height_sep)
                        node->Arc_Flag_color = 3;
    }
    QList<Edge*> bounding_edges;
    for (auto &edge: Edges){
        if (edge->source->Arc_Flag_color == edge->dest->Arc_Flag_color)
            edge->Arc_Flags[edge->source->Arc_Flag_color] = true;
        else
            bounding_edges.append(edge);
    }
    int *RD = new int[Nodes.size()];
    for (auto &edge: bounding_edges){
        int color = edge->dest->Arc_Flag_color;
        RD = Reverce_Dijkstra(edge->source->id, -1, true);
        for (int i(0); i < Nodes.size(); ++i){
            int destination = i;
            int parent = RD[i];
            if (parent == -1)
                continue;
            do{
                for (int y(0); y < Nodes[destination]->Edges.size(); ++y){
                    if (Nodes[destination]->Edges[y]->dest->id == parent && !Nodes[destination]->Edges[y]->IsShortCut){
                        Nodes[destination]->Edges[y]->Arc_Flags[color] = true;
                    }
                }
                destination = parent;
                parent = RD[destination];
            }while (parent != -1);
        }
    }
    delete[] RD;
    Arc_Flags_Created = true;
}

void GraphWidget::Arc_Flag_Dijkstra(int source_id, int dest_id)
{
    int dest_color = Nodes[dest_id]->Arc_Flag_color;
    int source;
    bool *edge_run_number = new bool[Nodes.size()];
    memset(edge_run_number, 0, Nodes.size());
    int *parent_id = new int[Nodes.size()];
    memset(parent_id, -1, Nodes.size()*sizeof(int));
    int *min_distance = new int[Nodes.size()];
    min_distance[source_id] = 0;
    edge_run_number[source_id] = true;
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source_id} );
    while (!active_vertices.empty()) {
        source = active_vertices.begin()->second;
        if (source == dest_id){
            break;
        }
        active_vertices.erase(active_vertices.begin());
        for (auto &edge : Nodes[source]->Edges){
            if (source != edge->dest->id && !edge->IsShortCut && edge->Arc_Flags[dest_color]){
                if (edge_run_number[edge->dest->id] != true){
                    min_distance[edge->dest->id] = min_distance[source] + edge->Length;
                    active_vertices.insert( { min_distance[edge->dest->id], edge->dest->id } );
                    edge_run_number[edge->dest->id] = true;
                    parent_id[edge->dest->id] = source;
                }
                else{
                    if (min_distance[edge->dest->id] > min_distance[source] + edge->Length){
                        active_vertices.erase( { min_distance[edge->dest->id], edge->dest->id } );
                        min_distance[edge->dest->id] = min_distance[source] + edge->Length;
                        active_vertices.insert( { min_distance[edge->dest->id], edge->dest->id } );
                        parent_id[edge->dest->id] = source;
                    }
                }
                edge->color = Edge::red;//2;
                Visited_Edges.append(edge);
            }
        }
    }
    if (edge_run_number[dest_id] == true)
        Draw_Edge(parent_id, source_id, dest_id);
    delete[] edge_run_number;
    delete[] min_distance;
    delete[] parent_id;
}

void GraphWidget::Constractional_Heirarchy_Prepricessing()
{
    CHP = new int*[Nodes.size()];
    for (auto &node: Nodes){
        CHP[node->id] = Dijkstra(node->id, -1, 1);
    }
    int* weights = new int[Nodes.size()];
    memset(weights, 0, Nodes.size()*sizeof(int));
    for (int i(0); i < Nodes.size(); ++i){
        for (int j(0); j < Nodes.size(); ++j){
            if (CHP[i][j] != -1){
                int parent = CHP[i][j];
                do{
                    weights[parent] += 1;
                    parent = CHP[i][parent];
                }while(parent != -1);
            }
        }
    }
    for (int i(0); i < Nodes.size(); ++i){
        Nodes[i]->weight = weights[i];
    }
    Node_Length* node_weight = new Node_Length[Nodes.size()];
    for (int i(0); i < Nodes.size(); ++i){
        node_weight[i].node = Nodes[i];
        node_weight[i].length = weights[i];
    }
    sort(node_weight,node_weight+Nodes.size());

    //creating shortcuts
    bool **shortcut_map = new bool*[Nodes.size()];
    for (int i(0); i < Nodes.size(); ++i){
        shortcut_map[i] = new bool[Nodes.size()];
        memset(shortcut_map[i], false, Nodes.size());
    }
    for (auto &edge: Edges){
        shortcut_map[edge->source->id][edge->dest->id] = true;
    }
    Edge* edge1;
    Edge* edge2;

    shortcuts_num = 0;

    for (int i(0); i < Nodes.size(); ++i){
        for (int j(0); j < node_weight[i].node->Edges.size(); ++j){
            edge1 = node_weight[i].node->Edges[j];
            for (int k(0); k < node_weight[i].node->Edges.size(); ++k){
                edge2 = node_weight[i].node->Edges[k];
                if (edge1->dest != edge2->dest &&  edge1->source != edge2->source && edge1->dest != edge2->source){//selection edges
                    if (edge1->dest == node_weight[i].node){//finding out whitch direction
                        if (edge1->source->weight >= node_weight[i].length && edge2->dest->weight >= node_weight[i].length){// && edge1->source->weight <= edge2->dest->weight){
                            if (!shortcut_map[edge1->source->id][edge2->dest->id]){
                                //rm->Logs->appendPlainText("creating shc: " + QString::number(edge1->source->id) + " " + QString::number(edge2->dest->id));
                                ++shortcuts_num;
                                shortcut_map[edge1->source->id][edge2->dest->id] = true;
                                Edge *new_edge = new Edge(edge1->source,edge2->dest);
                                new_edge->Length = edge1->Length + edge2->Length;
                                new_edge->color = Edge::orange;
                                new_edge->IsShortCut = true;
                                new_edge->Shortcut_Edges[0] = edge2;
                                new_edge->Shortcut_Edges[1] = edge1;
                                ShortCuts.append(new_edge);
                                this->scene()->addItem(new_edge);
                            }
                        }
                    }
                    else{
                        if (edge2->source->weight >= node_weight[i].length && edge1->dest->weight >= node_weight[i].length){// && edge2->source->weight <= edge1->dest->weight){
                            if (!shortcut_map[edge2->source->id][edge1->dest->id]){
                                //rm->Logs->appendPlainText("creating shc: " + QString::number(edge2->source->id) + " " + QString::number(edge1->dest->id));
                                ++shortcuts_num;
                                shortcut_map[edge2->source->id][edge1->dest->id] = true;
                                Edge *new_edge = new Edge(edge2->source,edge1->dest);
                                new_edge->Length = edge1->Length + edge2->Length;
                                new_edge->color = Edge::orange;
                                new_edge->IsShortCut = true;
                                new_edge->Shortcut_Edges[0] = edge1;
                                new_edge->Shortcut_Edges[1] = edge2;
                                ShortCuts.append(new_edge);
                                this->scene()->addItem(new_edge);
                            }
                        }
                    }
                }
            }    
        }
    }

    rm->Logs->appendPlainText("shortcuts created: " + QString::number(shortcuts_num));
    Shortcuts_Created = true;
    //calc levels
    for (int i(0); i < Nodes.size(); ++i){
        int max(0);
        for (int j(0); j < node_weight[i].node->Edges.size(); ++j){
            if (node_weight[i].node->Edges[j]->dest != node_weight[i].node && node_weight[i].node->Edges[j]->dest->level > max)
                max = node_weight[i].node->Edges[j]->dest->level;
            else{
                if (node_weight[i].node->Edges[j]->source != node_weight[i].node && node_weight[i].node->Edges[j]->source->level > max)
                    max = node_weight[i].node->Edges[j]->source->level;
            }
        }
        node_weight[i].node->level = max + 1;
    }
    //for (int i(0); i < Nodes.size(); ++i)
    //    rm->Logs->appendPlainText(QString::number(weights[i]));
    delete[] weights;
    delete[] node_weight;
    for (int i(0); i < Nodes.size(); ++i)
        delete[] shortcut_map[i];
    delete[] shortcut_map;
    for (int i(0); i < Nodes.size(); ++i)
        delete[] CHP[i];
    delete[] CHP;


}

void GraphWidget::CHP_Double_Dijkstra(int source_id, int dest_id)
{
    int dijkstra_meet_id  = -1;
    int source;
    int dest;
    int min_forward;
    int min_backward;
    bool *forward_edge_run_number = new bool[Nodes.size()];
    memset(forward_edge_run_number, 0, Nodes.size());
    bool *backward_edge_run_number = new bool[Nodes.size()];
    memset(backward_edge_run_number, 0, Nodes.size());
    int *forward_parent_id = new int[Nodes.size()];
    int *backward_parent_id = new int[Nodes.size()];
    int *forward_min_distance = new int[Nodes.size()];
    int *backward_min_distance = new int[Nodes.size()];
    forward_min_distance[source_id] = 0;
    backward_min_distance[dest_id] = 0;
    set< pair<int,int> > forward_active_vertices;
    set< pair<int,int> > backward_active_vertices;
    forward_active_vertices.insert( {0,source_id} );
    backward_active_vertices.insert( {0,dest_id} );
    while (!forward_active_vertices.empty() || !backward_active_vertices.empty()) {
        if (!forward_active_vertices.empty()){
        source = forward_active_vertices.begin()->second;
        forward_active_vertices.erase(forward_active_vertices.begin());
        //forward dijkstra
        for (auto &edge : Nodes[source]->Edges){
            if (source != edge->dest->id  && Nodes[source]->level < edge->dest->level){// && !edge->IsShortCut){
                if (forward_edge_run_number[edge->dest->id] != true){
                    forward_min_distance[edge->dest->id] = forward_min_distance[source] + edge->Length;
                    forward_active_vertices.insert( { forward_min_distance[edge->dest->id], edge->dest->id } );
                    forward_edge_run_number[edge->dest->id] = true;
                    forward_parent_id[edge->dest->id] = source;
                }
                else{
                    if (forward_min_distance[edge->dest->id] > forward_min_distance[source] + edge->Length){
                        forward_active_vertices.erase( { forward_min_distance[edge->dest->id], edge->dest->id } );
                        forward_min_distance[edge->dest->id] = forward_min_distance[source] + edge->Length;
                        forward_active_vertices.insert( { forward_min_distance[edge->dest->id], edge->dest->id } );
                        forward_parent_id[edge->dest->id] = source;
                    }
                }
                edge->color = Edge::red;//2;
                Visited_Edges.append(edge);
                if (backward_edge_run_number[edge->dest->id] != true)
                    continue;
                if (!forward_active_vertices.empty()){
                    min_forward = forward_active_vertices.begin()->first;
                }
                else
                {
                    min_forward = forward_min_distance[edge->dest->id];
                }
                if (!backward_active_vertices.empty()){
                    min_backward = backward_active_vertices.begin()->first;
                }
                else
                {
                    min_backward = backward_min_distance[edge->dest->id];
                }
                if (forward_min_distance[edge->dest->id] + backward_min_distance[edge->dest->id] <= min_forward+min_backward){
                    dijkstra_meet_id = edge->dest->id;
                    break;
                }
            }
        }
        if (dijkstra_meet_id != -1)
            break;
        }
        if (!backward_active_vertices.empty()){
        //backward dijkstra
        dest = backward_active_vertices.begin()->second;
        backward_active_vertices.erase(backward_active_vertices.begin());
        for (auto &edge : Nodes[dest]->Edges){
            if (dest != edge->source->id && Nodes[dest]->level < edge->source->level){// && !edge->IsShortCut){
                if (backward_edge_run_number[edge->source->id] != true){
                    backward_min_distance[edge->source->id] = backward_min_distance[dest] + edge->Length;
                    backward_active_vertices.insert( { backward_min_distance[edge->source->id], edge->source->id } );
                    backward_edge_run_number[edge->source->id] = true;
                    backward_parent_id[edge->source->id] = dest;
                }
                else{
                    if (backward_min_distance[edge->source->id] > backward_min_distance[dest] + edge->Length){
                        backward_active_vertices.erase( { backward_min_distance[edge->source->id], edge->source->id } );
                        backward_min_distance[edge->source->id] = backward_min_distance[dest] + edge->Length;
                        backward_active_vertices.insert( { backward_min_distance[edge->source->id], edge->source->id } );
                        backward_parent_id[edge->source->id] = dest;
                    }
                }
                edge->color = Edge::blue;//2;
                Visited_Edges.append(edge);
                if (forward_edge_run_number[edge->source->id] != true)
                    continue;
                if (!forward_active_vertices.empty()){
                    min_forward = forward_active_vertices.begin()->first;
                }
                else
                {
                    min_forward = forward_min_distance[edge->dest->id];
                }
                if (!backward_active_vertices.empty()){
                    min_backward = backward_active_vertices.begin()->first;
                }
                else
                {
                    min_backward = backward_min_distance[edge->dest->id];
                }
                if (forward_min_distance[edge->source->id] + backward_min_distance[edge->source->id] <= min_forward+min_backward){
                    dijkstra_meet_id = edge->source->id;
                    break;
                }
            }
        }
        if (dijkstra_meet_id != -1)
            break;
        }
    }
    if (dijkstra_meet_id != -1)
        DD_Draw_Edge(forward_parent_id, backward_parent_id, source_id, dijkstra_meet_id, dest_id);
    delete[] forward_edge_run_number;
    delete[] backward_edge_run_number;
    delete[] forward_parent_id;
    delete[] backward_parent_id;
    delete[] forward_min_distance;
    delete[] backward_min_distance;
}
