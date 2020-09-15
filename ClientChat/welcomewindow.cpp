#include "common.h"
#include "connection.h"
#include "welcomewindow.h"

//constructor
WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //set window's main properties
    this->setWindowTitle("Chat");
    this->setGeometry(0,0,800,600);
    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    QWidget *verticalLayoutWidget = new QWidget(centralWidget);
    verticalLayoutWidget->setGeometry(220,120,360,290);
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayoutWidget->setLayout(verticalLayout);

    //Connection Button
    connectionButton = new QPushButton("&Connect", verticalLayoutWidget);
    verticalLayout->addWidget(connectionButton);
    connect(connectionButton, SIGNAL(clicked()), this, SLOT(on_connectionButton_clicked()));

    //Log In Button
    logInButton = new QPushButton("&Log In", verticalLayoutWidget);
    logInButton->setEnabled(false);
    verticalLayout->addWidget(logInButton);
    connect(logInButton, SIGNAL(clicked()), this, SLOT(on_logInButton_clicked()));

    //Sign Up Button
    signUpButton = new QPushButton("&Sign Up", verticalLayoutWidget);
    signUpButton->setEnabled(false);
    verticalLayout->addWidget(signUpButton);
    connect(signUpButton, SIGNAL(clicked()), this, SLOT(on_signUpButton_clicked()));

    //Quit Button
    quitButton = new QPushButton("&Quit", verticalLayoutWidget);
    verticalLayout->addWidget(quitButton);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(on_quitButton_clicked()));

    connected=false;
    connection=new Connection();
}

//event handler for "Connection" button
void WelcomeWindow::on_connectionButton_clicked()
{
    //connect
    if (!connected){ //if not already connected
        //try to connect to the server
        ((Connection*)connection)->open_connection();


        //enable other options once connected
        logInButton->setEnabled(true);
        signUpButton->setEnabled(true);
        quitButton->setEnabled(false);
        connectionButton->setText("&Disconnect");

        connected = true;
    }
    //disconnect
    else {
        //close connection to the server
        ((Connection*)connection)->close_connection();

        //disable others options when disconnected
        logInButton->setEnabled(false);
        signUpButton->setEnabled(false);
        quitButton->setEnabled(true);
        connectionButton->setText("&Connect");

        connected = false;
    }

}

//event handler for "Log In" button
void WelcomeWindow::on_logInButton_clicked()
{}

//event handler for "Sign Up" button
void WelcomeWindow::on_signUpButton_clicked()
{}

//event handler for "Quit" button
void WelcomeWindow::on_quitButton_clicked()
{
    exit(EXIT_SUCCESS);
}

