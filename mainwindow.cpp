#include "mainwindow.h"
#include "ui_mainwindow.h"
#define PI 3.1415926

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Mat imgIn = imread("c.jpg", 0);
    qDebug() << "imgIn.size:" << imgIn.size;

    //柱面展开后的图像大小,将半圆柱展开为平面
    int colNum, rowNum;
    colNum = PI * imgIn.cols / 2;//柱面图像宽,根据半圆的周长公式 l = PI * D /2 计算,暂以原图像的宽度作为直径进行计算
    rowNum = imgIn.rows;//柱面图像高

    int radiusIn_mm = 20;//柱体的实际半径(mm),根据实际的成像系统设定,或者根据相机标定获得柱体的实际尺寸???
    int WD_mm = 200;//相机工作距离(焦点到柱面轴线实际距离(mm)),根据实际的成像系统设定,应该无法从图像信息中获取

    //为了便于计算，将半径、相机工作距离都转换为以像素为单位
    int radiusIn = 279 / 2;//柱体半径像素,可通过OpenCV直线检测测量/或通过PS等工具测像素
    int WD = WD_mm * radiusIn / radiusIn_mm;//焦点到柱面轴线像素

    qDebug() << "原图像宽：" << imgIn.cols << "  原图像高：" << imgIn.rows;//调试时显示原图像尺寸

    Mat imgOut = Mat::zeros(rowNum, colNum, CV_8UC1);
    qDebug() << "现图像宽：" << imgOut.cols << "  现图像高：" << imgOut.rows;//调试时显示结果图像尺寸

    Mat_<uchar> im1(imgIn);//定义Mat输入矩阵
    Mat_<uchar> im2(imgOut);//定义Mat输出矩阵

    //定义柱面复原后的坐标点
    int x1(0), y1(0);
    //双层循环,遍历原图像的每个像素点,坐标原点在原图像的左上角 cols为行,i表示纵坐标;rows为列,j表示横坐标
    for (int i = 0; i < imgIn.rows; i++)
        for (int j = 0; j < imgIn.cols; j++)
        {
            //以下的变换,是以原图像的中点坐标(0.5 * imgIn.cols, 0.5 * imgIn.rows)作为变换的中介
            double theta = asin( (j -0.5 * imgIn.cols) / radiusIn);//该像素点和圆点的连线 与光轴的夹角,范围: -PI/2 ~ PI/2
            //qDebug() << "i:" << i << "theta:" << theta << endl;

            x1 = radiusIn * theta + colNum / 2;//通过theta计算出柱面展开后的横坐标

            double z = qAbs( cos(theta) * radiusIn);//实际柱面上,该像素点的Z坐标
            y1 = (i - 0.5 * imgIn.rows) * ( WD - z ) / WD + 0.5 * imgIn.rows;//根据Z

            if (x1 >= 0 && x1 < colNum && y1 >= 0 && y1 < rowNum)
            {
                im2(y1, x1) = im1(i, j);
                //最邻近插值，双线性插值，双三次插值，兰索思插值等方法
            }
        }
    imshow("in", imgIn);
    imshow("out", imgOut);
}

MainWindow::~MainWindow()
{
    delete ui;
}
