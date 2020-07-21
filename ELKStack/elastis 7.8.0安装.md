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

## 启动elasticSearch

### 修改配置文件

修改`elasticsearch.yml`

```
cluster.name: youlin-es
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

### 安装中文分词插件ik

```
cd your-es-root/plugins/ && mkdir ik
unzip plugin to folder your-es-root/plugins/ik
```

重启elasticsearch

### 安装中文分词插件jieba

```
cp elasticsearch-jieba-plugin-7.8.0.zip ${es.home}/plugins
unzip elasticsearch-jieba-plugin-7.8.0.zip
```

重启elasticsearch

## 添加动态同义词插件

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
                    "ik_syno_max": {
                        "tokenizer": "ik_max_word",
                        "filter": [
                            "youlin_synonym_filter"
                        ]
                    }
                },
                "filter": {
                    "youlin_synonym_filter": {
                        "type": "dynamic_synonym",
                        "synonyms_path": "config/analysis/youlin_synonym.txt",
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
                "analyzer": "ik_max_word",
                "search_analyzer": "ik_syno_max",
                "fields": {
                    "keyword": {
                        "type": "keyword"
                    }
                }
            },
            "alias_names": {
                "type": "text",
                "analyzer": "ik_max_word",
                "search_analyzer": "ik_syno_max",
                "fields": {
                    "keyword": {
                        "type": "keyword"
                    }
                }
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
                            "youlin_synonym_filter"
                        ]
                    }
                },
                "filter": {
                    "youlin_synonym_filter": {
                        "type": "dynamic_synonym",
                        "synonyms_path": "config/analysis/youlin_synonym.txt",
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
                            "youlin_synonym_filter"
                        ]
                    }
                },
                "filter": {
                    "youlin_synonym_filter": {
                        "type": "dynamic_synonym",
                        "synonyms_path": "config/analysis/youlin_synonym.txt",
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







