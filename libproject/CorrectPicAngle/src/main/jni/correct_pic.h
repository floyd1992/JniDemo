#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define  LOG_TAG    "lijun"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace cv;
using namespace std;

float correctPic(const char *fromFile);//图片路径
float correctPic(const unsigned char * cbuf, int width, int height);//图片byte 宽度 高度
float correctPic(Mat srcImg);
void anglePic(const char *fromFile,const char *toFile,const int angelD);
int scalePic(const char *fromFile, int factor, int* width, int* height, long* time);
int enhancePic(const char *fromFile, const int type);