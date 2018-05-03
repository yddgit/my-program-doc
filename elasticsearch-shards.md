
# [How many shards should I have in my Elasticsearch cluster?](https://www.elastic.co/blog/how-many-shards-should-i-have-in-my-elasticsearch-cluster)

Elasticsearch的非常灵活，但灵活性导致我们不知道该如何更好的组织数据和索引。

集群可能在一开始的时候可以应付，但随着数据量的增长，性能问题就会突显。数据越多，问题越难处理，从而需要时不时的进行大量的索引重建工作。

大部分性能问题分析都会追溯到“集群中的数据是如何索引的？集群中shard的数量是多少？”，因而有很多人会问“应该设置多少个shard？每个shard应该设多大？”

下面我们尝试回答这个问题。

## What is shard

先了解一些基本概念

Elasticsearch中数据组织到index中，每个index由一个或多个shard组成，每个shard是一个能够在该shard的数据块上独立提供查询服务的Lucene索引。

数据写入shard时，会定期写到一个新的Lucene segment，此时该数据就可以被客户端查询了。当segment数量开始增长，会定期合并成更大的segment，称为merge。合并过程中可能会有磁盘空间的变化，合并未完成之前，旧的segment仍然不会被删除，合并过程磁盘空间占用会升高，但合并完成后，旧的segment就会被删除，磁盘占用就会降下来。

Shard是集群结点之间分布数据的最小单位。当集群有结点下线时，会导致shard的在结点之间的移动。此时的传输速度取决于shard的数量和大小，同时也要看网络和磁盘的性能。

> 要避免shard过大，这会影响集群故障恢复的能力。shard的大小没有固定的限制，但大多数场景下，不要超过50GB

## Index by retention period

Elasticsearch中索引在更新和删除操作时并不会真正的删除数据，而只是标记一下数据的version或标记为deleted。这就导致已删除的数据会一直占用系统资源，直到下一次merge操作。

Elasticsearch允许直接从文件系统删除整个索引而无须单独标记deleted。这是目前为止最有效的删除数据的方式。

使用time-based indices可以将数据保留任意时间，数据可以根据保留区间进行分组索引。这也可以很容易基于时间修改primary shards和relicas的数量，因为在下一个索引生成前这些参数是可以改的。

