/**
 * @file configure.cpp
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

#include <fstream>
#include "log.h"
#include "configure.h"

namespace wotsen
{

bool parser_app_config(const std::string& filename, app_configure_param_t& config)
{
	if (filename.empty() || config.type != kAppConfTypeJson) {
		return false;
	}

	std::ifstream file(filename);

	if (!file) {
		mb_log_e("open config file : %s failed.\n", filename.c_str());
		return false;
	}

	app_json_t *j = new app_json_t;

	try
	{
		file >> *j;
		file.close();
	}
	catch(const app_json_t::parse_error& e)
	{
		delete j;
		mb_log_e("parser config file : %s failed (%s).\n", filename.c_str(), e.what());
		file.close();
		return false;
	}

	app_json_t &_j = *j;

	if (!_j.contains("module")) {
		goto conf_err_exit;
	}

	// for (auto &el : _j["module"].items()) {
	// }

	config.conf = j;
	return true;

conf_err_exit:
	delete j;
	return false;
}

} // !namespace wotsen
