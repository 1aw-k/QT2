# 实验 2：简单文本编辑器

这是一个基于 Qt Widgets 的多窗口文本编辑器工程，可直接用 Qt Creator 打开
`QT2.pro` 运行。

## 环境

- Qt 5.12.11
- Desktop Qt 5.12.11 MinGW 64-bit
- qmake

## 已实现功能

- 新建、打开、保存、另存为和未保存修改提示
- 文本编辑、撤销/重做、剪切/复制/粘贴/全选
- 查找与替换，包括连续查找、方向、区分大小写和全部替换
- 字体、字体颜色、字体背景色和编辑器背景色
- 自动换行、行号显示、工具栏和状态栏显示控制
- 光标行列、字符数和行数状态显示
- 每 60 秒自动保存一次已命名且已修改的文件
- 二进制文件打开保护、文件读写失败提示和原子保存

## 运行

1. 使用 Qt Creator 打开 `QT2.pro`。
2. 选择 `Desktop Qt 5.12.11 MinGW 64-bit` 套件。
3. 构建并运行。

命令行构建方式：

```powershell
New-Item -ItemType Directory build -Force
Set-Location build
$env:PATH = "D:\Qt\5.12.11\mingw73_64\bin;D:\Qt\Tools\mingw730_64\bin;$env:PATH"
qmake ..\QT2.pro
mingw32-make -j4
```
