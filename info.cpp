#include "info.h"
#include "ui_info.h"
#include <QDebug>
extern Ptr<FaceRecognizer> model;
VideoCapture capture;
info::info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::info)
{
    ui->setupUi(this);
    capture.open(0);
    if(!capture.isOpened()){
        qDebug()<<"打开摄像头失败";
    }

    time = new QTimer;
    time->setInterval(30);
    connect(time,SIGNAL(timeout()),this,SLOT(showVedio()));
    time->start();
}
QImage info::Mat2Image(Mat image){
    cvtColor(image,image,CV_BGR2RGB);
    QImage src=QImage((const unsigned char *)(image.data),image.cols,image.rows,image.step,QImage::Format_RGB888);
    return src;
}
void info::showVedio(){
     Mat image;
     capture>> image;
     QImage res = Mat2Image(image);
     ui->label->setPixmap(QPixmap::fromImage(res));
}
info::~info()
{
    delete ui;
}

void info::on_ButtonBack_clicked()
{
    count=0;
    ui->lineEdit_name->setText("");
    emit home();
}

void info::on_ButtonTakePic_clicked(){
    Mat image;
    capture>> image;
    if(ui->lineEdit_name->text().isEmpty())
    {
        QMessageBox::warning(this,"信息录入","请输入姓名",QMessageBox::Ok);
    }
    else
    {
        if(image.empty())
        {
            QMessageBox::warning(this,"摄像头没有画面", "请检查摄像头是否正常", QMessageBox::Ok);
            return;
        }

        QDir *path = new QDir;

        bool exist = path->exists(QDir::currentPath() + "/TEMP");
        if(!exist)
            path->mkdir(QDir::currentPath() + "/TEMP");
        QString filename =  QDir::currentPath() + QString("/TEMP/%1.jpg").arg(count);
        ui->ButtonTakePic->setText(QString("拍照")+QString::number(count+1,10));
        Mat photo = image.clone();
        imwrite(filename.toStdString(), photo);
        count++;
    }
}
void info::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
       while( QTime::currentTime() < dieTime )
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void info::on_ButtonSample_clicked()
{
    QMessageBox *startSamplingBox = new QMessageBox(QMessageBox::Information,"信息","开始取样请稍后！！！",QMessageBox::NoButton,this);
    startSamplingBox->show();
    Sleep(100);
    /********向文件添加一行信息 源文件行数+姓名  usr.txt 统计系统录入的人脸类型个数信息********/
    QFile user(QDir::currentPath() + "/usr.txt");
    if(!user.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        startSamplingBox->close();
    }
    qint64 usrlinenum=1;
    while(!user.atEnd())
    {
        char buf[128];
        qint64 len = user.readLine(buf, sizeof(buf));

        if(len > 0)
            usrlinenum++;
        else if(len < 0)
        {
            startSamplingBox->close();
            QMessageBox::warning(this,tr("文件读取"),tr("文件读取失败！！！"));
            return;
        }
    }

    QString usrline = QString::number(usrlinenum)  + ";" +  ui->lineEdit_name->text() + "\r\n";
    QByteArray buf = usrline.toUtf8();

    char *str = buf.data();
    user.write(str);
    user.close();

    /*********创建编号文件夹(存放不同人的面部信息)*********/
    QDir *pathdata = new QDir;
    bool exist = pathdata->exists(QDir::currentPath() + "/" +QString::number(usrlinenum) );
    if(!exist)
    {
        bool ok =  pathdata->mkdir(QDir::currentPath() + "/" +QString::number(usrlinenum, 10));
        if(!ok)
        {
            startSamplingBox->close();
            QMessageBox::warning(this,tr("创建文件夹失败"),tr("文件夹创建失败！"));
            return;
        }
    }

    CascadeClassifier face_Classifier;   //定义人脸分类器
    string face_Classifier_name = "/root/opencv-3.4.5/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml";

    vector<Rect> face_rect;
    Mat dst_gray;
    if (!face_Classifier.load(face_Classifier_name))
    {
        startSamplingBox->close();
        QMessageBox::warning(this,"级联分类器错误","haarcascade_frontalface_alt.xml加载失败",QMessageBox::Ok);
        return;
    }

    QString pathdst = QDir::currentPath() + "/" +QString::number(usrlinenum) + "/";
    QString faceFile_path;   //./1/

    QDir dir(QDir::currentPath() + "/TEMP/");
    QStringList dst_file_name;
    dst_file_name << "*.jpg" ;
    //查找当前路径下所有的.jpg
    QFileInfoList fileList = dir.entryInfoList(dst_file_name, QDir::Files|QDir::Readable, QDir::Name);
    for(int i=0;i< fileList.size();i++)
    {
        QString photoPath = fileList[i].absoluteFilePath();//jpg绝对路径
        QString faceFile =pathdst+QString("%1.jpg").arg(i);

        Mat srcframe = imread(photoPath.toStdString());
        cvtColor(srcframe, dst_gray, cv::COLOR_BGR2GRAY);
        equalizeHist(dst_gray, dst_gray); //直方图均衡化
        //检测脸部
        face_Classifier.detectMultiScale(dst_gray,  face_rect, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, cv::Size(30, 30));
        for (int j = 0; j < (int)face_rect.size(); j++)
        {
            Mat faceROI = srcframe(face_rect[j]);
            Mat MyFace,gray_MyFace;
            if (faceROI.cols > 100)
            {
                cv::resize(faceROI, MyFace, cv::Size(92, 112));//92,112
                cv::cvtColor(MyFace, gray_MyFace, CV_BGR2GRAY);
                imwrite(faceFile.toStdString(), gray_MyFace);
            }
        }
    }
    /****************************创建csv文件******************************/
    QFile csv(QDir::currentPath() + "/csv.txt");
    if(!csv.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        startSamplingBox->close();
        QMessageBox::warning(this,"csv错误","无法创建csv文件！！！",QMessageBox::Ok);
        return;
    }

    QDir dst_file_dir(pathdst);
    QStringList dst_file_filter;
    dst_file_filter <<  "*.jpg" ;
    QFileInfoList dst_file_list_path = dst_file_dir.entryInfoList(dst_file_filter, QDir::Files | QDir::Readable, QDir::Name);

    for(int k=0; k<dst_file_list_path.size(); k++)
    {
        csv.write(QString(dst_file_list_path[k].absoluteFilePath() + ";" + QString::number(usrlinenum) + "\r\n").toLatin1());
    }
    startSamplingBox->close();
    QMessageBox::information(this,"取样结束","取样已经完成、csv文件完创建成！！！",QMessageBox::Ok);
    //清空缓存图片
    QDir *path = new QDir;
    path->setPath(QDir::currentPath() + "/TEMP");
    path->removeRecursively();

}

void info::on_ButtonTrain_clicked()
{
    QMessageBox *startTrainBox = new QMessageBox(QMessageBox::Information  ,"模型训练","开始训练请稍后！！！",QMessageBox::NoButton,this);
    startTrainBox->show();
    Sleep(1000);
    QFile csv(QDir::currentPath() + "/csv.txt");
    if(!csv.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        startTrainBox->close();
        QMessageBox::warning(this,"csv错误","csv文件未找到！！！",QMessageBox::Ok);
        return;
    }
    vector<Mat>images;
    vector<int>labels;
    while(!csv.atEnd())
    {
        QString p = csv.readLine(128);
        QStringList qlist = p.split(";");
        if(qlist.size() == 2)
        {
            QString path =qlist[0];
            int tag = qlist[1].mid(0, 1).toInt();

            images.push_back(cv::imread(path.toStdString(), 0));
            labels.push_back(tag);
        }
    }
    if(images.size() < 1)
    {
        QMessageBox::warning(this,"训练错误","加载图片失败！！！",QMessageBox::Ok);
        return;
    }

    model = EigenFaceRecognizer::create();
    /*
        第一个参数 :训练图像,用来学习的人脸图像。
        第二个参数 :人脸图像所对应的标签。
    */
    model->train(images, labels);
    model->save("facemodel.xml");
    startTrainBox->close();
    QMessageBox::information(this,"模型训练","恭喜模型训练完成！！！",QMessageBox::Ok);
}

void info::on_lineEdit_name_textChanged(const QString &arg1)
{
    ui->ButtonTakePic->setText(QString("拍照"));
}
