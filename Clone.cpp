#pragma once

#include "stdafx.h"
#include "Clone.h"

extern CServerInterface *ExtServer;

int Clone::UserGetInfo(int login, UserInfo *info)
{
	UserRecord user = { 0 };
	
	if (login<1 || info == NULL || ExtServer == NULL) return(FALSE);
	
	ZeroMemory(info, sizeof(UserInfo));
	
	if (ExtServer->ClientsUserInfo(login, &user) == FALSE) return(FALSE);

	info->login = user.login;

	info->enable = user.enable;
	info->enable_read_only = user.enable_read_only;

	info->leverage = user.leverage;
	info->agent_account = user.agent_account;
	info->credit = user.credit;
	info->balance = user.balance;
	info->prevbalance = user.prevbalance;

	COPY_STR(info->group, user.group);
	
	return(TRUE);
}

int Clone::OrderAdd(int login, TradeRecord* trade, const ConSymbol* symbol)
{
	UserInfo       info = { 0 };
	TradeRecord    trades = { 0 };
	int            order = 0;

	trades.login = login;
	trades.cmd = trade->cmd;
	trades.open_price = trade->open_price;
	trades.volume = trade->volume;
	trades.close_price = trade->close_price;
	trades.open_time = trade->open_time;
	trades.digits = trade->digits;
	COPY_STR(trades.symbol, trade->symbol);

	if (UserGetInfo(login, &info) == FALSE)
		return(0);

	order = ExtServer->OrdersAdd(&trades,&info,symbol);

	return(order);
}