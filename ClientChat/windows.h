#pragma once

#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

class Connection;

class Windows : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Windows(QWidget *parent = nullptr);
    ~Windows();

  private:
    QStackedWidget* windows;
    Connection* connection;
    QWidget* options;
    QWidget* login;
    QWidget* registration;
    QLineEdit* editUsername;
    QLineEdit* editPassword;
    QLineEdit* editNewUsername;
    QLineEdit* editNewPassword;

  private slots:
    void on_signInButton_clicked();
    void on_signUpButton_clicked();
    void on_quitButton_clicked();
    void on_logInButton_clicked();
    void on_backLogInButton_clicked();
    void on_registrationButton_clicked();
    void on_backRegistrationButton_clicked();
  signals:
    void clicked();
};
