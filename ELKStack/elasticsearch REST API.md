# elasticsearch REST API

Elasticsearch-开源分布式搜索分析引擎

- 近实时（Near Real Time）
- 分布式存储/搜索/分析引擎
- 降低全文检索的学习曲线，可以被任何编程语言调用

Elasticsearch 学习的三个目标：

- 开发层面：产品基本功能，底层基本原理，数据建模最佳实践
- 运维层面：容量规划，性能优化，问题诊断，滚动升级
- 解决方案：搜索与如何解决搜索的相关性问题；大数据分析实践与项目实战，理论知识运用到实际场景

Elasticsearch的主要功能：

- 海量数据的分布式存储以及集群管理
  - 服务与数据的高可用，水平扩展
- 近实时搜索，性能卓越
  - 结构化/全文/地理位置/自动完成
- 海量数据的近实时分析
  - 聚会功能

## 插件API

```
// 查看已安装的插件
GET /_cat/plugins
```

## 分词器API

分词器（analyzer）是专门处理分词的组件，由三部分组成

- Character Fiters：针对原始文本处理，例如去除html标签
- Tokenizer：按照规则切分为单词
- Token Filter：将切分的单次进行加工，小写，删除stopwords，增加同义词

Elasticsearch内置分词器

- Standard Analyzer：默认分词器，按词切分，小写处理，Stop（默认关闭）
- Simple Analyzer：按照非字母切分（符号被过滤），小写处理
- Stop Analyzer：小写处理，停用词过滤（the, a, is）
- Whitespace Analyzer：按照空格切分，不转小写
- Keyword Analyzer：不分词，直接将输入当中输出
- Pattern Analyzer：正则表达式，默认`\W+`（非字符分割）
- Language：提供了30多种常见语言的分词器，english, icu_analyzer
- Customer Analyzer：自定义分词器

```
// 直接指定Analyzer进行测试
GET /_analyze
{
  "analyzer": "standard",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}

// 指定索引的字段进行测试
POST books/_analyze
{
  "field": "title",
  "text": "2 running Quick brown-foxes leap over lazy dogs in the summer evening."
}

// 自定义分词进行测试
POST /_analyze
{
  "tokenizer": "standard",
  "filter": ["lowercase"],
  "text": "Mastering Elasticsearch"
}

```

## Search API

Search API分为两种：

- URI Search：在URL中使用查询参数
- Request Body Search：使用Elasticsearch提供的，基于JSON格式的更加完备的DSL（Query Domain Specific Language）

索引基本语句：

```
// 集群上所有的索引
/_search

// 指定索引查询
/index1/_search

// 多个索引查询
/index1,index2/_search

// 以index开头的索引
/index*/_search
```

URI Search，使用"q"指定查询字符串，"query string syntax"，KV键值对

```
curl -XGET "http://localhost:9200/index1/_search?q=name:Eddie"
```


Request Body Search

```
curl -XGET "http://localhost:9200/index1/_search" -H 'Content-Type:application/json' -d '
{
  "query": {
    "match_all": {}
  }
}
'
```
