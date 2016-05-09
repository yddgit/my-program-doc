#Jenkins 简单使用

1. 安装jenkins

   下载jenkins的RPM包：http://pkg.jenkins-ci.org/redhat/jenkins-1.641-1.1.noarch.rpm

   ```bash
   # 安装jenkins
   rpm -ivh jenkins-1.641-1.1.noarch.rpm
   ```

2. 配置jenkins

   ```bash
   # 查看jenkins服务状态
   systemctl status jenkins
   # 查看jenkins是否开机启动
   systemctl is-enabled jenkins
   # 禁止jenkins开机启动
   systemctl disable jenkins
   # 启动jenkins服务
   systemctl start jenkins
   # 重启jenkins服务
   systemctl restart jenkins
   # 关闭防火墙
   systemctl status firewalld
   # 设置jenkins的contextPath（修改/etc/sysconfig/jenkins的JENKINS_ARGS）
   JENKINS_ARGS="--prefix=/jenkins"
   ```

3. 安装Git插件

   https://wiki.jenkins-ci.org/display/JENKINS/Git+Plugin

   **注意**：要先安装Git插件依赖的其他插件

   下载对应插件的hpi文件，并上传安装即可

4. 配置用户登录

   系统管理-->Configure Global Security

   - 勾选启用安全

     访问控制-->安全域-->Jenkins专有用户数据库（不要勾选“允许用户注册”）

     授权策略-->项目矩阵授权策略-->输入用户名-->添加，然后勾选所有权限（初始创建管理员账号）

     > 安全矩阵和项目矩阵授权策略的配置是一模一样的，唯一的区别是项目矩阵授权策略支持在Job的配置页面再次配置授权策略。

   - 勾选Enable Slave → Master Access Control

   - 保存，会提示创建第一个用户，输入用户名、密码等信息保存即可

   - 后续创建用户方法：系统管理-->管理用户-->新建用户，创建好用户后需要在Configure Global Security里为其赋权限，至少要赋予Overall/Read用户才能登录jenkins

5. 创建简单的任务

   为了可以自动从版本库Checkout代码，需要配置Jenkins的认证信息，以Git为例：

   - 为Jenkins用户生成SSH密钥，使用`ssh-keygen`命令

   - 将Jenkins用户的~/.ssh/id_rsa.pub的内容添加到git用户的~/.ssh/authorized_keys文件中

   - 在Jenkins中添加Credentials：首页-->\[左侧\]Credentials-->\[右侧\]Global credentials-->\[左侧\]Add 

      + Kind：SSH Username with private key
      + Scope：Global
      + Username：jenkins
      + Private Key：From the Jenkins master ~/.ssh
      + 点击OK

   - 如果是使用类似GitBucket的服务器，则可以直接将Jenkins用户的~/.ssh/id_rsa.pub的内容添加到对应GitBucket用户的SSH Keys列表里

   接下开始创建任务，在Jenkins首页点击左侧“新建”，然后输入Item名称，选择“构建一个自由风格的软件项目”，点击OK进入任务配置界面：

   - 输入“描述”

   - 勾选启用项目安全，然后添加相应的用户/组到安全矩阵，并勾选对应的权限，只读权限只需勾选Job/Read即可

   - 源码管理选择Git，输入Repository URL，如：ssh://username@git.hostname/username/HelloJenkins.git，从Credentials里选择之前创建好的认证信息

   - 在“构建触发器”里勾选“Poll SCM”，可以指定（crontab格式）时间来进行定时构建

   - 最后在“构建”部分，配置项目构建的命令，如Ant，可以指定运行特定的target

   - 最后“保存”即可

   创建好之后，可以在首页任务列表中看到新加的任务，点击任务名称，查看任务详情，可点击“立即构建”构建当前任务。
