#pragma once

class Clone
{
public:
	static int UserGetInfo(int login, UserInfo *info);
	static int OrderAdd(int login, TradeRecord* trade, const ConSymbol* symbol);
	static int OrderAddReq(TradeTransInfo* trans, const UserInfo* user, int* request_id);
	static int OrderClose(TradeRecord *trade, int mode);
};