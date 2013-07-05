/*
 *  GameLogic.cpp
 *  GameServer
 *
 *  Created by studentg07 on 12-4-8.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameLogic.h"

void GameLogic::RandCardList()
{

	int RandNo1,RandNo2;
	int N;
	N=100;//交换N次牌
	while(N--){
		RandNo1=rand()%54;
		RandNo2=rand()%54;
		int tmp;
		tmp=CardListData[RandNo2];
		CardListData[RandNo2]=CardListData[RandNo1];
		CardListData[RandNo1]=tmp;
	}	
	return;
}

void GameLogic::InitGame(int iPlayerCount)
{
	nPlayerCount=iPlayerCount;
	for(int i=1;i<=54;i++)
	{
		CardListData[i]=i;
	}
	RandCardList();
	GetNewCardList();
	
}

void GameLogic::GetNewCardList()
{
	int n=0;
	for(int i=0;i<=nPlayerCount;i++)
		for(int j=0;j<MAX_CARD_COUNT;j++)
		{
			NewCardList[i][j]=CardListData[n];
			n++;
		}
}