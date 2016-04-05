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
    yum list installed | grep device-mapper-event-libs
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

 4. 使变更生效

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
  
 4. **Note**: 使用`visudo`命令可以将docker用户添加到sudo用户列表，实际修改的是/etc/sudoer文件
  
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

    Dockerfile中每一条指令都创建镜像的一层

    * 使用#来注释
    * FROM指令告诉Docker使用哪个镜像作为基础
    * 接着是维护者的信息
    * RUN开头的指令会在创建中运行，比如安装一个软件包
    * __一个镜像不能超过127层__
    * 使用ADD命令复制本地文件到镜像
    * 使用EXPOSE命令来向外部开放端口
    * 使用CMD命令来描述容器启动后运行的程序等

    示例Dockerfile的内容如下：

    ```
    # This is a comment
    FROM learn/ping
    # Maintainer info 
    MAINTAINER Dong Yang <yang@yang.com>
    # The command to run when create image
    #RUN apt-get install apache2
    # Expose httpd port
    EXPOSE 80
    # The command to run
    CMD ["echo", "Hello Docker"]
    ```

    使用如下命令从Dockerfile创建镜像

    ```
    # build  
    #   --no-cache=false Do not use cache when building the image  
    #   -q, --quiet=false Suppress the verbose output generated by the containers  
    #   --rm=true Remove intermediate containers after a successful build  
    #   -t, --tag="" Repository name (and optionally a tag) to be applied to the resulting image in case of success  
    docker build -t image-name dockerfile-path
    ```

    基于上例中的Dockerfile，执行`docker build -t="learn/apache" ~/apache-docker/`，输出如下内容

    ```
    Sending build context to Docker daemon 2.048 kB
    Step 1 : FROM learn/ping
     ---> c215e0e4d5a4
    Step 2 : MAINTAINER Dong Yang <yang@yang.com>
     ---> Running in bb9903ddb52b
     ---> 1bc4a0979dba
    Removing intermediate container bb9903ddb52b
    Step 3 : EXPOSE 80
     ---> Running in 923397a5ef49
     ---> 399328965f10
    Removing intermediate container 923397a5ef49
    Step 4 : CMD echo Hello Docker
     ---> Running in b50747b9b829
     ---> 3f39a12ea7b9
    Removing intermediate container b50747b9b829
    Successfully built 3f39a12ea7b9
    ```

    `docker images`命令的输出会多出一条记录

    ```
    REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
    learn/apache        latest              3f39a12ea7b9        4 minutes ago       139.5 MB
    ```

 11. 以守护态运行docker容器

    通过添加-d参数来实现

    ```
    docker run -d learn/apache /bin/sh -c "while true; do echo hello world; sleep 1; done"
    ```

    容器启动后会返回一个唯一的容器id，要获得容器的输出信息，可以使用`docker logs`命令

 12. 将主机的某个目录挂载到容器的指定目录下

    在运行容器时使用`-v`来声明Volume，`--name`指定容器名称（如不指定，则会自动生成一个随机数字符串当做UUID），`-h`设定hostname

    ```
    docker run -it --name container-name -h CONTAINER_HOST -v /data image-name /bin/bash
    ```

    上面的命令会将`/data`挂载到容器中，并绕过联合文件系统（Union File System），我们可以在主机上直接操作该目录。任何在该镜像`/data`路径的文件会将被复制到Volume。我们可以使用`docker inspect`命令找到Volume在主机上的存储位置

    ```
    docker inspect container-name
    ```

    *上述命令执行的输出结果中会包含类似如下内容的输出，这说明Docker把在`/var/lib/docker/volumes`下的某个目录挂载到了容器内的`/data`目录下。__注意：如果容器的`/data`目录一开始就不为空，那此时，`/data`目录下的文件在`/var/lib/docker/volumes`下也同样可以看到。__*

    ```    
    "Mounts": [
        {
            "Name": "83d47e38db88062886ccfce0f50a94683017ea70ec7462420fc2b366a5aed547",
            "Source": "/var/lib/docker/volumes/83d47e38db88062886ccfce0f50a94683017ea70ec7462420fc2b366a5aed547/_data",
            "Destination": "/data",
            "Driver": "local",
            "Mode": "",
            "RW": true,
            "Propagation": ""
        }
    ],
    ```

    **也可以在Dockerfile中通过使用VOLUME指令来达到相同的目的**

    ```
    VOLUME /data
    ```

    但还有另一件只有`-v`参数能做到而Dockerfile是做不到的事是在容器上挂载指定的主机目录

    ```
    docker run -it --name container-name -h CONTAINER_HOST -v /home/docker/data:/data image-name /bin/bash
    ```

    *该命令将挂载主机的`/home/docker/data`目录到容器内的`/data`目录上。任何在`/home/docker/data`目录的文件都将会出现在容器内。__注意：如果容器的`/data`目录一开始就不为空，那此时，`/data`目录下的文件仍显示为宿主机的`/home/docker/data`目录下的文件。__*

    更多关于Volume的内容参考：http://www.tuicool.com/articles/uYzeAnz

6. Dockerfile的语法
===================

Docker为我们提供了Dockerfile来解决自动化的问题。Dockerfile包含创建镜像所需要的全部指令。基于在Dockerfile中的指令，我们可以使用Docker build命令来创建镜像。通过减少镜像和容器的创建过程来简化部署。

Dockerfile支持支持的语法命令

```
INSTRUCTION argument
```

*指令不区分大小写。但是命名约定为全部大写。*

所有Dockerfile都必须以FROM命令开始。FROM命令会指定镜像基于哪个基础镜像创建，接下来的命令也会基于这个基础镜像。FROM命令可以多次使用，表示会创建多个镜像。如：`FROM ubuntu` 指令告诉我们，新的镜像将基于Ubuntu的镜像来构建。

```
FROM image-name
```

紧接着FROM命令，Dockerfile还提供了一些其它的命令以实现自动化。在文本文件或Dockerfile文件中这些命令的顺序就是它们被执行的顺序。

 1. `MAINTAINER`设置该镜像的作者。

    ```
    MAINTAINER author-name
    ```

 2. `RUN`在shell或者exec的环境下执行的命令。`RUN`指令会在新创建的镜像上添加新的层面，接下来提交的结果用在Dockerfile的下一条指令中。

    ```
    RUN command
    ```

 3. `ADD`复制文件指令。它有两个参数`source`和`destination`。`destination`是容器内的路径。`source`可以是URL或者是启动配置上下文中的一个文件。

    ```
    ADD src destination
    ```

 4. `CMD`提供了容器默认的执行命令。Dockerfile只允许使用一次`CMD`指令。使用多个`CMD`会抵消之前所有的指令，只有最后一个指令生效。

    ```
    CMD ["executable","param1","param2"]
    CMD ["param1","param2"]
    CMD command param1 param2
    ```

 5. `EXPOSE`指定容器在运行时监听的端口。

    ```
    EXPOSE port
    ```

 6. `ENTRYPOINT`配置给容器一个可执行的命令，这意味着在每次使用镜像创建容器时一个特定的应用程序可以被设置为默认程序。同时也意味着该镜像每次被调用时仅能运行指定的应用。类似于`CMD`，Docker只允许一个`ENTRYPOINT`，多个`ENTRYPOINT`会抵消之前所有的指令，只执行最后的`ENTRYPOINT`指令。

    ```
    ENTRYPOINT ["executable", "param1","param2"]
    ENTRYPOINT command param1 param2
    ```

    *如果设置了`ENTRYPOINT`，则`CMD`将作为参数*

 7. `WORKDIR`指定`RUN`、`CMD`与`ENTRYPOINT`命令的工作目录。

    ```
    WORKDIR /path/to/workdir
    ```

 8. `ENV`设置环境变量。它们使用键值对，增加运行程序的灵活性。

    ```
    ENV key value
    ```

 9. `USER`镜像正在运行时设置一个UID。

    ```
    USER uid
    ```

 10. `VOLUME`授权访问从容器内到主机上的目录。

    ```
    VOLUME ["/data"]
    ```

Dockerfile最佳实践
-----------------

参考：http://crosbymichael.com/dockerfile-best-practices.html

 * 保持常见的指令像`MAINTAINER`以及从上至下更新Dockerfile命令；
 * 当构建镜像时使用可理解的标签，以便更好地管理镜像；
 * 避免在Dockerfile中映射公有端口；
 * `CMD`与`ENTRYPOINT`命令请使用数组语法。

7. Docker搭建本地私有registry
============================

