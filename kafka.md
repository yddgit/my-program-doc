# Kafka

## 安装

```bash
tar -xzf kafka_2.11-1.1.0.tgz
cd kafka_2.11-1.1.0
bin/zookeeper-server-start.sh config/zookeeper.properties
bin/kafka-server-start.sh config/server.properties
```

对于windows系统，需要使用bin\windows\目录下的脚本来启动，**同时特别注意JDK安装目录不能含空格**

## 基本使用

```bash
# 创建topic
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic test
# 查看topic
bin/kafka-topics.sh --list --zookeeper localhost:2181
# 发送消息
bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test
# 消费消息
bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test --from-beginning

# 创建topic时指定replication factor
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 3 --partitions 1 --topic my-replicated-topic
# 查看topic详情
bin/kafka-topics.sh --describe --zookeeper localhost:2181 --topic my-replicated-topic
# 对比没有指定replication的topic
bin/kafka-topics.sh --describe --zookeeper localhost:2181 --topic test

# 导入导出数据
echo "test data" >> test.txt
# 开启connect(会创建一个connect-test的topic，然后把test.txt的内容逐行发达到kafka)
bin/connect-standalone.sh config/connect-standalone.properties config/connect-file-source.properties config/connect-file-sink.properties
# 以上示例中，connect开启后test.txt文件的内容会被原样写入test.sink.txt
# 同样可以用consumer消费connect-test topic的数据
bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic connect-test --from-beginning
# 此时继续向test.txt文件中写入数据，新数据会同步写入kafka，并被consumer消费掉
echo "new data" >> test.txt
```

## Kafka介绍

一个分布式流处理平台，具备以下三种能力：

* 发布、订阅消息流，类似MQ或消息系统
* 以容错持久的方式存储流数据
* 处理消息流

通过用于两类应用：

* 在系统或应用之间建立实时的数据管道用于可靠的获取数据
* 建立实时应用用于转换或处理数据流

基本概念：

* 集群方式运行，可跨数据中心部署
* 以topic的方式存储消息记录
* 每个消息记录包含一个key、一个value和一个timestamp

核心API

* Producer API用于发布消息到一个或多个topic
* Consumer API用于订阅一个或多个topic消费数据
* Streams API使应用做为一个流处理器，消费输入消息，转换处理后输出消息
* Connector API建立一个可利用的producer和consumer，用于连接现有的topic和应用程序。比如连接关系数据据获取表的所有变更数据

