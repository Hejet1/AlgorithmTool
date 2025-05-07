import cv2 as cv
import numpy as np

#参数结构体
CtrlInputParamDict = {
    "Input1": {
        "Value": "1",
        "Type": "string",
    },
    "Input2": {
        "Value": "2",
        "Type": "string",
    },
    "Input3": {
        "Value": "3",
        "Type": "string",
    },
}
CtrlOutputParamDict = {
    "IsOK": {
        "Value": 0.0,
        "Type": "real",
    },
    "Confidence": {
        "Value": 0.0,
        "Type": "real",
    },
    "Area": {
        "Value": 100,
        "Type": "real",
    },
    "Height": {
        "Value": 300,
        "Type": "real",
    },
    "Width": {
        "Value": 300,
        "Type": "real",
    }
}
IconicInputParamDict = {
    "Image1": {
        "Value": "",
        "Type": "img",
    },
    "Image2": {
        "Value": "",
        "Type": "img",
    }
}
IconicOutputParamDict = {
    "ImageOut": {
        "Value": "",
        "Type": "img",
    }
}

ImagePath1 = r"F:\HD2423\1.bmp"
ImagePath2 = r"F:\HD2423\2.bmp"
Image1 = cv.imread(ImagePath1)
Image2 = cv.imread(ImagePath2)


def main(Input1, Input2, Input3, Image1, Image2):
    # 解析图像元数据
    (rows, cols, dtype, size), img1_bytes = Image1
    (rows2, cols2, dtype2, size2), img2_bytes = Image2
    # 解码图像
    Image1 = cv.imdecode(np.frombuffer(img1_bytes, np.uint8), cv.IMREAD_COLOR)
    Image2 = cv.imdecode(np.frombuffer(img2_bytes, np.uint8), cv.IMREAD_COLOR)

    img = np.concatenate((Image1, Image2), axis=1)
    Text = Input1 + Input2 + Input3
    cv.putText(img, Text, (50, 200), cv.FONT_HERSHEY_SIMPLEX, 5, (0, 0, 255), 5)
    cv.imwrite(r"F:\HD2423\1_predict.bmp", img)
    CtrlOutputParamDict["IsOK"]["Value"] = 1
    CtrlOutputParamDict["Confidence"]["Value"] = 1
    IconicOutputParamDict["ImageOut"]["Value"] = img
    result = {
        "CtrlOutputParams": CtrlOutputParamDict,
        "IconicOutputParams": IconicOutputParamDict
        }
    return result


if __name__ == "__main__":
    result = main(Input1="1", Input2="2", Input3="3", Image1=Image1, Image2=Image2)
    print(result)
