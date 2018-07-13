# Hive的安装和简单配置

## 安装MySQL并创建hive数据库

```sql
--创建数据库
drop database if exists hive;
create database hive default charset latin1 collate latin1_swedish_ci;
--创建用户
create user if not exists 'hive'@'%' identified by 'password';
--分配权限
grant all privileges on hive.* to 'hive'@'%' identified by 'password';
flush privileges;
```

## 下载相关软件包

- Hive安装包：[apache-hive-2.1.1-bin.tar.gz](https://archive.apache.org/dist/hive/hive-2.1.1/apache-hive-2.1.1-bin.tar.gz "Hive安装包")
- Hive源码包：[apache-hive-2.1.1-src.tar.gz](https://archive.apache.org/dist/hive/hive-2.1.1/apache-hive-2.1.1-src.tar.gz "Hive源码包")
- MySQL JDBC 驱动：[mysql-connector-java-6.0.6.jar](http://central.maven.org/maven2/mysql/mysql-connector-java/6.0.6/mysql-connector-java-6.0.6.jar "MySQL JDBC 驱动")

## 安装Hive

将apache-hive-2.1.1-bin.tar.gz上传到hadoop集群的NameNode(node0)上

```bash
# 解压到/usr/local，目录命名为hive
mv apache-hive-2.1.1-bin.tar.gz /usr/local
cd /usr/local
tar -xzf apache-hive-2.1.1-bin.tar.gz
mv apache-hive-2.1.1-bin hive
```

### 配置环境变量

```bash
# /etc/profile.d/hadoop.sh
vim /etc/profile.d/hadoop.sh
export HADOOP_HOME=/usr/local/hadoop
export PATH=${PATH}:${HADOOP_HOME}/bin
# 使环境变量生效
source /etc/profile.d/hadoop.sh
```

```bash
# /etc/profile.d/hive.sh
vim /etc/profile.d/hive.sh
export HIVE_HOME=/usr/local/hive
export PATH=${PATH}:${HIVE_HOME}/bin
# 使环境变量生效
source /etc/profile.d/hive.sh
```

### 编译安装hive-hwi-2.1.1.war包

```bash
# 先上传apache-hive-2.1.1-src.tar.gz
tar -xzf apache-hive-2.1.1-src.tar.gz
cd apache-hive-2.1.1-src/hwi
# 使用maven打HWI的war包，可能会比较久
mvn clean war:war
# 将打好的war包复制${HIVE_HOME}/lib
cp target/hive-hwi-2.1.1.war /usr/local/hive/lib
# 将JDK目录下的tools.jar复制到${HIVE_HOME}/lib
cp ${JAVA_HOME}/lib/tools.jar /usr/local/hive/lib
```

### 安装MySQL JDBC驱动

```bash
cp mysql-connector-java-6.0.6.jar /usr/local/hive/lib
```

### 配置Hive

创建hive-site.xml

```bash
cd /usr/local/hive/conf
vim hive-site.xml
```

hive-site.xml文件内容

Server端:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <!-- mysql metastore -->
  <property>
    <name>javax.jdo.option.ConnectionURL</name>
    <value>jdbc:mysql://docker:3306/hive?useSSL=false&amp;nullNamePatternMatchesAll=true</value>
  </property>
  <property>
    <name>javax.jdo.option.ConnectionDriverName</name>
    <value>com.mysql.cj.jdbc.Driver</value>
  </property>
  <property>
    <name>javax.jdo.option.ConnectionUserName</name>
    <value>hive</value>
  </property>
  <property>
    <name>javax.jdo.option.ConnectionPassword</name>
    <value>password</value>
  </property>
  <!-- dir config -->
  <property>
    <name>hive.exec.scratchdir</name>
    <value>/hive/tmp/hive-${user.name}</value><!-- hdfs path -->
  </property>
  <property>
    <name>hive.exec.local.scratchdir</name>
    <value>/usr/local/hive/tmp/${user.name}</value><!-- local path -->
  </property>
  <!-- metastore -->
  <property>
    <name>hive.metastore.uris</name>
    <value>thrift://node0:9083</value>
  </property>
  <property>
    <name>hive.metastore.schema.verification</name>
    <value>false</value>
  </property>
  <property>
    <name>hive.metastore.warehouse.dir</name>
    <value>/hive/warehouse</value><!-- hdfs path -->
  </property>
  <!-- hwi -->
  <property>
    <name>hive.hwi.war.file</name>
    <value>lib/hive-hwi-2.1.1.war</value>
  </property>
  <property>
    <name>hive.hwi.listen.host</name>
    <value>node0</value>
  </property>
  <property>
    <name>hive.hwi.listen.port</name>
    <value>9999</value>
  </property>
  <!-- hiveserver2 -->
  <property>
    <name>hive.server2.thrift.port</name>
    <value>10000</value>
  </property>
  <property>
    <name>hive.server2.thrift.bind.host</name>
    <value>node0</value>
  </property>
  <property>
    <name>hive.server2.webui.port</name>
    <value>10002</value>
  </property>
  <property>
    <name>hive.server2.webui.host</name>
    <value>node0</value>
  </property>
</configuration>
```

Client端:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <!-- metastore -->
  <property>
    <name>hive.metastore.uris</name>
    <value>thrift://node0:9083</value>
  </property>
  <property>
    <name>hive.metastore.schema.verification</name>
    <value>false</value>
  </property>
  <property>
    <name>hive.metastore.warehouse.dir</name>
    <value>/hive/warehouse</value><!-- hdfs path -->
  </property>
</configuration>
```

### 初始化schema

```bash
cd /usr/local/hive/bin/
# 在mysql数据库里创建hive存储元数据用到的表
./schematool -initSchema -dbType mysql
```

### 依次启动metastore、hiveserver2、HWI服务

```bash
# start-hive-metastore.sh
nohup /usr/local/hive/bin/hive --service metastore > /usr/local/hive/logs/hive-metastore.log 2>&1 &
# start-hive-server2.sh
nohup /usr/local/hive/bin/hive --service hiveserver2 > /usr/local/hive/logs/hive-server2.log 2>&1 &
# start-hive-hwi.sh
nohup /usr/local/hive/bin/hive --service hwi > /usr/local/hive/logs/hive-hwi.log 2>&1 &
```

### 使用beeline访问Hive

```bash
# /usr/local/hive/connect-with-beeline.sh
/usr/local/hive/bin/beeline -u jdbc:hive2://node0:10000/default
```

