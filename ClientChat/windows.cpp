#include <iostream>
using namespace std;

#include "common.h"
#include "connection.h"
#include "login.h"
#include "registration.h"
#include "windows.h"

//constructor
Windows::Windows(QWidget *parent) :
  QMainWindow(parent)
{
  if (DEBUG) cerr << "Windows: STARTING..." << endl;

  QWidget* centralWidget = new QWidget(this);
  this->windows = new QStackedWidget(centralWidget);

  //------------------------------OPTION WIDGET------------------------------//
  this->options = new QWidget(this->windows);

  QVBoxLayout *optionsLayout = new QVBoxLayout();
  this->options->setLayout(optionsLayout);

  //Sign In Button
  QPushButton* signInButton = new QPushButton("&Sign In", this->options);
  optionsLayout->addWidget(signInButton);
  connect(signInButton, SIGNAL(clicked()), this, SLOT(on_signInButton_clicked()));

  //Sign Up Button
  QPushButton* signUpButton = new QPushButton("&Sign Up", this->options);
  optionsLayout->addWidget(signUpButton);
  connect(signUpButton, SIGNAL(clicked()), this, SLOT(on_signUpButton_clicked()));

  //Quit Button
  QPushButton* quitButton = new QPushButton("&Quit", this->options);
  optionsLayout->addWidget(quitButton);
  connect(quitButton, SIGNAL(clicked()), this, SLOT(on_quitButton_clicked()));
  //-------------------------------------------------------------------------//

  //------------------------------LOG IN WIDGET------------------------------//
  this->login = new QWidget(this->windows);

  QGridLayout *logInLayout = new QGridLayout();
  this->login->setLayout(logInLayout);

  //Username and Password Form
  QWidget* loginFormWidget = new QWidget(this->login);
  this->editUsername = new QLineEdit(loginFormWidget);
  this->editPassword = new QLineEdit(loginFormWidget);
  editPassword->setEchoMode(QLineEdit::Password);
  QLabel* labelUsername = new QLabel(loginFormWidget);
  QLabel* labelPassword = new QLabel(loginFormWidget);
  labelUsername->setText(tr("Username:"));
  labelUsername->setBuddy(editUsername );
  labelPassword->setText(tr( "Password:"));
  labelPassword->setBuddy(editPassword );

  QGridLayout *loginFormLayout = new QGridLayout();
  loginFormLayout->addWidget(labelUsername, 0, 0);
  loginFormLayout->addWidget(editUsername, 0, 1);
  loginFormLayout->addWidget(labelPassword, 1, 0);
  loginFormLayout->addWidget(editPassword, 1, 1);
  loginFormWidget->setLayout(loginFormLayout);
  logInLayout->addWidget(loginFormWidget);

  //Log In Button
  QPushButton* logInButton = new QPushButton("&Log In", this->login);
  logInLayout->addWidget(logInButton);
  connect(logInButton, SIGNAL(clicked()), this, SLOT(on_logInButton_clicked()));

  //Back Button
  QPushButton* backLogInButton = new QPushButton("&Back", this->login);
  logInLayout->addWidget(backLogInButton);
  connect(backLogInButton, SIGNAL(clicked()), this, SLOT(on_backLogInButton_clicked()));
  //-------------------------------------------------------------------------//

  //------------------------------REGISTRATION WIDGET------------------------------//
  this->registration = new QWidget(this->windows);

  QGridLayout *registrationLayout = new QGridLayout();
  this->registration->setLayout(registrationLayout);

  //New Username and New Password Form
  QWidget* registrationFormWidget = new QWidget(this->registration);
  this->editNewUsername = new QLineEdit(registrationFormWidget);
  this->editNewPassword = new QLineEdit(registrationFormWidget);
  editNewPassword->setEchoMode(QLineEdit::Password);
  QLabel* labelNewUsername = new QLabel(registrationFormWidget);
  QLabel* labelNewPassword = new QLabel(registrationFormWidget);
  labelNewUsername->setText(tr("Username:"));
  labelNewUsername->setBuddy(editNewUsername );
  labelNewPassword->setText(tr( "Password:"));
  labelNewPassword->setBuddy(editNewPassword );

  QGridLayout *registrationFormLayout = new QGridLayout();
  registrationFormLayout->addWidget(labelNewUsername, 0, 0);
  registrationFormLayout->addWidget(editNewUsername, 0, 1);
  registrationFormLayout->addWidget(labelNewPassword, 1, 0);
  registrationFormLayout->addWidget(editNewPassword, 1, 1);
  registrationFormWidget->setLayout(registrationFormLayout);
  registrationLayout->addWidget(registrationFormWidget);

  //Sign Up Button
  QPushButton* registrationButton = new QPushButton("&Sign Up", this->registration);
  registrationLayout->addWidget(registrationButton);
  connect(registrationButton, SIGNAL(clicked()), this, SLOT(on_registrationButton_clicked()));

  //Back Button
  QPushButton* backRegistrationButton = new QPushButton("&Back", this->registration);
  registrationLayout->addWidget(backRegistrationButton);
  connect(backRegistrationButton, SIGNAL(clicked()), this, SLOT(on_backRegistrationButton_clicked()));
  //-------------------------------------------------------------------------//

  //set window's main properties
  QDesktopWidget* desktop = new QDesktopWidget();
  QVBoxLayout *layout = new QVBoxLayout();
  this->windows->addWidget(this->options);
  this->windows->addWidget(this->login);
  this->windows->addWidget(this->registration);
  this->windows->setGeometry(this->width() * 0.25,
                             this->height() * 0.25,
                             this->width() * 0.5,
                             this->height() * 0.5);
  layout->addWidget(this->windows);
  centralWidget->setLayout(layout);
  this->setCentralWidget(centralWidget);
  this->setGeometry(desktop->availableGeometry(this).width() * 0.25,
                    desktop->availableGeometry(this).height() * 0.25,
                    desktop->availableGeometry(this).width() * 0.5,
                    desktop->availableGeometry(this).height() * 0.5);
  this->setWindowTitle("Chat");

  //start new connection with the server
  connection = new Connection();
  this->connection->open_connection();
  this->show();
}

//deconstructor
Windows::~Windows()
{
  this->connection->close_connection();
}

//------------------------------EVENT HANDLERS------------------------------//

//event handler for "Sign In" button (Option Widget)
void Windows::on_signInButton_clicked()
{
  this->windows->setCurrentIndex(1);
}

//event handler for "Sign Up" button (Option Widget)
void Windows::on_signUpButton_clicked()
{
  this->windows->setCurrentIndex(2);
}

//event handler for "Quit" button (Option Widget)
void Windows::on_quitButton_clicked()
{
  this->~Windows();
  if (DEBUG) cerr << "Windows: EXITING..." << endl;
  exit(EXIT_SUCCESS);
}

//event handler for "Log In" button (Login Widget)
void Windows::on_logInButton_clicked()
{
  string username = this->editUsername->text().toStdString();
  string password = this->editPassword->text().toStdString();
  username.erase(remove(username.begin(), username.end(), ' '), username.end());
  password.erase(remove(password.begin(), password.end(), ' '), password.end());
  if (username == "" || password == "") {
    this->editUsername->clear();
    this->editPassword->clear();
    return;
  }
  Login *login = new Login(this->connection, username, password);
  login->send_login_data();
  login->wait_login_response();
  if (login->get_login_status()) {
    if (DEBUG) cerr << "Windows: LOGGED SUCCESSFULLY" << endl;
  }
  else
    if (DEBUG) cerr << "Windows: FAILED TO LOGIN" << endl;
  this->editUsername->clear();
  this->editPassword->clear();
  return;
}

//event handler for "Back" button (Login Widget)
void Windows::on_backLogInButton_clicked()
{
  this->windows->setCurrentIndex(0);
}

//event handler for "Sign Up" button (Registration Widget)
void Windows::on_registrationButton_clicked()
{
  string username = this->editNewUsername->text().toStdString();
  string password = this->editNewPassword->text().toStdString();
  username.erase(remove(username.begin(), username.end(), ' '), username.end());
  password.erase(remove(password.begin(), password.end(), ' '), password.end());
  if ((username == "" || password == "") ||
      (username.find(",") != string::npos || password.find(",") != string::npos)) {
    this->editNewUsername->clear();
    this->editNewPassword->clear();
    return;
  }
  Registration *registration = new Registration(this->connection, username, password);
  registration->send_registration_data();
  registration->wait_registration_response();
  switch (registration->get_registration_status()) {
    case REGISTRATED:
      if (DEBUG) cerr << "Windows: REGISTRATED SUCCESSFULLY" << endl;
      this->windows->setCurrentIndex(0);
      break;
    case ALREADY_REGISTRATED:
      if (DEBUG) cerr << "Windows: ALREADY REGISTRATED" << endl;
      break;
    case NOT_YET_REGISTRATED:
      if (DEBUG) cerr << "Windows: FAILED TO REGISTRATE" << endl;
      break;
    default:
      break;
  }
  this->editNewUsername->clear();
  this->editNewPassword->clear();
  return;
}

//event handler for "Back" button (Registration Widget)
void Windows::on_backRegistrationButton_clicked()
{
  this->windows->setCurrentIndex(0);
}

//--------------------------------------------------------------------------//
