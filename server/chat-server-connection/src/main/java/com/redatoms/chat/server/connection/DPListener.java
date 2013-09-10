package com.redatoms.chat.server.connection;

import io.netty.channel.Channel;
import io.netty.handler.codec.http.websocketx.BinaryWebSocketFrame;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.mq.DataPackageListener;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;

public class DPListener implements DataPackageListener {
	
	private static final Logger logger = Logger.getLogger(DPListener.class);
	
	private PersistentProxy proxy;
	
	public DPListener() {
		super();
		proxy = RedisProxy.getInstance();
	}


	public void onMessage(DataPackage dataPackage) throws Exception {
    	CmdType cmd = dataPackage.getCmd();
		ProtoMeta.DataPackage.Data data = dataPackage.getData();
		long owner = data.getUserId();
		Channel conn = ConnectionManager.getConnection(owner + "");
		if (conn == null) {
			return;
		}

		
		switch(cmd) {
		case ACK:
		case NOTIFY:
		case RECIEVE:
		case LIST_FRIENDS:
			BinaryWebSocketFrame bframe;
			
			bframe = new BinaryWebSocketFrame();
			bframe.content().writeBytes(dataPackage.toByteArray());
			conn.writeAndFlush(bframe);
			break;
		default:
			throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
		}
    }

}
