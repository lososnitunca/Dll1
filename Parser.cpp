#pragma once

#include "stdafx.h"
#include "Parser.h"

extern CServerInterface *ExtServer;
CConfig Config;

CConfig::CConfig() : m_total(TOTAL_COUNT), m_count(0) 
{
	m_sync.Lock();

	m_cfg.insert(m_cfg.end(), { MASTER_ACCOUNT, DEFAULT_MASTER }); //default values for plugin`s configuration // master account
	m_cfg.insert(m_cfg.end(), { SLAVE_ACCOUNT, DEFAULT_SLAVE }); // slave account
	
	m_sync.Unlock();
}

CConfig::~CConfig()
{
}

int CConfig::Open(char *adr)
{
	
	m_sync.Lock();

	COPY_STR(fileName, adr);
	ExtServer->LogsOut(CmdOK, "JOPA Adress: ", fileName);

	try
	{
		boost::property_tree::read_ini(adr, iniData);

		if (const boost::optional<std::string> key = iniData.get_optional<std::string>(MASTER_ACCOUNT))
		{
			if (key.get() != "")
			{
				memcpy(m_cfg[m_count++].value, &key.get(),sizeof(char[128])); // m_cfg[0]
			}
		}

		if (const boost::optional<std::string> key = iniData.get_optional<std::string>(SLAVE_ACCOUNT))
		{
			if (key.get() != "")
			{
				memcpy(m_cfg[m_count].value, &key.get(), sizeof(char[128])); // m_cfg[1]
			}
		}
		ExtServer->LogsOut(CmdOK, "3433 Value: ", m_cfg[m_count].value); // test massage / delete
	}
	catch (boost::property_tree::ini_parser_error& error) // if file is not found - creat default ini file
	{

		for (int i = 0; i < TOTAL_COUNT; i++)
		{
			iniData.put(m_cfg[i].name, m_cfg[i].value); //default values
		}

		boost::property_tree::write_ini(adr, iniData);
	}
	m_sync.Unlock();
	return(0);
}


int CConfig::Set(const PluginCfg *cfg, const int total)
{
	ExtServer->LogsOut(CmdOK, "AHUET: SEEEEEET", NULL);

	if (total <= 1) return(FALSE);

	m_sync.Lock();
	
	if (cfg != NULL && total <= TOTAL_COUNT)
	{
		ExtServer->LogsOut(CmdOK, "AHUET: SEEEEEET", NULL);

		std::vector <PluginCfg> buf(2); // creat buffer for settings to check them
		memcpy(buf.data(), cfg, sizeof(PluginCfg)*total); // copy settings from admin`s pluging menu to buffer

		if ((std::find_if(
		buf.begin(),
			buf.end(),
			boost::bind(
				&boost::iequals<std::string, std::string>,
				boost::bind(&PluginCfg::name, _1),
				MASTER_ACCOUNT,
				std::locale()
			))) == buf.end() || //searching master account setting
			(std::find_if(
				buf.begin(),
				buf.end(),
				boost::bind(
					&boost::iequals<std::string, std::string>,
					boost::bind(&PluginCfg::name, _1),
					SLAVE_ACCOUNT,
					std::locale()
				))) == buf.end()) //searching slave account setting
		
		{
			ExtServer->LogsOut(CmdOK, "ANUS V OGNE", NULL);
			m_sync.Unlock();
			return(FALSE);
		}

		//memcpy(m_cfg.data(), cfg, sizeof(PluginCfg)*total);
		m_cfg.swap(buf);

		for (auto i : m_cfg) { iniData.put(i.name, i.value); 
		ExtServer->LogsOut(CmdOK, "AHUET: SEEEEEET", i.name);
		ExtServer->LogsOut(CmdOK, "AHUET: SEEEEEET", i.value); }//default value
		ExtServer->LogsOut(CmdOK, "AHUET: WRITE ADRESS", fileName);
		
		boost::property_tree::write_ini(fileName, iniData);
		
		m_sync.Unlock();
		return(TRUE);
	}
	m_sync.Unlock();
	return(FALSE);
}

int CConfig::Next(const int index, PluginCfg *cfg)
{
	ExtServer->LogsOut(CmdOK, "AHUET: NEEEEEXTT", NULL);
	//--- check
	if (cfg != NULL && index >= 0)
	{
		m_sync.Lock();
		if (m_cfg.empty() != TRUE && index<TOTAL_COUNT)
		{
			memcpy(cfg, &m_cfg[index], sizeof(PluginCfg));
			m_sync.Unlock();
			return(TRUE);
		}
		m_sync.Unlock();
	}
	//--- fail
	return(FALSE);
}

int CConfig::mAccount()
{
	std::vector<PluginCfg>::iterator it = std::find_if(
		m_cfg.begin(),
		m_cfg.end(),
		boost::bind(
			&boost::iequals<std::string, std::string>,
			boost::bind(&PluginCfg::name, _1),
			MASTER_ACCOUNT,
			std::locale()
		));
	int value = atoi(it->value);
	return(value);
}

int CConfig::sAccount()
{
	std::vector<PluginCfg>::iterator it = std::find_if(
		m_cfg.begin(),
		m_cfg.end(),
		boost::bind(
			&boost::iequals<std::string, std::string>,
			boost::bind(&PluginCfg::name, _1),
			SLAVE_ACCOUNT,
			std::locale()
		));
	int value = atoi(it->value);
	return(value);
}