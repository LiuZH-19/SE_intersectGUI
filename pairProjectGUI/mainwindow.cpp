#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPointF>
#include <QPen>
#include "QDir"
#include "QFileDialog"
#include "QTextStream"
#include "QFile"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow ::paintEvent(QPaintEvent *)
{
    init_canvas();
    if(figures.size()!=0){
         for(vector<string>::iterator iter=figures.begin(); iter!=figures.end(); ++iter ){
             draw_figure_from_str(*iter);
         }        
    }
}

void MainWindow::init_canvas()
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(0.05);
    painter.setRenderHint(QPainter::Antialiasing, true);//反锯齿
    painter.setPen(pen);

    painter.setViewport(20,20,CANVAS_SIZE,CANVAS_SIZE);
    //前面两个参数 左上角坐标  后面两个参数 宽和高
    painter.setWindow(-MAX_X,MAX_X,2*MAX_X,-2*MAX_X);
    painter.fillRect(-MAX_X,MAX_X,2*MAX_X,-2*MAX_X,Qt::white);

    //绘制坐标轴
    painter.drawLine(0,MAX_X,0,-MAX_X);
    painter.drawLine(MAX_X,0,-MAX_X,0);

}

void MainWindow::on_findFileButton_clicked()
{
    filePath=
                QDir::toNativeSeparators(QFileDialog::getOpenFileName(this,tr("Save path"),QDir::currentPath()));  //文件路径

            if(!filePath.isEmpty())
            {
                if(ui->fileBox->findText(filePath)==-1)
                    ui->fileBox->addItem(filePath);//在comboBox中显示文件路径
            }

}


void MainWindow::on_infileButton_clicked()
{
    figures.clear();//清空当前几何图形
    QFile *file=new QFile;   //申请一个文件指针
    file->setFileName(filePath);   //设置文件路径
    bool ok=file->open(QIODevice::ReadOnly);
            if(ok)
            {
                QTextStream in(file);
                msg = in.readAll();
                ui->textBrowser->append("msg:\n"+msg);
                QStringList list = msg.split("\n");
                int n = list.at(0).toInt();//结合体个数
                for(int i=1;i<n+1;i++){//在listWidget中展示 并存入figures中
                    QListWidgetItem *elem1 = new QListWidgetItem(list.at(i), ui->listWidget);
                      elem1->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
                      elem1->setCheckState(Qt::Unchecked);
                      figures.push_back(list.at(i).toStdString());
                }
                
                file->close();
            }
             ui->textBrowser->append("infile_button figurs_size:"+QString::number(figures.size())+"\n");
}



void MainWindow::on_addCButton_clicked()
{
    QString x0 = ui->x0Box->text();
    QString y0 = ui->y0Box->text();
    QString r = ui->rBox->text();
    QString str = "C "+x0+" "+y0+" "+r;
    QListWidgetItem *elem = new QListWidgetItem(str, ui->listWidget);
    elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    elem->setCheckState(Qt::Unchecked);
    figures.push_back(str.toStdString());
    ui->textBrowser->append("addC_button figurs_size:"+QString::number(figures.size())+"\n");
}

void MainWindow::on_addLineButton_clicked()
{
    QString x1 = ui->x1Box->text();
    QString y1 = ui->y1Box->text();
    QString x2 = ui->x2Box->text();
    QString y2 = ui->y2Box->text();
    QString str0 =x1+" "+y1+" "+x2+" "+y2;
    int type =ui->lineTypeBox->currentIndex();
    QString str;
    if(type==0){
        str = "L "+str0;
    }else if(type == 1){
        str = "R "+str0;
    }
    else{
        str = "S "+str0;
    }
    QListWidgetItem *elem = new QListWidgetItem(str, ui->listWidget);
    elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    elem->setCheckState(Qt::Unchecked);
    figures.push_back(str.toStdString());
    ui->textBrowser->append("addL_button figurs_size:"+QString::number(figures.size())+"\n");
}



void MainWindow::on_clearButton_clicked()
{
    figures.clear();
    ui->listWidget->clear();
    update();
    ui->textBrowser->append("clear_button figurs_size:"+QString::number(figures.size())+"\n");
}

void MainWindow::on_deleteButton_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *aItem = ui->listWidget->item(i);//获取一个项
        if (aItem->checkState() == Qt::Checked) {
            ui->listWidget->takeItem(i);
            delete aItem;
            figures.erase(figures.begin() + i);
            i--;
        }
    }
    ui->textBrowser->append("delete_button figurs_size:"+QString::number(figures.size())+"\n");
    /*
    for(size_t j=0;j<figures.size();j++){
        ui->textBrowser->append(QString::fromStdString(figures.at(j)));
    }
    */

}

void MainWindow::on_drawFigureButton_clicked()
{
    ui->textBrowser->append("drawALL_button figurs_size:"+QString::number(figures.size())+"\n");
    update();
}


void MainWindow::draw_ray(int x1, int y1, int x2, int y2, QColor const c){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//反锯齿
    painter.setViewport(20,20,CANVAS_SIZE,CANVAS_SIZE);
    painter.setWindow(-MAX_X,MAX_X,2*MAX_X,-2*MAX_X);
    QPen pen;
    pen.setColor(c);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(0.08);
    painter.setPen(pen);


    if (x2 == x1) {
            if (y2 > y1) {
                 painter.drawLine(x1,y1,x1,MAX_X);
            }
            else {
                 painter.drawLine(x1,y1,x1,-MAX_X);
            }
        }
        else if (y1 == y2) {
            if (x2 > x1) {
               painter.drawLine(x1,y1,MAX_X,y1);
            }
            else {
                painter.drawLine(x1,y1,-MAX_X,y1);
            }
        }
    else {
            double k = (double)(y2 - y1) / (x2 - x1);
            double b = y1 - k * x1;
            if (x2 > x1) {
                double y_ = MAX_X* k + b;
                if (y_ > MAX_X) {
                    y2 = MAX_X;
                    x2 = (y2 - b) / k;
                }
                else if (y_ < -MAX_X) {
                    y2 = -MAX_X;
                    x2 = (y2 - b) / k;
                }
                else {
                    x2 = MAX_X;
                    y2 = y_;
                }
            }
            else {
                double y_ = -MAX_X * k + b;
                if (y_ > MAX_X) {
                    y2 = MAX_X;
                    x2 = (y2 - b) / k;
                }
                else if (y_ < -MAX_X) {
                    y2 = -MAX_X;
                    x2 = (y2 - b) / k;
                }
                else {
                    x2 = -MAX_X;
                    y2 = y_;
                }
            }
             painter.drawLine(x1,y1,x2,y2);
        }
}

void MainWindow::draw_line(int x1, int y1, int x2, int y2, QColor const c )
{   /*
    draw_ray(x1,y1,x2,y2,c);
    draw_ray(x2,y2,x1,y1,c);
    */
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//反锯齿
    painter.setViewport(20,20,CANVAS_SIZE,CANVAS_SIZE);
    painter.setWindow(-MAX_X,MAX_X,2*MAX_X,-2*MAX_X);
    QPen pen;
    pen.setColor(c);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(0.08);
    painter.setPen(pen);

    double A =y1-y2;
    double B = x2-x1;
    double C = x1*y2-x2*y1;
    if(A==0){
        QPoint p1(-MAX_X,y1);
        QPoint p2(MAX_X,y1);
        painter.drawLine(p1,p2);
        return;
    }else if(B==0){
        QPoint p1(x1,-MAX_X);
        QPoint p2(x2,MAX_X);
        painter.drawLine(p1,p2);
        return;
    }else{
        QPoint p1;
        QPoint p2;
        double data1,data2,data3,data4;
        data1 = (-C-A*MAX_X)/B;//当x取MAX_X时，y的值
        data2 = (-C+A*MAX_X)/B;//当x取-MAX_X时，y的值
        data3 = (-C-B*MAX_X)/B;//当y取MAX_X时，x的值
        data4 = (-C+B*MAX_X)/B;//当y取-MAX_X时，x的值
        if(data1>=-MAX_X&&data1<=MAX_X
           &&data2>=-MAX_X&&data2<=MAX_X){
           painter.drawLine(MAX_X,data1,-MAX_X,data2);
           return;
        }else if(data1>=-MAX_X&&data1<=MAX_X
                 &&data3>=-MAX_X&&data3<=MAX_X){
           painter.drawLine(MAX_X,data1,data3,MAX_X);
           return;
        }else if(data1>=-MAX_X&&data1<=MAX_X
                 &&data4>=-MAX_X&&data4<=MAX_X){
           painter.drawLine(MAX_X,data1,data4,-MAX_X);
           return;
        }else if(data2>=-MAX_X&&data2<=MAX_X
                 &&data3>=-MAX_X&&data3<=MAX_X){
           painter.drawLine(-MAX_X,data2,data3,MAX_X);
           return;
        }else if(data2>=-MAX_X&&data2<=MAX_X
                 &&data4>=-MAX_X&&data4<=MAX_X){
           painter.drawLine(-MAX_X,data2,data4,-MAX_X);
           return;
        }else if(data3>=-MAX_X&&data3<=MAX_X
                 &&data4>=-MAX_X&&data4<=MAX_X){
           painter.drawLine(data3,MAX_X,data4,-MAX_X);
           return;
        }


    }

}

void MainWindow:: draw_seg(int x1, int y1, int x2, int y2, QColor const c )
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//反锯齿
    painter.setViewport(20,20,CANVAS_SIZE,CANVAS_SIZE);
    painter.setWindow(-MAX_X,MAX_X,2*MAX_X,-2*MAX_X);
    QPen pen;
    pen.setColor(c);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(0.08);
    painter.setPen(pen);


    painter.drawLine(x1,y1,x2,y2);
}

void MainWindow::draw_circle(int x0, int y0, int r, QColor const c)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//反锯齿
    painter.setViewport(20,20,CANVAS_SIZE,CANVAS_SIZE);
    painter.setWindow(-MAX_X,MAX_X,2*MAX_X,-2*MAX_X);

    QPen pen;
    pen.setColor(c);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(0.08);
    painter.setPen(pen);

    painter.drawEllipse(x0-r,y0-r, 2*r,2*r);
}

void MainWindow::draw_figure_from_str(string str)
{
    QStringList list =(QString::fromStdString(str)).split(" ");
    QString type = list.at(0);
    int x1,y1,x2,y2,r;
    if(list.size()==5){
        x1=list.at(1).toInt();
        y1 = list.at(2).toInt();
        x2 = list.at(3).toInt();
        y2 = list.at(4).toInt();
    }else{
        x1=list.at(1).toInt();
        y1 = list.at(2).toInt();
        r = list.at(3).toInt();
    }
        
    if (type == QString::fromLocal8Bit("L")) {
            draw_line(x1,y1,x2,y2);
        }
     else if (type == QString::fromLocal8Bit("S")) {
            draw_seg(x1,y1,x2,y2);
        }
     else if (type == QString::fromLocal8Bit("R")) {
            draw_ray(x1,y1,x2,y2);
        }
     else {
            draw_circle(x1,y1,r);
        }
}



