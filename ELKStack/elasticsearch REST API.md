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

### 衡量相关性

搜索相关性（Information Retrieval）的两个基本概念Precision（精确率，查准率）和Recall（召回率，查全率）

- Precision-尽可能返回较少的无关文档。获取到的**相关档数**占获取到的**总文档数（包括相关与不相关的）**的比例，用百分数表示。
  - Precision = True Positive / 全部返回的结果（True and False Positives）
- Recall-尽量返回较多的相关文档。获取到的**相关记录数**占数据库中**相关的记录总数**的比例，用百分数表示。
  - Recall = True Positive / 所有应该返回的结果（True Positives + False Negtives）

### URI Search详解

```
GET /movies/_search?q=2012&df=title&sort=year:desc&from=0&size=10&timeout=1s
{
  "profile": "true"
}
```

- `q`指定查询语句，使用`Query String Syntax`
- `df`指定默认字段，不指定时，会对所有字段进行查询
- `sort`排序
- `from`和`size`用于分页
- `Profile`可以查看查询是如何被执行的

#### Query String Syntax

- 指定查询 v.s 泛查询
  - q=title:2012 / q=2012
- Term v.s Phrase
  - Beautiful Mind 等效于 Beautiful OR Mind
  - "Beautiful Mind"等效于 Beautiful AND Mind。Phrase查询，还要求前后顺序保持一致。
- 分组与引号
  - title:(Beautiful AND Mind)
  - title:"Beautiful Mind"

- 布尔操作
  - `AND / OR / NOT` 或者 `&& / || / !`
  - `+`表示must，`-`表示must_not

- 范围查询
  - 区间表示：`[]`闭区间，`{}`开区间
- 算数符号
  - year:>2010
  - year:(>2010 && <=2018)
  - year:(+>2010 +<=2018)

- 通配符查询（通配符查询效率低，占用内存大，不建议使用。特别是放在最前面）
  - `?`代表1个字符，`*`代表0或多个字符
- 正则表达式查询
  - title:[bt]oy
- 模糊匹配查询与近似查询
  - title:beautifl~1，可以查询包含beautiful的记录
  - title:"lord rings"~2

```
// 指定默认字段查询
GET /movies/_search?q=2012&df=title
{
  "profile": "true"
}

//指定字段
GET /movies/_search?q=title:2012
{
  "profile": "true"
}

// 泛查询
GET /movies/_search?q=2012
{
  "profile": "true"
}

// Phrase查询
GET /movies/_search?q=title:"Beautiful Mind"
{
  "profile": "true"
}

// Mind为泛查询，需使用分组查询
GET /movies/_search?q=title:Beautiful Mind
{
  "profile": "true"
}

// 分组查询
GET /movies/_search?q=title:(Beautiful Mind)
{
  "profile": "true"
}

// AND操作
GET /movies/_search?q=title:(Beautiful AND Mind)
{
  "profile": "true"
}

// NOT操作
GET /movies/_search?q=title:(Beautiful NOT Mind)
{
  "profile": "true"
}

// 操作符，'%2B'表示'+'
GET /movies/_search?q=title:(Beautiful %2BMind)
{
  "profile": "true"
}

// 范围查询，算数符号写法
GET /movies/_search?q=year:>=1980
{
  "profile": "true"
}

// 通配符查询
GET /movies/_search?q=title:b*
{
  "profile": "true"
}

// 模糊匹配&近似度匹配
GET /movies/_search?q=title:beautifl~1
{
  "profile": "true"
}

// 模糊匹配&近似度匹配
GET /movies/_search?q=title:"Lord Rings"~2
{
  "profile": "true"
}
```

### Request Body Search详解

将查询语句通过HTTP Request Body发送给Elasticsearch。

Query DSL基本语法：

```
GET /movies/_search?ignore_unavailable=true
{
  "profile": "true",
  "_source": ["name", "img", "nutrition", "timestamp"], 
  "from": 0,
  "size": 20, 
  "sort": [
    {
      "timestamp": {
        "order": "desc"
      }
    }
  ], 
  "query": {
    "match_all": {}
  }
}
```

- `_source`：指定要返回的字段，如果`_source`没有存储，那就只返回匹配的文档的元数据；支持使用通配符`_source["name*", "img*"]`；
- `from`和`size`：分页功能，`from`从0开始，默认返回10个结果，获取靠后的翻页成本较高；
- `sort`：排序功能，最好在“数字型”与“日期型”字段上排序，因为对于多值类型或分析过的字段排序，系统会选一个值，无法得知该值。

查询示例

```
// 脚本字段
GET /movies/_search?ignore_unavailable=true
{
  "profile": "true",
  "script_fields": {
    "new_field": {
      "script": {
        "lang": "painless",
        "source": "doc['name'] + params.suffix",
        "params": {
          "suffix": "-app"
        }
      }
    }
  }, 
  "from": 0,
  "size": 20, 
  "sort": [
    {
      "timestamp": {
        "order": "desc"
      }
    }
  ], 
  "query": {
    "match_all": {}
  }
}

// match查询
// explain可以显示具体打分的细节
GET /movies/_search
{
  "explain": true, 
  "query": {
    "match": {
      "name": "Last Day"
    }
  }
}

// 短语搜索match phrase
// query中的词按顺序出现，slop表明中间可以介入的词
GET /movies/_search
{
  "explain": true, 
  "query": {
    "match_phrase": {
      "name": {
        "query": "Song Last Chrismas",
        "slop": 1
      }
    }
  }
}

```

## 索引API

```
// 设置分片和副本
PUT tmdb
{
  "settings": {
    "number_of_shards": 3,
    "numer_of_replicas":1
  }
}
```
