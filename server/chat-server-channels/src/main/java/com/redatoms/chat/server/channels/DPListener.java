package com.redatoms.chat.server.channels;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.MessageType;
import com.redatoms.chat.server.common.mq.DataPackageListener;

public class DPListener implements DataPackageListener {
	
	private static final Logger logger = Logger.getLogger(DPListener.class);
	
	public void onMessage(DataPackage dataPackage) throws Exception {
    	CmdType cmd = dataPackage.getCmd();
		ProtoMeta.DataPackage.Data data = dataPackage.getData();
		
		switch(cmd) {
		case SEND:
			ProtoMeta.DataPackage.Message msg = data.getMessage(0);
			MessageType mt = msg.getMsgType();
			switch(mt) {
			case PERSON_TO_PERSON:
				//send notify to TO client
				ProtoMeta.DataPackage.Data notifyData = ProtoMeta.DataPackage.Data.newBuilder().setUserId(msg.getTo()).build();
				ProtoMeta.DataPackage dp = ProtoMeta.DataPackage.newBuilder().setCmd(CmdType.NOTIFY).setData(notifyData).build();
				ChannelsServer.outboundMQ.putPackage(dp);
				
				//send ack to FROM client
				notifyData = ProtoMeta.DataPackage.Data.newBuilder().setUserId(msg.getFrom()).build();
				dp = ProtoMeta.DataPackage.newBuilder().setCmd(CmdType.ACK).setData(notifyData).build();
				ChannelsServer.outboundMQ.putPackage(dp);
				break;
			case GROUP:
			case CHANNEL:
			default:
				break;
			}
			break;
		default:
			throw new Exception();
		}
    }

}
