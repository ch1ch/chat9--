package com.redatoms.chat.server.common.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;

import redis.clients.jedis.Jedis;

import com.redatoms.chat.server.common.ebo.BaseSortSetEbo;

public class EntityManager {
	
	private static final Logger log = Logger.getLogger(EntityManager.class);
	
	private static final String CONNECTION_1 = "connection_1";
	
	private static Map<String, Jedis> map = new HashMap<String, Jedis>();
	
	private static EntityManager instance = new EntityManager();
	
	private EntityManager() {
		super();
		map.put(CONNECTION_1, new Jedis("192.168.8.27", 6379));
	}
	
	public static EntityManager getInstance() {
		return instance;
	}
	
	public <T> T find(Class<T> clazz, Object key) throws Exception {
		T ebo = (T) clazz.newInstance(); //TODO
		return ebo;
	}
	
	public <T> List<T> findByVersion(Class<T> clazz, Object key, Object version) throws Exception {
		List<T> list = new ArrayList<T>();
		return list;
	}
	
	public <T> List<T> findAll(Class<T> clazz) throws Exception {
		List<T> list = new ArrayList<T>();
		return null;
	}
	
	public <T> T persist(T ebo) throws Exception {
//		Object key = ebo.getClass().getName() + ebo.getId();
//		map.get(CONNECTION_1).
		return ebo;
	}
	
	public <E extends BaseSortSetEbo> E persistVersion(E ebo) throws Exception {
		String key = ebo.getClass().getName() + ":" + ebo.getId();
		map.get(CONNECTION_1).zadd(key.getBytes(), ebo.getVersion(), ebo.toByteArray());
		return ebo;
	}
	
	public <T> T delete(T ebo) throws Exception {
		return ebo;
	}
	
	public <T> T deleteByKey(Object key) throws Exception {
		T ebo = (T) new Object();
		return ebo;
	}
	
	public void flush() throws Exception {
		
	}


}
