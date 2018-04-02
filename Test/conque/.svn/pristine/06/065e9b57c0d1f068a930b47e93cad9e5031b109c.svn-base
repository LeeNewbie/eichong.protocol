package com.conqueue.task;

import java.util.Date;
import java.util.TimerTask;

import com.conqueue.server.ConTask;

public class CheckConTask extends TimerTask {
	public ConTask task;

	public ConTask getTask() {
		return task;
	}

	public void setTask(ConTask task) {
		this.task = task;
	}

	@Override
	public void run() {
        System.out.println("Timer task started at:"+new Date());
        task.reSendTask();
        System.out.println("Timer task finished at:"+new Date());
    }
}