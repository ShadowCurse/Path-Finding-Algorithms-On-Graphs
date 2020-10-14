#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <QKeyEvent>
#include <span>
#include <chrono>
#include <random>
#include <set>
#include "algorithms.h"
#include <ranges>

GraphWidget::GraphWidget(QWidget *parent) : QGraphicsView(parent) {
  auto scene = new QGraphicsScene(this);
  setScene(scene);
  prev_Start = prev_End = -1;
  setStyleSheet(
      "QGraphicsView"
      "{background-color:rgb(25,25,25);"
      "border: 3px solid gray;"
      "border-color: #A33643;"
      "border-radius: 15px;"
      "padding: 5px 5px 5px 5px;}"
      "QScrollBar:vertical"
      "{border:none;width: 15 px;}"
      "QScrollBar::handle"
      "{background-color: #A33643;"
      "border-radius: 5px;"
      "min-height: 0px;}"
      "QScrollBar::add-page:vertical"
      "{background-color: rgb(30,30,30);"
      "height: 10px;"
      "subcontrol-position: bottom;"
      "subcontrol-origin: margin;}"
      "QScrollBar::sub-page:vertical"
      "{background-color: rgb(30,30,30);"
      "height: 10 px;"
      "subcontrol-position: top;"
      "subcontrol-origin: margin;}"
      "QScrollBar::add-line:vertical"
      "{background-color: rgb(30,30,30);"
      "height: 10 px;"
      "subcontrol-position: bottom;"
      "subcontrol-origin: margin;}"
      "QScrollBar::sub-line:vertical"
      "{background-color: rgb(30,30,30);"
      "height: 10 px;"
      "subcontrol-position: top;"
      "subcontrol-origin: margin;}"
      "QScrollBar:horizontal"
      "{border:none;height: 15px;}"
      "QScrollBar::add-page:horizontal"
      "{background-color:  rgb(30,30,30);"
      "width: 10px;"
      "subcontrol-position: left;"
      "subcontrol-origin: margin;}"
      "QScrollBar::sub-page:horizontal"
      "{background-color: rgb(30,30,30);"
      "width: 10 px;"
      "subcontrol-position: right;"
      "subcontrol-origin: margin;}"
      "QScrollBar::add-line:horizontal"
      "{background-color: rgb(30,30,30);"
      "width: 10 px;"
      "subcontrol-position: left;"
      "subcontrol-origin: margin;}"
      "QScrollBar::sub-line:horizontal"
      "{background-color: rgb(30,30,30);"
      "width: 10 px;"
      "subcontrol-position: right;"
      "subcontrol-origin: margin;}");
  setRenderHint(QPainter::Antialiasing);
}

void GraphWidget::generate_graph(int node_num, int edges_per_node, double edge_weight) {
  if (node_num > 0 && edges_per_node < node_num) {
    right_bound = left_bound = bottom_bound = top_bound = 0;
    Shortcuts_Created = false;
    Arc_Flags_Created = false;
    items().clear();
    scene()->clear();
    graphical_nodes_.clear();
    graphical_edges_.clear();
    nodes_.clear();
    edges_.clear();
    ShortCuts.clear();
    Visited_Edges.clear();
    Green_Edges.clear();
    Arc_lines.clear();
    prev_Start = prev_End = -1;
    Create_Nodes(node_num, edge_weight);
    Create_Edges(edges_per_node);
    emit Nodes_Created(nodes_.size());
  } else {
    emit Nodes_Error();
  }
}

void GraphWidget::zoomIn() {
  scaleView(qreal(1.2));
}

void GraphWidget::zoomOut() {
  scaleView(1 / qreal(1.2));
}

void GraphWidget::Start_End_Change(int Start, int End) {
  if (!nodes_.empty()) {
    if (prev_Start != -1 && prev_End != -1) {
      graphical_nodes_[nodes_[prev_Start].id]->color = GraphicalNode::Color::def;
      graphical_nodes_[nodes_[prev_Start].id]->update();
      graphical_nodes_[nodes_[prev_End].id]->color = GraphicalNode::Color::def;
      graphical_nodes_[nodes_[prev_End].id]->update();
    }
    graphical_nodes_[nodes_[Start].id]->color = GraphicalNode::Color::green;
    graphical_nodes_[nodes_[Start].id]->update();
    graphical_nodes_[nodes_[End].id]->color = GraphicalNode::Color::red;
    graphical_nodes_[nodes_[End].id]->update();
    prev_Start = Start;
    prev_End = End;
  }
}

void GraphWidget::start(int method, int start_id, int end_id) {
  auto m = static_cast<Methods>(method);
  for (auto &[edge, graphical_edge]: graphical_edges_) {
    graphical_edge->color = GraphicalEdge::Color::def;
    graphical_edge->update();
  }
  Visited_Edges.clear();
  Green_Edges.clear();
  if (nodes_.empty()) {
    emit log_msg("<font color = red>Warning: Graph should be created first</font>");
    return;
  }
  if (start_id == end_id)
      emit log_msg("<font color = red>Warning: start and end ids should be different</font>");

  switch (m) {
    case Methods::None: {
      break;
    }
    case Methods::Kruskal: {
      auto start_point = std::chrono::steady_clock::now();
      auto[visited_edges, path] = algorithms::Kruskal(nodes_, edges_);
      auto end_point = std::chrono::steady_clock::now();
      for (auto edge: visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::green;
        graphical_edge->update();
      }
      for (auto edge: path) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::red;
        graphical_edge->update();
      }
      emit log_msg("Kruskal ends in: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
          end_point - start_point).count()) + "ms");
      break;
    }
    case Methods::Dijkstra: {
      auto start_point = std::chrono::steady_clock::now();
      auto[visited_edges, path, _] = algorithms::Dijkstra(nodes_, edges_, start_id, end_id);
      auto end_point = std::chrono::steady_clock::now();
      for (auto edge: visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::red;
        graphical_edge->update();
      }
      for (auto edge: path) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::green;
        graphical_edge->update();
      }
      emit log_msg("Dijkstra ends in: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
          end_point - start_point).count()) + "ms");
      break;
    }
    case Methods::Arc_Flag_Dijkstra: {
      auto start_point = std::chrono::steady_clock::now();
      auto[visited_edges, path] = algorithms::Arc_Flag_Dijkstra(nodes_, edges_, start_id, end_id);
      auto end_point = std::chrono::steady_clock::now();
      for (auto edge: visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::green;
        graphical_edge->update();
      }
      for (auto edge: path) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::red;
        graphical_edge->update();
      }
      emit log_msg(
          "Arc_Flag_Dijkstra ends in: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
              end_point - start_point).count()) + "ms");
      break;
    }
    case Methods::Double_Dijkstra: {
      auto start_point = std::chrono::steady_clock::now();
      auto[forward_visited_edges, backward_visited_edges, path] = algorithms::Double_Dijkstra(nodes_,
                                                                                              edges_,
                                                                                              start_id,
                                                                                              end_id);
      auto end_point = std::chrono::steady_clock::now();
      for (auto edge: forward_visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::green;
        graphical_edge->update();
      }
      for (auto edge: backward_visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::blue;
        graphical_edge->update();
      }
      for (auto edge: path) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::red;
        graphical_edge->update();
      }
      emit log_msg("DDijkstra ends in: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
          end_point - start_point).count()) + "ms");
      break;
    }
    case Methods::CHP_Double_Dijkstra: {
      auto start_point = std::chrono::steady_clock::now();
      auto[forward_visited_edges, backward_visited_edges, path] = algorithms::CHP_Double_Dijkstra(nodes_, edges_,
                                                                                                  start_id,
                                                                                                  end_id);
      auto end_point = std::chrono::steady_clock::now();
      for (auto edge: forward_visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::green;
        graphical_edge->update();
      }
      for (auto edge: backward_visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::blue;
        graphical_edge->update();
      }
      for (auto edge: path) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::red;
        graphical_edge->update();
      }
      emit log_msg("CHP DDijkstra ends in: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
          end_point - start_point).count()) + "ms");
      break;
    }
    case Methods::AStar: {
      auto start_point = std::chrono::steady_clock::now();
      auto[visited_edges, path] = algorithms::AStar(nodes_, edges_, start_id, end_id);
      auto end_point = std::chrono::steady_clock::now();
      for (auto edge: visited_edges) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::green;
        graphical_edge->update();
      }
      for (auto edge: path) {
        auto &graphical_edge = graphical_edges_[edge->id];
        graphical_edge->color = GraphicalEdge::Color::red;
        graphical_edge->update();
      }
      emit log_msg("AStar ends in: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(
          end_point - start_point).count()) + "ms");
      break;
    }
  }
}

void GraphWidget::CHP_state_change(bool state) {
//  if (nodes_.size()) {
//    QTime time;
//    if (state) {
//      if (!Shortcuts_Created) {
//        time.start();
//        ShortCuts = algorithms::Constractional_Heirarchy_Prepricessing(nodes_, edges_);
//        emit log_msg(
//            "Constractional_Heirarchy_Prepricessing ends in: " + QString::number(time.elapsed()) + "ms");
//        return;
//      } else {
//        for (auto &edge: ShortCuts) {
//          edge.color = Edge::Color::orange;
//          edge.show();
//        }
//      }
//      for (auto &node: nodes_) {
//        node.show_CHP = true;
//        node.update();
//      }
//    }
//    if (!state && Shortcuts_Created) {
//      for (auto &edge: ShortCuts) {
//        edge.color = Edge::Color::orange;
//        edge.hide();
//      }
//      for (auto &node: nodes_) {
//        node.show_CHP = false;
//        node.update();
//      }
//    }
//  }
}

void GraphWidget::Arc_Flags_state_change(bool state) {
//  if (nodes_.size()) {
//    QTime time;
//    if (state) {
//      if (!Arc_Flags_Created) {
//        time.start();
//
//        QPen pen(QColor("#fc6d07"), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//        QGraphicsLineItem *horizontal_line = new QGraphicsLineItem(QLineF(max_left * 6,
//                                                                          (max_top + max_bottom) / 2,
//                                                                          max_right * 6,
//                                                                          (max_top + max_bottom) / 2));
//        QGraphicsLineItem *vertical_line =
//            new QGraphicsLineItem(QLineF((max_left + max_top) / 2,
//                                         max_bottom * 6,
//                                         (max_left + max_top) / 2,
//                                         max_top * 6));
//        horizontal_line->setPen(pen);
//        vertical_line->setPen(pen);
//        Arc_lines.push_back(horizontal_line);
//        Arc_lines.push_back(vertical_line);
//        this->scene()->addItem(horizontal_line);
//        this->scene()->addItem(vertical_line);
//
//        qreal width_sep = (max_left + max_top) / 2;
//        qreal height_sep = (max_top + max_bottom) / 2;
//        for (auto &node: nodes_) {
//          if (node.pos().x() < width_sep && node.pos().y() > height_sep)
//            node.Arc_Flag_color = 0;
//          else if (node.pos().x() > width_sep && node.pos().y() > height_sep)
//            node.Arc_Flag_color = 1;
//          else if (node.pos().x() < width_sep && node.pos().y() < height_sep)
//            node.Arc_Flag_color = 2;
//          else if (node.pos().x() > width_sep && node.pos().y() < height_sep)
//            node.Arc_Flag_color = 3;
//        }
//        algorithms::Arc_Flags_Preprocessing(nodes_, edges_);
//        emit log_msg("Arc_Flags_Preprocessing ends in: " + QString::number(time.elapsed()) + "ms");
//        return;
//      }
//      for (auto &node: nodes_) {
//        node.show_Arc = true;
//        node.update();
//      }
//      for (auto &edge: edges_) {
//        edge.show_Arc = true;
//        edge.update();
//      }
//      for (auto &line: Arc_lines) {
//        line->show();
//      }
//    } else {
//      for (auto &node: nodes_) {
//        node.show_Arc = false;
//        node.update();
//      }
//      for (auto &edge: edges_) {
//        edge.show_Arc = false;
//        edge.update();
//      }
//      for (auto &line: Arc_lines) {
//        line->hide();
//      }
//    }
//  }
}

void GraphWidget::Create_Nodes(int nodes_num, double weight) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_real_distribution<> width_position(0, this->width());
  std::uniform_real_distribution<> height_position(0, this->height());
  std::uniform_real_distribution<> pos_distribution(-weight, weight);

  nodes_.reserve(nodes_num);
  auto &first_node = nodes_.emplace_back(nodes_.size());
  first_node.x = width_position(gen);
  first_node.y = height_position(gen);
  auto &first_graphical_node =
      graphical_nodes_.insert({first_node.id, new GraphicalNode(&first_node)}).first->second;
  this->scene()->addItem(first_graphical_node);

  for (int i{0}; i < nodes_num - 1; ++i) {
    std::uniform_real_distribution<> node_id_distribution(0, nodes_.size());
    int random_node_id = node_id_distribution(gen);
    auto &random_node = nodes_[random_node_id];
    auto node_x = random_node.x;
    auto node_y = random_node.y;
    auto new_x = node_x + pos_distribution(gen);
    auto new_y = node_y + pos_distribution(gen);
    auto &new_node = nodes_.emplace_back(nodes_.size());
    new_node.x = new_x;
    new_node.y = new_y;

    right_bound = std::max(right_bound, new_x);
    left_bound = std::min(left_bound, new_x);
    top_bound = std::max(top_bound, new_y);
    bottom_bound = std::min(bottom_bound, new_y);
    auto &new_graphical_node =
        graphical_nodes_.insert({new_node.id, new GraphicalNode(&new_node)}).first->second;
    this->scene()->addItem(new_graphical_node);
  }
}

void GraphWidget::Create_Edges(int edges_per_node) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_real_distribution<> edges_num_distribution(1, edges_per_node);

  auto nodes_size = nodes_.size();
  std::vector<Node *> sorted_x_nodes(nodes_size);
  std::transform(std::begin(nodes_), std::end(nodes_), std::begin(sorted_x_nodes), [](auto &node) { return &node; });
  std::sort(std::begin(sorted_x_nodes),
            std::end(sorted_x_nodes),
            [](const auto &e1, const auto &e2) { return e1->x < e2->x; });
  std::vector<Node *> sorted_y_nodes(sorted_x_nodes);
  std::sort(std::begin(sorted_y_nodes),
            std::end(sorted_y_nodes),
            [](const auto &e1, const auto &e2) { return e1->y < e2->y; });

  std::vector<std::pair<size_t, size_t>> node_x_y_pos(nodes_size);
  for (size_t i{0}; i < nodes_size; ++i)
    node_x_y_pos[sorted_x_nodes[i]->id].first = i;
  for (size_t i{0}; i < nodes_size; ++i)
    node_x_y_pos[sorted_y_nodes[i]->id].second = i;

  for (size_t i{0}; i < nodes_size; ++i) {
    auto &node = nodes_[i];
    auto node_x_pos = node_x_y_pos[i].first;
    auto node_y_pos = node_x_y_pos[i].second;
    auto min_x = std::clamp(node_x_pos - edges_per_node, size_t{0}, node_x_pos - edges_per_node);
    auto max_x = std::clamp(node_x_pos + edges_per_node, node_x_pos + edges_per_node, nodes_size - 1);
    auto min_y = std::clamp(node_y_pos - edges_per_node, size_t{0}, i - edges_per_node);
    auto max_y = std::clamp(node_y_pos + edges_per_node, node_y_pos + edges_per_node, nodes_size - 1);
    std::span x_span(std::begin(sorted_x_nodes) + min_x, std::begin(sorted_x_nodes) + max_x);
    std::span y_span(std::begin(sorted_y_nodes) + min_y, std::begin(sorted_y_nodes) + max_y);
    for (auto x_value: x_span) {
      if (auto result = std::find(std::begin(y_span), std::end(y_span), x_value); result != std::end(y_span)
          && *result != &node) {
        auto &new_edge = edges_.emplace_back(&node, x_value, edges_.size());
        auto &new_graphical_edge =
            graphical_edges_.insert({new_edge.id, new GraphicalEdge(&new_edge)}).first->second;
        this->scene()->addItem(new_graphical_edge);
        new_graphical_edge->adjust();
      }
    }
  }
}

#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent *event) {
  scaleView(pow((double) 2, event->delta() / 240.0));
}
#endif

void GraphWidget::scaleView(qreal scaleFactor) {
  auto factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
  if (factor < 0.000001 || factor > 1000000)
    return;
  scale(scaleFactor, scaleFactor);
}

