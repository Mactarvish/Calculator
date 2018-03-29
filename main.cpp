#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    //w.hide();
    //w.setWindowFlags(Qt::WindowStaysOnTopHint);
    w.show();

    return a.exec();
}
