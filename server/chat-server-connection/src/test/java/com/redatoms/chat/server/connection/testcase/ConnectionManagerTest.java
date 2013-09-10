package com.redatoms.chat.server.connection.testcase;

import io.netty.channel.Channel;
import junit.framework.TestCase;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.redatoms.chat.server.connection.ConnectionManager;
import com.redatoms.chat.server.connection.mockObject.MockChannel;


public class ConnectionManagerTest extends TestCase {
	
	@Before
	public void setUp() throws Exception {
		ConnectionManager.clear();
	}
	
	@After
	public void tearDown() throws Exception {
		
	}
	
	@Test
	public void testAddConnection() throws Exception {
		String userId = String.valueOf(System.currentTimeMillis());
		Channel conn = new MockChannel();
		assertNull(ConnectionManager.addConnection(userId, conn));
		assertEquals(conn, ConnectionManager.addConnection(userId, new MockChannel()));
	}
	
	@Test
	public void testGetConnection() throws Exception {
		String userId = String.valueOf(System.currentTimeMillis());
		Channel conn = new MockChannel();
		ConnectionManager.addConnection(userId, conn);
		assertEquals(conn, ConnectionManager.getConnection(userId));
	}
	
	@Test
	public void testRemoveConnection() throws Exception {
		String userId = String.valueOf(System.currentTimeMillis());
		Channel conn = new MockChannel();
		ConnectionManager.addConnection(userId, conn);
		assertEquals(conn, ConnectionManager.removeConnection(userId));
		assertNull(ConnectionManager.getConnection(userId));
		assertNull(ConnectionManager.removeConnection(userId));
	}

}
