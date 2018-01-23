#include "cn_edu_zafu_opencv_OpenCVHelper.h"
#include "correct_pic.h"
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C" {

    JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_getPicAngle(JNIEnv *env, jclass obj, jstring str) {
        const char *pPath = env->GetStringUTFChars(str,false);
        float angelD = correctPic(pPath);
        LOGI("the rotation angel to be applied: angelD = %f" , angelD);
        env->ReleaseStringUTFChars(str, pPath);
        return angelD;
    }

    JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_getPicAngleByData(JNIEnv *env, jclass obj, jobject correctParam) {
        jclass correctParamCls = env->GetObjectClass(correctParam); //获得CorrectParam类引用

        jfieldID yuvDataFieldID = env->GetFieldID(correctParamCls,"yuvData","[B"); //获得CorrectParam类的属性yuvData
        jfieldID yuvDataLengthFieldID = env->GetFieldID(correctParamCls,"yuvDataLength","I"); //yuvDataLength
        jfieldID widthFieldID = env->GetFieldID(correctParamCls,"width","I"); //获得CorrectParam类的属性width
        jfieldID heightFieldID = env->GetFieldID(correctParamCls,"height","I"); //获得CorrectParam类的属性height

        int yuvDataLength = env->GetIntField(correctParam,yuvDataLengthFieldID); //yuvDataLength
        int width = env->GetIntField(correctParam,widthFieldID); //获得CorrectParam类的属性width
        int height = env->GetIntField(correctParam,heightFieldID); //获得CorrectParam类的属性height
        jbyteArray jYuvData = (jbyteArray)env->GetObjectField(correctParam,yuvDataFieldID); //获得CorrectParam类的属性yuvData
        jbyte* ba = env->GetByteArrayElements(jYuvData, JNI_FALSE);
        const unsigned char *yuvData = (unsigned char*)ba;

        float angelD = correctPic(yuvData,width,height);

        LOGI("the rotation angel to be applied: angelD = %f" , angelD);
        env->ReleaseByteArrayElements(jYuvData, ba, 0);
        return angelD;
    }

    JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_anglePic(JNIEnv *env, jclass obj, jstring jfromPath, jstring jtoPath, jint jangle){
        const char *fromPath = env->GetStringUTFChars(jfromPath,false);
        const char *toPath = env->GetStringUTFChars(jtoPath,false);
        const int angle = (int)jangle;

        anglePic(fromPath,toPath,angle);

        env->ReleaseStringUTFChars(jfromPath, fromPath);
        env->ReleaseStringUTFChars(jtoPath, toPath);
        return 0;
    }

    JNIEXPORT jobject JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_optimizePicture(JNIEnv *env, jclass obj,jstring jfromPath, jfloat jfactor){
        int* width = (int*)malloc(sizeof(int));
        int* height = (int*)malloc(sizeof(int));
        long* time = (long*)malloc(sizeof(long));
        const char *fromPath = env->GetStringUTFChars(jfromPath,false);
        const int factor = (int)jfactor;
        scalePic(fromPath, factor, width, height, time);
        LOGI("optimizePicture = %d  = %d ", *width, *height);

    jclass resultCls = env->FindClass("cn/edu/zafu/opencv/ResultParam"); //或得类引用

    //获得得该类型的构造函数  函数名为 <init> 返回类型必须为 void 即 V
    jmethodID constrocMID = env->GetMethodID(resultCls,"<init>","(IIJ)V");

    jobject result_obj = env->NewObject(resultCls,constrocMID,*width,*height, *time);  //构造一个对象，调用该类的构造函数，并且传递参数
        return result_obj;
    };


    JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_enhancePicture(JNIEnv *env, jclass jobj, jstring jfromPath, jint jtype){
        const char *fromPath = env->GetStringUTFChars(jfromPath,false);
        int type = (int)jtype;
        enhancePic(fromPath, type);
        return 0;
    };




}
