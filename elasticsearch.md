# Elasticsearch

Elasticsearch是一个高度可扩展（highly scalable）的全文检索和分析引擎。它可用于大数据的存储，并以接近实时（NRT, Near Realtime）的速度进行检索和分析。

常见的使用场景示例：

1. 在线商品search或autocomplete，ES存储所有商品目录和库存信息

2. 挖掘日志或交易数据，获得一些统计、趋势等信息。这种场景可以使用logstash组件将数据feed到ES

3. 做价格提醒，如用户可以指定“当某个指定的商品价格低于$X时提醒他”，此时可以收集价格信息到ES中，然后用ES的反向检索（reverse-search）特性Percolator来匹配查询推送提醒
4. 需要在大量数据上做快速调查、分析、可视化、解决特定问题，可以用ES存储数据，用Kibana建立自定义的dashbord进行可视化展现。也可以使用聚合功能做一些复杂的BI查询

## Elasticsearch中的一些基本概念

### Cluster

Cluster必须有一个唯一的name，默认是elasticsearch，每个node只能属于一个cluster。

Cluster Name示例：logging-dev, logging-stage, logging-prod

### Node

存储数据、提供索引和查询能力，name默认是启动时分配的一个UUID标识，也可自定义，通过指定cluster name将node加入某个集群。单独机器启动时默认新建一个单结点集群（cluster name默认为elasticsearch）。

### Index

必须指定name，全小写，这个name用于search、update、delete该索引上的文档。

### Type

每个Index上可以定义Type，可根据需要命名，如：user/blog/comments

### Document

JSON格式，虽然物理上存储在index中，但必须关联index中的一个type。

### Shards & Replicas

一个index可能很大，一个结点存不了或者会响应太慢，ES可以将index分片，每个分片可以存储在不同的Node上，提供完整的index功能：

1. 创建索引时可以指定shards数量
2. 可以水平分割，分Partition存储
3. 可以垂直分割，允许跨shards的并行操作，能够提高性能和吞吐
4. shard的分布机制以及如何聚合搜索结果由ES完成，对用户是透明的
5. 为了保证高可用，ES会存储shards的复本，称为Replicas
6. index创建后，可以指定replicas的数量，但不能修改shards的数量
7. ES默认每个index分为5个shard，每个shard存储1份replicas，即两结点的集群每个index默认会有10个shard
8. 每个shard是一个Lucene Index，限制大小：Integer.MAX_VALUE - 128 = 2,147,483,519
9. 查看shards使用API：http://ip.addres:port/_cat/shards

## Elasticsearch的配置

配置文件：config/elasticsearch.yml

所有配置项参考官方Settings，均可通过配置文件或命令行配置。对于相同的配置项，命令行配置会覆盖配置文件中的配置。命令行参数均以-E开头，如：-Ecluster.name=my_cluster -Enode.name=node_1

大部分配置项都可以在集群运行过程中在线更改，参考官方Cluster Update Settings API。配置文件中只需要配置结点为了加入集群而必须要提前配置的项即可，类似node.name、cluster.name和network.host等。

## Elasticsearch的目录结构

|目录|描述|默认值|配置参数
|-------|-----------|---------|-----|
|home|ES的安装目录|$ES_HOME|-|
|bin|elasticsearch (用于启动node)，elasticsearch-plugin (用于安装插件)|$ES_HOME/bin|-|
|conf|配置文件路径，包含elasticsearch.yml等配置文件|$ES_HOME/conf|path.conf|
|data|数据文件路径（存储每个索引分配到该结点上的分片数据），可配置多个值|$ES_HOME/data|path.data|
|logs|日志文件路径|$ES_HOME/logs|path.logs|
|plugins|插件路径，每个插件分别存放在单独的子目录中|$ES_HOME/plugins|-|
|repo|Shared file system repository locations. Can hold multiple locations. A file system repository can be placed in to any subdirectory of any directory specified here|-|path.repo|
|script|Location of script files.|$ES_HOME/scripts|path.scripts|

## Elasticsearch的REST API

Elasticsearch提供的REST API可以进行如下操作：

1. 检查集群、结点、索引的状态信息
2. 管理集群、结点、索引的数据和元数据
3. 对索引执行CRUD和search操作
4. 执行高级检查功能：分页、排序、过滤、聚合等

示例：

Cluster Health: `GET http://ip.address:port/_cat/health?v`

Node Info: `GET http://ip.address:port/_cat/nodes?v`

List All Indices: `GET http://ip.address:port/_cat/indices?v`

Index Operation:

```bash
# 基本操作：<REST Verb> /<Index>/<Type>/<ID>
PUT /customer
PUT /customer/external/1
{"name": "John Doe"}
GET /customer/external/1
DELETE /customer

# 向同一个URL PUT不同的数据，可以更新替换doc数据
# 如：更新ID=1的doc的name字段，同时增加一个age字段
POST /customer/external/1/_update?pretty
{ "doc": { "name": "Jane Doe", "age": 20 } }
# 使用script更新age，如下的ctx._source表示当前将要被修改的文档对象
POST /customer/external/1/_update?pretty
{ "script" : "ctx._source.age += 5" }
# 每次Update只能更新一个doc，后续会提供类似SQL的where语句一样的批量更新的功能

# 也可不指定doc的ID，由ES随机生成doc id
POST /customer/external?pretty
{"name": "Jane Doe"}

# 删除某个文档
DELETE /customer/external/2?pretty
# 如果要删除全部文档，更有效的方式是直接删除整个index
```

Bulk API:

```bash
# 批处理-添加两个文档
POST /customer/external/_bulk?pretty
{"index":{"_id":"1"}}
{"name": "John Doe" }
{"index":{"_id":"2"}}
{"name": "Jane Doe" }

# 批处理-更新文档1删除文档2
POST /customer/external/_bulk?pretty
{"update":{"_id":"1"}}
{"doc": { "name": "John Doe becomes Jane Doe" } }
{"delete":{"_id":"2"}}
# 批处理遇到某个文档出错会继续执行，最后给出执行结果
```

Search API:

```bash
GET /bank/_search?q=*&sort=account_number:asc&pretty

GET /bank/_search
{
  "query": { "match_all": {} },
  "from": 10,
  "size": 10
  "sort": [
    { "account_number": "asc" }
  ],
  "_source": ["account_number", "balance"] // 返回指定字段
  // "sort": { "balance": { "order": "desc" } }
}
```

Match Query:

```bash
GET /bank/_search
{
  "query": { "match": { "account_number": 20 } }
  // "query": { "match": { "address": "mill" } } // address中包含mill的doc
  // "query": { "match": { "address": "mill lane" } } // address包含mill或lane的doc
  // "query": { "match_phrase": { "address": "mill lane" } } // address中包含mill lane的doc
}
```

Bool Query:

```bash
GET /bank/_search
{
  "query": {
    "bool": {
      "must": [ // address中同时包含mill和lane的doc
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}

GET /bank/_search
{
  "query": {
    "bool": {
      "should": [ // address中包含mill或lane的doc
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}

GET /bank/_search
{
  "query": {
    "bool": {
      "must_not": [ // address中既不含mill也不含lane的doc
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}

# 以上must/should/must_not可以在bool查询中混用
GET /bank/_search
{
  "query": {
    "bool": {
      "must": [
        { "match": { "age": "40" } }
      ],
      "must_not": [
        { "match": { "state": "ID" } }
      ]
    }
  }
}
```