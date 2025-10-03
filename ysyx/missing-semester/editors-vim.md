# Editors (Vim)

## 整理人：林睿楠

1. Complete `vimtutor`. Note: it looks best in a [80x24][1] (80 columns by 24 lines) terminal window.

	[1]: <https://en.wikipedia.org/wiki/VT100> "VT100"

	已完成，略．<br><br>
	
2. Download our [basic vimrc][2] and save it to `~/.vimrc`. Read through the well-commented file (using Vim!), and observe how Vim looks and behaves slightly differently with the new config.

	[2]: <https://missing.csail.mit.edu/2020/files/vimrc> "basic vimrc file"

	因为我已经有了一个`~/.vimrc`了，所以我会先逐行分析课件给出的文件是什么意思，再看看是否在我自己的`~/.vimrc`中采纳．

	- `set nocompatible`

		首先，什么叫compatible．compatible指的是Vim试图兼容Vi，但实现兼容的同时，Vim一些特有的高级功能将无法使用．所以，一般会在`~/.vimrc`的第一行就设置nocompatible选项，来使能更好的Vim功能．

		但其实，在`~/.vimrc`当中，这一行是多余的．这是因为，在默认情况下，当Vim发现`~/.vimrc`之后，会自动设置为nocompatible，因为设置了`~/.vimrc`一般都想用Vim的功能．在这里，之所以还写上了这一行，是因为这是一种防御性编程行为．事实上，如果并不使用`~/.vimrc`作为初始化配置文件，比如使用`vim -u foo`把foo作为初始化配置文件的时候，就必须写上这一行`set nocompatible`，否则Vim不会默认设置nocompatible．在`~/.vimrc`中写的确在功能上是多余的，但是一种防御性行为．

	- `syntax on`

		这个就是使能语法高亮，具体可用的高亮规则文件可以在/usr/share/vim/vim82/syntax/中查看，例如，你可以看到一些名为c.vim, python.vim, markdown.vim的文件．

		顺便提一句：如何显示在当前的Vim会话中，Vim已经加载了哪些规则？可以在Vim中运行`:scriptnames`即可．

	- `set shortmess+=I`

		shortmess这个选项能够控制Vim能显示什么信息，它的值是一系列单字母的flag，I就是flag之一，作用是隐藏启动介绍信息．当Vim启动时没有给出文件的时候，Vim会给出启动介绍信息，如下图所示：

		![Vim's startup intro message](./vim-startup-intro-message.png "Vim's startup intro message")

		而当加入了`set shortmess+=I`这行设置之后，当不给文件就启动Vim时，Vim只会给出一片空白，即空的buffer的界面：

		![Blank buffer after setting shortmess](./blank-buffer.png "Blank buffer after setting shortmess")

	- `set number`

		很简单，就是显示行号．

	- `set relativenumber`

		这个东西配合上面的`set number`一起用会非常好用．当仅仅设置了`set number`的时候，显示的行号是绝对行号：

		![Line numbers shown when relativenumber is not set](./absolute-line-number.png "When relativenumber is not set")

		而增设了`set relativenumber`之后，呈现的效果是这样的：

		![Line numbers shown when relativenumber is set](./relative-line-number.png "When relativenumber is set")

		当光标停留在第23行的时候，其他行呈现出来的行号就都是相对于23行的相对行号，这样就能非常方便地使用{count}k或者{count}j的方式来进行上下行的快速移动了．

	- `set laststatus=2`

		总是显示状态栏．laststatus默认的值是1，在这种情况下，状态栏只有在Ctrl+G的时候可以显示．当设置为2之后可以一直显示．

	- `set backspace=indent,eol,start`

		这个是指，在兼容Vi的情况下，Insert模式下的backspace行为有些反常．这一行就是说能让backspace也能删除一些缩进等东西．

		但是，在高版本的Vim中，backspace这个变量的默认值就已经是indent,eol,start了，可以通过运行`:set backspace?`这个命令来验证一下当前的值．这里应该也是作为一项防御性编程．

	- `set hidden`

		在默认情况下，如果你修改了一个buffer并且没有保存，Vim不允许你切换到别的buffer．虽然这可能可以提供一些保护，但是在有些情况下很麻烦．`set hidden`就可以避免掉这个设定．
