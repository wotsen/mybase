/**
 * @file configure_def.h
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

#ifndef __mybase_CONFIGURE_DEF_H__
#define __mybase_CONFIGURE_DEF_H__

#include <inttypes.h>
#include "nlohmann/json.hpp"

namespace wotsen
{

typedef struct app_configure_param_s app_configure_param_t;
using app_json_t = nlohmann::json;

/**
 * @brief 配置参数类型
 * 
 */
typedef enum app_configure_param_type_e : uint32_t
{
	kAppConfTypeJson = 0,
} app_configure_param_type_t;

/**
 * @brief 应用配置参数
 * 
 */
struct app_configure_param_s
{
	app_configure_param_type_t type;	///< 类型
	void *conf;							///< 配置内容
};
	
} // namespace wotsen

#endif // !__mybase_CONFIGURE_DEF_H__
