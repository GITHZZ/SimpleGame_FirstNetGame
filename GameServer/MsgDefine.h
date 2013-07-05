#pragma once

#define MAX_GAME_PLAYER 2//最多玩家个数
#define MAX_CARD_COUNT  10
//最多牌张数

#define MSGID_LOGIN            1   //登录消息
#define MSGID_CHAT             2   //聊天消息
#define MSGID_NEWPLAYER        3   //新玩家登录消息
#define MSGID_ALLPLAYER        4   //当前服务器所有玩家消息
#define MSGID_LEAVEGAME        5   //有人离开游戏的消息
#define MSGID_READY            6   //准备消息
#define MSGID_NEWCARD          7   //发牌消息
#define MSGID_PUTCARD          8   //出牌消息，如果牌张数为0，则为不出消息
#define MSGID_GAMERESULT       9   //游戏输赢得分情况消息
#define MSGID_GAMENUM		   10  //牌号消息
#define MSGID_GAMEDOUBLE       11  //游戏所需

struct MSG_GENERIC
{
	
	int nType;   //消息类型
	int nLen;    //消息长度，需分包时用
};

struct MSG_LOGIN:public MSG_GENERIC
{
	int  nID;              //发送消息人的编号
	char Name[20];         //用户名
	char Pwd[20];          //密码
};

struct MSG_CHAT:public MSG_GENERIC
{
	int  nID;          //发送消息人的编号
	char Chat[50];     //聊天信息长度，最长200字节
};

struct PLAYER_INFO{
	char Name[20];
	int  fd;//当前玩家套接字标识
	int  nID;//玩家的ID
	int  nScore;//玩家的得分
	int  nGrade;//玩家的等级
};

struct MSG_NEWPLAYER:public MSG_GENERIC
{
	PLAYER_INFO player;
};

struct MSG_ALLPLAYER:public MSG_GENERIC
{
	PLAYER_INFO playerList[MAX_GAME_PLAYER];
	int playerCount;
};

struct MSG_LEAVEGAME:public MSG_GENERIC
{
	int fd;
};

struct MSG_READY:public MSG_GENERIC
{
	int nID;
};

struct MSG_NEWCARD:public MSG_GENERIC 
{
	int CardList[MAX_CARD_COUNT];
	int game_Count;// 轮到谁了
	int is_MainPlayper;//是否是主机
};

struct MSG_PUTCARD:public MSG_GENERIC
{
	int nID;
	int game_Count;				// 轮到谁了
	int CardList;
	int nCount;                //实际出牌的张数
};

struct MSG_GAMERESULT:public MSG_GENERIC
{
	int ScoreList[MAX_GAME_PLAYER];
};

struct MSG_GAMENUM:public MSG_GENERIC
{
	int cardNum;
	int game_Count;
};

struct MSG_GAMEDOUBLE:public MSG_GENERIC
{
	
};

