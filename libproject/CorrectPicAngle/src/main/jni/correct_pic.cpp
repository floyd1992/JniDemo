#include "correct_pic.h"
#include <android/log.h>
#include <time.h>

#define GRAY_THRESH 150
#define HOUGH_VOTE 80

//#define DEGREE 27

//#define SAVE_RESULT 27

float correctPic(const char *fromFile){
	Mat srcImg = imread(fromFile, CV_LOAD_IMAGE_GRAYSCALE);
	LOGI("the correctPic type = %d" , srcImg.type());
	if(srcImg.empty())
		return -1;
	return correctPic(srcImg);
}

float correctPic(const unsigned char * cbuf, const int width, const int height){
	Mat srcImg(height, width, 0, (unsigned char*) cbuf);
	if(srcImg.empty())
		return -1;
	return correctPic(srcImg);
}

float correctPic(Mat srcImg){
	if(srcImg.empty())
		return -1;
//	imshow("source", srcImg);
    LOGI("tgetPicAngleByData 5555555555 cols = %d  rows = %d ",srcImg.cols,srcImg.rows);
	Point center(srcImg.cols/2, srcImg.rows/2);

#ifdef DEGREE
	//Rotate source image
	Mat rotMatS = getRotationMatrix2D(center, DEGREE, 1.0);
	warpAffine(srcImg, srcImg, rotMatS, srcImg.size(), 1, 0, Scalar(255,255,255));
//	imshow("RotatedSrc", srcImg);
	//imwrite("imageText_R.jpg",srcImg);
#endif

	//Expand image to an optimal size, for faster processing speed
	//Set widths of borders in four directions
	//If borderType==BORDER_CONSTANT, fill the borders with (0,0,0)
	Mat padded;
	int opWidth = getOptimalDFTSize(srcImg.rows);
	int opHeight = getOptimalDFTSize(srcImg.cols);
	copyMakeBorder(srcImg, padded, 0, opWidth-srcImg.rows, 0, opHeight-srcImg.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat comImg;
	//Merge into a double-channel image
	merge(planes,2,comImg);

	//Use the same image as input and output,
	//so that the results can fit in Mat well
	dft(comImg, comImg);

	//Compute the magnitude
	//planes[0]=Re(DFT(I)), planes[1]=Im(DFT(I))
	//magnitude=sqrt(Re^2+Im^2)
	split(comImg, planes);
	magnitude(planes[0], planes[1], planes[0]);

	//Switch to logarithmic scale, for better visual results
	//M2=log(1+M1)
	Mat magMat = planes[0];
	magMat += Scalar::all(1);
	log(magMat, magMat);

	//Crop the spectrum
	//Width and height of magMat should be even, so that they can be divided by 2
	//-2 is 11111110 in binary system, operator & make sure width and height are always even
	magMat = magMat(Rect(0, 0, magMat.cols & -2, magMat.rows & -2));

	//Rearrange the quadrants of Fourier image,
	//so that the origin is at the center of image,
	//and move the high frequency to the corners
	int cx = magMat.cols/2;
	int cy = magMat.rows/2;

	Mat q0(magMat, Rect(0, 0, cx, cy));
	Mat q1(magMat, Rect(0, cy, cx, cy));
	Mat q2(magMat, Rect(cx, cy, cx, cy));
	Mat q3(magMat, Rect(cx, 0, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q2.copyTo(q0);
	tmp.copyTo(q2);

	q1.copyTo(tmp);
	q3.copyTo(q1);
	tmp.copyTo(q3);

	//Normalize the magnitude to [0,1], then to[0,255]
	normalize(magMat, magMat, 0, 1, CV_MINMAX);
	Mat magImg(magMat.size(), CV_8UC1);
	magMat.convertTo(magImg,CV_8UC1,255,0);
//	imshow("magnitude", magImg);
	//imwrite("imageText_mag.jpg",magImg);

	//Turn into binary image
	threshold(magImg,magImg,GRAY_THRESH,255,CV_THRESH_BINARY);
//	imshow("mag_binary", magImg);
	//imwrite("imageText_bin.jpg",magImg);

	//Find lines with Hough Transformation
	vector<Vec2f> lines;
	float pi180 = (float)CV_PI/180;
	Mat linImg(magImg.size(),CV_8UC3);
	HoughLines(magImg,lines,1,pi180,HOUGH_VOTE,0,0);
	int numLines = lines.size();
	for(int l=0; l<numLines; l++)
	{
		float rho = lines[l][0], theta = lines[l][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line(linImg,pt1,pt2,Scalar(255,0,0),3,8,0);
	}
//	imshow("lines",linImg);
	//imwrite("imageText_line.jpg",linImg);
	if(lines.size() == 3){
		cout << "found three angels:" << endl;
		cout << lines[0][1]*180/CV_PI << endl << lines[1][1]*180/CV_PI << endl << lines[2][1]*180/CV_PI << endl << endl;
	}

	//Find the proper angel from the three found angels
	float angel=0;
	float piThresh = (float)CV_PI/90;
	float pi2 = CV_PI/2;
	for(int l=0; l<numLines; l++)
	{
		float theta = lines[l][1];
		if(abs(theta) < piThresh || abs(theta-pi2) < piThresh)
			continue;
		else{
			angel = theta;
			break;
		}
	}

	//Calculate the rotation angel
	//The image has to be square,
	//so that the rotation angel can be calculate right
	angel = angel<pi2 ? angel : angel-CV_PI;
	if(angel != pi2){
		float angelT = srcImg.rows*tan(angel)/srcImg.cols;
		angel = atan(angelT);
	}
	float angelD = angel*180/(float)CV_PI;
	cout << "the rotation angel to be applied:" << endl << angelD << endl << endl;
    LOGI("the rotation angel to be applied: angelD = %f" , angelD);
//  Rotate the image to recover
//	Mat rotMat = getRotationMatrix2D(center,angelD,1.0);
//	Mat dstImg = Mat::ones(srcImg.size(),CV_8UC3);
//	warpAffine(srcImg,dstImg,rotMat,srcImg.size(),1,0,Scalar(255,255,255));
//	imwrite("/storage/emulated/0/config/OneSearchDark/correctword/test_result.png",dstImg);

	srcImg.release();
	padded.release();
	comImg.release();
	magMat.release();
	linImg.release();
	srcImg.release();
//	rotMat.release();
//    dstImg.release();
	return angelD;
}

void anglePic(const char *fromFile,const char *toFile,int angelD){
    Mat srcImg = imread(fromFile, CV_LOAD_IMAGE_GRAYSCALE);
    Point center(srcImg.cols/2, srcImg.rows/2);
    Mat rotMat = getRotationMatrix2D(center,angelD,1.0);
    Mat dstImg = Mat::ones(srcImg.size(),CV_8UC3);
    warpAffine(srcImg,dstImg,rotMat,srcImg.size(),1,0,Scalar(255,255,255));
    imwrite(toFile,dstImg);
    srcImg.release();
    rotMat.release();
    dstImg.release();
}

double getSystemTime(){
        struct timeval tv;
        gettimeofday(&tv, NULL);
        double t = tv.tv_sec;
        t *= 1000;
        t += tv.tv_usec/1000;
        return (double)t;
}


int scalePic(const char *fromFile, int factor, int* width, int* height, long* time){
        LOGI("scalePic: 1111  = %f" , 0.0);
        LOGI("scalePic: 1111  = %d ===  %d" , *width, *height);
        Mat imagesrc = imread(fromFile, -1);//读取图像
        if (!imagesrc.data)
        {
            cout<<"no image can be load!"<<endl;
            return 0;
        }
        LOGI("scalePic: 2222  = %f" ,0.0);
        double fScale = factor;//缩放系数
        double realWidth = imagesrc.cols*fScale;
        double realHeight = imagesrc.rows*fScale;

        LOGI("scalePic:0000  = %f =====%f" , realWidth, realHeight);
        if(realWidth>=4000){
            realWidth = 4000;
        }
        if(realHeight>=4000){
            realHeight = 4000;
        }

        *width = realWidth;
        *height = realHeight;
        //计算目标图像的大小
        Size dsize = Size(realWidth, realHeight);
        Mat imagedst = Mat(dsize, CV_8UC3 );
        LOGI("scalePic:1111  = %f ====%f" , imagesrc.cols, imagesrc.rows);
        double start = getSystemTime();
        resize(imagesrc, imagedst, dsize, 0, 0);
        double end = getSystemTime();
        *time = (double)(end - start);
        LOGI("scalePic:2222  = %f =====%f ===&d === %d" , realWidth, realHeight, start, *time);
        imwrite(fromFile,imagedst);
        LOGI("scalePic: end  = %f" , imagesrc.rows*fScale);
        imagesrc.release();
        imagedst.release();
        return 0;
}


int enhancePic(const char *fromFile, int type){
        LOGI("enhancePic: 0000  = %f" , 0.0);
        Mat imagesrc = imread(fromFile, -1);//读取图像
        if (!imagesrc.data)
        {
            cout<<"no image can be load!"<<endl;
            return 0;
        }
        LOGI("enhancePic: 1111  = %f" ,0.0);
        Mat imageEnhance;
        if(type ==1){

        }else if(type==2){
            Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
            filter2D(imagesrc, imageEnhance, CV_8UC3, kernel);
            kernel.release();
            LOGI("enhancePic:2222  ============");
                    imwrite(fromFile,imageEnhance);
                    LOGI("enhancePic: end  ");
                    imagesrc.release();
                    imageEnhance.release();
        }else if(type==3){
            Mat imageEnhance(imagesrc.size(), CV_32FC3);
                for (int i = 0; i < imagesrc.rows; i++)
                {
                    for (int j = 0; j < imagesrc.cols; j++)
                    {
                        imageEnhance.at<Vec3f>(i, j)[0] = log(1 + imagesrc.at<Vec3b>(i, j)[0]);
                        imageEnhance.at<Vec3f>(i, j)[1] = log(1 + imagesrc.at<Vec3b>(i, j)[1]);
                        imageEnhance.at<Vec3f>(i, j)[2] = log(1 + imagesrc.at<Vec3b>(i, j)[2]);
                    }
                }
                //归一化到0~255
                normalize(imageEnhance, imageEnhance, 0, 255, CV_MINMAX);
                //转换成8bit图像显示
                convertScaleAbs(imageEnhance, imageEnhance);
                LOGI("enhancePic:2222  ============");
                        imwrite(fromFile,imageEnhance);
                        LOGI("enhancePic: end  ");
                        imagesrc.release();
                        imageEnhance.release();
        }else if(type==4){
            Mat imageEnhance(imagesrc.size(), CV_32FC3);
                for (int i = 0; i < imagesrc.rows; i++)
                {
                    for (int j = 0; j < imagesrc.cols; j++)
                    {
                        imageEnhance.at<Vec3f>(i, j)[0] = (imagesrc.at<Vec3b>(i, j)[0])*(imagesrc.at<Vec3b>(i, j)[0])*(imagesrc.at<Vec3b>(i, j)[0]);
                        imageEnhance.at<Vec3f>(i, j)[1] = (imagesrc.at<Vec3b>(i, j)[1])*(imagesrc.at<Vec3b>(i, j)[1])*(imagesrc.at<Vec3b>(i, j)[1]);
                        imageEnhance.at<Vec3f>(i, j)[2] = (imagesrc.at<Vec3b>(i, j)[2])*(imagesrc.at<Vec3b>(i, j)[2])*(imagesrc.at<Vec3b>(i, j)[2]);
                    }
                }
                //归一化到0~255
                normalize(imageEnhance, imageEnhance, 0, 255, CV_MINMAX);
                //转换成8bit图像显示
                convertScaleAbs(imageEnhance, imageEnhance);
                LOGI("enhancePic:2222  ============");
                        imwrite(fromFile,imageEnhance);
                        LOGI("enhancePic: end  ");
                        imagesrc.release();
                        imageEnhance.release();
        }
        return 0;
}


