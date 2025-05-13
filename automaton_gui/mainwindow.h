#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QTextBrowser>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void processInput();

private:
    QTextEdit *inputEdit;
    QTextBrowser *outputBrowser;
    QPushButton *processButton;

    // FA-related functions
    void setupAutomaton();
    bool testInputString(const std::string &input);
};

#endif // MAINWINDOW_H

