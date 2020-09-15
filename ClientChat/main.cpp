#include "welcomewindow.h"

#include <QApplication>

//open up the welcome window
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeWindow w;
    w.show();
    return a.exec();
}
