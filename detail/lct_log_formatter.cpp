/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_formatter.cpp
 * @version     1.0
 * @date        Jun 9, 2017 6:22:41 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/

#include <thread>
#include <iomanip>
#if !defined(WIN32) && !defined(_WIN32) && !defined(_WIN64) && !defined(WIN64)
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "lct_log_formatter.h"
#include "lct_log_stream.h"
#include "lct_logger.h"

static constexpr char FORMAT_DELIMITER_START   = '[';
static constexpr char FORMAT_DELIMITER_END     = ']';
static constexpr char FORMAT_DELIMITER_EMPTY   = ' ';
static constexpr char FORMAT_DELIMITER_COLON   = ':';
static constexpr char FORMAT_DELIMITER_DOT     = '.';

/*******************************************************************************
 *   Date & Time appenders
 * *****************************************************************************/
LCT_ERR_CODE CLCTLogFormatterDate::format(CLctLogStream& stream) const
{
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%y-%m-%d", &(stream.timestamp()));
   stream << FORMAT_DELIMITER_START << buf;
   return LCT_SUCCESS;
}

LCT_ERR_CODE CLCTLogFormatterTime::format(CLctLogStream& stream) const
{
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%X", &(stream.timestamp()));
   stream << FORMAT_DELIMITER_EMPTY << buf;
   return LCT_SUCCESS;
}

// milliseconds
LCT_ERR_CODE CLCTLogFormatterMillisecond::format(CLctLogStream& stream) const
{
   const auto& millis = std::chrono::duration_cast<std::chrono::milliseconds>(stream.timeDurationSinceEpoch()).count() % 1000;
   stream << FORMAT_DELIMITER_DOT << std::setfill('0') << std::setw(3) << millis << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

// microseconds
LCT_ERR_CODE CLCTLogFormatterMicrosecond::format(CLctLogStream& stream) const
{
   const auto& micros = std::chrono::duration_cast<std::chrono::microseconds>(stream.timeDurationSinceEpoch()).count() % 1000000;
   stream << FORMAT_DELIMITER_DOT << std::setfill('0') << std::setw(6) << micros << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

// nanoseconds
LCT_ERR_CODE CLCTLogFormatterNanosecond::format(CLctLogStream& stream) const
{
   const auto& ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stream.timeDurationSinceEpoch()).count() % 1000000000;
   stream << FORMAT_DELIMITER_DOT << std::setfill('0') << std::setw(9) << ns << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

/*******************************************************************************
 *   Thread & Process appenders
 * *****************************************************************************/

LCT_ERR_CODE CLCTLogFormatterThread::format(CLctLogStream& stream) const
{
   std::thread::id thisThreadId = std::this_thread::get_id();
   stream << FORMAT_DELIMITER_START << std::hex << "0x" << thisThreadId << std::dec << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

LCT_ERR_CODE CLCTLogFormatterPid::format(CLctLogStream& stream) const
{
#ifndef WIN32
   static pid_t thisPid = ::getpid();
#else
   static DWORD thisPid = GetCurrentProcessId();
#endif
   stream << FORMAT_DELIMITER_START << thisPid << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

/*******************************************************************************
 *   Logger name & Log level appenders
 * *****************************************************************************/

LCT_ERR_CODE CLCTLogFormatterLoggerName::format(CLctLogStream& stream) const
{
   stream << FORMAT_DELIMITER_START<< stream.loggerName() << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

LCT_ERR_CODE CLCTLogFormatterLogLevel::format(CLctLogStream& stream) const
{
   stream << FORMAT_DELIMITER_START << LCT_LOG_MGR->logLevelString(stream.logLevel()) << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}


LCT_ERR_CODE CLCTLogFormatterShortLogLevel::format(CLctLogStream& stream) const
{
   return LCT_SUCCESS;
}

/*******************************************************************************
 *   Location appenders
 * *****************************************************************************/
LCT_ERR_CODE CLCTLogFormatterLocation::format(CLctLogStream& stream) const
{
   stream << FORMAT_DELIMITER_START
         << stream.fileName()
         << FORMAT_DELIMITER_COLON
         << stream.lineNumber()
         << FORMAT_DELIMITER_COLON
         << stream.functionName()
         << FORMAT_DELIMITER_END;

   return LCT_SUCCESS;
}



