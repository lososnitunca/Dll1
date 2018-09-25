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
RequestInfo req = { 0 };

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
	//	return;
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

	//ExtServer->LogsOut(CmdOK, "TRADE TRANSACTION HATSUDO!", NULL);

	//if (trans == NULL ||
	//	user == NULL ||
	//	request_id == NULL)
	//{
	//	return(RET_ERROR);
	//}

	
		ExtServer->LogsOut(CmdOK, "TRY TO DO!", NULL);
		//order = Clone::OrderAdd(Config.sAccount(), trans, user, request_id);

		LPCSTR     symbol = "EURUSD";	// символ
		double    prices[2] = { 0 };    // текущие цены 
		time_t    ctm = 0;       // время последней котировки 
		int       dir = 0;        // направление последнего изменения цены 

		int            order = 0;

		order = ExtServer->HistoryPrices(symbol, prices, &ctm, &dir);

		char ooo[10] = "";
		_itoa(order, ooo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I PRICED", ooo);

		_itoa(trans->order, ooo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I ORDER", ooo);

		_itoa(trans->volume, ooo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I VOLUME", ooo);

		_itoa(*request_id, ooo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I FUCKING ID", ooo);

		int isdemo = 0;
		//int request_id2 = 0;
		UserInfo       info = { 0 };
		//TradeTransInfo    trades = { 0 };
		//trades = *trans;
		//TradeTransInfo trans = { 0 };
		//RequestInfo req = { 0 };
		UserRecord suser = { 0 };

		ExtServer->ClientsUserInfo(7, &suser);
		
		ZeroMemory(&req, sizeof(req));

		req.id = 0;
		req.manager = 0;
		req.status = DC_REQUEST;
		req.time = GetTickCount();

		//req.login = user->login;
		//COPY_STR(req.group, user->group);
		//req.credit = user->credit;
		//req.balance = user->balance;

		req.login = suser.login;
		COPY_STR(req.group, suser.group);
		req.credit = suser.credit;
		req.balance = suser.balance;


		req.prices[0] = trans->price;
		req.prices[1] = trans->price;

		//req.trade = *trans;

		req.trade.price = trans->price;
		req.trade.type = trans->type;
		//req.trade.type = TT_ORDER_MK_OPEN;
		req.trade.cmd = trans->cmd;
		//req.trade.cmd = OP_BUY;

		req.trade.crc = trans->crc;		req.trade.expiration = trans->expiration;
		req.trade.ie_deviation = trans->ie_deviation;
		//req.trade.orderby = trans->orderby;
		req.trade.orderby = suser.login;

		req.trade.volume = trans->volume;
		COPY_STR(req.trade.symbol, trans->symbol);
		COPY_STR(req.trade.comment, trans->comment);

		int request_id2 = 0;

		
		UserInfo m_manager = { 0 };

		ZeroMemory(&m_manager, sizeof(m_manager));
		m_manager.login = 1;


		//order = ExtServer->OrdersAdd(&trades,&info,symbol);
		//order = ExtServer->OrdersOpen(trans, &info);
		order = ExtServer->RequestsAdd(&req, FALSE, &request_id2);
		//req.login = 6;
		char oo[4] = "";
		_itoa(order, oo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED ADD CODE", oo);
		_itoa(request_id2, oo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED REQUEST ID", oo);
		
		
	return(RET_OK);
}

int APIENTRY MtSrvDealerConfirm(const int id, const UserInfo* us, double* prices)
{

	ExtServer->LogsOut(CmdOK, "I HOPE I FUCKING DEALER CONFIRM", NULL);

	if (id != req.id)return(TRUE);

	UserInfo info = { 0 };

	Clone::UserGetInfo(7, &info);

	ExtServer->LogsOut(CmdOK, "I HOPE I FUCKING OPEN", NULL);

	ExtServer->OrdersOpen(&(req.trade), &info);

	ExtServer->RequestsDelete(id);

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