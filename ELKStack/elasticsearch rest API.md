# elasticsearch rest API

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
