# Setup Samba as an Active Directory Domain Controller

Samba从4.0开始可以做为AD域的域控提供服务，在Windows Server 2008 R2的功能级别提供服务。

以下在一个全新的Ubuntu Server上安装Samba，大致需要5个步骤

> VirtualBox安装Ubuntu Server 22.04.1时，处理器务必配置为2，否则无法安装

- 安装Samba相关的包
- 删除已经提供好的Samba和Kerberos配置文件
- 使用自动化工具配置Samba
- 编辑`smb.conf`开启组策略
- 其他的环境配置

首先准备好域控的相关参数

- 主机名：dc1
- 域控IP: 192.168.56.2
- 域名：DINO.CORP

## 安装Samba相关软件

```sh
# 停用systemd-resolved
sudo systemctl disable --now systemd-resolved
# 修改resolv.conf
sudo cp /etc/resolv.conf /etc/resolv.conf.backup
sudo unlink /etc/resolv.conf
# /etc/resolv.conf文件内容参考下文
sudo vim /etc/resolv.conf
# 修改/etc/resolv.conf为不可变，防止系统重启后被修改
sudo chattr +i /etc/resolv.conf
# /etc/hosts文件添加一行，参考下文，确保主机名不会解析到127.0.*.1
sudo vim /etc/hosts
# 重启
sudo shutdown now -r

# VirtualBox虚拟机重启失败可以把安装系统时使用的iso文件关联一下再删除即可，或者可以在关机状态下直接修改虚拟机目录下的.vbox文件
# 启动报cloud-init错误可以执行如下命令关闭cloud-init相关服务
sudo systemctl status cloud-init-local cloud-init cloud-config cloud-final
sudo systemctl stop cloud-init-local cloud-init cloud-config cloud-final
sudo systemctl disable cloud-init-local cloud-init cloud-config cloud-final
sudo systemctl status cloud-init.target
sudo systemctl stop cloud-init.target
sudo systemctl disable cloud-init.target

# 安装samba相关包
# 安装过程需要依次填写如下配置值
# DINO.CORP
# dc1.dino.corp
# dc1.dino.corp
sudo apt-get install acl attr samba samba-dsdb-modules samba-vfs-modules winbind libpam-winbind libnss-winbind libpam-krb5 krb5-config krb5-user
# 如果AD用户可以登录本机可以安装如下软件包
sudo apt-get install libpam-winbind libnss-winbind libpam-krb5
# 域控会成为域中新的DNS服务器
sudo apt-get install dnsutils
# 因为域中机器需要同步时间，可以将域控做为ntp服务器，但这不是必须的，因为可以通过组策略将主机的时间服务器设置为time.windows.com
sudo apt-get install ntp
# or
sudo apt-get install chrony
```

/etc/resolv.conf

```sh
# Samba server IP address
nameserver 192.168.56.2
# fallback resolver
nameserver 114.114.114.114
# main domain for Samba
search dino.corp
```

/etc/hosts

```sh
192.168.56.2 dc1.dino.corp dc1
```

## 删除已有的配置文件

以上软件包安装完成后会自动创建相关服务的配置文件，这会影响自动化配置工具的执行，所以需要将其删掉

```sh
# 临时停止服务
sudo systemctl stop samba-ad-dc.service smbd.service nmbd.service winbind.service
sudo systemctl disable samba-ad-dc.service smbd.service nmbd.service winbind.service
# 备份配置文件
sudo mv /etc/samba/smb.conf /etc/samba/smb.conf.initial
sudo systemctl unmask samba-ad-dc
```

## 使用自动化工具配置Samba

```sh
sudo samba-tool domain provision --use-rfc2307 --interactive
  Realm [DINO.CORP]:
  Domain [DINO]:
  Server Role (dc, member, standalone) [dc]:
  DNS backend (SAMBA_INTERNAL, BIND9_FLATFILE, BIND9_DLZ, NONE) [SAMBA_INTERNAL]:
  DNS forwarder IP address (write 'none' to disable forwarding) [192.168.56.2]: 114.114.114.114
  Administrator password: Passw0rd
  Retype password: Passw0rd
  ...
```

## 编辑`smb.conf`开启组策略

```sh
# 开启samba-ad-dc服务
sudo systemctl start samba-ad-dc.service
sudo systemctl enable samba-ad-dc.service
# 验证domain
sudo samba-tool domain level show
# 验证服务启动状态
sudo systemctl status samba-ad-dc.service
```

编辑`/etc/samba/smb.conf`文件，在`[globals]`部分添加下面的内容

```
allow group policies = yes
```

## 其他的环境配置

确保`/etc/resolv.conf`文件中包含如下内容

```sh
search dino.corp
nameserver 192.168.56.2
```

创建reverse zone

```sh
# 下面创建命令会要求输入Administrator的密码：Passw0rd
sudo samba-tool dns zonecreate 192.168.56.2 56.168.192.in-addr.arpa -U Administrator
```

配置Kerberos

```sh
sudo mv /etc/krb5.conf /etc/krb5.conf.initial
sudo ln -s /var/lib/samba/private/krb5.conf /etc/
sudo systemctl restart samba-ad-dc.service
```

验证File Server

```sh
sudo smbclient -L localhost -N
sudo smbclient //localhost/netlogin -UAdministrator -c 'ls'
```

验证DNS

```sh
sudo host -t SRV _ldap._tcp.dino.corp.
sudo host -t SRV _kerberos._udp.dino.corp.
sudo host -t A dc1.dino.corp.
```

验证Kerberos

```sh
sudo kinit administrator
sudo klist
```

## Add Ubuntu Server to Domain as a Member

```sh
# 停用systemd-resolved
sudo systemctl disable --now systemd-resolved
# 修改resolv.conf
sudo cp /etc/resolv.conf /etc/resolv.conf.backup
sudo unlink /etc/resolv.conf
# /etc/resolv.conf文件内容参考下文
sudo vim /etc/resolv.conf
# 修改/etc/resolv.conf为不可变，防止系统重启后被修改
sudo chattr +i /etc/resolv.conf
# /etc/hosts文件添加一行，参考下文，确保主机名不会解析到127.0.*.1
sudo vim /etc/hosts
# 重启
sudo shutdown now -r

# 测试DNS
sudo nslookup dc1.dino.corp
sudo host dc1.dino.corp
sudo nslookup 192.168.56.2
sudo host 192.168.56.2
sudo host -t SRV _ldap._tcp.dino.corp

# 配置Kerberos，文件内容参考下文
sudo vim /etc/krb5.conf
sudo vim /etc/security/pam_winbind.conf

# 安装Samba
sudo apt-get install acl attr samba samba-dsdb-modules samba-vfs-modules winbind libpam-winbind libnss-winbind libpam-krb5 krb5-config krb5-user
sudo apt-get install dnsutils
sudo systemctl stop samba-ad-dc.service smbd.service nmbd.service winbind.service
sudo systemctl disable samba-ad-dc.service smbd.service nmbd.service winbind.service
sudo mv /etc/samba/smb.conf /etc/samba/smb.conf.initial
sudo systemctl unmask samba-ad-dc
# 确认Samba配置文件路径
sudo smbd -b | grep CONFIGFILE
# 配置Samba，配置文件内容参考下文
sudo vim /etc/samba/smb.conf
# 配置首次登录自动创建home目录
sudo pam-auth-update --enable mkhomedir
# 将域管理员映射到本地root用户，配置文件内容参考下文
sudo mkdir -p /usr/local/samba/etc/
sudo vim /usr/local/samba/etc/user.map

# 添加主机到域，以下命令会提示输入管理员密码：Passw0rd
sudo net ads join -U Administrator
sudo samba-tool domain join dino.corp MEMBER -U Administrator

# 配置用户和组的解析，找到passwd和group的配置，在后面追加winbind，效果类似下文示例
sudo vim /etc/nsswitch.conf

# 启动Samba相关服务
sudo systemctl enable smbd nmbd winbind
sudo systemctl start smbd nmbd winbind
sudo systemctl status smbd nmbd winbind

# 确认加域成功
sudo wbinfo --ping-dc

# 查询域账号
sudo getent passwd DINO\\Administrator
# 查询域用户组
sudo getent group "DINO\\Domain Users"
```

/etc/resolv.conf

```sh
# Samba server IP address
nameserver 192.168.56.2
# fallback resolver
nameserver 114.114.114.114
# main domain for Samba
search dino.corp
```

/etc/hosts

```sh
192.168.56.10 server01.dino.corp server01
```

/etc/krb5.conf

```conf
[libdefaults]
	default_realm = DINO.CORP
	dns_lookup_realm = false
	dns_lookup_kdc = true
```

/etc/security/pam_winbind.conf

```conf
[global]
      krb5_auth = yes
      krb5_ccache_type = FILE
```

/etc/samba/smb.conf

```conf
[global]
        log file = /var/log/samba/%m.log
        log level = 1
        realm = DINO.CORP
        security = ADS
        server role = member server
        template shell = /bin/bash
        username map = /usr/local/samba/etc/user.map
        workgroup = DINO
        idmap config * : backend = autorid
        idmap config * : range = 10000-9999999
        winbind use default domain = yes
```

/usr/local/samba/etc/user.map

```conf
!root = DINO\Administrator
```

/etc/nsswitch.conf

```conf
passwd: files systemd winbind
group:  files systemd winbind
```
