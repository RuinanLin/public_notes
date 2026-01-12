# Bug 1

## 整理人：林睿楠；整理时间：2026.1.12

### 问题发生描述

问题发生在`~/ysyx-workbench/npc/GCD/csrc/sim_main.cpp`。原先，所有的内容都写在`main()`函数中，我希望把`top`输入信号值的初始化过程通过一个函数调用`sim_input_init()`来实现。

我的实现细节如下：

```c++
// Initialize values of input signals
void sim_input_init(const std::unique_ptr<TOP_NAME> top) {
    top->clock = 0;
    top->reset = 0;
    top->io_value1 = 56;
    top->io_value2 = 48;
    top->io_loadingValues = 0;
}
```

发生报错如下：

```
/home/laotansuan/ysyx-workbench/npc/GCD/csrc/sim_main.cpp: In function ‘int main(int, char**)’:
/home/laotansuan/ysyx-workbench/npc/GCD/csrc/sim_main.cpp:117:31: error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = VGCD; _Dp = std::default_delete<VGCD>]’
  117 |                 sim_input_init(top);
      |                 ~~~~~~~~~~~~~~^~~~~
In file included from /usr/include/c++/11/memory:76,
                 from /home/laotansuan/ysyx-workbench/npc/GCD/csrc/sim_main.cpp:5:
/usr/include/c++/11/bits/unique_ptr.h:468:7: note: declared here
  468 |       unique_ptr(const unique_ptr&) = delete;
      |       ^~~~~~~~~~
/home/laotansuan/ysyx-workbench/npc/GCD/csrc/sim_main.cpp:45:53: note:   initializing argument 1 of ‘void sim_input_init(std::unique_ptr<VGCD>)’
   45 | void sim_input_init(const std::unique_ptr<TOP_NAME> top) {
      |                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~
make[1]: *** [VGCD.mk:65: sim_main.o] Error 1
make[1]: Leaving directory '/home/laotansuan/ysyx-workbench/npc/GCD/build/obj_dir'
%Error: make -C /home/laotansuan/ysyx-workbench/npc/GCD/build/obj_dir -f VGCD.mk exited with 2
%Error: Command Failed ulimit -s unlimited 2>/dev/null; exec /usr/local/bin/verilator_bin -cc --build -j -MMD -O3 --x-assign fast --x-initial fast --noassert -Wall --trace-fst -Wno-UNUSEDSIGNAL -Wno-DECLFILENAME --top-module GCD /home/laotansuan/ysyx-workbench/npc/GCD/vsrc/GCD.sv /home/laotansuan/ysyx-workbench/npc/GCD/csrc/sim_main.cpp -CFLAGS -DWAVE_FST -CFLAGS -DTRACE_PATH=/home/laotansuan/ysyx-workbench/npc/GCD/build/trace -CFLAGS -DTOP_NAME=VGCD --Mdir /home/laotansuan/ysyx-workbench/npc/GCD/build/obj_dir --exe -o /home/laotansuan/ysyx-workbench/npc/GCD/build/obj_dir/VGCD
make: *** [scripts/verilator.mk:95: /home/laotansuan/ysyx-workbench/npc/GCD/build/obj_dir/VGCD] Error 2
```

### Bug原因

当调用`sim_input_init(top)`时，C++会尝试复制一份`top`并传给函数。但是，`unique_ptr`已经显式地把它的拷贝构造器（`copy constructor`）给`delete`了。

所以，我们可以在报错中看到`error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = VGCD; _Dp = std::default_delete<VGCD>]’`。

### 解决方案

通过引用进行传参（`pass by reference`）：

```c++
// Initialize values of input signals
void sim_input_init(const std::unique_ptr<TOP_NAME>& top) {
    top->clock = 0;
    top->reset = 0;
    top->io_value1 = 56;
    top->io_value2 = 48;
    top->io_loadingValues = 0;
}
```