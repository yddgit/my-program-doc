# MongoDB

## Window下MongoDB的安装

下载地址：https://www.mongodb.com/download-center#community

直接双击运行`.msi`安装包，安装过程中可指定安装目录。

## 运行MongoDB

MongoDB需要一个数据目录存放所有数据，默认是磁盘根目录下的data\db,该目录必须提前创建好。然后运行如下命令启动：

```bat
"C:\Program Files\MongoDB\Server\3.6\bin\mongod.exe" --dbpath d:\test\mongodb\data
```

*如果路径中包含空格，需要用`""`括起来*

## 连接MongoDB

执行如下命令连接MongoDB

```bat
"C:\Program Files\MongoDB\Server\3.6\bin\mongo.exe"
```

## 将MongoDB配置成Window服务

首先创建数据和日志目录

```bat
mkdir c:\data\db
mkdir c:\data\log
```

创建配置文件`C:\Program Files\MongoDB\Server\3.6\mongod.cfg`

```yml
systemLog:
    destination: file
    path: c:\data\log\mongod.log
storage:
    dbPath: c:\data\db
```

安装MongoDB服务

```bat
"C:\Program Files\MongoDB\Server\3.6\bin\mongod.exe" --config "C:\Program Files\MongoDB\Server\3.6\mongod.cfg" --install
```

也可通过`--dbpath`选项直接指定数据目录。如果要安装多个实例，可以指定`--serviceName`和`--serviceDisplayName`

启动MongoDB服务

```bat
net start MongoDB
```

停止并删除MongoDB服务

```bat
net stop MongoDB
"C:\Program Files\MongoDB\Server\3.6\bin\mongod.exe" --remove
```