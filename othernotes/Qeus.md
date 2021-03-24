## Java

### Java基础
- int float short double long char 占字节数？
- int 范围？float 范围？
- hashcode 和 equals 的关系
- 深拷贝、浅拷贝区别
- java 异常体系？RuntimeException Exception Error 的区别，举常见的例子
- lambda 表达式中使用外部变量，为什么要 final？
- static修饰的变量是线程安全的吗，final呢
- System.out.println()是线程安全的吗 （是的，println源码之前用了synchronize(this)修饰，这个原因让效率低了，所以有了logger）。
- 对象头占多大空间？对象除了对象头，由什么组成？
- 强引用、弱引用、虚引用区别？

### Java常用集合类
- Collection 有什么子接口、有哪些具体的实现
- 简单介绍下 ArrayList 怎么实现，加操作、取值操作，什么时候扩容？
- 讲一下 hashMap 原理。hashMap 可以并发读么？并发写会有什么问题？为什么用红黑树？为什么不用平衡树？
- 讲一下 concurrentHashMap 原理。头插法还是尾插法？扩容怎么做？
- 堆是怎么存储的，插入是在哪里？
- 集合在迭代的过程中，插入或删除数据会怎样？
- TreeSet的底层数据结构
- Java collection.sort()的底层实现原理

### Java虚拟机 内存 类加载
- jvm 内存区域分布？gc 发生在哪些部分？如何设置Java堆的大小？
- 介绍一下垃圾回收过程。
- 垃圾回收算法的了解。现在用的什么回收算法？
- 现在使用的什么垃圾回收器？知道哪些？讲讲 G1
- 类加载过程？
- 初始化顺序？
- 怎么实现自己的类加载器？


### Java并发
#### 线程
- 进程和线程的区别？并行和并发的区别？了解协程么？
- 进程间如何通信：进程 A 想读取进程 B 的主存怎么办？
- 线程间通信？（volitaile , object wait和notify , JUC countDownLatch , Reentrantlock 和 condition, locksupport的park和unpark）
- 线程的生命周期有哪些状态？怎么转换？
- wait 和 sleep 有什么区别？什么情况下会用到 sleep？
- 怎么停止线程？（interrupt）
- Java中创建线程的方式(继承thread , 实现runnable接口 , callable接口有返回值, 线程池)
- 线程中能不能抛出异常(线程中是无法抛出异常的 想要处理异常需要UncaughtExceptionHandler )
#### 线程池
- 会用到线程池么？怎么使用的？用什么实现的？
- 常用的线程池有哪些？什么情况下怎么选择？
- ThreadPoolExecutor 有什么参数？各有什么作用？拒绝策略?（int corePoolSize,int maximumPoolSize,long keepAliveTime,TimeUnit unit，BlockingQueue<Runnable> workQueue,ThreadFactory threadFactory,RejectedExecutionHandler handler）
- 一个任务从被提交到被执行，线程池做了哪些工作？
#### JUC (Lock CAS AQS)
- 讲一下锁，有哪些锁，有什么区别，怎么实现的？死锁条件?
- ReentrantLock 应用场景
- 了解 AQS 么？讲讲底层实现原理 AQS 有那些实现？
- 讲讲 AtomicInteger 的底层实现
- LongAdder实现原理
- CountDownLatch原理 CyclicBarrier原理
#### synchronized and volatile and ThreadLocal
- synchronized如何保证可见性？（https://www.zhihu.com/question/48313299/answer/1166823164）
- volatile 关键字有什么用？怎么理解可见性，一般什么场景去用可见性
- volatile有什么不好的影响 （不能保证变量被有序更改）
- 讲一下 threadLocal 原理？threadLocal 是存在 jvm 内存哪一块的？Threadlocal Map的结构和hashmap有什么不同？Threadlocal内存泄露问题？

### JavaIO相关
- Filechannel 介绍
- Select poll epoll
- mmap sendfile

### 其他零散

## Mysql
- 数据仓库与 mysql 区别？hive 和 mysql 有什么区别？spark 和 hadoop 区别？mapreduce 互相等待，怎么解决？
### 事务
- acid 含义？事务隔离级别？幻读怎么解决的？
- 如何保证原子性？
- 用过 mysql 的锁么？有哪些锁？
- MyISAM、InnoDB 区别？为什么不用 MyISAM？
- mvcc 原理？多版本数据存放在哪？(隐式字段https://www.jianshu.com/p/8845ddca3b23)
- mysql 脏页？(先写在内存中，后台进程再刷新到磁盘，日志先行)
- redo log，undo log？
### 索引
- innodb 的索引结构是什么？什么是聚簇索引？聚簇索引和非聚簇索引插入数据时的区别？字符串类型和数字类型索引的效率？数据类型隐式转换
- b+ 树与 b 树的区别？b+ 树与二叉树区别，优点？为什么不用红黑树？
- 多列索引的结构(最左匹配原则)
- 主键与普通索引的联系？存储上的区别？
### sql
- 索引失效场景？explain执行计划关注那些字段？ref字段了解哪些级别？从索引执行效率具体说说
- sql执行顺序？select From where group by先后顺序?
- join 和 in 怎么选择？有什么区别？
- union 和 union all 有什么区别？怎么选择？
- 怎么处理 sql 慢查询？
- 索引用得不太正常怎么处理？同时有（a，b）和（a，c）的索引，查询 a 的时候，会选哪个索引？
- 分库分表有哪些策略？怎么保证 id 唯一？
- 主键选随机 id、uuid 还是自增 id？为什么？主键有序无序对数据库的影响？
### 主从复制
- 主从复制的过程？复制原理？怎么保证强一致性？

## Redis
- Redis 数据结构、对象，使用场景
- Redis 内存淘汰策略
- 缓存的热点 Key 怎么处理？redis 缓存穿透，怎么避免？
- redis keys 命令有什么缺点
- 主从同步原理，新加从库的过程
- RDB 和 AOF 怎么选择，什么场景使用？
- redis 的 zset 的使用场景？底层实现？为什么要用跳表？
- 怎么实现 redis 分布式锁？(setnx)
- redis和数据库是如何保证一致性的？(双删除)
- redis主从复制（全量复制和部分复制runid和offset http://blog.itpub.net/31545684/viewspace-2213629/）
- redis哨兵机制（心跳机制和选举机制）
- redis脑裂解决方案（限制每个主库至少连接的从库的个数，数据复制和同步的延迟不能超过10秒，否则的话master就会拒绝写请求https://my.oschina.net/lishangzhi/blog/4742868）

## 消息队列
- 用 kafka 做了什么功能？
- kafka 内部原理？工作流程？
- Kafka 怎么保证数据可靠性？
- 怎么实现 Exactly-Once？

## 分布式
- CAP理论和base定理
- CPU 100% 问题排查

## OS
- 虚拟内存的作用
- wc命令是干什么的（统计单词数 字节数）查看隐藏文件 ls -l
- 大端小端规则
- x86是大端还是小端，为什么?
- 死锁的形成和规避?

## Network
### TCP UDP
- tcp 有哪些机制确保可靠性？拥塞控制怎么实现？
- close_wait 太多怎么处理？为什么会出现这种情况？
- 讲讲三次握手，四次挥手
### Http
- http 协议，报文格式？http请求行有什么？状态码有哪些？有哪些请求方法？put、post 实现上有什么区别？
- http referrer字段？（该字段的作用主要是表示链接来源，空表示直接通过浏览器访问，容易伪造）
- http 缓存机制都有哪些？什么是 cdn？header 中涉及到缓存的字段有哪些？头部阻塞如何解决？keepalive 有什么用？http 2 有了解过么，新增了哪些功能，现在用的什么版本？1.1？
  https://blog.csdn.net/a66666_/article/details/104102448 https://blog.csdn.net/weixin_48182198/article/details/107838685?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-5&spm=1001.2101.3001.4242
- cookie session 介绍一下
- Https 原理？对称加密的密钥除了随机数还有什么生成方法？安全性怎么保证？ca证书怎么校验


## 数据结构
- 红黑树知道吗？平衡二叉搜索树的结构？

## 设计问题
- 一个千万的大v，发了微博，怎么通知粉丝(https://redisbook.readthedocs.io/en/latest/feature/pubsub.html)
- 设计一个分布式id生成器，就是说，有很多计算机，从我们需要设计的这个系统里面获得唯一的id，不能出现重复