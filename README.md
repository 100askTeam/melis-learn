# 说明

这是基于全志 melis4.0 编写的组件包，仅供学习参考使用！

# 使用方法

将本仓库的内容放在 `melis4.0/ekernel/subsys/` 的目录下：

```shell
D1s-Melis/ekernel/subsys/melis-learn
```

然后在 `melis4.0/ekernel/subsys/Kconfig` 中包含 melis-learn 中的 Kconfig ：

```shell
source "ekernel/subsys/melis-learn/Kconfig"
```

在 `melis4.0/ekernel/subsys/Makefile` 中包含 melis-learn ：

```shell
obj-${CONFIG_SUBSYS_100ASK} += melis-learn/
```

然后，执行 `make menuconfig` 进入到 `> Kernel Setup > Subsystem support` 将 `100ASK Components Support  ---> ` 打开，再进入其中选择你想要的使用的测试。

最后，编译、打包、烧写镜像到板子，进入到控制台，执行 `help` 命令会看到多出了以 **100ask_** 为前缀的命令。