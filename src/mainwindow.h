#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QSettings>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QList>

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
    QString section;
    QSettings *settings;
    QPushButton *cmdbtn;
    QClipboard *clipboard;
    QList<QPushButton*> btn_list;

    void flush_ui();
    void creat_tab();
    void creat_button();
    void creat_commandui();

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

private slots:
    void creat_aboutWindows();
    void set_Textedit_Visible();
    void set_fixOnTop();
    void on_horizontalSlider_sliderMoved(int position);
    void on_tabWidget_currentChanged(int index);
    void on_pushCmdButton_clicked();
    void on_tabWidget_tabBarDoubleClicked(int index);

};
#endif // MAINWINDOW_H
