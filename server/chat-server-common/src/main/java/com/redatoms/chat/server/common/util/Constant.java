package com.redatoms.chat.server.common.util;

public class Constant {

	// Message Queue
	public static final String MQ_INBOUND_MESSAGE = "Inbound Message";
	public static final String MQ_OUTBOUND_MESSAGE = "Outbound Message";
	public static final String MQ_NOTIFY_MESSAGE = "Notify Message";
	public static final String MQ_SYNC_MESSAGE = "Sync Message";
	public static final String MQ_FRIENDS_MESSAGE = "Friends Message";
	public static final String MQ_GROUPS_MESSAGE = "Groups Message";
	public static final String MQ_CHANNELS_MESSAGE = "Channels Message";

	// redis key 相关
	// 连接配置
	public static final String REDIS_CONFIG_CONN = "redis_config";
	public static final String REDIS_CONFIG_HOST = "192.168.8.27";
	public static final int REDIS_CONFIG_PORT = 6379;
	// 动态消息
	public static final String REDIS_MSG_CONN = "redis_msg";
	public static final String REDIS_MSG_HOST = "192.168.8.27";
	public static final int REDIS_MSG_PORT = 6380;

	// key
	// last version key prefix hash table
	public static final String LAST_MSG_VER = "lastMsgVer_";
	// last version fields prefix
	public static final String MSG_P2P_VER = "msgP2PVer_";
	public static final String MSG_GROUP_VER = "msgGroupVer_";
	public static final String MSG_CHANNEL_VER = "msgChannelVer_";

	// 最新消息版本 存储消息版本 key-string
	/*
	 * public static final String MSG_P2P_LATEST_VER = "msgP2PLatestVer_";
	 * public static final String MSG_GROUP_LATEST_VER = "msgGroupLatestVer_";
	 * public static final String MSG_CHANNEL_LATEST_VER =
	 * "msgChannelLatestVer_";
	 */
	// 存储消息内容 sorted set
	// key
	public static final String MSG_P2P_CONTENT = "msgP2P_";
	public static final String MSG_GROUP_CONTENT = "msgGroup_";
	public static final String MSG_CHANNEL_CONTENT = "msgChannel_";

	// 存数个人属性 key- hashmap
	public static final String USER_PROFILE = "userProfile_";
	// 存储添加好友关系列表 key-set
	public static final String USER_INVITE_FRIENDS = "userInviteFriends_";
	// 存储添加好友关系列表 key-set
	public static final String USER_INVITE_AGREE_FRIEND = "userInviteAgreeFriend_";
	// 存储添加好友关系列表 key-set
	public static final String USER_INVITE_REJECT_FRIEND = "userInviteRejectFriend_";
	// 存储删除好友关系列表 key-set
	public static final String USER_DEL_FRIENDS = "userDelFriends_";
	// 存储好友关系列表 key-set
	public static final String USER_FRIENDS = "userFriends_";
	// 存储群组列表 key-set
	public static final String GROUP_MEMBERS = "groupMembers_";
	// 存储群组列表 key-hashtable
	public static final String GROUP_PROFILE = "groupProfile_";

	// 存储群组列表 key-set
	public static final String GROUP_MEMBER_DEL = "groupMemberDel_";
	// 存储channel属性
	public static final String CHANNEL_PROFILE = "channelProfile_";
	// 存储群组number
	public static final String GLOBAL_GROUP_SEQ = "groupSeq";
	// 存储群组number
	public static final String GLOBAL_CHANNEL_SEQ = "channelSeq";
	// 存储群组connection information
	public static final String CONNECTON_INFO = "connectionInfo";
	// 存储群组notification information
	public static final String NOTIFICATION_FLAG = "notificationFlg";

}
