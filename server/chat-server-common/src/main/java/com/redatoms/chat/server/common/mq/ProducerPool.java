package com.redatoms.chat.server.common.mq;

import java.io.InputStream;
import java.io.InterruptedIOException;
import java.lang.reflect.Field;
import java.util.LinkedList;
import java.util.Properties;

import javax.jms.Connection;
import javax.jms.DeliveryMode;
import javax.jms.Destination;
import javax.jms.MessageProducer;
import javax.jms.ObjectMessage;
import javax.jms.Session;

import org.apache.activemq.ActiveMQConnection;
import org.apache.activemq.ActiveMQConnectionFactory;
import org.apache.activemq.util.IndentPrinter;
import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.util.PropertiesUtil;
import com.redatoms.chat.server.common.util.ProtoMetaUtil;
import com.redatoms.chat.server.common.util.StringUtil;

public class ProducerPool extends ThreadGroup {
	
	private static final Logger logger = Logger.getLogger(ProducerPool.class);
	
    private Destination destination;
    private long sleepTime;
    private int parallelThreads = 2;
    private long timeToLive;
    private String user = ActiveMQConnection.DEFAULT_USER;
    private String password = ActiveMQConnection.DEFAULT_PASSWORD;
    private String url = ActiveMQConnection.DEFAULT_BROKER_URL;
//    private String url = "failover://tcp://192.168.8.27:61616";
    private String subject = "TOOL.DEFAULT";
    private boolean topic;
    private boolean transacted;
    private boolean persistent;
    private static Object lockResults = new Object();
    
	private boolean isClosed = false; // 线程池是否关闭
	private LinkedList<DataPackage> packageQueue; // 工作队列

	public ProducerPool(String name) {
		this(name, null);
	}
	
	public ProducerPool(String name, String configFileName) { 
		super("ProducerPool:" + name); // 指定ThreadGroup的名称
		setDaemon(true); // 继承到的方法，设置是否守护线程池
		packageQueue = new LinkedList<DataPackage>(); // 创建工作队列
		
		PropertiesUtil.setProperties(this, configFileName);
		
		subject = name;
		
		for (int i = 0; i < parallelThreads; i++) {
			new WorkThread(i).start(); // 创建并启动工作线程,线程池数量是多少就创建多少个工作线程
		}
	}
	


	/** 向工作队列中加入一个新任务,由工作线程去执行该任务 */
	public synchronized void putPackage(DataPackage dataPackage) {
		if (isClosed) {
			throw new IllegalStateException();
		}
		if (dataPackage != null) {
			packageQueue.add(dataPackage);// 向队列中加入一个任务
			notify(); // 唤醒一个正在getPackage()方法中待任务的工作线程
		}
	}

	/** 从工作队列中取出一个任务,工作线程会调用此方法 */
	private synchronized DataPackage getPackage()
			throws InterruptedException {
		while (packageQueue.size() == 0) {
			if (isClosed)
				return null;
			if (logger.isDebugEnabled()) {
				logger.debug("[" + this.getName() + ":worker_" + Thread.currentThread().getName() + "] is waiting for packages...");
			}
			wait(); // 如果工作队列中没有任务,就等待任务
		}
		if (logger.isDebugEnabled()) {
			logger.debug("[" + this.getName() + ":worker_" + Thread.currentThread().getName() + "] active");
		}
		return (DataPackage) packageQueue.removeFirst(); // 反回队列中第一个元素,并从队列中删除
	}

	/** 关闭线程池 */
	public synchronized void closePool() {
		if (!isClosed) {
			waitFinish(); // 等待工作线程执行完毕
			isClosed = true;
			packageQueue.clear(); // 清空工作队列
			interrupt(); // 中断线程池中的所有的工作线程,此方法继承自ThreadGroup类
		}
	}

	/** 等待工作线程把所有任务执行完毕 */
	public void waitFinish() {
		synchronized (this) {
			isClosed = true;
			notifyAll(); // 唤醒所有还在getPackage()方法中等待任务的工作线程
		}
		Thread[] threads = new Thread[activeCount()]; // activeCount()
														// 返回该线程组中活动线程的估计值。
		int count = enumerate(threads); // enumerate()方法继承自ThreadGroup类，根据活动线程的估计值获得线程组中当前所有活动的工作线程
		for (int i = 0; i < count; i++) { // 等待所有工作线程结束
			try {
				threads[i].join(); // 等待工作线程结束
			} catch (InterruptedException ex) {
				ex.printStackTrace();
			}
		}
	}

	/**
	 * 内部类,工作线程,负责从工作队列中取出任务,并执行
	 * 
	 */
	private class WorkThread extends Thread {
		private int id;

		public WorkThread(int id) {
			// 父类构造方法,将线程加入到当前ThreadPool线程组中
			super(ProducerPool.this, id + "");
			this.id = id;
		}

		public void run() {
			Connection connection = null;
	        try {
	            // Create the connection.
	            ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory(user, password, url);
	            connection = connectionFactory.createConnection();
	            connection.start();

	            // Create the session
	            Session session = connection.createSession(transacted, Session.AUTO_ACKNOWLEDGE);
	            if (topic) {
	                destination = session.createTopic(subject);
	            } else {
	                destination = session.createQueue(subject);
	            }

	            // Create the producer.
	            MessageProducer producer = session.createProducer(destination);
	            if (persistent) {
	                producer.setDeliveryMode(DeliveryMode.PERSISTENT);
	            } else {
	                producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);
	            }
	            if (timeToLive != 0) {
	                producer.setTimeToLive(timeToLive);
	            }
	            if (logger.isDebugEnabled()) {
	            	logger.debug("[" + this.getName() + "] begin to take packages.");
	            }
	            // Start sending messages
	            sendLoop(session, producer);

	            logger.info("[" + this.getName() + "] Done.");

	            synchronized (lockResults) {
	                ActiveMQConnection c = (ActiveMQConnection) connection;
//	                System.out.println("[" + this.getName() + "] Results:\n");
	                c.getConnectionStats().dump(new IndentPrinter());
	            }

	        } catch (Exception e) {
	            logger.error("[" + this.getName() + "] Caught: ", e);
	        } finally {
	            try {
	                connection.close();
	            } catch (Throwable ignore) {
	            	//ignore
	            }
	        }
	        

		}// end run
		
	    protected void sendLoop(Session session, MessageProducer producer) throws Exception {
	    	
			while (!isInterrupted()) { // isInterrupted()方法继承自Thread类，判断线程是否被中断
				DataPackage dataPackage = null;
				try {
					dataPackage = getPackage(); // 取出任务
				} catch (InterruptedException e) {
					logger.error("failed to get package, worder is interrupted", e);
				}
				// 如果getPackage()返回null或者线程执行getPackage()时被中断，则结束此线程
				if (dataPackage == null)
					return;

				try {
					ObjectMessage message = session.createObjectMessage(dataPackage.toByteArray());
					if (logger.isDebugEnabled()) {
						logger.debug("sending message: " + ProtoMetaUtil.print(dataPackage));
					}
					producer.send(message);

		            if (transacted) {
		            	if (logger.isDebugEnabled()) {
		            		logger.debug("[" + this.getName() + "] Committing message");
		            	}
		                session.commit();
		            }
				} catch (Exception e) {
					logger.error("failed to send message", e);
				}
				Thread.sleep(sleepTime);
			}
	    }
	}
}
