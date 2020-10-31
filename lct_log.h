/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log.h
 * @version     1.0
 * @date        Jun 8, 2017 6:38:36 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_H_
#define SRC_LOG_LCT_LOG_H_

#ifndef __DISABLE_LCT_LOG__
#ifdef ERROR
    #undef ERROR
#endif

#include "detail/lct_logger.h"

#define LCT_LOG LCT_LOG_MGR

#define LOG_TRACE LCT_LOG_MGR->isLogged(ELogLevel::TRACE) && CLCT_LOG(ELogLevel::TRACE)
#define LOG_DEBUG LCT_LOG_MGR->isLogged(ELogLevel::DEBUG) && CLCT_LOG(ELogLevel::DEBUG)
#define LOG_INFOR LCT_LOG_MGR->isLogged(ELogLevel::INFOR) && CLCT_LOG(ELogLevel::INFOR)
#define LOG_WARNG LCT_LOG_MGR->isLogged(ELogLevel::WARNG) && CLCT_LOG(ELogLevel::WARNG)
#define LOG_ERROR LCT_LOG_MGR->isLogged(ELogLevel::ERROR) && CLCT_LOG(ELogLevel::ERROR)
#define LOG_CRITC LCT_LOG_MGR->isLogged(ELogLevel::CRITC) && CLCT_LOG(ELogLevel::CRITC)

#define LOG_TRACE_MARK() LOG_TRACE << ""
#define LOG_DEBUG_MARK() LOG_DEBUG << ""
#define LOG_INFOR_MARK() LOG_INFOR << ""
#define LOG_WARNG_MARK() LOG_WARNG << ""
#define LOG_ERROR_MARK() LOG_ERROR << ""
#define LOG_CRITC_MARK() LOG_CRITC << ""

#else

#include "detail/lct_dummy_log.h"

#define LCT_LOG LCT_DUMMY_LOGGER

#define LOG_TRACE CLctDummyLog()
#define LOG_DEBUG CLctDummyLog()
#define LOG_INFOR CLctDummyLog()
#define LOG_WARNG CLctDummyLog()
#define LOG_ERROR CLctDummyLog()
#define LOG_CRITC CLctDummyLog()

#define LOG_TRACE_MARK() LOG_TRACE << ""
#define LOG_DEBUG_MARK() LOG_DEBUG << ""
#define LOG_INFOR_MARK() LOG_INFOR << ""
#define LOG_WARNG_MARK() LOG_WARNG << ""
#define LOG_ERROR_MARK() LOG_ERROR << ""
#define LOG_CRITC_MARK() LOG_CRITC << ""

#endif

#endif /* SRC_LOG_LCT_LOG_H_ */
