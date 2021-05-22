# 基础

# 集合类

# JVM 内存 & GC 

# JVM 类加载




# 多线程

## 线程的实现
windows和Linux上使用的是轻量级进程，本质上是内核级线程。使用一对一的模型，一条java线程对应一条系统线程。  
java线程是抢占式的线程，有优先级。但是优先级不靠谱，windows中线程有自己的优先级，可能不会按照java中的优先级运行。  

## ThreadLocal
ThreadLoacl 有一个静态内部类 ThreadLocalMap，其 Key 是 ThreadLocal 对象，值是 Entry 对象，Entry 中只有一个 Object 类的 vaule 值。ThreadLocal 是线程共享的，但 ThreadLocalMap 是每个线程私有的。ThreadLocal 主要有 set、get 和 remove 三个方法。   
- 脏数据：线程池线程复用，得到前一个没有销毁的Threadlocal
- 内存泄漏:Threadlocal是弱引用，entry的value是强引用。

## ThreadPool
七个参数：
```java
 public ThreadPoolExecutor(int corePoolSize,//核心线程数
                              int maximumPoolSize,//最大线程数
                              long keepAliveTime,//非核心线程数等待时间，和阻塞队列配和使用，到时间后直接回收
                              TimeUnit unit,//等待时间的单位
                              BlockingQueue<Runnable> workQueue//阻塞的队列 aquire(time)
                              ThreadFactory threadFactory,//线程的工厂类
                              RejectedExecutionHandler handler//拒绝策略
                            )
//拒绝策略：
/*
1.AbortPolicy:丢弃任务并抛出RejectedExecutionException异常。 
2.DiscardPolicy：丢弃任务，但是不抛出异常。 
3.DiscardOldestPolicy：丢弃队列最前面的任务，然后重新提交被拒绝的任务。
4.CallerRunsPolicy：由调用线程（提交任务的线程）处理该任务
*/

//关闭线程池
/*
1.shutdown 将状态设置为STOP中断没有正在执行的线程
2.shutdownNow 将状态设置成SHUTDOWN 中断正在执行或者暂停任务的线程，返回等待执行任务的列表
*/
```

# JAVA并发
## Sychronized详解  
#### 0.java对象内存布局（sychronized对对象头进行操作）  
***普通对象***  
markword 32位为4字节 64位为8字节  
classpointer 类指针 32位为8字节 64位为8字节  
instancedata 实例数据  
padding 对齐（被8整除）  
***数组对象***  
markword 32位为4字节 64位为8字节  
classpointer 类指针 32位为8字节 64位为8字节  
length 长度4字节  
instancedata 实例数据  
padding 对齐（被8整除）    
####　1.本质moniter （重量级锁）
编译成字节码：moniterenter  moniterexit  
重量级锁的对象头保存的是moniter的指针和重量级锁状态。  
moniter管程本质：(ObjectMonitor.hpp)  
- 可重入（有重入次数的计数）
- 维护等待队列 非公平锁（会唤醒所有等待队列的线程进行竞争）
- 底层调用操作系统的锁（涉及系统调用即）
- 非公平锁 因为当一个线程释放锁而且要唤醒一个等待队列的线程时候，外部来了一个线程可以竞争到锁，造成不公平。
```C++
class ObjectMonitor {
...
  ObjectMonitor() {
    _header       = NULL; //markOop对象头
    _count        = 0;    
    _waiters      = 0,   //等待线程数
    _recursions   = 0;   //线程重入次数
    _owner        = NULL;  //获得ObjectMonitor对象的线程
    _WaitSet      = NULL;  //持锁对象调用锁对象wait()方法，将自己封装成一个等待节点放入这个set中，然后唤醒一个等待节点，用park将自己挂起(会释放锁)，直到其他线程通过notify，会将它从waitset放到竞争队列。 
    _cxq          = NULL ;	// 单向列表
    _EntryList    = NULL ; //处于等待锁BLOCKED状态的线程
  }
```
#### 2.1.6之后增加的优化： 
##### 1.偏向锁: 无锁和偏向锁最后两位01 （假定只有一个线程去获取，性能最高） 
***获取锁的步骤***： 
  - 在当前线程栈内添加一个锁记录，锁记录里的锁标识指向锁对象。
  - 通过CAS设置锁对象的markword存储当前线程地址    
***锁释放步骤***：
拿到当前线程栈内的所有的锁记录，释放最后一条，检查锁对象是偏向状态就什么都不做，意味着就算是释放了，锁对象还是保留了偏向状态。下次该线程再次进入的时候，对比一下如果偏向自己，就不需要CAS。

##### 2.轻量级锁：（假定没有竞争，线程之间交替运行）
***获取锁的步骤***  
在当前线程栈内添加一个锁记录，锁记录里的锁标识指向锁对象。锁对象生成一条无锁状态的markword,displacedmarkword，让线程栈内的锁记录保存这个markword,用CAS将对象头设置成持有轻量级锁的状态. 
锁重入:执行上述步骤,CAS失效,检查markword指向当前线程空间,判断是锁重入,将空的displacedmarkworod加入线程栈的当前锁记录,通过锁记录的条数记录锁重入次数.否则就是锁升级.    
***释放锁的步骤***  
找到锁记录,将锁记录指针设置为null,判断displacedmarkword是空就直接释放.
非空,用CAS将displacedmarkword放入对象头中。释放完。

##### 3.锁升级
- 从偏向锁到轻量级锁：CAS失败，markword不指向当前线程。提交一个任务给VM，VM到Safepoint检查当前锁是不是被占有（查看栈帧有无锁记录），没有的话就执行轻量级锁获取锁的步骤。
- 从轻量级锁到重量级锁：CAS失败，当前锁是轻量级锁，锁会获取一个空闲管程对象，再通过CAS将锁对象设置成正在膨胀状态，如果失败说明正在膨胀或者膨胀结束，再次自璇获取就可以了。CAS成功,将管程的owner设置成持锁线程,再将持锁线程的第一条锁记录存在管程里.

##### 锁升级的集中情况:
- hashcode方法被调用.
- 持锁对象执行wait方法,需要一个队列维护.
- 发生并发竞争  

## CAS
判断内存地址的值是不是期望值 lock锁住总线,多核CPU时候极大影响性能。
逻辑 : 获取一个值 再用这个值和原来的值比较如果相同则更新.  
ABA问题:AtomicMarkableReference AtomicStampedReference加时间戳和加标志位

## Locksupport
https://blog.csdn.net/hengyunabc/article/details/28126139  
park/unpark实现原理：互斥锁加上条件变量实现。维护counter值，作为状态，CAS判断counter是否>0即可用。不可用则等待在条件变量上。  
互斥锁和条件变量合作使用，互斥锁保证每次只有一个线程来改变条件变量。条件变量是通知模式。


## AQS 
内部维护了FIFO双向队列  
voliate 修饰的state标志是否持有锁  
死循环+AQS用来实现同步  
- 独占模式  
线程获取同步状态失败后会用CAS加入这个队列的队尾，判断前驱节点是不是有效的节点，有效的节点signal=1,这样他就可以阻塞了，因为前驱节点会唤醒它。如果前驱节点是头节点，标识当前节点是队列的第一个等待Node，它就会自旋不断获取状态标志的值。  
- 共享模式  
tryAcquireShared()方法，这个方法自己检查上下文是否支持获取共享锁，如果支持再进行获取。
返回值小于0获取失败，进入等待队列，等于零获取成功但是无法唤醒后继节点，大于0标识获取成功并且可以唤醒后继的所有节点。

## Lock
## Atomic
## CountdownLatch/CyclicBarrier/Semaphore
## LongAdder


## Hashmap(线程不安全)


## ConcurrentHashmap
- 结构：数组+链表+红黑树 Node结构（key,value,next,hash）  
***hash值一般大于0***红黑树的代理节点treebinhash值是-2，扩容时标志节点hash值是-1.  
- 负载因子：0.75final修饰无法修改 但是普通的hashmap可以修改
- sizectr int
  - -1表示散列表在初始化（懒加载） 
  - >0表示下次触发的阈值
  - <0但是不等于-1表示正在扩容，高16位是扩容标识，低16位标识参与扩容线程+1
- 寻址算法key的hashcode进行扰动,让高16位和低16位异或参与寻址.最后&容量-1,增强散列性
- 保证写数据安全 sychronized锁住桶节点保证桶内的线程安全 slot是空的，用CAS写.
- 长度size LongAdder 不用Atomiclong可能因为性能不行
- 扩容  
  - 扩容线程设置sizectr,算标识戳.创建一个新table,把引用放在map.nexttable中.map.transferindex记录迁移记录,此时设置位老表长度(迁移从高位桶开始).
  - 迁移完的桶 forwordingNode表示这个节点被迁移了,同时会提供一个指向新表的方法,到新表去查询.
  - 扩容时遇到写操作  没有迁移的桶直接CAS或者sychronized直接写,写迁移后的数据,需要协助扩容
  - 最后一个退出扩容任务的线程 更新sizectr低16位减一,结果等于1说明是最后一个线程.他会检查老表是不是都是fwd节点.算出下次扩容阈值更行sizectr
- Treebin的读写问题:
  - 正在读,此时有写操作:检查state字段是不是0,是0CAS设置为1.不是0,让state第二位变1,把自己线程的引用给到Treebin中,将自己挂起.等到最后一个读线程结束后,唤醒.
  - 正在写,此时有读操作state值为1加了写锁,treebin保留了链表,可以直接到链表上查询.