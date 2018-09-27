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
	if (trans == NULL ||
		user == NULL ||
		request_id == NULL)
	{
		return(RET_ERROR);
	}

	if (user->login != Config.mAccount())
	{
		return(RET_OK);
	}
	
	UserRecord suser = { 0 }; // struct for slave account info

	ExtServer->ClientsUserInfo(Config.sAccount(), &suser);

	RequestInfo req = { 0 };

	req.id = 0;
	req.manager = 0;
	req.status = DC_REQUEST;
	req.time = GetTickCount();

	req.login = suser.login;
	COPY_STR(req.group, suser.group);
	req.credit = suser.credit;
	req.balance = suser.balance;

	req.prices[0] = trans->price;
	req.prices[1] = trans->price;

	req.trade.price = trans->price;
	req.trade.type = trans->type;
	req.trade.cmd = trans->cmd;
	
	req.trade.crc = trans->crc;
	req.trade.expiration = trans->expiration;

	req.trade.ie_deviation = trans->ie_deviation;
		
	req.trade.orderby = suser.login;

	req.trade.volume = trans->volume;
	COPY_STR(req.trade.symbol, trans->symbol);
	COPY_STR(req.trade.comment, trans->comment);

	int req_id = 0;

	order = ExtServer->RequestsAdd(&req, FALSE, &req_id);
	
	if (order != RET_TRADE_ACCEPTED || req_id == 0)
	{
		ExtServer->LogsOut(CmdOK, "WE HAVE TROUBLE WITH APPLYING NEW REQUEST FOR SLAVE USER", NULL);
	}

	return(RET_OK);
}

int APIENTRY MtSrvDealerConfirm(const int id, const UserInfo* us, double* prices)
{
	if (us == NULL ||
		prices == NULL ||
		id == 0)
	{
		return(FALSE);
	}

	ExtServer->LogsOut(CmdOK, "MtSrvDealerConfirm START", NULL);

	int key = 0;    // ключ для определения новых запросов 
	RequestInfo req = { 0 };     // массив запросов 
	const int max = 1;      // максимально число запросов 
	char ooo[4] = "";

	order = ExtServer->RequestsGet(&key, &req, max);
	if (order == 0)
	{
		_itoa(order, ooo, 4);
		ExtServer->LogsOut(CmdOK, "REQUESTS: ", ooo);
		return(TRUE);
	}

	if (req.login != Config.sAccount())
	{
		return(TRUE);
	}

	UserInfo info = { 0 };
	Clone::UserGetInfo(Config.sAccount(), &info);

	order = ExtServer->OrdersOpen(&(req.trade), &info);
	if (order == 0)
	{
		ExtServer->LogsOut(CmdOK, "WE HAVE TROUBLE WITH OPENING NEW ORDER FOR SLAVE USER", NULL);
	}

	ExtServer->RequestsDelete(req.id);

	return(TRUE);
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