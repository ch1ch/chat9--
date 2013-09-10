package com.redatoms.chat.server.friends;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.mq.ConsumerPool;
import com.redatoms.chat.server.common.mq.DataPackageListener;
import com.redatoms.chat.server.common.mq.ProducerPool;
import com.redatoms.chat.server.common.util.Constant;


public class FriendsServer {
	
	private static final Logger logger = Logger.getLogger(FriendsServer.class);
	
	
	public static ProducerPool outboundMQ;

    public static void main(String[] args) {
    	try {
    		start();
    	} catch(Exception e) {
    		logger.error("faild to start friends server");
    	}
        
    }
    
    public static void start() throws Exception {
    	DataPackageListener listener = new DPListener();
    	new ConsumerPool(Constant.MQ_FRIENDS_MESSAGE, listener);
    	outboundMQ = new ProducerPool(Constant.MQ_OUTBOUND_MESSAGE);
    	logger.info("Friends Server started.");
    }

}
