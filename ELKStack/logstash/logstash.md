## 配置

```
input {
    file {
        path => ["/Users/muzhiye/Documents/GitHub/SpringBootDemo-master/SpringBootDemo/logs/zipkin.log", "/Users/muzhiye/Documents/GitHub/sleuth-webmvc-example/logs/zipkin.log"]
        discover_interval => 5
        start_position => "beginning"
        codec => plain {
            charset => "UTF-8"
        }
    }
}

output {
    kafka {
        topic_id => "zipkin"
        bootstrap_servers => "192.168.49.22:9092"
        codec => plain {
            format => "%{message}"
            charset => "UTF-8"
        }
    }
}

```

## 启动命令

测试服务
```shell
    bin/logstash -e 'input { stdin { } } output { stdout {} }'
```

启动服务
```shell
    bin/logstash -f config/logstash.conf
```