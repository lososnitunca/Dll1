#include "Configuration.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#define PRICE_BUFFER_SIZE  7

struct SymbolVolatile
{
	char              symbol[16];
	double            buffer[PRICE_BUFFER_SIZE];
	int               buffer_size;
	int               buffer_position;
	double            last_price;
	int               process_allow;
};
//+------------------------------------------------------------------+
//| Plugin configuration base                                        |
//+------------------------------------------------------------------+
class CProcessor
{
private:
	PluginCfg * m_cfg;                  // массив записей

public:
	//--- dealer helper user info
	UserInfo          m_manager;
	//--- set pending
	int               m_pend_set_allow;                // allow to setup new pending orders
	int               m_pend_set_minpips;              // minimum points from market
													   //--- modify pending
	int               m_pend_mod_allow;                // allow to modify pending orders
	int               m_pend_mod_minpips;              // minimum points from market
													   //--- delete pending
	int               m_pend_del_allow;                // allow to delete pending orders
	int               m_pend_del_minpips;              // minimum points from market
													   //--- modify orders
	int               m_order_mod_allow;               // allow to modify SL & TP of opened positions
	int               m_order_mod_pips;                // minimum points from market
													   //--- close loss orders
	int               m_order_close_loss_allow;        // allow to close positions with Losses
	int               m_order_close_loss_minpips;      // minimum points from market
													   //--- close profit orders
	int               m_order_close_profit_allow;      // allow to close positions with Profits
	int               m_order_close_profit_minpips;    // minimum points from market
													   //--- close by
	int               m_order_closeby_allow;           // allow to close by operations
	int               m_order_multcloseby_allow;       // allow to multiple close by operations
													   //--- statistic
	LONG              m_reinitialize_flag;             //
	int               m_requests_total;                // number of all transactions
	int               m_requests_processed;            // number of processed transactions
													   //--- 
	SymbolVolatile   *m_volatiles;
	int               m_volatiles_total;
	int               m_volatiles_max;
	//---
	char              m_symbols[256];
	//---
	CSync             m_sync;

public:
	CProcessor();
	~CProcessor();

	void              Initialize();
	inline void       Reinitialize() { InterlockedExchange(&m_reinitialize_flag, 1); }

};

extern CProcessor ExtProcessor;
