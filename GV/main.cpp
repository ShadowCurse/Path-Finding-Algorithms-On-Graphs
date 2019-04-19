#include <QApplication>
#include <QMainWindow>
#include "programmwindow.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ProgrammWindow main_window;
    main_window.show();

    return app.exec();
}
