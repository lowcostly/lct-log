/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_formatter.h
 * @version     1.0
 * @date        Jun 8, 2017 6:56:49 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_FORMATTER_H_
#define SRC_LOG_LCT_LOG_FORMATTER_H_

#include "lct_common_define.h"

template <typename CFormatType, typename CLogStream>
class CLctLogFormatter
{
public:
   CLctLogFormatter()
   {
   }

   virtual ~CLctLogFormatter()
   {
   }

   LCT_ERR_CODE format(CLogStream& stream) const
   {
      return static_cast<CFormatType*>(this)->Format(stream);
   }

};

class CLctLogStream;

/*******************************************************************************
 *   Date & Time appenders
 * *****************************************************************************/
class CLCTLogFormatterDate final:public CLctLogFormatter<CLCTLogFormatterDate, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CLCTLogFormatterTime final:public CLctLogFormatter<CLCTLogFormatterTime, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

// milliseconds
class CLCTLogFormatterMillisecond final:public CLctLogFormatter<CLCTLogFormatterMillisecond, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

// microseconds
class CLCTLogFormatterMicrosecond final:public CLctLogFormatter<CLCTLogFormatterMicrosecond, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

// nanoseconds
class CLCTLogFormatterNanosecond final:public CLctLogFormatter<CLCTLogFormatterNanosecond, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

/*******************************************************************************
 *   Thread & Process appenders
 * *****************************************************************************/
class CLCTLogFormatterThread final:public CLctLogFormatter<CLCTLogFormatterThread, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CLCTLogFormatterPid final:public CLctLogFormatter<CLCTLogFormatterPid, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};


/*******************************************************************************
 *   Logger Name & Log lever appenders
 * *****************************************************************************/
class CLCTLogFormatterLoggerName final:public CLctLogFormatter<CLCTLogFormatterLoggerName, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CLCTLogFormatterLogLevel final:public CLctLogFormatter<CLCTLogFormatterLogLevel, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CLCTLogFormatterShortLogLevel final:public CLctLogFormatter<CLCTLogFormatterShortLogLevel, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};


/*******************************************************************************
 *   Location appenders
 * *****************************************************************************/
class CLCTLogFormatterLocation final:public CLctLogFormatter<CLCTLogFormatterLocation, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};


#endif /* SRC_LOG_LCT_LOG_FORMATTER_H_ */
