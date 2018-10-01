#pragma once

class Clone
{
public:
	static int UserGetInfo(int login, UserInfo *info);
	static int OrderRequest(TradeTransInfo *trans);
	static int OrderOpen(const int id);
	static int OrderClose(TradeRecord *trade, int mode);
};