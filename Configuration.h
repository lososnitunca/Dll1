#pragma once

//+------------------------------------------------------------------+
//| Simple synchronizer                                              |
//+------------------------------------------------------------------+
class CSync
{
private:
	CRITICAL_SECTION  m_cs;

public:
	CSync() { ZeroMemory(&m_cs, sizeof(m_cs)); InitializeCriticalSection(&m_cs); }
	~CSync() { DeleteCriticalSection(&m_cs);   ZeroMemory(&m_cs, sizeof(m_cs)); }
	inline void       Lock() { EnterCriticalSection(&m_cs); }
	inline void       Unlock() { LeaveCriticalSection(&m_cs); }
};
//+------------------------------------------------------------------+
//| Simple configuration                                             |
//+------------------------------------------------------------------+
class CConfiguration
{
private:
	CSync             m_sync;                 // �������������
	char              m_filename[MAX_PATH];   // ��� ����� ������������
	PluginCfg        *m_cfg;                  // ������ �������
	int               m_cfg_total;            // ����� ���������� �������
	int               m_cfg_max;              // ������������ ���������� �������
	int i; // ����� ���������� �������

public:
	CConfiguration();
	~CConfiguration();
	//--- ������������� ���� (������ ������ �����)
	void              Load();
	//--- ������ � �������
	//int               Add(const PluginCfg* cfg);
	int               Add(PluginCfg* cfg);
	int               Set(const PluginCfg *values, const int total);
	int               Get(LPCSTR name, PluginCfg* cfg);
	int               Next(const int index, PluginCfg* cfg);
	int               Delete(LPCSTR name);
	inline int        Total(void) { m_sync.Lock(); int total = m_cfg_total; m_sync.Unlock(); return(total); }
	//inline int Total(void) { m_sync.Lock(); int total = i; m_sync.Unlock(); return(i); }

	int               GetInteger(LPCSTR name, int *value, LPCSTR defvalue = NULL);
	int               GetString(LPCSTR name, LPTSTR value, const int maxlen, LPCSTR defvalue = NULL);

private:
	
	PluginCfg*        Search(LPCSTR name);
	static int        SortByName(const void *left, const void *right);
	static int        SearchByName(const void *left, const void *right);
};

extern CConfiguration ExtConfig;
//+------------------------------------------------------------------+
