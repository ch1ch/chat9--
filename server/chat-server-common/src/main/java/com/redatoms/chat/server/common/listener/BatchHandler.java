package com.redatoms.chat.server.common.listener;

import java.util.LinkedList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;


public class BatchHandler {
	
	private static final Logger logger = Logger.getLogger(BatchHandler.class);
	
	private static int MAX_PACKAGE_SIZE = 1;
	
	private static int TIMER_DELAY = 500000;		//default 50(ms)
	
	private Timer timer;
	
	private TimerTask task;
	
	private BatchHandlerListener listener;
	
	private List<DataPackage> dataPackageList;
	
	private static final ThreadLocal<BatchHandler> threadLocal = new ThreadLocal<BatchHandler>();
	
	private BatchHandler(BatchHandlerListener listener) {
		this.timer = new Timer();
		this.dataPackageList = new LinkedList<DataPackage>();
		this.listener = listener;
		resetTimer();
	}
	
	public static BatchHandler getInstance(BatchHandlerListener listener) {
		BatchHandler threadData = threadLocal.get();
		if (threadData == null) {
			threadData = new BatchHandler(listener);
			threadLocal.set(threadData);
		}
		return threadData;
	}

	public void addDataPackage(DataPackage dataPackage) {
		dataPackageList.add(dataPackage);
		if (logger.isDebugEnabled()) {
			logger.debug("dataPackage size: " + dataPackageList.size());
		}
		if (dataPackageList.size() >= MAX_PACKAGE_SIZE) {
			handle();
		}
	}
	
	private void handle() {
		synchronized (dataPackageList) {
			if (dataPackageList.size() > 0) {
				if (listener != null) {
					listener.onBatchHandle(dataPackageList);
				}
				dataPackageList.clear();
			}
		}
		resetTimer();
	}
	
	private void resetTimer() {
		if (task != null) {
			task.cancel();
		}
		task = createTask();
		timer.schedule(task, TIMER_DELAY);
	}
	
	public TimerTask createTask() {
		return new TimerTask() {
			public void run() {
				if (logger.isDebugEnabled()) {
					logger.debug("time to handle dataPackage.");
				}
				handle();
			}
		};
	}
}
