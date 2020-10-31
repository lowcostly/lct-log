/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_stream.cpp
 * @version     1.0
 * @date        Jun 9, 2017 11:41:09 AM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log_stream.h"
#include "lct_time.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(WIN64) || defined(__APPLE__)
static const std::string program_invocation_short_name = "mercury_plugin";
#endif

static constexpr char LOG_ENDL = '\n';

CLctLogStream::CLctLogStream(const ELogLevel level,
      const char* const file,
      const char* const function,
      const int32_t line):
      m_logLevel(level),
      m_line(line),
      m_fileNm(file),
      m_functionNm(function),
      m_loggerName(program_invocation_short_name),
      m_timepoint(CLctLogClock::now()),
      m_timestampDuration(m_timepoint.time_since_epoch())
{
   m_timestamp = LCT_TIME->timePoint2Tm(m_timepoint);
}

CLctLogStream::CLctLogStream(const std::string& loggerNm,
      const ELogLevel level,
      char const* const file,
      const char* const function,
      const int32_t line):
      m_logLevel(level),
      m_line(line),
      m_fileNm(file),
      m_functionNm(function),
      m_loggerName(loggerNm),
      m_timepoint(CLctLogClock::now()),
      m_timestampDuration(m_timepoint.time_since_epoch())
{
   m_timestamp = LCT_TIME->timePoint2Tm(m_timepoint);
}

CLctLogStream::~CLctLogStream()
{
}

LCT_ERR_CODE CLctLogStream::logEnd()
{
   m_sstream << LOG_ENDL;
   m_content = m_sstream.str();
   m_sstream.str("");
   m_sstream.clear();
   return LCT_SUCCESS;
}

const CLctLogMessageSp& CLctLogStream::messagePtr()
{
   if (m_messageSp == nullptr) {
      m_messageSp = std::make_shared<CLctLogMessage>(m_sstream.str() + m_content);
   }
   return m_messageSp;
}


const std::string& CLctLogStream::message()
{
   return *(messagePtr());
}
