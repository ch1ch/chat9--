//
//  MessageHandle.h
//  chats
//
//  Created by 付剑超 on 13-8-14.
//
//

#ifndef __chats__MessageHandle__
#define __chats__MessageHandle__

#include <iostream>
//#include "MessageBase.h"


struct messageDate
{
    char *MeslocalID;
    char *MesSvrID;
    char *createTime;
    char *message;
    char *status;
    char *type;
    char *Des;
    char *fromid;
    
};

struct FriendDate
{
    char *FriLocalID;
    char *FriID;
    char *NickName;
    char *Mobile;
    char *Type;
    char *Sex;
    char *Picture;
	char *lastMessage;
};

struct ChatDate
{
    char *chatLocalID;
    char *chatID;
    char *FriID;
    char *nickName;
    char *Type;
    char *Picture;
    char *lastChattime;
    char *lastMsg;
    
};


bool SavesendMsgtoDB(const char *dbFilename,const int username,const char *message,const int type,const int fromID);

bool SavereceMsgtoDB(const char *dbFilename,const int username,const char *message,const int type,const int fromID);

bool DelMessage();

bool DelChat();

messageDate get1Msg(const char *dbFilename,const int username,const int mesLocalID);

messageDate* getLast10Msg(const char *dbFilename,const int username);

messageDate gettheLastMsg(const char *dbFilename,const int username);

char **get1lastMsg(const char *dbFilename,const int username);

char **getsomeMsg(const char *dbFilename,const int username,const int num);

messageDate* GetnumMsg(const char *dbFilename,const int username,const int msgnum);

int GetmsgCount(const char *dbFilename,const int username);

bool DelMsglist(const char *dbFilename,const int username);

FriendDate GetOneFriendfromDB(const char *dbFilename,const int FriID);

FriendDate GetOneFriendfromDBlocal(const char *dbFilename,const int FrilocID);

char** GetOneFriendFDB(const char *dbFilename,const int FriID);

FriendDate* GetAllFriendfromDB(const char *dbFilename);

char **getallFDB(const char *dbFilename);

bool AddFriendtoDB(const char *dbFilename,const int FriID);

bool DelFriendtoDB(const char *dbFilename,const int FriID);

int GetFriendCount(const char *dbFilename);

int FriendFindCount(const char *dbFilename,const int FriID);

bool SaveChattoDB(const char *dbFilename,const int FriID,const char *lastmsg,const int type);

char **GetoneChat(const char *dbFilename,const int FriID);

char **GetallChats(const char *dbFilename);

int GetchatNum(const char *dbFilename);

int GetchatfindCount(const char *dbFilename,const int FriID);

int GetChatType(const char *dbFilename,const int FriID);

bool DelchatTable(const char *dbFilename);

bool updatemsgtochat(const char *dbFilename,const int FriID,const char *lastmsg);

//bool updatechatlastmsg();

    
#endif /* defined(__chats__MessageHandle__) */
