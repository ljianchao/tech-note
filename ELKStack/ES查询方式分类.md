# ES查询方式分类

## 基本查询

简单查询这一类可包括：match, multi_match, common, fuzzy_like_this, fuzz_like_this_field, geoshape, ids, match_all, query_string, simple_query_string, rang, prefix, regexp, span_term, term, terms, wildcard等

### match查询

适用于执行全文检索且需要对输入进行分析的场景。

### match_all查询

这个查询匹配所有文档，常用于需要对所有索引内容进行归类处理的场景。

### term查询

一种简单的，无需对输入进行分析的查询方式，可以查询单个词项。经常跟过滤器配合使用。



## 组合查询

组合查询的唯一用途是把其他查询组合在一起使用。

组合查询包括：bool, boosting, constant_score, dis_max, filterd, function_score, has_child, has_parent, indices, nested, span_first, span_multi, span_first, span_multi, span_near, span_not, span_or, span_term, top_children等。

### bool查询

能够把多个查询用布尔逻辑组织在一起，可以控制查询的某个子查询部分是必须匹配，可以匹配还是不应该匹配。bool查询适用的场景-希望文档的最终得分为所有子查询得分的和。

- must：写在这个从句里面的条件必须匹配上，才能返回文档

- should：写在should从句中的查询条件可能被匹配上，也可能不匹配，但如果bool查询中没有must从句，那就至少要匹配上一个should条件，文档才会返回。

- must_not：这个从句中的条件一定不能被匹配上

- filter：写在这个从句中的查询条件必须被选中的文档匹配上，只是这种匹配与评分无关

bool查询的结构如下：

```json
{
    "query": {
        "bool": {
            "must": [{}],
            "should": [{}],
            "must_not": [{}],
            "filter": [{}]
        }
    }
}
```

- boost：这个参数用于控制must或should查询从句的分数

- minimum_should_match：只适用于should从句。限定要返回一份儿文档的话，至少要匹配上多少个should从句

- disable_coord：一般情况下，bool查询会对所有的should从句使用查询协调。这么做通常来说很好，应为匹配上的从句越多，文档得分就越高 

### dis_max查询

这种查询的文档得分结果和最高权重的子查询得分高度相关。如果希望最高得分子查询能够在打分过程中起决定作用，dis_max查询是不二选择。

## 无分析查询

这一类查询不会被分析，而是被直接传递给Lucene索引。这类查询会精确匹配传入的词语，不会使用语言分析器等工具对词语进行分词和其他处理。

这类查询包括：common, ids, prefix, span_term, term, terms, wildcard等。

- term查询

即词项查询。匹配某个字段中的给定值。

- prefix查询

即前缀查询。前缀查询常用于自动完成功能。

## 全文检索查询

这类查询会根据索引映射配置对输入进行分析，支持lucene查询语法和打分计算等功能。

这类查询包括：query_string, match, multi_match, simple_query_string等。

- simple_query_string查询

通常情况下，如果不希望在遭遇继续错误时直接失败，而是想要尝试给出用户期望的答案，那么这种查询方式是不错的选择。

## 模式匹配查询

本类查询包括：prefix, regexp, wildcard等。

## 支持相似度操作的查询

本类别查询包括：fuzzy_like_this, fuzzy_like_this_field, fuzzy, more_like_this, more_like_this_field等。

可以使用本类别下的某个查询来查找包含与给定输入内容近似的此项或字段的文档。

## 支持修改得分的查询

这是一组用于改善查询精度和相关度的查询方式，通过指定自定义权重因子或提供额外处理逻辑的方式来改变文档得分。

本类别查询包括：boosting, constant_score, function_score, indices等。

## 位置敏感查询

匹配特定词项，以及匹配词项的位置信息。

本类别查询包括：match_phrase, span_first, span_mutil, span_near, span_not, span_or, span_term等。

## 结构敏感查询

本类别查询包括：nested, has_child, has_parent, top_children等。







