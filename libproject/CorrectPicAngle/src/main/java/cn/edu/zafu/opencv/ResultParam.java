package cn.edu.zafu.opencv;

/**
 * Created by wj on 2018/1/22 0022.
 */

public class ResultParam {

    public int picWidth = 2;
    public int picHeight = 3;
    public float scaleRate;
    public long resizeTime;

    public ResultParam(){

    }

    public ResultParam(int width, int height, long time){
        this.picWidth = width;
        this.picHeight = height;
        this.resizeTime = time;
    }


}
