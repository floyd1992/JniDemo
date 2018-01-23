package cn.edu.zafu.opencv;

/**
 * User:lizhangqu(513163535@qq.com)
 * Date:2015-10-30
 * Time: 22:21
 */
public class OpenCVHelper {
    static {
        System.loadLibrary("OpenCV");
    }
    public static native int getPicAngle(String path);
    public static native int anglePic(String fromPath,String toPath,int angle);
    public static native int getPicAngleByData(Object correctParam);

    public static native ResultParam optimizePicture(String path, float factor);

    public static native int enhancePicture(String path, int type);

}
