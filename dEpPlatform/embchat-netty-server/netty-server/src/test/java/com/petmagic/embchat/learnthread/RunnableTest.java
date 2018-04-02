package com.petmagic.embchat.learnthread;

/**
 * Created by c on 15/7/13.
 */
public class RunnableTest {

    public static void main(String[] args) {
        MyRunnable r = new MyRunnable();
        for (int i = 0; i < 10; i++) {
            new Thread(r).start();
        }

    }
}


