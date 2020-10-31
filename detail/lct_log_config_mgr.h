/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_config_mgr.h
 * @version     1.0
 * @date        Jul 20, 2017 3:55:29 AM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_CONFIG_MGR_H_
#define SRC_LOG_LCT_LOG_CONFIG_MGR_H_

#include "lct_singleton.h"

class CLctLogConfigMgr final:public CLctSingleton<CLctLogConfigMgr>
{
public:
   LCT_ERR_CODE init(const std::string& configFile);

   bool traceLevel() const
   {
      return m_traceLevel;
   }

   bool debugLevel() const
   {
      return m_debugLevel;
   }

   bool inforLevel() const
   {
      return m_inforLevel;
   }

   bool warngLevel() const
   {
      return m_warngLevel;
   }

   bool errorLevel() const
   {
      return m_errorLevel;
   }

   bool critcLevel() const
   {
      return m_critcLevel;
   }

private:
#ifdef WIN32
   template <typename ClassType>
#endif
   friend class CLctSingleton;
   CLctLogConfigMgr();
   ~CLctLogConfigMgr();

   bool               m_traceLevel  = true;
   bool               m_debugLevel  = true;
   bool               m_inforLevel  = true;
   bool               m_warngLevel  = true;
   bool               m_errorLevel  = true;
   bool               m_critcLevel  = true;
};

#define LCT_LOG_CONFIG_MGR CLctLogConfigMgr::instance()


#endif /* SRC_LOG_LCT_LOG_CONFIG_MGR_H_ */
