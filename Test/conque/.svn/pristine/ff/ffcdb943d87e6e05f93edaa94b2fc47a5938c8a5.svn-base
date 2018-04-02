package com.conqueue.server;

import java.util.Timer;

import com.conqueue.constant.QueConstant;
import com.conqueue.constant.TaskStatus;
import com.conqueue.task.CheckConTask;

public class ConTask implements Runnable {

	/** 单例 可以提高性能 */
	protected final static Exception withoutException = new Exception(
			"The state is without");

	/** 用于任务完成后发送消息 */
	/** 结果 */
	public Object result;
	/** 任务编号标示 */
	public int taskID = -1;

	/** 保存一个对象 */
	public Timer timer = new Timer(true);
	protected int tryAgain = 0;
	/** 获得当前自身线程的引用 在threadRun方法 */
	protected Thread thread;

	public ConTask() {
	}

	/** 状态 */
	TaskStatus status = TaskStatus.untreated;

	public void setWithout() {
		this.status = TaskStatus.without;
	}

	public void remove() {
		this.status = TaskStatus.without;
	}

	/** 启动线程 */
	public void start() {
		synchronized (ConTaskQueue.tasks_wait) {
			ConTaskQueue.tasks_wait.remove(this);
			ConTaskQueue.tasks_wait.add(this);
			// 启动此服务
			ConTaskQueue.serivesRun();
		}

	}

	/** 启动线程 */
	final void threadRun() {
		thread = new Thread(this);
		thread.start();
	}

	// 中断Execute方法
	public void shutDownExecute() {
	};

	// 禁止被重写
	public final Object Execute() throws Exception {
		// 状态从未处理改变为处理中
		status = TaskStatus.running;

		// 获取最后一次是否错误
		Exception exception = null;
		// 是否有缓存数据如果没有
		if (result == null) {
			try {
				exception = null;
				// 如果失败
				if (result == null) {
					try {
						Thread.sleep(QueConstant.TRY_AGAIN_TIME);
					} catch (Exception e1) {
						e1.printStackTrace();
					}
				}
			} catch (Exception e) {
				if ((exception = e) != withoutException) {
					e.printStackTrace();
				}
			}
		}
		// 如果最后一次仍然失败则抛出
		if (exception != null) {
			throw exception;
		}

		status = TaskStatus.finsh;
		return result;
	}

	private void startCheckConTask(long initDelay) {
		CheckConTask checkConTask =  new CheckConTask();
		checkConTask.setTask(this);

        timer.scheduleAtFixedRate(checkConTask, 0, initDelay);
	}

	public void reSendTask() {
		if (tryAgain <= QueConstant.TRY_AGAIN_COUNT) this.start();
		tryAgain++;
		System.out.println("tryAgain=" + tryAgain);
    }

	@Override
	public void run() {
		if (tryAgain == 0) onStart();

		try {
			Execute();
		} catch (Exception e) {
			e.printStackTrace();
			status = TaskStatus.error;
		} finally {
			// 完成监听并将结果加入到主线程
			if (result != null || tryAgain == QueConstant.TRY_AGAIN_COUNT) onFinish();
			OnSystemFinish();
		}

		// 递归 避免新开线程 唤醒等待中的任务
		ConTaskQueue.getRunnable().notifyWaitingTask();

	}

	private void onStart() {
		startCheckConTask(QueConstant.TRY_AGAIN_TIME);
		System.err.println("任务编号" + taskID + "任务开始");
	}
	private void onFinish() {
		timer.cancel();
		System.err.println("任务编号" + taskID + "任务完成");
	}

	private void OnSystemFinish() {
		synchronized (ConTaskQueue.tasks_running) {
			// 从处理中的动态数组中移除此任务
			ConTaskQueue.tasks_running.remove(this);
			System.out.println("执行队列中移除任务taskid=" + taskID);
			// 通知执行后续未处理的任务
			System.out.println("正在执行任务数" + ConTaskQueue.tasks_running.size() + "/上限"
					+ QueConstant.THREAD_MAX_NUM);
		}

	}

	public Thread getThread() {
		return thread;
	}

	public TaskStatus getStatus() {
		return status;
	}

	public int getTaskID() {
		return taskID;
	}

	public ConTask setTaskID(int taskID) {
		this.taskID = taskID;
		return this;
	}

	public Object getResult() {
		return result;
	}
}