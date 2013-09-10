package com.redatoms.chat.server.common.redisBase;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.redatoms.chat.server.common.util.Constant;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.JedisShardInfo;
import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;
import redis.clients.util.Hashing;
import redis.clients.util.Sharded;

public class redisBase {

	public static ShardedJedisPool redisConfigPool;
	public static ShardedJedisPool redisMsgPool;
	static {
		JedisPoolConfig config = new JedisPoolConfig();
		// 链接池中最大连接数,默认为8.
		config.setMaxActive(64);
		// 链接池中最大空闲的连接数,默认为8.
		config.setMaxIdle(16);
		// 连接池中最少空闲的连接数,默认为0.
		config.setMinIdle(8);
		// 当连接池资源耗尽时，调用者最大阻塞的时间，超时将跑出异常。单位，毫秒数;默认为-1.表示永不超时.
		config.setMaxWait(3000);
		// 连接空闲的最小时间，达到此值后空闲连接将可能会被移除。负值(-1)表示不移除。
		config.setMinEvictableIdleTimeMillis(300000);
		// 连接空闲的最小时间，达到此值后空闲链接将会被移除，且保留“minIdle”个空闲连接数。默认为-1.
		config.setSoftMinEvictableIdleTimeMillis(-1);
		// 对于“空闲链接”检测线程而言，每次检测的链接资源的个数。默认为3.
		config.setNumTestsPerEvictionRun(3);
		// 向调用者输出“链接”资源时，是否检测是有有效，如果无效则从连接池中移除，并尝试获取继续获取。默认为false。建议保持默认值
		config.setTestOnBorrow(false);
		// 向连接池“归还”链接时，是否检测“链接”对象的有效性。默认为false。建议保持默认值.
		config.setTestOnReturn(false);
		// 向调用者输出“链接”对象时，是否检测它的空闲超时；默认为false。如果“链接”空闲超时，将会被移除。建议保持默认值.
		config.setTestWhileIdle(false);
		// “空闲链接”检测线程，检测的周期，毫秒数。如果为负值，表示不运行“检测线程”。默认为-1.
		config.setTimeBetweenEvictionRunsMillis(60000);// 一分钟
		// 当“连接池”中active数量达到阀值时，即“链接”资源耗尽时，连接池需要采取的手段, 默认为1：
		// -> 0 : 抛出异常，
		// -> 1 : 阻塞，直到有可用链接资源
		// -> 2 : 强制创建新的链接资源
		config.setWhenExhaustedAction((byte) 1);
		List<JedisShardInfo> jdsInfoListCfg = new ArrayList<JedisShardInfo>();
		List<JedisShardInfo> jdsInfoListMsg = new ArrayList<JedisShardInfo>();
		JedisShardInfo infoCfgA = new JedisShardInfo(
				Constant.REDIS_CONFIG_HOST, Constant.REDIS_CONFIG_PORT);
		JedisShardInfo infoMsgB = new JedisShardInfo(Constant.REDIS_MSG_HOST,
				Constant.REDIS_MSG_PORT);
		jdsInfoListCfg.add(infoCfgA);
		jdsInfoListMsg.add(infoMsgB);
		redisConfigPool = new ShardedJedisPool(config, jdsInfoListCfg,
				Hashing.MURMUR_HASH, Sharded.DEFAULT_KEY_TAG_PATTERN);
		redisMsgPool = new ShardedJedisPool(config, jdsInfoListMsg,
				Hashing.MURMUR_HASH, Sharded.DEFAULT_KEY_TAG_PATTERN);
		/*
		 * redisConfigPool = new JedisPool(config, Constant.REDIS_CONFIG_HOST,
		 * Constant.REDIS_CONFIG_PORT); redisMsgPool = new JedisPool(config,
		 * Constant.REDIS_MSG_HOST, Constant.REDIS_MSG_PORT);
		 */
	}

	// set hash table map
	public static String setHashMap(ShardedJedis redis, String key,
			Map<String, String> map) {
		return redis.hmset(key, map);
	}

	// set hash table single field
	public static Long setHashField(ShardedJedis redis, String key,
			String field, String value) {
		return redis.hset(key, field, value);
	}

	// get all hash table
	public static Map<String, String> getHashAll(ShardedJedis redis, String key) {
		return redis.hgetAll(key);
	}

	// get hash table single field
	public static String getHashField(ShardedJedis redis, String key,
			String field) {
		return redis.hget(key, field);
	}

	// delete hash table fields
	public static Long delHashField(ShardedJedis redis, String key,
			String... fields) {
		return redis.hdel(key, fields);
	}

	// set sorted set
	public static Long setSortedSet(ShardedJedis redis, String key,
			double score, byte[] value) {
		return redis.zadd(key.getBytes(), score, value);
	}

	// get sorted set
	public static Set<byte[]> getSortedSet(ShardedJedis redis, String key,
			double minScore, double maxScore) {
		return redis.zrangeByScore(key.getBytes(), minScore, maxScore);
	}

	// set String
	public static String setString(ShardedJedis redis, String key, String value) {
		return redis.set(key, value);
	}

	// get String
	public static String getString(ShardedJedis redis, String key) {
		return redis.get(key);
	}

	// get incr String
	public static Long getIncr(ShardedJedis redis, String key) {
		return redis.incr(key);
	}

	//
	// set set
	public static Long setSet(ShardedJedis redis, String key, String... value) {
		return redis.sadd(key, value);
	}

	// get set members
	public static Set<String> getSetMembers(ShardedJedis redis, String key) {
		return redis.smembers(key);
	}

	// del key
	public static Long delKey(ShardedJedis redis, String key) {
		return redis.del(key);
	}

	// del key
	public static Long delSetMembers(ShardedJedis redis, String key,
			String... members) {
		return redis.srem(key, members);
	}
	// check key
		public static Boolean checkKeyExists(ShardedJedis redis, String key) {
			return redis.exists(key);
		}

	// 从连接池获得 config redis 连接
	public static ShardedJedis getCfgRedis() {
		ShardedJedis client = redisConfigPool.getResource();// 从pool中获取资源
		return client;
	}

	// 从连接池获得msg redis 连接
	public static ShardedJedis getMsgRedis() {
		ShardedJedis client = redisMsgPool.getResource();// 从pool中获取资源
		return client;
	}

	// 归还msg redis 连接
	public static void returnMsgRedis(ShardedJedis client) {
		redisMsgPool.returnResource(client);// 向连接池“归还”资源
	}

	// 归还通信出错的 msg redis 连接
	public static void returnBadMsgRedis(ShardedJedis client) {
		redisMsgPool.returnBrokenResource(client);// 向连接池“归还”资源
	}

	// 归还config redis 连接
	public static void returnCfgRedis(ShardedJedis client) {
		redisConfigPool.returnResource(client);// 向连接池“归还”资源
	}

	// 归还通信出错的 config redis 连接
	public static void returnBadCfgRedis(ShardedJedis client) {
		redisConfigPool.returnBrokenResource(client);// 向连接池“归还”资源
	}

}
