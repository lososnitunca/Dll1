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
		int request_id2 = 0;
		UserInfo       info = { 0 };
		TradeRecord    trades = { 0 };
		//TradeTransInfo trans = { 0 };
		RequestInfo req = { 0 };

		ZeroMemory(&req, sizeof(req));

		req.login = 7;

		req.trade = *trans;
		//request.login = 7;
		req.id = *request_id + 1;
		req.manager = 1;
		req.prices[0] = prices[0];
		req.prices[1] = prices[1];
		req.status = DC_REQUEST;
		//request.trade.volume = 234;

		_snprintf(ooo, sizeof(ooo) - 1, "%.5f", prices[0]);
		ExtServer->LogsOut(CmdOK, "I HOPE I PRICED", ooo);
		_snprintf(ooo, sizeof(ooo) - 1, "%.5f", prices[1]);
		ExtServer->LogsOut(CmdOK, "I HOPE I PRICED", ooo);
		_snprintf(ooo, sizeof(ooo) - 1, "%.5f", req.trade.price);
		ExtServer->LogsOut(CmdOK, "I HOPE I TRANS PRICED", ooo);

		//trades = *trade;
		//trades.login = login;
		//trades.order = 0;
		//trades.gw_order = 0;

		//_snprintf(trades.comment, sizeof(trades.comment) - 1, "%d+%d+%d+%d+%d", trade->order, trade->api_data[0], trade->api_data[1], trade->api_data[2], trade->api_data[3]);

		

		//order = ExtServer->OrdersAdd(&trades,&info,symbol);
		//order = ExtServer->OrdersOpen(trans, &info);
		order = ExtServer->RequestsAdd(&req, isdemo, &req.id);
		req.login = 7;
		char oo[4] = "";
		_itoa(order, oo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED ADD CODE", oo);
		_itoa(*request_id, oo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED REQUEST ID", oo);
		_itoa(isdemo, oo, 10);
		ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED ISDEMO", oo);

		int er = 0;
		er = ExtServer->RequestsLock(req.id, 1);
		if (er != TRUE)ExtServer->LogsOut(CmdOK, "I HOPE I`LL BE OKAY NOT GAY!", NULL);
		else ExtServer->LogsOut(CmdOK, "I HOPE I`LL BE OKAY!", NULL);
		//if(order!=0)ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED", NULL);

		UserInfo m_manager = { 0 };
		
		ZeroMemory(&m_manager, sizeof(m_manager));
		m_manager.login = 1;

		if ((ExtServer->RequestsConfirm(req.id, &m_manager, prices)) != RET_OK)
		{
				ExtServer->LogsOut(CmdOK, "I HOPE I`LL DIE!", NULL);
		}
		ExtServer->LogsOut(CmdOK, "I HOPE I FUCKING REQUEST!", NULL);
	

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

	//ExtServer->LogsOut(CmdOK, "I HOPE WIN!", NULL);

	return(RET_OK);
}

/*void APIENTRY MtSrvTradeRequestApply(RequestInfo* request, const int isdemo)
{
	
	
	double prices[2] = { 0 };
	UserInfo m_manager = { 0 };
	TradeTransInfo trans = request->trade;

	if (request->login != Config.sAccount())return;

	ZeroMemory(&m_manager, sizeof(m_manager));
	m_manager.login = 1;
	//COPY_STR(m_manager.name, "Dealer Helper");
	//COPY_STR(m_manager.ip, "DealerHelper");
	//COPY_STR(m_symbols, "*");

	//if (request->login == Config.mAccount())
	//{
	//	order = Clone::OrderAdd(Config.sAccount(), &trans);
	//}
	//ExtServer->LogsOut(CmdOK, "I HOPE I REQUEST!", NULL);
	//if (request->login != Config.sAccount())
	{
		//ExtServer->HistoryPricesGroup(request, prices);
		//if ((ExtServer->RequestsConfirm(request->id, &m_manager, prices)) != RET_OK)
		{
	//		ExtServer->LogsOut(CmdOK, "I HOPE I`LL DIE!", NULL);
		}
		//ExtServer->LogsOut(CmdOK, "I HOPE I FUCKING REQUEST!", NULL);
		//return;
	}

	//int er = 0;
	//er = ExtServer->RequestsLock(request->id, 1);
	//if (er != TRUE)ExtServer->LogsOut(CmdOK, "I HOPE I`LL BE OKAY NOT GAY!", NULL);
	//else ExtServer->LogsOut(CmdOK, "I HOPE I`LL BE OKAY!", NULL);
	// AZAZAZAZAZAZAZA

	LPCSTR     symbol = "EURUSD";	// символ
	//double    prices[2] = { 0 };    // текущие цены 
	time_t    ctm = 0;       // время последней котировки 
	int       dir = 0;        // направление последнего изменения цены 

	int            order = 0;

	order = ExtServer->HistoryPrices(symbol, prices, &ctm, &dir);

	char ooo[4] = "";
	_itoa(order, ooo, 10);
	ExtServer->LogsOut(CmdOK, "I HOPE I PRICED IN THIS APP", ooo);

	if ((ExtServer->RequestsConfirm(request->id, &m_manager, prices)) != RET_OK)
	{
		ExtServer->LogsOut(CmdOK, "I HOPE I`LL DIE 7!", NULL);
	}
	ExtServer->LogsOut(CmdOK, "I HOPE I FUCKING REQUEST 7!", NULL);
	// AZAZAZAZAZAZAZAZAZAZAZ
}*/

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