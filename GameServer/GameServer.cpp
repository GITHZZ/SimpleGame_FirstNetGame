/*
 *  GameServer.cpp
 *  GameServer
 *
 *  Created by studentg07 on 12-4-7.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameServer.h"

GameServer::GameServer()
{
	nReadyCount=0;
	nPlayerCount=0;
	gameState=1;
	tempNum=1;
}

GameServer::~GameServer()
{
}

void GameServer::sendNewCardList()
{
	MSG_NEWCARD theNewCard; 
	theNewCard.nType=MSGID_NEWCARD;
	theNewCard.nLen=sizeof(struct MSG_NEWCARD);
	theNewCard.game_Count=gameState;
	gameState=-gameState;
	
	for(int i=0;i<nPlayerCount;i++)
	{
		for(int j=0;j<MAX_CARD_COUNT;j++)
		{
			theNewCard.CardList[j]=theGameLogic.NewCardList[i][j];
		}
		char *p=(char*)&theNewCard;
		theNewCard.is_MainPlayper=1*i;
		sendMsg(playerList[i].fd,p,theNewCard.nLen);
	}
}
void GameServer::onRead(struct CLIENT *client,char *recvbuf,int nLen)
{
	MSG_GENERIC* pGenericMsg = (MSG_GENERIC*)recvbuf;
	char tmp[MAXDATASIZE];
	bzero(tmp,MAXDATASIZE);
	switch(pGenericMsg->nType) // 判断收到的消息类型
	{
		case MSGID_READY:
		{
			//	MSG_READY* pReaad=(MSG_READY*)recvbuf;
			nReadyCount++;
			if(nReadyCount==nPlayerCount&&nPlayerCount>=1)
			{
				MSG_NEWCARD theNewCard;
				theNewCard.nType=MSGID_NEWCARD;
				theNewCard.nLen=sizeof(struct MSG_NEWCARD);
				theGameLogic.InitGame(nPlayerCount);
				sendNewCardList();
				
			}
			
		}
			break;
			
		case MSGID_CHAT:
		{
			MSG_CHAT* pChat=(MSG_CHAT*)recvbuf;
			
			sprintf(tmp,"%s说：%s",client->name,pChat->Chat);
			strcpy(pChat->Chat,tmp);
			strcat(tmp,"\r");
			printf("%s",tmp);
			fflush(stdout);
			sendMsg(recvbuf,sizeof(struct MSG_CHAT));	
		}
			break;
			
		case MSGID_LOGIN:
		{
			MSG_LOGIN *pLogin=(MSG_LOGIN*)recvbuf;
			strcpy(client->name,pLogin->Name);
			printf("%s登录了游戏！\r",client->name);
			if (nPlayerCount<MAX_GAME_PLAYER) 
			{
				if (nPlayerCount<1) 
				{
					MSG_CHAT myChat;
					myChat.nType=MSGID_CHAT;
					myChat.nLen=sizeof(struct MSG_CHAT);
					strcpy(myChat.Chat,"系统信息:玩家个数不足两人，请再加入一个玩家!");
					char *p=(char*)&myChat;
					sendMsg(p,myChat.nLen);
				}
				
				if (nPlayerCount==1) 
				{
					MSG_CHAT myChat;
					myChat.nType=MSGID_CHAT;
					myChat.nLen=sizeof(struct MSG_CHAT);
					strcpy(myChat.Chat,"系统信息:玩家个数足够，可以开始游戏");
					char *p=(char*)&myChat;
					sendMsg(p,myChat.nLen);
				}
				
				strcpy(playerList[nPlayerCount].Name,client->name);
				playerList[nPlayerCount].fd=client->fd;
				playerList[nPlayerCount].nID=nPlayerCount;
				playerList[nPlayerCount].nScore=0;
				playerList[nPlayerCount].nGrade=0;
				
				MSG_NEWPLAYER theNewPlayer;
				
				theNewPlayer.nType=MSGID_NEWPLAYER;
				theNewPlayer.nLen=sizeof(struct MSG_NEWPLAYER);
				theNewPlayer.player=playerList[nPlayerCount];
				char *p=(char*)&theNewPlayer;
				for (int i=0; i<=nPlayerCount; i++)
				{
					if(playerList[i].fd!=client->fd)//不是当前玩家
					{
						sendMsg(playerList[i].fd,p,theNewPlayer.nLen);
					}
					else
					{
						MSG_ALLPLAYER theAllPlayer;
						theAllPlayer.nType=MSGID_ALLPLAYER;
						theAllPlayer.nLen=sizeof(struct MSG_ALLPLAYER);
						
						memcpy(theAllPlayer.playerList,playerList,sizeof(struct PLAYER_INFO)*MAX_GAME_PLAYER);
						theAllPlayer.playerCount=nPlayerCount;
						char *q=(char*)&theAllPlayer;
						sendMsg(client->fd,q,theAllPlayer.nLen);
					}
					
				}
				nPlayerCount++;
				
			}
			else {
				MSG_CHAT myChat;
				myChat.nType=MSGID_CHAT;
				myChat.nLen=sizeof(struct MSG_CHAT);
				strcpy(myChat.Chat,"系统信息:玩家个数已满，不能登录游戏！");
				char *p=(char*)&myChat;
				sendMsg(client->fd,p,myChat.nLen);
				deleteClient(client);
				
			}
			
			sendMsg(recvbuf,sizeof(struct MSG_LOGIN));
		}
			break;
			
		case MSGID_PUTCARD:
		{
			MSG_PUTCARD *pPutCard=(MSG_PUTCARD*)recvbuf;
			strcat(tmp,"\r");
			printf("%d",pPutCard->CardList);
			printf("%s的出牌里面的数字：%d\r",client->name,pPutCard->CardList);
			
			cardNum=pPutCard->CardList;
			
			MSG_GAMENUM myNum;
			tempNum=tempNum+1;
			
			if (tempNum==14) 
			{
				tempNum=1;
			}
			
			myNum.nType=MSGID_GAMENUM;
			myNum.nLen=sizeof(struct MSG_GAMENUM);
			myNum.cardNum=tempNum;

			for(int i=0;i<nPlayerCount;i++)
			{
				char *p=(char*)&myNum;
				if (playerList[i].fd==client->fd) 
				{
					myNum.game_Count=1;
				}
				else 
				{
					myNum.game_Count=0;
				}
				
				sendMsg(playerList[i].fd,p,myNum.nLen);
			}
			
			gameState=-gameState;
		}
		    break;
			
		case MSGID_GAMEDOUBLE:
		{
			printf("123\n");
			printf("(cardNum=%d,tempNum=%d)",cardNum,tempNum-1);
			if (cardNum==tempNum-1) {
				MSG_CHAT myChat;
				myChat.nType=MSGID_CHAT;
				myChat.nLen=sizeof(struct MSG_CHAT);
				strcpy(myChat.Chat,"牌号与发牌的那个牌的号码一样.");
				char *p=(char*)&myChat;
				sendMsg(client->fd,p,myChat.nLen);
			}
			else 
			{
				MSG_CHAT myChat;
				myChat.nType=MSGID_CHAT;
				myChat.nLen=sizeof(struct MSG_CHAT);
				strcpy(myChat.Chat,"牌号与发牌的那个牌的号码不一样.");
				char *p=(char*)&myChat;
				sendMsg(client->fd,p,myChat.nLen);
			}
			
		}
			break;
		
	}
}

void GameServer::onClose(struct CLIENT *client)
{
	nPlayerCount--;
	nReadyCount--;
	printf("%d",nPlayerCount);
}
