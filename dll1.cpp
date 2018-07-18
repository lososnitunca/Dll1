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
PluginInfo          ExtPluginInfo={ "DLL MAMKA",101,"ANUS Corp.",{0} };
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
		  if ((cp = strrchr(tmp, '.')) != NULL) { *cp = 0; strcat(tmp, ".ini"); }

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
   ExtServer->LogsOut(CmdOK, "Adress: ", tmp); // test massage / delete
   
   Config.Open(tmp); //read or create ini file and settings: master and slave accounts
   
   ExtServer->LogsOut(CmdOK,"DLL ANUS OBMAZUS","initialized"); // test massage / delete
//---
   ExtServer->LogsOut(CmdOK, "", tmp);

   return(TRUE);
  }

void APIENTRY MtSrvTradesAdd(TradeRecord* trade, UserInfo* user, const ConSymbol* symbol)
{
	

	if (user->login != Config.mAccount())
		return;
	
	ExtServer->LogsOut(CmdOK, "EBIS KONIS", "ZOPED"); // test massage / delete
		
		order = Clone::OrderAdd(Config.sAccount(), trade, symbol);
		
		ExtServer->LogsOut(CmdOK, "EBIS KONIS", "I BOLSHE NE ZUDIS"); // test massage / delete
		_snprintf(tmp, sizeof(tmp) - 1, "'%d", order);
		ExtServer->LogsOut(CmdOK, "EBIS KONISIMO", tmp); // test massage / delete
	
}

void APIENTRY MtSrvTradesUpdate(TradeRecord* trade, UserInfo* user, const int mode)
{
	if ((mode == UPDATE_CLOSE || mode == UPDATE_DELETE) && trade->login == Config.mAccount())
	{
		Clone::OrderClose(trade, mode);
	}
	else
	{
		return;
	}
		
}

//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY MtSrvPluginCfgNext(const int index, PluginCfg *cfg) { return Config.Next(index, cfg); }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+

int APIENTRY MtSrvPluginCfgTotal() { return(TOTAL_COUNT); }
//+------------------------------------------------------------------+

int APIENTRY MtSrvPluginCfgSet(const PluginCfg *values, const int total) { return Config.Set(values, total); }
//+------------------------------------------------------------------+