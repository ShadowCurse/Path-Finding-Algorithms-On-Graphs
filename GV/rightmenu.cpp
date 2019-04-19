#include "rightmenu.h"
#include <QRandomGenerator>
RightMenu::RightMenu(QWidget *parent) : QWidget(parent)
{
    Generate_Graph = new QPushButton(this);
    Generate_Graph->setText("Generate graph");
    Random = new QPushButton(this);
    Random->setText("Random");
    Start = new QPushButton(this);
    Start->setText("Start");
    Node_num = new QLabel(this);
    Node_num->setText("Node num: ");
    Edge_weight = new QLabel(this);
    Edge_weight->setText("Edge weight limit:");
    Edges_Per_Node = new QLabel(this);
    Edges_Per_Node->setText("Max edges per node: ");
    Start_id = new QLabel(this);
    Start_id->setText("Start id: ");
    End_id = new QLabel(this);
    End_id->setText("End id: ");
    Node_num_value = new QSpinBox(this);
    Node_num_value->setMaximum(1000000);
    Edge_weight_value = new QSpinBox(this);
    Edge_weight_value->setMaximum(1000000);
    Edges_Per_Node_value = new QSpinBox(this);
    Edges_Per_Node_value->setMaximum(1000000);
    Start_id_value = new QSpinBox(this);
    Start_id_value->setMaximum(1000000);
    End_id_value = new QSpinBox(this);
    End_id_value->setMaximum(1000000);
    Method_Select = new QComboBox(this);
    Method_Select->addItem("-------");
    Method_Select->addItem("Kruskal");
    Method_Select->addItem("Dijkstra");
    Method_Select->addItem("Arc_Flag_Dijkstra");
    Method_Select->addItem("Double_Dijkstra");
    Method_Select->addItem("CHP_Double_Dijkstra");
    Method_Select->addItem("AStar");
    Constractional_Heirarchy_Prepricessing = new QCheckBox(this);
    Constractional_Heirarchy_Prepricessing->setText("Constractional Heirarchy Preprocessing");
    Arc_Flags_Prepricessing = new QCheckBox(this);
    Arc_Flags_Prepricessing->setText("Arc Flags Preprocessing");


    Logs = new QPlainTextEdit(this);
    Logs->setReadOnly(true);
    Logs->appendPlainText("Logs");

    //layout managment
    create_Node_Edge_Layout();
    create_Start_End_Layout();
    Node_Edge_Box->setMinimumHeight(250);
    Start_End_Box->setMinimumHeight(190);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(Node_Edge_Box);
    main_layout->addWidget(Start_End_Box);
    main_layout->addWidget(Method_Select);
    main_layout->addWidget(Constractional_Heirarchy_Prepricessing);
    main_layout->addWidget(Arc_Flags_Prepricessing);
    Start->setStyleSheet("QPushButton{color: #b1b1b1;background-color: #565656 ;border-width: 1px;border-color: #1e1e1e;border-style: solid;border-radius: 6;padding: 3px;font-size: 18px;padding-left: 5px;padding-right: 5px;}QPushButton:hover {background-color: #A33643;widget-animation-duration: 100}QPushButton:pressed {background-color: #AA6439;border-color: #AA6439;widget-animation-duration: 100}");
    main_layout->addWidget(Start);
    main_layout->addWidget(Logs);
    setLayout(main_layout);

    setStyleSheet("QObject {background-color: rgb(56, 56, 56);color: #b1b1b1;widget-animation-duration: 100}QRadioButton{font-size: 14px;}QSpinBox{font-size: 10px;width: 20px;border-color: rgb(50, 50, 50);border-radius: 15px;background-color: rgb(50, 50, 50);}QSpinBox::up-button{subcontrol-origin: margin;subcontrol-position: center left;width: 0px;border-width: 1px;}QSpinBox::down-button  {subcontrol-origin: margin;subcontrol-position: center right;width: 0px;border-width: 0px;}QLabel{font-size: 16px;}QPlainTextEdit  {border: 3px solid gray;border-color: #A33643;margin-top: 15px;padding: 3px 3px 3px 3px;font-size: 14px;border-radius: 15px;}QScrollBar:vertical {border: 1px solid transparent;border-radius: 20px;width: 9	px;margin: 1px 1px 1px 1px;}QScrollBar::handle:vertical {background-color: #A33643;border-radius: 2px;min-height: 0px; }QScrollBar::add-page:vertical {background-color:  rgb(56, 56, 56);height: 10px;subcontrol-position: bottom;subcontrol-origin: margin;}QScrollBar::sub-page:vertical {background-color:  rgb(56, 56, 56);height: 10 px;subcontrol-position: top;subcontrol-origin: margin;}QScrollBar::add-line:vertical {background-color:  rgb(56, 56, 56);height: 10 px;subcontrol-position: bottom;subcontrol-origin: margin;}QScrollBar::sub-line:vertical {background-color:  rgb(56, 56, 56);height: 10 px;subcontrol-position: top;subcontrol-origin: margin;}");

    //connections
    QObject::connect(Random, SIGNAL(clicked()),this, SLOT(Random_Start_End()));

}

void RightMenu::create_Node_Edge_Layout(){
    Node_Edge_Box = new QWidget(this);
    QHBoxLayout *Node_layout = new QHBoxLayout(this);
    Node_num->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
    Node_layout->addWidget(Node_num);
    Node_layout->addWidget(Node_num_value);
    QWidget *Node = new QWidget(this);
    Node->setLayout(Node_layout);
    Node->setMinimumHeight(70);
    QHBoxLayout *Edge_layout = new QHBoxLayout(this);
    Edge_weight->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
    Edge_layout->addWidget(Edge_weight);
    Edge_layout->addWidget(Edge_weight_value);
    QWidget *Edge = new QWidget(this);
    Edge->setLayout(Edge_layout);
    Edge->setMinimumHeight(70);
    QHBoxLayout *Edge_num_layout = new QHBoxLayout(this);
    Edges_Per_Node->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
    Edge_num_layout->addWidget(Edges_Per_Node);
    Edge_num_layout->addWidget(Edges_Per_Node_value);
    QWidget *Edge_num = new QWidget(this);
    Edge_num->setLayout(Edge_num_layout);
    Edge_num->setMinimumHeight(70);
    QVBoxLayout *V_layout = new QVBoxLayout(this);
    Node->setStyleSheet("font-size: 18px;border: none");
    Edge->setStyleSheet("font-size: 18px;border: none");
    Edge_num->setStyleSheet("font-size: 18px;border: none");
    Generate_Graph->setStyleSheet("QPushButton{color: #b1b1b1;background-color: #565656 ;border-width: 1px;border-color: #1e1e1e;border-style: solid;border-radius: 6;padding: 3px;font-size: 18px;padding-left: 5px;padding-right: 5px;}QPushButton:hover {background-color: #A33643;widget-animation-duration: 100}QPushButton:pressed {background-color: #AA6439;border-color: #AA6439;widget-animation-duration: 100}");
    V_layout->addWidget(Node);
    V_layout->addWidget(Edge);
    V_layout->addWidget(Edge_num);
    V_layout->addWidget(Generate_Graph);
    V_layout->addStretch();
    Node_Edge_Box->setLayout(V_layout);
    Node_Edge_Box->setStyleSheet("border: 3px solid gray;border-color: #A33643;margin-top: 27px;font-size: 14px;border-radius: 15px;");
}
void RightMenu::create_Start_End_Layout(){
    Start_End_Box = new QWidget(this);
    QHBoxLayout *Start_layout = new QHBoxLayout(this);
    Start_id->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
    Start_layout->addWidget(Start_id);
    Start_layout->addWidget(Start_id_value);
    QWidget *Start = new QWidget(this);
    Start->setLayout(Start_layout);
    Start->setMinimumHeight(70);
    QHBoxLayout *End_layout = new QHBoxLayout(this);
    End_id->setStyleSheet("border-bottom: 3px solid;border-color: #A33643;");
    End_layout->addWidget(End_id);
    End_layout->addWidget(End_id_value);
    QWidget *End = new QWidget(this);
    End->setLayout(End_layout);
    End->setMinimumHeight(70);
    QVBoxLayout *V_layout = new QVBoxLayout(this);
    Start->setStyleSheet("font-size: 18px;border: none");
    End->setStyleSheet("font-size: 18px;border: none");
    Random->setStyleSheet("QPushButton{color: #b1b1b1;background-color: #565656 ;border-width: 1px;border-color: #1e1e1e;border-style: solid;border-radius: 6;padding: 3px;font-size: 18px;padding-left: 5px;padding-right: 5px;}QPushButton:hover {background-color: #A33643;widget-animation-duration: 100}QPushButton:pressed {background-color: #AA6439;border-color: #AA6439;widget-animation-duration: 100}");
    V_layout->addWidget(Start);
    V_layout->addWidget(End);
    V_layout->addWidget(Random);
    Start_End_Box->setLayout(V_layout);
    Start_End_Box->setStyleSheet("border: 3px solid gray;border-color: #A33643;margin-top: 27px;font-size: 14px;border-radius: 15px;");
}
void RightMenu::Random_Start_End(){
    if (Node_num_value->value() > 1){
        Start_id_value->setValue(QRandomGenerator::global()->bounded(Node_num_value->value()));
        do{
        End_id_value->setValue(QRandomGenerator::global()->bounded(Node_num_value->value()));
        }while(Start_id_value->value() == End_id_value->value());
        Logs->appendPlainText("Start id set to: " + QString::number(Start_id_value->value()));
        Logs->appendPlainText("End id set to: " + QString::number(End_id_value->value()));
        emit Start_End_Changed(Start_id_value->value(),End_id_value->value());
    }
    else {
        Logs->appendHtml("<font color = red>Warning: number of nodes should be > 1</font>");
    }
}

void RightMenu::Nodes_Created(int edges)
{
    Logs->appendPlainText("Nodes created: " + QString::number(Node_num_value->value()));
    Logs->appendPlainText("Edges created: " + QString::number(edges));
}

void RightMenu::Nodes_Error()
{
    if (!Node_num_value->value()){
        Logs->appendHtml("<font color = red>Warning: number of nodes should be > 1</font>");
        return;
    }
    if (Edges_Per_Node_value->value() > Node_num_value->value()){
        Logs->appendHtml("<font color = red>Warning: edges per node should be > nodes num</font>");
    }

}
