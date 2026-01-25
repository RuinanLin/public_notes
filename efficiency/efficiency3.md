# Efficiency 3：让Vim更好看

## 整理人：林睿楠
## 整理时间：2026.1.25

### 问题描述

在配置 Vim 插件[vim-gitgutter](https://github.com/airblade/vim-gitgutter)的时候，无意中发现了一个让 Vim 颜色瞬间变漂亮的设置：

```
set termguicolors
```

目前已经加到我的`~/.vimrc`文件当中了。

简单来说，`set termguicolors`的作用是让 Vim **开启真彩色（True Color）支持**。在没有开启这个选项时，Vim 默认使用的是 **Xterm 256 色模式**。256 色模式就像是一盒只有 256 支颜色的蜡笔，无论你的屏幕多好，Vim 只能从这 256 个预定义的颜色里找最接近的。这往往导致色彩过渡生硬，甚至出现“迷之发紫”或“迷之发绿”的情况。**Termguicolors (真彩色)**开启后，Vim 可以使用 24 位色（RGB），也就是支持 16,777,216 种颜色。这和你显示器、照片、浏览器支持的颜色范围是一致的。