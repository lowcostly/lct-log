/**********************************************************************
 * @copyright   Copyright (C), 2020
 * @version     1.0
 * @date        2020-04-04 11:11:48
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOGGER_INL_
#define SRC_LOG_LCT_LOGGER_INL_

inline bool CLctLog::operator ==(CLctLogStream& stream) const
{
   return (LCT_SUCCESS == LCT_LOG_MGR->log(stream));
}

inline bool CLctLogger::isLogged(const ELogLevel level) const
{
   return ((m_loglevels.find(level) != m_loglevels.end()) || !m_isInitiated);
}

#endif /* SRC_LOG_LCT_LOGGER_INL_ */
