#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAX_X 10  //表示画布中能显示的最大的x坐标
#define CANVAS_SIZE  (height()-40)
#include <QMainWindow>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

private slots:
    void on_infileButton_clicked();//读入文件内容到msg，并对msg解析，并呈现在当前几何图形的框中，更新figures

    void on_findFileButton_clicked();//从文件夹中选择文件并显示
    

    void on_addCButton_clicked();

    void on_addLineButton_clicked();

    void on_clearButton_clicked();

    void on_deleteButton_clicked();
    

   
    void on_drawFigureButton_clicked();

private:
    Ui::MainWindow *ui;
    vector<string> figures; //保存当前几何对象
    vector<pair<double,double>> points;//保存求解的交点
    QString msg;//当前需要处理的信息
    QString filePath;

    void init_canvas();
    void draw_line(int x1, int y1, int x2, int y2, QColor const c = Qt::black);
    void draw_seg(int x1, int y1, int x2, int y2, QColor const c = Qt::green);
    void draw_ray(int x1, int y1, int x2, int y2, QColor const c = Qt::blue);
    void draw_circle(int x0, int y0, int r, QColor const c = Qt::black);
    void draw_figure_from_str(string str);
    
};

#endif // MAINWINDOW_H
