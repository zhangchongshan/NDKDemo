package com.sonoptek.javalib;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class SocketServer {

    public static ReaderThread readerThread;
    public static void main(String[] args) {
        try {
            final ServerSocket serverSocket=new ServerSocket();
            serverSocket.bind(new InetSocketAddress("0.0.0.0",9998));
//            serverSocket.setReuseAddress(true); //设置 ServerSocket 的选项
//            serverSocket.bind(new InetSocketAddress(8080)); //与 8080 端口绑定
            System.out.println("server start.......");

            System.out.println(""+serverSocket.getInetAddress().getHostAddress());
            new Thread(new Runnable() {
                @Override
                public void run() {
                    while (true){


                        try {
                            Thread.sleep(5000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        Socket socket= null;
                        try {
                            socket = serverSocket.accept();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                        if (readerThread!=null){
                            readerThread.interrupt();
                            readerThread=null;
                        }
                        readerThread=new ReaderThread(socket);
                        readerThread.start();
                    }
                }
            }).start();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    private static class ReaderThread extends Thread{
        BufferedReader reader;

        public ReaderThread(Socket socket) {
            try {
                reader=new BufferedReader(new InputStreamReader(socket.getInputStream()));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            super.run();

            String content=null;
            while (true){

                if (isInterrupted()){
                    break;
                }
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                try {
                    while ((content=reader.readLine())!=null) {
                        if (isInterrupted()){
                            break;
                        }
                        System.out.println("接收到了客户端："+content);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        }
    }
}