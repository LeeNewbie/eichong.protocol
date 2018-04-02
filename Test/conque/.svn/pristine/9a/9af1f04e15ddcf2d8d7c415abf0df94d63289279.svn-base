package com.conqueue.server;

import java.util.concurrent.Callable;
import java.util.concurrent.ConcurrentLinkedQueue;

import com.conqueue.cache.RepeatMessage;

public class ConPoll implements Callable<RepeatMessage> {

    private final ConcurrentLinkedQueue<RepeatMessage> queue;

	public ConPoll(ConcurrentLinkedQueue<RepeatMessage> queue) {
		this.queue = queue;
	}

	public RepeatMessage call() {
		RepeatMessage mes = queue.poll();
		return mes;
	}
}