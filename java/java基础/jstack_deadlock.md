"Thread-3" #15 prio=5 os_prio=0 tid=0x000000001f1fa000 nid=0x4d48 waiting on condition [0x000000001fd0f000]
   java.lang.Thread.State: WAITING (parking)
        at sun.misc.Unsafe.park(Native Method)
        - parking to wait for  <0x000000076f900178> (a java.util.concurrent.locks.ReentrantLock$NonfairSync)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:175)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.parkAndCheckInterrupt(AbstractQueuedSynchronizer.java:836)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.doAcquireInterruptibly(AbstractQueuedSynchronizer.java:897)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.acquireInterruptibly(AbstractQueuedSynchronizer.java:1222)
        at java.util.concurrent.locks.ReentrantLock.lockInterruptibly(ReentrantLock.java:335)
        at cn.jc.demos.concurrency.lockdemo.ReentrantLockDemo$IntLock.run(ReentrantLockDemo.java:86)
        at java.lang.Thread.run(Thread.java:745)

"Thread-2" #14 prio=5 os_prio=0 tid=0x000000001f171800 nid=0x3778 waiting on condition [0x000000001fc0f000]
   java.lang.Thread.State: WAITING (parking)
        at sun.misc.Unsafe.park(Native Method)
        - parking to wait for  <0x000000076f908178> (a java.util.concurrent.locks.ReentrantLock$NonfairSync)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:175)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.parkAndCheckInterrupt(AbstractQueuedSynchronizer.java:836)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.doAcquireInterruptibly(AbstractQueuedSynchronizer.java:897)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.acquireInterruptibly(AbstractQueuedSynchronizer.java:1222)
        at java.util.concurrent.locks.ReentrantLock.lockInterruptibly(ReentrantLock.java:335)
        at cn.jc.demos.concurrency.lockdemo.ReentrantLockDemo$IntLock.run(ReentrantLockDemo.java:76)
        at java.lang.Thread.run(Thread.java:745)
		
		
Found one Java-level deadlock:
=============================
"Thread-3":
  waiting for ownable synchronizer 0x000000076f900178, (a java.util.concurrent.locks.ReentrantLock$NonfairSync),
  which is held by "Thread-2"
"Thread-2":
  waiting for ownable synchronizer 0x000000076f908178, (a java.util.concurrent.locks.ReentrantLock$NonfairSync),
  which is held by "Thread-3"

Java stack information for the threads listed above:
===================================================
"Thread-3":
        at sun.misc.Unsafe.park(Native Method)
        - parking to wait for  <0x000000076f900178> (a java.util.concurrent.locks.ReentrantLock$NonfairSync)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:175)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.parkAndCheckInterrupt(AbstractQueuedSynchronizer.java:836)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.doAcquireInterruptibly(AbstractQueuedSynchronizer.java:897)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.acquireInterruptibly(AbstractQueuedSynchronizer.java:1222)
        at java.util.concurrent.locks.ReentrantLock.lockInterruptibly(ReentrantLock.java:335)
        at cn.jc.demos.concurrency.lockdemo.ReentrantLockDemo$IntLock.run(ReentrantLockDemo.java:86)
        at java.lang.Thread.run(Thread.java:745)
"Thread-2":
        at sun.misc.Unsafe.park(Native Method)
        - parking to wait for  <0x000000076f908178> (a java.util.concurrent.locks.ReentrantLock$NonfairSync)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:175)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.parkAndCheckInterrupt(AbstractQueuedSynchronizer.java:836)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.doAcquireInterruptibly(AbstractQueuedSynchronizer.java:897)
        at java.util.concurrent.locks.AbstractQueuedSynchronizer.acquireInterruptibly(AbstractQueuedSynchronizer.java:1222)
        at java.util.concurrent.locks.ReentrantLock.lockInterruptibly(ReentrantLock.java:335)
        at cn.jc.demos.concurrency.lockdemo.ReentrantLockDemo$IntLock.run(ReentrantLockDemo.java:76)
        at java.lang.Thread.run(Thread.java:745)

Found 1 deadlock.

		