#include "windows.h"
#include <QApplication>

//open up the welcome window
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Windows w;
    return a.exec();
}
