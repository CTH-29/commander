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

    clipboard = QApplication::clipboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creat_commandui()
{
    this->creat_button();
    this->creat_tab();
    this->flush_ui();
}

void MainWindow::creat_button()
{
   int num;

   for (num = 0; num < CMD_BTN_NUM; num++) {
       cmdbtn = new QPushButton(this);
       cmdbtn->setParent(this->ui->tabWidget);
       cmdbtn->setGeometry(CMD_BTN_X + (CMD_BTN_GAP + CMD_BTN_W) * (num % CMD_BTN_LINE_NUM),
                           CMD_BTN_Y + (CMD_BTN_GAP + CMD_BTN_H) * (num / CMD_BTN_LINE_NUM),
                           CMD_BTN_W, CMD_BTN_H);
       cmdbtn->setObjectName("cmd"+QString::number((num)));
       connect(cmdbtn, SIGNAL(clicked()), this, SLOT(on_pushCmdButton_clicked()));
       btn_list.append(cmdbtn);
       cmdbtn->show();
   }
}

void MainWindow::creat_tab()
{
    int i;
    int tap_num;

    tap_num = settings->value("BaseInfo/tapnum").toInt();

    /* Add taps. */

    for (i = 1; i < tap_num; i++) {
        this->ui->tabWidget->addTab(new QWidget(), "tap" + QString::number(i + 1));
    }

    /* Set the tap name by ini file. */

    for (i = 0; i < tap_num; i++) {
        QString tap_name = settings->value("BaseInfo/tapname" + QString::number(i + 1)).toString();
        this->ui->tabWidget->setTabText(i, tap_name);
    }
}

void MainWindow::flush_ui()
{
    int i;
    int tap_num;
    int tap_current;

    /* Flash taps. */

    tap_num = settings->value("BaseInfo/tapnum").toInt();

    for (i = 0; i < tap_num; i++) {
        QString tap_name = settings->value("BaseInfo/tapname" + QString::number(i + 1)).toString();
        this->ui->tabWidget->setTabText(i, tap_name);
    }

    /* Flash buttons. */

    tap_current = this->ui->tabWidget->currentIndex();
    this->section = "Tap" + QString::number(tap_current + 1);

   for (i = 0; i < CMD_BTN_NUM; i++) {
       QString btn_name = settings->value(this->section + "/" + btn_list.at(i)->objectName()+"_k").toString();
       btn_list.at(i)->setText(btn_name);
   }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    setWindowOpacity(position/100.0);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    qDebug()<<"tap index"<<index<<endl;
    this->flush_ui();
}

void MainWindow::on_pushCmdButton_clicked()
{
    /* Get pushed button. */

    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    /* Get the cmd key and value. */

    QString btn_key = this->section + "/" + btn->objectName()+"_k";
    QString btn_cmd = this->section + "/" + btn->objectName()+"_v";

    QString cmd_key = settings->value(btn_key).toString();
    QString cmd_value = settings->value(btn_cmd).toString();

    /* Set to the edit. */

    if (this->ctrl_state == 0) {
       this->ui->lineEdit->setText(cmd_key);
       this->ui->textEdit->setText(cmd_value);
    }

    /* Save or copy cmd. */

    if (this->ctrl_state == 1) {
        settings->setValue(btn_key, this->ui->lineEdit->text());
        settings->setValue(btn_cmd, this->ui->textEdit->toPlainText());
        this->flush_ui();
    } else {
        clipboard->setText(cmd_value);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Control)
    {
        this->ctrl_state=1;
        return;
    }
    QWidget::keyPressEvent(ev);
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Control)
    {
        this->ctrl_state=0;
        return;
    }

    QWidget::keyReleaseEvent(ev);
}

void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    if (this->ctrl_state == 1) {
        this->ui->tabWidget->setTabText(index, this->ui->lineEdit->text());
        settings->setValue("BaseInfo/tapname" + QString::number(index + 1), this->ui->lineEdit->text());
    }
}

void MainWindow::on_toolButton_clicked()
{
    QMessageBox::warning(this,"source code","https://github.com/Savior2016/commander\n"
                         "(github address already copy to the clipboard)\n"
                         "\n"
                         "click: copy cmd to clipboard\n"
                         "ctrl + click: save cmd to button\n"
                         "ctrl + double click tap: change tap name\n"
                         ,QMessageBox::Yes);
   clipboard->setText("https://github.com/Savior2016/commander");
}
