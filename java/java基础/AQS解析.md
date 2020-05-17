
# AQS(AbstractQueuedSynchronizer)

## 子类同步器可以重写的方法

``` java
    /**
    * 独占模式获取同步状态
    */
    protected boolean tryAcquire(int arg) {
        throw new UnsupportedOperationException();
    }
    /**
    * 独占模式释放同步状态
    */
    protected boolean tryRelease(int arg) {
        throw new UnsupportedOperationException();
    }
    /**
    * 共享模式获取同步状态
    */
    protected int tryAcquireShared(int arg) {
        throw new UnsupportedOperationException();
    }
    /**
    * 共享模式释放同步状态
    */
    protected boolean tryReleaseShared(int arg) {
        throw new UnsupportedOperationException();
    }
    /**
    * 当前状态是否在独占模式被线程占用，一般该方法表示是否被当前线程所占用
    */
    protected boolean isHeldExclusively() {
        throw new UnsupportedOperationException();
    }
```

## 子类同步器