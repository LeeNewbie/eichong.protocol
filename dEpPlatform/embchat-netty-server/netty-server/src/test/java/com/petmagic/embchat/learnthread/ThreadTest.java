package com.petmagic.embchat.learnthread;

/**
 * Created by c on 15/7/13.
 */
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        MyThread t = new MyThread();
        t.start();
        t.join();  //让当前线程等待线程t执行完成才继续运行  本例中是主线程等待
        System.out.println(101);
    }

}

class MyThread extends Thread {
    private int x = 0;

    @Override
    public void run() {
        for (int i = 0; i < 100; i++) {
            try {

                Thread.sleep(10);
            } catch (Exception e) {
            }

            System.out.println(x++);
        }
    }
}