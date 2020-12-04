//
// Created by T490 on 2020/11/26.
//

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>

//第一步：导入Socket编程的标准库
//这个标准库：linux数据类型(size_t、time_t等等......)
#include <sys/types.h>
//提供Socket函数及数据结构
#include <sys/socket.h>
//数据结构(socket_in)
#include <netinet/in.h>
//ip地址的转换函数
#include <arpa/inet.h>

#define LOG_TAG "socket_client"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
extern "C"
JNIEXPORT void JNICALL
Java_com_sonoptek_ndkdemo_NDKUtil_startClient(JNIEnv *env, jclass clazz, jstring java_server_ip,
jint server_port) {

    const char * server_ip=env->GetStringUTFChars(java_server_ip,NULL);

    //客户端socket
    int client_socket_fd;

    //服务端网络地址
    struct sockaddr_in server_addr;


    //初始化网络地址
    //参数1：传变量的地址（&server_addr）
    //参数2：开始位置
    //参数3：大小
    //初始化服务的网络地址
    memset(&server_addr,0,sizeof(server_addr));

    //AF_INET:TCP/IP协议、UDP
    //AF_ISO:ISO 协议
    server_addr.sin_family=AF_INET;

    //设置服务端IP地址(自动获取系统默认的本机IP，自动分配)
    server_addr.sin_addr.s_addr=inet_addr(server_ip);

    //设置服务端端口
    server_addr.sin_port=htons(server_port);

    //创建客户端
    client_socket_fd=socket(PF_INET,SOCK_STREAM,0);

    //判断是否创建成功
    if (client_socket_fd<0){
        LOGE("create error");
        return;
    }


    //连接服务器
    //参数一：哪一个客户端
    //参数二：连接服务器地址
    //参数三：地址大小
    int con_result=connect(client_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if (con_result<0){
        LOGE("connect error");
        return;
    }


    //发送消息(向服务器发送内容)
    char buffer[BUFSIZ]="hello Socket Server";

    //参数一：指定客户端
    //参数二：指定缓冲区(冲那里数据读取)
    //参数三：实际读取的大小strlen(buffer)(其实读取到"\0"结束)
    //参数四：从哪里开始读取
    send(client_socket_fd,buffer,strlen(buffer),0);


    //关闭
    shutdown(client_socket_fd,SHUT_RDWR);

    LOGE("client end-------------");
    env->ReleaseStringUTFChars(java_server_ip,server_ip);


}
