package com.redatoms.chat.server.channels;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.mq.ConsumerPool;
import com.redatoms.chat.server.common.mq.ProducerPool;
import com.redatoms.chat.server.common.util.Constant;


public class ChannelsServer {
	
	private static final Logger logger = Logger.getLogger(ChannelsServer.class);
	
	
	public static ProducerPool outboundMQ;

    public static void main(String[] args) {
    	try {
    		start();
    	} catch(Exception e) {
    		logger.error("faild to start notify server");
    	}
        
    }
    
    public static void start() throws Exception {
    	DPListener listener = new DPListener();
    	new ConsumerPool(Constant.MQ_NOTIFY_MESSAGE, listener);
    	outboundMQ = new ProducerPool(Constant.MQ_OUTBOUND_MESSAGE);
    	logger.info("Notify Server started.");
    }

}
