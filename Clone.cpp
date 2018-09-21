#pragma once

#include "stdafx.h"
#include "Clone.h"

extern CServerInterface *ExtServer;

int Clone::UserGetInfo(int login, UserInfo *info)
{
	UserRecord user = { 0 };
	
	if (login < 1 ||
		info == NULL ||
		ExtServer == NULL)
	{
		return(FALSE);
	}
	
	ZeroMemory(info, sizeof(UserInfo));
	
	if (ExtServer->ClientsUserInfo(login, &user) == FALSE)
	{
		return(FALSE);
	}

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

int Clone::OrderAdd(int login, TradeTransInfo *trans)
{
	if (login < 1 ||
		trans == NULL ||
		ExtServer == NULL)
	{
		return(FALSE);
	}
	
	UserInfo       info = { 0 };
	TradeRecord    trades = { 0 };
	//TradeTransInfo trans = { 0 };
	int            order = 0;

	//trades = *trade;
	//trades.login = login;
	//trades.order = 0;
	//trades.gw_order = 0;

	//_snprintf(trades.comment, sizeof(trades.comment) - 1, "%d+%d+%d+%d+%d", trade->order, trade->api_data[0], trade->api_data[1], trade->api_data[2], trade->api_data[3]);
		
	if (UserGetInfo(login, &info) == FALSE)
	{
		return(0);
	}

	//order = ExtServer->OrdersAdd(&trades,&info,symbol);
	//order = ExtServer->OrdersOpen(trans, &info);
	if(order!=0)ExtServer->LogsOut(CmdOK, "I HOPE I SUCCESSED", NULL);

	return(order);
}

int Clone::OrderAddReq(TradeTransInfo* trans, const UserInfo* user, int* request_id)
{
	
	if (trans == NULL ||
		user == NULL ||
		request_id == NULL)
	{
		return(FALSE);
	}

	RequestInfo request = { 0 };
	const int isdemo = 1;

	UserInfo       info = { 0 };
	TradeTransInfo    trades = { 0 };
	int            order = 0;

	//trades = *trans;

	request.trade = *trans;
	request.login = 7;
	request.id = *request_id;
	request.manager = 1;

	//trades.order = 0;
	//trades.gw_order = 0;

	//if (UserGetInfo(login, &info) == FALSE)
	//{
	//	return(0);
	//}
	ExtServer->LogsOut(CmdOK, "I HOPE I TRANS INTO TRADE", NULL);
	order = ExtServer->RequestsAdd(&request, isdemo, request_id);
	char oo[4]="";
	_itoa(order, oo, 10);
	ExtServer->LogsOut(CmdOK, "I HOPE I TRANS INTO TRADE ANSWER:    ", oo);

	return(order);
}

int Clone::OrderClose(TradeRecord *trade, int mode)
{
	if (trade == NULL ||
		ExtServer == NULL)
	{
		return(FALSE);
	}

	UserInfo       info = { 0 };
	TradeRecord    trades = { 0 };
	TradeTransInfo trans = { 0 };
	int            order = (trade->order)+1;

	order = ExtServer->OrdersGet(order, &trades);
	_snprintf(trades.comment, sizeof(trades.comment) - 1, "%d", trades.order);
		
	if (trades.open_time != trade->open_time &&
		trades.close_time != trade->close_time &&
		trades.api_data != trade->api_data)
	{
		return FALSE;
	}
	
	if (UserGetInfo(trades.login, &info) == FALSE)
	{
		return(0);
	}

	switch (mode)
	{
	case UPDATE_CLOSE:
		trans.order = trades.order;
		trans.volume = trade->volume;
		trans.price = trade->close_price;
		ExtServer->OrdersClose(&trans, &info);
		break;
	case UPDATE_DELETE:
		_snprintf(trades.comment, sizeof(trades.comment) - 1, "%d", trades.order);
		ExtServer->OrdersUpdate(&trades, &info, UPDATE_DELETE);
		break;
	}

	return TRUE;
}