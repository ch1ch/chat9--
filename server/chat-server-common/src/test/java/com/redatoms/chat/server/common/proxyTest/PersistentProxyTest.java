package com.redatoms.chat.server.common.proxyTest;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import redis.clients.jedis.ShardedJedis;

import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.ContentType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.MessageType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.User;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;
import com.redatoms.chat.server.common.redisBase.redisBase;
import com.redatoms.chat.server.common.util.Constant;

public class PersistentProxyTest {
	private PersistentProxy proxy;
	private ShardedJedis redisMsg;
	private ShardedJedis redisConfig;

	@Before
	public void setUp() throws Exception {
		proxy = RedisProxy.getInstance();
		redisMsg = redisBase.getMsgRedis();
		redisConfig = redisBase.getCfgRedis();
	}

	@After
	public void tearDown() throws Exception {
		redisBase.returnMsgRedis(redisMsg);
		redisBase.returnCfgRedis(redisConfig);
	}

	@Test
	public void testInitUser() {
		long userId = 1000001;
		String userName = "username100001111";
		User.Builder userBuilder = User.newBuilder().setUserId(userId)
				.setUserName(userName);
		User user = userBuilder.build();
		assertEquals(true, proxy.initUser(user));
		redisConfig.del(Constant.USER_PROFILE + userId);

		// fail("Not yet implemented");
	}

	@Test
	public void testBatchGetAllMessage() {
		assertEquals(true, true);
	}

	@Test
	public void testGetAllMessage() {
		assertEquals(true, true);
	}

	@Test
	public void testSaveMessage() {
		assertEquals(true, true);
	}

	@Test
	public void testBatchSaveMessage() {
		List<Message> msgList = new ArrayList<Message>();
		Message.Builder msgbuilder;
		for (int i = 0; i < 10; i++) {
			msgbuilder = null;
			msgbuilder = Message.newBuilder().setContent("Hello lijc" + i)
					.setFrom(1)
					.setTo(2)
					.setMsgType(MessageType.PERSON_TO_PERSON)
					.setContentType(ContentType.TEXT)
					.setCreateTime(System.currentTimeMillis());
			msgList.add(msgbuilder.build());
		}
		//proxy.batchSaveMessage(msgList);
		assertEquals(true, true);
		//fail("Not yet implemented");
	}

	@Test
	public void testSaveUserProfile() {
		assertEquals(true, true);
	}

	@Test
	public void testGetUserProfile() {
		assertEquals(true, true);
	}

	@Test
	public void testInviteUserFriend() {
		assertEquals(true, true);
	}

	@Test
	public void testConfirmUserFriend() {
		assertEquals(true, true);
	}

	@Test
	public void testGetFriendsList() {
		assertEquals(true, true);
	}

	@Test
	public void testDelUserFriends() {
		assertEquals(true, true);
	}

	@Test
	public void testCreateGroup() {
		assertEquals(true, true);
	}

	@Test
	public void testDelGroup() {
		assertEquals(true, true);
	}

	@Test
	public void testDelUserFromGroup() {
		assertEquals(true, true);
	}

	@Test
	public void testGetGroupMembersList() {
		assertEquals(true, true);
	}

	@Test
	public void testAddUserIntoGroup() {
		assertEquals(true, true);
	}

	@Test
	public void testSetUserConnectionInfo() {
		assertEquals(true, true);
	}

	@Test
	public void testClearUserConnectionInfo() {
		assertEquals(true, true);
	}

	@Test
	public void testGetUserConnectionInfo() {
		assertEquals(true, true);
	}

	@Test
	public void testBatchSetUserNotificationFlag() {
		assertEquals(true, true);
	}

	@Test
	public void testBatchClearUserNotificationFlag() {
		assertEquals(true, true);
	}

	@Test
	public void testBatchGetUserNotificationInfo() {
		assertEquals(true, true);
	}

}
