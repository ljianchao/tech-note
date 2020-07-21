# logstash 7.8.0安装

## 安装插件

在网站`rubygems.org`下载插件`logstash-output-elasticsearch-10.5.1-java.gem`，然后压缩为zip文件

```
    bin/logstash-plugin install file:///opt/soft/logstash-output-elasticsearch-10.5.1-java.zip
```

## 启动logstash

在config目录下增加logstash-mysql.conf配置文件

```conf
input {
  jdbc {
    jdbc_driver_library => "/opt/soft/logstash-7.8.0/config/mysql-connector-java-8.0.20.jar"
    jdbc_driver_class => "com.mysql.cj.jdbc.Driver"
    jdbc_connection_string => "jdbc:mysql://192.168.1.221:3306/demo?useUnicode=true&characterEncoding=utf8&zeroDateTimeBehavior=convertToNull&serverTimezone=CTT&useSSL=false&rewriteBatchedStatements=true"
    jdbc_user => "root"
    jdbc_password => "123456"
    schedule => "* * * * *"
    clean_run => true
    statement => "SELECT * FROM demo"
  }
}

output {
  elasticsearch {
    hosts => ["http://127.0.0.1:9200"]
    index => "demo"
    document_id => "%{demo_id}"
    #user => "elastic"
    #password => "changeme"
  }
}

```

运行启动命令

```
bin/logstash -f config/logstash-mysql.conf
```
