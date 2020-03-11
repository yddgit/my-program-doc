# Redis

Redis是一个key-value的数据库

## 安装（CentOS 7）

```sh
# 下载安装包
wget http://download.redis.io/releases/redis-4.0.8.tar.gz
# 解压
tar xzf redis-4.0.8.tar.gz
cd redis-4.0.8
# 编译时指定MALLOC变量，具体参考README中Allocator一节的内容
make MALLOC=libc
# 编译完成之后提示执行make test进行测试，需要先安装tcl
yum install tcl
# 测试验证
make test
# 启动redis服务
src/redis-server redis.conf
# 启动redis客户端
src/redis-cli
```

修改redis.conf的如下配置项

```sh
# 指定redis端口绑定的IP的地址
bind 192.168.56.15
# redis默认以daemon方式启动，需要修改以下配置
daemonize yes
```

给redis设置密码

```sh
# 使用如下方式连接redis
src/redis-cli -h 192.168.56.15 -p 6379
# 连接后执行如下命令设置密码
config set requirepass <password>
# 退出客户端再次连接，执行ping命令会提示Authentication required
ping
# 此时执行auth命令验证密码
auth <password>
# 验证成功后，再次执行ping命令会显示PONG
ping
```

## 常用命令

```sh
# http://redis.io/commands

# 基本使用
SET <key> <value>
GET <key>
SETNX <key> <value> // SET-if-not-exists
INCR <key> // SET key = key + 1, 这是一个原子操作
DEL <key>

# 指定key的存活时长
EXPIRE <key> <seconds> // key将会在指定的秒数之后被删除
TTL <key> // 查看key还有多久会被删除，-2表示key不存在，-1表示key没有设置超时时间

# List: RPUSH/LPUSH/LLEN/LRANGE/LPOP/RPOP
RPUSH friends "Alice"
RPUSH friends "Bob"
LPUSH friends "Sam" // 从列表头部插入元素
LRANGE friends 0 -1 // 获取列表从0开始的所有元素
LRANGE friends 0  1 // 获取列表从0到1位置的元素
LRANGE friends 1  2 // 获取列表从1到2位置的元素
LLEN friends // 列表长度
LPOP friends // 从列表头部取出一个元素
RPOP friedns // 从列表尾部取出一个元素

# Set: SADD/SREM/SISMEMBER/SMEMBERS/SUNION
SADD superpowers "flight"
SADD superpowers "x-ray vision"
SADD superpowers "reflexes" // 向set中添加元素
SREM superpowers "reflexes" // 从set中移除元素
SISMEMBER superpowers "reflexes" // 判断元素是否存在，不存在返回0，存在返回1
SMEMBERS superpowers // 获取set中所有元素
SADD birdpowers "pecking"
SADD birdpowers "flight"
SUNION superpowers birdpowers // 合并两个set，返回合并后的set

# Redis 1.2+ Sorted Set: ZADD/ZRANGE
ZADD hackers 1940 "Alan Kay" // 每个元素均关联一个数字用于排序
ZADD hackers 1906 "Grace Hopper"
ZADD hackers 1953 "Richard Stallman"
ZADD hackers 1965 "Yukihiro Matsumoto"
ZADD hackers 1916 "Claude Shannon"
ZADD hackers 1969 "Linus Torvalds"
ZADD hackers 1957 "Sophie Wilson"
ZADD hackers 1912 "Alan Turing"
ZRANGE hackers 2 4

# Hashes: HSET/HGETALL/HMSET/HGET
HSET user:1000 name "John Smith" // 设置对象属性
HSET user:1000 email "john.smith@example.com"
HSET user:1000 password "secret"
HGETALL user:1000 // 获取对象所有属性
HMSET user:1001 name "Mary Jones" password "hidden" email "mjones@example.com" // 一次设置多个属性值
HGET user:1001 name // 获取对象某个属性
HSET user:1000 visits 10 // 数值类型的对象属性与普通的数据类型值相同
HINCRBY user:1000 visits 1 // 对数值类型的值可以进行自增的原子操作，自增1
HINCRBY user:1000 visits 10 // 自增10
HDEL user:1000 visits // 删除指定对象的指定属性
HINCRBY user:1000 visits 1 // 自增操作时若属性不存在则自动创建
# Hashes的其他命令: http://redis.io/commands#hash

# Redis数据类型: https://redis.io/topics/data-types-intro

# 查看所有keys: https://redis.io/commands/keys
KEYS *
# 查看指定模式的key
KEYS *name*
# 清空所有数据: https://redis.io/commands/flushall
FLUSHALL
```

