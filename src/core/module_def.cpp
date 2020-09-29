/**
 * @file module_def.cpp
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

#include "module_def.h"

namespace wotsen
{
/**
 * @brief Construct a new  Module Configure object
 * @details 构造模块配置
 * 
 * @param info 模块信息，传引用，需要持久内存
 * @param init_fn 初始化接口
 * @param run_fn 启动接口
 * @param chg_fn 变更接口
 * @param stop_fn 停止接口
 * @param exit_fn 退出接口
 * @param run_st_fn 运行时状态
 */
ModuleConfigure::ModuleConfigure(module_info_t *info,
					module_fun_t init_fn,
					module_fun_t run_fn,
					module_fun_t chg_fn,
					module_fun_t stop_fn,
					module_fun_t exit_fn,
					module_status_fun_t run_st_fn):
					info_(info),
					init_(init_fn),
					run_(run_fn),
					change_(chg_fn),
					stop_(stop_),
					exit_(exit_fn),
					run_status_(run_st_fn)
{
	if (!info_) {
		throw std::logic_error("module info null");
	} else if (!init_ || !run_ || !change_ || !stop_ || !exit_ || !run_status_) {
		throw std::logic_error("module function null");
	}

	// TODO:更多校验
}

ModuleConfigure::~ModuleConfigure()
{
	if (info_->status == e_module_runing)
	{
		stop_(info_, nullptr);
		info_->status = e_module_inited;
	}

	if (info_->status == e_module_inited)
	{
		exit_(info_, nullptr);
	}
}

// 初始化
module_err_t ModuleConfigure::init(void *conf)
{
	module_err_t ret = init_(info_, conf);

	if (e_module_ok == ret)
	{
		info_->status = e_module_inited;
	}
	else
	{
		info_->status = e_module_uninit;
	}
	

	return ret;
}

// 启动
module_err_t ModuleConfigure::run(void* res)
{
	module_err_t ret = run_(info_, res);

	if (e_module_ok == ret)
	{
		info_->status = e_module_runing;
	}
	else
	{
		info_->status = e_module_unruning;
	}

	return ret;
}

// 修改参数
module_err_t ModuleConfigure::change(void *param)
{
	// 设置配置参数
	info_->param = param;

	module_err_t ret = change_(info_, param);

	// 重置该变量
	info_->param = nullptr;

	return ret;
}

// 停止
module_err_t ModuleConfigure::stop(void* res)
{
	module_err_t ret = stop_(info_, res);

	info_->status = e_module_unruning;

	return ret;
}

// 退出
module_err_t ModuleConfigure::exit(void* res)
{
	module_err_t ret = exit_(info_, res);

	info_->status = e_module_uninit;

	return ret;
}

// 运行时状态查询
module_run_status_t ModuleConfigure::run_status(void)
{
	return run_status_(info_);
}

// 查询模块信息
const module_base_info_t& ModuleConfigure::module_info(void) const
{
	return *((module_base_info_t*)info_);
}

// 模块匹配
bool ModuleConfigure::match(uint64_t identifier) const
{
	return identifier == info_->identifer;
}

void ModuleConfigure::set_enable(bool en)
{
	info_->enable = en;
}

} // namespace wotsen

