package com.petmagic.embchat.learnthread;

/**
 * Created by c on 15/7/13.
 */
public class MyRunnable implements Runnable {
    private int x = 0;

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