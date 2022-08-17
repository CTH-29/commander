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
#include <QScrollArea>

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

    int bt_width;
    int bt_height;
    int bt_cnt_now;


    void create_tab();
    void create_button();
    void flush_ui(int index);

    void resizeEvent(QResizeEvent *event);
    void resize_button(QSize size);

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

private slots:
    void create_aboutWindows();
    void set_Textedit_Visible();
    void set_fixOnTop();
    void on_horizontalSlider_sliderMoved(int position);
    void on_tabWidget_currentChanged(int index);
    void on_pushCmdButton_clicked();
    void on_tabWidget_tabBarDoubleClicked(int index);

};
#endif // MAINWINDOW_H
