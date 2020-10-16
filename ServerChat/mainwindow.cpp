#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#include "common.h"
#include "connection.h"
#include "mainwindow.h"

//constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  if (DEBUG) cerr << "MainWindow: STARTING..." << endl;

  //set window's main properties
  this->setWindowTitle("Server");
  QDesktopWidget* desktop = new QDesktopWidget();
  this->setGeometry(desktop->availableGeometry(this).width() * 0.25,
                    desktop->availableGeometry(this).height() * 0.25,
                    desktop->availableGeometry(this).width() * 0.5,
                    desktop->availableGeometry(this).height() * 0.5);
  QWidget *centralWidget = new QWidget(this);
  this->setCentralWidget(centralWidget);
  QWidget *verticalLayoutWidget = new QWidget(centralWidget);
  verticalLayoutWidget->setGeometry(this->width() * 0.25,
                                    this->height() * 0.25,
                                    this->width() * 0.5,
                                    this->height() * 0.5);
  QVBoxLayout *verticalLayout = new QVBoxLayout();
  verticalLayoutWidget->setLayout(verticalLayout);

  //Start / Stop Button
  startStopButton = new QPushButton("&Start", verticalLayoutWidget);
  verticalLayout->addWidget(startStopButton);
  connect(startStopButton, SIGNAL(clicked()), this, SLOT(on_startStopButton_clicked()));

  //Quit Button
  quitButton = new QPushButton("&Quit", verticalLayoutWidget);
  verticalLayout->addWidget(quitButton);
  connect(quitButton, SIGNAL(clicked()), this, SLOT(on_quitButton_clicked()));

  started = false;
}

//thread that open the socket for future connections
void* thread_routine(void* args) {
    ((Connection*)args)->start_server();
    return NULL;
}

//event handler for "Start"/"Stop" button
void MainWindow::on_startStopButton_clicked()
{
    //start the server
    if (!started) {//if not yet started
        connection = new Connection();
        /*we have to give the task to a thread to avoid to lock the app
         *while waiting for incoming connection*/
        pthread_t thread;
        pthread_create(&thread, NULL, thread_routine, (void*)connection);
        pthread_detach(thread);
        startStopButton->setText("&Stop");
        quitButton->setEnabled(false);
        started = true;
    }
    else { //stop the server
        connection->stop_server();
        startStopButton->setText("&Start");
        quitButton->setEnabled(true);
        started = false;
    }
}

//event handler for "Quit" button
void MainWindow::on_quitButton_clicked()
{
  if (DEBUG) cerr << "MainWindow: EXITING..." << endl;
  exit(EXIT_SUCCESS);
}
