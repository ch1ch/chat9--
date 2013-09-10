//
//  MessageBase.h
//  chats
//
//  Created by 付剑超 on 13-8-15.
//
//
#ifndef namelong
#define namelong 100
#endif
#ifndef messagecolumn
#define messagecolumn 8
#endif
#ifndef friendcolumn
#define friendcolumn 8
#endif

#ifndef __chats__MessageBase__
#define __chats__MessageBase__

#include <iostream>

//#include "sqlite3.h"



int SyncTime(const int localTime);


int SaveMsgdb(const char *dbFilename,const int username,const int mesSvrID,const int createTime,const char *message,const int status,const int type,  const int des,const int fromID);

int SendtoSvr(const char *packMsg);

bool SendstatusOk(const int mesSvrID,const int username);

bool SendMesLocalID(const int meslocalID);

int SyncSvrID(const int meslocalID);


bool MsgReadstatus(const char *user,const int mesSvrID);

bool DelMsg(const int username,const int MesLocID);

bool DelChat(const int username);

char **GetOneMessage(const char *dbFilename,const int username,const int mesLocalID);

char **GetNumMsg(const char *dbFilename,const int username,const int msgnum);

int msgcount(const char *dbFilename,const int username);

bool DelmsgTable(const char *dbFilename,const int username);

char **GetOneFriend(const char *dbFilename,const int FriID);

char **GetOneFriendlocal(const char *dbFilename,const int FrilocalID);

char **GetAllFriend(const char *dbFilename);

int FriendNum(const char *dbFilename);

int FriendFindNum(const char *dbFilename,const int FriID);

bool AddFriend(const char *dbFilename,const int FriID);

bool DelFriend(const char *dbFilename,const int FriID);

bool UpdateFriend(const char *dbFilename,const int FriID,const char *nickName,const int mobile,const int type,const int sex,const char *picture);

bool addChattoDb(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg);

char **GetOneChat(const char *dbFilename,const int FriID);

char **GetAllChats(const char *dbFilename);

int ChatNum(const char *dbFilename);

int getchatfindnum(const char *dbFilename,const int FriID);

int getChattype(const char *dbFilename,const int FriID);

bool DelChatTable(const char *dbFilename);

bool UpdateoneChat(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg);

#endif /* defined(__chats__MessageBase__) */



