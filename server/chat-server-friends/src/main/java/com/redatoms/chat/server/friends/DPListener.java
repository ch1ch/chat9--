package com.redatoms.chat.server.friends;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Data;
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
		User friend = null;
		
		switch(cmd) {
		case SEARCH_USER_BY_NAME:
		case SEARCH_USER_BY_ID:
		case DELETE_FRIEND:
			friend = data.getUserList().get(0);
			proxy.delUserFriends(userId, friend.getUserId());
			break;
		case ADD_FRIEND:
			friend = data.getUserList().get(0);
			friend = proxy.inviteUserFriend(userId, friend.getUserId());
			proxy.confirmUserFriend(friend.getUserId(), userId, true);		//TODO send invite friend message to invited user
			responseData = Data.newBuilder().setUserId(userId).addUser(friend).build();
			break;
		case LIST_FRIENDS:
			List<User> friendList = proxy.getFriendsList(userId);
			responseData = Data.newBuilder().setUserId(userId).addAllUser(friendList).build();
			break;

		default:
			throw new UnsupportedOperationException("Unsupported cmd: " + cmd);
		}
		FriendsServer.outboundMQ.putPackage(ProtoMetaUtil.buildResponsePackage(dataPackage, CmdType.LIST_FRIENDS, responseData));
    }

}
