package com.redatoms.chat.server.client;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

public class RedisTest {
	
	private JedisPoolConfig config;
	private JedisPool pool;
	
	public RedisTest() {
		config = new JedisPoolConfig();
		config.setMaxActive(100);
		config.setMaxIdle(20);
		config.setMinIdle(10);
		config.setMaxWait(3000);
		pool = new JedisPool(config, "192.168.8.27", 6380);
	}
	
	public void test() {
		for (long i = 0; i < 64; i++) {
			new Worker(10000).start();
		}
		
		
	}

	public static void main(String[] args) {
		
		List<Integer> arrayList = new ArrayList<Integer>();
		
		long starttime = System.currentTimeMillis();
		
		for (int i = 0; i < 100000; i++) {
			arrayList.add(i);
		}
		arrayList.clear();
		
		long endtime = System.currentTimeMillis();
		
		System.out.println("finished in " + (endtime - starttime) + " ms");
		
		List<Integer> linkedList = new LinkedList<Integer>();
		
		starttime = System.currentTimeMillis();
		
		for (int i = 0; i < 100000; i++) {
			linkedList.add(i);
		}
		
		linkedList.clear();
		
		endtime = System.currentTimeMillis();
		
		System.out.println("finished in " + (endtime - starttime) + " ms");
		
//		new RedisTest().test();
//		
//		List<JedisShardInfo> hosts = new ArrayList<JedisShardInfo>();
//        //server1
//        JedisShardInfo host1 = new JedisShardInfo("192.168.8.27", 6379, 2000);
//        //server2
//        JedisShardInfo host2 = new JedisShardInfo("192.168.8.27", 6380, 2000);
//        hosts.add(host1);
//        hosts.add(host2);
//        ShardedJedis jedis = new ShardedJedis(hosts);
//        ShardedJedisPipeline p = jedis.pipelined();
//  
//		
//		Message message = Message.newBuilder().setContent("Hello World").setFrom(123).setTo(456).setMsgType(MessageType.PERSON_TO_PERSON).setContentType(ContentType.TEXT).setCreateTime(System.currentTimeMillis()).build();
//		Data data = Data.newBuilder().addMessage(message).build();
//		DataPackage dataPackage = DataPackage.newBuilder().setCmd(CmdType.ACK).setSeqNo(10).setVersion(20).setData(data).build();
//		
//		Gson gson = new Gson();
//		byte[] bytes = message.toByteArray();
//		System.out.println(gson.toJson(bytes));
		
//		bytebytes[] = new byte[] { 50, 0, -1, 28, -24 };
//
//
//
//		StringisoString = new String(bytes, "ISO-8859-1");
//
//		byte[] isoret = isoString.getBytes("ISO-8859-1");

		
//		System.out.println(gson.toJson(SafeEncoder.encode(bytes)));
//		System.out.println(bytes.toString());
//		try {
//			String str = new String(bytes, "ISO-8859-1");
////			p.set("TEST", str);
////			Response<String> response = p.get("TEST");
////			p.sync();
////			String str2 = response.get();
////			System.out.println(str);
////			System.out.println(str2);
////			System.out.println(gson.toJson(SafeEncoder.encode(SafeEncoder.encode(str))));
////			
//			System.out.println(message);
//			System.out.println(Message.parseFrom(SafeEncoder.encode(SafeEncoder.encode(str)).getBytes("ISO-8859-1")));
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
		
		
		
//		try {
//			System.out.println(gson.fromJson(str, Message.class));
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
//		String str = SafeEncoder.encode(dataPackage.toByteArray());
//		System.out.println(str);
		
//		System.out.println(dataPackage);
//		try {
//			System.out.println(Message.parseFrom(SafeEncoder.encode(str)));
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
        
	}
	
	private class Worker extends Thread {
		
		private long count = 0;
		
		public Worker(long count) {
			super();
			this.count = count;
		}
		
		public void run() {
			long starttime = System.currentTimeMillis();
			
			
			try {
				while ((count--) > 0) {
					Jedis jedis = pool.getResource();
//					jedis.set("TEST" + count, "Hello world");
					pool.returnResource(jedis);
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				
			}
			long endtime = System.currentTimeMillis();
			System.out.println("wrote " + count + " msg in " + (endtime - starttime) + " ms");
		}
	}

}
