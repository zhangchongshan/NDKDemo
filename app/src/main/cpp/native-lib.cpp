#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include <android/NeuralNetworks.h>

#define LOG_TAG "ndk log"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

typedef struct tick_context{
    JavaVM *javaVm;
    jclass jniHelperClz;
    jobject jniHelperObj;
    jclass  mainActivityClz;
    jobject mainActivityObj;
    pthread_mutex_t lock;
    int done;
}Tick_Context;
Tick_Context g_ctx;


char* jstr2Char(JNIEnv *env,jstring src){
    char * rtn=NULL;
    jclass stringClz=env->FindClass("java/lang/String");
    jstring strCode=env->NewStringUTF("GB2312");
    jmethodID getBytes=env->GetMethodID(stringClz,"getBytes", "(Ljava/lang/String;)[B");
    jbyteArray buff=(jbyteArray)env->CallObjectMethod(src,getBytes,strCode);
    jsize len=env->GetArrayLength(buff);
    jbyte * ba=env->GetByteArrayElements(buff,JNI_FALSE);
    if (len>0){
        rtn=(char *)malloc(len+1);
        memcpy(rtn,ba,len);
        rtn[len]=0;
    }
    env->ReleaseByteArrayElements(buff,ba,0);
    return rtn;
}

jstring char2JStirng(JNIEnv* env,char * data){
    jclass stringClz=env->FindClass("java/lang/String");
    jmethodID strInit=env->GetMethodID(stringClz,"<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes=env->NewByteArray(strlen(data));
    env->SetByteArrayRegion(bytes,0,strlen(data),(jbyte*)data);
    jstring encoding=env->NewStringUTF("GB2312");
    return (jstring)env->NewObject(stringClz,strInit,bytes,encoding);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sonoptek_ndkdemo_MainActivity_initJni(JNIEnv *env, jobject thiz) {
    // TODO: implement init()
    jclass mainClz=env->GetObjectClass(thiz);
    g_ctx.mainActivityClz=(jclass)env->NewGlobalRef(mainClz);
    g_ctx.mainActivityObj=env->NewGlobalRef(thiz);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm,void* reserved){
    JNIEnv *env ;
    memset(&g_ctx,0,sizeof(g_ctx));
    g_ctx.javaVm=vm;
    if (vm->GetEnv((void**)&env,JNI_VERSION_1_6)!=JNI_OK){
        return JNI_ERR;
    }
    LOGE("jni start");
    jclass helpClz=env->FindClass("com/sonoptek/ndkdemo/NDKUtil");
    g_ctx.jniHelperClz=(jclass)env->NewGlobalRef(helpClz);
    jmethodID helpInit=env->GetMethodID(g_ctx.jniHelperClz,"<init>","()V");
    jobject helpObj=env->NewObject(g_ctx.jniHelperClz,helpInit);
    g_ctx.jniHelperObj=env->NewGlobalRef(helpObj);
    g_ctx.done=0;
    return JNI_VERSION_1_6;

}

pthread_t pthread;
void* threadCallBack(void * data){
    LOGE("thread data: %d",100);
    pthread_exit(NULL);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sonoptek_ndkdemo_NDKUtil_ndkThread(JNIEnv *env, jclass clazz) {
    // TODO: implement ndkThread()
    pthread_create(&pthread,NULL,threadCallBack,(void *)1);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_sonoptek_ndkdemo_NDKUtil_ndkCallJavaMethod(JNIEnv *env, jclass clazz) {
    // TODO: implement ndkCallJavaMethod()
    jmethodID javaMethod=env->GetMethodID(g_ctx.jniHelperClz,"getAndroidVersion","()Ljava/lang/String;");
    if (javaMethod!=NULL){
        jstring version=(jstring)env->CallObjectMethod(g_ctx.jniHelperObj,javaMethod);
        char * str=jstr2Char(env,version);
        LOGE("call java forget android version= %s",str);
    }
}

