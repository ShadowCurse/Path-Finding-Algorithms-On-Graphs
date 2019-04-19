#ifndef PROGRAMMWINDOW_H
#define PROGRAMMWINDOW_H

#include <QMainWindow>
#include "rightmenu.h"
#include "graphwidget.h"
class ProgrammWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ProgrammWindow(QWidget *parent = nullptr);
     RightMenu *right_menu;
     GraphWidget *Graph;
signals:

public slots:
};

#endif // PROGRAMMWINDOW_H
