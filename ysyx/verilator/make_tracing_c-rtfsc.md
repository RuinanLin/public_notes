# 深入理解Verilator提供的项目例子`make_tracing_c`

## 整理人：林睿楠

Verilator提供的该项目目录在`verilator/examples/make_tracing_c/`下：

```
.
├── input.vc
├── Makefile
├── Makefile_obj
├── sim_main.cpp
├── sub.v
└── top.v

0 directories, 6 files

```

### 仿真波形导出

在`top.v`中，其他东西都是熟悉的，关键在于理解这一段：

```verilog
initial begin
    if ($test$plusargs("trace") != 0) begin
        $display("[%0t] Tracing to logs/vlt_dump.vcd...\n", $time);
        $dumpfile("logs/vlt_dump.vcd");
        $dumpvars();
    end
    $display("[%0t] Model running...\n", $time);
end
```

这些都是Verilog自带的一些系统函数。

`$test$plusargs("trace")`这个是在检查运行时，是否传入了`+trace`参数。如果传入了，才会开启VCD波形导出。`+trace`叫做plusargs，以加号开头，属于运行时参数。相关的配置可以在Makefile中看到：

```makefile
# Make waveforms
VERILATOR_FLAGS += --trace

...

run:
...

	@echo
	@echo "-- RUN ---------------------"
	@rm -rf logs
	@mkdir -p logs
	obj_dir/Vtop +trace
```

这两个`trace`选项有什么区别？它们一个是编译时选项，另一个是运行时选项。在编译时传入`--trace`，Verilator才会在编译时给生成的C++类中加入用于生成波形记录的API。在执行二进制文件时传入`+trace`字符串，运行时才会真正生成波形。

Verilog中的`$test$plusargs("trace")`检测到这个运行时选项，才会激活内部的`$dumpfile`逻辑。`$dumpfile("logs/vlt_dump.vcd")`指定波形文件的存储路径和文件名。

`$dumpvars()`告诉仿真器要记录哪些信号，调用格式通常如下：

```verilog
$dumpvars(level, module_or_signal);
```

根据参数的不同，调用的效果也不同：

| 用法 | 效果说明 |
| --- | --- |
| `$dumpvars();` | **最常用**。记录设计中**所有**模块的所有信号。 |
| `$dumpvars(0, top);` | 记录`top`模块及其下属**所有**子模块的所有信号（0代表无限深）。 |
| `$dumpvars(1, top);` | **仅**记录`top`模块顶层的信号，不进入任何子模块。 |
| `$dumpvars(2, top);` | 记录`top`模块及其第一层子模块的信号，不再向下延伸。 |
| `$dumpvars(0, top.module_a.sig_x);` | 仅记录特定的某个信号`sig_x`。 |

在`sim_main.cpp`中也有几处相关的配合：

```c++
    // Verilator must compute traced signals
    contextp->traceEverOn(true);

    // Pass arguments so Verilated code can see them, e.g. $value$plusargs
    // This needs to be called before you create any model
    contextp->commandArgs(argc, argv);
```

首先，`contextp->traceEverOn(true)`也是运行时决定是否生成波形的关键。那么问题来了：为什么不直接通过有没有`+trace`选项看是否要生成波形，而是还要加一个`contextp->traceEverOn(true)`呢？它主要是用来告诉运行时程序，得留一定的内存资源。

- **你需要`--trace`**，因为你可能在测试到第9999小时发现Bug，需要调试；
- **你平时不加`+trace`**，因为你不想在磁盘上生成几百TB的波形文件；
- **你保留`traceEverOn`**，是为了在C++里写个逻辑：“如果检测到错误信号，立即开始记录最后100个周期的波形”，例如`if (time > 1000) traceEverOn(true);`。

总结对比如下：

| 组件 | 代表意义 | 负责的事情 | 如果缺失 |
| --- | --- | --- | --- |
| `--trace` | **基础设施** | 编译时在C++代码里“挖好坑”，生成追踪用的函数。 | C++代码直接编译报错，说不知道`traceEverOn`是什么，连运行的机会都没有。 |
| `traceEverOn` | **内存资源** | 运行时在内存里“铺好路”，分配缓冲区。 | 运行到记录逻辑时，因为没有内存空间而失效。 |
| `+trace` | **用户开关** | 告诉Verilog逻辑“现在开始录制”。 | 仿真正常跑完，但不会产生波形文件。 |

而`contextp->commandArgs(argc, argv)`实现了把`+trace`传给了`contextp`。

### 断言

`sub.v`中大多数也是熟悉的，来看断言部分：

```verilog
   // An example assertion
   always_ff @ (posedge clk) begin
      AssertionExample: assert (!reset_l || count_c<100);
   end
```

首先，来看`always_ff`和`always`有什么区别。简单来说，`always_ff`是SystemVerilog引入的**更严谨、更具语义化**的关键字，而`always`是Verilog时代的通用关键字，在Verilator和现代数字设计中，强烈建议优先使用`always_ff`。

1. **语义明确性（Semantic Intent）**

    - `always`：它的语义非常宽泛。它可以用来描述**组合逻辑**、**时序逻辑**，甚至是**锁存器**（Latch）。仿真器和综合工具必须根据块内部的代码逻辑来“猜”你的意图。
    - `always_ff`：其中`ff`代表**Flip-Flop（触发器）**。它明确告诉工具：“我这段代码就是要生成时序逻辑寄存器”。

2. **工具检查与约束（Tool Checking）**

    这是两者最本质的区别。如果你使用了`always_ff`，编译器（如Verilator）会强制进行逻辑检查：
    
    - **非法赋值检查**：如果在`always_ff`块内部使用了阻塞赋值（`=`）而不是非阻塞赋值（`<=`），或者没有事件控制（如缺失`@posedge clk`），SystemVerilog编译器会报错。
    - **禁止组合逻辑**：如果在`always_ff`里写了组合逻辑（例如忘记写始终边沿），工具会直接提醒你：“这不符合触发器的特征”。
    - **禁止多驱动**：它能更好地帮助工具检查是否对同一个寄存器进行了多次驱动（Multiple Drivers）。

3. **软件仿真器的行为**

    对于Verilator这种“静态编译”的工具：

    - 使用`always_ff`可以帮助Verilator更快地构建**灵敏度列表（Sensitivity List）**。
    - 在`sub.v`的例子中，使用`always_ff`能让阅读代码的人（以及工具）一眼看出`count_c`是一个在时钟上升沿更新的硬件寄存器。

4. **为什么会有`always_ff`？**

    在Verilog-1995/2001时代，由于只有一个`always`，经常会出现以下低级错误：

    ```verilog
    // 程序员本意是写组合逻辑，但漏掉了某个信号在敏感列表里
    always @(a) begin
        out = a | b;    // 这种代码会产生意想不到的仿真结果（可能是Latch）
    end
    ```

    SystemVerilog引入了三个专门的关键字来解决这个问题：

    1. `always_ff`：专门用于触发器（时序逻辑）。
    2. `always_comb`：专门用于组合逻辑。
    3. `always_latch`：专门用于锁存器。

接下来来看**SystemVerilog即时断言（Immediate Assertion）**在时序逻辑中的典型用法。它是验证硬件逻辑是否符合预期的一种“自动监视器”。`AssertionExample`是**断言标号**，用于给这个断言起个名字。当仿真失败时，Verilator会在终端报错并指出是`AssertionExample`失败了。这比只打印文件名和行号要直观得多。其他内容显而易见。

### 覆盖率分析

来看`sub.v`中的这段代码：

```verilog
cover property (@(posedge clk) count_c == 3);
```

如果说“断言（Assertion）”是检查代码有没有**做错**，那么“覆盖率（Coverage）”就是检查代码有没有**做到**。以下是这一行代码的深度解析：

1. **语法结构拆解**

    - `cover`：关键字，告诉仿真器：“请观察并记录括号内的事件是否发生过”。
    - `property`：定义一个属性。在这里，它描述了一个特定的时序条件。
    - `@(posedge clk)`：采样时钟。它规定仿真器只在时钟的上升沿去检查后面的表达式。
    - `count_c == 3`：目标事件。这是我们关心的核心状态。

2. **它的实际意义是什么？**

    在验证复杂的硬件设计时，我们不仅要看测试是否通过，还要看测试是否**充分**。它告诉验证工程师：“在整个仿真生命周期中，时钟上升沿到来时，计数器确实达到过3这个值。”如果仿真结束了，但覆盖率报告显示这个属性从来没被触发（Hit），那说明你的测试用例根本没有跑完整个计数流程，测试是**不完整**的。

3. **工程联动：它是如何变成报告的？**

    这个简单的语句需要通过`Makefile`和`sim_main.cpp`的多方协作才能显现价值：

    1. **第一步：Makefile开启收集**

        `Makefile`中必须包含`--coverage`标志。这会让Verilator在生成的C++代码中植入覆盖率计数器。我们项目中`Makefile`是这么写的：

        ```makefile
        # Generate coverage analysis
        VERILATOR_FLAGS += --coverage
        ```
    
    2. **第二步：C++写入数据**

        在`sim_main.cpp`的末尾，有这么一段代码：

        ```c++
        #if VM_COVERAGE
            contextp->coveragep()->write("logs/coverage.dat");
        #endif
        ```

        这意味着，当仿真结束时，它会将内存中的覆盖率统计信息写入`logs/coverage.dat`这个二进制文件。

    3. **第三步：生成可读报告**

        `Makefile`中的`run`目标最后执行了：

        ```makefile
        $(VERILATOR_COVERAGE) --annotate logs/annotated logs/coverage.dat
        ```

        这会调用`verilator_coverage`工具，将二进制数据“翻译”回源代码。打开生成的`logs/annotated/sub.v`，会看到类似这样的标注：

        ```
        > 1: cover property (@(posedge clk) count_c==3);
        ```

        前面的`1`表示这个事件在仿真中发生了1次）。

### `sim_main.cpp`中的其他要点

#### `contextp->debug(0)`

看以下代码：

```c++
// Set debug level, 0 is off, 9 is highest presently used
// May be overridden by commandArgs argument parsing
contextp->debug(0);
```

它控制的是**Verilator运行时（Runtime）本身的内部调试行为**。这与你通过断言或打印语句调试自己的硬件逻辑不同，它更像是开启了仿真引擎的“透视模式”。

1. **它是干什么的？**

    `debug`级别决定了Verilator运行时库向标准输出（stdout/stderr）打印多少**内部状态信息**。

    - **当 debug = 0 时（默认值）**：仿真引擎保持安静，只运行你的电路逻辑，输出你在Verilog里写的`$display`信息。
    - **当 debug > 0 时**：仿真引擎会开始“碎碎念”，解释它每一刻在做什么。

2. **调试信息的具体内容**

    随着你将数值从1提高到9，你会看到越来越底层的信息：

    - **事件调度**：显示哪些信号触发了评估，哪些代码块正在被执行。
    - **文件操作**：显示波形文件（VCD）何时开启、何时刷新缓冲区。
    - **覆盖率/断言处理**：记录内部断言引擎的状态。
    - **内存管理**：对于复杂的模型，会显示内部数据结构的分配情况。
    - **多线程状态**：如果你开启了多线程，会显示线程间的同步和通信细节。

3. **注释中的两个关键细节**

    1. **"9 is highest presently used"**

        这代表了信息的密集程度。

        - **Level 1-3**：适合普通开发者，查看仿真是否按预期启动。
        - **Level 4-7**：适合排查Verilator自身工具链问题的专家。
        - **Level 9**：会产生海量的日志，每秒钟可能生成几GB的文本，通常只有在怀疑Verilator软件本身有Bug时才会开启。
    
    2. **"May be overridden by commandArgs"**

        这一行非常重要。它意味着你在代码里写的`debug(0)`只是一个**初始默认值**。在`sim_main.cpp`中，由于后面执行了：

        ```c++
        contextp->commandArgs(argc, argv);
        ```

        如果你在运行仿真时，在命令行输入：

        ```bash
        ./Vtop +verilator+debug+level+5
        ```

        那么程序会自动忽略代码里的`debug(0)`，改用命令行指定的等级`5`。这种设计让你**无需重新编译代码**就能动态改变调试深度。

4. **为什么平时要设为0？**

    - **性能极度损耗**：开启调试模式（特别是高等级）会使仿真速度下降10倍甚至100倍，因为磁盘IO成了瓶颈。
    - **日志污染**：过多的底层信息会淹没你自己在Verilog中写的打印信息，让你找不到真正的业务逻辑错误。

#### `contextp->randReset(2)`

```c++
// Randomization reset policy
// May be overridden by commandArgs argument parsing
contextp->randReset(2);
```

在硬件开发中，“未初始化的寄存器”是一个非常棘手的存在。`contextp->randReset(2);`这行代码正是为了处理这个问题，它的作用是设定仿真开始时，所有未显式初始化的信号（寄存器、变量等）的初始随机化策略。在真实的硬件电路中，寄存器在上电瞬间的状态是随机的（可能是0也可能是1）。为了让仿真更贴近真实情况，Verilator提供了这种机制。

1. **`randReset`的三个取值含义**

    | 数值 | 策略名称 | 具体行为 |
    | --- | --- | --- |
    | **0** | 全0初始化 | 将所有未初始化的信号强制设为`0`。这是最确定的模式，但可能会掩盖一些“依赖复位”的Bug。 |
    | **1** | 全1初始化 | 将所有未初始化的信号强制设为`1`。 |
    | **2** | 随机初始化 | **（本示例所用）**。Verilator会根据某种随机算法，将这些信号初始化为随机的`0`或`1`。（包括多位宽向量中的每一位）。 |

2. **为什么要使用`randReset(2)`？（核心价值）**

    如果你在Verilog里写了`reg [7:0] data;`但没有给它初值，也没有在`reset`时给它赋值：

    - **发现“复位缺失”Bug**：如果你的逻辑在复位撤销后直接读取了`data`，而`data`在模式2下是随机的，那么你的仿真结果每次运行可能都不一样。这会立刻暴露你的设计依赖了某个“未定义”的初始状态。
    - **模拟真实上电过程**：真实芯片上电时，触发器的物理状态是不确定的。通过随机化，可以验证你的复位逻辑（Reset Logic）是否足够鲁棒，能否将系统从任何随机状态拉回到正确的初始状态。

3. **注释中的"May be overridden by commandArgs"**

    这和之前的`debug`类似。虽然你在代码里写死了`2`，但如果你在执行可执行文件时加入特定参数：

    ```bash
    ./Vtop +verilator+rand+reset+0
    ```

    那么仿真器会忽略代码里的`2`，改用模式`0`。

### 