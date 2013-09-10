package com.redatoms.chat.server.handler;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.MessageType;
import com.redatoms.chat.server.common.listener.BatchHandler;
import com.redatoms.chat.server.common.listener.BatchHandlerListener;
import com.redatoms.chat.server.common.mq.DataPackageListener;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;

public class DPListener implements DataPackageListener, BatchHandlerListener {
	
	private static final Logger logger = Logger.getLogger(DPListener.class);
	
	private PersistentProxy proxy = RedisProxy.getInstance();

	public void onMessage(DataPackage dataPackage) throws Exception {
		if (logger.isDebugEnabled()) {
			logger.debug("Thread: " + Thread.currentThread().getId() + " is handling message");
		}
    	CmdType cmd = dataPackage.getCmd();
		ProtoMeta.DataPackage.Data data = dataPackage.getData();
		
		switch(cmd) {
		case SEND:
			ProtoMeta.DataPackage.Message msg = data.getMessage(0);
			MessageType mt = msg.getMsgType();
			switch(mt) {
			case PERSON_TO_PERSON:
			case GROUP:
			case CHANNEL:
				BatchHandler.getInstance(this).addDataPackage(dataPackage);

//				MessageServer.notifyMQ.putPackage(dataPackage);
				break;
			default:
				throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
			}
			break;
		default:
			throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
		}
    }

	public void onBatchHandle(List<DataPackage> dataPackageList) {
		proxy.batchSaveMessage(dataPackageList);
		for (DataPackage dataPackage : dataPackageList) {
			MessageServer.notifyMQ.putPackage(dataPackage);
		}
	}

}
