package com.redatoms.chat.server.notify;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.MessageType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.User;
import com.redatoms.chat.server.common.mq.DataPackageListener;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;
import com.redatoms.chat.server.common.util.ProtoMetaUtil;

public class DPListener implements DataPackageListener {
	
	private static final Logger logger = Logger.getLogger(DPListener.class);
	
	private static final PersistentProxy proxy = RedisProxy.getInstance();
	
	public void onMessage(DataPackage dataPackage) throws Exception {
    	CmdType cmd = dataPackage.getCmd();
		ProtoMeta.DataPackage.Data data = dataPackage.getData();
		ProtoMeta.DataPackage.Data notifyData;
		ProtoMeta.DataPackage dp;
		
		switch(cmd) {
		case SEND:
			ProtoMeta.DataPackage.Message msg = data.getMessage(0);
			MessageType mt = msg.getMsgType();
			switch(mt) {
			case PERSON_TO_PERSON:
				//send notify to TO client
				notifyData = ProtoMeta.DataPackage.Data.newBuilder().setUserId(msg.getTo()).build();
				dp = ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.NOTIFY, notifyData);
				NotifyServer.outboundMQ.putPackage(dp);
				
				//send ack to FROM client
				notifyData = ProtoMeta.DataPackage.Data.newBuilder().setUserId(msg.getFrom()).build();
				dp = ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.ACK, notifyData);
				NotifyServer.outboundMQ.putPackage(dp);
				break;
			case GROUP:
				//send ack to FROM client
				notifyData = ProtoMeta.DataPackage.Data.newBuilder().setUserId(msg.getFrom()).build();
				dp = ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.ACK, notifyData);
				NotifyServer.outboundMQ.putPackage(dp);
				
				List<User> members = proxy.getGroupMembersList(data.getUserId(), msg.getTo());
				
				for (User user : members) {
					notifyData = ProtoMeta.DataPackage.Data.newBuilder().setUserId(user.getUserId()).build();
					dp = ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.NOTIFY, notifyData);
					NotifyServer.outboundMQ.putPackage(dp);
				}
				//send notify to TO client
				break;
			case CHANNEL:
			default:
				break;
			}
			break;
		default:
			throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
		}
    }

}
