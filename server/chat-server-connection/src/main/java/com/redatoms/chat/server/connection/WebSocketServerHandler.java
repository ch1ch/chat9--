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

import static io.netty.handler.codec.http.HttpHeaders.isKeepAlive;
import static io.netty.handler.codec.http.HttpHeaders.setContentLength;
import static io.netty.handler.codec.http.HttpHeaders.Names.CONTENT_TYPE;
import static io.netty.handler.codec.http.HttpHeaders.Names.HOST;
import static io.netty.handler.codec.http.HttpMethod.GET;
import static io.netty.handler.codec.http.HttpResponseStatus.BAD_REQUEST;
import static io.netty.handler.codec.http.HttpResponseStatus.FORBIDDEN;
import static io.netty.handler.codec.http.HttpResponseStatus.NOT_FOUND;
import static io.netty.handler.codec.http.HttpResponseStatus.OK;
import static io.netty.handler.codec.http.HttpVersion.HTTP_1_1;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.DefaultFullHttpResponse;
import io.netty.handler.codec.http.FullHttpRequest;
import io.netty.handler.codec.http.FullHttpResponse;
import io.netty.handler.codec.http.websocketx.BinaryWebSocketFrame;
import io.netty.handler.codec.http.websocketx.CloseWebSocketFrame;
import io.netty.handler.codec.http.websocketx.PingWebSocketFrame;
import io.netty.handler.codec.http.websocketx.PongWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketServerHandshaker;
import io.netty.handler.codec.http.websocketx.WebSocketServerHandshakerFactory;
import io.netty.util.CharsetUtil;

import java.util.List;

import org.apache.log4j.Logger;

import com.redatoms.chat.server.common.data.ProtoMeta;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.CmdType;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.Message;
import com.redatoms.chat.server.common.data.ProtoMeta.DataPackage.User;
import com.redatoms.chat.server.common.proxy.PersistentProxy;
import com.redatoms.chat.server.common.proxy.RedisProxy;
import com.redatoms.chat.server.common.util.ProtoMetaUtil;


/**
 * Handles handshakes and messages
 */
public class WebSocketServerHandler extends SimpleChannelInboundHandler<Object> {
    private static final Logger logger = Logger.getLogger(WebSocketServerHandler.class);

    private static final String WEBSOCKET_PATH = "/websocket";

    private WebSocketServerHandshaker handshaker;
    
    private long userId;
    
//    static final ChannelGroup channels = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);
    
//    private static long starttime = 0;
//    
//    private static long endtime = 0;
//    
//    private static int connectCount = 0;
    
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
    	// TODO Auto-generated method stub
    	super.channelActive(ctx);
//    	if (connectCount == 0) {
//    		starttime = System.currentTimeMillis();
//    	}
//    	connectCount++;
//    	if (connectCount == 2980) {
//    		endtime = System.currentTimeMillis();
//    		logger.info("2980 connections created in " + (endtime - starttime) + "ms");
//    	}
//    	channels.add(ctx.channel());
//    	ConnectionManager.addConnection(ctx.channel().hashCode() + "", ctx.channel());
    }

    @Override
    public void channelRead0(ChannelHandlerContext ctx, Object msg) throws Exception {
        if (msg instanceof FullHttpRequest) {
            handleHttpRequest(ctx, (FullHttpRequest) msg);
        } else if (msg instanceof WebSocketFrame) {
            handleWebSocketFrame(ctx, (WebSocketFrame) msg);
        }
    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {
//        ctx.flush();
    }

    private void handleHttpRequest(ChannelHandlerContext ctx, FullHttpRequest req) throws Exception {
        // Handle a bad request.
        if (!req.getDecoderResult().isSuccess()) {
            sendHttpResponse(ctx, req, new DefaultFullHttpResponse(HTTP_1_1, BAD_REQUEST));
            return;
        }

        // Allow only GET methods.
        if (req.getMethod() != GET) {
            sendHttpResponse(ctx, req, new DefaultFullHttpResponse(HTTP_1_1, FORBIDDEN));
            return;
        }

        // Send the demo page and favicon.ico
        if ("/".equals(req.getUri())) {
            ByteBuf content = WebSocketServerIndexPage.getContent(getWebSocketLocation(req));
            FullHttpResponse res = new DefaultFullHttpResponse(HTTP_1_1, OK, content);

            res.headers().set(CONTENT_TYPE, "text/html; charset=UTF-8");
            setContentLength(res, content.readableBytes());

            sendHttpResponse(ctx, req, res);
            return;
        }
        if ("/favicon.ico".equals(req.getUri())) {
            FullHttpResponse res = new DefaultFullHttpResponse(HTTP_1_1, NOT_FOUND);
            sendHttpResponse(ctx, req, res);
            return;
        }

        // Handshake
        WebSocketServerHandshakerFactory wsFactory = new WebSocketServerHandshakerFactory(
                getWebSocketLocation(req), "*", false);
        handshaker = wsFactory.newHandshaker(req);
        if (handshaker == null) {
            WebSocketServerHandshakerFactory.sendUnsupportedWebSocketVersionResponse(ctx.channel());
        } else {
            handshaker.handshake(ctx.channel(), req);
        }
    }

    private void handleWebSocketFrame(ChannelHandlerContext ctx, WebSocketFrame frame) {

        // Check for closing frame
        if (frame instanceof CloseWebSocketFrame) {
            handshaker.close(ctx.channel(), (CloseWebSocketFrame) frame.retain());
            return;
        }
        if (frame instanceof PingWebSocketFrame) {
            ctx.channel().write(new PongWebSocketFrame(frame.content().retain()));
            return;
        }
        if (!(frame instanceof BinaryWebSocketFrame)) {
            throw new UnsupportedOperationException(String.format("%s frame types not supported", frame.getClass()
                    .getName()));
        }
        
		byte[] bytes = new byte[frame.content().readableBytes()];

		frame.content().readBytes(bytes);

		if (logger.isDebugEnabled()) {
			String str = "";
			for (int i = 0; i < bytes.length; i++) {
				str += bytes[i];
			}
    		logger.debug("data: " + str);
		}
		ProtoMeta.DataPackage dataPackage;
		try {
			dataPackage = ProtoMeta.DataPackage.parseFrom(bytes);
		} catch (Exception e) {
			logger.error("failed to parse data package");
			return;
		}
		if (logger.isDebugEnabled()) {
			logger.debug("Inbound Message: " + ProtoMetaUtil.print(dataPackage));
		}
		ProtoMeta.DataPackage.Data data = dataPackage.getData();
		
		CmdType cmd = dataPackage.getCmd();
		if (!CmdType.NOTIFY.equals(cmd)) {
			checkUser(dataPackage);
		}
		switch (cmd) {
		case SEND:
			//验证user发送的消息是否有效
			List<Message> list = data.getMessageList();
			if (list == null || list.size() != 1) {
				throw new UnsupportedOperationException("Invalid message");
			}
    		Message msg = list.get(0);
    		if (userId != msg.getFrom()) {
    			throw new UnsupportedOperationException("Invalid message");
    		}
    		WebSocketServer.inboundMsgMQ.putPackage(dataPackage);
    		break;
		case NOTIFY:	//TODO 添加用户验证机制
			userId = data.getUserId();
			ConnectionManager.addConnection(data.getUserId() + "", ctx.channel());
			RedisProxy.getInstance().initUser(User.newBuilder().setUserId(userId).setUserName(data.getUserName()).build());
			break;
		case SYNC:
			WebSocketServer.syncMQ.putPackage(dataPackage);
			break;
		case SEARCH_USER_BY_NAME:
		case SEARCH_USER_BY_ID:
		case DELETE_FRIEND:
		case ADD_FRIEND:
		case LIST_FRIENDS:
			WebSocketServer.friendsMQ.putPackage(dataPackage);
			break;
		case CREATE_GROUP:
		case DELETE_GROUP:
		case ADD_GROUP_MEMBERS:
		case UPDATE_GROUP:
		case DELETE_GROUP_MEMBERS:
			WebSocketServer.groupsMQ.putPackage(dataPackage);
			break;
		default:
			throw new UnsupportedOperationException(String.format("CmdType: %s not supported", cmd));
		}
    	
    }
    
    private void checkUser(DataPackage dataPackage) throws UnsupportedOperationException {
    	if (dataPackage == null || dataPackage.getData() == null || dataPackage.getData().getUserId() != userId) {
    		throw new UnsupportedOperationException("Invalid data package");
    	}
    }

    private static void sendHttpResponse(
            ChannelHandlerContext ctx, FullHttpRequest req, FullHttpResponse res) {
        // Generate an error page if response getStatus code is not OK (200).
        if (res.getStatus().code() != 200) {
            ByteBuf buf = Unpooled.copiedBuffer(res.getStatus().toString(), CharsetUtil.UTF_8);
            res.content().writeBytes(buf);
            buf.release();
            setContentLength(res, res.content().readableBytes());
        }

        // Send the response and close the connection if necessary.
        ChannelFuture f = ctx.channel().writeAndFlush(res);
        if (!isKeepAlive(req) || res.getStatus().code() != 200) {
            f.addListener(ChannelFutureListener.CLOSE);
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        cause.printStackTrace();
        ConnectionManager.removeConnection(ctx.channel());
        ctx.close();
    }
    
    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
    	ConnectionManager.removeConnection(ctx.channel());
    };

    private static String getWebSocketLocation(FullHttpRequest req) {
        return "ws://" + req.headers().get(HOST) + WEBSOCKET_PATH;
    }
}
