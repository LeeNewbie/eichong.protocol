package com.petmagic.embchat.learnthread;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
/**
 * Created by c on 15/7/13.
 */
public class ExecutorServiceExecuteTest {

    public static void main(String[] args) throws InterruptedException {
        ExecutorService executorService = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        for (int i = 0; i < 10; i++) {
            executorService.execute(new MyRunnable());
        }

        //已提交的任务为已经加载到线程池的任务
        //停止接受新任务，等待已提交的任务(包括正在执行的和还没有开始执行的)完成 关闭线程池
        executorService.shutdown();

        //取消运行中的任务和队列中等待的任务 强制关闭线程池
        //返回值是等待在工作队列中的任务
        //List<Runnable> list = executorService.shutdownNow();

        //每隔一秒检测一次ExecutorService的关闭状态
        /*while (!executorService.awaitTermination(1, TimeUnit.SECONDS)) {
            System.out.println("线程池没有关闭");
        }
        System.out.println("线程池已关闭");*/

        //校验ExecutorService的状态是否为TERMINATED 即关闭状态
        //当为TERMINATED时 返回true
        System.out.println(System.currentTimeMillis());
        while (!executorService.isTerminated()) {
            //没有关闭前一直循环
        }
        System.out.println(System.currentTimeMillis());

    }
}
