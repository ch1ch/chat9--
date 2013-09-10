//
//  MessageBase.cpp
//  chats
//
//  Created by 付剑超 on 13-8-15.
//
//

#include "MessageBase.h"
#include "sqlite3DB.h"




int SyncTime(const int localTime){
    return time(NULL);
    
}


int SaveMsgdb(const char *dbFilename,const int username,const int mesSvrID,const int createTime,const char *message,const int status,const int type, const int des,const int fromID){
    SQLiteUtil *DButil=new SQLiteUtil();
    int LocID=DButil->AddMessage(dbFilename,username, mesSvrID, createTime,message, status, type, des,fromID);
    //DButil->ReadFromSQLite(dbFilename,username);
    
    //delete DButil;

   
    return LocID;
    
}


bool SendstatusOk(const int mesSvrID,const int username){
    

    return true;
    
}

bool SendMesLocalID(const int meslocalID){
    return true;
    
}

int SyncSvrID(const int meslocalID){
    return 0;
}



bool MsgReadstatus(const char *user,const int mesSvrID){
    

    return true;
    
}

bool DelMsg(const int username,const int MesLocID){
    
   
    return true;
    
}

bool DelChat(const int username){
    

    return true;
    
}
char **GetOneMessage(const char *dbFilename,const int username,const int mesLocalID){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetOnemsg(dbFilename, username, mesLocalID);
    
    //delete DButil;

    return result;
}

char **GetNumMsg(const char *dbFilename,const int username,const int msgnum){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetlastMsg(dbFilename, username, msgnum);
    
    //delete DButil;

    
    return result;
}

int msgcount(const char *dbFilename,const int username){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->MessageallRow (dbFilename,username);
    //delete DButil;
    return result;

}


bool DelmsgTable(const char *dbFilename,const int username){
    SQLiteUtil *DButil=new SQLiteUtil();
    
    char name[10];
    sprintf(name, "%d",username);
    
    char *temp=strcat("chat_", name);
    
    int result= DButil->DeleteTable(dbFilename,temp);
    //delete DButil;
    return result;
    
}



char **GetOneFriend(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetOneFri(dbFilename, FriID);
    
    ////delete DButil;
    return result;
    
}
char **GetOneFriendlocal(const char *dbFilename,const int FrilocalID){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetOneFrilocal(dbFilename, FrilocalID);
    
    return result;
    
}

char **GetAllFriend(const char *dbFilename){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetAllfri(dbFilename);
    
    //delete DButil;
    return result;
    
}

int FriendNum(const char *dbFilename){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->GetFriRow(dbFilename);
    
    //delete DButil;
    return result;
    
}

int FriendFindNum(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->GetFrifindRow (dbFilename,FriID);
    
    //delete DButil;
    return result;
    
}


bool AddFriend(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    DButil->AddFri(dbFilename, FriID);
    
    //delete DButil;
    return true;
    
}

bool DelFriend(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    DButil->DelFri(dbFilename, FriID);
    
    //delete DButil;
    return true;
    
}

bool UpdateFriend(const char *dbFilename,const int FriID,const char *nickName,const int mobile,const int type,const int sex,const char *picture){
    
    SQLiteUtil *DButil=new SQLiteUtil();
    DButil->UpdateFri(dbFilename, FriID, nickName, mobile, type, sex, picture);
    
    //delete DButil;
    return true;
    
}


bool DelFriendTable(const char *dbFilename){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->DeleteTable(dbFilename,"friend");
    //delete DButil;
    return result;
    
    return true;
}


bool addChattoDb(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg){
    
    SQLiteUtil *DButil=new SQLiteUtil();
    bool result= DButil->addchat(dbFilename, chatID, FriID, nickname, type, picture, lastchattime, lastmsg);
    //delete DButil;
    
    return result;
    
}


char **GetOneChat(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetOneChat(dbFilename, FriID);
    
    ////delete DButil;
    return result;
    
}

char **GetAllChats(const char *dbFilename){
    SQLiteUtil *DButil=new SQLiteUtil();
    char** result= DButil->GetAllChat(dbFilename);
    
    //delete DButil;
    return result;
    
}

int ChatNum(const char *dbFilename){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->GetChatRow(dbFilename);
    
    //delete DButil;
    return result;
    
}

int getchatfindnum(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->GetchatfindNum(dbFilename,FriID);
    
    //delete DButil;
    return result;
    
}

int getChattype(const char *dbFilename,const int FriID){
    SQLiteUtil *DButil=new SQLiteUtil();
    int result= DButil->Getchattype(dbFilename,FriID);
    

    return result;
    
}

bool DelChatTable(const char *dbFilename){
    SQLiteUtil *DButil=new SQLiteUtil();
    bool result= DButil->DeleteTable(dbFilename,"chatlist");
    //delete DButil;
    //return result;
    
    return result;
}

bool UpdateoneChat(const char *dbFilename,const int chatID,const int FriID,const char *nickname,const int type,const char *picture,const int lastchattime,const char *lastmsg){
    
    SQLiteUtil *DButil=new SQLiteUtil();
    bool result= DButil->UpdateChat(dbFilename, chatID, FriID, nickname, type, picture, lastchattime, lastmsg);
    //delete DButil;
    
    return result;
    
}