package com.jyqqhw.jnidemo;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    TextView tv = (TextView) findViewById(R.id.sample_text);
    TextView tv1 = (TextView) findViewById(R.id.sample_text1);
    TextView tv2 = (TextView) findViewById(R.id.sample_text2);
        JNI jni = new JNI();
    tv.setText(jni.sayHello());
    tv1.setText(jni.add(1,1)+"");
    tv2.setText(jni.copy("i am from java "));
//    tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native String stringFromJNI();

    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("native-lib");
//    }
}
