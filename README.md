# NothingHere Windows

Windows 上的「老板来了」一键清理屏幕神器。

按一个全局热键，瞬间完成：
- 隐藏/最小化所有窗口（桌面瞬间干净）
- 静音系统 + 暂停所有媒体播放
- 自动打开预设的「正经工作文件」

## Features

### 核心功能
- **一键 Panic**: 按下热键，瞬间隐藏所有窗口、静音、暂停媒体、打开伪装文档
- **全局热键**: 支持自定义热键（默认 `Ctrl+Alt+H`）
- **Guard Mode（守护模式）**: 激活后按键盘任意键立即触发 panic

### 系统托盘
- 托盘图标常驻
- 右键菜单：Arm/Disarm Guard Mode、Settings、About、Quit
- 托盘图标状态实时显示（已武装/未武装）

### 设置
- 自定义全局热键
- 选择伪装文档（支持任意文件类型）
- Guard Mode 开关

## System Requirements

- Windows 10 (1809+) / Windows 11 (64-bit)
- Qt 5.15+ Runtime
- 约 30MB 内存占用
- 无需管理员权限（普通用户即可运行）

## Installation

### 预编译版本
1. 下载 `NothingHere.exe`
2. 运行即可

### 从源码编译
```bash
# 安装 Qt 5 (MinGW)
# 克隆项目
git clone https://github.com/yourrepo/NothingHere.git
cd NothingHere

# 构建
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make

# 部署
windeployqt NothingHere.exe
```

## Permissions

本程序需要的权限：

| 权限 | 用途 |
|------|------|
| 窗口枚举 | 最小化所有窗口 |
| 全局热键注册 | 监听快捷键 |
| 键盘钩子 | Guard Mode 任意键触发 |
| 媒体控制 | 静音/暂停媒体 |
| 文件打开 | 打开伪装文档 |

**无需管理员权限** - 程序可以在普通用户权限下运行所有功能。

## License

MIT License

Copyright (c) 2026 NothingHere

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
