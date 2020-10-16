#pragma once

#include <QDesktopWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>

class Connection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_startStopButton_clicked();
    void on_quitButton_clicked();

private:
    QPushButton *startStopButton;
    QPushButton *quitButton;
    Connection *connection;
    bool started;

signals:
    void clicked();
};
