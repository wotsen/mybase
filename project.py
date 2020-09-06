#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# file project.py
# author wotsen
# email astralrovers@outlook.com
# date 2020/09/05 周六
# time 18:57:03
#
# description 
# copyright Copyright (c) 2020 yuwangliang

import fire
import inquirer
import re
import json
import os
from pprint import pprint
from inquirer import errors
from string import Template
from datetime import datetime
import time

__author__ = "wotsen"
__version__ = "1.0.0"

config_template = {
    "project": {
        "name": "",
        "version": "0.0.0",
        "type": "component",
        "language": "c++"
    },
    "build": {
        "obj name": "",
        "debug": False,
        "c standard": "gnu99",
        "c++ standard": "c++17",
        "c flag": [],
        "c++ flag": [],
        "macro": [],
        "include": [],
        "lib": []
    },
    "release": {
        "distribute path": "dist"
    },
    "file": {
        "author": "",
        "email": "your-email@xx.com",
        "copyright": ""
    },
    "third party": [
        {
            "name": "",
            "enable": True,
            "version": "",
            "url": ""
        }
    ]
}

project_lock = {
    "project tool version": "1.0.0",
}

version_log = {
    "version": [
        {
            "version": "",
            "time": ""
        }
    ]
}

MIT_LICENSE = Template("""
MIT License

Copyright (c) ${year} ${author}

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
""")
BSD_LICENSE = Template("""
BSD License

Copyright (c) ${year} ${author}

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
""")

LICENSES = {
    "MIT": MIT_LICENSE
}

def check_name(anwser, name):
    """检查名称，只能是数字、字母 、下划线、横线

    Args:
        anwser: 结果
        name: 名称
    """
    name_re = r'[a-zA-Z0-9-_]+'
    if not re.match(name_re, name):
        raise errors.ValidationError('', reason='使用数字、字母、下划线、连接线的组合')
    return True


def check_author(anwser, name):
    """检查用户名

    Args:
        anwser: 结果
        name: 用户名
    """
    if len(name.strip()) == 0:
        raise errors.ValidationError('', reason='请输入用户')
    return True
        

def check_email(anwser, email):
    """检查邮箱

    Args:
        anwser: 结果
        email: 邮箱
    """
    name_re = r'^[0-9a-zA-Z_]{0,19}@[0-9a-zA-Z]{1,13}\.[com,cn,net]{1,3}$'
    if not re.match(name_re, email):
        raise errors.ValidationError('', reason='请输入正确的邮箱地址')
    return True


def check_version(anwser, version):
    """检查邮箱

    Args:
        anwser: 结果
        version: 版本号
    """
    if not re.match(r'(0|[1-9][0-9]*.?)\.(0|[1-9][0-9]*.?)\.(0|[1-9][0-9]*.?)$', version):
        raise errors.ValidationError('', reason='版本号格式错误，以0开头的版本号只能有一个数字')

    return True
    # old_ver = old_version.split(".")
    # old_ver = [int(item) for item in old_ver]
    # new_ver = new_version.split(".")
    # new_ver = [int(item) for item in new_ver]

    # if old_ver[0] <= new_ver[0]:
    #     if old_ver[0] < new_ver[0]:
    #         return True
    #     if old_ver[1] <= new_ver[1]:
    #         if old_ver[1] < new_ver[1]:
    #             return True
    #         if old_ver[2] <= new_ver[2]:
    #             return True

    # print("新版本号错误，不能比历史最近版本号还小，old:%s, new:%s" % (old_version, new_version))


def new_dir(path, root=None):
    _path = path
    if root:
        _path = os.path.join(root, path)

    if os.access(_path, os.F_OK):
        raise ValueError("direct [%s] exsit." % path)
    
    print("create direct : %s" % path)
    os.mkdir(_path)


class Project:
    """项目开发工具.
    
    该工具用于创建c/c++组件和应用程序项目，并且能够辅助开发，具体请查看其他命令.
    """

    def __init__(self):
        self._config_path = "config.json"
        # self.config = {} # 定义字典可以分组
        self._config = None

    def create_project(self):
        """
        创建组件. \n
        创建组件项目，c/c++.
        """
        project_txt = [
            inquirer.Text("name", message="project name", validate=check_name),
            inquirer.Text("object", message="object name", validate=check_name),
            inquirer.Text("author", message="project author", validate=check_author),
            inquirer.Text("email", message="author email", validate=check_email)
        ]
        project_an_txt = inquirer.prompt(project_txt)

        project_select = [
            inquirer.List("project",
                          message="project type",
                          choices=["application", "component"]),
            inquirer.List("type",
                          message="language type",
                          choices=["c++", "c"]),
            inquirer.List("copyright",
                          message="project copytight",
                          choices=["MIT", "BSD", "custom"]),
        ]
        project_an_select = inquirer.prompt(project_select)

        c_time = datetime.now()
        licence = {}
        if "custom" == project_an_select["copyright"]:
            custom_licence_q = [
                # linux可以使用: select-editor来设置默认编辑器
                # 或者是: echo export EDITOR=/usr/bin/vim >> ~/.bashrc
                inquirer.Editor("license", message="input your own license")
            ]
            licence = inquirer.prompt(custom_licence_q)
        else:
            licence = LICENSES[project_an_select["copyright"]].substitute(year=c_time.year, author=project_an_txt["author"])

        cur = os.getcwd()

        # 创建项目目录
        project_path = os.path.join(cur, project_an_txt["name"])
        try:
            new_dir(project_path)
        except ValueError as e:
            print(e)
            exit(0)
        
        os.chdir(project_path)
        
        try:
            new_dir("doc")
            new_dir("src")
            new_dir("test")
            new_dir("third_party")
            new_dir("package")
            new_dir("tools")
            new_dir(".project")
        except ValueError as e:
            print(e)
            os.chdir(cur)
            os.removedirs(project_path)
            exit(0)
        
        with open("config.json", "w", encoding="utf-8") as f:
            config_template["project"]["name"] = project_an_txt["name"]
            config_template["project"]["type"] = project_an_select["project"]
            config_template["project"]["language"] = project_an_select["type"]
            config_template["build"]["obj name"] = project_an_txt["object"]
            config_template["file"]["author"] = project_an_txt["author"]
            config_template["file"]["email"] = project_an_txt["email"]
            config_template["file"]["copyright"] = licence
            f.write(json.dumps(config_template, indent=4))
        
        with open("LICENSE", "w", encoding="utf-8") as f:
            f.write(licence)
        
        with open("readme.md", "w", encoding="utf-8") as f:
            f.write("# %s" % project_an_txt["name"])
        
        with open(os.path.join(project_path, ".project/project_lock.json"), "w", encoding="utf-8") as f:
            project_lock["project tool version"] = __version__
            f.write(json.dumps(project_lock, indent=4))

        with open(os.path.join(project_path, ".project/version_log.json"), "w", encoding="utf-8") as f:
            version_log["version"][0]["version"] = "0.0.0"
            version_log["version"][0]["time"] = time.asctime(time.localtime(time.time()))
            f.write(json.dumps(version_log, indent=4))

    def create_c_file(self, name):
        """
        创建c源文件. \n
        创建c源码文件，会同时创建对应的头文件.
        :param name: 文件名称，可以不添加文件后缀
        """
        pass

    def create_cpp_file(self, name):
        """
        创建c++源文件. \n
        创建c++源码文件，会同时创建对应的头文件(.cpp, .h).
        :param name: 文件名称，可以不添加文件后缀
        """
        pass

    def create_h_file(self, name):
        """
        创建头文件. \n
        用于单独创建头文件.
        :param name: 文件名称，可以不添加文件后缀
        """
        pass

    def create_dir(self, name):
        """
        创建目录. \n
        用于创建目录，以项目路径为根路径，新增的[src|test]下的路径将用于编译，规则和普通路径一致.
        :param name: 目录路径
        """
        pass
    
    def build(self, debug=False):
        """
        构建项目. \n
        编译应用代码.
        :param debug: [False|True] 调试模式，默认非调试模式。
        """
        print("build", debug)
        print("version", self._version)

    def release(self, debug=False):
        """
        构建发布. \n
        构建组件，应用发布包.
        :param debug: [False|True] 调试模式，默认非调试模式。
        """
        print("release ....")

    def clean(self):
        """
        清除编译中间产物.
        """
        pass

    def distclean(self):
        """
        清除所有中间物.
        """
        pass

    def run_test(self):
        """
        运行测试代码.
        """
        pass

    def show_lib(self):
        """
        显示依赖库信息.
        """
        pass

    def build_lib(self, name=None):
        """
        构建依赖库.
        """
        pass


if __name__ == "__main__":
    fire.Fire(Project())
