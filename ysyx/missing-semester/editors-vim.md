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

	- `set ignorecase` && `set smartcase`

		`set ignorecase`就是在搜索的时候忽略大小写．`set smartcase`则是基于`set ignorecase`做了一些改进：如果搜索串是全小写，则在搜索的时候忽略大小写；如果在搜索串中出现了大写，则进行严格匹配，大小写敏感．

	- `set incsearch`

		在搜索的过程中就开始匹配．

	- `nmap Q <Nop>`

		首先，nmap是一个修改映射的命令，意思是迭代地修改对应键在Normal状态下的映射．完整的对映射的介绍参考[这篇StackOverflow问答][3]．在这里，意思是改变大写Q的映射，使得在Normal状态下按下Q，什么都不会发生．

		[3]: <https://stackoverflow.com/questions/3776117/what-is-the-difference-between-the-remap-noremap-nnoremap-and-vnoremap-mapping> "Introduction to different vim mapping commands"

		然后来看原来的Q是什么东西，为什么要取消它的映射．原来的Q是一个Vi留下的很老的功能，叫Ex模式，非常麻烦．为了避免哪天误触到Q，我们需要把它映射成什么都不做．

	- `set noerrorbells visualbell t_vb=`

		当你有一些无效的键入时，默认情况下Vim可能会有一些音效或者视觉上的提示．有些人觉得这些很烦，所以把禁止Vim使用这些提示．

	- `set mouse+=a`

		使能鼠标的使用．a指的是所有的模式下．

	- `nnoremap <Left> :echoe "Use h"<CR>`

		将使用箭头键移动映射成一种报错，从而改掉使用箭头键来移动的坏习惯．`:echoe`是指报错输出．<CR>的作用相当于回车，如果没有它，报错之后你要自己敲回车．<br><br>

3. Install and configure a plugin: [ctrlp.vim][4].

	[4]: <https://github.com/ctrlpvim/ctrlp.vim> "ctrlp.vim"

	1. Create the plugins directory with `mkdir -p ~/.vim/pack/vendor/start`

	2. Download the plugin: `cd ~/.vim/pack/vendor/start; git clone https://github.com/ctrlpvim/ctrlp.vim`

	3. Read the [documentation][5] for the plugin. Try using CtrlP to locate a file by navigating to a project directory, opening Vim, and using the Vim command-line to start `:CtrlP`.

		[5]: <https://github.com/ctrlpvim/ctrlp.vim/blob/master/readme.md> "ctrlp documentation"

	4. Customize CtrlP by adding [configuration][6] to your `~/.vimrc` to open CtrlP by pressing Ctrl-P.

		[6]: <https://github.com/ctrlpvim/ctrlp.vim/blob/master/readme.md#basic-options> "ctrlp configuration added to ~/.vimrc"

	第1~2步都是比较简单的，略．接下来对CtrlP这个插件进行一下简要的总结．

	- 三种基本的模式：file，buffers，MRU．file是找当前目录下的文件，buffers是找当前会话打开的其他文件，MRU是找过去打开过的文件．当按下Ctrl+R的时候，默认打开模式是file，可以通过`<c-f>`和`<c-b>`来切换模式．

	- 可以对选中的文件通过分屏打开．`<c-t>`是创建新的tab，`<c-v>`和`<c-x>`是创建新的window．

	- `<c-y>`用来创建一个新文件．

	- `<c-z>`用于多选选中，`<c-o>`是在选中多个文件之后打开它们．

	- 提交`..`可以在原先目录的基础上向上一个目录．

	对于问题的第4条，在仔细研究之后，发现很多配置已经在CtrlP中默认实现了，所以绝大多数都没有写进我的`~/.vimrc`，除了最后一个忽略`.gitignore`中列出文件的：

		  let g:ctrlp_user_command = ['.git', 'cd %s && git ls-files -co --exclude-standard']

	`g:ctrlp_user_command`是一个CtrlP会用到的变量，它是一个list．在使用时，它的意思是：如果在当前目录或者更上层的目录中发现了`.git`，则在呈现搜索结果的时候，使用后面的那条命令．后面那条命令就是列出git文件夹中除了被排除的文件以外的其他文件．<br><br>

4. To practice using Vim, re-do the [Demo](https://missing.csail.mit.edu/2020/editors/#demo) from lecture on your own machine.

	略．<br><br>

5. Use Vim for *all* your text editing for the next month. Whenevere something seems inefficient, or when you think "there must be a better way", try Googling it, there probably is. If you get stuck, come to office hours or send us an email.

	略．<br><br>

6. Configure your other tools to use Vim bindings (see instructions above).

	尝试使用Chrome浏览器的Vim emulation工具[Vimium](https://vimium.github.io/)，发现真的能够极大地提高使用Chrome浏览器的效率．

	尤其是`f/F`，`j`，`k`，`J`，`K`，`T`，`H`，`L`，`m`等一系列操作，真的是极大地简化了浏览器使用时非常常见的一些操作．

	还有就是记住`?`相当于help，可以经常打开来看看．<br><br>

7. Further customize your `~/.vimrc` and install more plugins.

	`~/.vimrc`就不说了，已经用得很熟了，也和ysyx里面提供的那个版本融合过了，接下来重点探索一下课件上提到的其他插件．

	1. 首先来看[vim-easymotion](https://github.com/easymotion/vim-easymotion)。
	
		它能让你仅通过敲击键盘上的1~2个键来实现光标的快速移动。
		
		如果你在使用vim的时候不使用vim-easymotion这种插件，那么，当你需要移动光标的时候，你就需要使用`<number>w`或者`<number>f{char}`等命令。即使你使用`set relativenumber`显示了相对行号，你也要在移动的时候用余光看一下相对行号，还要自己键入，还是很麻烦。

		首先总结在不使用自行配置的前提下使用vim-easymotion的方法。使用vim-easymotion，你需要首先键入`<Leader><Leader>`来触发它。紧接着，你需要键入一个动作，例如`w`（以单词为单位移动）或者`fe`（移动到下一个`e`字符处）等。这时，vim-easymotion会将所有符合条件的地方用一个或两个字符标识，只要再键入对应的标识，就可以把光标移动到对应的位置。

		以下是一些频繁配合使用的动作：

		- `w/b`，分别是以单词为单位向后或向前；
		- `f<char>/F<char>/s<char>`，分别是找到后续的`<char>`、前面的`<char>`以及全局u的`<char>`；
		- `j/k`，行跳转，向下或向上；
		
		在以上总结中，可以看到默认情况下的使用有一些问题：

		- `<Leader>`需要键入两遍，这是完全没有必要的；
		- 默认条件下的`<Leader>`是`'\'`，它在键盘上很远的位置；
		- 不能在多个窗口之间跳转；
		- 大小写敏感，事实上大多数情况下，无论大小写都列出来是更加方便的。

		因此，可以在`~/.vimrc`中进行配置：

			" Set vim-easymotion's <Leader> as a space, which has not been mapped and is easy to reach on a keyboard.
			let mapleader = " "

			" Disable default mappings
			let g:EasyMotion_do_mapping = 0

			" Manually map the motions in order to enable one <Leader> and overwin motions
			nmap <Leader>w <Plug>(easymotion-overwin-w)
			nmap <Leader>e <Plug>(easymotion-e)
			nmap <Leader>b <Plug>(easymotion-b)

			nmap <Leader>j <Plug>(easymotion-j)
			nmap <Leader>k <Plug>(easymotion-k)
			nmap <Leader>l <Plug>(easymotion-lineforward)
			nmap <Leader>h <Plug>(easymotion-linebackward)

			" This one is really useful, which can jump to any character
			" But it seems that we cannot map it to <Leader>s, because in Normal mode, s
			" means delete the current character and enter Insert mode
			" So we map it to <Leader>;
			nmap <Leader>; <Plug>(easymotion-s)
