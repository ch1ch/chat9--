package com.redatoms.chat.server.common.mq;

import java.io.IOException;

import javax.jms.Connection;
import javax.jms.DeliveryMode;
import javax.jms.Destination;
import javax.jms.ExceptionListener;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.MessageListener;
import javax.jms.MessageProducer;
import javax.jms.ObjectMessage;
import javax.jms.Session;
import javax.jms.Topic;

import org.apache.activemq.ActiveMQConnection;
import org.apache.activemq.ActiveMQConnectionFactory;
import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.util.PropertiesUtil;
import com.redatoms.chat.server.common.util.ProtoMetaUtil;

public class ConsumerPool extends ThreadGroup {
	
	private static final Logger logger = Logger.getLogger(ConsumerPool.class);
	
    private Session session;
    private Destination destination;
    private MessageProducer replyProducer;

    private int maxiumMessages;
    private static int parallelThreads = 2;
    private String subject = "TOOL.DEFAULT";
    private boolean topic;
    private String user = ActiveMQConnection.DEFAULT_USER;
    private String password = ActiveMQConnection.DEFAULT_PASSWORD;
    private String url = ActiveMQConnection.DEFAULT_BROKER_URL;
//    private String url = "failover://tcp://192.168.8.27:61616";
    private boolean transacted;
    private boolean durable;
    private String clientId;
    private int ackMode = Session.AUTO_ACKNOWLEDGE;
    private String consumerName = "Default consumer";
    private long sleepTime;
    private long receiveTimeOut;
	private long batch = 10; // Default batch size for CLIENT_ACKNOWLEDGEMENT or SESSION_TRANSACTED
    
	private boolean isClosed = false; // 线程池是否关闭
	
	private DataPackageListener dataPackageListener;

	public ConsumerPool(String name, DataPackageListener mqListener) { 
		this(name, mqListener, null);
	}
	
	public ConsumerPool(String name, DataPackageListener mqListener, String configFileName) { 
		super("ConsumerPool:" + name); // 指定ThreadGroup的名称
		PropertiesUtil.setProperties(this, configFileName);
		subject = name;
		consumerName = name;
		dataPackageListener = mqListener;
		setDaemon(true); // 继承到的方法，设置是否守护线程池
		
		for (int i = 0; i < parallelThreads; i++) {
			new WorkThread(i).start(); // 创建并启动工作线程,线程池数量是多少就创建多少个工作线程
		}
	}

	/** 关闭线程池 */
	public synchronized void closePool() {
		if (!isClosed) {
			waitFinish(); // 等待工作线程执行完毕
			isClosed = true;
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
	
	public void handleMessage(DataPackage dataPackage) throws Exception {
    	if (logger.isDebugEnabled()) {
    		logger.debug("Thread: " + Thread.currentThread().getId() + " is handling message");
    	}
		if (dataPackageListener != null) {
			dataPackageListener.onMessage(dataPackage);
		}
	}

	/**
	 * 内部类,工作线程,负责从工作队列中取出任务,并执行
	 * 
	 */
	private class WorkThread extends Thread implements MessageListener, ExceptionListener {
		
		private int id;
		
		private boolean running;
		
		private long messagesReceived = 0;

		public WorkThread(int id) {
			// 父类构造方法,将线程加入到当前ThreadPool线程组中
			super(ConsumerPool.this, id + "");
			this.id = id;
		}

		public void run() {
			try {
				running = true;
				
	        	ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory(user, password, url);
	            Connection connection = connectionFactory.createConnection();
	            if (durable && clientId != null && clientId.length() > 0 && !"null".equals(clientId)) {
	                connection.setClientID(clientId);
	            }
	            connection.setExceptionListener(this);
	            connection.start();

	            session = connection.createSession(transacted, ackMode);
	            if (topic) {
	                destination = session.createTopic(subject);
	            } else {
	                destination = session.createQueue(subject);
	            }

	            replyProducer = session.createProducer(null);
	            replyProducer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);

	            MessageConsumer consumer = null;
	            if (durable && topic) {
	                consumer = session.createDurableSubscriber((Topic) destination, consumerName);
	            } else {
	                consumer = session.createConsumer(destination);
	            }

	            if (maxiumMessages > 0) {
	                consumeMessagesAndClose(connection, session, consumer);
	            } else {
	                if (receiveTimeOut == 0) {
	                    consumer.setMessageListener(this);
	                } else {
	                    consumeMessagesAndClose(connection, session, consumer, receiveTimeOut);
	                }
	            }
	        } catch (Exception e) {
	            logger.error("[" + this.getName() + "] Caught: ", e);
	        }
		}
		

	    public void onMessage(Message message) {
	    	
	    	if (logger.isDebugEnabled()) {
	    		logger.debug("Thread: " + Thread.currentThread().getId() + ": " + this.getId() + " is handling message");
	    	}
	    	
	    	messagesReceived++;
	    	
	        try {
	            if (message.getJMSReplyTo() != null) {
	                replyProducer.send(message.getJMSReplyTo(), session.createTextMessage("Reply: " + message.getJMSMessageID()));
	            }

	            if (transacted) {
					if ((messagesReceived % batch) == 0) {
						if (logger.isDebugEnabled()) {
							logger.debug("Commiting transaction for last " + batch + " messages; messages so far = " + messagesReceived);
						}
						session.commit();
					}
	            } else if (ackMode == Session.CLIENT_ACKNOWLEDGE) {
					if ((messagesReceived % batch) == 0) {
						if (logger.isDebugEnabled()) {
							logger.debug("Acknowledging last " + batch + " messages; messages so far = " + messagesReceived);
						}
						message.acknowledge();
					}
	            }
	            try {
	            	if (message instanceof ObjectMessage) {
	            		ObjectMessage dataMsg = (ObjectMessage) message;
	            		ProtoMeta.DataPackage dataPkg = ProtoMeta.DataPackage.parseFrom((byte[]) dataMsg.getObject());
	            		if (logger.isDebugEnabled()) {
	            			logger.debug("got package: " + ProtoMetaUtil.print(dataPkg));
	            		}
	            		handleMessage(dataPkg);
	            	} else {
	            		logger.error("unknow message");
	            	}
	            } catch(Exception e) {
	            	logger.error("failed to handle message", e);
	            }

	        } catch (JMSException e) {
	            logger.error("[" + this.getName() + "] Caught: ", e);
	        } finally {
	            if (sleepTime > 0) {
	                try {
	                    Thread.sleep(sleepTime);
	                } catch (InterruptedException e) {
	                }
	            }
	        }
	    }

	    public synchronized void onException(JMSException ex) {
	        logger.error("[" + this.getName() + "] JMS Exception occured.  Shutting down client.", ex);
	        running = false;
	    }

	    synchronized boolean isRunning() {
	        return running;
	    }

	    protected void consumeMessagesAndClose(Connection connection, Session session, MessageConsumer consumer) throws JMSException,
	            IOException {
	        logger.info("[" + this.getName() + "] We are about to wait until we consume: " + maxiumMessages
	                + " message(s) then we will shutdown");

	        for (int i = 0; i < maxiumMessages && isRunning();) {
	            Message message = consumer.receive(1000);
	            if (message != null) {
	                i++;
	                onMessage(message);
	            }
	        }
	        logger.info("[" + this.getName() + "] Closing connection");
	        consumer.close();
	        session.close();
	        connection.close();
	    }

	    protected void consumeMessagesAndClose(Connection connection, Session session, MessageConsumer consumer, long timeout)
	            throws JMSException, IOException {
	        logger.info("[" + this.getName() + "] We will consume messages while they continue to be delivered within: " + timeout
	                + " ms, and then we will shutdown");

	        Message message;
	        while ((message = consumer.receive(timeout)) != null) {
	            onMessage(message);
	        }

	        logger.info("[" + this.getName() + "] Closing connection");
	        consumer.close();
	        session.close();
	        connection.close();
	    }
	}
}
