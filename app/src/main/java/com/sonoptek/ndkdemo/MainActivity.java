package com.sonoptek.ndkdemo;

import androidx.appcompat.app.AppCompatActivity;

import org.androidannotations.annotations.AfterViews;
import org.androidannotations.annotations.Click;
import org.androidannotations.annotations.EActivity;

@EActivity(R.layout.activity_main)
public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @AfterViews
    void init(){
        initJni();
    }

    @Click({R.id.btn_ndk_thread})
    void ndkThread(){
        NDKUtil.ndkThread();
    }

    @Click({R.id.btn_ndk_method_call})
    void ndkCalJavaMethod(){
        NDKUtil.ndkCallJavaMethod();
    }

    @Click({R.id.btn_ndk_socket})
    void ndkSocket(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                NDKUtil.startClient("190.166.3.1",52001);
            }
        }).start();
    }


    public native void initJni();
}