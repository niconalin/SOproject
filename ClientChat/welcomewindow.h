#pragma once

#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

private slots:
    void on_connectionButton_clicked();
    void on_logInButton_clicked();
    void on_signUpButton_clicked();
    void on_quitButton_clicked();

private:
    QPushButton *connectionButton;
    QPushButton *logInButton;
    QPushButton *signUpButton;
    QPushButton *quitButton;
    void *connection;
    bool connected;

signals:
    void clicked();
};
