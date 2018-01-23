package com.jyqqhw.jnidemo;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import cn.edu.zafu.opencv.OpenCVHelper;
import cn.edu.zafu.opencv.ResultParam;

public class TestActivity extends Activity {

    private static final String PATH = Environment.getExternalStorageDirectory().toString() + "/帅气111.bmp";
    private static final String TAG = "MainActivity";

    //1. 基于直方图均衡化的图像增强
    public static int ENHANCE_1 = 1;
    //2. 基于拉普拉斯算子的图像增强
    public static int ENHANCE_2 = 2;
    //3. 基于对数Log变换的图像增强
    public static int ENHANCE_3 = 3;
    //4. 基于伽马变换的图像增强
    public static int ENHANCE_4 = 4;

    private int type = ENHANCE_2;


    private ImageView imageView;
    private Button scaleBtn;
    private Button selectBtn;
    private Button enhanceBtn;
    private TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);
        init();
        int angle = OpenCVHelper.getPicAngle(PATH);
        Log.d(TAG, "current angle is " + angle);
    }

    private void init(){
        imageView = (ImageView) findViewById(R.id.image_view);
        selectBtn = (Button) findViewById(R.id.select);
        selectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                startActivityForResult(intent, 100);
            }
        });
        textView = (TextView) findViewById(R.id.text);
        enhanceBtn = (Button) findViewById(R.id.enhance);
        enhanceBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(type==ENHANCE_2){
                    OpenCVHelper.enhancePicture(PATH, ENHANCE_2);
                    type = 3;
                    enhanceBtn.setText("增强:拉普拉斯");
                }else if(type==ENHANCE_3){
                    OpenCVHelper.enhancePicture(PATH, ENHANCE_3);
                    type = 4;
                    enhanceBtn.setText("增强:对数变换");
                }else if(type==ENHANCE_4){
                    OpenCVHelper.enhancePicture(PATH, ENHANCE_4);
                    type = 2;
                    enhanceBtn.setText("增强:伽马变换");
                }
                Toast.makeText(TestActivity.this, "enhance picture ok!  type = "+type, Toast.LENGTH_SHORT).show();
                showImage();
            }
        });
        scaleBtn = (Button) findViewById(R.id.scale);
        scaleBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                long start = System.currentTimeMillis();
                imageView.setDrawingCacheEnabled(true);
                Bitmap bitmap = imageView.getDrawingCache();
                Log.d(TAG, "get drawingCache time = "+(System.currentTimeMillis()-start));
                imageView.setDrawingCacheEnabled(false);
                Log.d(TAG, "after save bitmap time = "+(System.currentTimeMillis()-start));
                ResultParam resultParam =  OpenCVHelper.optimizePicture(PATH, scaleRate);
                textView.setText("当前图片分辨率: "+resultParam.picHeight+" x "+resultParam.picWidth
                        +", 放大倍率为 2, 插值时间为: "+resultParam.resizeTime);
                Log.d(TAG, "final optimize bitmap time = "+(System.currentTimeMillis()-start));
                showImage();
            }
        });
        imageView.setImageResource(R.drawable.flower);
    }
    private float scaleRate = 2;

    private void showImage(){
        Bitmap bitmap = BitmapFactory.decodeFile(PATH);
        imageView.setImageBitmap(bitmap);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch(requestCode) {
            case 100:
                if(resultCode == RESULT_OK){
                    Uri selectedImage = data.getData();
                    String[] filePathColumn = {MediaStore.Images.Media.DATA};
                    Cursor cursor = getContentResolver().query(
                            selectedImage, filePathColumn, null, null, null);
                    cursor.moveToFirst();
                    int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
                    String filePath = cursor.getString(columnIndex);
                    cursor.close();
                    Bitmap yourSelectedImage;
                    try {
                        yourSelectedImage = BitmapFactory.decodeFile(filePath);
                        FileOutputStream fos = new FileOutputStream(PATH);
                        yourSelectedImage.compress(Bitmap.CompressFormat.JPEG, 100, fos);
                        fos.flush();
                        fos.close();
                        Bitmap bitmap = BitmapFactory.decodeFile(PATH);
                        textView.setText("当前图片分辨率: "+bitmap.getHeight()+" x "+bitmap.getWidth()+", 放大倍率为 2");
                        imageView.setImageBitmap(bitmap);
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                break;
            default:
                    break;
        }
    }
}
