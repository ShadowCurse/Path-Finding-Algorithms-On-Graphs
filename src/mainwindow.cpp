#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  right_menu_widget_ = new RightMenuWidget(this);
  graph_widget_ = new GraphWidget(this);

  QSizePolicy sp_left(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sp_left.setHorizontalStretch(1);
  graph_widget_->setSizePolicy(sp_left);

  QSizePolicy sp_right(QSizePolicy::Maximum, QSizePolicy::Maximum);
  sp_right.setHorizontalStretch(2);
  right_menu_widget_->setMinimumWidth(200);
  right_menu_widget_->setMaximumWidth(300);
  right_menu_widget_->setMaximumHeight(1000);
  right_menu_widget_->setSizePolicy(sp_right);

  graph_widget_->setMinimumWidth(500);
  auto qhbl = new QHBoxLayout();
  qhbl->addWidget(graph_widget_);
  qhbl->addWidget(right_menu_widget_);
  auto widget = new QWidget(this);
  widget->setLayout(qhbl);
  setCentralWidget(widget);
  setStyleSheet("background-color:rgb(15,15,15)");

  //connections
  QObject::connect(right_menu_widget_, &RightMenuWidget::generate_graph, graph_widget_, &GraphWidget::generate_graph);
  QObject::connect(right_menu_widget_, &RightMenuWidget::start_end_changed, graph_widget_, &GraphWidget::Start_End_Change);
  QObject::connect(right_menu_widget_, &RightMenuWidget::start, graph_widget_, &GraphWidget::start);
  QObject::connect(right_menu_widget_, &RightMenuWidget::constractional_heirarchy_prepricessing, graph_widget_, &GraphWidget::CHP_state_change);
  QObject::connect(right_menu_widget_, &RightMenuWidget::arc_flags_prepricessing, graph_widget_, &GraphWidget::Arc_Flags_state_change);
  QObject::connect(graph_widget_, &GraphWidget::Nodes_Created, right_menu_widget_, &RightMenuWidget::nodes_created);
  QObject::connect(graph_widget_, &GraphWidget::Nodes_Error, right_menu_widget_, &RightMenuWidget::nodes_error);
  QObject::connect(graph_widget_, &GraphWidget::log_msg, right_menu_widget_, &RightMenuWidget::log);
}
