# NothingWin

[![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11-blue?logo=windows)](https://github.com/yourrepo/NothingWin)
[![Version](https://img.shields.io/badge/version-1.0.0-green)](https://github.com/yourrepo/NothingWin/releases)
[![License](https://img.shields.io/badge/license-MIT-orange)](LICENSE)
[![Qt](https://img.shields.io/badge/Qt-5.15+-41CD52?logo=qt)](https://www.qt.io/)

**🖥️ Windows 上的「老板来了」一键清理屏幕神器**

按一个全局热键，瞬间完成：隐藏所有窗口 · 静音系统 · 暂停媒体 · 打开伪装文档

---

**📑 目录**

1. [项目简介](#-项目简介)
2. [核心功能](#-核心功能)
3. [工作原理](#-工作原理)
4. [快速开始](#-快速开始)
5. [使用场景](#-使用场景)
6. [权限说明](#-权限说明)
7. [工程结构](#-工程结构)
8. [技术栈](#-技术栈)
9. [系统要求](#-系统要求)
10. [编译指南](#-编译指南)
11. [许可证](#-许可证)

---

## 🚀 项目简介

**NothingWin** 是一个轻量级的 Windows 隐私保护工具，专为需要快速切换工作场景的用户设计。

当你在摸鱼、聊天、看视频时，只需按下一个热键，程序立即：
- 🪟 隐藏/最小化所有窗口
- 🔇 静音系统音频
- ⏸️ 暂停所有媒体播放
- 📄 自动打开预设的「正经工作文件」

整个过程在 **100ms 内** 完成，让你的桌面瞬间变得「人畜无害」。

### 设计哲学

- **⚡ 极速响应** - 从触发到完成 < 100ms
- **🎯 零学习成本** - 安装即用，无需配置
- **🔒 隐私本地** - 不收集任何用户数据
- **💪 极简依赖** - 仅依赖 Qt 运行时，无其他三方库

---

## 🎯 核心功能

### 一键 Panic

| 动作 | 说明 |
|------|------|
| 隐藏窗口 | 最小化所有可见窗口，桌面瞬间干净 |
| 系统静音 | 静音主音频设备，防止声音泄露 |
| 暂停媒体 | 暂停所有活跃媒体播放器 |
| 伪装文档 | 打开预设的「工作文件」（Excel/PDF/代码等） |

### 全局热键

- **默认热键**: `Ctrl + Alt + H`
- **可自定义**: 在设置中修改为任意组合键
- **全局监听**: 任何窗口焦点下均可触发

### Guard Mode（守护模式）

激活后，按键盘**任意键**立即触发 Panic——专为「键盘型摸鱼」场景设计：

1. 启用 Guard Mode（托盘菜单或设置）
2. 托盘图标变为「已武装」状态
3. 按任意键盘 → 瞬间清理屏幕

### 系统托盘

- 🛡️ **托盘图标** - 实时显示 Guard Mode 状态
- 📋 **右键菜单**:
  - Arm/Disarm Guard Mode
  - Settings（设置）
  - About（关于）
  - Quit（退出）

---

## 🔄 工作原理

```
┌─────────────────────────────────────────────────────────┐
│                    用户触发 Panic                        │
│                   (热键 / 任意键)                        │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│              并行执行（< 100ms）                         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐ │
│  │ 隐藏窗口 │  │ 系统静音 │  │ 暂停媒体 │  │ 打开伪装 │ │
│  │          │  │          │  │          │  │          │ │
│  │ EnumWindows│ │ IAudio  │ │ SMTC      │ │ ShellExec│ │
│  │ Minimize  │ │ Mute    │ │ Pause     │ │ Open     │ │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘ │
└─────────────────────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│                    桌面已清理 ✅                         │
│              托盘图标更新 → 已激活状态                   │
└─────────────────────────────────────────────────────────┘
```

### 核心技术

- **窗口枚举**: `EnumWindows` + `GetWindowPlacement` 遍历并最小化
- **音频控制**: `IAudioEndpointVolume` 控制主音量
- **媒体控制**: `SystemMediaTransportControls` 暂停媒体
- **文件打开**: `ShellExecute` 调用默认应用

---

## 📦 快速开始

### 预编译版本（推荐）

1. 从 [Releases](https://github.com/yourrepo/NothingWin/releases) 下载 `NothingWin.exe`
2. 双击运行即可
3. 默认热键 `Ctrl + Alt + H` 已激活

### 首次使用配置

1. 右键托盘图标 → **Settings**
2. 选择你的「伪装文档」（推荐：Excel 表格 / PDF 文档 / 代码编辑器）
3. （可选）修改热键组合
4. （可选）启用 Guard Mode

---

## 💡 使用场景

| 场景 | 操作 | 效果 |
|------|------|------|
| 💼 **办公室摸鱼** | 刷网页时听到脚步声 | 按热键 → 桌面只剩 Excel |
| 🎮 **上班打游戏** | 游戏窗口 + 耳机 | 按热键 → 窗口消失 + 静音 |
| 📱 **聊天摸鱼** | 微信/钉钉聊天中 | 按热键 → 聊天窗口最小化 |
| 🎬 **看视频** | 浏览器看 B 站 | 按热键 → 视频暂停 + 窗口隐藏 |
| ⌨️ **键盘型摸鱼** | 持续敲键盘玩游戏 | 启用 Guard Mode → 按任意键触发 |

---

## 🔐 权限说明

本程序需要的权限：

| 权限 | 用途 | 安全风险 |
|------|------|----------|
| 窗口枚举 | 最小化所有窗口 | ✅ 无风险 |
| 全局热键注册 | 监听快捷键 | ✅ 无风险 |
| 键盘钩子 | Guard Mode 任意键触发 | ✅ 仅监听，不记录 |
| 媒体控制 | 静音/暂停媒体 | ✅ 无风险 |
| 文件打开 | 打开伪装文档 | ✅ 用户指定文件 |

**无需管理员权限** - 程序可在普通用户权限下运行所有功能。

**隐私承诺**: 不收集、不上传任何用户数据。所有配置本地存储。

---

## 📁 工程结构

```
NothingWin/
├── src/
│   ├── main.cpp                  # 程序入口
│   ├── panic_engine.cpp          # Panic 核心引擎
│   ├── panic_engine.h
│   ├── hotkey_manager.cpp        # 全局热键管理
│   ├── hotkey_manager.h
│   ├── audio_controller.cpp      # 音频控制（静音）
│   ├── audio_controller.h
│   ├── media_controller.cpp      # 媒体控制（暂停）
│   ├── media_controller.h
│   ├── window_enumerator.cpp     # 窗口枚举与隐藏
│   ├── window_enumerator.h
│   ├── tray_icon.cpp             # 系统托盘
│   ├── tray_icon.h
│   ├── settings_dialog.cpp       # 设置对话框
│   ├── settings_dialog.h
│   └── resources/
│       ├── icon_normal.ico       # 托盘图标（未武装）
│       └── icon_armed.ico        # 托盘图标（已武装）
├── cmake/
│   └── FindQt5.cmake
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

## 🛠️ 技术栈

| 组件 | 选型 | 理由 |
|------|------|------|
| 语言 | C++17 | 性能优先，系统级 API 调用 |
| GUI 框架 | Qt 5.15+ | 跨平台支持，系统托盘/热键成熟方案 |
| 构建系统 | CMake | 标准 C++ 构建工具 |
| 窗口 API | Win32 API | 原生 Windows 窗口控制 |
| 音频 API | Windows Core Audio | 系统级音量控制 |
| 媒体 API | Windows SMTC | 系统媒体传输控制 |

---

## 💻 系统要求

| 要求 | 说明 |
|------|------|
| 操作系统 | Windows 10 (1809+) / Windows 11 (64-bit) |
| 运行时 | Qt 5.15+ Runtime（或使用预编译版本） |
| 内存占用 | ~30 MB |
| 磁盘空间 | ~50 MB（含 Qt 运行时） |
| 权限 | 普通用户权限即可（无需管理员） |

---

## 🔨 编译指南

### 环境准备

1. 安装 **Qt 5.15+** (MinGW 或 MSVC)
2. 安装 **CMake 3.16+**
3. 安装 **Git**

### 编译步骤

```bash
# 克隆仓库
git clone https://github.com/yourrepo/NothingWin.git
cd NothingWin

# 创建构建目录
mkdir build && cd build

# 配置（MinGW）
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..

# 配置（MSVC）
# cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release ..

# 编译
cmake --build . --config Release

# 部署 Qt 运行时
windeployqt Release/NothingWin.exe
```

### 生成可执行文件

编译完成后，`build/Release/` 目录下将生成：
- `NothingWin.exe` - 主程序
- Qt DLLs - 运行时依赖
- `platforms/` - 平台插件

打包整个目录即可分发。

---

## 📄 许可证

MIT License

Copyright (c) 2026 NothingWin

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

---

<div align="center">

**Made with ❤️ by NothingWin Team**

[Report Bug](https://github.com/yourrepo/NothingWin/issues) · [Request Feature](https://github.com/yourrepo/NothingWin/issues)

</div>
