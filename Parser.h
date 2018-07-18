#pragma once
#include "stdafx.h"

#define MAX_BUFF 256
#define TOTAL_COUNT 2 // total count of plugin`s settings
#define MASTER_ACCOUNT "Master Account"
#define DEFAULT_MASTER "6"
#define SLAVE_ACCOUNT "Slave Account"
#define DEFAULT_SLAVE "7"

class ÑSync
{
private:
CRITICAL_SECTION  m_cs;

public:
	ÑSync() { ZeroMemory(&m_cs, sizeof(m_cs)); InitializeCriticalSection(&m_cs); }
	~ÑSync() { DeleteCriticalSection(&m_cs);   ZeroMemory(&m_cs, sizeof(m_cs)); }
	inline void       Lock() { EnterCriticalSection(&m_cs); }
	inline void       Unlock() { LeaveCriticalSection(&m_cs); }
};

class CConfig
{
private:

	ÑSync m_sync;
	boost::property_tree::ptree iniData;
	char fileName[256];
	int m_total, m_count;
	std::vector <PluginCfg> m_cfg;

public:
	 
	CConfig();
	~CConfig();

	int Open(char *adr);
	int Set(const PluginCfg *cfg, const int total);
	int Next(const int index, PluginCfg *cfg);
	int mAccount();
	int sAccount();

};

extern CConfig Config;