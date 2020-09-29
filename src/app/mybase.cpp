/**
 * @file mybase.cpp
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

#define MODULE_NAME "sys-mybase"

#include "app_curing_param.h"
#include "core/configure.h"
#include "core/log_init.h"
#include "core/log.h"
#include "mybase.h"

namespace wotsen
{

#define APP_INIT(expr) \
if (!expr) { return false; }

///< 应用配置
static app_configure_param_t mybase_config = {kAppConfTypeJson, nullptr};
	
bool mybase_init(void)
{
	// 配置解析
	APP_INIT(parser_app_config(MB_APP_CONFIG_FILE, mybase_config));
	// 日志初始化
	APP_INIT(log_init(mybase_config));
}

} // namespace wotsen

