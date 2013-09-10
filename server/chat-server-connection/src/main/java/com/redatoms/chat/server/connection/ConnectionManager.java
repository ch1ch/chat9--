package com.redatoms.chat.server.connection;

import io.netty.channel.Channel;

import java.util.HashMap;
import java.util.Map;

import org.apache.log4j.Logger;

public class ConnectionManager {
	
	private static final Logger logger = Logger.getLogger(ConnectionManager.class);

	private static final Map<String, Channel> connections = new HashMap<String, Channel>();
	
	private static final Map<Channel, String> users = new HashMap<Channel, String>();
	
	//此方法有线程安全问题，考虑到单个用户不会同时调用这个方法，为了不影响性能，暂不加Synchronize
	public static Channel addConnection(String userId, Channel conn) {
		if (userId == null || conn == null) {
			return null;
		}
		Channel c = connections.put(userId, conn);
		users.put(conn, userId);
		if (logger.isDebugEnabled()) {
			logger.debug("User " + userId + " connected.");
			logger.debug(connections.size() + " connections are active.");
		}
		return c;
	}
	
	public static Channel getConnection(String userId) {
		return connections.get(userId);
	}
	
	//此方法有线程安全问题，考虑到单个用户不会同时调用这个方法，为了不影响性能，暂不加Synchronize
	public static Channel removeConnection(String userId) {
		if (userId == null) {
			return null;
		}
		Channel c = connections.remove(userId);
		if (c == null) {
			return null;
		}
		users.remove(c);
		if (logger.isDebugEnabled()) {
			logger.debug("User " + userId + " disconnected.");
			logger.debug(connections.size() + " connections are active.");
		}
		return c;
	}
	
	//此方法有线程安全问题，考虑到单个用户不会同时调用这个方法，为了不影响性能，暂不加Synchronize
	public static Channel removeConnection(Channel conn) {
		if (conn == null) {
			return null;
		}
		String userId = users.remove(conn);
		if (userId == null) {
			return null;
		}
		if (logger.isDebugEnabled()) {
			logger.debug("User " + userId + " disconnected.");
			logger.debug(connections.size() + " connections are active.");
		}
		return connections.remove(userId);
	}
	
//	public static Collection<Channel> listAllConnections() {
//		return connections.values();
//	}
	
	public static int poolSize() {
		if (logger.isDebugEnabled()) {
			logger.debug("pool size: " + connections.size());
		}
		return connections.size();
	}
	
	public static void clear() {
		connections.clear();
		users.clear();
	}
}
