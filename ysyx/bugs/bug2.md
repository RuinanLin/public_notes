# Bug 2

## 整理人：林睿楠；整理时间：2026.1.13

### 问题发生描述

在仿真双控开关例子的过程中，因为是组合逻辑，我们不需要开波形。于是发生了如下报错：

```
/home/laotansuan/ysyx-workbench/npc/common/include/sim_base.hpp: In member function ‘void SimBase::open_trace(const string&)’:

/home/laotansuan/ysyx-workbench/npc/common/include/sim_base.hpp:96:30: error: ‘class Vtop’ has no member named ‘trace’

   96 |                         top->trace(tfp, 99);    // Trace 99 levels of hierarchy

      |                              ^~~~~

make[1]: *** [Vtop.mk:64: sim_main.o] Error 1

make[1]: Leaving directory '/home/laotansuan/ysyx-workbench/npc/dual_control_switch/build/obj_dir'

%Error: make -C /home/laotansuan/ysyx-workbench/npc/dual_control_switch/build/obj_dir -f Vtop.mk exited with 2

%Error: Command Failed ulimit -s unlimited 2>/dev/null; exec /usr/local/bin/verilator_bin -cc --build -j -MMD -O3 --x-assign fast --x-initial fast --noassert -Wall -Wno-UNUSEDSIGNAL -Wno-DECLFILENAME --top-module top /home/laotansuan/ysyx-workbench/npc/dual_control_switch/vsrc/top.v /home/laotansuan/ysyx-workbench/npc/dual_control_switch/csrc/sim_main.cpp -CFLAGS -I/home/laotansuan/ysyx-workbench/npc/common/include -CFLAGS -I/home/laotansuan/ysyx-workbench/npc/dual_control_switch/include -CFLAGS -DTOP_NAME=Vtop --Mdir /home/laotansuan/ysyx-workbench/npc/dual_control_switch/build/obj_dir --exe -o /home/laotansuan/ysyx-workbench/npc/dual_control_switch/build/obj_dir/Vtop

make: *** [scripts/verilator.mk:96: /home/laotansuan/ysyx-workbench/npc/dual_control_switch/build/obj_dir/Vtop] Error 2
```

发现即使判断了宏定义`VM_TRACE`，在没有传入`--trace`的情况下，仍然编译了宏包围部分的逻辑。

### Bug原因

读`make`后具体得到的命令可知，在不传入`--trace`的情况下，`VM_TRACE`仍然是被定义的，只是值为`0`而已。所以区别不在于`VM_TRACE`是否被定义，而是值是`0`还是`1`。

### 解决方案

把`#ifdef VM_TRACE`改成`#if VM_TRACE`。