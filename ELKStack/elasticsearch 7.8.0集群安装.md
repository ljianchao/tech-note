# elasticsearch 7.8.0集群安装

## 开发机运行多个Elasticsearch实例

默认配置下使用以下命令启动三个节点：

```
bin/elasticsearch -E cluster.name=demo -E node.name=demo-node1  -E path.data=node1_data -E http.host=0.0.0.0 -E http.port=9201 -d
bin/elasticsearch -E cluster.name=demo -E node.name=demo-node2  -E path.data=node2_data -E http.host=0.0.0.0 -E http.port=9202 -d
bin/elasticsearch -E cluster.name=demo -E node.name=demo-node3  -E path.data=node3_data -E http.host=0.0.0.0 -E http.port=9203 -d
```

查看节点信息:

```
// 查看节点信息
GET /_cat/nodes
```

