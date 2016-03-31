1. docker安装
===================================

Required:
 * Linux, x64, kernel > 3.10
 * 服务器正常连网

使用docker官网的安装脚本安装
--------------------------

  1. 使用命令如下命令查看内核版本：

     ```
     uname -r
     3.10.0-327.3.1.el7.x86_64
     ```
  
  2. 更新系统（可以避免一些已发现有bug）

     ```
     yum update
     ```

  3. 执行安装脚本

     ```
     curl -sSL https://get.docker.com/ | sh
     ```

  4. 启动docker服务

     ```
     systemctl start docker.service
     ```

  5. 查看docker服务运行状态

     ```
     systemctl status -l docker.service
     ```

  6. **Note**: 如果没有执行`yum update`更新系统，在查看docker服务服务状态时可能会显示如下错误
  
     ```
     Failed to start Docker Application Container Engine
     ```
  
     此时需要首先确认已正确安装了如下软件包
  
     ```
     device-mapper-event-libs
     ```

     如果确认已经安装了，仍然出现上述错误，则需要执行`yum update`更新系统

为docker设置http代理
-------------------

  1. 创建目录

     ```
     mkdir /etc/systemd/system/docker.service.d
     ```

  2. 创建文件
  
     ```
     /etc/systemd/system/docker.service.d/http-proxy.conf
     ```

  3. 在http-proxy.conf中写入如下内容

     ```
     [Service]
     Environment="HTTP_PROXY=http://username:password@hostname:port/" "NO_PROXY=localhost,127.0.0.1"
     ```

  4. 使用变更生效

     ```
     systemctl daemon-reload
     ```

  5. 确认配置已生效

     ```
     systemctl show docker --property Environment
     ```

  6. 重启docker服务

     ```
     systemctl restart docker.service
     ```

确认docker安装成功
-----------------

```
docker run hello-world
```

2. docker的用户组
================

docker是使用root用户运行的，为了避免输入sudo，docker创建了一个docker用户组，该组内的用户可以直接读写docker的unix socket，也就可以正常执行docker命令了。

  1. 新建用户docker，添加到docker用户组

     ```
     useradd -g docker docker
     ```
  
  2. 将现有用户添加docker用户组

     ```
     usermod -aG docker username
     ```
  
  3. 使用变更后的用户登录系统，确认可以运行`docker`命令

     ```
     docker run hello-world
     ```
  
  4. **Note**: 使用`visudo`命令可以添加sudo用户，实际修改的是/etc/sudoer文件
  
     在以下内容

     ```
     root        ALL=(ALL)       ALL
     ```

     之后添加

     ```
     username    ALL=(ALL)       ALL
     ```


3. 设置docker服务自动启动
=======================

```
systemctl enable docker.service
```

设置docker使用不同的目录或分区，及其他个性化设置可参考

https://docs.docker.com/engine/articles/systemd/

4. 删除docker
=============

  1. 列出docker包的具体名字

     ```
     yum list installed | grep docker
     ```
  
  2. 删除docker

     ```
     yum remove docker-engine.x86_64
     ```

     此处只删除docker的运行环境，并不会删除镜像/容器/卷文件及用户创建的配置文件
  
  3. 清除镜像和容器文件

     ```
     rm -rf /var/lib/docker
     ```
  
  4. 手工查找并删除用户创建的配置文件

5. docker的基本使用
==================

查看版本

```
docker version
```

从 https://index.docker.io/ 检索可用镜像

```
docker search container-name
e.g.: docker search tutorial
```

下载docker镜像

```
docker pull container-name
e.g.: docker pull learn/tutorial
```

在容器中运行hello world

```
docker run container-name command
e.g.: docker run learn/tutorial echo "Hello World"
```

在容器中安装新的程序（以ping程序为例）

上例中下载的tutorial镜像是基于ubuntu的，所以可以使用`apt-get`命令安装ping程序

```
docker run learn/tutorial apt-get install -y ping
```

*apt-get命令带上-y参数，是困为docker环境无法响应命令行的交互模式*

查看安装完ping命令之后的容器id

```
docker ps -l
```

保存对容器的修改（不需要完整的id，前3-4位即可）

```
docker commit container-id container-name
e.g.: docker commit b3cc1d1e4876 learn/ping
```

执行完`docker commit`命令之后，会返回新版本镜像的id号

在新的镜像中运行`ping www.docker.com`命令

```
docker run learn/ping ping www.docker.com
```

查看所有正在运行的容器列表

```
docker ps
```

查看容器详细信息（不需要完整的id，前3-4位即可）

```
docker inspect contianer-id
e.g.: docker inspect 883
```
