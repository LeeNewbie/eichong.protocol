package com.conqueue.server;

import java.util.concurrent.ConcurrentLinkedQueue;

import com.conqueue.cache.RepeatMessage;

public class ConOffer implements Runnable {

    private final ConcurrentLinkedQueue<RepeatMessage> queue;
    private final RepeatMessage mes;

	public ConOffer(ConcurrentLinkedQueue<RepeatMessage> queue, RepeatMessage message) {
		this.queue = queue;
		this.mes = message;
	}

	public void run() {
		queue.offer(mes);
	}
}