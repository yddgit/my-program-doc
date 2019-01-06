# [HBase](http://hbase.apache.org/2.0/book.html "Apache HBase ™ Reference Guide")

## Standalone HBase

* 下载: https://www.apache.org/dyn/closer.lua/hbase/
* 安装JDK, 设置JAVA_HOME环境变量, 或者在conf/hbase-env.sh中设置JAVA_HOME. JAVA_HOME是一个包含bin/java命令的目录, 在RHEL或CentOS上通常为/usr/bin/alternatives
* 修改conf/hbase-site.xml
  ```xml
  <configuration>
    <property>
      <!--这个目录hbase会自动创建, 如果已经存在hbase会尝试迁移数据-->
      <name>hbase.rootdir</name>
      <value>file:///home/to/hbase/data</value>
    </property>
    <property>
      <name>hbase.zookeeper.property.dataDir</name>
      <value>/home/to/zookeeper/data</value>
    </property>
  </configuration>
  ```
* 启动hbase: bin/start-hbase.sh
* 启动后可以通过 http://localhost:16010 查看hbase基本信息
* 登录hbase: bin/hbase shell
* 停止hbase: bin/stop-hbase.sh

## 常用命令

```sh
# 获取帮助
help
# 创建表, 指定表名和ColumnFamily名
create 'test', 'cf'
# 查看表信息
list 'test'
# put数据到数据表
put 'test', 'row1', 'cf:a', 'value1'
put 'test', 'row2', 'cf:b', 'value2'
put 'test', 'row3', 'cf:c', 'value3'
# 查询所有数据
scan 'test'
# 获取单独一行数据
get 'test', 'row1'
# 在删除表或修改表配置时需要先禁用表, 再启用表
disable 'test'
enable 'test'
# 删除表
drop 'test'
# 退出hbase shell
exit
```

# Pseudo-Distributed Local Install

* 在conf/hbase-site.xml中增加如下配置
  ```xml
  <property>
    <name>hbase.cluster.distributed</name>
    <value>true</value>
  </property>
  <property>
    <!--这个目录hbase会自动创建, 如果已经存在hbase会尝试迁移数据-->
    <name>hbase.rootdir</name>
    <value>hdfs://localhost:8020/hbase</value>
  </property>
  ```
* bin/start-hbase.sh
* 查看HDFS上的目录: bin/hadoop fs -ls /hbase
* 启动backup HMaster服务(参数表示相比默认端口16010/16020/16030的递增量)
  ```sh
  # 启动三个backup HMaster, 端口为16012/16022/16032, 16013/16023/16033, and 16015/16025/16035
  bin/local-master-backup.sh 2 3 5
  ```
* 停止backup HMaster
  在数据目录中hbase-USER-X-master.pid记录了backup HMaster的PID
  ```sh
  cat /data/to/hbase/hbase-testuser-1-master.pid | xargs kill -9
  ```
* 启动RegionServer(参数表示相比默认端口16200/16300的递增量)
  ```sh
  # 启动四个RegionServer
  bin/local-regionservers.sh start 2 3 4 5
  ```
* 停止RegionServer
  ```sh
  bin/local-regionservers.sh stop 3
  ```
* bin/stop-hbase.sh

## Advanced-Fully Distributed

|Node|Master|ZooKeeper|RegionServer|
|----|------|---------|------------|
|node0|yes|yes|no|
|node1|backup|yes|yes|
|node2|no|yes|yes|

配置node0和node1到所有结点(包括本机)的ssh免密登录
```sh
# 对于node0
ssh-keygen -t rsa
ssh-copy-id node0
ssh-copy-id node1
ssh-copy-id node2
```

配置node0:
* 修改conf/regionservers
  ```
  node1
  node2
  ```
* 创建或修改conf/backup-masters
  ```
  node1
  ```
* 若使用外部zookeeper, 需修改conf/hbase-env.sh
  ```sh
  export HBASE_MANAGES_ZK=false
  ```
* 修改conf/hbase-site.xml
  ```xml
  <property>
    <name>zookeeper.znode.parent</name>
    <value>/hbase</value>
  </property>
  <property>
    <name>hbase.cluster.distributed</name>
    <value>true</value>
  </property>
  <property>
    <name>hbase.rootdir</name>
    <value>hdfs://node0:9000/hbase</value>
  </property>
  <property>
    <name>hbase.zookeeper.property.clientPort</name>
    <value>2181</value>
  </property>
  <property>
    <name>hbase.zookeeper.quorum</name>
    <value>node0,node1,node2</value>
  </property>
  <property>
    <name>hbase.unsafe.stream.capability.enforce</name>
    <value>false</value>
  </property>
  ```
* 将node0的配置copy到node1和node2
* 启动集群: 在node0上执行bin/start-hbase.sh
  终端输出类似如下内容(启动顺序: ZooKeeper->master->RegionServers->backup masters):
  ```
  running master, logging to /usr/local/hbase-2.0.0/bin/../logs/hbase-root-master-node0.out
  node1: running regionserver, logging to /usr/local/hbase-2.0.0/bin/../logs/hbase-root-regionserver-node1.out
  node2: running regionserver, logging to /usr/local/hbase-2.0.0/bin/../logs/hbase-root-regionserver-node2.out
  node1: running master, logging to /usr/local/hbase-2.0.0/bin/../logs/hbase-root-master-node1.out
  ```

