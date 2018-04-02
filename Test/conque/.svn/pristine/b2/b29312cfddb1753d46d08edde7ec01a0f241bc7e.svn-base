package com.conqueue.service;

import java.util.concurrent.ConcurrentLinkedQueue;

public class SendTaskService {
    private static ConcurrentLinkedQueue<Object> conQueue = new ConcurrentLinkedQueue<Object>();

    public static void offer(Object o) {
    	conQueue.offer(o);
    }
}