//+------------------------------------------------------------------+
//|                                            MetaTrader Server API |
//|                   Copyright 2001-2014, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"

//+------------------------------------------------------------------+
//| Constants                                                        |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
PluginInfo          ExtPluginInfo={ "DLL MAMKA",101,"ANUS Corp.",{0} };

CServerInterface   *ExtServer=NULL;                // link to server
UserRecord *inf = NULL;
UserInfo *info = NULL;
//+------------------------------------------------------------------+
//| Command description                                              |
//+------------------------------------------------------------------+
LPCSTR GetCmd(const int cmd)
  {
   static LPCSTR ExtOperations[9]={ "buy","sell","buy limit","sell limit",
      "buy stop","sell stop","balance","credit","error" };
//--- проверки
   if(cmd<OP_BUY || cmd>OP_CREDIT) return(ExtOperations[8]);
//---
   return(ExtOperations[cmd]);
  }
//+------------------------------------------------------------------+
//| DLL entry point                                                  |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
  {
//---
   switch(ul_reason_for_call)
     {
      case DLL_PROCESS_ATTACH:
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
   if(info!=NULL) memcpy(info,&ExtPluginInfo,sizeof(PluginInfo));
  }
//+------------------------------------------------------------------+
//| Set server interface point                                       |
//+------------------------------------------------------------------+
int APIENTRY MtSrvStartup(CServerInterface *server)
  {
//--- check version
   if(server==NULL)                        return(FALSE);
   if(server->Version()!=ServerApiVersion) return(FALSE);
//--- save server interface link
   ExtServer=server;
   ExtServer->LogsOut(CmdOK,"DLL ANUS OBMAZUS","initialized"); // test massage / delete
//---
   return(TRUE);
  }

void APIENTRY  MtSrvTradesAdd(TradeRecord* trade, UserInfo* user, const ConSymbol* symbol)
{
	
	if (user->login != 6)return;
	
	ExtServer->LogsOut(CmdOK, "EBIS KONIS", "ZOPED"); // test massage / delete
	
	/*trade->order++;
	trade->login = 7;

	ExtServer->ClientsUserInfo(7, inf);
	info->agent_account = inf->agent_account;
	info->balance = inf->balance;
	info->credit = inf->credit;
	info->enable = inf->enable;
	info->enable_change_password = inf->enable_change_password;
	info->enable_read_only = inf->enable_read_only;
	info->leverage = inf->leverage;
	info->login = inf->login;
	info->prevbalance = inf->prevbalance;

	ExtServer->OrdersAdd(trade, info, symbol);*/

}
//+------------------------------------------------------------------+