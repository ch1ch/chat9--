//
//  sqlite3DB.h
//  chats
//
//  Created by 付剑超 on 13-8-12.
//
//

#ifndef __chats__sqlite3DB__
#define __chats__sqlite3DB__

#include <iostream>
#include "sqlite3.h"

#include "Main.h"

USING_NS_CC;
using namespace std;

class SQLiteUtil
{
public:
    SQLiteUtil();
    
    ~SQLiteUtil();
	
	static void updateDatabaseName(string dbName);
	static bool createTableIfNotExist(string tableName, string sql);
	static bool executeUpdate(string sql);
	static CCArray * executeQuery(string sql);
//	static CCArray getAllDataFromTable(string tableName);

    
    sqlite3 *PrepareMsgDB(const char *dbFilename,const int username);
    
    void ReadFromSQLite(const char *dbFilename,const int username);
    
    int AddMessage(const char *dbFilename,const int username,const int SvrID,const int creatTime,const char *message,const int status,const int type,const int Des,const int fromID);
    
    bool DelMessage(const char *dbFilename,const int username,const int MesLocalID);
    
    char ** MessageallGet(const char *dbFilename,const int username);
    
    char ** GetlastMsg(const char *dbFilename,const int username,const int msgnum);

    char ** GetOnemsg(const char *dbFilename,const int username,const int mesLocalID);

    int MessageallRow(const char *dbFilename,const int  username);
    
    bool UpdateMesdb(const char *dbFilename,const int username,const int mesLocalID,const int SvrID,const int creatTime,const char *message,const int status,const int type,const int Des);
    
    sqlite3 *  PrepareFriDB(const char *dbFilename);
    
    char ** GetOneFri(const char *dbFilename,const int FriID);
    
    char ** GetOneFrilocal(const char *dbFilename,const int FrilocalID);
    
    char **GetAllfri(const char *dbFilename);
    
    int GetFriRow(const char *dbFilename);
    
    int GetFrifindRow(const char *dbFilename,const int FriID);
    
    bool AddFri(const char *dbFilename,const int FriID);
    
     bool DelFri(const char *dbFilename,const int FriID);
    
     bool UpdateFri(const char *dbFilename,const int FriID,const char *nickName,const int mobile,const int type,const int sex,const char *picture);
    
    bool DeleteTable(const char *dbFilename,const char *tablename);
    
    sqlite3 *PrepareChatDB(const char *dbFilename);
    
    bool addchat(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg);
    
    char **GetOneChat(const char *dbFilename,const int FriID);
    
    char **GetAllChat(const char *dbFilename);
    
    int GetChatRow(const char *dbFilename);
    
    int GetchatfindNum(const char *dbFilename,const int FriID);
    
    int Getchattype(const char *dbFilename,const int FriID);
    
    bool UpdateChat(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg);
    
    

};

//int loadUserRecord(void *para,int n_column,char**column_value,char**column_name);

#endif /* defined(__chats__sqlite3DB__) */
