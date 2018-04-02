package com.conqueue.service;
import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

import com.conqueue.cache.RepeatMessage;

public class RepeatConQueue {
	public ConcurrentLinkedQueue<RepeatMessage> queue = new ConcurrentLinkedQueue<RepeatMessage>();
	public ConcurrentHashMap<String,RepeatMessage> quesend = new ConcurrentHashMap<String,RepeatMessage>();

    /**
     * 生产
     */
    public void put(RepeatMessage mes) {
    	queue.offer(mes);
    }

    /**
     * 发送
     */
    public boolean send() {
    	if (queue.isEmpty()) return false;

    	RepeatMessage mes = queue.poll();
    	mes.send();
	    quesend.put(mes.getKey(),mes);

	    return true;
    }

    /**
     * 消费
     */
    public RepeatMessage get(String key) {
    	RepeatMessage mes = quesend.get(key);
    	
    	return mes;
    }

    /**
     * 检查
     */
	@SuppressWarnings("rawtypes")
    public void check(int cnt) {
    	int ret = 0;
    	int count = 0;
    	Iterator iter = quesend.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
    		RepeatMessage mes = (RepeatMessage) entry.getValue();
    		if (null == mes) continue;

    		if (mes.getTimes() > 0) {
	    		ret = mes.check();
				if (ret == 2) quesend.remove(mes.getKey());
				count++;
				if (count >= cnt) break;
    		}
    	}
	}
}