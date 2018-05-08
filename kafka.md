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

通常用于两类应用：

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
* Connector API建立一个可重用的producer和consumer，用于连接现有的topic和应用程序。比如连接关系数据库获取表的所有变更数据

## Producer API(java)

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
2. `send()`方法是异步的，调用后会将数据添加到buffer中然后返回，这样producer内部就可以进行批量提交，提高效率
3. `ack`配置何种情况认为请求已完成，`all`表示所有记录都要确认提交，请求才算完成，是最慢但是最常用的设置
4. 请求失败后，producer会自动重试，除非`retries`设为0，允许重试可能会导致重复提交
5. Producer管理着每个partition未发送数据的buffer，由`batch.size`进行配置，增大该值可以提高批处理量，但需要更多的内存空间（因为对于每个活动partition通常都会有一个buffer）
6. 默认缓冲区未满也可以发送数据，但如果需要减少请求次数，可以设置`linger.ms`参数为大于0的值x。这样每次请求前如果buffer未满，会等待x毫秒，使批量提交的数据记录尽可能的多。当然，如果负载很大，这个参数基本会被忽略，而设置一个大于0的值可以用微小的延迟换取低负载下更少更有效的请求次数。
7. `buffer.memory`控制producer可用于缓存数据的总内存大小。产生数据的速度快于发送数据的速度时缓冲区就会很快被占满，此时新的请求就会被阻塞，可以用`max.block.ms`参数指定阻塞的时间，一旦超时就会抛出`TimeoutException`
8. `key.serializer`和`value.serializer`指定了数据转换的规则，内置的有`ByteArraySerializer`和`StringSerializer`可以用于简单的`string`和`byte`类型的转换

从Kafka 0.11开始，Kafka支持两种类型的producer：idempotent(幂等的) producer和transactional(支持事务的) producer。Idempotent producer使Kafka可以保证exactly once的消息投递，并且producer的重试不会导致数据重复。Transactional producer可以保证应用向多个partition或topic发送消息的原子性。

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

## Consumer API(java)

Kafka的Java Consumer API
1. 可以自动感知kafka broker结点failure
2. 可以自动适配topic中partition在集群结点中的迁移
3. 与broker交互，确保consumer group中的各个consumer负载均衡
4. consumer与broker之间通过TCP连接来获取数据，没有成功关闭consumer会导致连接溢出
5. consumer不是线程安全的

对于每个partition，Kafka都维护了一个offset数值，是一条消息在partition内的唯一标识，也标识了consumer在partition内的位置。如一个consumer的offset=5表示它已经消费了0-4的消息，下一个要消费的就offset=5的消息。这里有两个概念：

一、consumer的position标识了下一个将被消费的数据位置，是一个比之前消费数据的offset都大的offset数值(在当前partition中)，它会在poll()方法接收到消息后自动增大。

二、consumer的committed position是最后一次成功处理消息的offset。如果处理失败或重启，consumer可以从此offset恢复。consumer可以定期自动提交此offset，也可以通过调用`commitSync`/`commitAsync` API手动控制该offset。

### Consumer Groups and Topic Subscriptions

Kafka使用consumer group支持多个进程进行消息的消费和处理，这些进程可以在同一台机器也可在不同的机器上，从而为这些进程提供扩展和容错机制。使用同一个`group.id`的consumer实例就属于同一个consumer group。

Group中的每个consumer都可以通过`subscribe` API动态订阅一组topic，Kafka会将已订阅topic的每条消息发送到consumer group中的一个进程。为了在consumer group中的每个consumer之间均衡partition，每个partition会分配group中的一个确定的consumer。如对于一个有4个partition的topic，一个有两个consumer实例的consumer group，那每个consumer进程将会分配两个partition的数据。

Consumer group中的成员关系是动态管理的：如果一个consumer挂了，分配在其上的partition会重新分配到同一group中的另一个consumer上。如果一个新的consumer加入group，其他consumer上的partition会移到新consumer上。同样，如果已订阅的topic增加了partition或有新的匹配`subscribed regex`的topic被创建，consumer group也会通过周期性元数据刷新自动检测到新的partition，然后将其分配到group内的consumer上

逻辑上可以将一个consumer group当作一个订阅者，只不过它由多个进程组成。Kafka支持一个topic关联任意数量的consumer group而无需复制数据。

当consumer group发生自动重新分配partition时，consumer可以通过`ConsumerRebalanceListener`获得通知，可以在获得通知后执行一些应用层面的必要处理，如：状态清理、手动提交offset等。同时，consumer也可以使用`assign(Collection)`方法手动分配指定的partition，此时，动态partition分配和consumer group协调将被禁用。

### Detecting Consumer Failures

订阅一组topic后，consumer会随`poll()`方法的调用自动加入consumer group。`poll` API使consumer是存活的、有状态的。只要`poll()`方法在被调用，consumer就会始终存活在group中，并接收分配在其上的partition的消息。事实上，consumer与server之间维护着周期性的心跳检测。如果consumer挂了或没有发送心跳，超过`session.timeout.ms`配置的值，该consumer会被认为已经dead，分配在其上的partition会被重新分配到其他存活的consumer上。

还有一种可能，某个consumer能够正常发送心跳，但不处理任何消息。为了避免这种情况下consumer无限期的保留分配在其上的partition而不处理，Kafka通过配置`max.poll.interval.ms`提供了活跃度检测的机制。也就是consumer调用`poll()`方法的周期超过配置的时间间隔时，consumer会主动退出group，由其他consumer来接管该partition的消息。如果发生此情行，会发生offset提交失败(即调用`commitSync()`方法时抛出`CommitFailedException`)。这是确保只有active的结点才能提交offset的一种安全机制。因此要想待在consumer group中，就必须持续调用`poll()`方法。

Consumer提供了两个配置项控制poll循环的行为：

1. `max.poll.interval.ms`: 增大两次poll的时间间隔，可以给consumer足够的时间去处理poll()返回的一批数据。缺点是会延迟consumer group的rebalance，因为只有在调用poll()时，consumer才能加入group。可以用此配置的值来界定完成一次rebalance的时间。如果consumer不能足够频繁的调用poll()方法，那就会有处理变慢的风险。

2. `max.poll.records`: 此配置用于限制单次poll调用返回的总记录数。这可以很容易预测每次循环调用间隔内必须被处理的最大记录数。调优这个配置可以减少每次循环的间隔，从而减少group rebalance的影响。

不同的用户场景，消息处理时间各不相同，难以预测，这两个配置可能都不足以解决问题。处理这种场景推荐的做法是将消息处理的工作放到另外一个线程中完成，这样consumer就能持续的调用poll()。不过，必须确保提交的offset不会超过实际的位置。通常，要禁用自动offset提交，手动在消息处理线程结束后提交offset。同时还需要`pause` partition，这样在上一次接收的消息处理完毕之前，consumer就不会从poll调用中接收到新的消息。

#### Automatic Offset Committing

```java
Properties props = new Properties();
props.put("bootstrap.servers", "localhost:9092");
props.put("group.id", "test");
props.put("enable.auto.commit", "true");
props.put("auto.commit.interval.ms", "1000");
props.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
props.put("value.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
KafkaConsumer<String, String> consumer = new KafkaConsumer<>(props);
consumer.subscribe(Arrays.asList("foo", "bar"));
while (true) {
    ConsumerRecords<String, String> records = consumer.poll(100);
    for (ConsumerRecord<String, String> record : records)
        System.out.printf("offset = %d, key = %s, value = %s%n", record.offset(), record.key(), record.value());
}
```

上例中consumer订阅了`foo`、`bar`两个topic，并加入名为`test`的consumer group。

`bootstrap.servers`: 连接集群中的一组broker，不用列出全部broker，至少列出一台以上，防止客户端连接时集群结点发生故障
`enable.auto.commit`: offset会以`auto.commit.interval.ms`指定的周期自动提交
`*.deserializer`: 指定了byte数组转换为对象的规则。`StringDeserializer`是将key、value都转换为string

#### Manual Offset Control

用户可以控制消息何时消费完成，然后手动提交offset。当消费消息和处理消息的逻辑混合在一起的时候，很明显只有消息处理完成之后才算是消费完成。

```java
Properties props = new Properties();
props.put("bootstrap.servers", "localhost:9092");
props.put("group.id", "test");
props.put("enable.auto.commit", "false");
props.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
props.put("value.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
KafkaConsumer<String, String> consumer = new KafkaConsumer<>(props);
consumer.subscribe(Arrays.asList("foo", "bar"));
final int minBatchSize = 200;
List<ConsumerRecord<String, String>> buffer = new ArrayList<>();
while (true) {
    ConsumerRecords<String, String> records = consumer.poll(100);
    for (ConsumerRecord<String, String> record : records) {
        buffer.add(record);
    }
    if (buffer.size() >= minBatchSize) {
        insertIntoDb(buffer);
        consumer.commitSync();
        buffer.clear();
    }
}
```

**上例**：consumer将消费的数据缓存在内存中，当缓存的数据足够多的时候，再将其插入数据库中。

如果允许offset自动提交，那当poll()接收到消息时就认为消费完成了，但在数据插入数据库之前，程序可能会出错，这就会导致数据丢失。

为了避免上述问题，需要在对应的数据记录成功插入数据库之后再提交offset。这使得我们能够明确的控制消息何时消费完成。但是，在数据库提交成功之后，手动提交offset之前，程序可能出错。这就会导致数据重复，这就是Kafka的`at-least-once`的消息投递保证：通常只会投递一次，失败的情形会导致数据重复。

**注**：自动提交offset也是一种`at-least-once`的投递，但如果消息处理失败会导致数据丢失，而手动提交offset不会有这种问题。

```java
try {
    while(running) {
        ConsumerRecords<String, String> records = consumer.poll(Long.MAX_VALUE);
        for (TopicPartition partition : records.partitions()) {
            List<ConsumerRecord<String, String>> partitionRecords = records.records(partition);
            for (ConsumerRecord<String, String> record : partitionRecords) {
                System.out.println(record.offset() + ": " + record.value());
            }
            long lastOffset = partitionRecords.get(partitionRecords.size() - 1).offset();
            consumer.commitSync(Collections.singletonMap(partition, new OffsetAndMetadata(lastOffset + 1)));
        }
    }
} finally {
  consumer.close();
}
```

**上例**：在每个partition上消费完消息之后才提交offset。因为提交的offset是应用程序读取的下一条消息的的offset，所以需要在lastOffset上加1。

#### Manual Partition Assignment

在有些场合下，需要更好的控制特定partition的分配，如：

* 如果处理包含关联partition的一些本地状态，就需要从partition的本地磁盘获取数据

* 如果处理程序是支持HA的，当结点失效后会自动重启，这时就不需要kafka检测失效并重新分配partition，因为consumer会自动在别一台机器上重启并接管其原来分配到的partition。

使用这种模型，不需要调用subscribe()方法，只需要直接调用`assign(Collection)`方法，方法参数指定需要消费的partition列表。

```java
String topic = "foo";
TopicPartition partition0 = new TopicPartition(topic, 0);
TopicPartition partition1 = new TopicPartition(topic, 1);
consumer.assign(Arrays.asList(partition0, partition1));
```

一旦分配成功，就可以循环调用poll()方法，与之前消费数据的操作类似。consumer指定的group仍然可以提交offset，但修改partition只能再次调用assign()方法。手动分配之后，就不会使用consumer group的自动分配了，consumer失效也不会导致rebalance。每个consumer虽然在同一个group中但却相互独立。为了防止offset提交冲突，通常应该保证每个consumer的group.id是唯一的。

注意：不能将手动分配partition和自动分配partition混合使用。

#### Storing Offsets Outside Kafka

Consumer程序可以不使用kafka内置的offset存储，它可以将offset存储在其他地方。这一功能主要的使用场景是允许应用程序在同一个系统中以原子方式存储offset和消费结果。如果可以，就可以保证消费的原子性，保证exactly once的消息投递。这比Kafka提供的offset提交功能更健壮。

* 如果消费结果存储在关系数据库中，那么offset也可以在同一个事务中被提交到数据库。从而要么事务成功提交，offset根据已消费记录的位置被更新；要么事务失败回滚，消费结果不会保存，offset也不会更新。

* 如果消费结果保存在本地，那offset也可以保存在本地。例如：可以通过订阅特定的partition，基于offset和数据构建查询索引。如果这是一个原子操作，那么通常即使程序崩溃导致非同步数据丢失，存量数据里总是有对应的offset的。这时恢复索引数据就只需要从最后一次提交的offset开始恢复即可。

每条消息都有一个offset，手动管理offset需要做如下操作：

* `enable.auto.commit=false`
* 使用每个`ConsumerRecord`提供的offset保存位置
* Consumer重启恢复时调用`seek(TopicPartition, long)`方法

这是手动分配partition时最简单的一种用法。如果partition是自动分配的，那就要关注partition分配的变化，可以通过在调用`subsribe(Collection, ConsumerRebalanceListener)`和`subsribe(Pattern, ConsumerRebalanceListener)`方法传入一个`ConsumerRebalanceListener`实例来实现。例如：当一个partition被从一个consumer转移到另一个consumer时要提交该partition的offset，就要实现`ConsumerRebalanceListener.onPartitionsRevoked(Collection)`方法。当partition被分配到一个consumer上时，consumer想要查询这些新partition的offset以正确初始化consumer的offset，就要实现`ConsumerRebalanceListener.onPartitionsAssigned(Collection)`方法。

另一个ConsumerRebalanceListener的用法是刷新应用程序管理的partition的缓存数据。

#### Controlling The Consumer's Position

大多数情况下，consumer只是简单的从头到尾的消费数据，周期性的提交自已的offset（不管是自动还是手动）。Kafka允许consumer手动控制自己的offset，在一个partition内自由向前或向后移动。这意味着consumer可以重新消费已经消费过的数据，或者跳过中间记录，直接消费最近的消息。

有几个手动控制offset的consumer实例可能会有一定的作用。

对于对时间敏感的消息处理程序，如果已经落后了很多条消息，而且也不想处理所有的数据记录，那就直接跳到最近的消息位置即可。

还有一个场景是应用程序维护了一些本地状态，这类应用需要在启动时就从本地存储中初始化position。同样如果本地状态被销毁(如磁盘数据丢失)，那可以在新的机器上重新消费所有数据重建本地状态(假设Kakfa保留了足够的历史数据)

Kafka使用`seek(TopicPartition, long)`方法指定新的position。特别的，跳转到开始/最新的offset使用如下两个方法：`seekToBeginning(Collection)`和`seekToEnd(Collection)`

#### Consumption Flow Control

如果一个consumer分配了多个partition，就会试图在同一时间消费所有partition的数据，给所的partition以相同的消费优先级。但有时consumer希望对于某一部分partition给予更快的处理速度，只有当这些partition数据很少或已全部消费完才会消费其他partition的数据。

其中的一种场景就是流处理，处理器从两个topic获取数据，然后将这两个数据流进行合并。当其中一个topic远远落后于另外一个时，处理器会希望暂停后者，先处理落后的topic以赶上进度。

还有一个例子是consumer启动时，有很多的历史数据需要处理，应用程序通常要在处理其他topic前先获取某些topic的最新数据。

Kafka支持通过`pause(Collection)`和`resume(Collection)`方法动态控制消费流，暂停消费或恢复已暂停的topic(在下一次调用`poll(long)`方法时)

### Reading Transactional Message

Transaction在Kafka 0.11.0中介绍过，应用可以以原子方式写多个topic或partition。为此，consumer从这些partition读取数据时就需要配置为只读取已提交的数据。

```js
// 客户端配置
isolation.level: read_committed
```

在`read_committed`模式下，consumer会只读取到已成功提交的事务数据，然后继续读取无事务的消息数据。这种模式下client端没有buffer，最后的offset就是已打开事务内的第一条消息的位置，这个offset被称为“Last Stable Offset(LSO)”

一个`read_committed`的consumer读到LSO时会过滤掉所有被中止的事务消息。LSO还会影响consumer的`seekToEnd(Collection)`和`endOffsets(Collection)`方法。最后获取数据的延迟也与LSO相关。

包含事务数据的partition会包含commit和abort的标记，以指示事务的执行结果。这些标记不会返回给应用程序，但是会占用一个offset。因此，消费事务数据时会发现offset有不连续(see gaps)。这些缺失的消息就是那些事务标记。同时，使用`read_committed`的consumer在事务被中止的时候也会看到不连续的offset。因为那些消息不会返回给consumer但他们占用了一个offset。

### Multi-threaded Processing

Kafka的consumer不是线程安全的，所有网络IO都会在应用线程中发起。用户需要保证多线程下的同步访问。非同步访问会导致`ConcurrentModifiactionException`。

唯一的例外是`wakeup()`方法，它可以被外部线程安全调用以打断一个active operation。这种情况下，如果线程阻塞在这个operation上，会抛出一个`WakeupException`。它可以用来从另一个线程关闭consumer。

```java
public class KafkaConsumerRunner implements Runnable {
    private final AtomicBoolean closed = new AtomicBoolean(false);
    private final KafkaConsumer consumer;

    public void run() {
        try {
            consumer.subscribe(Arrays.asList("topic"));
            while (!closed.get()) {
                ConsumerRecords records = consumer.poll(10000);
                // Handle new records
            }
        } catch (WakeupException e) {
            // Ignore exception if closing
            if (!closed.get()) throw e;
        } finally {
            consumer.close();
        }
    }

    // Shutdown hook which can be called from a separate thread
    public void shutdown() {
        closed.set(true);
        consumer.wakeup();
    }
}
```

然后在另一个线程中，可以通过设置closed标志并wakeup来关闭consumer。

```java
closed.set(true);
consumer.wakeup();
```

注意：也可以使用线程的interrupt方法中止阻塞的operation(这时会抛出`InterruptException`)。Kafka不鼓励这样做，因为这会导致consumer clean shutdown被中止。interrupt通常用于无法使用wakeup的场景，如consumer线程无法感知到kafka client的情况。

Kafka有意避免实现一个详细的线程模型去处理消息，给多线程处理消息的实现以更多的选择。

#### One Consumer Per Thread

简单的可以一个线程一个consumer实例，这里列出一些优缺点：
* **PRO**：实现简单
* **PRO**：的确是无线程交互的最快的实现
* **PRO**：使得每个partition按顺序处理消息实现非常容易（每个线程只需要按照接收消息的顺序处理消息就可以了）
* **CON**：Consumer越多，意味着集群TCP连接数越多，但Kafka处理连接效率很高，基本上损失很小
* **CON**：Consumer越多发送到服务器的请求数越多，略微减小了数据batch，这会导致IO吞吐量下降
* **CON**：所有进程中的全部线程数受限于partition的总数量

#### Decouple Consumption and Processing

另一个可选方案是启动一个或多个线程去做所有的数据消费工作，将消费到的`ConsumerRecord`实例存入一个处理线程池的阻塞队列中，由处理线程池中的线程去做具体的消息处理。这个方法同样有一些优缺点：

* **PRO**：可以独立的扩展consumer和processor的数量，也可以一个consumer匹配多个processor，避免partition的限制
* **CON**：要保证多个processor之间的处理顺序，需要特别关注，因为各个线程独立运行，最先消费到的数据可能最后被处理。但对于没有顺序要求的场合这不是问题
* **CON**：手动提交offset很难实现，这需要所有线程互相协调，确保partition上的处理已完成。

这种方案也会有很多其他可能的实现，比如每个processor线程可以有一个自己的队列，consumer线程可以分partition将消息发送到不同的队列来保证消费的顺序性，简化offset提交。

