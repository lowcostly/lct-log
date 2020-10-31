/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_config_mgr.cpp
 * @version     1.0
 * @date        Jul 20, 2017 3:55:29 AM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/

#include "lct_properties.h"
#include "lct_log_config_mgr.h"

LCT_ERR_CODE CLctLogConfigMgr::init(const std::string& configFile)
{
   CLctProperties properties(configFile);
   LCT_ERR_CODE errCode = properties.parse();
   if (LCT_SUCCESS != errCode) {
      std::cout << "Failed to parse Lct Log properties file(" << configFile
            << ") due to error(" << ErrCodeFormat(errCode) << ")" << std::endl;
      return errCode;
   } else {
      std::cout << "Successful to load Lct Log properties file" << std::endl;
   }

   m_traceLevel  = properties.getBool("LctLog.LevelEnable.Trace", true);
   m_debugLevel  = properties.getBool("LctLog.LevelEnable.Debug", true);
   m_inforLevel  = properties.getBool("LctLog.LevelEnable.Infor", true);
   m_warngLevel  = properties.getBool("LctLog.LevelEnable.Warng", true);
   m_errorLevel  = properties.getBool("LctLog.LevelEnable.Error", true);
   m_critcLevel  = properties.getBool("LctLog.LevelEnable.Critc", true);
  
   return LCT_SUCCESS;
}

CLctLogConfigMgr::CLctLogConfigMgr()
{
}

 CLctLogConfigMgr::~CLctLogConfigMgr()
{
}


