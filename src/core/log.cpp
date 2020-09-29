/**
 * @file log.cpp
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

#define MODULE_NAME "log"

#include <stdio.h>
#include <map>
#include <memory>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils/macros.h"
#include "log_init.h"
#include "log.h"

namespace wotsen
{

#if defined(LANGUAGE_ZH)
	#define TIME_FORMAT "%04d年 %02d月 %02d日 %s %02d:%02d:%02d.%03ld"
	static const char *const _wday[] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
#else
	#define TIME_FORMAT "%04d-%02d-%02d %s %02d:%02d:%02d.%03ld"
	static const char *const _wday[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
#endif // endof : #if defined(LANGUAGE_ZH)

/**
 * @brief 日志输出接口
 * 
 * @param lv 等级
 * @param module_name 模块名
 * @param fmt 格式
 * @param ... 
 */
static void log_output(const mb_log_level_t& lv, const std::string& module_name, const char* fmt, ...);

static const std::map<std::string, mb_log_level_t> cfg_lv_map = {
	{"all", e_mb_log_all},
	{"debug", e_mb_log_debug},
	{"info", e_mb_log_info},
	{"warning", e_mb_log_warning},
	{"error", e_mb_log_error},
	{"none", e_mb_log_non},
};

/**
 * @brief 日志
 * 
 */
class MbLog
{
	public:
		MbLog() : fd_(-1), enable_(true), log_output_fun_(nullptr) {}
		~MbLog() {
			if (fd_ >= 0) {
				close(fd_);
			}
		}

	public:
		/**
		 * @brief 获取日志指针
		 * 
		 * @return std::shared_ptr<MbLog>& 
		 */
		static std::shared_ptr<MbLog> &mb_log_ptr(void)
		{
			static std::shared_ptr<MbLog> log = std::make_shared<MbLog>();
			return log;
		}

		/**
		 * @brief 日志过滤
		 * 
		 * @param lv 等级
		 * @param module_name 模块名
		 * @return true 过滤
		 * @return false 不过滤
		 */
		bool filter(const mb_log_level_t lv, const std::string& module_name)
		{
			if (fd_ < 0) {
				return false;
			}

			if (!enable_) {
				return true;
			}

			if (!log_file_.empty() && access(log_file_.c_str(), F_OK) < 0) {
				close(fd_);
				fd_ = -1;
				open_log_file();
			}

			auto key = log_lv_map_.find(module_name);
			if (key == log_lv_map_.end()) {
				// 没有配置日志的模块使用默认等级
				if (lv > log_lv_map_[DEFAULT_MODULE_NAME]) {
					return true;
				}
				return false;
			}

			if (lv > key->second) {
				return true;
			}

			return false;
		}

		/**
		 * @brief 日志初始化
		 * 
		 * @param conf 配置
		 * @return true 成功
		 * @return false 失败
		 */
		bool init(const app_configure_param_t& conf)
		{
			if (conf.type != kAppConfTypeJson || !conf.conf) {
				mb_log_e("log config error.\n");
				return false;
			}

			app_json_t *log_cfg = (app_json_t*)conf.conf;

			mb_log_v("\n%s\n", log_cfg->dump(4).c_str());

			app_json_t &_j = *log_cfg;

			// 全局日志使能
			enable_ = _j["app"]["log"]["enable"].get<bool>();
			///< 无模块的默认等级
			parser_log_lv(DEFAULT_MODULE_NAME, _j["app"], e_mb_log_error);

			// 模块日志等级映射
			for (auto &el : _j["module"].items()) {
				// 一级模块的日志等级
				parser_log_lv(el.key(), el.value(), log_lv_map_[DEFAULT_MODULE_NAME]);

				// 二级模块日志
				if (!el.value().contains("children")) {
					continue;
				}

				// 遍历数组
				for (auto &cel : el.value()["children"].items()) {
					// 遍历数组成员对象
					for (auto &icel : cel.value().items()) {
						parser_log_lv(icel.key(), icel.value(), log_lv_map_[DEFAULT_MODULE_NAME]);
					}
				}
			}

			///< 日志文件
			log_file_ = _j["app"]["log"]["path"].get<std::string>();
			open_log_file();

			return true;
		}
	
	private:
		void open_log_file(void)
		{
			int fd = open(log_file_.c_str(), O_CREAT|O_APPEND|O_WRONLY, 0666);

			if (fd < 0) {
				mb_log_e("open log file : %s error.\n", log_file_.c_str());
			} else {
				fd_ = fd;
			}
		}

		/**
		 * @brief 接卸日志等级
		 * 
		 * @param key 模块名
		 * @param value 配置内容
		 * @param def 默认等级
		 */
		void parser_log_lv(const std::string &key, app_json_t &value, const mb_log_level_t &def)
		{
			if (!value.contains("log lv")) {
				mb_log_e("module : [%s] no [log lv] key.\n", key.c_str());
				log_lv_map_[key] = def;
				return;
			}

			auto lv = cfg_lv_map.find(value["log lv"]);
			if (lv == cfg_lv_map.end()) {
				mb_log_e("module : [%s] no [log lv] key.\n", key.c_str());
				log_lv_map_[key] = def;
			} else {
				log_lv_map_[key] = lv->second;
			}
		}
	
	public:
		int fd_;						///< 文件描述符
		bool enable_;					///< 使能
		mb_log_fn_t log_output_fun_;	///< 输出接口 
		std::string log_file_;			///< 日志文件名
		std::map<std::string, mb_log_level_t> log_lv_map_;	///< 模块映射
};
	
static void log_output(const mb_log_level_t& lv, const std::string& module_name, const char* fmt, ...)
{
	auto log = MbLog::mb_log_ptr();
	static const char *colors[] = {
		COLORE_WHITE,
		COLORE_GREEN,
		COLORE_CYAN,
		COLORE_YELLOW,
		COLORE_RED
	};

	if (log->filter(lv, module_name)) {
		return ;
	}

	int fd = log->fd_;

	if (fd < 0) {
		fd = STDOUT_FILENO;
	}

	struct timeval tmval;
	struct tm *tm = nullptr;

	gettimeofday(&tmval, nullptr);
	tm = localtime(&tmval.tv_sec);

	// 色彩值不输入到文件
	if (lv < ARRAY_SIZE(colors) && fd <= STDERR_FILENO) {
		dprintf(fd, "%s", colors[lv]);
	}

	// 日期输出
	dprintf(fd, TIME_FORMAT,
			tm->tm_year + 1900,
			tm->tm_mon + 1,
			tm->tm_mday,
			_wday[tm->tm_wday],
			tm->tm_hour,
			tm->tm_min,
			tm->tm_sec,
			tmval.tv_usec / 1000);

	va_list ap;
	va_start(ap, fmt);

	// 用户打印输出
	vdprintf(fd, fmt, ap);

	va_end(ap);

	if (lv < ARRAY_SIZE(colors) && fd <= STDERR_FILENO) {
		dprintf(fd, COLORE_RESET);
	}
}

mb_log_fn_t get_log_output_fun(void)
{
	return log_output;
}

/**
 * @brief 日志配置
 * 
 * @param conf 配置参数
 * @return true 成功
 * @return false 失败
 */
bool log_init(const app_configure_param_t& conf)
{
	auto log = MbLog::mb_log_ptr();

	return log->init(conf);
}

} // namespace wotsen
