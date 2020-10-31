/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_dummy_log.h
 * @version     1.0
 * @date        Aug 29, 2017 2:32:45 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_DUMMY_LOG_H_
#define SRC_LOG_LCT_DUMMY_LOG_H_

#include "lct_common_define.h"
#include "lct_singleton.h"

class CLctDummyLog final
{
public:
   explicit CLctDummyLog()
   {
   }

   ~CLctDummyLog()
   {
   }

   template <typename CArgs>
   CLctDummyLog& operator << (const CArgs& args) {
      return *this;
   }

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctDummyLog);
};


class CLctDummyLogger final: public CLctSingleton<CLctDummyLogger>
{
public:
   LCT_ERR_CODE init(const std::string& logLocalDir, const int64_t rotateMaxFileSize = 0,
      const int64_t rotateMaxFileCount = 0)
   {
      return LCT_SUCCESS;
   }

   LCT_ERR_CODE init(const std::string& logLocalDir, const std::string& configFile, const int64_t rotateMaxFileSize = 0,
         const int64_t rotateMaxFileCount = 0)
   {
      return LCT_SUCCESS;
   }

   LCT_ERR_CODE close()
   {
      return LCT_SUCCESS;
   }

   LCT_ERR_CODE log(CLctDummyLog&)
   {
      return LCT_SUCCESS;
   }

private:
   CLctDummyLogger()
   {
   }

   ~CLctDummyLogger()
   {
   }

   friend class CLctSingleton;
};

#define LCT_DUMMY_LOGGER CLctDummyLogger::instance()


#endif /* SRC_LOG_LCT_DUMMY_LOG_H_ */
