#include "run.h"
using namespace HalconCpp;
Algorithm Alg1;
Run::Run() {}

HObject MatToHObject(QList<cv::Mat>& mat_list) {
    if (mat_list.empty()) {
        throw std::runtime_error("Input image list is empty.");
    }

    HObject h_images;
    HTuple empty_tuple;


    GenEmptyObj(&h_images);

    for (const auto& mat : mat_list) {
        if (mat.empty()) {
            throw std::runtime_error("One of the input images is empty.");
        }

        cv::Mat image = mat.clone();


        if (image.channels() == 3) {
            cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        }


        HObject h_single_image;
        int width = image.cols;
        int height = image.rows;
        int channels = image.channels();

        if (channels == 1) {
            GenImage1(&h_single_image, "byte", width, height, (Hlong)image.data);
        } else if (channels == 3) {
            std::vector<cv::Mat> rgb_channels;
            cv::split(image, rgb_channels);
            Hlong r_ptr = (Hlong)rgb_channels[0].data;
            Hlong g_ptr = (Hlong)rgb_channels[1].data;
            Hlong b_ptr = (Hlong)rgb_channels[2].data;
            GenImage3(&h_single_image, "byte", width, height, r_ptr, g_ptr, b_ptr);
        } else {
            throw std::runtime_error("Unsupported number of channels.");
        }


        ConcatObj(h_images, h_single_image, &h_images);
    }

    return h_images;
}

cv::Mat HObjectToMat(const HObject& halcon_image) {
    HTuple channels, width, height;
    HTuple pointer, type;


    GetImageSize(halcon_image, &width, &height);
    CountChannels(halcon_image, &channels);
    GetImagePointer1(halcon_image, &pointer, &type, &width, &height);


    if (type != "byte") {
        throw std::runtime_error("Unsupported image type. Only 8-bit images are supported.");
    }


    cv::Mat cv_image;
    switch (channels.I()) {
    case 1:
    {
        cv_image = cv::Mat(height.I(), width.I(), CV_8UC1, (void*)pointer.L());
        cv_image = cv_image.clone();
        break;
    }
    case 3:
    {
        HTuple ptr_r, ptr_g, ptr_b;
        GetImagePointer3(halcon_image, &ptr_r, &ptr_g, &ptr_b, &type, &width, &height);

        std::vector<cv::Mat> cv_channels = {
            cv::Mat(height.I(), width.I(), CV_8UC1, (void*)ptr_r.L()),
            cv::Mat(height.I(), width.I(), CV_8UC1, (void*)ptr_g.L()),
            cv::Mat(height.I(), width.I(), CV_8UC1, (void*)ptr_b.L())
        };

        cv::merge(cv_channels, cv_image);
        cv::cvtColor(cv_image, cv_image, cv::COLOR_RGB2BGR);
        break;
    }
    }

    return cv_image;
}

void Run::run (QString ProcName,QList<cv::Mat> Images,QList <QString> &CtrlResults,QList <cv::Mat> &IconicResults)
{
    QFile file;
    QString filePath = "C:/AlgorithmData/algorithmparams.json";
    file.setFileName(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No saved parameter file found";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject rootObj = doc.object();
    QJsonArray paramsArray = rootObj["algorithmParams"].toArray();

    for (const QJsonValue &value : paramsArray) {
        QJsonObject obj = value.toObject();
        m_ProcName = obj["ProcedureName"].toString();
        if(ProcName == m_ProcName)
        {
            m_ProgramPath = obj["ProgramPath"].toString();
            m_ProcType = obj["Type"].toString();
            CtrlInputParams.clear();
            QJsonArray ctrlInputNames = obj["ctrlInputParams"].toArray();
            for (const QJsonValue &param : ctrlInputNames) {
                QJsonObject paramObj = param.toObject();
                CtrlInputParams.push_back(paramObj["value"].toString().toStdString());
            }
            if(m_ProcType=="Halcon")
            {
                HObject IconicInputParamsData = MatToHObject(Images);
                HTuple CtrlInputParamsData,CtrlOutputParamsData;
                HObject IconicOutputParamsData,SelectedObj;
                QString CtrlOutputParam;
                for (const auto& param : CtrlInputParams)
                {
                    CtrlInputParamsData.Append(HTuple(param.c_str()));
                }
                Alg1.ExcuteProcedure(m_ProgramPath,CtrlInputParamsData,IconicInputParamsData,CtrlOutputParamsData,IconicOutputParamsData,TypeList,IsRunSucess);
                for (int i = 0; i < CtrlOutputParamsData.Length(); i++)
                {
                    if(TypeList[i] == "string")
                    {
                        CtrlOutputParam = QString(CtrlOutputParamsData[i].S());
                    }
                    else if(TypeList[i] == "real")
                    {
                        CtrlOutputParam = QString::number(CtrlOutputParamsData[i].D());
                    }
                    CtrlResults.push_back(CtrlOutputParam);
                }
                for (int i = 0; i < IconicOutputParamsData.CountObj(); i++) {
                    SelectObj(IconicOutputParamsData,&SelectedObj,i+1);
                    IconicResults.push_back(HObjectToMat(SelectedObj));
                    cv::imwrite("D:/QtAlgorithm/Algorithm/Image"+to_string(i)+".jpg",HObjectToMat(SelectedObj));
                }
            }
            else if (m_ProcType=="Python")
            {
                QList <QString> CtrlOutputParamsData,CtrlInputParamsData;
                QList<cv::Mat> IconicOutputParamsData,IconicInputParamsData;
                for (const auto& param : CtrlInputParams)
                {
                    CtrlInputParamsData.push_back(param.c_str());
                }
                IconicInputParamsData = Images;
                Alg1.ExcuteProcedure(m_ProgramPath,CtrlInputParamsData,IconicInputParamsData,CtrlOutputParamsData,IconicOutputParamsData,TypeList,IsRunSucess);

                for (int i = 0; i < CtrlOutputParamsData.size(); i++)
                {
                    CtrlResults.push_back(CtrlOutputParamsData[i]);
                }

                for (int i = 0; i < IconicOutputParamsData.size(); i++) {
                    IconicResults.push_back(IconicOutputParamsData[i]);
                    cv::imwrite("D:/QtAlgorithm/Algorithm/Image"+to_string(i)+".jpg",IconicOutputParamsData[i]);
                }
            }
        }
    }
}

