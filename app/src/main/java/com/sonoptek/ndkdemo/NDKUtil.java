package com.sonoptek.ndkdemo;

import android.os.Build;

/**
 * Created by zhangchongshan on 2020/11/26.
 */
public class NDKUtil {

    public String getAndroidVersion(){
        return Build.VERSION.RELEASE;
    }

    public static native void ndkThread();

    public static native void ndkCallJavaMethod();

    public static native void startClient(String serverIP,int serverPort);

}
