/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_sink.h
 * @version     1.0
 * @date        Jun 8, 2017 7:01:33 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_SINK_H_
#define SRC_LOG_LCT_LOG_SINK_H_

#include <time.h>
#include <thread>
#include "lct_common_define.h"
#include "lct_singleton.h"
#include "lct_queue.h"
#include "lct_file_helper.h"
#include "lct_log_stream.h"
#include "lct_time.h"

static constexpr const char* const LOG_FILE_SUFFIX     = ".log";
static constexpr const char* const LOG_FILE_NAME_DELIM  =  "_";

template <typename CSinkType, typename CLogMessage>
class CLctLogSink
{
public:
   CLctLogSink()
   {
   }

   virtual ~CLctLogSink()
   {
   }

   LCT_ERR_CODE log(const CLogMessage& message) 
   {
      return static_cast<CSinkType*>(this)->log(message);
   }

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctLogSink);
};


/*******************************************************************************
 *
 *   CRotateFileSink
 *
 * *****************************************************************************/

class CRotateFileSink final: public CLctLogSink<CRotateFileSink, CLctLogMessageSp>
{
public:
   CRotateFileSink(const bool forceFlush, const std::string& fileDir, const std::string& baseFileName,
      const int64_t maxSize, const int64_t maxCount);
   ~CRotateFileSink();

   LCT_ERR_CODE log(const CLctLogMessageSp& message)
   {
      std::lock_guard<CRotateMutex> lk(m_mutex);
      return sinkIt(message);
   }

protected:
   LCT_ERR_CODE sinkIt(const CLctLogMessageSp& message)
   {
      LCT_ERR_CODE errCode = LCT_SUCCESS;
      m_currentSize += message->size();
      if (m_currentSize > m_maxSize) {
         rotate();
         m_currentSize = message->size();
      }
      errCode = m_fileHelper.write(message);

      if (m_forceFlush) {
         errCode = flush();
      }

      return errCode;
   }

   LCT_ERR_CODE flush();

private:
   std::string calcFileName(const int64_t index) const;

   LCT_ERR_CODE rotate();

private:
   const bool                m_forceFlush;
   const int64_t             m_maxSize;
   const int64_t             m_maxFiles;
   int64_t                   m_currentSize;
   const std::string         m_fileDir;
   const std::string         m_baseFileName;
   CFileHelper               m_fileHelper;

   //typedef std::mutex        CRotateMutex;
   typedef CNullMutex        CRotateMutex;
   mutable CRotateMutex      m_mutex;
};

typedef std::shared_ptr<CRotateFileSink>   CRotateFileSinkSp;

/*******************************************************************************
 *
 *   CDailyFileSink
 *
 * *****************************************************************************/
struct CDailyTimeFileNameCalculator
{
   // Create filename for the form basename.YYYY-MM-DD_hh-mm
   static std::string CalcFileName(const std::string& basename)
   {
      return std::string(basename + LOG_FILE_NAME_DELIM + LCT_TIME->currentDateTime());
   }
};

/*
 * Generator of daily log file names in format basename.YYYY-MM-DD
 */
struct CDailyDateFileNameCalculator
{
   // Create filename for the form basename.YYYY-MM-DD
   static std::string CalcFileName(const std::string& basename)
   {
      return std::string(basename + LOG_FILE_NAME_DELIM + LCT_TIME->formatedDate());
   }
};

template<class FileNameCalc = CDailyDateFileNameCalculator>
class CDailyFileSink final: public CLctLogSink<CDailyFileSink<FileNameCalc> , CLctLogMessageSp>
{
public:
   //create daily file sink which rotates on given time
   CDailyFileSink(const bool forceFlush,
         const std::string& fileDir,
         const std::string& baseFileName,
         int rotationHour,
         int rotationMimute):
      m_forceFlush(forceFlush),
      m_rotationHour(rotationHour),
      m_rotationMinute(rotationMimute),
      m_fileDir(fileDir),
      m_baseFileName(baseFileName)
   {
      if (m_rotationHour < 0 || m_rotationHour > 23) {
         m_rotationHour = 0;
      }

      if (m_rotationMinute < 0 || m_rotationMinute > 59) {
         m_rotationMinute = 1;
      }

      m_rotationTp = firstRotationTp();
      m_fileHelper.open(calcFileName());
   }

   LCT_ERR_CODE log(const CLctLogMessageSp& message)
   {
      std::lock_guard<CRotateMutex> lk(m_mutex);
      return sinkIt(message);
   }

protected:
   LCT_ERR_CODE sinkIt(const CLctLogMessageSp& message)
   {
      LCT_ERR_CODE errCode = LCT_SUCCESS;
      if (CLctLogClock::now() >= m_rotationTp)
      {
         m_fileHelper.open(calcFileName());
         m_rotationTp = nextRotationTp();
      }
      m_fileHelper.write(message);

      if (m_forceFlush) {
         errCode = flush();
      }
      return errCode;
   }

   LCT_ERR_CODE flush()
   {
      return m_fileHelper.flush();
   }

private:
   CLctLogClock::time_point firstRotationTp()
    {
       const CLctLogClock::time_point now  = CLctLogClock::now();
       const time_t tnow = CLctLogClock::to_time_t(now);
       struct tm tstruct;
       #if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(WIN64)
         localtime_s(&tstruct, &tnow);
       #else
         tstruct = *localtime_r(&tnow, &tstruct);
       #endif

       tstruct.tm_hour   = m_rotationHour;
       tstruct.tm_min   = m_rotationMinute;
       tstruct.tm_sec   = 0;

      const auto& rotation_time = CLctLogClock::from_time_t(std::mktime(&tstruct));
      if (rotation_time > now) {
         return rotation_time;
      } else {
         return CLctLogClock::time_point(rotation_time + std::chrono::hours(24));
      }
    }

   CLctLogClock::time_point nextRotationTp()
   {
      return CLctLogClock::time_point(m_rotationTp + std::chrono::duration<int>(60*60*24));
   }

   std::string calcFileName() const
   {
      return m_fileDir + FileNameCalc::CalcFileName(m_baseFileName) + LOG_FILE_SUFFIX;
   }

private:
   bool                      m_forceFlush;
   int32_t                   m_rotationHour;
   int32_t                   m_rotationMinute;
   const std::string         m_fileDir;
   const std::string         m_baseFileName;
   CLctLogClock::time_point  m_rotationTp;
   CFileHelper               m_fileHelper;
   //typedef std::mutex        CRotateMutex;
   typedef CNullMutex         CRotateMutex;
   mutable CRotateMutex      m_mutex;
};

typedef std::shared_ptr<CDailyFileSink<CDailyDateFileNameCalculator> >   CDailyFileSinkSp;


/*******************************************************************************
 *
 *   CDailyRotateFileSink
 *
 * *****************************************************************************/
class CDailyRotateFileSink final: public CLctLogSink<CDailyRotateFileSink , CLctLogMessageSp>
{
public:
   CDailyRotateFileSink(const bool forceFlush,
         const std::string& fileDir,
         const std::string& baseFileName,
         const int64_t maxSize,
         const int64_t maxCount,
         int rotationHour,
         int rotationMimute);

   LCT_ERR_CODE log(const CLctLogMessageSp& message)
   {
      std::lock_guard<CRotateMutex> lk(m_mutex);
      return sinkIt(message);
   }

protected:
   LCT_ERR_CODE sinkIt(const CLctLogMessageSp& message)
   {
      LCT_ERR_CODE errCode = LCT_SUCCESS;
      if (CLctLogClock::now() >= m_rotationTp) {
         m_fileHelper.close();
         m_fileHelper.open(calcFileName(0));
         m_rotationTp = nextRotationTp();
         m_currentSize = 0;
      }

      m_currentSize += message->size();
      if (m_currentSize > m_maxSize) {
         errCode = sizeRotate();
         if (LCT_SUCCESS != errCode) {
            m_defaultStream << "Failed to size rotate log file " << ErrCodeFormat(errCode) << std::endl;
            return errCode;
         }
         m_currentSize = message->size();
      }

      errCode = m_fileHelper.write(message);
      if (LCT_SUCCESS != errCode) {
         m_defaultStream << "Failed to write log" << ErrCodeFormat(errCode) << std::endl;
      }

      if (m_forceFlush) {
         errCode = flush();
         if (LCT_SUCCESS != errCode) {
            m_defaultStream << "Failed to flush log" << ErrCodeFormat(errCode) << std::endl;
         }
      }
      return errCode;
   }

   LCT_ERR_CODE flush();

private:
   CLctLogClock::time_point firstRotationTp();

   CLctLogClock::time_point nextRotationTp();

   LCT_ERR_CODE sizeRotate();

   LCT_ERR_CODE rotateFile();

   std::string calcFileName(const int64_t index) const;

private:
   bool                      m_forceFlush          = true;
   int32_t                   m_rotationHour        = 24;
   int32_t                   m_rotationMinute      = 0;
   const int64_t             m_maxSize             = 10;
   const int64_t             m_maxFiles            = 0;
   int64_t                   m_currentSize         = 0;
   const std::string         m_fileDir;
   const std::string         m_baseFileName;
   CLctLogClock::time_point  m_rotationTp;
   CFileHelper               m_fileHelper;
   //typedef std::mutex    CRotateMutex;
   typedef CNullMutex        CRotateMutex;
   mutable CRotateMutex      m_mutex;
   std::ostream&             m_defaultStream = std::cout;
};

typedef std::shared_ptr<CDailyRotateFileSink>   CDailyRotateFileSinkSp;

/*******************************************************************************
 *
 *   CDailyRotateTerminalSink
 *
 * *****************************************************************************/
class CDailyRotateTerminalSink final: public CLctLogSink<CDailyRotateTerminalSink , CLctLogMessageSp>
{
public:
   CDailyRotateTerminalSink()
   {
   }

   LCT_ERR_CODE log(const CLctLogMessageSp& message)
   {
      return sinkIt(message);
   }

protected:
   LCT_ERR_CODE sinkIt(const CLctLogMessageSp& message)
   {
      m_defaultStream << message->data();
      return LCT_SUCCESS;
   }

private:
   std::ostream&            m_defaultStream = std::cout;
};

typedef std::shared_ptr<CDailyRotateTerminalSink>   CDailyRotateTerminalSinkSp;

/*******************************************************************************
 *
 *   CRemoteSink
 *
 * *****************************************************************************/

class CRemoteSink final : public CLctLogSink<CRemoteSink, CLctLogMessageSp>
{
public:
   CRemoteSink(const std::string& remoteIp, std::size_t remotePort);
   ~CRemoteSink();
   LCT_ERR_CODE log(const CLctLogMessageSp& message);
};


typedef std::shared_ptr<CRemoteSink>   CRemoteSinkSp;


#endif /* SRC_LOG_LCT_LOG_SINK_H_ */
