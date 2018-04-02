package com.conqueue.server;

import java.util.ArrayList;
import java.util.LinkedList;

import com.conqueue.constant.QueConstant;
import com.conqueue.constant.TaskStatus;

public class ConTaskQueue implements Runnable {
	
	// 在等待的任务队列
	static LinkedList<ConTask> tasks_wait = new LinkedList<ConTask>();
	// 正在执行的任务
	static ArrayList<ConTask> tasks_running = new ArrayList<ConTask>();
	// 是否持续运行
	public static boolean isRun = true;
	// runnable保证线程安全
	private static ConTaskQueue runnable = new ConTaskQueue();;

	public static ConTaskQueue getRunnable() {
		return runnable;
	}

	// 如果队列线程为空或者停止则重新开启
	public static void serivesRun() {
		boolean isCanSeriver = false;
		synchronized (tasks_running) {
			isCanSeriver = tasks_running.size() < QueConstant.THREAD_MAX_NUM;
		}
		if (isCanSeriver) runnable.run();
	}

	// 获取正在执行的任务数
	public static int getRunningTaskCount() {
		synchronized (ConTaskQueue.tasks_running) {
			return ConTaskQueue.tasks_running.size();
		}
	}

	// 线程锁 如果等待队列的任务数不为空，并且当前线程数字少于最大线程数
	public static boolean taskRun() {
		synchronized (tasks_wait) {
			synchronized (tasks_running) {
				return !tasks_wait.isEmpty()
						&& tasks_running.size() < QueConstant.THREAD_MAX_NUM;
			}
		}
	}

	// 开启新线程
	public void run() {
		// 线程锁 如果等待队列的任务数不为空，并且当前线程数字少于最大线程数
		ConTask newTask;
		while ((newTask = getWaittingTask()) != null) {
			System.err.println("开启新线程处理一个新任务，ID：" + newTask.getTaskID());
			//newTask.setOnSystemFinishListen(runnable);
			newTask.threadRun();
			newTask = null;
		}

	}

	// 递归 避免新开线程 唤醒等待中的任务 但此方案会造成java.lang.StackOverflowError
	void notifyWaitingTask() {
		ConTask newTask;
		while ((newTask = getWaittingTask()) != null) {
			System.err.println("唤醒旧线程处理一个新任务，ID：" + newTask.getTaskID());
			//newTask.setOnSystemFinishListen(runnable);
			newTask.run();
			newTask = null;
		}

	}

	private ConTask getWaittingTask() {
		ConTask t = null;
		// 测试
		while (isRun && taskRun()) {
			// 添加带执行中动态数组中
			synchronized (tasks_wait) {
				// 从等待任务的队列中获取并移除此列表的头（第一个元素）
				t = tasks_wait.poll();
				// 如果h为空则从队列重新取对象或者任务绑定的状态变化了
				if (t == null || t.status == TaskStatus.without) {
					System.out.println("任务取消 编号" + t != null ? String.valueOf(t
							.getTaskID()) : "空任务");
					continue;
				}
			}
			synchronized (tasks_running) {
				tasks_running.add(t);
			}
			System.out.println("正在执行任务数" + tasks_running.size() + "/上限"
					+ QueConstant.THREAD_MAX_NUM);

			return t;
		}
		return t;
	}
}