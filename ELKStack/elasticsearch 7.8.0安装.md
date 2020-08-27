# elasticsearch 7.8.0安装

## 下载elasticSearch

下载地址

```
    https://artifacts.elastic.co/downloads/elasticsearch/elasticsearch-7.8.0-linux-x86_64.tar.gz
```

## 设置用户和权限

### 创建elastic用户

```shell
    adduser elastic
    passwd elastic
```

### 赋予权限

```
    chown -R elastic:elastic elasticsearch-7.8.0
```

### 开启防火墙

```
    firewall-cmd --zone=public --add-port=9200/tcp --permanent
    firewall-cmd --zone=public --add-port=5601/tcp --permanent
    firewall-cmd --reload
```

### 修改打开文件和进程的数量及虚拟内存数据量

编辑`/etc/security/limits.conf`

```
* soft nofile 65536
* hard nofile 65536
```

### 增加虚拟内存空间大小

编辑文件`/etc/sysctl.conf`

```
vm.max_map_count=655300
```

然后执行以下命令使其生效

```
sysctl -p
```

## 安装和启动elasticSearch

### JVM配置

修改`config/jvm.options`，配置的建议：

- Xms和Xmx设置成一样
- Xmx不要超过机器内存的50%
- 不要超过30GB - [a-heap-of-trouble](https://www.elastic.co/blog/a-heap-of-trouble)

启动后JVM输出：

```
[elastic@localhost elasticsearch-7.8.0]$ jinfo -flags 8243
Attaching to process ID 8243, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.131-b11
Non-default VM flags: -XX:+AlwaysPreTouch -XX:CICompilerCount=2 -XX:CMSInitiatingOccupancyFraction=75 -XX:ErrorFile=null -XX:GCLogFileSize=67108864 -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=null -XX:InitialHeapSize=134217728 -XX:MaxDirectMemorySize=67108864 -XX:MaxHeapSize=134217728 -XX:MaxNewSize=44695552 -XX:MaxTenuringThreshold=6 -XX:MinHeapDeltaBytes=196608 -XX:NewSize=44695552 -XX:NumberOfGCLogFiles=32 -XX:OldPLABSize=16 -XX:OldSize=89522176 -XX:-OmitStackTraceInFastThrow -XX:+PrintGC -XX:+PrintGCApplicationStoppedTime -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintTenuringDistribution -XX:-RequireSharedSpaces -XX:ThreadStackSize=1024 -XX:+UseCMSInitiatingOccupancyOnly -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseConcMarkSweepGC -XX:+UseFastUnorderedTimeStamps -XX:+UseGCLogFileRotation -XX:+UseParNewGC -XX:-UseSharedSpaces 
Command line:  -Xshare:auto -Des.networkaddress.cache.ttl=60 -Des.networkaddress.cache.negative.ttl=10 -XX:+AlwaysPreTouch -Xss1m -Djava.awt.headless=true -Dfile.encoding=UTF-8 -Djna.nosys=true -XX:-OmitStackTraceInFastThrow -Dio.netty.noUnsafe=true -Dio.netty.noKeySetOptimization=true -Dio.netty.recycler.maxCapacityPerThread=0 -Dio.netty.allocator.numDirectArenas=0 -Dlog4j.shutdownHookEnabled=false -Dlog4j2.disable.jmx=true -Djava.locale.providers=SPI,JRE -Xms128m -Xmx128m -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=75 -XX:+UseCMSInitiatingOccupancyOnly -Djava.io.tmpdir=/tmp/elasticsearch-6984692651824079039 -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=data -XX:ErrorFile=logs/hs_err_pid%p.log -XX:+PrintGCDetails -XX:+PrintGCDateStamps -XX:+PrintTenuringDistribution -XX:+PrintGCApplicationStoppedTime -Xloggc:logs/gc.log -XX:+UseGCLogFileRotation -XX:NumberOfGCLogFiles=32 -XX:GCLogFileSize=64m -XX:MaxDirectMemorySize=67108864 -Des.path.home=/opt/soft/demo/elasticsearch-7.8.0 -Des.path.conf=/opt/soft/demo/elasticsearch-7.8.0/config -Des.distribution.flavor=default -Des.distribution.type=tar -Des.bundled_jdk=true
```

堆配置

```
-Xms128m -Xmx128m
```

垃圾回收器配置

```
-XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=75 -XX:+UseCMSInitiatingOccupancyOnly
```

打印输出配置

```
-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=data -XX:ErrorFile=logs/hs_err_pid%p.log -XX:+PrintGCDetails -XX:+PrintGCDateStamps -XX:+PrintTenuringDistribution -XX:+PrintGCApplicationStoppedTime -Xloggc:logs/gc.log -XX:+UseGCLogFileRotation -XX:NumberOfGCLogFiles=32 -XX:GCLogFileSize=64m -XX:MaxDirectMemorySize=67108864
```

### 修改配置文件

修改`config/elasticsearch.yml`

```
cluster.name: my-es
node.name: node-1
cluster.initial_master_nodes: ["node-1"]

network.host: 0.0.0.0
http.port: 9200
transport.port: 9300
# 配置X-Pack
xpack.license.self_generated.type: basic
http.cors.enabled: true
http.cors.allow-origin: "*"
http.cors.allow-headers: Authorization
xpack.security.enabled: true
xpack.security.transport.ssl.enabled: true
```

### 启动服务

终端以交互方式启动es进程

```
./bin/elasticsearch
```

守护进程方式启动es进程

```
./bin/elasticsearch -d
```

### 设置密码

输入以下命令，为用户设置密码，用户包括：elastic, apm_system, kibana_system, logstash_system, beats_system, remote_monitoring_user

```
bin/elasticsearch-setup-passwords interactive
```

## 安装插件

### 在线安装

在线安装命令

```
// 查看已安装的插件
bin/elasticsearch-plugin list

// 在线安装插件
bin/elasticsearch-plugin install analysis-icu
```

REST API

```
// 查看已安装的插件
GET /_cat/plugins
```

### 离线安装

#### 安装中文分词插件ik

```
cd your-es-root/plugins/ && mkdir ik
unzip plugin to folder your-es-root/plugins/ik
```

重启elasticsearch

#### 安装中文分词插件jieba

```
cp elasticsearch-jieba-plugin-7.8.0.zip ${es.home}/plugins
unzip elasticsearch-jieba-plugin-7.8.0.zip
```

重启elasticsearch

#### 添加动态同义词插件

```
cp dynamic-synonym.zip ${es.home}/plugins
unzip dynamic-synonym.zip
```

重启elasticsearch

## 索引操作

### 创建索引和映射-ik分词

包含同义词的配置

```
PUT http://192.168.255.128:9200/demo_index -d '
{
    "settings": {
        "index": {
            "analysis": {
                "analyzer": {
                    "ik_syno_smart": {
                        "tokenizer": "ik_smart",
                        "filter": [
                            "my_synonym_filter"
                        ]
                    },                  
                    "ik_syno_max": {
                        "tokenizer": "ik_max_word",
                        "filter": [
                            "my_synonym_filter"
                        ]
                    }
                },
                "filter": {
                    "my_synonym_filter": {
                        "type": "dynamic_synonym",
                        "synonyms_path": "config/analysis/my_synonym.txt",
                        "interval": 60
                    }
                }
            }
        },
        "similarity": {
            "my_similarity": {
                "type": "BM25",
                "b": 0.75,
                "k1": 0.6,
                "discount_overlaps": true
            }
        }
    },
    "mappings": {
        "dynamic": true,
        "properties": {
            "name": {
                "type": "text",
                "analyzer": "ik_max_word",
                "search_analyzer": "ik_syno_smart",
                "fields": {
                    "keyword": {
                        "type": "keyword"
                    }
                }
            },
            "alias_names": {
                "type": "text",
                "analyzer": "ik_max_word",
                "search_analyzer": "ik_syno_smart",
                "fields": {
                    "keyword": {
                        "type": "keyword"
                    }
                },
                "similarity": "my_similarity"
            },
            "timestamp": {
                "type": "date"
            }
        }
    }
}
'
```

## 创建索引和映射-jieba分词

包含同义词的配置

```
PUT http://192.168.255.128:9200/demo_index -d '
{
    "settings": {
        "index": {
            "analysis": {
                "analyzer": {
                    "jieba_syno": {
                        "tokenizer": "jieba_search",
                        "filter": [
                            "my_synonym_filter"
                        ]
                    }
                },
                "filter": {
                    "my_synonym_filter": {
                        "type": "dynamic_synonym",
                        "synonyms_path": "config/analysis/my_synonym.txt",
                        "interval": 60
                    }
                }
            }
        }
    },
    "mappings": {
        "dynamic": true,
        "properties": {
            "name": {
                "type": "text",
                "analyzer": "jieba_syno",
                "search_analyzer": "jieba_syno",
                "fields": {
                    "keyword": {
                        "type": "keyword"
                    }
                }
            },
            "alias_names": {
                "type": "text",
                "analyzer": "jieba_syno",
                "search_analyzer": "jieba_syno",
                "fields": {
                    "keyword": {
                        "type": "keyword"
                    }
                }
            }
        }
    }
}
'
```

### 查询yl_food索引语句

```json
GET demo_index/_search
{
  "query": {
    "dis_max": {
      "queries": [
        {
          "bool": {
            "should": [
              {
                "match": {
                  "name": {
                    "_name": "name_match",
                    "query": "张三",
                    "boost": 1.2
                  }
                }
              },
              {
                "prefix": {
                  "name.keyword": {
                    "_name": "name_keyword_prefix",
                    "value": "张三",
                    "boost": 2
                  }
                }
              }
            ]
          }
        },
        {
          "match": {
            "alias_names": {
              "_name": "alias_names_match",
              "query": "张三",
              "minimum_should_match": "70%"
            }
          }
        }
      ]
    }
  },
  "highlight": {
    "pre_tags": [
      "<tag1>",
      "<tag2>"
    ],
    "post_tags": [
      "</tag1>",
      "</tag2>"
    ],
    "fields": {
      "name": {},
      "name_keyword_prefix": {},
      "food_alias_names": {},
      "food_base_names": {},
      "food_base_alias_names": {}
    }
  },
  "size": 30
}
```

### 处理同义词 

关闭索引

```
POST http://192.168.255.128:9200/demo_index/_close
```

配置同义词

```
PUT http://192.168.255.128:9200/demo_index/_settings -d '
{
    "settings": {
        "index": {
            "analysis": {
                "analyzer": {
                    "ik_syno_max": {
                        "tokenizer": "ik_max_word",
                        "filter": [
                            "my_synonym_filter"
                        ]
                    }
                },
                "filter": {
                    "my_synonym_filter": {
                        "type": "dynamic_synonym",
                        "synonyms_path": "config/analysis/my_synonym.txt",
                        "interval": 60
                    }
                }
            }
        }
    }
}
'
```

打开索引

```
POST http://192.168.255.128:9200/demo_index/_open
```

## es api


```
curl http://localhost:9200/

# 查看集群健康状态
curl http://localhost:9200/_cluster/health


# 查看nodes

curl http://192.168.255.128:9200/_nodes

# 查看索引
curl http://192.168.255.128:9200/_stats?pretty

# 查看某个索引结构
curl http://localhost:9200/index_name/_settings

# 查看索引内容
curl http://localhost:9200/index_name/type_name/_search? pretty -d '
{
    "size": 10,
    "query": {
        "match_all": {}
    }
}
'

# 索引某个文档
POST index_name/type_name/index_id?pretty -d '
{
    "name": "张三",
    "alias": "张娃子"
}
'

# 查看某个文档
GET index_name/type_name/index_id?pretty

# 查看分词结果
curl http://localhost:9200/_analyze -d '
{
    "text": "hello world"
}
'

curl http://localhost:9200/_analyze -d '
{
    "analyzer": "ik_syno_max",
    "text": "宫爆鸡丁"
}
'

# 查询语句测试

GET yl_food/_validate/query?explain -d '
{
  "query": {
    "dis_max": {
      "queries": [
        {
          "bool": {
            "should": [
              {
                "match": {
                  "name": {
                    "_name": "name_match",
                    "query": "张三",
                    "boost": 1.2
                  }
                }
              },
              {
                "prefix": {
                  "name.keyword": {
                    "_name": "name_keyword_prefix",
                    "value": "张三",
                    "boost": 2
                  }
                }
              }
            ]
          }
        },
        {
          "match": {
            "alias_names": {
              "_name": "alias_names_match",
              "query": "张三",
              "minimum_should_match": "70%"
            }
          }
        }
      ]
    }
  },
  "highlight": {
    "pre_tags": [
      "<tag1>",
      "<tag2>"
    ],
    "post_tags": [
      "</tag1>",
      "</tag2>"
    ],
    "fields": {
      "name": {},
      "name_keyword_prefix": {},
      "food_alias_names": {},
      "food_base_names": {},
      "food_base_alias_names": {}
    }
  },
  "size": 30
}
'

```

### 插件

列出已安装插件

```
sudo bin/elasticsearch-plugin list
```
