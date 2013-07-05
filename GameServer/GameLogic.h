/*
 *  GameLogic.h
 *  GameServer
 *
 *  Created by studentg07 on 12-4-8.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdlib.h>
#include "MsgDefine.h"


class GameLogic
{
private:
	int CardListData[54]; //54张扑克

	int nPlayerCount;
public:
	int NewCardList[MAX_GAME_PLAYER][MAX_CARD_COUNT];
	void InitGame(int iPlayerCount);
	//洗牌
	void RandCardList();
	bool CompareCard(int FirstCardData,int SecondCardData);
	void GetNewCardList();

	
};