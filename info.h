#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include "/root/opencv-3.4.5/opencv_contrib-3.4.5/modules/face/include/opencv2/face.hpp"
#include "/root/opencv-3.4.5/opencv_contrib-3.4.5/modules/face/include/opencv2/face/facerec.hpp"
#include "/root/opencv-3.4.5/opencv_contrib-3.4.5/modules/face/include/opencv2/face.hpp"
#include </usr/local/include/opencv2/face.hpp>
#include "/usr/local/include/opencv2/face/facerec.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core.hpp"
#include "/root/opencv-3.4.5/opencv_contrib-3.4.5/modules/face/include/opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "opencv2/imgproc/types_c.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <QImage>
#include <QMessageBox>
#include <QDir>
#include <QTime>
#include <QTimer>

using namespace cv;
using namespace std;
using namespace cv::face;

namespace Ui {
class info;
}

class info : public QWidget
{
    Q_OBJECT

public:
    explicit info(QWidget *parent = 0);
    QTimer *time;
    int count = 0;
    QImage Mat2Image(Mat image);
    void Sleep(int msec);
    ~info();
signals:
    void home();

private slots:
    void on_ButtonBack_clicked();
    void showVedio();

    void on_ButtonTakePic_clicked();

    void on_ButtonSample_clicked();

    void on_ButtonTrain_clicked();

    void on_lineEdit_name_textChanged(const QString &arg1);

private:
    Ui::info *ui;
};

#endif // INFO_H
