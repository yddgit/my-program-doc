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
> 
> 本文档中指定下载的`Redmine Installer`中包含如下组件：
> - Redmine 3.2.2
> - Apache 2.4.20
> - ImageMagick 6.7.5
> - MySQL 5.6.30
> - Subversion 1.8.13
> - Git 2.7.4
> - Ruby 2.1.9
> - Rails 4.2.5.1
> - RubyGems 1.8.12


一. Redmine服务的安装
--------------------

1. 将下载好的Redmine安装包（`bitnami-redmine-3.2.2-0-linux-x64-installer.run`）上传到服务器

2. 执行如下命令为安装包赋予可执行权限：

   ```bash
   chmod u+x bitnami-redmine-3.2.2-0-linux-x64-installer.run
   ```

3. 执行如下命令开始安装：

   ```bash
   ./bitnami-redmine-3.2.2-0-linux-x64-installer.run --mode text
   ```

4. 接下来根据提示，进行相关设置，直至安装完成

   - 选择安装语言：5（简体中文）

     ```
     Language Selection

     Please select the installation language
     [1] English - English
     [2] Spanish - Español
     [3] Japanese - 日本語
     [4] Korean - 한국어
     [5] Simplified Chinese - 简体中文
     [6] Hebrew - עברית
     [7] German - Deutsch
     [8] Romanian - Română
     [9] Russian - Русский
     Please choose an option [1] : 
     ```

   - 选择要安装的组件：全部选**Y**

     ```
     ----------------------------------------------------------------------------
     欢迎来到 Bitnami Redmine Stack 安装程序。

     ----------------------------------------------------------------------------
     选择您想要安装的组件，清除您不想安装的组件。当您准备继续时，点击前进。

     Subversion [Y/n] :Y

     PhpMyAdmin [Y/n] :Y

     Redmine : Y (Cannot be edited)

     Git [Y/n] :Y

     上述选择是否正确？ [Y/n]: Y
     ```

   - 选择安装目录：/opt/redmine-3.2.2-0（默认即可）

     ```
     ----------------------------------------------------------------------------
     安装文件夹

     请选择安装Bitnami Redmine Stack的文件夹

     选择一个文件夹 [/opt/redmine-3.2.2-0]:
     ```

   - 设置管理员账号密码

     ```
     ----------------------------------------------------------------------------
     创建管理员帐户

     Bitnami Redmine Stack admin 用户创建

     您的真实姓名 [User Name]: admin

     Email地址 [user@example.com]: admin@admin.com

     登录 [user]: admin

     密码 :
     请确认密码 :
     ```

   - 选择缺省数据配置语言：30（中文）

     ```
     ----------------------------------------------------------------------------
     缺省数据配置语言

     选择缺省数据配置语言：

     [1] Bosnian
     [2] 保加利亚语
     [3] Catalan
     [4] 捷克语
     [5] Danish
     [6] 德语
     [7] 英语
     [8] 西班牙
     [9] 法语
     [10] Galician
     [11] 希伯来语
     [12] Hungarian
     [13] 意大利语
     [14] 日语
     [15] 朝鲜语
     [16] Lithuanian
     [17] 荷兰语
     [18] Norwegian
     [19] 波兰语
     [20] 葡萄牙语
     [21] 罗马尼亚语
     [22] 俄语
     [23] Slovak
     [24] Slovenian
     [25] 塞尔维亚语
     [26] 瑞典语
     [27] Turkish
     [28] Ukrainian
     [29] Vietnamese
     [30] 中文
     请选择选项 [30] : 30
     ```

   - 邮件配置：N（暂不设置）

     ```
     Do you want to configure mail support? [y/N]: N
     ```

   - 确认开始安装：Y（确认）

     ```
     ----------------------------------------------------------------------------
     安装程序已经准备好将 Bitnami Redmine Stack 安装到您的电脑。

     您确定要继续吗？ [Y/n]: Y

     ----------------------------------------------------------------------------
     正在安装 Bitnami Redmine Stack 至您的电脑中，请稍候。

     正在安装
     0% ______________ 50% ______________ 100%
     #########################
     ```

   - 稍等片刻......安装完成，暂不启动Redmine相关服务

     ```
     ----------------------------------------------------------------------------
     安装程序已经将 Bitnami Redmine Stack 安装于您的电脑中。

     启动Redmine应用程序。 [Y/n]: n

     信息: To access the Bitnami Redmine Stack, go to
     http://127.0.0.1:80 from your browser.
     按 [Enter] 继续：
     ```

5. 配置phpMyAdmin远程访问

   ```apache
   # 修改/opt/redmine-3.2.2-0/apps/phpmyadmin/conf/httpd-app.conf，注意Allow from all和Require all granted的配置
   <IfVersion < 2.3 >
       Order allow,deny
       Allow from all
       Satisfy all
   </IfVersion>
   <IfVersion >= 2.3>
       Require all granted
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
   # 重新load后台服务
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
   # 在/opt/redmine-3.2.2-0/svnrepo目录下创建一个svn的repository
   /opt/redmine-3.2.2-0/subversion/bin/svnadmin create /opt/redmine-3.2.2-0/svnrepo
   # 创建指向/opt/redmine-3.2.2-0/svnrepo的符号链接/svn，方便通过svn://ip.address/svn访问svn仓库
   ln -s /opt/redmine-3.2.2-0/svnrepo /svn
   # 重启subversion服务
   /opt/redmine-3.2.2-0/ctlscript.sh restart subversion
   # 此时可以通过svn://ip.address/svn访问刚刚创建的svn仓库
   ```

2. 配置svn的http访问（使用Apache HTTP Server）

   - 为svn仓库创建用户名和密码文件

     ```bash
     # 创建管理员账号
     /opt/redmine-3.2.2-0/apache2/bin/htpasswd -bcm /opt/redmine-3.2.2-0/svnrepo/conf/http-passwd admin admin-password
     # 创建其他用户账号（注意没有-c参数）
     /opt/redmine-3.2.2-0/apache2/bin/htpasswd -bm /opt/redmine-3.2.2-0/svnrepo/conf/http-passwd yang yang-password
     /opt/redmine-3.2.2-0/apache2/bin/htpasswd -bm /opt/redmine-3.2.2-0/svnrepo/conf/http-passwd user user-password
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
     # 创建svn的apache配置文件
     vi /opt/redmine-3.2.2-0/apache2/conf/bitnami/subversion.conf
     ```

     **subversion.conf**
     ```apache
     <Location /svn>
         DAV svn
         SVNPath /opt/redmine-3.2.2-0/svnrepo
         AuthType Basic
         AuthName "Subversion Repository"
         AuthUserFile /opt/redmine-3.2.2-0/svnrepo/conf/http-passwd
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

   - 配置svn通过http提交

     现在可以通过http方式访问svn服务：http://ip.address/svn

     但仍然不能通过http提交文件到svn仓库，可通过如下命令允许http提交

     ```bash
     chown -R daemon:subversion /opt/redmine-3.2.2-0/svnrepo
     chown -R daemon:subversion /svn
     ```

三. GitBucket安装
-----------------

1. 安装GitBucket服务

   - 安装Tomcat

     ```bash
     yum install tomcat tomcatjss tomcat-webapps tomcat-javadoc tomcat-docs-webapp tomcat-admin-webapps
     ```

   - 配置GitBucket数据目录

     ```bash
     # 创建/opt/redmine-3.2.2-0/gitbucket目录，并修改目录权限
     mkdir /opt/redmine-3.2.2-0/gitbucket
     chown -R tomcat:tomcat /opt/redmine-3.2.2-0/gitbucket
     # 编辑tomcat配置文件，添加环境变量GITBUCKET_HOME
     vi /etc/tomcat/tomcat.conf
     # 在tomcat.conf文件最后追加如下内容
     GITBUCKET_HOME="/opt/redmine-3.2.2-0/gitbucket"
     ```

   - 将下载好的gitbucket.war上传到如下目录

     ```bash
     /var/lib/tomcat/webapps/
     ```

     ```bash
     # 修改文件权限
     chown tomcat:tomcat /var/lib/tomcat/webapps/gitbucket.war
     ```

   - 启动tomcat服务

     ```bash
     # 重新load后台服务
     systemctl daemon-reload
     # 查看tomcat服务状态
     systemctl status tomcat
     # 启动tomcat服务
     systemctl start tomcat
     ```

2. 为GitBucket配置反向代理

   ```bash
   # 创建gitbucket的apache配置文件
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

3. 使用管理员登录GitBucket，设置GitBucket

   GitBucket主页：http://ip.address/gitbucket

   账号/密码：`root/root`

   登录成功后，访问：http://ip.address/gitbucket/admin/system

   - Base URL：http://ip.address/gitbucket
   - Default option to create a new repository：**Private**
   - Anonymous access：**Deny**
   - 去选：Use Gravatar for Profile-Images
   - 勾选：Enable SSH access to git repository
     + SSH Host：ip.address
     + SSH Port：29418
   - Apply changes

四. 配置Redmine
------------------------------------------

1. 修改Apache主页

   ```bash
   # 修改/opt/redmine-3.2.2-0/apache2/htdocs/index.html
   vi /opt/redmine-3.2.2-0/apache2/htdocs/index.html
   ```

   在index.html的22行和23行之间，添加如下内容
   ```html
   <h1><a id="bitnami-link" href="phpmyadmin/?lang=en" target="_blank">Access phpMyAdmin</a></h1>
   <h1><a id="bitnami-link" href="svn/" target="_blank">Access Subversion</a></h1>
   <h1><a id="bitnami-link" href="gitbucket/" target="_blank">Access GitBucket</a></h1>
   ```

   以下是各个服务的URL和账号密码：

   - Redmine
     + URL：http://ip.address/redmine/
     + 账号/密码：安装Redmine时配置的用户名和密码，如：admin/xxxxx
   - phpMyAdmin
     + URL：http://ip.address/phpmyadmin/
     + 账号/密码：root/安装Redmine时配置的密码
     + **!! 建议登录phpMyAdmin时语言选择English，这个版本的phpMyAdmin对中文的支持不是很好**
   - Subversion
     + URL：http://ip.address/svn/
     + 账号/密码：配置svn的http访问时配置的用户名和密码，如：admin/xxxxx
   - GitBucket
     + URL：http://ip.address/gitbucket/
     + 账号/密码：root/root

2. 修改Redmine配置

   - 更换主题

     将Redmine默认的主题换成Gitmike主题，下载地址：

     https://github.com/makotokw/redmine-theme-gitmike

     将下载好的主题文件上传到服务器：

     ```bash
     # 将下载好的文件redmine-theme-gitmike-1.0.7.zip上传到如下目录
     /opt/redmine-3.2.2-0/apps/redmine/htdocs/public/themes
     # 解压
     unzip redmine-theme-gitmike-1.0.7.zip
     # 修改目录名
     mv redmine-theme-gitmike-1.0.7 gitmike
     ```

   - 用管理员登录Redmine，点击 **管理** --> **配置**，修改如下配置：
     + **一般** --> **主机名称**：ip.address/redmine
     + **一般** --> **文本格式**：Markdown
     + **保存**
     + **显示** --> **主题**：Gitmike
     + **显示** --> **默认语言**：简体中文
     + **显示** --> **用户显示格式**：_选择姓在前名在后的格式_

五. 在Redmine中创建项目并配置Subversion和Git
------------------------------------------



六. Redmine的邮件配置
--------------------



七、配置防火墙
-------------

```bash
# 添加Apache的80端口
firewall-cmd --zone=public --add-port=80/tcp --permanent
# 添加GitBucket的29418端口用于SSH方式访问Git仓库
firewall-cmd --zone=public --add-port=29418/tcp --permanent
# 动态更新防火墙规则
firewall-cmd --reload
```