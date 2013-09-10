package com.redatoms.chat.server.handler;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.mq.ConsumerPool;
import com.redatoms.chat.server.common.mq.ProducerPool;
import com.redatoms.chat.server.common.util.Constant;


public class MessageServer {
	
	private static final Logger logger = Logger.getLogger(MessageServer.class);
	
	public static ProducerPool notifyMQ;

    public static void main(String[] args) {
    	try {
    		start();
    	} catch(Exception e) {
    		//TODO
    	}
        
    }
    
    public static void start() throws Exception {
    	DPListener listener = new DPListener();
    	new ConsumerPool(Constant.MQ_INBOUND_MESSAGE, listener, "MQConfig.properties");
    	notifyMQ = new ProducerPool(Constant.MQ_NOTIFY_MESSAGE);
    	logger.info("Message Server started.");
    }

}
