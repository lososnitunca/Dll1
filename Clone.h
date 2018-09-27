#pragma once

class Clone
{
public:
	static int UserGetInfo(int login, UserInfo *info);
	static int OrderClose(TradeRecord *trade, int mode);
};