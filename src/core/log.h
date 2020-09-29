/**
 * @file log.h
 * @author wotsen (astralrovers@outlook.com)
 * @brief 
 * @version 0.0.0
 * @date 2020-09-12
 * 
 * @copyright 
 * MIT License
 * 
 * Copyright (c) 2020 wotsen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

 * 
 */

#ifndef __mybase_LOG_H__
#define __mybase_LOG_H__

#include <stdio.h>

namespace wotsen
{

#define DEFAULT_MODULE_NAME "NON"

#ifndef MODULE_NAME
	#define MODULE_NAME DEFAULT_MODULE_NAME
#endif

///< 打印日志
#define mb_log(lv, module_name, fmt, args...) get_log_output_fun()(lv, module_name, fmt, ##args)

///< 标准输出
#define mb_stdout printf

///< 全打印
#define mb_log_v(fmt, args...) mb_log(e_mb_log_all, MODULE_NAME, \
							   " [ALL][" MODULE_NAME "][%s %d %s] " fmt, \
							   __FILE__, __LINE__, __func__, ##args)

///< debug等级
#define mb_log_d(fmt, args...) mb_log(e_mb_log_debug, MODULE_NAME, \
							   " [DEBUG][" MODULE_NAME "][%s %d %s] " fmt, \
							   __FILE__, __LINE__, __func__, ##args)

///< 提示信息
#define mb_log_i(fmt, args...) mb_log(e_mb_log_info, MODULE_NAME, \
							   " [INFO][" MODULE_NAME "][%s %d %s] " fmt, \
							   __FILE__, __LINE__, __func__, ##args)

///< 警告信息
#define mb_log_w(fmt, args...) mb_log(e_mb_log_warning, MODULE_NAME, \
							   " [WARNING][" MODULE_NAME "][%s %d %s] " fmt, \
							   __FILE__, __LINE__, __func__, ##args)

///< 错误打印
#define mb_log_e(fmt, args...) mb_log(e_mb_log_error, MODULE_NAME, \
							   " [ERROR][" MODULE_NAME "][%s %d %s] " fmt, \
							   __FILE__, __LINE__, __func__, ##args)

/**
 * @brief 日志打印等级
 * 
 */
typedef enum mb_log_level_e {
	e_mb_log_all = 0,
	e_mb_log_debug,
	e_mb_log_info,
	e_mb_log_warning,
	e_mb_log_error,
	e_mb_log_non
} mb_log_level_t;

///< 日志打印接口
typedef void (*mb_log_fn_t)(const mb_log_level_t& lv, const std::string& module_name, const char* fmt, ...);

/**
 * @brief 获取日志输出接口
 * 
 * @return mb_dbg_fn_t 
 */
mb_log_fn_t get_log_output_fun(void);

} // !namespace wotsen

#endif // !__mybase_LOG_H__
