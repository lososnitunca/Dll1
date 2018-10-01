#pragma once

#include "stdafx.h"
#include "Clone.h"
#include "Parser.h"

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

int Clone::OrderRequest(TradeTransInfo *trans)
{
	UserRecord suser = { 0 }; // struct for slave account info
	int order = 0;

	order = ExtServer->ClientsUserInfo(Config.sAccount(), &suser);
	if (order == FALSE)
	{
		return(FALSE);
	}

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
		return(FALSE);
	}

	return(TRUE);
}

int Clone::OrderOpen(const int id)
{
	ExtServer->LogsOut(CmdOK, "MtSrvDealerConfirm START", NULL);

	int key = 0;    // ключ для определения новых запросов 
	RequestInfo req = { 0 };     // массив запросов 
	const int max = 1;      // максимально число запросов 
	char ooo[4] = "";

	int order = 0;
	order = ExtServer->RequestsGet(&key, &req, max);
	if (order == 0)
	{
		return(FALSE);
	}

	if (id != req.id)
	{
		return(FALSE);
	}

	_itoa(id, ooo, 4);
	ExtServer->LogsOut(CmdOK, "CONST INT ID: ", ooo);

	_itoa(req.id, ooo, 4);
	ExtServer->LogsOut(CmdOK, "REQ_ID: ", ooo);

	if (req.login != Config.sAccount())
	{
		return(FALSE);
	}

	UserInfo info = { 0 };
	order = Clone::UserGetInfo(Config.sAccount(), &info);
	if (order == FALSE)
	{
		return(FALSE);
	}

	order = ExtServer->OrdersOpen(&(req.trade), &info);
	if (order == 0)
	{
		ExtServer->LogsOut(CmdOK, "WE HAVE TROUBLE WITH OPENING NEW ORDER FOR SLAVE USER", NULL);
	}

	ExtServer->RequestsDelete(req.id);

	return(TRUE);
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
	int            order = (trade->order)-1;

	order = ExtServer->OrdersGet(order, &trades);
	_snprintf(trades.comment, sizeof(trades.comment) - 1, "%d", trades.order);
		
	if (trades.open_time != trade->open_time &&
		trades.close_time != trade->close_time &&
		trades.api_data != trade->api_data)
	{
		return(FALSE);
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

	return(TRUE);
}