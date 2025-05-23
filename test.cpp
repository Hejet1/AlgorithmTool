#include "test.h"
#include "ui_test.h"
Run run;

test::test(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::test)
{
    ui->setupUi(this);
}

test::~test()
{
    delete ui;
}
QImage cvMatToQImage(const cv::Mat& mat) {
    switch (mat.type()) {
    // 处理 8UC3 类型（BGR 彩色图）
    case CV_8UC3: {
        QImage image(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step), QImage::Format_BGR888);
        return image.copy(); // 避免内存被释放后失效
    }
    // 处理 8UC1 类型（灰度图）
    case CV_8UC1: {
        QImage image(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    default:
        throw std::runtime_error("Unsupported Mat format");
    }
}
void test::on_Run_clicked()
{
    QList <cv::Mat> imgs,IconicResults;
    QList <QString> CtrlResults,ImgList;
    QString Imgs = ui->ImageTextEdit->toPlainText();
    ImgList =Imgs.split(",");
    for (QString Path:ImgList)
    {
        cv::Mat img = cv::imread(Path.toStdString());
        imgs.push_back(img);
    }
    QString ProcName = ui->ProcNameLineEdit->text();
    run.run(ProcName,imgs,CtrlResults,IconicResults);
    for (QString CResult:CtrlResults)
    {
        ui->ResultTextEdit->append(CResult);
    }

    for (cv::Mat IResult:IconicResults)
    {
        QImage image = cvMatToQImage(IResult);
        QPixmap pixmap = QPixmap::fromImage(image);
        QGraphicsScene* scene = new QGraphicsScene(this);
        ui->ImageGraphicsView->setScene(scene);
        scene->clear();
        scene->addPixmap(pixmap);
        scene->setSceneRect(pixmap.rect());

        // 自动调整视图缩放
        ui->ImageGraphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}



void test::on_LoadImage_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择文件",
        QDir::homePath(),
        "All Files (*.*)"
        );

    if (!files.isEmpty()) {
        // 用逗号分隔连接路径
        QString fileList = files.join(",");
        ui->ImageTextEdit->setText(fileList);
    }
}

