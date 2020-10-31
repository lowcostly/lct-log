/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_formatter_pool.h
 * @version     1.0
 * @date        Jun 9, 2017 2:17:02 PM
 * @author      wayfarerly@yahoo.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_FORMATTER_POOL_H_
#define SRC_LOG_LCT_LOG_FORMATTER_POOL_H_

#include <memory>
#include <iomanip>
#include "lct_common_define.h"
#include "lct_cplusplus_14.h"
#include "lct_log_stream.h"

template <typename... CFormatterType>
class CLctLogFormatterPool final
{
public:
   explicit CLctLogFormatterPool()
   {
   }

   ~CLctLogFormatterPool()
   {
   }

   LCT_ERR_CODE format(CLctLogStream& stream) const
   {
      stream << std::dec;
      return Format(stream, m_formatters);
   }

   template <typename CFormatter>
   LCT_ERR_CODE appendFormatter(const CFormatter& formatter)
   {
      auto& formatterRef = cplusplus14::get<CFormatter>(m_formatters);
      formatterRef = formatter;
      return LCT_SUCCESS;
   }

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctLogFormatterPool);

private:
   template<size_t I = 0, typename... CFormattersType>
   static typename std::enable_if<I == sizeof...(CFormattersType), LCT_ERR_CODE>::type Format(CLctLogStream& stream, const std::tuple<CFormattersType...>& formatterTuple)
   {
      return LCT_SUCCESS;
   }

   template<size_t I = 0, typename... CFormattersType>
   static typename std::enable_if<I < sizeof...(CFormattersType), LCT_ERR_CODE>::type Format(CLctLogStream& stream, const std::tuple<CFormattersType...>& formatterTuple)
   {
      auto& formatter = std::get<I>(formatterTuple);

      LCT_ERR_CODE errCode = formatter.format(stream);
      if (LCT_SUCCESS != errCode) {
         return errCode;
      }
      return Format<I+1>(stream, formatterTuple);
   }

private:
   typedef typename std::tuple<CFormatterType...>   CLctFormatterTuple;
   CLctFormatterTuple                        m_formatters;;
};


#endif /* SRC_LOG_LCT_LOG_FORMATTER_POOL_H_ */
