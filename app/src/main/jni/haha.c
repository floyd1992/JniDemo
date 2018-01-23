//
// Created by admin on 2018/1/21 0021.
//
#include "com_jyqqhw_jnidemo_JNI.h"
#include <string.h>


jstring Java_com_jyqqhw_jnidemo_JNI_sayHello(JNIEnv* env, jobject obj){
    char* text = "来自 c 的问候";
    return (*env)->NewStringUTF(env, text);

}

jint Java_com_jyqqhw_jnidemo_JNI_add(JNIEnv* env, jobject jobj, jint a, jint b){
  int result = a+b;
  return result;
}

JNIEXPORT jstring JNICALL Java_com_jyqqhw_jnidemo_JNI_copy
        (JNIEnv * env, jobject jobj, jstring jstr){

    const char* js = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
    char* toJava = "我是c里的一个字符串  ";
    strcat(js, toJava);
    return (*env)->NewStringUTF(env, js);
};