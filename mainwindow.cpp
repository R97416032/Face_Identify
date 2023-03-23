#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
extern  VideoCapture capture;
Ptr<FaceRecognizer> model;
QString path = "/root/opencv-3.4.5/data/haarcascades_cuda/haarcascade_frontalface_alt.xml";
CascadeClassifier cascade;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!cascade.load(path.toStdString())){
        qDebug()<<"Haar级联分类器加载失败";
        return ;
    }
    if(!capture.isOpened()){
        qDebug()<<"打开摄像头失败";
    }
    time = new QTimer;
    time->setInterval(30);
    connect(time,SIGNAL(timeout()),this,SLOT(showVedio()));
    time->start();
    connect(&infowidget,SIGNAL(home()),this,SLOT(showhome()));
    QString path = QDir::currentPath();
    QFile facemodel(path + "/facemodel.xml");
    if(facemodel.exists()){
        model= Algorithm::load<EigenFaceRecognizer>("facemodel.xml");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ButtonSet_clicked()
{
    this->hide();
    infowidget.show();
}

void MainWindow::showhome(){
    infowidget.hide();
    this->show();
}


void MainWindow::showVedio()
{
    Mat image;
    capture >> image;
    CascadeClassifier faceClassifier;
    //训练好的文件名称，放置在可执行文件同目录下
    bool flag =faceClassifier.load("/root/opencv-3.4.5/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml");
    if(flag == false){
        QMessageBox::warning(this,"Haar级联分类器错误","不能加载haarcascade_frontalface_alt2.xml文件！！",QMessageBox::Ok);
        return;
    }
    Mat frame = image.clone();
    Mat gray;
    vector<Rect> faces;
    //RGB转BGR格式
    cvtColor(frame, gray, CV_BGR2GRAY);
    //变换后的图像进行直方图均值化处理
    equalizeHist(gray, gray);
    //通过人脸分类器识别有多少人脸
    faceClassifier.detectMultiScale(gray, faces, 1.8, 2, cv::CASCADE_FIND_BIGGEST_OBJECT|cv::CASCADE_DO_ROUGH_SEARCH, cv::Size(50, 50));

    Mat face;
    for (size_t i = 0; i < faces.size(); i++){
        if (faces[i].height > 0 && faces[i].width > 0)
        {
            face = gray(faces[i]);
            rectangle(frame, faces[i], cv::Scalar(0, 0, 255), 1, 8, 0);//函数是用来绘制一个矩形框的，通常用在图片的标记上
        }
    }
    QImage res = infowidget.Mat2Image(frame);
    ui->label->setPixmap(QPixmap::fromImage(res));
    QFile facemodel(QDir::currentPath() + "/facemodel.xml");
    bool ok = facemodel.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!ok)
        return;
    if(faces.size() == 0){
        ui->label_result->setText("识别结果：");
    }
    Mat face_test;
    if((face.rows > 112) || (face.cols > 92)){
        cv::resize(face, face_test, cv::Size(92,112));
    }
    double confidence=0.0;
    if (!face_test.empty())
    {
        flag = false;
        //测试图像应该是灰度图
        int predictedLabel=-1;
        //predictPCA = model->predict(face_test);
        /*
            第一个参数是测试样品，每行作为一个测试数据，即要将每一个样品处理成导入的模型同样维度大小的一行数据，
                    可同时放入多行数据，这时候就要用到第二个参数。

            当第二个参数为空时，函数会返回测试数据的标签值，假如放入一个Mat  暂名为result，则会保存在这个Mat中
        */
        model->predict(face_test,predictedLabel,confidence);
        QFile user(QDir::currentPath() + "/usr.txt");
        if(!user.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << user.errorString() <<endl;
        }
        qint64 usrlinenum =0;
        QString line;
        QTextStream in(&user);
        while((!in.atEnd()) && (usrlinenum < predictedLabel) )
        {
           line = in.readLine();
           usrlinenum++;
        }
        QStringList list = line.split(";");
        ui->label_result->setText(QString("识别结果：")+list[1]);
        user.close();
    }
    else{
        ui->label_result->setText("识别结果：查无此人");
    }
    facemodel.close();

}

void MainWindow::on_pushButton_3_clicked()
{
    QString path = QDir::currentPath();
    QFile facemodel(path + "/facemodel.xml");
    if(facemodel.exists()){
            facemodel.remove();
        }
    QFile csv(path + "/csv.txt");
    if(csv.exists()){
            csv.remove();
        }
    QFile usr(path + "/usr.txt");
    if(usr.exists()){
            usr.remove();
        }
    //删除存有的人脸数据
    QDir *p = new QDir;
    bool exist = p->exists(QDir::currentPath() + "/TEMP");
    if(exist){
        p->setPath(QDir::currentPath() + "/TEMP");
        p->removeRecursively();
    }
    int i=1;
    exist = p->exists(QDir::currentPath() +"/"+QString::number(i));
    while (exist) {
        p->setPath(QDir::currentPath()+"/"+QString::number(i));
        p->removeRecursively();
        i++;
        exist = p->exists(QDir::currentPath()+"/"+QString::number(i));
    }
    ui->label_result->setText("识别结果：");
}
