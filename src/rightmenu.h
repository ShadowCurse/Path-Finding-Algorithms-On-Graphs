#ifndef RIGHTMENU_H
#define RIGHTMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QComboBox>

class RightMenuWidget : public QWidget {
 Q_OBJECT
 public:
  explicit RightMenuWidget(QWidget *parent = nullptr);

 signals:
  void generate_graph(int node_num, int edges_per_node, double edge_weight);
  void start(int method, int start_id, int end_id);
  void constractional_heirarchy_prepricessing(int);
  void arc_flags_prepricessing(int);
  void start_end_changed(int, int);

 public slots:
  void pre_start();
  void pre_generate();
  void random_start_end();
  void nodes_created(int);
  void nodes_error();
  void log(QString msg);

 private:
  friend class MainWindow;

  [[nodiscard]] auto create_node_edge_layout() -> QWidget *;
  [[nodiscard]] auto create_start_end_layout() -> QWidget *;

  QSpinBox *node_num_value_;
  QSpinBox *edge_weight_value_;
  QSpinBox *edges_per_node_value_;
  QSpinBox *start_id_value_;
  QSpinBox *end_id_value_;
  QComboBox *method_select_;
  QCheckBox *constractional_heirarchy_prepricessing_;
  QCheckBox *arc_flags_prepricessing_;
  QPlainTextEdit *logs_;
};

#endif // RIGHTMENU_H
