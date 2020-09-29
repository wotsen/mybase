/**
 * @file module_def.h
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

#ifndef __mybase_MODULE_DEF_H__
#define __mybase_MODULE_DEF_H__

#include <exception>
#include <mutex>
#include <inttypes.h>
#include <limits.h>
#include "log.h"

namespace wotsen
{

typedef uint64_t module_idenf_t;
///< 分组
typedef uint64_t module_group_t;
///< 模块信息
typedef struct module_info_s module_info_t;
///< 模块基本信息
typedef struct module_base_info_s module_base_info_t;
///< 模块配置
typedef struct module_configure_s module_configure_t;

///< 非法模块标识
#define MODULE_INVALID_IDENTIFIER 0

/**
 * @brief 模块错误码
 * 
 */
typedef enum module_err_e : uint64_t
{
	e_module_ok = 0,
	e_module_err_notfind = 1,
	e_module_err_op_notsupport = 2,
	e_module_unknown_err = UINT64_MAX
} module_err_t;

/**
 * @brief 模块状态码
 * 
 */
typedef enum module_status_e {
	e_module_uninit,
	e_module_inited,
	e_module_unruning,
	e_module_runing,
	e_module_bad,
} module_status_t;

/**
 * @brief 模块运行状态码
 * 
 */
typedef enum module_run_status_e {
	e_module_run_st_ok,
	e_module_run_st_err,
	e_module_run_st_unknown,
} module_run_status_t;

/**
 * @brief 模块权限
 * 
 */
typedef enum module_permission_e {
	e_module_permission_cfg_en,
	e_module_permission_cfg_disen,
} module_permission_t;

/**
 * @brief 模块类型
 * 
 */
typedef enum module_type_e {
	e_module_core_type = 0,		///< 系统核心模块
	e_module_other_type,		///< 其他模块
	e_module_pri_ext_type,		///< 私有扩展
	e_module_pub_ext_type,		///< 开放扩展
	e_module_pri_type,			///< 私有
	e_module_pub_type,			///< 开放
} module_type_t;

/**
 * @brief 基本信息基类
 * 
 */
#define MODULE_BASE_INFO \
char name[128];			 	/* 模块名称 */	\
bool enable;		 		/* 使能 */ \
module_idenf_t identifer;	 		/* 唯一标识 */ \
module_type_t type;		 	/* 类别 */ \
module_group_t group;		/* 分组 */ \
module_status_t status;	 	/* 状态 */ \
mb_log_level_t dbg_lv;	/* 日志等级 */	\
module_permission_t permission; /* 权限 */

/**
 * @brief 模块基本信息
 * 
 */
struct module_base_info_s
{
	MODULE_BASE_INFO
};

/**
 * @brief 模块配置，使用的是json配置，每个模块有专用的节点
 * 
 */
struct module_configure_s
{
	char node[128];
	void *configure;		///< 配置
};

/**
 * @brief 模块信息
 * 
 */
struct module_info_s {
	MODULE_BASE_INFO
	module_configure_t conf;	///< 配置
	void *handle;				///< 句柄
	void *param;				///< 配置，临时变量，每次调用配置修改时会置位
	void *data;					///< 私有数据
};

///< 模块处理接口
typedef module_err_t (*module_fun_t)(module_info_t*, void *);
typedef module_run_status_t (*module_status_fun_t)(module_info_t*);

using module_t = class ModuleConfigure;

/**
 * @brief 模块配置
 * 
 */
class ModuleConfigure {
public:
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
	ModuleConfigure(module_info_t *info,
					   module_fun_t init_fn,
					   module_fun_t run_fn,
					   module_fun_t chg_fn,
					   module_fun_t stop_fn,
					   module_fun_t exit_fn,
					   module_status_fun_t run_st_fn);
	~ModuleConfigure();

public:
	// 初始化
	module_err_t init(void *confg);
	// 启动
	module_err_t run(void *res);
	// 修改参数
	module_err_t change(void *param);
	// 停止
	module_err_t stop(void *res);
	// 退出
	module_err_t exit(void *res);
	// 运行时状态查询
	module_run_status_t run_status(void); 
	// 查询模块信息
	const module_base_info_t& module_info(void) const;
	// 模块匹配
	bool match(module_idenf_t identifier) const;
	void set_enable(bool en);

private:
	std::mutex mtx_;					///< 操作锁
	// TODO:改为只能指针?
	module_info_t *info_;			///< 模块信息
	module_fun_t init_;				///< 模块初始化
	module_fun_t run_;				///< 模块启动
	module_fun_t change_;			///< 模块配置变更
	module_fun_t stop_;				///< 模块停止
	module_fun_t exit_;				///< 模块卸载
	module_status_fun_t run_status_;	///< 模块运行时状态
};

/**
 * @brief 新建模块
 * 
 */
#define MODULE(info, init, run, chg, stop, exit, run_st) \
std::make_shared<module_t>(info, init, run, chg, stop, exit, run_st)

} // !wotsen

#endif // !__mybase_MODULE_DEF_H__
