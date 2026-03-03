# NothingHere Windows Qt 版 - 产品需求文档 (PRD) + 技术实现指南

**项目名称**：NothingHere Windows  
**版本**：v1.0 (2026-03)  
**平台**：Windows 10 / Windows 11 (64-bit)  
**开发框架**：Qt 5+ (Widgets + Core)  
**语言**：C++ 
**目标**：100% 复刻 macOS NothingHere 的「一键恐慌按钮」体验，同时提供 Windows 原生最佳实现  
**体积目标**：≤ 8 MB (Release 打包后)  
**下载形式**：单个 .exe + 可选 .msi 安装包（可选 Inno Setup）

---

## 1. 产品概述

**一句话定位**：Windows 上的「老板来了」一键清理屏幕神器。

按一个全局热键（或 Guard Mode 下任意键），瞬间完成：
- 隐藏/最小化所有窗口（桌面瞬间干净）
- 静音系统 + 暂停所有媒体播放
- 自动打开预设的「正经工作文件」

**核心价值**：比手动 Win+M + 静音 + 打开文件快 10 倍以上，毫秒级无痕迹切换。

---

## 2. 目标用户与使用场景

- 程序员、设计师、学生、远程办公人群
- 开放式办公室 / 家里有家人 / 公共场所使用笔记本
- 喜欢「老板键 / Panic Button」梗的用户

---

## 3. 完整功能清单

### 3.1 核心 Panic Action（一键三连）
| 功能 | Windows 实现方式 | 触发时机 | 备注 |
|------|------------------|----------|------|
| **隐藏所有窗口** | `EnumWindows` + `ShowWindow(SW_MINIMIZE)`（排除自身、任务栏、桌面） | 热键按下瞬间 | < 50ms 完成 |
| **静音 + 暂停媒体** | 模拟 `VK_VOLUME_MUTE` + `VK_MEDIA_PLAY_PAUSE`（或 Core Audio API） | 热键按下瞬间 | 兼容绝大多数播放器 |
| **打开伪装文档** | `QDesktopServices::openUrl(QUrl::fromLocalFile(...))` | 热键按下瞬间 | 支持任意文件类型 |

**性能要求**：整个动作 ≤ 100ms。

### 3.2 配置功能
- **全局热键自定义**（默认建议 `Ctrl + Alt + H`）
- **伪装文档（Cover Document）**：支持任意本地文件，可随时修改
- **设置界面**：简洁弹窗（热键录入 + 文件选择 + 开关）

### 3.3 Guard Mode（守护模式）★ 重头功能
- 系统托盘常驻图标（`QSystemTrayIcon`）
- **武装状态**：图标变为「已武装」样式（可自定义两套 PNG）
- **任意键触发**：武装后按键盘**任意一个键**（包括修饰键）立即执行 Panic 并自动解除武装
- **实现方式**：低级键盘钩子 `WH_KEYBOARD_LL`（`SetWindowsHookEx`）
- **视觉反馈**：托盘图标实时切换 + 可选 Tooltip

### 3.4 系统托盘菜单
- 显示当前 Guard Mode 状态
- 一键开关 Guard Mode
- 打开设置
- 关于 / 检查更新 / 退出

### 3.5 其他功能
- 首次启动自动检查并提示「以管理员身份运行」（可选，非强制）
- 支持开机自启动（注册表或任务计划程序）
- 自动更新（推荐集成 `Qt-AutoUpdater` 或简单 GitHub Release 检查）
- 极简无窗口主界面（后台运行）
- 日志记录（可选，`QLoggingCategory`）

---

## 4. 非功能需求

- **权限**：推荐普通用户权限即可运行（钩子不需要管理员）；若使用 Core Audio 完整静音可提示管理员
- **兼容性**：Windows 10 1809+ / Windows 11 全系列
- **性能**：启动 < 800ms，内存占用 < 30 MB，CPU 几乎为 0
- **安全性**：不开网络（除更新外），不开任何不必要权限
- **国际化**：默认中文 + 英文（`QTranslator`）
- **打包**：windeployqt + NSIS/Inno Setup 单文件可选

---

## 5. 技术实现指南（Qt + Windows 核心代码参考）

### 5.1 项目依赖与 CMake 配置
```cmake
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
# 推荐子模块引入 QHotkey（https://github.com/Skycoder42/QHotkey）
add_subdirectory(3rdparty/QHotkey)