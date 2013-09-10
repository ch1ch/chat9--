package com.redatoms.chat.server.client;

import java.net.URI;
import java.net.URISyntaxException;
import java.nio.ByteBuffer;

import org.apache.log4j.Logger;
import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft;
import org.java_websocket.drafts.Draft_10;
import org.java_websocket.framing.Framedata;
import org.java_websocket.handshake.ServerHandshake;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.ContentType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message.MessageType;

/** This example demonstrates how to create a websocket connection to a server. Only the most important callbacks are overloaded. */
public class ExampleClient extends WebSocketClient {
	
	private static final Logger logger = Logger.getLogger(ExampleClient.class);
	
	private long messageCount = 0;
	
	private long userId;
	
	private boolean keepSending = false;
	
	public ExampleClient(URI serverUri , Draft draft, long userId, boolean keepSending) {
		super( serverUri, draft );
		this.userId = userId;
		this.keepSending = keepSending;
	}

	public ExampleClient( URI serverUri , Draft draft ) {
		super( serverUri, draft );
	}

	public ExampleClient( URI serverURI ) {
		super( serverURI );
	}

	@Override
	public void onOpen( ServerHandshake handshakedata ) {
		System.out.println( "opened connection" );
		// if you plan to refuse connection based on ip or httpfields overload: onWebsocketHandshakeReceivedAsClient
		sendUserId();
		if (keepSending) {
			new StatisticsWorker().start();
		}
	}
	
	private long getUserId() {
		return userId;
	}
	
	private void sendUserId() {
		ProtoMeta.DataPackage.Data data = ProtoMeta.DataPackage.Data.newBuilder().setUserId(getUserId()).build();
		ProtoMeta.DataPackage dataPackage = ProtoMeta.DataPackage.newBuilder().setSeqNo(10).setVersion(10).setCmd(CmdType.NOTIFY).setData(data).build();
		send(dataPackage.toByteArray());
	}
	
	private void sendMessage(String text) {
		ProtoMeta.DataPackage.Message message = ProtoMeta.DataPackage.Message.newBuilder().setFrom(getUserId()).setTo(getUserId()).setMsgType(MessageType.PERSON_TO_PERSON).setContentType(ContentType.TEXT).setContent(text).build();
		ProtoMeta.DataPackage.Data data = ProtoMeta.DataPackage.Data.newBuilder().setUserId(getUserId()).addMessage(message).build();
		ProtoMeta.DataPackage dataPackage = ProtoMeta.DataPackage.newBuilder().setSeqNo(10).setVersion(10).setCmd(CmdType.SEND).setData(data).build();
		send(dataPackage.toByteArray());
		messageCount++;
	}
	
	private void sync() {
		ProtoMeta.DataPackage.Data data = ProtoMeta.DataPackage.Data.newBuilder().setUserId(getUserId()).build();
		ProtoMeta.DataPackage dataPackage = ProtoMeta.DataPackage.newBuilder().setSeqNo(10).setVersion(10).setCmd(CmdType.SYNC).setData(data).build();
		send(dataPackage.toByteArray());
	}

	@Override
	public void onMessage( String message ) {
		System.out.println( "received: " + message );
	}
	
	@Override
	public void onMessage( ByteBuffer blob ) {
		if (blob.hasArray()) {
			try {
				ProtoMeta.DataPackage dataPackage = ProtoMeta.DataPackage.parseFrom(blob.array());
				onMessage(dataPackage);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			System.out.println("empty blob");
		}
	}
	
	private void onMessage(DataPackage dataPackage) {
		logger.debug(dataPackage);
		CmdType cmd = dataPackage.getCmd();
		switch (cmd) {
		case ACK:
			break;
		case NOTIFY:
			sync();
			break;
		case RECIEVE:
//			sendMessage("Hello world!");
			break;
		default:
			break;
		}
	}

//	@Override
	public void onFragment( Framedata fragment ) {
		System.out.println( "received fragment: " + new String( fragment.getPayloadData().array() ) );
	}

	@Override
	public void onClose( int code, String reason, boolean remote ) {
		// The codecodes are documented in class org.java_websocket.framing.CloseFrame
		System.out.println( "Connection closed by " + ( remote ? "remote peer" : "us" ) );
	}

	@Override
	public void onError( Exception ex ) {
		ex.printStackTrace();
		// if the error is fatal then onClose will be called additionally
	}

	public static void main( String[] args ) throws URISyntaxException {
		ExampleClient c;
		URI uri = new URI("ws://localhost:8080/websocket");
		int active = 1;
		int inactive = 0;
		
		if (args.length > 0) {
			uri = new URI(args[0]);
		}
		if (args.length > 1) {
			active = Integer.valueOf(args[1]);
		}
		if (args.length > 2) {
			inactive = Integer.valueOf(args[2]);
		}
		for (int i = 1; i <= active; i++) {
//			c = new ExampleClient( new URI( "ws://192.168.8.28:8080/websocket" ), new Draft_10(), i ); 
			c = new ExampleClient(uri, new Draft_10(), i, true); 
			c.connect();
		}
		for (int i = 1; i <= inactive; i++) {
//			c = new ExampleClient( new URI( "ws://192.168.8.28:8080/websocket" ), new Draft_10(), i ); 
			c = new ExampleClient(uri, new Draft_10(), active + i, false); 
			c.connect();
		}

	}
	
	private class StatisticsWorker extends Thread {
		
		private long sec = 0;
		
		public void run() {
			try {
				while (keepSending) {
//					System.out.println(messageCount + " message sent in " + sec + " secends.");
//					sleep(1000);
//					sec++;
					sleep(500);
					sendMessage("Hello jingcai!");
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}