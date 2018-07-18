//+------------------------------------------------------------------+
//|                                            MetaTrader Server API |
//|                   Copyright 2001-2014, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "MT4ServerAPI.h"

//--- macros
#define COPY_STR(dst,src) { strncpy(dst,src,sizeof(dst)-1); dst[sizeof(dst)-1]=0; }
//+------------------------------------------------------------------+

