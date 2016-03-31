CentOS 7 New Features
=====================

1. `ip ss <--> ifconfig route arp netstat`

   查看网络接口地址：`ip addr`

   网络接口统计信息：`ip -s link`

   显示路由表：`ip route`（格式化显示：`ip route|column -t`）

   添加静态路由：`ip route add 10.15.150.0/24 via 192.168.250.253 dev enp0s3`

   删除静态路由：`ip route del 10.15.150.0/24`

   以上静态路由设置不能保存，如需永久保存设置，需要修改`/etc/sysconfig/network-scripts/route-enp0s3`

2. `ip neighbor <--> arp -n`

3. `ss <--> netstat`

   ```
   ss -ant
   ss -antp
   ss -antp|column -t
   ```

4. 网络配置文件:

   ```
   /etc/sysconfig/network（全局设置，默认为空）
   /etc/hostname（用nmtui修改hostname后，主机名保存在这里）
   /etc/resolv.conf（保存DNS设置，不需要手工改，nmtui里面设置的DNS会出现在这里）
   /etc/sysconfig/network-scripts/（连接配置信息 ifcfg 文件）
   /etc/NetworkManager/system-connections/（VPN、移动宽带、PPPoE连接）
   ```

5. `nmtui`（通过文本用户界面管理网络）

   编辑连接、启用/禁用连接、更改主机名，`nmtui help`查看帮助

6. `nmcli`和其他网络设置

   device叫网络接口，是物理设备，connection是连接，偏重于逻辑设置，多个connection可以应用到同一个device，但同一时间只能启用其中一个connection。

   这样的好处是针对一个网络接口，我们可以设置多个网络连接，比如静态IP和动态IP，再根据需要up相应的connection。

   查看连接：`nmcli con show`

   查看设置：`nmcli dev show enp0s3`

   接口信息:`nmcli -t -f IP4.ADDRESS dev show enp0s3`（以脚本友好的方式显示网络接口信息）

   增加连接:`nmcli con add con-name eth1-1 ifname enp0s8 type ethernet ip4 10.0.3.120/24 gw4 10.0.3.2`

   启动连接：`nmcli con up eth1-1`

   无论使用nmtui编辑连接或是直接修改ifcfg文件，都需要重新加载配置文件并重新up连接

   ```
   nmcli con reload 或 nmcli con load /etc/sysconfig/network-scripts/ifcfg-eth1-1
   nmcli con up eth1-1
   ```

7. `systemctl`

   使用systemctl来启动/停止/重启服务

   ```
   systemctl start httpd.service
   systemctl stop httpd.service
   systemctl restart httpd.service
   systemctl try-restart httpd.service
   ```

   重新加载配置文件

   ```
   systemctl reload httpd.service
   ```

   检查服务状态

   ```
   systemctl status httpd.service
   ```

   使用启用/禁用服务来控制开机启动

   ```
   systemctl enable httpd.service
   systemctl disable httpd.service
   systemctl is-enabled httpd.service（检查单元是否配置为自动启动）
   ```

   输出激活的单元

   ```
   systemctl
   systemctl list-units
   ```

   查看所有被激活的服务

   ```
   systemctl list-units -t service
   ```

   输出运行失败的单元

   ```
   systemctl --failed
   ```

   查看所有已安装服务

   ```
   systemctl list-unit-files
   ```

   所有可用的单元文件存放在`/usr/lib/systemd/system/`和`/etc/systemd/system/`目录（后者优先级更高）

   重新载入systemd，扫描新的或有变动的单元

   ```
   systemctl daemon-reload
   ```

   电源管理（安装polkit后才可使用电源管理）

   重启：`systemctl reboot`

   退出系统并停止电源：`systemctl poweroff`

   待机：`systemctl suspend`

   休眠：`systemctl hibernate`

   混合休眠模式（同时休眠到硬盘并待机）：`systemctl hybrid-sleep`

   列出所有目标

   ```
   systemctl list-units --type=target
   ```

   修改当前目标

   ```
   systemctl isolate graphical.target
   ```

   列出默认目标

   ```
   systemctl get-default
   ```

   改变默认目标

   ```
   systemctl set-default graphical.target
   ```

   读取日志

   ```
   journalctl
   ```

   查看所有引导日志

   ```
   journalctl -b
   ```

   即时显示引导日志

   ```
   journalctl -f
   ```

   查看特定服务的日志

   ```
   journalctl /usr/sbin/dnsmasq
   ```

   查看主机名及其他信息

   ```
   hostnamectl
   ```

   查看系统引导用时

   ```
   systemd-analyze
   systemd-analyze time
   ```

   以进程初始化所占用时间排序打印出所有正在运行的单元列表

   ```
   systemd-analyze blame
   ```

8. 防火墙firewalld

   查看版本：`firewall-cmd --version`

   查看帮助：`firewall-cmd --help`

   显示状态：`firewall-cmd --state`

   查看区域信息：`firewall-cmd --get-active-zones`

   查看指定接口所属区域：`firewall-cmd --get-zone-of-interface=eth0`

   更新防火墙规则：

   ```
   firewall-cmd --reload （动态添加规则）
   firewall-cmd --complete-reload （需要断开连接）
   ```

   将接口添加到区域，默认接口都在public

   ```
   firewall-cmd --zone=public --add-interface=eth0（永久生效再加上--permanent然后reload防火墙）
   ```

   设置默认接口区域

   ```
   firewall-cmd --set-default-zone=public（立即生效无需重启）
   ```

   打开端口，查看所有打开的端口：

   ```
   firewall-cmd --zone=dmz --list-ports
   ```

   加入一个端口到区域：

   ```
   firewall-cmd --zone=dmz --add-port=8080/tcp（若要永久生效再加上--permanent然后reload防火墙）
   ```

   打开一个服务，类似于将端口可视化，服务需要在配置文件中添加，/etc/firewalld目录下有services文件夹

   ```
   firewall-cmd --zone=work --add-service=smtp
   ```

   移除服务

   ```
   firewall-cmd --zone=work --remove-service=smtp
   ```

*****

Systemd和SysVinit的比较
======================

1. Service Related Commands
---------------------------

| Comments                                          | SysVinit                  | Systemd                             | 
|---------------------------------------------------|---------------------------|-------------------------------------|
| Start a service                                   | service dummy start       | systemctl start dummy.service       |
| Stop a service                                    | service dummy stop        | systemctl stop dummy.service        |
| Restart a service                                 | service dummy restart     | systemctl restart dummy.service     |
| Reload a service                                  | service dummy reload      | systemctl reload dummy.service      |
| Service status                                    | service dummy status      | systemctl status dummy.service      |
| Restart a service if already running              | service dummy condrestart | systemctl condrestart dummy.service |
| Enable service at startup                         | chkconfig dummy on        | systemctl enable dummy.service      |
| Disable service at startup                        | chkconfig dummy off       | systemctl disable dummy.service     |
| Check if a service is enabled at startup          | chkconfig dummy           | systemctl is-enabled dummy.service  |
| Create a new service file or modify configuration | chkconfig dummy --add     | systemctl daemon-reload             |

*Note : New version of systemd support "systemctl start dummy" format.*

2. Runlevels
------------

| Comments                                 | SysVinit                                     | Systemd                                                                        |
|------------------------------------------|----------------------------------------------|--------------------------------------------------------------------------------|
| System halt                              | 0                                            | runlevel0.target, poweroff.target                                              |
| Single user mode                         | 1, s, single                                 | runlevel1.target, rescue.target                                                |
| Multi user                               | 2                                            | runlevel2.target, multi-user.target                                            |
| Multi user with Network                  | 3                                            | runlevel3.target, multi-user.target                                            |
| Experimental                             | 4                                            | runlevel4.target, multi-user.target                                            |
| Multi user, with network, graphical mode | 5                                            | runlevel5.target, graphical.target                                             |
| Reboot                                   | 6                                            | runlevel6.target, reboot.target                                                |
| Emergency Shell                          | emergency                                    | emergency.target                                                               |
| Change to multi user runlevel/target     | telinit 3                                    | systemctl isolate multi-user.target (OR systemctl isolate runlevel3.target)    |
| Set multi-user target on next boot       | sed s/^id:.*:initdefault:/id:3:initdefault:/ | ln -sf /lib/systemd/system/multiuser.target /etc/systemd/system/default.target |
| Check current runlevel                   | runlevel                                     | systemctl get-default                                                          |
| Change default runlevel                  | sed s/^id:.*:initdefault:/id:3:initdefault:/ | systemctl set-default multi-user.target                                        |

3. Miscellaneous Commands
-------------------------

| Comments                   | SysVinit                                             | Systemd             |
|----------------------------|------------------------------------------------------|---------------------|
| System halt                | halt                                                 | systemctl halt      |
| Power off the system       | poweroff                                             | systemctl poweroff  |
| Restart the system         | reboot                                               | systemctl reboot    |
| Suspend the system         | pm-suspend                                           | systemctl suspend   |
| Hibernate                  | pm-hibernate                                         | systemctl hibernate |
| Follow the system log file | tail -f /var/log/messages or tail -f /var/log/syslog | journalctl -f       |

4. Systemd New Commands
-----------------------

| Comments                                                    | Systemd                                    |
|-------------------------------------------------------------|--------------------------------------------|
| Execute a systemd command on remote host                    | systemctl dummy.service start -H user@host |
| Check boot time                                             | systemd-analyze or systemd-analyze time    |
| Kill all processes related to a service                     | systemctl kill dummy                       |
| Get logs for events for today                               | journalctl --since=today                   |
| Hostname and other host related information                 | hostnamectl                                |
| Date and time of system with timezone and other information | timedatectl                                |

