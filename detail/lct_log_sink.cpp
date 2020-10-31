/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_sink.cpp
 * @version     1.0
 * @date        Jun 8, 2017 7:31:49 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/

#include <mutex>

#include "lct_log_sink.h"


static int32_t DigitCount(int32_t figure)
{
   if (figure < 0) {
      return -1;
   }
   int32_t count = 0;
   while (figure > 0) {
      ++count;
      figure /= 10;
   }
   return count;
 }

static std::string IndexOnDigitCount(const int32_t digitCount, int32_t index)
{
   std::string indexVal = std::to_string(index);

   int32_t indexDigitCount = DigitCount(index);

   while (indexDigitCount < digitCount) {
      indexVal = "0" + indexVal;
      indexDigitCount++;
   }
   return indexVal;
}

/*******************************************************************************
 *
 *   CRotateFileSink
 *
 * *****************************************************************************/

CRotateFileSink::CRotateFileSink(
      const bool forceFlush,
      const std::string& fileDir,
      const std::string& baseFileName,
      const int64_t maxSize,
      const int64_t maxCount) :
   m_forceFlush(forceFlush),
   m_maxSize(maxSize),
   m_maxFiles(maxCount),
   m_currentSize(0),
   m_fileDir(fileDir),
   m_baseFileName(baseFileName),
   m_fileHelper()
{
   m_fileHelper.open(calcFileName(0));
   m_currentSize = m_fileHelper.size(); //expensive. called only once
}

CRotateFileSink::~CRotateFileSink()
{
   flush();
}

LCT_ERR_CODE CRotateFileSink::flush()
{
   return m_fileHelper.flush();
}

// Rotate files:
// log.txt -> log.txt.1
// log.txt.1 -> log.txt.2
// log.txt.2 -> log.txt.3
// lo3.txt.3 -> delete
LCT_ERR_CODE CRotateFileSink::rotate()
{
   m_fileHelper.close();
   for (auto i = m_maxFiles; i > 0; --i) {
      const std::string src      = calcFileName(i - 1);
      const std::string target   = calcFileName(i);

      if (FileExists(target)) {
         if (std::remove(target.c_str()) != 0) {
            return LCT_FAIL;
         }
      }
      if (FileExists(src)) {
         if (std::rename(src.c_str(), target.c_str()) != 0) {
            return LCT_FAIL;
         }
      }
   }
   return m_fileHelper.reopen(false);
}

std::string CRotateFileSink::calcFileName(const int64_t index) const
{
   if (index > 0) {
      return m_fileDir + m_baseFileName + LOG_FILE_NAME_DELIM + IndexOnDigitCount(DigitCount(m_maxFiles - 1), index) + LOG_FILE_SUFFIX;
   } else {
      return m_fileDir + m_baseFileName + LOG_FILE_SUFFIX;
   }
}


/*******************************************************************************
 *
 *   CDailyRotateFileSink
 *
 * *****************************************************************************/
CDailyRotateFileSink::CDailyRotateFileSink(const bool forceFlush,
      const std::string& fileDir,
      const std::string& baseFileName,
      const int64_t maxSize,
      const int64_t maxFiles,
      int rotationHour,
      int rotationMimute):
   m_forceFlush(forceFlush),
   m_rotationHour(rotationHour),
   m_rotationMinute(rotationMimute),
   m_maxSize(maxSize),
   m_maxFiles(maxFiles),
   m_fileDir(fileDir),
   m_baseFileName(baseFileName)
{
   if (m_rotationHour < 0 || m_rotationHour > 23) {
      m_rotationHour = 0;
   }

   if (m_rotationMinute < 0 || m_rotationMinute > 59) {
      m_rotationMinute = 1;
   }

   m_defaultStream << "CDailyRotateFileSink:LogDir(" << m_fileDir << ") fileSize(" << m_maxSize << ") fileCount(" << m_maxFiles << ")" << std::endl;

   const std::string logFilePath = calcFileName(0);
   if (FileExists(logFilePath)) {
      rotateFile();
   }

   m_rotationTp = firstRotationTp();
   m_fileHelper.open(logFilePath);
}

LCT_ERR_CODE CDailyRotateFileSink::flush()
{
   return m_fileHelper.flush();
}

CLctLogClock::time_point CDailyRotateFileSink::firstRotationTp()
{
   const CLctLogClock::time_point now  = CLctLogClock::now();
   const time_t tnow = CLctLogClock::to_time_t(now);
   struct tm tstruct;
#ifndef WIN32
   tstruct = *localtime_r(&tnow, &tstruct);
#else
   localtime_s(&tstruct, &tnow);
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

CLctLogClock::time_point CDailyRotateFileSink::nextRotationTp()
{
   return CLctLogClock::time_point(m_rotationTp + std::chrono::duration<int32_t>(60*60*24));
}


LCT_ERR_CODE CDailyRotateFileSink::sizeRotate()
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;
   m_fileHelper.close();

   errCode = rotateFile();
   if (LCT_SUCCESS != errCode) {
      m_defaultStream << "Failed to rotateFile" << std::endl;
      return errCode;
   }

   return m_fileHelper.reopen(false);
}

LCT_ERR_CODE CDailyRotateFileSink::rotateFile()
{
   for (auto i = m_maxFiles; i > 0; --i) {
      const std::string src      = calcFileName(i - 1);
      const std::string target   = calcFileName(i);

      if (FileExists(target)) {
         if (std::remove(target.c_str()) != 0) {
            m_defaultStream << "Failed to remove file(" << target << ")" << std::endl;
            return LCT_FAIL;
         }
      }
      if (FileExists(src)) {
         if (std::rename(src.c_str(), target.c_str()) != 0) {
            m_defaultStream << "Failed to rename file(" << src << ")" << std::endl;
            return LCT_FAIL;
         }
      }
   }
   return LCT_SUCCESS;
}

std::string CDailyRotateFileSink::calcFileName(const int64_t index) const
{
   if (index > 0) {
      return m_fileDir + CDailyDateFileNameCalculator::CalcFileName(m_baseFileName) + LOG_FILE_NAME_DELIM + IndexOnDigitCount(DigitCount(m_maxFiles - 1), index) + LOG_FILE_SUFFIX;
   } else {
      return m_fileDir + CDailyDateFileNameCalculator::CalcFileName(m_baseFileName) + LOG_FILE_SUFFIX;
   }
}


/*******************************************************************************
 *
 *   CRemoteSink
 *
 * *****************************************************************************/

CRemoteSink::CRemoteSink(const std::string& remoteIp, std::size_t remotePort)
{
}

CRemoteSink::~CRemoteSink()
{
}

LCT_ERR_CODE CRemoteSink::log(const CLctLogMessageSp& message)
{
   return LCT_SUCCESS;
}
