/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.redatoms.chat.server.connection;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.Channel;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.mq.ConsumerPool;
import com.redatoms.chat.server.common.mq.DataPackageListener;
import com.redatoms.chat.server.common.mq.ProducerPool;
import com.redatoms.chat.server.common.util.Constant;



/**
 * A HTTP server which serves Web Socket requests at:
 *
 * http://localhost:8080/websocket
 *
 * Open your browser at http://localhost:8080/, then the demo page will be loaded and a Web Socket connection will be
 * made automatically.
 *
 * This server illustrates support for the different web socket specification versions and will work with:
 *
 * <ul>
 * <li>Safari 5+ (draft-ietf-hybi-thewebsocketprotocol-00)
 * <li>Chrome 6-13 (draft-ietf-hybi-thewebsocketprotocol-00)
 * <li>Chrome 14+ (draft-ietf-hybi-thewebsocketprotocol-10)
 * <li>Chrome 16+ (RFC 6455 aka draft-ietf-hybi-thewebsocketprotocol-17)
 * <li>Firefox 7+ (draft-ietf-hybi-thewebsocketprotocol-10)
 * <li>Firefox 11+ (RFC 6455 aka draft-ietf-hybi-thewebsocketprotocol-17)
 * </ul>
 */
public class WebSocketServer {
	
	private static final Logger logger = Logger.getLogger(WebSocketServer.class);

    private final int port;
    
    public static ProducerPool inboundMsgMQ;
    
    public static ProducerPool syncMQ;
    
    public static ProducerPool friendsMQ;
    
    public static ProducerPool groupsMQ;

    public WebSocketServer(int port) {
        this.port = port;
    }

    public void run() throws Exception {
        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap b = new ServerBootstrap();
            b.group(bossGroup, workerGroup)
             .channel(NioServerSocketChannel.class)
             .childHandler(new WebSocketServerInitializer());

            Channel ch = b.bind(port).sync().channel();
            logger.info("Web socket server started at port " + port + '.');
            logger.info("Open your browser and navigate to http://localhost:" + port + '/');
            ch.closeFuture().sync();
            logger.info("Web socket server stop. ");
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
        }
    }

    public static void main(String[] args) throws Exception {
        int port;
        if (args.length > 0) {
            port = Integer.parseInt(args[0]);
        } else {
            port = 8080;
        }
        start(port);
    }
    
    public static void start(int port) throws Exception {
    	inboundMsgMQ = new ProducerPool(Constant.MQ_INBOUND_MESSAGE);
    	syncMQ = new ProducerPool(Constant.MQ_SYNC_MESSAGE);
    	friendsMQ = new ProducerPool(Constant.MQ_FRIENDS_MESSAGE);
    	groupsMQ = new ProducerPool(Constant.MQ_GROUPS_MESSAGE);
    	DataPackageListener listener = new DPListener();
    	new ConsumerPool(Constant.MQ_OUTBOUND_MESSAGE, listener, "MQConfig.properties");
    	new WebSocketServer(port).run();
    	stop();
    }
    
    public static void stop() throws Exception {
    	inboundMsgMQ.closePool();
    	syncMQ.closePool();
    	friendsMQ.closePool();
    	groupsMQ.closePool();
    }

//	@Override
//	public void start(BundleContext context) throws Exception {
//		logger.debug("connection server started");
//    	inboundMsgMQ = new ProducerPool(Constant.MQ_INBOUND_MESSAGE);
//    	syncMQ = new ProducerPool(Constant.MQ_SYNC_MESSAGE);
//    	friendsMQ = new ProducerPool(Constant.MQ_FRIENDS_MESSAGE);
//    	groupsMQ = new ProducerPool(Constant.MQ_GROUPS_MESSAGE);
//    	DataPackageListener listener = new DPListener();
//    	new ConsumerPool(Constant.MQ_OUTBOUND_MESSAGE, listener);
//    	new WebSocketServer(8888).run();
//	}

//	@Override
//	public void stop(BundleContext context) throws Exception {
//		logger.debug("connection server stopped");
//    	inboundMsgMQ.closePool();
//    	syncMQ.closePool();
//    	friendsMQ.closePool();
//    	groupsMQ.closePool();
//	}
}
