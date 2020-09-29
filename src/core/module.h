/**
 * @file module.h
 * @author wotsen (astralrovers@outlook.com)
 * @brief 
 * @version 0.0.0
 * @date 2020-09-08
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

#ifndef __mybase_MODULE_H__
#define __mybase_MODULE_H__

#include <vector>
#include <memory>
#include "module_def.h"

namespace wotsen
{

/**
 * @brief 模块管理
 * 
 */
class Module
{
public:
	Module(const std::initializer_list<std::shared_ptr<module_t>> &li);
	~Module();

public:
	// 动态注册模块
	module_err_t register_module(const std::shared_ptr<module_t> &module);
	// 动态卸载模块
	void unregister_module(module_idenf_t identifier);

public:
	// 初始化模块，可指定
	module_err_t init_module(module_idenf_t identifier, void *confres=nullptr);
	// 运行模块，可指定
	module_err_t run_module(module_idenf_t identifier, void *res=nullptr);
	// 停止模块，可指定
	module_err_t stop_module(module_idenf_t identifier, void *res=nullptr);
	// 反初始化模块，可指定
	module_err_t finit_module(module_idenf_t identifier, void *res=nullptr);
	// 修改模块参数
	module_err_t change_module(module_idenf_t identifier, void *paramres=nullptr);
	// 修改使能
	module_err_t enable_module(module_idenf_t identifier, bool en, void *res=nullptr);
	// 查询模块信息
	bool query_module(module_idenf_t identifier, module_base_info_t &info);
	// 查询所有模块信息
	bool query_module(std::vector<module_base_info_t> &info);

	/// 所有模块信息
	std::vector<module_idenf_t> modules(void);

public:
	std::mutex mtx_;
	// FIXME:使用map
	std::vector<std::shared_ptr<module_t>> modules_;	///< 模块列表，改为二叉搜索树?
};

} // !wotsen

#endif // !__mybase_MODULE_H__
