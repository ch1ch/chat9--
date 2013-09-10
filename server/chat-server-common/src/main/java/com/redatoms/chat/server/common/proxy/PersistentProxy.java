package com.redatoms.chat.server.common.proxy;

import java.util.List;
import java.util.Map;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Group;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.User;

public interface PersistentProxy {

	Boolean initUser(User user);

	List<DataPackage> batchGetAllMessage(List<DataPackage> list);

	List<Message> getAllMessage(long userId);

	// save message
	Message saveMessage(Message msg);

	List<DataPackage> batchSaveMessage(List<DataPackage> list);

	// 更新个人属性
	Boolean saveUserProfile(User user);

	// 获取个人属性
	User getUserProfile(long userId);

	// 邀请加好友
	User inviteUserFriend(long userId, long friendId);

	// 好友反馈是否同意
	Boolean confirmUserFriend(long userId, long friendId, boolean agree);

	// 获得好友列表
	List<User> getFriendsList(long userId);

	// 删除好友
	Boolean delUserFriends(long userId, long friendId);

	// 创建group
	Group createGroup(long ownerId, List<User> members);

	// 解散组
	Boolean delGroup(long ownerId, long groupId);

	// 从群组中删除个人
	Boolean delUserFromGroup(long ownerId, long groupId, long memberId);

	// 获得群组列表
	List<User> getGroupMembersList(long ownerId, long groupID);

	// 群组 添加成员
	Boolean addUserIntoGroup(long ownerId, long groupId, long memberId);

	/*
	 * 设置user接入服务器记录
	 */
	void setUserConnectionInfo(long userId, String serverId) throws Exception;

	/*
	 * 清除user接入服务器记录
	 */
	void clearUserConnectionInfo(long userId) throws Exception;

	/*
	 * 清除user接入服务器记录
	 */
	String getUserConnectionInfo(long userId) throws Exception;

	/*
	 * 批量设置用户不可接受Notification(消息同步中)，过期时间暂定为30秒
	 */
	void batchSetUserNotificationFlag(List<Long> userIdList) throws Exception;

	/*
	 * 批量设置用户可接受Notification(消息同步完毕)
	 */
	void batchClearUserNotificationFlag(List<Long> userIdList) throws Exception;

	/*
	 * 批量查询用户Notification info
	 */
	Map<Long, String> batchGetUserNotificationInfo(List<Long> userIdList)
			throws Exception;

}
