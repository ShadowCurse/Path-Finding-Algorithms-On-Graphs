#ifndef PROGRAMMWINDOW_H
#define PROGRAMMWINDOW_H

#include <QMainWindow>
#include "rightmenu.h"
#include "graphwidget.h"

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);

 private:
  RightMenuWidget *right_menu_widget_;
  GraphWidget *graph_widget_;

};

#endif // PROGRAMMWINDOW_H
