//+------------------------------------------------------------------+
//|                                            MetaTrader Server API |
//|                   Copyright 2001-2014, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "stdafx.h"
#include "Clone.h"
#include "Parser.h"

//+------------------------------------------------------------------+
//| Constants                                                        |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
PluginInfo          ExtPluginInfo={ "DLL MMK",101,"AN Corp.",{0} };
CServerInterface   *ExtServer=NULL;                // link to server
bool trigger = TRUE;
int order = 0;
char tmp[256];

//CConfig Config;
//+------------------------------------------------------------------+
//| Command description                                              |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| DLL entry point                                                  |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
  {
//---
	WCHAR  lpFilename[256];
	char *cp;

  switch(ul_reason_for_call)
     {
      case DLL_PROCESS_ATTACH:
					  		  
		  GetModuleFileName((HMODULE)hModule, lpFilename, sizeof(lpFilename)-5); // read plugin`s adress and create link to ini file
		  WideCharToMultiByte(CP_ACP, 0, lpFilename, -1, tmp, 256, NULL, NULL);
		  if ((cp = strrchr(tmp, '.')) != NULL) 
		  {
			  *cp = 0;
			  strcat(tmp, ".ini");
		  }

         break;
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
         break;
      case DLL_PROCESS_DETACH:
         break;
     }
//---
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| About, must be present always!                                   |
//+------------------------------------------------------------------+
void APIENTRY MtSrvAbout(PluginInfo *info)
  {
   if(info!=NULL) memcpy(info, &ExtPluginInfo, sizeof(PluginInfo));
  }
//+------------------------------------------------------------------+
//| Set server interface point                                       |
//+------------------------------------------------------------------+
int APIENTRY MtSrvStartup(CServerInterface *server)
  {
//--- check version
	if (server == NULL) 
	{
		return(FALSE);
	}
	if (server->Version() != ServerApiVersion)
	{
		return(FALSE);
	}
   //--- save server interface link
   ExtServer=server;
   Config.Open(tmp); //read or create ini file and settings: master and slave accounts
   //---
   return(TRUE);
  }

void APIENTRY MtSrvTradesAdd(TradeRecord* trade, UserInfo* user, const ConSymbol* symbol)
{
	if (trade == NULL ||
		user == NULL ||
		symbol == NULL)
	{
		return;
	}

	if (user->login != Config.mAccount())
	{
		return;
	}

	//order = Clone::OrderAdd(Config.sAccount(), trade, symbol);
}

void APIENTRY MtSrvTradesUpdate(TradeRecord* trade, UserInfo* user, const int mode)
{
	if (trade == NULL ||
		user == NULL)
	{
		return;
	}
	
	if ((mode == UPDATE_CLOSE || mode == UPDATE_DELETE) && 
		trade->login == Config.mAccount())
	{
		Clone::OrderClose(trade, mode);
	}
	else
	{
		return;
	}
		
}

int APIENTRY MtSrvTradeTransaction(TradeTransInfo* trans, const UserInfo* user, int* request_id)
{

	ExtServer->LogsOut(CmdOK, "TRADE TRANSACTION HATSUDO!", NULL);

	if (trans == NULL ||
		user == NULL ||
		request_id == NULL)
	{
		return(RET_ERROR);
	}

	if (user->login == Config.mAccount())
	{
		order = Clone::OrderAddReq(trans, user, request_id);
	//	ExtServer->LogsOut(CmdOK, "TRADE TRANSACTION NI TOTTO ACCOUNTOTO!", NULL);
	//	return(1);
	}

	//ExtServer->LogsOut(CmdOK, "TRY TO DO!", NULL);
		
	//request->id;                         // идентификатор запроса
	//request->status;                     // состояние запроса
	//request->time;                       // время запроса
	//request->manager;                    // менеджер, которому направлен запрос
												  //--- данные клиента
	//request->login;                      // логин клиента
	//request->group[16];                  // группа клиента
	//request->balance;                    // баланс клиента
	//request->credit;                     // кредитные средства клиента
												  //--- данные выполненного запроса
	//request->trade = *trans;                      // тип торговой транзакции
	//request->gw_volume;                  // объем, выведенный через шлюз
	//request->gw_order;                   // тикет ордера, выведенного через шлюз
	//request->gw_price;                   // отклонение цены выведенного ордера от запрошенной

	//ExtServer->RequestsAdd(request, isdemo, request_id);

	ExtServer->LogsOut(CmdOK, "I HOPE WIN!", NULL);

	return(RET_OK);
}

//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY MtSrvPluginCfgNext(const int index, PluginCfg *cfg) 
{ 
	if (cfg == NULL)
	{
		return(1);
	}

	return Config.Next(index, cfg);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+

int APIENTRY MtSrvPluginCfgTotal()
{
	return(TOTAL_COUNT);
}
//+------------------------------------------------------------------+

int APIENTRY MtSrvPluginCfgSet(const PluginCfg *values, const int total)
{
	if (values == NULL)
	{
		return(1);
	}

	return Config.Set(values, total);
}
//+------------------------------------------------------------------+