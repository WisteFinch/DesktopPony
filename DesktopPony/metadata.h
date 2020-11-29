/**
 * @file metadata.h
 * @brief 元数据
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2020 WisteFinch
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

const QString STR_VERSION = "build-xxxx.xx.xx";

const QString STR_DEFAULT_CONFIG_PATH = ":\\data\\default_config.ini";///<标准配置文件路径
const QString STR_CHARACTER_ROOT_PATH = ".\\character";///< 角色主地址
const QString STR_ACCESSORIES_ROOT_PATH = ".\\data\\accessories";///< 配件主地址
const QString STR_RULES_USER_ROOT_PATH = ".\\data\\rules\\";///< 规则（用户）主地址
const QString STR_RULES_SYSTEM_ROOT_PATH = ":\\data\\rules\\";///< 规则（系统）主地址



#endif // METADATA_H
