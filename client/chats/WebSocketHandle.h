//
//  WebSocketHandle.h
//  chats
//
//  Created by 廖丹 on 13-8-23.
//
//

#ifndef __chats__WebSocketHandle__
#define __chats__WebSocketHandle__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "WebSocket.h"
#include "ProtoMeta.pb.h"

#ifndef namelong
#define namelong 100
#endif
#ifndef DbName
#define DbName "db1.db"
#endif


#define LAYER_TAG_GROUPUSER		100
#define LAYER_TAG_FRIENDHOME	101

extern int toID;

class WebsocketHandle
: public cocos2d::CCLayer
, public cocos2d::extension::WebSocket::Delegate

{
private:
	int _requestNum;
	
public:
    
    std::string dbFilename;
    WebsocketHandle();
    WebsocketHandle(void *messagelayer);
    virtual ~WebsocketHandle();
    
    virtual void onOpen(cocos2d::extension::WebSocket* ws);//做初始化
    virtual void onClose(cocos2d::extension::WebSocket* ws);
    virtual void onError(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::ErrorCode& error);
    
    virtual void onMessage(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::Data& data);//接受消息
    virtual void sendMessage(int cmdType, int userIDTo, int messageType, int contentType, const char*messageContext);//发送消息
    
public:
    cocos2d::extension::WebSocket* _wsiSendBinary;
    void *messagelayer;
	
	int generateSquenceNumber(int cmdType);
	void unpackageSquenceNumber(int squenceNum, int *cmdType, int *requestNum);
	bool analysisResponse(int cmdType, proto::DataPackage package);
   // int owerID;

};

#endif