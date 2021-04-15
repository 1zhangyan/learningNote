# 基础

# 集合类

# JVM 内存 & GC 

# JVM 类加载

# 多线程
## ThreadPool







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
***锁释放步骤***：拿到当前线程栈内的所有的锁记录，释放最后一条，检查锁对象是偏向状态就什么都不做，意味着就算是释放了，锁对象还是保留了偏向状态。下次该线程再次进入的时候，对比一下如果偏向自己，就不需要CAS。

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
判断内存地址的值是不是期望值 lock锁住总线.
逻辑 : 获取一个值 再用这个值和原来的值比较如果相同则更新.  
ABA问题:AtomicMarkableReference AtomicStampedReference加时间戳和加标志位


## AQS 
内部维护了FIFO双向队列
voliate 修饰的state标志是否持有锁
独占模式 共享模式
## Reentrandlock
## CountdownLatch/CyclicBarrier/Semaphore
## AtuomaticLong 
## LongAdder
## ConcurrentHashmap
