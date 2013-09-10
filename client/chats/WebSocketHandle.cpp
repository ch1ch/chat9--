//
//  WebSocketHandle.cpp
//  chats
//
//  Created by 廖丹 on 13-8-23.
//
//

#include "WebSocketHandle.h"
#include "ProtoMeta.pb.h"
#include "MessageLayer.h"
#include "FriendsHomeLayer.h"
#include "MessageHandle.h"
#include "MessageHomeLayer.h"
#include "GroupUserListLayer.h"
#include "strutil.h"
#include "CommonDataManager.h"
#include "DatabaseManager.h"
#include "FriendModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define SERVER_ADDRESS	"ws://192.168.8.28:8080/websocket/"
#define REQUESTNUM_BASE		1000

WebsocketHandle::WebsocketHandle()
: _wsiSendBinary(NULL)
{
    //owerID = userID;
    
    _wsiSendBinary = new cocos2d::extension::WebSocket();
    
//	192.168.8.28
//	"ws://192.168.10.50:8080/websocket/"
    if (!_wsiSendBinary->init(*this, "ws://192.168.10.50:8080/websocket/"))
    {
        
        CC_SAFE_DELETE(_wsiSendBinary);
    }
    
}
WebsocketHandle::WebsocketHandle(void *messagelayer)
: _wsiSendBinary(NULL)
{

    
    _wsiSendBinary = new cocos2d::extension::WebSocket();
    messagelayer = (MessageLayer *)messagelayer;
    
    if (!_wsiSendBinary->init(*this, "ws://192.168.10.50:8080/websocket/"))
    {
        CC_SAFE_DELETE(_wsiSendBinary);
    }
}

WebsocketHandle::~WebsocketHandle()
{
    if (_wsiSendBinary)
        _wsiSendBinary->close();

}

// Delegate methods
void WebsocketHandle::onOpen(cocos2d::extension::WebSocket* ws)
{
	int ownid = CommonDataManager::getMyUserID();
	int squenceNum = generateSquenceNumber(proto::DataPackage_CmdType_NOTIFY);
    proto::DataPackage datapackage;
    datapackage.set_cmd(proto::DataPackage_CmdType_NOTIFY);
    proto::DataPackage_Data  data;
    datapackage.set_version(10);
    datapackage.set_seqno(squenceNum);
    data.set_userid(ownid);
    

    datapackage.set_allocated_data(&data);
        
    int size = datapackage.ByteSize();
    void * buf = malloc(size);
    datapackage.SerializeToArray(buf, size);
        
    _wsiSendBinary->send((Byte*)buf, size);
    //free(buf);

}

int WebsocketHandle::generateSquenceNumber(int cmdType)
{
	_requestNum ++;
    _requestNum = _requestNum % REQUESTNUM_BASE;
	return cmdType * REQUESTNUM_BASE + _requestNum;
}

void WebsocketHandle::unpackageSquenceNumber(int squenceNum, int *cmdType, int *requestNum)
{
	*requestNum = squenceNum % REQUESTNUM_BASE;
	*cmdType	= (squenceNum - *requestNum) / REQUESTNUM_BASE;
}

bool WebsocketHandle::analysisResponse(int cmdType, proto::DataPackage package)
{
	bool hasHandleMessage = false;
	CCScene *runscene = CCDirector::sharedDirector()->getRunningScene();
	proto::DataPackage_Data responseData = package.data();
	printf("message backed");

	switch (cmdType)
	{ 
		case proto::DataPackage_CmdType_CREATE_GROUP:
		{
			int groupSize = responseData.group_size();
			GroupUserListLayer *layer = (GroupUserListLayer *)runscene->getChildByTag(LAYER_TAG_GROUPUSER);
			if (groupSize == 1 && layer)
			{
				hasHandleMessage = true;
				
				proto::DataPackage_Group currentGroup = responseData.group(0);
				int groupID = currentGroup.groupid();
				std::string groupName = currentGroup.groupname();
				
				layer->onGroupCreateReceived(groupID, groupName);
			}
		}
			break;
		default:
			break;
	}
	
	return hasHandleMessage;
}

void WebsocketHandle::onMessage(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::Data& data)
{
    
    //char db[namelong]=DbName;
	int ownid = CommonDataManager::getMyUserID();
    char db[namelong];
    sprintf(db, "db%d.db",ownid);
    dbFilename=cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+db;

    CCScene *runscene = CCDirector::sharedDirector()->getRunningScene();
    

    
    proto::DataPackage dataPackageNew;
    dataPackageNew.ParseFromArray(data.bytes, data.len);
	
    int seqid = dataPackageNew.seqno();
    int version=dataPackageNew.version();
		
    proto::DataPackage_Data dataNew = dataPackageNew.data();
    proto::DataPackage_CmdType cmdType = dataPackageNew.cmd();
    
    switch (cmdType) {
            
            
        case proto::DataPackage_CmdType_NOTIFY:
        {
            proto::DataPackage datapackage;
            datapackage.set_version(10);
            datapackage.set_seqno(generateSquenceNumber(proto::DataPackage_CmdType_SYNC));
            datapackage.set_cmd(proto::DataPackage_CmdType_SYNC);
            proto::DataPackage_Data  data;
            data.set_userid(ownid);
            datapackage.set_allocated_data(&data);
            
            int size = datapackage.ByteSize();
            void * buf = malloc(size);
            datapackage.SerializeToArray(buf, size);
            
            _wsiSendBinary->send((Byte*)buf, size);
           // free(buf);

            break;
        }
        case proto::DataPackage_CmdType_RECIEVE:
        {
            
            int messageSize = dataNew.message_size();
            if (messageSize > 0)
            {
                //MessageLayer *list = (MessageLayer *)messagelayer;
                
                for (int i=0; i<messageSize; i++) {
                    

                    proto::DataPackage_Message messageNew = dataNew.message(i);
                    

                    const ::std::string contentNew = messageNew.content();
                    
                    proto::DataPackage_Message_MessageType messagetype = messageNew.msgtype();
                    proto::DataPackage_Message_ContentType contenttype = messageNew.contenttype();
                    
                    
                    switch (messagetype) {
                        case proto::DataPackage_Message_MessageType_PERSON_TO_PERSON:
                        {
                            switch (contenttype) {
                                case proto::DataPackage_Message_ContentType_TEXT:
                                {
                                    proto::DataPackage_Message messageNew = dataNew.message(i);
                                    
                                    const ::std::string contentNew = messageNew.content();
                                   // proto::DataPackage_Message_ContentType type = messageNew.contenttype();
                                    //proto::DataPackage_Message_MessageType messagetype = messageNew.msgtype();
                                    
                                    int from=messageNew.from();
                                    
                                    char fromid[100];
                                    sprintf(fromid, "%d",from);
                                                                    
                                    messageDate messagedate;
                                    
                                    messagedate.MeslocalID="";
                                    messagedate.MesSvrID="";
                                    messagedate.createTime="";
                                    messagedate.message=(char*)contentNew.c_str();
                                    messagedate.status="3";
                                    messagedate.type="1";
                                    messagedate.Des="2";
                                    messagedate.fromid=fromid;
                                    
                                    //发送到显示层
                                    printf("\n receive a msg '%s' from %d \n",(char*)contentNew.c_str(),from);
                                    
                                    SavereceMsgtoDB(dbFilename.c_str(),from ,(char*)contentNew.c_str() , 1,from);
                                    
                                    updatemsgtochat(dbFilename.c_str(), from,(char*)contentNew.c_str() );
                                    
									char fromString[100];
									sprintf(fromString, "%d", from);
									DatabaseManager::sharedInstance()->addToChatList(contentNew, string(fromString), eChatTypePersonal);
									
                                    //printf("%s",runscene);
                                    
                                    MessageLayer *layer=(MessageLayer *)runscene->getChildByTag(88);

                                    if (layer) {
                                            layer->AddMsgtolayer(messagedate);
                                    }
                                    
                                    //发送到对话列表
                                    
                                    int chaton=GetchatfindCount(dbFilename.c_str(), from);
                                    
                                    if (0==chaton){
                                        
                                        MessageHomeLayer *layer=(MessageHomeLayer *)runscene->getChildByTag(77);
                                        
                                        if (layer) {
                                            
                                            ChatDate onechatdate;
                                            
                                            onechatdate.chatID="20";
                                            onechatdate.FriID=fromid;
                                            onechatdate.nickName="zhanfsan";
                                            onechatdate.Type="1";
                                            onechatdate.Picture="head.png";
                                            
                                            char thetime[100];
                                            sprintf(thetime, "%ld",time(NULL));
                                            onechatdate.lastChattime=thetime;
                                            onechatdate.lastMsg=(char*)contentNew.c_str();
                                            
                                            
                                            //layer->AddChattolayer(onechatdate);
                                        }

                                    }
                                    //更新对话最后一条消息
                                    else{
                                       // updatemsgtochat(dbFilename.c_str(), from, (char*)contentNew.c_str());
                                        
                                    }
                                    
                                    SaveChattoDB(dbFilename.c_str(), from, (char*)contentNew.c_str(),1);
                                    

                                    
                                    break;
                                }
                                case proto::DataPackage_Message_ContentType_VOICE:
                                {
                                    
                                    
                                    break;
                                }
                                case proto::DataPackage_Message_ContentType_FILE:
                                {
                                    
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        }
                            
                        case proto::DataPackage_Message_MessageType_GROUP:
                        {
                            switch (contenttype) {
                                case proto::DataPackage_Message_ContentType_TEXT:
                                {
                                    proto::DataPackage_Message messageNew = dataNew.message(i);
                                    
                                    const ::std::string contentNew = messageNew.content();
                                    //proto::DataPackage_Message_ContentType type = messageNew.contenttype();
                                   // proto::DataPackage_Message_MessageType messagetype = messageNew.msgtype();
                                    
                                    
                                    int from=messageNew.from();
                                    char fromid[100];
                                    sprintf(fromid, "%d",from);
                                    
                                    int groupid=messageNew.to();
                                    char groupname[100];
                                    sprintf(groupname, "%d",groupid);
                                    
                                    
                                    messageDate messagedate;
                                    
                                    messagedate.MeslocalID="";
                                    messagedate.MesSvrID="";
                                    messagedate.createTime="";
                                    messagedate.message=(char*)contentNew.c_str();
                                    messagedate.status="3";
                                    messagedate.type="1";
                                    messagedate.Des="2";
                                    messagedate.fromid=fromid;
                                    
                                    printf("\n receive a msg '%s' from %d from group %d\n",(char*)contentNew.c_str(),from,groupid);
                                    
                                    //发送到显示层
                                    
                                    SavereceMsgtoDB(dbFilename.c_str(),groupid ,(char*)contentNew.c_str(), 1,from);
                                    
									char groupIDString[100];
									sprintf(groupIDString, "%d", groupid);
									DatabaseManager::sharedInstance()->addToChatList(contentNew, string(groupIDString), eChatTypeGroup);
                                    
                                    MessageLayer *layer=(MessageLayer *)runscene->getChildByTag(88);
                                    
                                    if (layer&&toID==groupid) {
                                        
                                        layer->AddMsgtolayer(messagedate);
                                    }
                                    
                                    
                                    
                                    int chaton=GetchatfindCount(dbFilename.c_str(), groupid);
                                    
                                    if (0==chaton){
                                        
                                        MessageHomeLayer *layer=(MessageHomeLayer *)runscene->getChildByTag(77);
                                        
                                        if (layer) {
                                            
                                            ChatDate onechatdate;
                                            
                                            onechatdate.chatID="20";
                                            onechatdate.FriID=groupname;
                                            onechatdate.nickName="zhanfsan";
                                            onechatdate.Type="2";
                                            onechatdate.Picture="head.png";
                                            
                                            char thetime[100];
                                            sprintf(thetime, "%ld",time(NULL));
                                            onechatdate.lastChattime=thetime;
                                            onechatdate.lastMsg=(char*)contentNew.c_str();
                                            
                                            
                                           // layer->AddChattolayer(onechatdate);
                                        }
                                        
                                    }
                                    else{
                                      //  updatemsgtochat(dbFilename.c_str(), groupid, (char*)contentNew.c_str());
                                    }

                                    
                                    //发送到对话列表
                                    
                                    SaveChattoDB(dbFilename.c_str(), groupid, (char*)contentNew.c_str(),2);

            
                                    break;
                                }
                                case proto::DataPackage_Message_ContentType_VOICE:
                                {
                                    
                                    
                                    break;
                                }
                                case proto::DataPackage_Message_ContentType_FILE:
                                {
                                    
                                    
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                            
                        }
                            
                        case proto::DataPackage_Message_MessageType_CHANNEL:
                        {
                            switch (contenttype) {
                                case proto::DataPackage_Message_ContentType_TEXT:
                                {
                                    
                                    
                                    break;
                                }
                                case proto::DataPackage_Message_ContentType_VOICE:
                                {
                                    
                                    
                                    break;
                                }
                                case proto::DataPackage_Message_ContentType_FILE:
                                {
                                    
                                    
                                    break;
                                }
                                default:
                                    break;
                            }

                            break;
                        }
                            
                        default:
                            break;
                    }
                
                }
            
            }
            break;
        }
        case proto::DataPackage_CmdType_ACK:
        {
			int type;
			int originRequestNumber;
			unpackageSquenceNumber(seqid, &type, &originRequestNumber);
			proto::DataPackage_CmdType originCmdType = (proto::DataPackage_CmdType)type;

			bool handled = analysisResponse(originCmdType, dataPackageNew);
			if (handled) return;

            int userid= dataNew.userid();
            if (dataNew.group_size() != 0)
			{
				for (int k = 0; k < dataNew.group_size(); k ++)
				{
					proto::DataPackage_Group group = dataNew.group(k);
				//	printf(group.groupname());
				}

			}
            printf("\nACK seqno=%d  verion =%d  from %d\n",seqid,version,userid);
            //roto::DataPackage_Message messageNew = dataNew.message(0);

            
            break;
        }
        case proto::DataPackage_CmdType_CREATE_GROUP:
        {
            
            break;
        }
        case proto::DataPackage_CmdType_LIST_FRIENDS:
        {
			int type;
			int originRequestNumber;
			unpackageSquenceNumber(seqid, &type, &originRequestNumber);
			proto::DataPackage_CmdType originCmdType = (proto::DataPackage_CmdType)type;
			
            int friengcount=dataNew.user().size();

			int friendCount = dataNew.user().size();
			CCArray *friendArray = new CCArray();
			for (int i = 0; i < friendCount; i ++)
			{
				FriendModel *model = new FriendModel();
				friendArray->addObject(model);				
				
				int userID = dataNew.user(i).userid();
				string userName = dataNew.user(i).username();
				model->friendID	= google::protobuf::SimpleItoa(userID);
				model->nickName = userName;
				model->picture = "head2.png";
			}
			
			if (originCmdType == proto::DataPackage_CmdType_LIST_FRIENDS)
			{
				DatabaseManager::sharedInstance()->updateAllFriend(friendArray);
			}
			else if(originCmdType == proto::DataPackage_CmdType_ADD_FRIEND)
			{
				DatabaseManager::sharedInstance()->addFriends(friendArray);
			}
			
			GroupUserListLayer *groupLayer = (GroupUserListLayer *)runscene->getChildByTag(LAYER_TAG_GROUPUSER);
			if (groupLayer)
			{
				groupLayer->onFriendUserListReceived(friendArray);
			}
			
			FriendsHomeLayer *friendLayer = (FriendsHomeLayer *)runscene->getChildByTag(LAYER_TAG_FRIENDHOME);
			if (friendLayer)
			{
				friendLayer->onRequestFriendDataReceived(friendArray);
			}
        
            break;
        }
            
        default:
            break;
    }
    
}

void WebsocketHandle::onClose(cocos2d::extension::WebSocket* ws)
{
    CCLog("websocket instance (%p) closed.", ws);
    if (ws == _wsiSendBinary)
    {
        _wsiSendBinary = NULL;
    }
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
}

void WebsocketHandle::onError(cocos2d::extension::WebSocket* ws, const cocos2d::extension::WebSocket::ErrorCode& error)
{
    CCLog("Error was fired, error code: %d", error);

}

void WebsocketHandle::sendMessage(int cmdType,  int userIdTo, int messageType, int contentType, const char*messageContext)
{
	int ownid = CommonDataManager::getMyUserID();
	
    if (_wsiSendBinary->getReadyState() == cocos2d::extension::WebSocket::kStateOpen)
    {
       // CCScene*  test = CCDirector::sharedDirector()->getRunningScene();
       // CCLayer *testlayer;
        int squenceNum = generateSquenceNumber(cmdType);

        switch (cmdType) {
                
            case proto::DataPackage_CmdType_NOTIFY:{
                
                proto::DataPackage datapackage;
                datapackage.set_cmd(proto::DataPackage_CmdType_NOTIFY);
                proto::DataPackage_Data  data;

                datapackage.set_version(10);
                datapackage.set_seqno(squenceNum);
                data.set_userid(ownid);
                
                datapackage.set_allocated_data(&data);
                
                int size = datapackage.ByteSize();
                void * buf = malloc(size);
                datapackage.SerializeToArray(buf, size);
                
                _wsiSendBinary->send((Byte*)buf, size);
                //free(buf);
         
                break;
            }
            
            case proto::DataPackage_CmdType_SEND:
            {
                switch (messageType) {
                    case proto::DataPackage_Message_MessageType_GROUP:
                    {
                        proto::DataPackage datapackage;
                        datapackage.set_cmd(proto::DataPackage_CmdType_SEND);
                        proto::DataPackage_Data  data;
                        datapackage.set_version(10);
                        datapackage.set_seqno(squenceNum);
                        data.set_userid(ownid);
                        
                        proto::DataPackage_Message *msg = data.add_message();
                        msg->set_content(messageContext);
                        msg->set_from(ownid);
                        msg->set_to(toID);
                        msg->set_msgtype(proto::DataPackage_Message_MessageType_GROUP);
                        msg->set_contenttype(proto::DataPackage_Message_ContentType_TEXT);
                        
                        datapackage.set_allocated_data(&data);
                        
                        int size = datapackage.ByteSize();
                        void * buf = malloc(size);
                        datapackage.SerializeToArray(buf, size);
                        
                        _wsiSendBinary->send((Byte*)buf, size);
                        
                        printf("\nsend message'%s' ok~\n",messageContext);
                       //free(buf);
                        
                        break;
                    }
                    
                    case proto::DataPackage_Message_MessageType_PERSON_TO_PERSON:
                    {
                        proto::DataPackage datapackage;
                        datapackage.set_cmd(proto::DataPackage_CmdType_SEND);
                        proto::DataPackage_Data  data;
                        datapackage.set_version(10);
                        datapackage.set_seqno(squenceNum);
                        data.set_userid(ownid);
                        
                        proto::DataPackage_Message *msg = data.add_message();
                        msg->set_content(messageContext);
                        msg->set_from(ownid);
                        msg->set_to(toID);
                        msg->set_msgtype(proto::DataPackage_Message_MessageType_PERSON_TO_PERSON);
                        msg->set_contenttype(proto::DataPackage_Message_ContentType_TEXT);
                        
                        datapackage.set_allocated_data(&data);
                        
                        int size = datapackage.ByteSize();
                        void * buf = malloc(size);
                        datapackage.SerializeToArray(buf, size);
                        
                        _wsiSendBinary->send((Byte*)buf, size);
                        
                        printf("\nsend message'%s' ok~\n",messageContext);
                       // free(buf);
                        
                        break;
                    }
                    
                    case proto::DataPackage_CmdType_LIST_FRIENDS:
                    {
                        
                       
                        
                    }
                        
                    default:
                        break;
                }
                
                
                break;
            }
            case proto::DataPackage_CmdType_LIST_FRIENDS:
            {
                proto::DataPackage datapackage;
                datapackage.set_cmd(proto::DataPackage_CmdType_LIST_FRIENDS);
                proto::DataPackage_Data  data;
                datapackage.set_version(10);
                datapackage.set_seqno(squenceNum);
                data.set_userid(ownid);
                
                
                datapackage.set_allocated_data(&data);
                
                int size = datapackage.ByteSize();
                void * buf = malloc(size);
                datapackage.SerializeToArray(buf, size);
                
                _wsiSendBinary->send((Byte*)buf, size);
                
                printf("\nsend message'%s' ok~\n",messageContext);
                //free(buf);
                
                break;


            }
            case proto::DataPackage_CmdType_ADD_FRIEND:
            {
                proto::DataPackage datapackage;
                datapackage.set_cmd(proto::DataPackage_CmdType_ADD_FRIEND);
                proto::DataPackage_Data  data;
                data.set_userid(ownid);
                datapackage.set_version(10);
                datapackage.set_seqno(squenceNum);
                
                proto::DataPackage_User *fri = data.add_user();
                
                fri->set_userid(userIdTo);
            
                
                datapackage.set_allocated_data(&data);
                
                int size = datapackage.ByteSize();
                void * buf = malloc(size);
                datapackage.SerializeToArray(buf, size);
                
                _wsiSendBinary->send((Byte*)buf, size);
                printf("\nadd friend ok~\n");
                //free(buf);
                break;
                
            }
            case proto::DataPackage_CmdType_CREATE_GROUP:
            {
				
                proto::DataPackage datapackage;
                datapackage.set_cmd(proto::DataPackage_CmdType_CREATE_GROUP);
                proto::DataPackage_Data  data;
                data.set_userid(ownid);
                datapackage.set_version(10);
                datapackage.set_seqno(squenceNum);
                

				char *pEnd;
				int userID = strtol(messageContext, &pEnd, 10);
                proto::DataPackage_User *fri = data.add_user();
				fri->set_userid(userID);
				while (pEnd[0] != '\0')
				{
					pEnd ++;
					userID = strtol(pEnd, &pEnd, 10);
					fri = data.add_user();
					fri->set_userid(userID);
				}
				
                datapackage.set_allocated_data(&data);
                
                int size = datapackage.ByteSize();
                void * buf = malloc(size);
                datapackage.SerializeToArray(buf, size);
                
                _wsiSendBinary->send((Byte*)buf, size);
                printf("\ncreat group ok~\n");
                //free(buf);
                break;
            }

            default:
                
                break;
        }
        

    }
    else
    {
        std::string warningStr = "send binary websocket instance wasn't ready...";
        CCLog("%s", warningStr.c_str());

    }
}

