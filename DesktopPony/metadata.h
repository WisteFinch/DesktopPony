/**
 * @file metadata.h
 * @brief 元数据
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2021 WisteFinch
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
 */

#ifndef METADATA_H
#define METADATA_H

#include<QString>

const QString STR_APP_VERSION = QStringLiteral("build-xxxx.xx.xx"); ///< 程序版本
const QString STR_APP_COPYRIGHT = QStringLiteral("© 2021 WisteFinch");  ///< 版权
const QString STR_APP_AUTHOR = QStringLiteral("WisteFinch");///< 作者
const QString STR_APP_LICENSE = QStringLiteral("MIT");   ///< 许可证
const QString STR_APP_LICENSE_URL = QStringLiteral("https://github.com/WisteFinch/DesktopPony/blob/master/LICENSE");///< 许可证地址
const QString STR_APP_GITHUB_URL = QStringLiteral("https://github.com/WisteFinch/DesktopPony"); ///< GitHub地址

// 插件
const QString STR_CONFIG_PATH = QStringLiteral("./config.json");///< 配置文件路径
const QString STR_PLUGIN_PATH = QStringLiteral("./plugins/");   /// 插件路径
const QString STR_PLUGIN_HEAD_PATH = QStringLiteral("plugin.json"); /// 插件对象头文件名称
const QString STR_PLUGIN_ELEMENT_HEAD_PATH = QStringLiteral("element.json");/// 插件元素头文件名称

// 文本
const QString STR_PARA_REPLACE_REGEXP = QStringLiteral("(§\\[.*?\\])"); ///< 文本参数替换正则表达式

// ui
const QString STR_IMG_PATH = QStringLiteral(":/resources/img/");///< 图像资源路径

const QString STR_DEFAULT_CONFIG_PATH = QStringLiteral(":resources/data/default_config.ini");   ///<标准配置文件路径
const QString STR_SYSTEM_PLUGIN_PATH = QStringLiteral(":resources/plugins/");   /// 系统插件路径
const QString STR_CHARACTER_ROOT_PATH = QStringLiteral("./character/"); ///< 角色主地址
const QString STR_ACCESSORIES_ROOT_PATH = QStringLiteral("./data/accessories/");///< 配件主地址
const QString STR_RULES_USER_ROOT_PATH = QStringLiteral("./data/rules/");   ///< 规则（用户）主地址
const QString STR_RULES_SYSTEM_ROOT_PATH = QStringLiteral(":resources/data/rules/");///< 规则（系统）主地址



#endif // METADATA_H
