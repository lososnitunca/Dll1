#include "stdafx.h"
#include "Configuration.h"

extern CServerInterface *ExtServer;
CConfiguration ExtConfig;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CConfiguration::CConfiguration() : m_cfg(NULL), m_cfg_total(0), m_cfg_max(0), i(0)
{
	m_filename[0] = 0;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CConfiguration::~CConfiguration()
{
	m_sync.Lock();
	//---
	if (m_cfg != NULL) { delete[] m_cfg; m_cfg = NULL; }
	//m_cfg_total = m_cfg_max = 0;
	i = m_cfg_max = 0;
	//---
	m_sync.Unlock();
}
//+------------------------------------------------------------------+
//| �� ������ ������ ������������, �� ������ �� ������ � ���         |
//+------------------------------------------------------------------+
void CConfiguration::Load()
{
	//char        tmp[MAX_PATH], *cp, *start;
	PluginCfg   cfg, *buf;
	//--- ��������
	//if (filename == NULL) return;
	//--- �������� ��� ����������������� �����
	m_sync.Lock();
	//COPY_STR(m_filename, filename);
	//--- ������� ����
	
			COPY_STR(cfg.name, "2");
			//--- ����� ��������� �������
			
			COPY_STR(cfg.value, "3");
			
	m_sync.Unlock();
}
//+------------------------------------------------------------------+
//| ����� �������� �� ����                                           |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| ������������� ����� �� �����                                     |
//+------------------------------------------------------------------+
PluginCfg* CConfiguration::Search(LPCSTR name)
{
	PluginCfg *config = NULL;
	//---
	if (m_cfg != NULL && m_cfg_total>0)
		config = (PluginCfg*)bsearch(name, m_cfg, m_cfg_total, sizeof(PluginCfg), SearchByName);
	//---
	return(config);
}
//+------------------------------------------------------------------+
//| ����������/����������� �������                                   |
//+------------------------------------------------------------------+
//int CConfiguration::Add(const PluginCfg *cfg)
int CConfiguration::Add(PluginCfg *cfg)
{
	char tmp[256];
	PluginCfg *config, *buf;
	//--- ��������
	if (cfg == NULL || cfg->name[0] == 0) return(FALSE);
	//---
	m_sync.Lock();
	//if ((config = Search(cfg->name)) != NULL) memcpy(config, cfg, sizeof(PluginCfg));
	//else
	{
		//--- ����� ����?
		if (m_cfg == NULL || m_cfg_total >= m_cfg_max)
		//if (m_cfg == NULL || i >= m_cfg_max)
		{
			//--- ������� �����
			if ((buf = new PluginCfg[m_cfg_total + 64]) == NULL) { m_sync.Unlock(); return(FALSE); }
			//if ((buf = new PluginCfg[i + 64]) == NULL) { m_sync.Unlock(); return(FALSE); }
			//--- ��������� ������� �� ������� ������
			if (m_cfg != NULL)
			{
				if (m_cfg_total>0) memcpy(buf, m_cfg, sizeof(PluginCfg)*m_cfg_total);
				//if (i>0) memcpy(buf, m_cfg, sizeof(PluginCfg)*i);
				delete[] m_cfg;
			}
			//--- ������� �����
			m_cfg = buf;
			m_cfg_max = m_cfg_total + 64;
			//m_cfg_max = i + 64;
		}
		//--- ��������� � �����

		COPY_STR(cfg->name, "333");
		COPY_STR(cfg->value, "333");

		_snprintf(tmp, sizeof(tmp) - 1, "%d", i);
		ExtServer->LogsOut(CmdOK, "TOTAL DO PERVOGO", tmp); // test massage / delete

		memcpy(&m_cfg[m_cfg_total++], cfg, sizeof(PluginCfg));
		//memcpy(&m_cfg[i++], cfg, sizeof(PluginCfg));

		_snprintf(tmp, sizeof(tmp) - 1, "%d", i);
		ExtServer->LogsOut(CmdOK, "TOTAL POSLE PERVOGO", tmp); // test massage / delete

		//memcpy(&m_cfg[0], cfg, sizeof(PluginCfg));
		//--- �������������

		COPY_STR(cfg->name, "444");
		COPY_STR(cfg->value, "444");

		memcpy(&m_cfg[m_cfg_total++], cfg, sizeof(PluginCfg));

		//qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
	}
	m_sync.Unlock();
	//--- ����������, ��������������
	//--- �������
	return(TRUE);
}
//+------------------------------------------------------------------+
//| ���������� ����� ��������                                        |
//+------------------------------------------------------------------+
int CConfiguration::Set(const PluginCfg *values, const int total)
{
	//--- ��������
	if (total<0) return(FALSE);
	//---
	m_sync.Lock();
	if (values != NULL && total>0)
	{
		//--- ����� ����?
		if (m_cfg == NULL || total >= m_cfg_max)
		{
			//--- ������ ������ � ������� ����� �����
			if (m_cfg != NULL) delete[] m_cfg;
			if ((m_cfg = new PluginCfg[total + 64]) == NULL)
			{
				m_cfg_max = m_cfg_total = 0;
				m_sync.Unlock();
				return(FALSE);
			}
			//--- �������� ����� ������
			m_cfg_max = total + 64;
		}
		//--- ��������� ���� ������
		memcpy(m_cfg, values, sizeof(PluginCfg)*total);
	}
	//--- �������� ����� ���������� � �������������
	m_cfg_total = total;
	if (m_cfg != NULL && m_cfg_total>0) qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
	m_sync.Unlock();
	//--- ����������
	
	return(TRUE);
}
//+------------------------------------------------------------------+
//| ���� ������ �� �����                                             |
//+------------------------------------------------------------------+
int CConfiguration::Get(LPCSTR name, PluginCfg *cfg)
{
	PluginCfg *config = NULL;
	//--- ��������
	if (name != NULL && cfg != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL) memcpy(cfg, config, sizeof(PluginCfg));
		m_sync.Unlock();
	}
	//--- ������ ���������
	return(config != NULL);
}
//+------------------------------------------------------------------+
//| ���� ������                                                      |
//+------------------------------------------------------------------+
int CConfiguration::Next(const int index, PluginCfg *cfg)
{
	//--- ��������
	if (cfg != NULL && index >= 0)
	{
		m_sync.Lock();
		if (m_cfg != NULL && index<m_cfg_total)
		{
			memcpy(cfg, &m_cfg[index], sizeof(PluginCfg));
			m_sync.Unlock();
			return(TRUE);
		}
		m_sync.Unlock();
	}
	//--- �������
	return(FALSE);
}
//+------------------------------------------------------------------+
//| ������� ������                                                   |
//+------------------------------------------------------------------+
int CConfiguration::Delete(LPCSTR name)
{
	PluginCfg *config = NULL;
	//--- ��������
	if (name != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL)
		{
			int index = config - m_cfg;
			if ((index + 1)<m_cfg_total) memmove(config, config + 1, sizeof(PluginCfg)*(m_cfg_total - index - 1));
			m_cfg_total--;
		}
		//--- �������������
		if (m_cfg != NULL && m_cfg_total>0) qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
		m_sync.Unlock();
	}
	//--- ������ ���������
	return(config != NULL);
}
//+------------------------------------------------------------------+
//| ���������� �� �����                                              |
//+------------------------------------------------------------------+
int CConfiguration::SortByName(const void *left, const void *right)
{
	return strcmp(((PluginCfg*)left)->name, ((PluginCfg*)right)->name);
}
//+------------------------------------------------------------------+
//| ����� �� �����                                                   |
//+------------------------------------------------------------------+
int CConfiguration::SearchByName(const void *left, const void *right)
{
	return strcmp((char*)left, ((PluginCfg*)right)->name);
}
//+------------------------------------------------------------------+
//| ���� ������ �� �����                                             |
//+------------------------------------------------------------------+
int CConfiguration::GetInteger(LPCSTR name, int *value, LPCSTR defvalue)
{
	PluginCfg *config = NULL;
	//--- ��������
	//if (name != NULL && value != NULL)
	{
		m_sync.Lock();
	//	if ((config = Search(name)) != NULL) *value = atoi(config->value);
	//	else
			if (defvalue != NULL)
			{
				m_sync.Unlock();
				//--- �������� ����� ������
				PluginCfg cfg = { 0 };
				//COPY_STR(cfg.name, name);
				COPY_STR(cfg.name, name);
				//COPY_STR(cfg.value, defvalue);
				COPY_STR(cfg.value, defvalue);
				Add(&cfg);
				//--- �������� �������� �� ��������� � ��������
				*value = atoi(cfg.value);
				return(TRUE);
			}
		m_sync.Unlock();
	}
	//--- ������ ���������

	/*m_sync.Lock();
	PluginCfg cfg = { 0 };
	PluginCfg *buf;
	COPY_STR(cfg.name, "fename");
	COPY_STR(cfg.value, "1");

	buf = new PluginCfg[64];
	m_cfg = buf;
	memcpy(&m_cfg[0], &cfg, sizeof(PluginCfg));

	m_sync.Unlock();*/

	return(config != NULL);
}
//+------------------------------------------------------------------+
//| ���� ������ �� �����                                             |
//+------------------------------------------------------------------+
int CConfiguration::GetString(LPCSTR name, LPTSTR value, const int maxlen, LPCSTR defvalue)
{
	PluginCfg *config = NULL;
	//--- ��������
	if (name != NULL && value != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL)
		{
			strncpy((char*)value, config->value, maxlen);
			value[maxlen] = 0;
		}
		else
			if (defvalue != NULL)
			{
				m_sync.Unlock();
				//--- �������� ����� ������
				PluginCfg cfg = { 0 };
				COPY_STR(cfg.name, name);
				COPY_STR(cfg.value, defvalue);
				Add(&cfg);
				//--- �������� �������� �� ��������� � ��������
				strncpy((char*)value, cfg.value, maxlen);
				value[maxlen] = 0;
				return(TRUE);
			}
		m_sync.Unlock();
	}
	//--- ������ ���������
	return(config != NULL);
}
//+------------------------------------------------------------------+