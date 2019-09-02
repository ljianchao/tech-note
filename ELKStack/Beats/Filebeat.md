# Filebeat

## Install Filebeat

下载和解压

mac版本
```shell
    curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-6.3.2-darwin-x86_64.tar.gz
    tar xzvf filebeat-6.3.2-darwin-x86_64.tar.gz
```

linux版本
```shell
    curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-6.3.2-linux-x86_64.tar.gz
    tar xzvf filebeat-6.3.2-linux-x86_64.tar.gz
```

## Configure Filebeat

## 启动

启动命令：
```shell
    sudo ./filebeat -e -c filebeat.yml
```

