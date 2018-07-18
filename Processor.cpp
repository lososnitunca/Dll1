#include "stdafx.h"
#include "Processor.h"
//---  server interface
extern CServerInterface *ExtServer;
//--- processor
CProcessor               ExtProcessor;
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CProcessor::CProcessor() :
	m_pend_set_allow(FALSE), m_pend_set_minpips(20),
	m_pend_mod_allow(FALSE), m_pend_mod_minpips(20),
	m_pend_del_allow(FALSE), m_pend_del_minpips(20),
	m_order_mod_allow(FALSE), m_order_mod_pips(20),
	m_order_close_loss_allow(FALSE), m_order_close_loss_minpips(20),
	m_order_close_profit_allow(FALSE), m_order_close_profit_minpips(20),
	m_order_closeby_allow(FALSE), m_order_multcloseby_allow(FALSE),
	m_reinitialize_flag(0), m_requests_total(0), m_requests_processed(0),
	m_volatiles(NULL), m_volatiles_total(0), m_volatiles_max(0),
	m_cfg(NULL)
{
	//--- fill user info
	ZeroMemory(&m_manager, sizeof(m_manager));
	m_manager.login = 55555;
	COPY_STR(m_manager.name, "Dealer Helper");
	COPY_STR(m_manager.ip, "DealerHelper");
	COPY_STR(m_symbols, "*");
	//---
}
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CProcessor::~CProcessor()
{
	//--- delete symbols
	m_sync.Lock();
	if (m_volatiles != NULL) { delete[]m_volatiles; m_volatiles = NULL; }
	m_volatiles_total = m_volatiles_max = 0;
	m_sync.Unlock();
	//---
}

void CProcessor::Initialize()
{
	//---
	ExtConfig.GetInteger("Order CloseLoss Allow", &m_order_close_loss_allow, "56");
	//ExtConfig.GetInteger("Order CloseLoss MinPips", &m_order_close_loss_minpips, "456");

	//PluginCfg *config = NULL;


	//--- receive symbols
	//ExtConfig.GetString("Symbols", (LPTSTR)m_symbols, sizeof(m_symbols) - 1, "*");
	//if (m_symbols[0] == 0) COPY_STR(m_symbols, "*");
	//--- notify
}