package com.petmagic.embchat.learnthread;

import java.util.concurrent.*;

/**
 * Created by c on 15/7/13.
 */
public class CallableTest {

    /**
     * Callable和Runnable的区别
     * Callable的call方法可以有返回值，Runnable的run方法没有返回值
     * Callable的call方法可以抛出异常，Runnable的run方法不能抛出异常
     * 运行Callable任务会返回一个Future对象
     * Future 为Callable任务返回的结果
     * Future的get方法会等待当前任务完成 并获取结果
     * Future的cancel方法可以取消当前任务的执行 true为立即终端， false为允许任务完成
     * Future的isCancelled方法返回该任务是否在执行完成之前被取消
     * Future的isDone方法返回当该任务是否执行完成
     */
    public static void main(String[] args) {
        MyCallable c1 = new MyCallable(0);
        MyCallable c2 = new MyCallable(1);
        MyCallable c3 = new MyCallable(2);

        ExecutorService service = Executors.newFixedThreadPool(3);

        try {
            Future future1 = service.submit(c1);

            // 如果调用get方法，当前线程会等待当前任务执行完毕后才往下执行
            System.out.println("c1: " + future1.get());
            System.out.println("c1: " + future1.isCancelled());
            System.out.println("c1: " + future1.isDone());

            Future future2 = service.submit(c2);
            //等待5秒，在停止第二个任务，因为第二个任务执行的是无线循环
            Thread.sleep(5000);
            System.out.println("c2 cancel: " + future2.cancel(true));

            //获取第三个任务的输出
            Future future3 = service.submit(c3);
            System.out.println("c3: " + future3.get());
        } catch (Exception e) {
            System.out.println(e.toString());
        }
        service.shutdown();
    }
}

class MyCallable implements Callable<String> {

    private int flag;

    public MyCallable(int flag) {
        this.flag = flag;
    }

    public String call() throws Exception {
        if(this.flag == 0) {
            return "flag = 0";
        }

        if(this.flag == 1) {
            try {
                while (true) {
                    System.out.println("looping");
                    Thread.sleep(2000);
                }
            } catch (InterruptedException e) {
                System.out.println("Interrupted");
            }
            return "false";
        } else {
            throw new Exception("Bad flag value!");
        }
    }
}
