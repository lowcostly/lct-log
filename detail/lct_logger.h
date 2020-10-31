/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_logger.h
 * @version     1.0
 * @date        Jun 8, 2017 6:39:56 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOGGER_H_
#define SRC_LOG_LCT_LOGGER_H_

#include <map>
#include "lct_singleton.h"
#include "lct_log_formatter.h"
#include "lct_log_formatter_pool.h"
#include "lct_log_sink.h"
#include "lct_log_sink_pool.h"
#include "lct_log_stream.h"

typedef CLctLogSinkPool<
         CDailyRotateTerminalSinkSp,
         CDailyRotateFileSinkSp>    CLctLogSinkPoolType;

typedef CLctLogFormatterPool<
         CLCTLogFormatterDate,
         CLCTLogFormatterTime,
         CLCTLogFormatterMicrosecond,
         CLCTLogFormatterLogLevel,
         CLCTLogFormatterThread,
         CLCTLogFormatterLocation>    CLctLogFormatterPoolType;

struct CLctLog
{
   bool operator == (CLctLogStream&) const;
};


static constexpr int32_t LOG_ROTATE_FILE_MAX_SIZE   = 1024 * 1024 * 10;
static constexpr int32_t LOG_ROTATE_FILE_MAX_COUNT   = 60;

class CLctLogger final: public CLctSingleton<CLctLogger>
{
public:
   bool isLogged(const ELogLevel) const;

   LCT_ERR_CODE init(const std::string& logLocalDir, const int64_t rotateMaxFileSize = LOG_ROTATE_FILE_MAX_SIZE,
      const int64_t rotateMaxFileCount = LOG_ROTATE_FILE_MAX_COUNT);

   LCT_ERR_CODE init(const std::string& logLocalDir, const std::string logFileBaseName, const int64_t rotateMaxFileSize = LOG_ROTATE_FILE_MAX_SIZE,
      const int64_t rotateMaxFileCount = LOG_ROTATE_FILE_MAX_COUNT);

   LCT_ERR_CODE init(const std::string& logLocalDir, const std::string& configFile, const std::string logFileBaseName, const int64_t rotateMaxFileSize = LOG_ROTATE_FILE_MAX_SIZE,
         const int64_t rotateMaxFileCount = LOG_ROTATE_FILE_MAX_COUNT);

   LCT_ERR_CODE close();

   LCT_ERR_CODE log(CLctLogStream&);

   const std::string logLevelString(const ELogLevel level) const;

protected:
   LCT_ERR_CODE initSinks(const std::string& logLocalDir, const std::string logFileBaseName, const int64_t rotateMaxFileSize,  const int64_t rotateMaxFileCount);
   LCT_ERR_CODE initFormatter();
   LCT_ERR_CODE initLogLevel();
   LCT_ERR_CODE initLogLevelByDefault();

private:
#ifdef WIN32
   template <typename ClassType>
#endif
   friend class CLctSingleton;
   CLctLogger();
   ~CLctLogger();

private:
   bool                      m_isInitiated = false;
   CLctLogFormatterPoolType  m_formatterPool;
   CLctLogSinkPoolType       m_sinkPool;

   typedef std::map<ELogLevel, std::string> CLogLevelContainer;
   CLogLevelContainer        m_loglevels;
   std::ostream&             m_defaultStream = std::cout;
};

#define LCT_LOG_MGR CLctLogger::instance()

#include <cstring>
#define __LCT_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define CLCT_LOG(LEVEL) CLctLog() == CLctLogStream(LEVEL, __LCT_FILENAME__, __func__, __LINE__)

#include "lct_logger.inl"

#endif /* SRC_LOG_LCT_LOGGER_H_ */
