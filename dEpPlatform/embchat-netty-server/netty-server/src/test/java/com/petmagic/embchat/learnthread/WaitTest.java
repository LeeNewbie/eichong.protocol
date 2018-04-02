package com.petmagic.embchat.learnthread;

/**
 * Created by c on 15/7/13.
 */
public class WaitTest {

    public static void main(String[] args) {
        new Thread(new Thread1()).start();
        try {
            //sleep 没有释放锁   也就是说占着CPU资源不工作  其他线程不能进入
            //谁调用谁去睡觉   例如有a b两个线程  即使在a线程里调用b的sleep方法，
            //实际还是a去睡觉
            Thread.sleep(5000);
        } catch (Exception e) {
            e.printStackTrace();
        }

        new Thread(new Thread2()).start();
    }

    // interrupt 可以打断sleep 也可以打断wait

}

class Thread1 implements Runnable {

    public void run() {
        synchronized (WaitTest.class) {
            System.out.println("enter thread1...");
            System.out.println("thread1 is waiting...");
            try {
                //调用wait()方法，线程会放弃对象锁，进入等待此对象的等待锁定池
                WaitTest.class.wait();
            } catch (Exception e){
                e.printStackTrace();
            }
            System.out.println("thread1 is going on ... ");
            System.out.println("thread1 is over!!!");
        }
    }
}

class Thread2 implements Runnable {

    public void run() {
        synchronized (WaitTest.class) {
            System.out.println("enter thread2 ...");
            //只有针对此对象调用notify()方法后本线程才进入对象锁定池准备获取对象锁进入运行状态。
            WaitTest.class.notify();
            // ======================
            // 区别
            // 如果我们把代码：TestD.class.notify();给注释掉，即TestD.class调用了wait()方法，
            // 但是没有调用notify()方法，则线程永远处于挂起状态。
            System.out.println("thread2 is sleep!");
            try {
                Thread.sleep(5000);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println("thread2 is going on...");
            System.out.println("thread2 is over!!");
        }
    }
}
