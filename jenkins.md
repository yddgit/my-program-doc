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

     授权策略-->项目矩阵授权策略-->输入用户名-->添加，然后勾选所有选项（初始创建管理员账号）

     > 安全矩阵和项目矩阵授权策略的配置是一模一样的，唯一的区别是项目矩阵授权策略支持在Job的配置页面再次配置授权策略。

   - 勾选Enable Slave → Master Access Control

   - 保存，会提示创建第一个用户，输入用户名、密码等信息保存即可

   - 后续创建用户方法：系统管理-->管理用户-->新建用户，创建好用户后需要在Configure Global Security里为其赋权限，至少要赋予Overall/Read用户才能登录jenkins

5. 创建简单的任务