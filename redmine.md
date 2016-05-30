基于Redmine的项目管理平台搭建
===========================

**软件环境：**

| Name         | Version         | Download URL                                 |
|--------------|-----------------|----------------------------------------------|
| OS           | CentOS 7 x86_64 | [CentOS-7-x86_64-DVD-1511.iso][1]            |
| Redmine      | 3.2.2           | [Redmine 3.2.2-0 (64-bit) for Linux][2]      |
| Subversion   | 1.8.13          | *Have already packaged in Redmine Installer* |
| GitBucket    | 3.14            | [GitBucket 3.14][3]                          |

[1]: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1511.iso "CentOS-7-x86_64-DVD-1511.iso"
[2]: https://bitnami.com/stack/redmine/installer "Redmine 3.2.2-0 (64-bit) for Linux"
[3]: https://github.com/gitbucket/gitbucket/releases/tag/3.14 "GitBucket 3.14"

> **注意**：Redmine使用的是Bitnami提供的一站式安装包，无须关心其环境和依赖包的安装配置。Bitnami提供了很多常用服务的安装包，具体可参考这里：https://bitnami.com/stacks

一. Redmine服务的安装
--------------------

1. 将下载好的Redmine安装包（`bitnami-redmine-3.2.2-0-linux-x64-installer.run`）上传到服务器

2. 用root用户登录，执行如命令为安装包赋予可执行权限：

   ```bash
   chmod 755 bitnami-redmine-3.2.2-0-linux-installer.run
   ```

3. 执行如下命令开始安装：

   ```bash
   ./bitnami-redmine-3.2.2-0-linux-installer.run --mode text
   ```

4. 接下来根据提示，输入相应信息，一路Next，直至安装完成

5. 配置phpMyAdmin远程访问

   ```apache
   # 修改/opt/redmine-3.2.2-0/apps/phpmyadmin/conf/httpd-app.conf，注意[]里的配置
   <IfVersion < 2.3 >
       Order allow,deny
       Allow from [all]
       Satisfy all
   </IfVersion>
   <IfVersion >= 2.3>
       Require [all granted]
   </IfVersion>
   ```

6. 将redmine添加到系统服务

   ```bash
   # 创建/usr/lib/systemd/system/redmine.service
   vi /usr/lib/systemd/system/redmine.service
   ```

   **redmine.service**
   ```
   # Systemd unit file for Redmine
   [Unit]
   Description=Redmine A Flexible Project Management Web Application
   After=syslog.target network.target remote-fs.target nss-lookup.target

   [Service]
   Type=forking

   ExecStart=/opt/redmine-3.2.2-0/ctlscript.sh start
   ExecStop=/opt/redmine-3.2.2-0/ctlscript.sh stop

   User=root
   Group=root

   [Install]
   WantedBy=multi-user.target
   ```

   ```bash
   # 重新load服务列表
   systemctl daemon-reload
   # 查看redmine服务状态
   systemctl status redmine
   # 启动redmine服务
   systemctl start redmine
   ```

二. Subversion服务的配置
-----------------------

1. 使用如下命令创建svn仓库

   ```bash
   # 在/svn目录下创建一个svn的repository,之所以/svn下创建，是为了方便通过svn://ip.address/svn访问svn仓库
   /opt/redmine-3.2.2-0/subversion/bin/svnadmin create /opt/redmine-3.2.2-0/svnrepo
   # 创建指向/opt/redmine-3.2.2-0/svnrepo的符号链接
   ln -s /opt/redmine-3.2.2-0/svnrepo /svn
   # 重启subversion服务
   /opt/redmine-3.2.2-0/ctlscript.sh restart subversion
   # 此时可以通过svn://ip.address/svn访问刚刚创建的svn仓库
   ```

2. 配置svn仓库的http访问（使用Apache HTTP Server）

   - 为svn仓库创建用户名和密码文件

     ```bash
     # 创建管理员账号
     /opt/redmine-3.2.2-0/apache2/bin/htpasswd -bcm /opt/redmine-3.2.2-0/svnrepo/conf/passwd admin *****
     # 创建其他用户账号（注意没有-c参数）
     /opt/redmine-3.2.2-0/apache2/bin/htpasswd -bm /opt/redmine-3.2.2-0/svnrepo/conf/passwd yang ****
     /opt/redmine-3.2.2-0/apache2/bin/htpasswd -bm /opt/redmine-3.2.2-0/svnrepo/conf/passwd user ****
     ```

   - 配置用户权限

     ```bash
     # 修改/opt/redmine-3.2.2-0/svnrepo/conf/authz
     vi /opt/redmine-3.2.2-0/svnrepo/conf/authz
     ```

     **authz**
     ```
     [groups]
     manager = yang,admin
     users = user
     [/]
     admin = rw
     yang = rw
     user = r
     ```

   - 创建apache配置文件

     ```bash
     # 创建svn仓库的apache配置文件
     vi /opt/redmine-3.2.2-0/apache2/conf/bitnami/subversion.conf
     ```

     **subversion.conf**
     ```apache
     <Location /svn>
         DAV svn
         SVNPath /opt/redmine-3.2.2-0/svnrepo
         AuthType Basic
         AuthName "SVN Repo"
         AuthUserFile /opt/redmine-3.2.2-0/svnrepo/conf/passwd
         AuthzSVNAccessFile /opt/redmine-3.2.2-0/svnrepo/conf/authz
         Require valid-user
     </Location>
     ```

   - 使apache配置生效

     ```bash
     # 编辑/opt/redmine-3.2.2-0/apache2/conf/bitnami/bitnami.conf
     vi /opt/redmine-3.2.2-0/apache2/conf/bitnami/bitnami.conf
     ```

     ```apache
     # 追加如下内容：
     Include "/opt/redmine-3.2.2-0/apache2/conf/bitnami/subversion.conf"
     ```

     ```apache
     # 重启apache
     /opt/redmine-3.2.2-0/ctlscript.sh restart apache
     ```

   - 访问svn并通过http提交

     现在可以通过http方式访问svn服务：http://ip.address/svn

     但仍然不能通过http提交文件到svn仓库，可通过如下命令允许http提交

     ```bash
     chown -R daemon:subversion /opt/redmine-3.2.2-0/svnrepo
     ```

三. GitBucket安装
-----------------

1. 安装GitBucket服务

2. 为GitBucket配置反向代理

   ```bash
   # 创建gitbucket仓库的apache配置文件
   vi /opt/redmine-3.2.2-0/apache2/conf/bitnami/gitbucket.conf
   ```

   **gitbucket.conf**
   ```apache
   ProxyPreserveHost On
   ProxyPass /gitbucket http://localhost:8080/gitbucket
   ProxyPassReverse /gitbucket http://localhost:8080/gitbucket
   ```

   使apache配置生效

   ```bash
   # 编辑/opt/redmine-3.2.2-0/apache2/conf/bitnami/bitnami.conf
   vi /opt/redmine-3.2.2-0/apache2/conf/bitnami/bitnami.conf
   ```

   ```apache
   # 追加如下内容：
   Include "/opt/redmine-3.2.2-0/apache2/conf/bitnami/gitbucket.conf"
   ```

   ```apache
   # 重启apache
   /opt/redmine-3.2.2-0/ctlscript.sh restart apache
   ```

3. 使用管理员root（密码：root）登录GitBucket，设置GitBucket的URL

   http://ip.address/gitbucket

四. 在Redmine中创建项目并配置Subversion和Git
------------------------------------------



五. Redmine的邮件配置
--------------------

