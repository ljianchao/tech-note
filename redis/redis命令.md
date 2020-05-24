# Redis命令

Redis不区分命令大小写

## key操作

获得符合规则的键名列表

```
    keys pattern，pattern支持glob通配符格式，如keys *
    keys命令会遍历redis中的所有键，不建议在生产环境中使用
```

判断键是否存在

```
    EXISTS key，存在返回1，否则返回0
```

删除键

```
    DEL key [key...]，删除1个或多个键，返回值是删除的键的个数
    使用linux管道删除符合规则的键
    redis-cli KEYS "foo*"| xargs redis-cli DEL
```

获取键值的类型

```
    TYPE key
```

## 密码操作  

查看密码：

```     
    config get requirepass
```

设置密码
```
    config set requirepass 123456
```

密码登录：
```
    redis-3.2.0]# redis-cli -h 127.0.0.1 -p 6379 -a 123456
```