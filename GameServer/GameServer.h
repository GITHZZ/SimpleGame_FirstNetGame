/*
 *  GameServer.h
 *  GameServer
 *
 *  Created by studentg07 on 12-4-7.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include "SelectServer.h"
#include "MsgDefine.h"
#include "GameLogic.h"
class GameServer : public SelectServer {
public:
	GameServer();
	virtual ~GameServer();

protected:
	//定义虚函数onRead
	void onRead(struct CLIENT *client,char *recvbuf,int nLen);	
	//定义虚函数onClose
	void onClose(struct CLIENT *client);
	
	int nPlayerCount; //当前游戏玩家个数
	int nReadyCount;  //当前点准备玩家个数
	int gameState;
	int tempNum;
	int cardNum;
	PLAYER_INFO playerList[MAX_GAME_PLAYER];
	private:
	GameLogic theGameLogic;
	void sendNewCardList();
};
