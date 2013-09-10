package com.redatoms.chat.server.common.proxy;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.log4j.Logger;

import redis.clients.jedis.Response;
import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;

import com.google.protobuf.Descriptors.FieldDescriptor;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Data;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Group;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.User;
import com.redatoms.chat.server.common.redisBase.redisBase;
import com.redatoms.chat.server.common.util.Constant;

public class RedisProxy implements PersistentProxy {

	private static final Logger logger = Logger.getLogger(RedisProxy.class);

	private static PersistentProxy instance;

	private static ThreadLocal<ShardedJedis> cfgRedisLocal = new ThreadLocal<ShardedJedis>() {
		protected synchronized ShardedJedis initialValue() {
			return redisBase.getCfgRedis();
		}
	};
	private static ThreadLocal<ShardedJedis> msgRedisLocal = new ThreadLocal<ShardedJedis>() {
		protected synchronized ShardedJedis initialValue() {
			return redisBase.getMsgRedis();
		}
	};

	public static ShardedJedis getCfgRedis() {
		return cfgRedisLocal.get();
	}

	public static ShardedJedis getMsgRedis() {
		return msgRedisLocal.get();
	}

	/*
	 * private Map<String, Jedis> redisMap; // private Map<String, String>
	 * gotMsgVer; private Jedis cfgConn; private Jedis msgConn;
	 */

	// init redis connection
	private RedisProxy() {
		/*
		 * redisMap = new HashMap<String, Jedis>();
		 * redisMap.put(Constant.REDIS_CONFIG_CONN, new Jedis(
		 * Constant.REDIS_CONFIG_HOST, Constant.REDIS_CONFIG_PORT));
		 * redisMap.put(Constant.REDIS_MSG_CONN, new Jedis(
		 * Constant.REDIS_MSG_HOST, Constant.REDIS_MSG_PORT));
		 */
	}

	public static PersistentProxy getInstance() {
		if (instance == null) {
			instance = new RedisProxy();
		}
		return instance;
	}

	// 初始化user profile and msg last version and latest version when init user
	public Boolean initUser(User user) {
		ShardedJedis redis = getCfgRedis();
		try {
			Long userId = user.getUserId();
			Boolean profile = redisBase.checkKeyExists(redis,
					Constant.USER_PROFILE + userId);
			if (!profile) {
				Map<String, String> userProfile = new HashMap<String, String>();
				userProfile.put("userId", String.valueOf(userId));
				userProfile.put("userName", String.valueOf(user.getUserName()));
				redisBase.setHashMap(redis, Constant.USER_PROFILE + userId,
						userProfile);
			}
			Boolean lastVer = redisBase.checkKeyExists(redis,
					Constant.LAST_MSG_VER + userId);
			if (!lastVer) {
				redisBase.setHashField(redis, Constant.LAST_MSG_VER + userId,
						Constant.MSG_P2P_VER + userId, "0");
				redisBase.setString(redis, Constant.MSG_P2P_VER + userId, "0");
			}
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);
		}
		return true;
	}

	// get last version
	private Map<String, String> getLastVerMap(ShardedJedis cfgConn, long userId) {
		// TODO Auto-generated method stub
		Map<String, String> LastMsgVer = null;
		LastMsgVer = redisBase.getHashAll(cfgConn, Constant.LAST_MSG_VER
				+ userId);
		// redisMap.get(Constant.REDIS_CONFIG_CONN).hgetAll(Constant.LAST_MSG_VER
		// + userId);

		// 如果为空的时候，对player进行初始化
		// 如果为空进行初始化，以后可能删除掉
		if (LastMsgVer.isEmpty()) {
			redisBase.setHashField(cfgConn, Constant.LAST_MSG_VER + userId,
					Constant.MSG_P2P_VER + userId, "0");
			LastMsgVer.put(Constant.MSG_P2P_VER + userId, "0");
		}
		return LastMsgVer;
	}

	// 获取p2p message
	private List<Message> listP2PMessage(ShardedJedis msgConn, long userId,
			double minVersion, double maxVersion) {
		List<Message> list = new ArrayList<Message>();
		//

		Set<String> set = null;

		set = msgConn.zrangeByScore(Constant.MSG_P2P_CONTENT + userId,
				minVersion, maxVersion);

		logger.debug("minVersion: " + minVersion + ", maxVersion: "
				+ maxVersion + "msg: " + set);

		Message message;
		try {
			for (Iterator<String> iterator = set.iterator(); iterator.hasNext();) {
				byte[] bs = iterator.next().getBytes("ISO-8859-1");
				message = Message.parseFrom(bs);
				list.add(message);
			}
		} catch (Exception e) {

		}
		//
		return list;
	}

	// 获取group message
	private List<Message> listGroupMessage(ShardedJedis msgConn, long userId,
			String groupId, double minVersion, double maxVersion) {
		List<Message> list = new ArrayList<Message>();
		Set<String> set = null;
		//

		set = msgConn.zrangeByScore(Constant.MSG_GROUP_CONTENT + groupId,
				minVersion, maxVersion);
		Message message;
		try {
			for (Iterator<String> iterator = set.iterator(); iterator.hasNext();) {
				byte[] bs = iterator.next().getBytes("ISO-8859-1");
				message = Message.parseFrom(bs);
				if (message.getFrom() != userId) {
					list.add(message);
				}

			}
		} catch (Exception e) {

		}
		//
		return list;
	}

	// 获取channel message
	private List<Message> getChannelMessage(ShardedJedis msgConn, long userId,
			String channelId, double minVersion, double maxVersion) {
		List<Message> list = new ArrayList<Message>();
		Set<String> set = msgConn.zrangeByScore(Constant.MSG_CHANNEL_CONTENT
				+ channelId, minVersion, maxVersion);
		Message message;
		try {
			for (Iterator<String> iterator = set.iterator(); iterator.hasNext();) {
				byte[] bs = iterator.next().getBytes("ISO-8859-1");

				message = Message.parseFrom(bs);
				if (message.getFrom() != userId) {
					list.add(message);
				}
				// list.add(message);
			}
		} catch (Exception e) {

		}
		//
		return list;
	}

	// get all message
	public List<Message> getAllMessage(long userId) {

		// get redis connection from pool
		ShardedJedis cfgConn = getCfgRedis();
		ShardedJedis msgConn = getMsgRedis();
		Map<String, String> lastVerMap = null;
		Map<String, String> latestVerMap = null;
		List<Message> allMessage = new ArrayList<Message>();
		try {
			lastVerMap = getLastVerMap(cfgConn, userId);
			latestVerMap = getLatestVerMap(cfgConn, lastVerMap);

			if (logger.isDebugEnabled()) {
				logger.debug(lastVerMap);
				logger.debug(latestVerMap);
			}
			Iterator<Entry<String, String>> iter = latestVerMap.entrySet()
					.iterator();

			while (iter.hasNext()) {
				Map.Entry<String, String> entry = (Entry<String, String>) iter
						.next();
				String key = entry.getKey();
				if (key.contains(Constant.MSG_P2P_VER)) {
					// 获取psp latest version
					String msgP2PLatestVer = entry.getValue();
					// 如果为空进行初始化，以后可能删除掉
					if (msgP2PLatestVer == null) {
						msgP2PLatestVer = "0";
					}
					// get p2p message
					allMessage.addAll(listP2PMessage(msgConn, userId,
							Double.valueOf(lastVerMap.get(key)) + 1,
							Double.valueOf(msgP2PLatestVer)));
				}
				if (key.contains(Constant.MSG_GROUP_VER)) {
					// 获取group key
					String groupId = key.replaceAll(Constant.MSG_GROUP_VER, "");
					// String groupKey = key.split("_")[1];
					// 获取group latest version
					String msgGroupLatestVer = entry.getValue();

					// 如果为空进行初始化，以后可能删除掉
					if (msgGroupLatestVer == null) {
						msgGroupLatestVer = "0";
					}
					// get group message
					allMessage.addAll(listGroupMessage(msgConn, userId,
							groupId, Double.valueOf(lastVerMap.get(key)) + 1,
							Double.valueOf(msgGroupLatestVer)));
				}
				if (key.contains(Constant.MSG_CHANNEL_VER)) {
					// 获取channel key
					String channelId = key.replaceAll(Constant.MSG_CHANNEL_VER,
							"");

					// String channelID = key.split("_")[1];
					// 获取group latest version
					String msgChannelLatestVer = entry.getValue();

					// 如果为空进行初始化，以后可能删除掉
					if (msgChannelLatestVer == null) {
						msgChannelLatestVer = "0";
					}
					// get channel message
					allMessage.addAll(listGroupMessage(msgConn, userId,
							channelId, Double.valueOf(lastVerMap.get(key)) + 1,
							Double.valueOf(msgChannelLatestVer)));
				}

			}
			if (logger.isDebugEnabled()) {
				logger.debug(userId + ": " + latestVerMap);
			}
			if (!latestVerMap.isEmpty()) {
				if (logger.isDebugEnabled()) {
					logger.debug("update latestVerMap");
				}
				updateLastVerMap(cfgConn, userId, latestVerMap);
			}
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(cfgConn);
			redisBase.returnBadMsgRedis(msgConn);
			logger.error(e);
		}
		return allMessage;

	}

	// get all message
	public List<DataPackage> batchGetAllMessage(
			List<DataPackage> dataPackageList) {
		// get redis connection from pool
		if (dataPackageList == null) {
			return null;
		}
		List<Response<Map<String, String>>> lastVerMapResponse = new ArrayList<Response<Map<String, String>>>();
		List<Map<String, Response<String>>> latestVerMapResponse = new ArrayList<Map<String, Response<String>>>();
		List<Map<String, String>> lastVerMap = new ArrayList<Map<String, String>>();
		List<Map<String, String>> latestVerMap = new ArrayList<Map<String, String>>();
		// 取上次版本号
		long userId;
		ShardedJedis redisConfig = null;

		try {
			redisConfig = getCfgRedis();
			ShardedJedisPipeline cp = redisConfig.pipelined();
			for (int i = 0; i < dataPackageList.size(); i++) {
				userId = dataPackageList.get(i).getData().getUserId();
				// get last message version map
				lastVerMapResponse.add(i,
						cp.hgetAll(Constant.LAST_MSG_VER + userId));
			}
			cp.sync();
			//
			Map<String, String> tempLastVer = null;
			Map<String, Response<String>> tempResponse = new HashMap<String, Response<String>>();

			for (int i = 0; i < dataPackageList.size(); i++) {
				// get last message version map
				tempLastVer = lastVerMapResponse.get(i).get();
				lastVerMap.add(i, tempLastVer);
				Iterator<Entry<String, String>> iter = tempLastVer.entrySet()
						.iterator();
				tempResponse.clear();
				while (iter.hasNext()) {
					Map.Entry<String, String> entry = iter.next();
					String key = entry.getKey();
					// Map<String, Response<String>> element = new
					// HashMap<String,
					// Response<String>>();
					tempResponse.put(key, cp.get(key));
				}
				latestVerMapResponse.add(i, tempResponse);
				tempLastVer = null;
			}
			cp.sync();
		} catch (Exception e) {
			if (redisConfig != null)
				redisBase.returnBadCfgRedis(redisConfig);
		}
		if (logger.isDebugEnabled()) {
			logger.debug("lastVerMap:" + lastVerMap);
		}
		//
		String newVer = null;
		String key = null;
		Map<String, String> tmpMap = new HashMap<String, String>();

		for (int i = 0; i < dataPackageList.size(); i++) {
			Iterator<Entry<String, String>> iter = lastVerMap.get(i).entrySet()
					.iterator();
			tmpMap.clear();
			while (iter.hasNext()) {
				Map.Entry<String, String> entry = iter.next();
				key = entry.getKey();
				newVer = latestVerMapResponse.get(i).get(key).get();
				if (newVer == null)
					newVer = "0";
				if (Double.valueOf(newVer) > Double.valueOf(entry.getValue())) {
					tmpMap.put(key, newVer);
				}

			}
			latestVerMap.add(i, tmpMap);
		}
		if (logger.isDebugEnabled()) {
			logger.debug("latestVerMap:" + latestVerMap);
		}

		// get latest message version map

		String msgP2PLatestVer = null;
		String msgGroupLatestVer = null;
		String msgChannelLatestVer = null;
		String channelId = null;
		String groupId = null;
		List<List<Response<Set<String>>>> allMessageResp = new ArrayList<List<Response<Set<String>>>>();
		List<Response<Set<String>>> allMessageTmp = new ArrayList<Response<Set<String>>>();

		ShardedJedis redisMsg = null;
		try {
			redisMsg = getMsgRedis();
			ShardedJedisPipeline mp = redisMsg.pipelined();
			allMessageTmp.clear();
			for (int i = 0; i < dataPackageList.size(); i++) {
				userId = dataPackageList.get(i).getData().getUserId();

				Iterator<Entry<String, String>> iter = latestVerMap.get(i)
						.entrySet().iterator();

				while (iter.hasNext()) {
					Map.Entry<String, String> entry = (Entry<String, String>) iter
							.next();
					key = entry.getKey();
					if (key.contains(Constant.MSG_P2P_VER)) {
						// 获取psp latest version
						msgP2PLatestVer = entry.getValue();
						// 如果为空进行初始化，以后可能删除掉
						if (msgP2PLatestVer == null) {
							msgP2PLatestVer = "0";
						}
						if (logger.isDebugEnabled()) {
							logger.debug("msgP2PLastVer: " + lastVerMap.get(i).get(key) + ", msgP2PLatestVer:" + msgP2PLatestVer);
						}
						// get p2p message
						allMessageTmp.add(mp.zrangeByScore(
								Constant.MSG_P2P_CONTENT + userId,
								Double.valueOf(lastVerMap.get(i).get(key) + 1),
								Double.valueOf(msgP2PLatestVer)));
					}
					if (key.contains(Constant.MSG_GROUP_VER)) {
						// 获取group key
						groupId = key.replaceAll(Constant.MSG_GROUP_VER, "");
						// 获取group latest version
						msgGroupLatestVer = entry.getValue();

						// 如果为空进行初始化，以后可能删除掉
						if (msgGroupLatestVer == null) {
							msgGroupLatestVer = "0";
						}
						// get group message
						allMessageTmp.add(mp.zrangeByScore(
								Constant.MSG_GROUP_CONTENT + groupId,
								Double.valueOf(lastVerMap.get(i).get(key) + 1),
								Double.valueOf(msgGroupLatestVer)));
					}
					if (key.contains(Constant.MSG_CHANNEL_VER)) {
						// 获取channel key
						channelId = key
								.replaceAll(Constant.MSG_CHANNEL_VER, "");
						// 获取group latest version
						msgChannelLatestVer = entry.getValue();

						// 如果为空进行初始化，以后可能删除掉
						if (msgChannelLatestVer == null) {
							msgChannelLatestVer = "0";
						}
						// get channel message
						allMessageTmp.add(mp.zrangeByScore(
								Constant.MSG_CHANNEL_CONTENT + channelId,
								Double.valueOf(lastVerMap.get(i).get(key) + 1),
								Double.valueOf(msgChannelLatestVer)));
					}
					allMessageResp.set(i, allMessageTmp);

				}
			}
			mp.sync();
		} catch (Exception e) {
			if (redisMsg != null)
				redisBase.returnBadMsgRedis(redisMsg);
		}
		Set<String> msgSet;
		Message message;
		Data data = null;
		for (int i = 0; i < dataPackageList.size(); i++) {
			userId = dataPackageList.get(i).getData().getUserId();
			data = dataPackageList.get(i).getData();
			for (int j = 0; j < allMessageResp.get(i).size(); j++) {
				msgSet = allMessageResp.get(i).get(j).get();
				try {
					for (Iterator<String> iterator = msgSet.iterator(); iterator
							.hasNext();) {
						byte[] bs = iterator.next().getBytes("ISO-8859-1");
						message = Message.parseFrom(bs);
						if (message.getMsgType().equals(
								Message.MessageType.GROUP)
								&& message.getTo() == userId) {
							continue;
						} else {
							data = data.toBuilder().addMessage(message).build();
						}
						message = null;
					}
				} catch (Exception e) {

				}

			}
			dataPackageList.set(i,
					dataPackageList.get(i).toBuilder().setData(data).build());
		}
		if (logger.isDebugEnabled()) {
			logger.debug(dataPackageList);
		}
		return dataPackageList;
	}

	// update last version
	private Boolean updateLastVerMap(ShardedJedis jedis, long userId,
			Map<String, String> latestVer) {
		// ShardedJedis client = redisBase.getCfgRedis();
		redisBase.setHashMap(jedis, Constant.LAST_MSG_VER + userId, latestVer);
		// redisBase.returnCfgRedis(client);
		return true;
	}

	// get latest version
	private Map<String, String> getLatestVerMap(ShardedJedis cfgConn,
			Map<String, String> LastMsgVer) {
		// Map<String, String> LatestMsgVer = LastMsgVer;
		Map<String, String> LatestMsgVer = new HashMap<String, String>();
		Iterator<Entry<String, String>> iter = LastMsgVer.entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry<String, String> entry = (Map.Entry<String, String>) iter
					.next();
			String key = entry.getKey();
			String newVer = redisBase.getString(cfgConn, key);
			if (logger.isDebugEnabled()) {
				logger.debug("key: " + key + ", newVer: " + newVer
						+ ", lastVer:" + entry.getValue());
			}
			// 测试使用，删除掉
			if (newVer == null)
				newVer = "0";
			if (Double.valueOf(newVer) > Double.valueOf(entry.getValue())) {
				LatestMsgVer.put(key, newVer);
			}
		}
		return LatestMsgVer;
	}

	// save message
	public Message saveMessage(Message msg) {
		// get redis connection from pool
		ShardedJedis cfgConn = redisBase.getCfgRedis();
		ShardedJedis msgConn = redisBase.getMsgRedis();
		long Toid = msg.getTo();
		String latestVerKeyPrefix = "";
		String msgVerKeyPrefix = "";
		switch (msg.getMsgType()) {
		case PERSON_TO_PERSON:
			latestVerKeyPrefix = Constant.MSG_P2P_VER;
			msgVerKeyPrefix = Constant.MSG_P2P_CONTENT;
			break;
		case GROUP:
			latestVerKeyPrefix = Constant.MSG_GROUP_VER;
			msgVerKeyPrefix = Constant.MSG_GROUP_CONTENT;

			break;
		case CHANNEL:
			latestVerKeyPrefix = Constant.MSG_CHANNEL_VER;
			msgVerKeyPrefix = Constant.MSG_CHANNEL_CONTENT;
			break;
		default:
			break;
		}
		// update message latest version
		long score = redisBase.getIncr(cfgConn, latestVerKeyPrefix + Toid);

		msg = msg.toBuilder().setCreateTime(System.currentTimeMillis()).build();

		// save message
		redisBase.setSortedSet(msgConn, msgVerKeyPrefix + Toid, score,
				msg.toByteArray());

		if (logger.isDebugEnabled()) {
			logger.debug("msg: "
					+ redisBase.getSortedSet(msgConn, msgVerKeyPrefix + Toid,
							0, 100));
		}

		redisBase.returnCfgRedis(cfgConn);
		redisBase.returnMsgRedis(msgConn);
		return msg;
	}

	public List<DataPackage> batchSaveMessage(List<DataPackage> dataPackageList) {
		if (dataPackageList == null) {
			return null;
		}

		List<Response<Long>> scoreList = new ArrayList<Response<Long>>();
		String latestVerKeyPrefix = "";
		String msgVerKeyPrefix = "";
		Message message;
		Data data;
		ShardedJedis redisConfig = null;
		try {
			redisConfig = getCfgRedis();
			ShardedJedisPipeline cp = redisConfig.pipelined();
			for (int i = 0; i < dataPackageList.size(); i++) {
				message = dataPackageList.get(i).getData().getMessage(0)
						.toBuilder().setCreateTime(System.currentTimeMillis())
						.build();
				data = dataPackageList.get(i).getData().toBuilder()
						.setMessage(0, message).build();
				dataPackageList.set(i, dataPackageList.get(i).toBuilder()
						.setData(data).build());
				switch (message.getMsgType()) {
				case PERSON_TO_PERSON:
					latestVerKeyPrefix = Constant.MSG_P2P_VER;
					break;
				case GROUP:
					latestVerKeyPrefix = Constant.MSG_GROUP_VER;

					break;
				case CHANNEL:
					latestVerKeyPrefix = Constant.MSG_CHANNEL_VER;
					break;
				default:
					break;
				}
				scoreList.add(i, cp.incr(latestVerKeyPrefix + message.getTo()));

			}
			cp.sync();
		} catch (Exception e) {
			if (redisConfig != null)
				redisBase.returnBadCfgRedis(redisConfig);
		}
		ShardedJedis redisMsg = null;
		try {
			redisMsg = getMsgRedis();
			ShardedJedisPipeline mp = redisMsg.pipelined();
			for (int i = 0; i < dataPackageList.size(); i++) {
				message = dataPackageList.get(i).getData().getMessage(0);
				switch (message.getMsgType()) {
				case PERSON_TO_PERSON:
					msgVerKeyPrefix = Constant.MSG_P2P_CONTENT;
					break;
				case GROUP:
					msgVerKeyPrefix = Constant.MSG_GROUP_CONTENT;
					break;
				case CHANNEL:
					msgVerKeyPrefix = Constant.MSG_CHANNEL_CONTENT;
					break;
				default:
					break;
				}
				mp.zadd(msgVerKeyPrefix + message.getTo(), scoreList.get(i)
						.get(), new String(message.toByteArray(), "ISO-8859-1"));
			}
			mp.sync();
		} catch (UnsupportedEncodingException e) {
			logger.error("failed to encode bytes", e);
			if (redisMsg != null)
				redisBase.returnBadMsgRedis(redisMsg);
		}

		return dataPackageList;
	}

	// 更新个人属性
	public Boolean saveUserProfile(User user) {
		ShardedJedis redis = getCfgRedis();
		Map<FieldDescriptor, Object> userMap = new HashMap<FieldDescriptor, Object>();
		Map<String, String> userMapNew = new HashMap<String, String>();
		userMap = user.getAllFields();
		Iterator<Entry<FieldDescriptor, Object>> iter = userMap.entrySet()
				.iterator();
		while (iter.hasNext()) {
			Map.Entry<FieldDescriptor, Object> entry = (Map.Entry<FieldDescriptor, Object>) iter
					.next();
			userMapNew.put(entry.getKey().getName(), entry.getValue()
					.toString());
		}

		long userId = user.getUserId();

		try {
			redisBase.setHashMap(redis, Constant.USER_PROFILE + userId,
					userMapNew);
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);
		}
		return true;
	}

	// 获取个人属性
	// key:userProfile_xxxuserIdxxx value: userId userName
	public User getUserProfile(long userId) {
		ShardedJedis redis = getCfgRedis();
		Map<String, String> userProfile = new HashMap<String, String>();
		try {
			userProfile = redisBase.getHashAll(redis, Constant.USER_PROFILE
					+ userId);
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);
		}
		if (!userProfile.isEmpty()) {
			User.Builder userBuilder = User.newBuilder().setUserId(userId)
					.setUserName(userProfile.get("userName"));
			return userBuilder.build();
		}

		return null;
	}

	// 邀请加好友
	public User inviteUserFriend(long userId, long friendId) {
		User user = getUserProfile(friendId);
		ShardedJedis redis = getCfgRedis();
		if (user != null) {
			try {
				redisBase.setSet(redis, Constant.USER_INVITE_FRIENDS + userId
						+ "_" + friendId, String.valueOf(friendId));

			} catch (Exception e) {
				redisBase.returnBadCfgRedis(redis);
			}
		}
		return user;
	}

	// 好友反馈是否同意
	public Boolean confirmUserFriend(long friendId, long userId, boolean agree) {
		if (agree) {
			ShardedJedis redis = getCfgRedis();
			try {
				redis.sadd(Constant.USER_FRIENDS + userId,
						String.valueOf(friendId));
				redis.del(Constant.USER_INVITE_FRIENDS + userId + "_"
						+ friendId);
			} catch (Exception e) {
				redisBase.returnBadCfgRedis(redis);
			}
			return true;
		}

		return false;
	}

	// 删除好友
	public Boolean delUserFriends(long userId, long friendId) {
		ShardedJedis redis = getCfgRedis();
		try {
			redisBase.setSet(redis, Constant.USER_DEL_FRIENDS + userId + "_"
					+ friendId, String.valueOf(friendId));

			Set<String> delSet = redisBase.getSetMembers(redis,
					Constant.USER_DEL_FRIENDS + userId + "_" + friendId);
			String[] strSet = new String[delSet.size()];
			delSet.toArray(strSet);
			redisBase.delSetMembers(redis, Constant.USER_FRIENDS + userId,
					strSet);

			redisBase.delKey(redis, Constant.USER_DEL_FRIENDS + userId + "_"
					+ friendId);
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);

		}
		return true;
	}

	// 获得friends list
	// 当有新的属性加入时，需要更新以下代码
	public List<User> getFriendsList(long userId) {
		List<User> userList = new ArrayList<User>();
		User.Builder user = User.newBuilder();
		Set<String> friendSet = new HashSet<String>();
		ShardedJedis redis = getCfgRedis();
		try {
			friendSet = redisBase.getSetMembers(redis, Constant.USER_FRIENDS
					+ userId);

			Iterator<String> it = friendSet.iterator();
			while (it.hasNext()) {
				String str = it.next();
				Map<String, String> userProfiles = redisBase.getHashAll(redis,
						Constant.USER_PROFILE + str);

				user.setUserId(Long.valueOf(userProfiles.get("userId")))
						.setUserName(userProfiles.get("userName"));
				userList.add(user.build());
			}
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);

		}
		return userList;
	}

	// 创建group
	public Group createGroup(long ownerId, List<User> members) {
		// 取得最新global group id
		ShardedJedis redis = getCfgRedis();
		/* long groupId = getUserLatestGroupId(); */
		long groupId = redisBase.getIncr(redis, Constant.GLOBAL_GROUP_SEQ);
		// add myself
		List<User> membersNew = new ArrayList<User>();
		membersNew.addAll(members);
		membersNew.add(getUserProfile(ownerId));
		String[] memberIds = new String[membersNew.size()];
		String initGroupName = "";
		User redisUser = null;

		int j = 0;
		for (Iterator<User> i = membersNew.iterator(); i.hasNext();) {
			User user = i.next();
			redisUser = getUserProfile(user.getUserId());
			memberIds[j] = String.valueOf(user.getUserId());
			if (!initGroupName.equals("")) {
				initGroupName = initGroupName + "," + redisUser.getUserName();
			} else {
				initGroupName = redisUser.getUserName();
			}
			j++;
			// 为每个member设定last version 信息，以便能取到该group消息
			String lastVerGroupFeild = Constant.MSG_GROUP_VER + groupId;

			// 添加人到group时，更新 msg last version group 信息
			redisBase.setHashField(redis,
					Constant.LAST_MSG_VER + user.getUserId(),
					lastVerGroupFeild, "0");
			/* saveLastVerGroupInfo(user.getUserId(), lastVerGroupFeild); */
		}

		// 设定group的属性
		Map<String, String> groupProfile = new HashMap<String, String>();
		groupProfile.put("groupId", String.valueOf(groupId));
		groupProfile.put("groupName", initGroupName);
		groupProfile.put("owner", String.valueOf(ownerId));
		try {
			// 创建group属性
			redisBase.setHashMap(redis, Constant.GROUP_PROFILE + groupId,
					groupProfile);

			// 添加group 成员
			redisBase
					.setSet(redis, Constant.GROUP_MEMBERS + groupId, memberIds);

		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);
		}
		Group.Builder groupBuild = Group.newBuilder().setGroupId(groupId)
				.setGroupName(initGroupName);

		return groupBuild.build();
	}

	// 解散组
	public Boolean delGroup(long ownerId, long groupId) {
		// 删除每个member设定last version 信息，以便能不再取到该group消息
		ShardedJedis redis = getCfgRedis();
		try {

			Set<String> groupSet = redisBase.getSetMembers(redis,
					Constant.GROUP_PROFILE + groupId);
			Iterator<String> it = groupSet.iterator();
			while (it.hasNext()) {
				String lastVerGroupFeild = Constant.MSG_GROUP_VER + groupId;
				// 添加人到group时，更新 msg last version group 信息
				redisBase.delHashField(redis, Constant.LAST_MSG_VER + ownerId,
						lastVerGroupFeild);
				// delLastVerGroupInfo(ownerId, lastVerGroupFeild);
			}
			// 删除组:userGroup_ownerId_groupId
			redisBase.delKey(redis, Constant.GROUP_PROFILE + groupId);
			redisBase.delKey(redis, Constant.GROUP_MEMBERS + groupId);

		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);

		}

		return true;
	}

	// 从群组中删除个人
	public Boolean delUserFromGroup(long ownerId, long groupId, long memberId) {
		// 删除每个member设定last version 信息，以便能不再取到该group消息
		String lastVerGroupFeild = Constant.MSG_GROUP_VER + groupId;
		ShardedJedis redis = getCfgRedis();
		try {
			// 添加人到group时，更新 msg last version group 信息
			redisBase.delHashField(redis, Constant.LAST_MSG_VER + memberId,
					lastVerGroupFeild);
			// delLastVerGroupInfo(memberId, lastVerGroupFeild);
			// 删除个人临时key userDelGroup_userId_groupId
			redisBase.setSet(redis, Constant.GROUP_MEMBER_DEL + groupId,
					String.valueOf(memberId));

			// group key: userGroup_userId_groupId
			Set<String> delSet = redisBase.getSetMembers(redis,
					Constant.GROUP_MEMBER_DEL + groupId);
			String[] strSet = new String[delSet.size()];
			delSet.toArray(strSet);
			redisBase.delSetMembers(redis, Constant.GROUP_MEMBERS + groupId,
					strSet);

			// 删除临时key: userDelGroup_userId_groupId
			redisBase.delKey(redis, Constant.GROUP_MEMBER_DEL + groupId);

		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);
		}
		return true;
	}

	// 获得group memebers list
	// 当有新的属性加入时，需要更新以下代码
	public List<User> getGroupMembersList(long ownerId, long groupID) {
		// TODO Auto-generated method stub
		List<User> userList = new ArrayList<User>();
		User.Builder user = User.newBuilder();
		ShardedJedis redis = getCfgRedis();
		try {

			Set<String> groupSet = redisBase.getSetMembers(redis,
					Constant.GROUP_MEMBERS + groupID);

			Iterator<String> it = groupSet.iterator();
			while (it.hasNext()) {
				String str = it.next();
				Map<String, String> userProfiles = redisBase.getHashAll(redis,
						Constant.USER_PROFILE + str);

				user.setUserId(Long.valueOf(userProfiles.get("userId")))
						.setUserName(userProfiles.get("userName"));
				userList.add(user.build());
			}
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);
		}
		return userList;
	}

	// 添加一个人到群组
	public Boolean addUserIntoGroup(long ownerId, long groupId, long memberId) {
		// TODO Auto-generated method stub
		// 删除每个member设定last version 信息，以便能不再取到该group消息
		String lastVerGroupFeild = Constant.MSG_GROUP_VER + groupId;
		ShardedJedis redis = getCfgRedis();
		ShardedJedisPipeline cp = getCfgRedis().pipelined();
		try {
			// 添加人到group时，更新 msg last version group 信息
			cp.hset(Constant.LAST_MSG_VER + memberId, lastVerGroupFeild, "0");
			/*
			 * redisBase.setHashField(redis, Constant.LAST_MSG_VER + memberId,
			 * lastVerGroupFeild, "0");
			 */
			cp.sadd(Constant.GROUP_MEMBERS + groupId, String.valueOf(memberId));

			/*
			 * redisBase.setSet(redis, Constant.GROUP_MEMBERS + groupId,
			 * String.valueOf(memberId));
			 */
			cp.sync();
		} catch (Exception e) {
			redisBase.returnBadCfgRedis(redis);

		}
		return true;
	}

	/*
	 * 设置user接入服务器记录
	 */
	public void setUserConnectionInfo(long userId, String serverId)
			throws Exception {
		ShardedJedis redis = getMsgRedis();
		try {
			redis.set(Constant.CONNECTON_INFO + userId, serverId);
		} catch (Exception e) {
			redisBase.returnBadMsgRedis(redis);
		}
	}

	/*
	 * 清除user接入服务器记录
	 */
	public void clearUserConnectionInfo(long userId) throws Exception {
		ShardedJedis redis = getMsgRedis();
		try {
			redis.del(Constant.CONNECTON_INFO + userId);
		} catch (Exception e) {
			redisBase.returnBadMsgRedis(redis);
		}
	}

	/*
	 * 清除user接入服务器记录
	 */
	public String getUserConnectionInfo(long userId) throws Exception {
		ShardedJedis redis = getMsgRedis();
		String connectionId = null;
		try {
			connectionId = redis.get(Constant.CONNECTON_INFO + userId);
		} catch (Exception e) {
			redisBase.returnBadMsgRedis(redis);
		}
		return connectionId;
	}

	/*
	 * 批量设置用户不可接受Notification(消息同步中)，过期时间暂定为30秒
	 */
	public void batchSetUserNotificationFlag(List<Long> userIdList)
			throws Exception {

		ShardedJedisPipeline mp = getMsgRedis().pipelined();
		for (int i = 0; i < userIdList.size(); i++) {
			mp.set(Constant.NOTIFICATION_FLAG + userIdList.get(i), "1");
			mp.expire(Constant.NOTIFICATION_FLAG + userIdList.get(i), 30);
		}
		mp.sync();
	}

	/*
	 * 批量设置用户可接受Notification(消息同步完毕)
	 */
	public void batchClearUserNotificationFlag(List<Long> userIdList)
			throws Exception {
		ShardedJedisPipeline mp = getMsgRedis().pipelined();
		for (int i = 0; i < userIdList.size(); i++) {
			mp.set(Constant.NOTIFICATION_FLAG + userIdList.get(i), "0");
		}
		mp.sync();
	}

	/*
	 * 批量查询用户Notification info
	 */
	public Map<Long, String> batchGetUserNotificationInfo(List<Long> userIdList)
			throws Exception {
		ShardedJedisPipeline mp = getMsgRedis().pipelined();
		Map<Long, Response<String>> userResponse = new HashMap<Long, Response<String>>();
		Map<Long, String> userNotify = new HashMap<Long, String>();
		for (int i = 0; i < userIdList.size(); i++) {
			userResponse.put(userIdList.get(i),
					mp.get(Constant.NOTIFICATION_FLAG + userIdList.get(i)));
		}
		mp.sync();
		long userId = 0;
		for (int i = 0; i < userIdList.size(); i++) {
			userId = userIdList.get(i);
			userNotify.put(userId, userResponse.get(userId).get());
		}
		return userNotify;
	}

}
