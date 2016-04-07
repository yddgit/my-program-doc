Apache的httpd命令详解
====================

`httpd`是Apache HTTP服务器程序。直接执行程序可启动服务器的服务。本文以`Apache2.2.21`版本为例详细介绍该指令的各个参数及用法。

**语法格式**：

```
httpd [-D name] [-d directory] [-f file]
      [-C "directive"] [-c "directive"]
      [-w] [-k start|restart|stop|shutdown]
      [-k install|config|uninstall] [-n service_name]
      [-v] [-V] [-h] [-l] [-L] [-t] [-T] [-S]
```

**参数选项**：

将ServerRoot指令设置初始值为serverroot。它可以被配置文件中的ServerRoot指令所覆盖。其默认值是/usr/local/apache2 。
```
-d serverroot
```

在启动中使用config作为配置文件。如果config不以"/"开头，则它是相对于ServerRoot的路径。其默认值是conf/httpd.conf 。
```
-f config
```

发送信号使httpd启动、重新启动或停止 。
```
-k start|restart|graceful|stop|graceful-stop
```

在读取配置文件之前，先处理directive的配置指令。
```
-C directive
```

在读取配置文件之后，再处理directive的配置指令。
```
-c directive
```

设置参数parameter ，它配合配置文件中的<IfDefine>段，用于在服务器启动和重新启动时，有条件地跳过或处理某些命令。
```
-D parameter
```

在服务器启动时，设置LogLevel为level 。它用于在启动时，临时增加出错信息的详细程度，以帮助排错。
```
-e level
```

将服务器启动过程中的出错信息发送到文件file 。
```
-E file
```

当在服务器编译中使用了SHARED_CORE规则时，它指定共享目标文件的目录为directory 。
```
-R directory
```

输出一个可用的命令行选项的简要说明。
```
-h
```

输出一个静态编译在服务器中的模块的列表。它不会列出使用LoadModule指令动态加载的模块。
```
-l
```

输出一个指令的列表，并包含了各指令的有效参数和使用区域。
```
-L
```

输出一个已经启用的模块列表，包括静态编译在服务器中的模块和作为DSO动态加载的模块。
```
-M
```

显示从配置文件中读取并解析的设置结果(目前仅显示虚拟主机的设置)
```
-S
```

在启动/重启的时候跳过根文件检查 (该参数在Apache 2.2.17及其以后版本有效)
```
-T
```

仅对配置文件执行语法检查。程序在语法解析检查结束后立即退出，或者返回"0"(OK)，或者返回非0的值(Error)。如果还指定了"-D DUMP_VHOSTS"，则会显示虚拟主机配置的详细信息。
```
-t
```

显示httpd的版本，然后退出。
```
-v
```

显示httpd和APR/APR-Util的版本和编译参数，然后退出。
```
-V
```

以调试模式运行httpd 。仅启动一个工作进程，并且服务器不与控制台脱离。
```
-X
```

**下列参数仅用于Windows平台**：

安装Apache为一个Windows NT的服务；改变Apache服务的启动方式；删除Apache服务。
```
-k install|config|uninstall
```

指定Apache服务的名称为name
```
-n name
```

保持打开控制台窗口，使得可以阅读出错信息。
```
-w
```

**在windows下使用http.exe命令的例子**：

平时我们使用最多的可能就是用http.exe命令安装、开启、停止，删除apache服务这些操作。
将Apache注册为windows服务，因为我们使用的是apache2.2版本，所以默认的服务名为"Apache2.2"。
```
httpd -k install
```
将Apache注册为windows服务，自己指定一个服务名字。
```
httpd -k install -n "服务名"
```
将Apache注册为windows服务，自己指定一个服务名字，并且使用特定配置文件。
```
httpd -k install -n "服务名" -f "conf\my.conf"
```
移除Apache服务，缺省地，将使用"Apache2.2"
```
httpd -k uninstall
```
移除Apache服务，自己制定一个服务名字。
```
httpd -k uninstall -n "服务名"
```
启动Apache服务。
```
httpd -k start
```
停止Apache服务。
```
httpd -k stop
```
重启Apache服务。
```
httpd -k restart
```