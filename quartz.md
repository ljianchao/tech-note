# quartz

## 属性文件

quartz使用名为`quartz.properties`的配置文件

基础配置

```properties
org.quartz.scheduler.instanceName=MyScheduler
# 线程池中有3个线程，这意味着最多可以同时运行3个job
org.quartz.threadPool.threadCount=3
# quartz的所有数据，包括job和trigger的配置，都会存储在内存中（而不是数据库里）
org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
```

## 基本实现

需要三个对象Scheduler、JobDetail、Trigger

## 集群和线程池配置文件

项目配置文件`application.yml`内容

```yml
server:
    port: 8115

spring:
    jackson:
        time-zone: GMT+8
    application:
        name: youlin-scheduler-job
    quartz:
        #数据库方式
        job-store-type: jdbc
        properties:
            org:
                quartz:
                    scheduler:
                        instanceId: AUTO
                        instanceName: youlin-scheduler-job
                    jobStore:
                        class: org.quartz.impl.jdbcjobstore.JobStoreTX
                        clusterCheckinInterval: 10000
                        driverDelegateClass: org.quartz.impl.jdbcjobstore.StdJDBCDelegate
                        isClustered: true
                        tablePrefix: QRTZ_
                        useProperties: false
                        dataSource: schedulerDS
                    dataSource:
                        schedulerDS:
                            provider: hikaricp
                            driver: com.mysql.cj.jdbc.Driver
                            URL: jdbc:mysql://192.168.1.123:3309/youlin-scheduler-job?useUnicode=true&characterEncoding=utf8&zeroDateTimeBehavior=convertToNull&serverTimezone=CTT&useSSL=false
                            user: yltapi
                            password: yltest@2018
                            maxConnections: 15
                    threadPool:
                        class: org.quartz.simpl.SimpleThreadPool
                        threadCount: 15
                        threadPriority: 5
                        threadsInheritContextClassLoaderOfInitializingThread: true

dubbo:
    registry:
        protocol: zookeeper
        address: 127.0.0.1:2181
        id: youlin-registry-01
    protocol:
        port: 20895
        name: dubbo
        id: dubbo
        status: server
        threadpool: cached
        threads: 1000
        accepts: 1000
    application:
        qos-enable: true
        name: youlin-dubbo-scheduler-job-name01
        id: youlin-dubbo-scheduler-job-id01
        qos-port: 22235
    scan:
        basePackages: com.youlin365.scheduler.job.task
    consumer:
        check: false
        timeout: 10000
    provider:
        timeout: 10000
youlin:
    service:
        cache:
            group: youlin-core-cache
        version: 1.0.0  # 标识dubbo服务的版本号

youlinSchedulerJob:
    cronExp:
        schoolEventLogJob: "0 0 3 * * ?"    # 每天凌晨3点执行一次
        schoolFoodBookJob: "1 0 0 ? * 1"    # 每周日凌晨执行一次
        userReportJob: "1 0 0 ? * 2"    # 每周一凌晨执行一次
        userReportCreateJob: "1 0 0 ? * 4,5,6,7,1"    # 每周三四五六日凌晨执行一次
        sendSMSNoCookbookJob: "0 0 9 ? * 6"    # 每周五上午9点执行一次
        sendSMSNoCookbookLinkJob: "0 0 9 ? * 7"    # 每周六上午9点执行一次
logging:
    config: classpath:config/log4j2-spring.xml
```

quartz配置文件`quartz.properties`内容

```properties
#调度标识名 集群中每一个实例都必须使用相同的名称
org.quartz.scheduler.instanceName=youlin-scheduler
#调度器实例编号自动生成，每个实例不能不能相同
org.quartz.scheduler.instanceId=AUTO
#开启分布式部署，集群
org.quartz.jobStore.isClustered=true
#分布式节点有效性检查时间间隔，单位：毫秒,默认值是15000
org.quartz.jobStore.clusterCheckinInterval=15000

#远程管理相关的配置,全部关闭
org.quartz.scheduler.rmi.export=false
org.quartz.scheduler.rmi.proxy=false

org.quartz.scheduler.wrapJobExecutionInUserTransaction=false
#实例化ThreadPool时，使用的线程类为SimpleThreadPool（一般使用SimpleThreadPool即可满足几乎所有用户的需求）
org.quartz.threadPool.class=org.quartz.simpl.SimpleThreadPool
##并发个数,指定线程数，至少为1（无默认值）(一般设置为1-100之间的的整数合适)
org.quartz.threadPool.threadCount=5
##设置线程的优先级（最大为java.lang.Thread.MAX_PRIORITY 10，最小为Thread.MIN_PRIORITY 1，默认为5）
org.quartz.threadPool.threadPriority=5
#org.quartz.threadPool.threadsInheritContextClassLoaderOfInitializingThread=true
#容许的最大作业延长时间,最大能忍受的触发超时时间，如果超过则认为“失误”,不管在内存中还是数据中都要配置，默认值为60000
org.quartz.jobStore.misfireThreshold=60000

#持久化方式配置
# 默认存储在内存中，保存job和Trigger的状态信息到内存中的类
#org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
#数据库方式
org.quartz.jobStore.class=org.quartz.impl.jdbcjobstore.JobStoreTX
#持久化方式配置数据驱动，MySQL数据库
org.quartz.jobStore.driverDelegateClass=org.quartz.impl.jdbcjobstore.StdJDBCDelegate
#quartz相关数据表前缀名
org.quartz.jobStore.tablePrefix=QRTZ_

#数据库别名 随便取
#org.quartz.jobStore.dataSource=qzDS
#org.quartz.dataSource.qzDS.driver=com.mysql.jdbc.Driver
#org.quartz.dataSource.qzDS.URL=jdbc:mysql://192.168.184.135:3306/quartzdb?useSSL=false&useUnicode=true&characterEncoding=UTF-8
#org.quartz.dataSource.qzDS.user=root
#org.quartz.dataSource.qzDS.password=123456
#org.quartz.dataSource.qzDS.maxConnections=10
#org.quartz.dataSource.qzDS.acquireIncrement=1
```
