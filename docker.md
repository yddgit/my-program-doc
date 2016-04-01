1. docker安装
===================================

Required:
 * Linux, x64, kernel 3.10+
 * 服务器正常连网

使用docker官网的安装脚本安装
--------------------------

 1. 使用命令如下命令查看内核版本：

    ```
    uname -r
    3.10.0-327.3.1.el7.x86_64
    ```
  
 2. 更新系统（可以避免一些已发现的bug）

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

5. docker的简单使用
==================

 1. 查看docker信息（`version`、`info`）

    ```
    # 查看版本
    docker version

    # 显示docker系统的信息
    docker info
    ```

 2. 对image的操作（`search`、`pull`、`images`、`rmi`、`history`）

    ```
    # 从 https://index.docker.io/ 检索可用镜像（e.g.: docker search tutorial）
    docker search image-name

    # 下载docker镜像（docker pull learn/tutorial）
    docker pull image-name

    # 列出镜像列表（-a, --all=false Show all images; --no-trunc=false Don't truncate output; -q, --quiet=false Only show numeric IDs）
    docker images

    # 删除一个或者多个镜像（-f, --force=false Force; --no-prune=false Do not delete untagged parents）
    docker rmi image-name

    # 删除所有镜像
    docker rmi `docker images -q`

    # 显示一个镜像的历史（--no-trunc=false Don't truncate output; -q, --quiet=false Only show numeric IDs）
    docker history image-name
    ```

 3. 启动容器（`run`）

    docker容器可以理解为在沙盒中运行的进程。这个沙盒包含了该进程运行所必须的资源，包括文件系统、系统类库、shell 环境等等。但这个沙盒默认是不会运行任何程序的。你需要在沙盒中运行一个进程来启动某一个容器。这个进程是该容器的唯一进程，所以当该进程结束的时候，容器也会完全的停止。

    ```
    # 在容器中运行"echo"命令，输出"hello word"
    docker run image-name echo "hello word"

    # 交互式进入容器中
    docker run -i -t image-name /bin/bash

    # 在容器中安装新的程序（以ping程序为例：docker run learn/tutorial apt-get install -y ping）
    docker run image-name apt-get install -y app-name
    ```

    *__Note__：在执行`apt-get`命令的时候，要带上`-y`参数。如果不指定`-y`参数的话，`apt-get`命令会进入交互模式，需要用户输入命令来进行确认，但在docker环境中是无法响应这种交互的。`apt-get`命令执行完毕之后，容器就会停止，但对容器的改动不会丢失。*

 4. 查看容器（`ps`、`inspect`）

    ```
    # 列出当前所有正在运行的container
    docker ps

    # 列出所有的container
    docker ps -a

    # 列出最近一次启动的container
    docker ps -l

    # 列出所有容器的ID
    docker ps -a -q

    # 列出指定容器的详细信息
    docker inspect contianer-name/container-id
    ```

 5. 保存对容器的修改（`commit`）

    当你对某一个容器做了修改之后（通过在容器中运行某一个命令），可以把对容器的修改保存下来，这样下次可以从保存后的最新状态运行该容器。

    ```
    # 保存对容器的修改（-a, --author="" Author; -m, --message="" Commit message）
    # 以上例中安装了ping程序的容器为例：docker commit b3cc1d1e4876 learn/ping
    # container-id不需要完整的id，前3-4位即可
    docker commit container-id new-image-name

    # 在新的镜像中运行ping www.docker.com命令
    docker run learn/ping ping www.docker.com
    ```

    *__Note__：image相当于类，container相当于实例，不过可以动态给实例安装新软件，然后把这个container用commit命令固化成一个image。上述执行完`docker commit`命令之后，会返回新版本镜像的id号。*

 6. 对容器的操作（`rm`、`stop`、`start`、`kill`、`logs`、`diff`、`top`、`cp`、`restart`、`attach`）

    ```
    # 删除所有容器
    docker rm `docker ps -a -q`
    
    # 删除单个容器（-f, --force=false; -l, --link=false Remove the specified link and not the underlying container; -v, --volumes=false Remove the volumes associated to the container）
    docker rm container-name/container-id
    
    # 停止、启动、杀死一个容器
    docker stop container-name/container-id
    docker start container-name/container-id
    docker kill container-name/container-id
    
    # 从一个容器中取日志（-f, --follow=false Follow log output; -t, --timestamps=false Show timestamps）
    docker logs container-name/container-id
    
    # 列出一个容器里面被改变的文件或者目录，list列表会显示出三种事件，A增加的，D删除的，C被改变的
    docker diff container-name/container-id
    
    # 显示一个运行的容器里面的进程信息
    docker top container-name/container-id
    
    # 从容器里面拷贝文件/目录到本地一个路径
    docker cp container-name:/container-path to-path
    docker cp container-id:/container-path to-path
    
    # 重启一个正在运行的容器（-t, --time=10 Number of seconds to try to stop for before killing the container, Default=10）
    docker restart container-name/container-id
    
    # 附加到一个运行的容器上面（--no-stdin=false Do not attach stdin; --sig-proxy=true Proxify all received signal to the process）
    docker attach container-name/container-id
    ```

    *__Note__：使用`attach`命令有时候并不方便。当多个窗口同时`attach`到同一个容器的时候，所有窗口都会同步显示。当某个窗口因命令阻塞时,其他窗口也无法执行操作了。*

 7. 保存和加载镜像（`save`、`load`）

    当需要把一台机器上的镜像迁移到另一台机器的时候，需要保存镜像与加载镜像。

    ```
    # 保存镜像到一个tar包（-o, --output="" Write to an file）
    docker save image-name -o file-path
    
    # 加载一个tar包格式的镜像（-i, --input="" Read from a tar archive file）
    docker load -i file-path
    
    # 主机A
    docker save image-name > /home/image1.tar
    
    # 使用scp将image1.tar拷到主机B上，然后：
    docker load < /home/image1.tar
    ```

 8. 登录registry server（`login`）

    ```
    # 登陆registry server（-e, --email="" Email; -p, --password="" Password; -u, --username="" Username）
    docker login
    ```

 9. 发布image（`push`）

    ```
    # 发布docker镜像
    docker push new-image-name
    ```

 10. 根据Dockerfile 构建出一个容器

    ```
    # build  
    #   --no-cache=false Do not use cache when building the image  
    #   -q, --quiet=false Suppress the verbose output generated by the containers  
    #   --rm=true Remove intermediate containers after a successful build  
    #   -t, --tag="" Repository name (and optionally a tag) to be applied to the resulting image in case of success  
    docker build -t image-name dockerfile-path
    ```

6. docker的基本功能介绍
======================

  1. 以交互式命令行运行容器

    ```
    docker run -i -t container-name /bin/bash
    ```