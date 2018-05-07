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
* Connector API建立一个可利用的producer和consumer，用于连接现有的topic和应用程序。比如连接关系数据库获取表的所有变更数据

## Producer API

```java
Properties props = new Properties();
props.put("bootstrap.servers", "localhost:9092");
props.put("acks", "all");
props.put("retries", 0);
props.put("batch.size", 16384);
props.put("linger.ms", 1);
props.put("buffer.memory", 33554432);
props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");

Producer<String, String> producer = new KafkaProducer<>(props);
for (int i = 0; i < 100; i++) {
    producer.send(new ProducerRecord<String, String>("my-topic", Integer.toString(i), Integer.toString(i)));
}

producer.close();
```

1. Producer会缓存还未发送到cluster的数据，如果没有成功关闭producer会导致内存泄露
2. `send()`方法是异步的，调用后会将数据添加到buffer中然后返回，这样producer就可以进行批量提交，提高效率
3. `ack`配置何种情况认为请求已完成，`all`表示所有记录都要确认提交，请求才算完成，是最慢但是最常用的设置
4. 请求失败后，producer会自动重试，除非`retries`设为0，允许重试可能会导致重复提交
5. Producer管理着每个partition未发送数据的buffer，由`batch.size`进行配置，增大该值可以提高批处理量，但需要更多的内存空间（因为对于每个活动分区通常都会有一个buffer）
6. 默认缓冲区未满也可以发送数据，但如果需要减少请求次数，可以设置`linger.ms`参数为大于0的值x。这样每次请求前如果buffer未满，会等待x毫秒，使批量提交的数据记录尽可能的多。当然，如果负载很大，这个参数基本会被忽略，而设置一个大于0的值可以用微小的延迟换取低负载下更少更有效的请求次数。
7. `buffer.memory`控制producer可用于缓存数据的总内存大小。产生数据的速度快于发送数据的速度时缓冲区就会很快被占满，此时新的请求就会被阻塞，可以用`max.block.ms`参数指定阻塞的时间，一旦超时就会抛出`TimeoutException`
8. `key.serializer`和`value.serializer`指定了数据转换的规则，内置的有`ByteArraySerializer`和`StringSerializer`可以用于简单的`string`和`byte`类型的转换

从Kafka 0.11开始，Kafka支持两种类型的producer：idempotent(幂等的) producer和transactional(支持事务的) producer。Idempotent producer使用Kafka可以保证exactly once的消息投递，并且producer的重试不会导致数据重复。Transactional producer可以保证应用向多个partition或topic发送消息的原子性。

### Idempotent producer

```js
enable.idempotence: true
// enable.idempotence设为true后会导致如下默认值
retries: Integer.MAX_VALUE
ack: "all"
```
目前的API不需要特别设置，默认就是Idempotent producer。需要注意的是，一定要避免应用层面的消息重发，这对于kafka来说是没办法去重的。如果应用本身支持消息幂等性，那就不要设置`retries`参数，让它默认为`Integer.MAX_VALUE`。同时如果send()方法返回错误，建议关闭producer实例，检查最后一次生产的数据确保不会重复。Producer只能在单个session中保证消息的幂等性。

### Transactional producer

```js
transactional.id: "transaction-1"
// transactional.id一设置，idempotence和相关的配置也会自动启用
// 同时transaction内的topic必须有冗余和高可用的参数配置
// replication.factor >= 3
// min.insync.replicas = 2
// 最后为了保证事务在两端的一致，consumer也必须配置只能读取已提交的数据
```
`transactional.id`允许单个producer实例的多个session之间的事务，它通常由分区的、有状态的应用的分片标识生成。在这个分区应用中每个producer实例的`transactional.id`必须保证唯一。

```java
Properties props = new Properties();
props.put("bootstrap.servers", "localhost:9092");
props.put("transactional.id", "my-transactional-id");
Producer<String, String> producer = new KafkaProducer<>(props, new StringSerializer(), new StringSerializer());

producer.initTransactions();

try {
    producer.beginTransaction();
    for (int i = 0; i < 100; i++) {
        producer.send(new ProducerRecord<>("my-topic", Integer.toString(i), Integer.toString(i)));
    }
    producer.commitTransaction();
} catch (ProducerFencedException | OutOfOrderSequenceException | AuthorizationException e) {
    // We can't recover from these exceptions, so our only option is to close the producer and exit.
    producer.close();
} catch (KafkaException e) {
    // For all other exceptions, just abort the transaction and try again.
    producer.abortTransaction();
}
producer.close();
```

所有新的transactional API如果请求失败都会停止并抛出异常。每个producer只能打开一个transaction，在`beginTransaction()`和`commitTransaction()`中间的调用都是单个transaction的一部分。Transactional producer使用异常标识错误状态，不需要为`producer.send()`方法指定回调或调用get()方法获取Future对象。如果`send()`方法或者事务调用发生了不可恢复的错误会抛出`KafkaException`，此时调用`producer.abortTransaction()`方法可以中止已经成功提交的写操作，回滚事务。

