/**
 * @file module.cpp
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

#include "module.h"

namespace wotsen
{

Module::Module(const std::initializer_list<std::shared_ptr<module_t>> &li) :
				modules_(li)
{
	// TODO:模块冲突检测
}

Module::~Module()
{
}

// 动态注册模块
module_err_t Module::register_module(const std::shared_ptr<module_t> &module)
{
	// TODO:
	return e_module_ok;
}

// 动态卸载模块
void Module::unregister_module(module_idenf_t identifier)
{
	// TODO:
}

// 初始化模块，可指定
module_err_t Module::init_module(module_idenf_t identifier, void *conf)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			auto bf = item->module_info();
			if (bf.status == e_module_uninit) {
				lck.unlock();
				return item->init(conf);
			}
			return e_module_err_op_notsupport;
		}
	}

	return e_module_err_notfind;
}

// 运行模块，可指定
module_err_t Module::run_module(module_idenf_t identifier, void *res)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			auto bf = item->module_info();

			if (bf.status == e_module_inited) {
				return item->run(res);
			}
			return e_module_err_op_notsupport;
		}
	}

	return e_module_err_notfind;
}
// 停止模块，可指定
module_err_t Module::stop_module(module_idenf_t identifier, void *res)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			auto bf = item->module_info();

			if (bf.status == e_module_runing) {
				return item->stop(res);
			}
			return e_module_err_op_notsupport;
		}
	}

	return e_module_err_notfind;
}
// 反初始化模块，可指定
module_err_t Module::finit_module(module_idenf_t identifier, void *res)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			auto bf = item->module_info();

			if (bf.status == e_module_unruning) {
				return item->exit(res);
			}
			return e_module_err_op_notsupport;
		}
	}

	return e_module_err_notfind;
}
// 修改模块参数
module_err_t Module::change_module(module_idenf_t identifier, void *param)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			lck.unlock();
			return item->change(param);
		}
	}

	return e_module_err_notfind;
}
// 修改使能
module_err_t Module::enable_module(module_idenf_t identifier, bool en, void *res)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			auto bf = item->module_info();

			if (bf.enable == en) {
				return e_module_ok;
			}

			if (en = false) {
				if (bf.status != e_module_uninit) {
					item->stop(res);
				}
			}

			item->set_enable(en);
			return e_module_ok;
		}
	}

	return e_module_err_notfind;
}
// 查询模块信息
bool Module::query_module(module_idenf_t identifier, module_base_info_t &info)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		if (item->match(identifier)) {
			lck.unlock();
			info = item->module_info();
			return true;
		}
	}

	return false;
}
// 查询所有模块信息
bool Module::query_module(std::vector<module_base_info_t> &info)
{
	std::unique_lock<std::mutex> lck(mtx_);

	for (auto &item : modules_) {
		// info.push_back(item->module_info());
		info.emplace_back(item->module_info());
	}

	return true;
}

/// 所有模块信息
std::vector<module_idenf_t> Module::modules(void)
{
	std::unique_lock<std::mutex> lck(mtx_);
	std::vector<module_idenf_t> mds;

	for (auto &item : modules_) {
		auto bf = item->module_info();
		mds.push_back(bf.identifer);
	}

	return std::move(mds);
}
	
} // namespace wotsen
