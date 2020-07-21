# kibana 7.8.0安装

## 下载

地址

```
    https://artifacts.elastic.co/downloads/kibana/kibana-7.8.0-linux-x86_64.tar.gz
```

## 设置用户和权限

### 赋予权限

```
    chown -R elastic kibana-7.8.0-linux-x86_64
```

## 启动kibana

### 修改配置文件

修改`kibana.yml`

```
server.port: 5601
server.host: "192.168.255.128"
server.name: "youlin-kibana"
elasticsearch.hosts: ["http://192.168.255.128:9200"]
# xpack.reporting.capture.browser.chromium.disableSandbox: false
elasticsearch.username: "kibana_system"
elasticsearch.password: "123456"
```

### 启动服务

终端以交互方式启动es进程

```
bin/kibana
```

守护进程方式启动es进程

```
nohup bin/kibana &
```
