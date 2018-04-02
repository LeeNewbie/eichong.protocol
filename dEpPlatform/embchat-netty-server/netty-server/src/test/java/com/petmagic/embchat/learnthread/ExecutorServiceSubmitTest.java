package com.petmagic.embchat.learnthread;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.*;

/**
 * Created by c on 15/7/13.
 */
public class ExecutorServiceSubmitTest {

    //submit方法
    //和execute区别 1、submit返回Future，execute没有返回值
    public static void main(String[] args) {
        // newCachedThreadPool不固定大小线程池
        // 线程池的大小依赖于 操作系统或者说是JVM能够创建的最大线程大小
        ExecutorService executorService = Executors.newCachedThreadPool();
        List<Future<String>> resultList = new ArrayList<Future<String>>();

        for (int i = 0; i < 3; i++) {
            Future<String> future = executorService.submit(new TaskWithResult(i));
            resultList.add(future);
        }

        executorService.shutdown();

        //遍历任务结果
        for (Future<String> future : resultList) {
            try {
                //打印各个任务（线程）执行结果
                System.out.println(future.get());
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (ExecutionException e) {
                e.printStackTrace();
                return;
            }
        }
    }
}

class TaskWithResult implements Callable<String> {

    private int id;

    public TaskWithResult(int id) {
        this.id = id;
    }

    public String call() throws Exception {
        System.out.println("call()方法被自动调用 " + Thread.currentThread().getName());
        if(new Random().nextBoolean()) {
            throw new TaskException("Meet error in task." + Thread.currentThread().getName());
        }

        //一个模拟耗时的操作
        for (int i = 0; i < 999999999; i--);

        return "call()方法被自动调用，任务的结果是：" + id + "  " + Thread.currentThread().getName();
    }
}

class TaskException extends Exception {
    public TaskException(String message) {
        super(message);
    }
}