#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QSettings>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int ctrl_state;
    QSettings *settings;
    QPushButton *cmdbtn;

    void creat_commandui();
    void creat_button();
    void creat_tab();
protected:
//    virtual void keyPressEvent(QKeyEvent *ev);
//    virtual void keyReleaseEvent(QKeyEvent *ev);
};
#endif // MAINWINDOW_H
