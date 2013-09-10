package com.redatoms.chat.server.syncs;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message;
import com.redatoms.chat.server.common.listener.BatchHandler;
import com.redatoms.chat.server.common.listener.BatchHandlerListener;
import com.redatoms.chat.server.common.mq.DataPackageListener;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;
import com.redatoms.chat.server.common.util.ProtoMetaUtil;

public class DPListener implements DataPackageListener, BatchHandlerListener {
	
	private static final Logger logger = Logger.getLogger(DPListener.class);
	
	private PersistentProxy proxy;
	
	public DPListener() {
		super();
		proxy = RedisProxy.getInstance();
	}


	public void onMessage(DataPackage dataPackage) throws Exception {
    	CmdType cmd = dataPackage.getCmd();
		ProtoMeta.DataPackage.Data data = dataPackage.getData();
		
		switch(cmd) {
		case SYNC:
			BatchHandler.getInstance(this).addDataPackage(dataPackage);
//			long owner = data.getUserId();
//			List<Message> list = proxy.getAllMessage(owner);
//			if (logger.isDebugEnabled() && list != null) {
//				list.size();
//			}
//			ProtoMeta.DataPackage.Data dt = ProtoMeta.DataPackage.Data.newBuilder().setUserId(owner).addAllMessage(list).build();
//			SyncServer.outboundMQ.putPackage(ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.RECIEVE, dt));
			break;
		default:
			throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
		}
    }
	
	public void onBatchHandle(List<DataPackage> dataPackageList) {
		List<DataPackage> list = proxy.batchGetAllMessage(dataPackageList);
		ProtoMeta.DataPackage.Data data;
		for (DataPackage dataPackage : list) {
			data = ProtoMeta.DataPackage.Data.newBuilder().setUserId(dataPackage.getData().getUserId()).addAllMessage(dataPackage.getData().getMessageList()).build();
			SyncServer.outboundMQ.putPackage(ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.RECIEVE, data));
		}
	}


}
