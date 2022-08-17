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

    settings = new QSettings("commander.ini", QSettings::IniFormat);

    clipboard = QApplication::clipboard();

    connect(ui->actionOnTop, SIGNAL(triggered()), this, SLOT(set_fixOnTop()));
    connect(ui->actionShow, SIGNAL(triggered()), this, SLOT(set_Textedit_Visible()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(create_aboutWindows()));

    this->bt_width = 120;
    this->bt_height = 30;

    this->setWindowFlags(this->windowFlags()&~Qt::WindowMinMaxButtonsHint);

    this->create_tab();
    this->create_button();

//    this->ui->scrollArea->setLayout(new QVBoxLayout);
//    this->ui->scrollArea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    resize_button(this->size());
}

void MainWindow::create_tab()
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

    this->section = settings->value("BaseInfo/tapname1").toString();


}

void MainWindow::create_button()
{
    for (int num = 0; num < btn_list.count(); num++) {
        btn_list.at(num)->deleteLater();
    }
    btn_list.clear();
    QString prefix = QString("Tap") + QString::number(this->ui->tabWidget->currentIndex()+1) + "/";

    this->bt_cnt_now = settings->value(prefix + "cmdcnt").toInt();
    if(this->bt_cnt_now <= 0)
    {
        this->bt_cnt_now = 20;
        settings->setValue(prefix + "cmdcnt",20);
    }

//    if(this->scrollarea != NULL)
//    {
//        this->scrollarea->deleteLater();
//    }
//    this->ui->tabWidget->currentWidget()->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
//    this->scrollarea = new QScrollArea(this->ui->tabWidget->currentWidget());
//    this->scrollarea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
//    this->scrollarea->show();

//    this->ui->scrollArea->setParent(this->ui->tabWidget->currentWidget());
//    QLayout *lout = this->ui->scrollArea->layout();

    for (int num = 0; num < this->bt_cnt_now; num++) {
        cmdbtn = new QPushButton(this->ui->tabWidget);
        cmdbtn->setObjectName("cmd"+QString::number((num)));
        cmdbtn->setText(settings->value( prefix+"cmd" + QString::number(num)+"_k").toString());
        cmdbtn->setStatusTip(settings->value( prefix+"cmd" + QString::number(num)+"_V").toString());
        connect(cmdbtn, SIGNAL(clicked()), this, SLOT(on_pushCmdButton_clicked()));
        btn_list.append(cmdbtn);
        cmdbtn->show();
        //lout->addWidget(cmdbtn);
    }
    resize_button(this->size());
}

#define BT_EDGE_X 30
#define BT_EDGE_Y 30
#define BT_GAP_X 4
#define BT_GAP_Y 4

void MainWindow::resize_button(QSize size)
{
    qDebug()<<"w="<<size.width()<<"h"<<size.height()<<endl;

    int cnt_per_line = (size.width() - BT_EDGE_X*2)/(this->bt_width + BT_GAP_X);
    cnt_per_line = cnt_per_line < 1 ? 1 : cnt_per_line;
    int bt_tmp_width = (size.width() - BT_EDGE_X*2 - (cnt_per_line-1)*BT_GAP_X) / cnt_per_line;

    for (int num = 0; num < this->bt_cnt_now; num++) {
        btn_list.at(num)->setGeometry(BT_EDGE_X + (bt_tmp_width    + BT_GAP_X) * (num%cnt_per_line)-15,\
                                      BT_EDGE_Y + (this->bt_height + BT_GAP_Y) * (num/cnt_per_line),\
                                      bt_tmp_width ,this->bt_height);
    }

    this->ui->tabWidget->update();
}



void MainWindow::flush_ui(int index)
{

    this->section = settings->value("BaseInfo/tapname"+QString::number(index + 1)).toString();
    create_button();
    qDebug()<<"section:"<<this->section<< " bt_cnt_now:" << this->bt_cnt_now << endl;;
}

void MainWindow::set_Textedit_Visible()
{
    static bool visiable;

    this->ui->lineEdit->setVisible(visiable);
    this->ui->textEdit->setVisible(visiable);
    visiable = !visiable;
}
void MainWindow::set_fixOnTop()
{
    this->setWindowFlags(this->windowFlags() ^ Qt::WindowStaysOnTopHint);
    this->show();
}

void MainWindow::create_aboutWindows()
{
    QMessageBox::information(this,"source code","https://github.com/Savior2016/commander\n"
                         "(github address already copy to the clipboard)\n"
                         "\n"
                         "click: copy cmd to clipboard\n"
                         "ctrl + click: save cmd to button\n"
                         "ctrl + double click tap: change tap name\n"
                         ,QMessageBox::Yes);
    clipboard->setText("https://github.com/Savior2016/commander");
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    double val = position/80.0 + 0.2;
    setWindowOpacity(val);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    qDebug()<<"tap index"<<index<<endl;
    this->flush_ui(index);
}

void MainWindow::on_pushCmdButton_clicked()
{
    /* Get pushed button. */

    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    /* Get the cmd key and value. */
    QString prefix = QString("Tap") + QString::number(this->ui->tabWidget->currentIndex()+1) + "/";
    QString btn_key = prefix + btn->objectName()+"_k";
    QString btn_cmd = prefix + btn->objectName()+"_v";

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
        this->flush_ui(this->ui->tabWidget->currentIndex());
    } else {
        clipboard->setText(cmd_value);
        this->ui->statusbar->showMessage("copied",1000);
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

