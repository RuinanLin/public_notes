# 深入理解npc-chisel框架代码

## 整理人：林睿楠

这里的npc-chisel框架代码，对应我自己ysyx-workbench中的commit号`6c65e91`。项目的根目录在下面语境中都是`~/ysyx-workbench/npc/`。

1. `make test`做了什么

  首先，看`Makefile`中的规则：

    PRJ = playground

    test:
      mill -i $(PRJ).test
  
  首先，`-i`选项是什么意思。`mill --help`告诉我们：

    -i --interactive     Run Mill in interactive mode, suitable for opening REPLs and taking user input. This implies --no-server. Must be the first argument.
  
  就是运行在交互模式。

  - 禁用服务端模式（Non-Daemon）：默认情况下，Mill会启动一个后台守护进程（Server）来加速后续编译。使用`-i`会让Mill在当前进程中直接运行，任务结束后进程立即退出。
  - 启用标准输入（Standard Input）：这是它最重要的功能。在默认模式下，Mill的后台进程无法直接接收键盘输入。如果测试代码（例如`test`）需要用户在控制台输入任何内容，就必须加上`-i`。

  而在Chisel设计验证环境中，这一点很重要，因为Mill默认会并行运行测试。如果在两个终端里同时运行不带`-i`的命令，会发现它们可能共用同一个Mill Server，这时会发生各种资源的冲突，比如锁定端口（如果测试涉及网络协议仿真，可能会占用固定端口）、写死路径（很多EDA工具默认把日志写在固定的`top.log`里）、大内存占用（多个仿真同时跑，容易把内存挤爆）。总结一下，就是我们要追求`-i`带来的“确定性”：

  - 确定输入：确保你的键盘输入（stdin）能直接传给仿真器；
  - 确定路径：让Mill作为一个简单的、可预测的程序运行，而不是一个复杂的“客户端-服务器架构；
  - 确定环境：在Makefile这种可能存在并行调度的环境下，保护环境不被弄乱。

  mill需要配合一个叫`build.mill`的配置文件。它使用Scala语言编写，告诉mill程序你的项目有哪些模块、依赖哪些库、使用哪个版本的编译器等。下面是`build.mill`文件中的代码：

    package build

    // import Mill dependency
    import mill._
    import mill.define.Sources
    import mill.modules.Util
    import mill.scalalib.scalafmt.ScalafmtModule
    import mill.scalalib.TestModule.ScalaTest
    import mill.scalalib._
    // support BSP
    import mill.bsp._

    object playground extends ScalaModule with ScalafmtModule { m =>
      override def scalaVersion = "2.13.15"

      override def scalacOptions = Seq(
        "-language:reflectiveCalls",
        "-deprecation",
        "-feature",
        "-Xcheckinit"
      )

      override def ivyDeps             = Agg(ivy"org.chipsalliance::chisel:6.6.0")
      override def scalacPluginIvyDeps = Agg(ivy"org.chipsalliance:::chisel-plugin:6.6.0")

      object test extends ScalaTests with TestModule.ScalaTest with ScalafmtModule {
        override def ivyDeps = m.ivyDeps() ++ Agg(
          ivy"org.scalatest::scalatest::3.2.19",
          // for formal flow in future
          ivy"edu.berkeley.cs::chiseltest:6.0.0"
        )
      }

      def repositoriesTask = Task.Anon {
        Seq(
          coursier.MavenRepository("https://repo.scala-sbt.org/scalasbt/maven-releases"),
          coursier.MavenRepository("https://oss.sonatype.org/content/repositories/releases"),
          coursier.MavenRepository("https://oss.sonatype.org/content/repositories/snapshots")
        ) ++ super.repositoriesTask()
      }
    }

  `build.mill`定义了一个名为`playground`的模块，以及一个名为`test`的子模块。在Mill的世界里，`playground.test`这种写法被称为任务选择器（Task Selector）。
  
  因为`playground`继承了`ScalaModule`，Mill会自动为它分配标准的任务（Task），例如`compile`、`run`、`jar`等。这些任务通常分为只读配置项、操作任务和结果输出，遵循DAG（有向无环图）结构，像Make那样根据缓存与文件改动来构建项目。`test`也是一样的道理，只是它继承的是`ScalaTests`。

  以下是键入`mill -i playground.test`之后Mill详细的执行步骤：

  1. 路径自动发现（Path Resolution）

    由于`playground`模块继承了`ScalaModule`，Mill会按照默认约定查找代码。

    - 源码路径：Mill发现`src`目录下有`GCD.scala`等文件，会自动将`src/`映射为`playground.sources`。

      为什么Mill会自己发现`src`下的文件，并将它们映射到`playground.sources`？这是由`playground`继承的`ScalaModule`继承的`JavaModule`定义在它`sources`中的任务。在`mill.scalalib.JavaModule`的源码中，`sources`任务有一个默认实现，大致逻辑如下（伪代码）：

        ```scala
        def sources = T.sources {
          millSourcePath / "src"
        }
        ```
      
      假设你并不想把源码放在`playground/src`这个文件夹中，而是想放在`playground/code`，只需在`build.mill`中重写这个任务：

        ```scala
        object playground extends ScalaModule {
          // 修改默认的源码映射路径
          override def sources = T.sources {
            millSourcePath / "code"
          }
        }
        ```
    
    - 测试路径：Mill发现`test/src`目录，会自动将其映射为`playground.test.sources`。

      为什么`test`是个`object`，但也有任务？首先，`ScalaTests`是Mill定义的一个标准“任务包”；其次，复杂的继承链将带来许多继承而来的任务。

      如果想要查看`test`模块拥有多少个任务，可以运行：

      ```bash
      mill resolve playground.test._
      ```

  2. 构建任务流的详细拆解

    当敲下回车，Mill内部会发生以下连锁反应：

    1. 资源准备（Resource Mapping）

    Mill检查是否存在`resources`文件夹。虽然目录里没有显示，但Mill仍会生成一个空的`playground.resources`任务节点。

    首先，什么是`resources`？`sources`是什么很明确，就是源码，而`resources`代表了诸如`.conf`、`.json`、`.txt`、图像、硬件约束文件等一系列资源文件。

    按照约定，Mill会查找`millSourcePath / "resources"`文件夹，并将找到的所有文件拷贝到`out/playground/resources.dest/`中。在运行JVM时，它会将这个`.dest`目录加入到`Classpath`中。

    为什么硬件开发（Chisel）可能会需要它？

    - 内存初始化
