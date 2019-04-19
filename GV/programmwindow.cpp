#include "programmwindow.h"

ProgrammWindow::ProgrammWindow(QWidget *parent) : QMainWindow(parent)
{
    right_menu = new RightMenu(this);
    Graph = new GraphWidget(this);
    Graph->rm = right_menu;
    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    Graph->setSizePolicy(spLeft);
    QSizePolicy spRight(QSizePolicy::Maximum, QSizePolicy::Maximum);
    spRight.setHorizontalStretch(2);
    right_menu->setMinimumWidth(200);
    right_menu->setMaximumWidth(300);
    right_menu->setMaximumHeight(1000);
    right_menu->setSizePolicy(spRight);
    Graph->setMinimumWidth(500);
    QHBoxLayout *QHBL = new QHBoxLayout();
    QHBL->addWidget(Graph);
    QHBL->addWidget(right_menu);
    QWidget *widget = new QWidget(this);
    widget->setLayout(QHBL);
    setCentralWidget(widget);
    setStyleSheet("background-color:rgb(15,15,15)");
    //connections
    QObject::connect(right_menu->Generate_Graph, SIGNAL(clicked()), Graph, SLOT(Generate_Graph()));
    QObject::connect(Graph, SIGNAL(Nodes_Created(int)), right_menu, SLOT(Nodes_Created(int)));//logs message
    QObject::connect(right_menu, SIGNAL(Start_End_Changed(int,int)), Graph, SLOT(Start_End_Change(int,int)));
    QObject::connect(right_menu->Start, SIGNAL(clicked()), Graph, SLOT(Start()));
    QObject::connect(Graph, SIGNAL(Nodes_Error()), right_menu, SLOT(Nodes_Error()));
    QObject::connect(right_menu->Constractional_Heirarchy_Prepricessing, SIGNAL(stateChanged(int)),Graph,SLOT(CHP_state_change()));
    QObject::connect(right_menu->Arc_Flags_Prepricessing, SIGNAL(stateChanged(int)),Graph,SLOT(Arc_Flags_state_change()));
}
