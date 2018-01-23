package com.jyqqhw.jnidemo;

/**
 * Created by floyd1992 on 2018/1/21 0021.
 */

public class JNI {

    static {
        System.loadLibrary("Hello");
    }

    public native String sayHello();


    public native int add(int a, int b);

    public native String copy(String text);


}
