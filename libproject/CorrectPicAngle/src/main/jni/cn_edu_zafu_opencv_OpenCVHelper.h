/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "correct_pic.h"
#include <android/log.h>

#define  LOG_TAG    "lijun"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/* Header for class cn_edu_zafu_opencv_OpenCVHelper */

#ifndef _Included_cn_edu_zafu_opencv_OpenCVHelper
#define _Included_cn_edu_zafu_opencv_OpenCVHelper
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     cn_edu_zafu_opencv_OpenCVHelper
 * Method:    gray
 * Signature: ([III)[I
 */
JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_getPicAngle(JNIEnv *, jclass, jstring);

JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_getPicAngleByData(JNIEnv *env, jclass obj, jobject correctParam);

JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_anglePic(JNIEnv *env, jclass obj, jstring, jstring, jint);

/*
 * Class:     cn_edu_zafu_opencv_OpenCVHelper
 * Method:    optimizePicture
 * Signature: (Ljava/lang/String;F)Lcn/edu/zafu/opencv/ResultParam;
 */
JNIEXPORT jobject JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_optimizePicture(JNIEnv *, jclass, jstring, jfloat);


/*
 * Class:     cn_edu_zafu_opencv_OpenCVHelper
 * Method:    optimizePicture
 * Signature: (Ljava/lang/String;F)Lcn/edu/zafu/opencv/ResultParam;
 */
JNIEXPORT jint JNICALL Java_cn_edu_zafu_opencv_OpenCVHelper_enhancePicture(JNIEnv *, jclass, jstring, jint);

#ifdef __cplusplus
}
#endif
#endif