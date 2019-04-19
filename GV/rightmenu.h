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

class RightMenu : public QWidget
{
    Q_OBJECT
public:
    explicit RightMenu(QWidget *parent = nullptr);

    void create_Node_Edge_Layout();
    void create_Start_End_Layout();
    QWidget *Node_Edge_Box;
    QWidget *Start_End_Box;
    QPushButton *Generate_Graph;
    QPushButton *Random;
    QPushButton *Start;
    QLabel *Node_num;
    QLabel *Edge_weight;
    QLabel *Edges_Per_Node;
    QLabel *Start_id;
    QLabel *End_id;
    QSpinBox *Node_num_value;
    QSpinBox *Edge_weight_value;
    QSpinBox *Edges_Per_Node_value;
    QSpinBox *Start_id_value;
    QSpinBox *End_id_value;
    QComboBox *Method_Select;
    QCheckBox *Constractional_Heirarchy_Prepricessing;
    QCheckBox *Arc_Flags_Prepricessing;
    QPlainTextEdit *Logs;

signals:
    void Start_End_Changed(int,int);
public slots:
    void Random_Start_End();
    void Nodes_Created(int);
    void Nodes_Error();
};

#endif // RIGHTMENU_H
