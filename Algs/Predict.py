from ultralytics import YOLO
from TEST import encode_image
import os
import cv2
import numpy as np
#参数结构体
CtrlInputParamDict = {

}
CtrlOutputParamDict = {
    "IsOK": {
        "Value": 0.0,
        "Type": "real",
    },
    "Confidence": {
        "Value": 0.0,
        "Type": "real",
    }
}
IconicInputParamDict = {
    "Image1": {
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

def main(Image1):
    Image1 = encode_image(Image1)
    model = YOLO(r"F:\Python\YOLO\best.pt")
    results = model(Image1)
    result = results[0].probs.top1
    conf = results[0].probs.top1conf
    if result == 0:
        img = Image1
        text = f"NG:{conf:.3f}"
        org = (30, 30)
        fontFace = cv2.FONT_HERSHEY_SIMPLEX
        fontScale = 1
        color = (0, 0, 255)
        thickness = 2
        cv2.putText(img, text, org, fontFace, fontScale, color, thickness)
        CtrlOutputParamDict["IsOK"]["Value"] = 0
        CtrlOutputParamDict["Confidence"]["Value"] = conf
        IconicOutputParamDict["ImageOut"]["Value"] = img
    elif result == 1:
        img = Image1
        text = f"OK:{conf:.3f}"
        org = (30, 30)
        fontFace = cv2.FONT_HERSHEY_SIMPLEX
        fontScale = 1
        color = (0, 255, 0)
        thickness = 2
        cv2.putText(img, text, org, fontFace, fontScale, color, thickness)
        CtrlOutputParamDict["IsOK"]["Value"] = 1
        CtrlOutputParamDict["Confidence"]["Value"] = conf
        IconicOutputParamDict["ImageOut"]["Value"] = img
    result = {
            "CtrlOutputParams": CtrlOutputParamDict,
            "IconicOutputParams": IconicOutputParamDict
    }
    return result

