package com.redatoms.chat.server.groups;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Data;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Group;
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
		Data data = dataPackage.getData();
		long userId = data.getUserId();
		Data responseData = null;
		
		switch(cmd) {
		case CREATE_GROUP:
			Group group = proxy.createGroup(userId, data.getUserList());
			logger.debug(group);
			responseData = Data.newBuilder().setUserId(userId).addGroup(group).build();
			break;
		case DELETE_GROUP:
			if (data == null || data.getGroupList() == null || data.getGroupList().size() != 1) {
				//TODO Output error
			}
			proxy.delGroup(userId, data.getGroup(0).getGroupId());
			break;
		case ADD_GROUP_MEMBERS:
			if (data == null || data.getGroupList() == null || data.getGroupList().size() != 1 || data.getUserList() == null) {
				//TODO Output error
			}
			proxy.addUserIntoGroup(userId, data.getGroup(0).getGroupId(), data.getUser(0).getUserId());
			break;
		case UPDATE_GROUP:
			//TODO implement
			break;
		case DELETE_GROUP_MEMBERS:
			if (data == null || data.getGroupList() == null || data.getGroupList().size() != 1 || data.getUserList() == null) {
				//TODO Output error
			}
			proxy.delUserFromGroup(userId, data.getGroup(0).getGroupId(), data.getUser(0).getUserId());
			break;
		default:
			throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
		}
		GroupsServer.outboundMQ.putPackage(ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.ACK, responseData));
	}

}
