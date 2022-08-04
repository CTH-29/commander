#include "mainwindow.h"
#include "ui_mainwindow.h"

#define CMD_BTN_NUM 36 /* Button number */
#define CMD_BTN_LINE_NUM 4 /* Button number each line */
#define CMD_BTN_W 100 /* Button width */
#define CMD_BTN_H 35  /* Button hight */
#define CMD_BTN_X 10 /* Button start position x */
#define CMD_BTN_Y 50  /* Button start position y */
#define CMD_BTN_GAP 10  /* Button gap */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ctrl_state = 0;

    settings=new QSettings("commander.ini", QSettings::IniFormat);
    this->creat_commandui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creat_commandui()
{
    this->creat_button();
    this->creat_tab();
}

void MainWindow::creat_button()
{
   int num=0;

   for (num = 0; num < CMD_BTN_NUM; num++) {
       cmdbtn = new QPushButton(this);
       cmdbtn->setParent(this->ui->tabWidget);
       cmdbtn->show();
       cmdbtn->setGeometry(CMD_BTN_X + (CMD_BTN_GAP + CMD_BTN_W) * (num % CMD_BTN_LINE_NUM),
                           CMD_BTN_Y + (CMD_BTN_GAP + CMD_BTN_H) * (num / CMD_BTN_LINE_NUM),
                           CMD_BTN_W, CMD_BTN_H);
       cmdbtn->setObjectName("cmdbtn"+QString::number((num)));
   }
}

void MainWindow::creat_tab()
{
    int i;
    int tap_num;

    tap_num = settings->value("BaseInfo/tapnum").toInt();

    for (i = 1; i < tap_num; i++) {
        this->ui->tabWidget->addTab(new QWidget(), "tap" + QString::number(i + 1));
    }

}
