package com.conqueue.service;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import com.conqueue.cache.RepeatMessage;
import com.conqueue.server.ConOffer;
import com.conqueue.server.ConPoll;

public class ConQueueService {
    private static int count = 2; // 线程个数

    public static void main(String[] args) throws InterruptedException, ExecutionException {
        long timeStart = System.currentTimeMillis();
        RepeatConQueue que = new RepeatConQueue();
        offer(que);
        while (que.send());
        que.check(100);
        System.out.println("cost time " + (System.currentTimeMillis() - timeStart) + "ms");
    }
    
    /**
     * 生产
     */
    public static void offer(RepeatConQueue que) {
    	RepeatMessage mes;
        ExecutorService exeService = Executors.newFixedThreadPool(4);
        for (int i = 0; i < 100000; i++) {
        	mes = new RepeatMessage(String.valueOf(i), null);
        	exeService.submit(new ConOffer(que.queue, mes));
        }

        exeService.shutdown();
    }

    /**
     * 消费
     * @throws InterruptedException 
     * @throws ExecutionException 
     */
    public static void poll(RepeatConQueue que) throws InterruptedException, ExecutionException {
        ExecutorService es = Executors.newFixedThreadPool(4);
        for (int i = 0; i < count; i++) {
        	while (!que.queue.isEmpty()) {
        		consume(que,es);
        	}
        }
        es.shutdown();
    }

	private static void consume(RepeatConQueue que, ExecutorService es) throws InterruptedException, ExecutionException {
    	ConPoll cPoll;
    	RepeatMessage mes;

    	cPoll = new ConPoll(que.queue);
    	Future<RepeatMessage> f = es.submit(cPoll);
        mes = f.get();
		//if (mes == null) return;
        mes.send();
        que.quesend.put(mes.getKey(),mes);
		
		while (!check(que,mes));
	}

	private static boolean check(RepeatConQueue que,RepeatMessage mes) {
		int ret = mes.check();

		if (ret == 0) {
			return false;
		} else if (ret == 1) {
			mes.rePackage();
			mes.send();
			return false;
		} else if (ret == 2) {
			que.quesend.remove(mes.getKey());
		}

		return true;
	}
}