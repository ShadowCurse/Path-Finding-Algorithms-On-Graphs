#include "rightmenu.h"
#include <QRandomGenerator>

RightMenuWidget::RightMenuWidget(QWidget *parent) : QWidget(parent) {
  node_num_value_ = new QSpinBox(this);
  node_num_value_->setMaximum(1000000);

  edge_weight_value_ = new QSpinBox(this);
  edge_weight_value_->setMaximum(1000000);

  edges_per_node_value_ = new QSpinBox(this);
  edges_per_node_value_->setMaximum(1000000);

  start_id_value_ = new QSpinBox(this);
  start_id_value_->setMaximum(1000000);

  end_id_value_ = new QSpinBox(this);
  end_id_value_->setMaximum(1000000);

  method_select_ = new QComboBox(this);
  method_select_->addItem("-------");
  method_select_->addItem("Kruskal");
  method_select_->addItem("Dijkstra");
  method_select_->addItem("Arc_Flag_Dijkstra");
  method_select_->addItem("Double_Dijkstra");
  method_select_->addItem("CHP_Double_Dijkstra");
  method_select_->addItem("AStar");

  constractional_heirarchy_prepricessing_ = new QCheckBox(this);
  constractional_heirarchy_prepricessing_->setText("Constructional Hierarchy Preprocessing");
  connect(constractional_heirarchy_prepricessing_, &QCheckBox::stateChanged, this, &RightMenuWidget::constractional_heirarchy_prepricessing);

  arc_flags_prepricessing_ = new QCheckBox(this);
  arc_flags_prepricessing_->setText("Arc Flags Preprocessing");
  connect(arc_flags_prepricessing_, &QCheckBox::stateChanged, this, &RightMenuWidget::arc_flags_prepricessing);

  logs_ = new QPlainTextEdit(this);
  logs_->setReadOnly(true);
  logs_->appendPlainText("Logs");

  //layout managment
  auto node_edge_box = create_node_edge_layout();
  auto start_end_box = create_start_end_layout();

  node_edge_box->setMinimumHeight(250);
  start_end_box->setMinimumHeight(190);

  auto main_layout = new QVBoxLayout(this);
  main_layout->addWidget(node_edge_box);
  main_layout->addWidget(start_end_box);
  main_layout->addWidget(method_select_);
  main_layout->addWidget(constractional_heirarchy_prepricessing_);
  main_layout->addWidget(arc_flags_prepricessing_);

  auto start = new QPushButton(this);
  start->setText("Start");
  start->setStyleSheet(
      "QPushButton"
      "{color: #b1b1b1;"
      "background-color: #565656;"
      "border-width: 1px;"
      "border-color: #1e1e1e;"
      "border-style: solid;"
      "border-radius: 6;"
      "padding: 3px;"
      "font-size: 18px;"
      "padding-left: 5px;"
      "padding-right: 5px;}"
      "QPushButton:hover "
      "{background-color: #A33643;"
      "widget-animation-duration: 100}"
      "QPushButton:pressed"
      "{background-color: #AA6439;"
      "border-color: #AA6439;"
      "widget-animation-duration: 100}");
  connect(start, &QPushButton::clicked, this, &RightMenuWidget::pre_start);

  main_layout->addWidget(start);
  main_layout->addWidget(logs_);
  this->setLayout(main_layout);

  this->setStyleSheet(
      "QObject"
      "{background-color: rgb(56, 56, 56);"
      "color: #b1b1b1;"
      "widget-animation-duration: 100}"
      "QRadioButton"
      "{font-size: 14px;}"
      "QSpinBox"
      "{font-size: 10px;"
      "width: 20px;"
      "border-color: rgb(50, 50, 50);"
      "border-radius: 15px;"
      "background-color: rgb(50, 50, 50);}"
      "QSpinBox::up-button"
      "{subcontrol-origin: margin;"
      "subcontrol-position: center left;"
      "width: 0px;"
      "border-width: 1px;}"
      "QSpinBox::down-button"
      "{subcontrol-origin: margin;"
      "subcontrol-position: center right;"
      "width: 0px;border-width: 0px;}"
      "QLabel"
      "{font-size: 16px;}"
      "QPlainTextEdit"
      "{border: 3px solid gray;"
      "border-color: #A33643;"
      "margin-top: 15px;"
      "padding: 3px 3px 3px 3px;"
      "font-size: 14px;"
      "border-radius: 15px;}"
      "QScrollBar:vertical"
      "{border: 1px solid transparent;"
      "border-radius: 20px;"
      "width: 9	px;"
      "margin: 1px 1px 1px 1px;}"
      "QScrollBar::handle:vertical"
      "{background-color: #A33643;"
      "border-radius: 2px;"
      "min-height: 0px; }"
      "QScrollBar::add-page:vertical"
      "{background-color:  rgb(56, 56, 56);"
      "height: 10px;"
      "subcontrol-position: bottom;"
      "subcontrol-origin: margin;}"
      "QScrollBar::sub-page:vertical"
      "{background-color:  rgb(56, 56, 56);"
      "height: 10 px;"
      "subcontrol-position: top;"
      "subcontrol-origin: margin;}"
      "QScrollBar::add-line:vertical"
      "{background-color:  rgb(56, 56, 56);"
      "height: 10 px;"
      "subcontrol-position: bottom;"
      "subcontrol-origin: margin;}"
      "QScrollBar::sub-line:vertical"
      "{background-color:  rgb(56, 56, 56);"
      "height: 10 px;"
      "subcontrol-position: top;"
      "subcontrol-origin: margin;}");
}

auto RightMenuWidget::create_node_edge_layout() -> QWidget * {
  auto Node_layout = new QHBoxLayout(this);
  auto node_num = new QLabel(this);
  node_num->setText("Node num: ");
  node_num->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");

  Node_layout->addWidget(node_num);
  Node_layout->addWidget(node_num_value_);

  auto Node = new QWidget(this);
  Node->setLayout(Node_layout);
  Node->setMinimumHeight(70);
  Node->setStyleSheet("font-size: 18px;"
                      "border: none");

  auto Edge_layout = new QHBoxLayout(this);
  auto edge_weight = new QLabel(this);
  edge_weight->setText("Edge weight limit:");
  edge_weight->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
  Edge_layout->addWidget(edge_weight);
  Edge_layout->addWidget(edge_weight_value_);

  auto Edge = new QWidget(this);
  Edge->setLayout(Edge_layout);
  Edge->setMinimumHeight(70);
  Edge->setStyleSheet("font-size: 18px;"
                      "border: none");

  auto Edge_num_layout = new QHBoxLayout(this);
  auto edges_per_node = new QLabel(this);
  edges_per_node->setText("Max edges per node: ");
  edges_per_node->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
  Edge_num_layout->addWidget(edges_per_node);
  Edge_num_layout->addWidget(edges_per_node_value_);

  auto Edge_num = new QWidget(this);
  Edge_num->setLayout(Edge_num_layout);
  Edge_num->setMinimumHeight(70);
  Edge_num->setStyleSheet("font-size: 18px;"
                          "border: none");

  auto generate_graph = new QPushButton(this);
  generate_graph->setText("Generate graph");
  generate_graph->setStyleSheet(
      "QPushButton"
      "{color: #b1b1b1;"
      "background-color: #565656;"
      "border-width: 1px;"
      "border-color: #1e1e1e;"
      "border-style: solid;"
      "border-radius: 6;"
      "padding: 3px;"
      "font-size: 18px;"
      "padding-left: 5px;"
      "padding-right: 5px;}"
      "QPushButton:hover"
      "{background-color: #A33643;"
      "widget-animation-duration: 100}"
      "QPushButton:pressed"
      "{background-color: #AA6439;"
      "border-color: #AA6439;"
      "widget-animation-duration: 100}");
  connect(generate_graph, &QPushButton::clicked, this, &RightMenuWidget::pre_generate);

  auto V_layout = new QVBoxLayout(this);
  V_layout->addWidget(Node);
  V_layout->addWidget(Edge);
  V_layout->addWidget(Edge_num);
  V_layout->addWidget(generate_graph);
  V_layout->addStretch();

  auto layout = new QWidget(this);
  layout->setLayout(V_layout);
  layout->setStyleSheet(
      "border: 3px solid gray;"
      "border-color: #A33643;"
      "margin-top: 27px;"
      "font-size: 14px;"
      "border-radius: 15px;");
  return layout;
}

auto RightMenuWidget::create_start_end_layout() -> QWidget *{
  auto start_id = new QLabel(this);
  start_id->setText("Start id: ");
  start_id->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");

  auto start_layout = new QHBoxLayout(this);
  start_layout->addWidget(start_id);
  start_layout->addWidget(start_id_value_);

  auto start = new QWidget(this);
  start->setLayout(start_layout);
  start->setMinimumHeight(70);

  auto end_id = new QLabel(this);
  end_id->setText("End id: ");
  end_id->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");

  auto end_layout = new QHBoxLayout(this);
  end_layout->addWidget(end_id);
  end_layout->addWidget(end_id_value_);

  auto end = new QWidget(this);
  end->setLayout(end_layout);
  end->setMinimumHeight(70);

  start->setStyleSheet("font-size: 18px;border: none");
  end->setStyleSheet("font-size: 18px;border: none");

  auto random = new QPushButton(this);
  random->setText("Random");
  connect(random, &QPushButton::clicked, this, &RightMenuWidget::random_start_end);
  random->setStyleSheet(
      "QPushButton"
      "{color: #b1b1b1;"
      "background-color: #565656;"
      "border-width: 1px;"
      "border-color: #1e1e1e;"
      "border-style: solid;"
      "border-radius: 6;"
      "padding: 3px;"
      "font-size: 18px;"
      "padding-left: 5px;"
      "padding-right: 5px;}"
      "QPushButton:hover"
      "{background-color: #A33643;"
      "widget-animation-duration: 100}"
      "QPushButton:pressed"
      "{background-color: #AA6439;"
      "border-color: #AA6439;"
      "widget-animation-duration: 100}");

  auto V_layout = new QVBoxLayout(this);
  V_layout->addWidget(start);
  V_layout->addWidget(end);
  V_layout->addWidget(random);

  auto layout = new QWidget(this);
  layout->setLayout(V_layout);
  layout->setStyleSheet(
      "border: 3px solid gray;"
      "border-color: #A33643;"
      "margin-top: 27px;"
      "font-size: 14px;"
      "border-radius: 15px;");
  return layout;
}

void RightMenuWidget::pre_start() {
  emit start(method_select_->currentIndex(), start_id_value_->value(), end_id_value_->value());
}

void RightMenuWidget::pre_generate() {
  emit generate_graph(node_num_value_->value(), edges_per_node_value_->value(), edge_weight_value_->value());
}

void RightMenuWidget::random_start_end() {
  if (node_num_value_->value() > 1) {
    start_id_value_->setValue(QRandomGenerator::global()->bounded(node_num_value_->value()));
    do {
      end_id_value_->setValue(QRandomGenerator::global()->bounded(node_num_value_->value()));
    } while (start_id_value_->value() == end_id_value_->value());
    logs_->appendPlainText("Start id set to: " + QString::number(start_id_value_->value()));
    logs_->appendPlainText("End id set to: " + QString::number(end_id_value_->value()));
    emit start_end_changed(start_id_value_->value(), end_id_value_->value());
  } else {
    logs_->appendHtml("<font color = red>Warning: number of nodes should be > 1</font>");
  }
}

void RightMenuWidget::nodes_created(int edges) {
  logs_->appendPlainText("Nodes created: " + QString::number(node_num_value_->value()));
  logs_->appendPlainText("Edges created: " + QString::number(edges));
}

void RightMenuWidget::nodes_error() {
  if (!node_num_value_->value()) {
    logs_->appendHtml("<font color = red>Warning: number of nodes should be > 1</font>");
    return;
  }
  if (edges_per_node_value_->value() > node_num_value_->value()) {
    logs_->appendHtml("<font color = red>Warning: edges per node should be > nodes num</font>");
  }

}

void RightMenuWidget::log(QString msg) {
  logs_->appendHtml(msg);
}

