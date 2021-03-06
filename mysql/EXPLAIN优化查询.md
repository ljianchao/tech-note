# EXPLAIN优化查询

## EXPLAIN输出格式

输入格式如下：

```sql
MySQL [nideshop]> explain select * from nideshop_user;
+----+-------------+---------------+------------+--------+---------------+------+---------+------+------+----------+-------+
| id | select_type | table         | partitions | type   | possible_keys | key  | key_len | ref  | rows | filtered | Extra |
+----+-------------+---------------+------------+--------+---------------+------+---------+------+------+----------+-------+
|  1 | SIMPLE      | nideshop_user | NULL       | system | NULL          | NULL | NULL    | NULL |    1 |   100.00 | NULL  |
+----+-------------+---------------+------------+--------+---------------+------+---------+------+------+----------+-------+
1 row in set, 1 warning (0.02 sec)
```

### EXPLAIN输出列

#### id列

这一列总是包含一个编号，标识`SELECT`所需的列。如果在语句当中没有**子查询**或**联合**，那么只会有唯一的`SELECT`，于是每一行在这个列中都将显示一个1。否则，内层的`SELECT`语句一般会**顺序编号**，对应于其在原始语句中的位置。

MySQL将`SELECT`查询分为**简单和复杂类型**，复杂类型可分成三大类：**简单子查询**、**所谓的派生表（在FROM子句中的子查询）**，以及**UNION查询**。

简单子查询：

```sql
MySQL [nideshop]> explain select (select 1 from nideshop_user_level) from nideshop_user;
+----+-------------+---------------------+------------+--------+---------------+---------+---------+------+------+----------+-------------+
| id | select_type | table               | partitions | type   | possible_keys | key     | key_len | ref  | rows | filtered | Extra       |
+----+-------------+---------------------+------------+--------+---------------+---------+---------+------+------+----------+-------------+
|  1 | PRIMARY     | nideshop_user       | NULL       | system | NULL          | NULL    | NULL    | NULL |    1 |   100.00 | NULL        |
|  2 | SUBQUERY    | nideshop_user_level | NULL       | index  | NULL          | PRIMARY | 1       | NULL |    2 |   100.00 | Using index |
+----+-------------+---------------------+------------+--------+---------------+---------+---------+------+------+----------+-------------+
2 rows in set, 1 warning (0.00 sec)
```

FROM子句中的子查询：

```sql
MySQL [nideshop]> explain select id from (select id from nideshop_user) as user;
+----+-------------+---------------+------------+--------+---------------+------+---------+------+------+----------+-------+
| id | select_type | table         | partitions | type   | possible_keys | key  | key_len | ref  | rows | filtered | Extra |
+----+-------------+---------------+------------+--------+---------------+------+---------+------+------+----------+-------+
|  1 | SIMPLE      | nideshop_user | NULL       | system | NULL          | NULL | NULL    | NULL |    1 |   100.00 | NULL  |
+----+-------------+---------------+------------+--------+---------------+------+---------+------+------+----------+-------+
1 row in set, 1 warning (0.00 sec)
```

UNINO查询

```sql
MySQL [nideshop]> explain select id from nideshop_user union all select id from nideshop_user_level;
+----+-------------+---------------------+------------+--------+---------------+---------+---------+------+------+----------+-------------+
| id | select_type | table               | partitions | type   | possible_keys | key     | key_len | ref  | rows | filtered | Extra       |
+----+-------------+---------------------+------------+--------+---------------+---------+---------+------+------+----------+-------------+
|  1 | PRIMARY     | nideshop_user       | NULL       | system | NULL          | NULL    | NULL    | NULL |    1 |   100.00 | NULL        |
|  2 | UNION       | nideshop_user_level | NULL       | index  | NULL          | PRIMARY | 1       | NULL |    2 |   100.00 | Using index |
+----+-------------+---------------------+------------+--------+---------------+---------+---------+------+------+----------+-------------+
```

#### select_type列

这一列显示了对应行是简单还是复杂`SELECT`（如果是后者，那么是三种复杂类型中的哪一种）。`SIMPLE`值意味着查询不包括子查询和UNION。如果查询有任何复杂的子部分，则**最外层**部分标记为`PRIMARY`，其他部分标记如下：

- SUBQUERY：包含在`SELECT`列表中的子查询中的`SELECT`（不在FROM子句中）标记为`SUBQUERY`；
- DERIVED：DERIVED值用来表示包含在FROM子句中的子查询中的`SELECT`，MySQL会递归执行并将结果放到一个**临时表**中。服务器内部称其为“派生表”，因为该临时表是从子查询中派生来的。
- UNION：在UNION中的第二个和随后的`SELECT`被标记为UNION，第一个`SELECT`被标记为`PRIMARY`。如果UNION被FROM子句中的子查询包含，那么它的第一个`SELECT`会被标记为`DERIVED`；
- UNION RESULT：用来从UNION的匿名临时表检索结果的`SELECT`被标记为`UNION RESULT`；
- DEPENDENT：意味着`SELECT`依赖于外层查询中发现的数据；
- UNCACHEABLE：意味着`SELECT`中的某些特性阻止结果被缓存于一个Item_cache中。

#### table列

这一列显示了对应行正在访问哪个表。通常情况下，它就是那个表，或是该表的别名（如果SQL中定义了别名）。

当在FROM子句中有子查询时，table列是`<derivedN>`的形式，其中N是子查询的id。

当有UNION时，UNION RESULT的table列包含一个参与UNION的id列表，形式为`<unionM,N>`。

#### type列

type列显示了关联类型，更准确的说法是访问类型-就是MySQL决定如何查找表中的行。下面是最重要的访问方法，以此从最差到最优。

- `ALL`：**全表扫描**，通常意味着MySQL必须扫描整张表，从头到尾，去找到需要的行。（这里也有个例外，例如在查询里使用了LIMIT，或者在Extra列中显示“Using distinct/not exists”）；
- `index`：这个跟全表扫描一样，只是MySQL扫描表时**按索引次序**进行而不是行。它的主要优点是避免了排序；最大的缺点是要承担按索引次序读取整个表的开销。这通常意味着若是按随机次序访问行，开销将会非常大。如果在Extra列中看到"Using index"，说明MySQL正在使用**覆盖索引**，它只扫描索引的数据，而不是按索引次序的每一行。它比按索引次序全表扫描的开销要少很多（不要回表查询）。
- `range`：范围扫描是一个有限制的**索引扫描**（列上已设置索引），它开始于索引里的某一点，返回匹配这个值域的行。这比全索引扫描好一些，因为它用不着遍历全部索引，此类扫描的开销跟索引类型相当。显而易见的范围扫描是带有`BETWEEN`或在`WHERE`子句里带有`>`的查询。当在索引列上使用`IN()`和`OR`时，也会显示为范围扫描，然而，这两者其实是相当不同的访问类型，在性能上有重要的差异。
- `ref`：这是一种**索引访问（也称索引查找）**，它返回所有匹配某个单个值的行。然而，它可能找到多个符号条件的行，因此，它是查找和扫描的混合体。此类索引访问只有当使用**非唯一性索引或者唯一性索引的非唯一性前缀**时才会发生。把它叫做ref是因为索引要跟某个参考值相比较。这个参考值或者是一个**常数**，或者是来自多表查询前一个表里的结果值。`ref_or_null`是ref之上的一个变体，它意味着MySQL必须在初次查找的结果里进行第二次查找以找出NULL条目。
- `eq_ref`：使用这种**索引查找**，MySQL知道最多返回一条符合条件的记录。在使用主键或者唯一性索引查找时看到，它会将它们与某个参考值做比较。MySQL对于这类访问类型的优化做得非常好，因为它知道无须估计匹配行的范围或在找到匹配行后再继续查找。
- `const, system`：当MySQL能对查询的某部分进行优化并将其转换为一个常量时，它就会使用这些访问类型。例如，如果你通过将某一行的主键放入WHERE子句里的方式来选取此行的主键，MySQL就能把这个查询转换为一个常量。然后就可以高效地将表从联接执行中移除。
- `NULL`：这种方式意味着MySQL能在优化阶段分解查询语句，在执行阶段甚至用不着再访问表或者索引。例如，从一个索引列里选取最小值可以通过单独查找索引来完成，不需要在执行时访问表。

#### possible_keys列

这一列显示了查询可以使用哪些索引，这是基于查询访问的列和使用的比较操作符来判断的。这个列表是在优化过程的早期创建的，因此有些罗列出来的索引可能对于后续优化过程是没用的。

#### key列

这一列显示了MySQL决定采用哪个索引来优化对该表的访问。如果该索引没有出现在possible_keys列中，那么MySQL选用它是出于另外的原因-例如，它可能选择了一个覆盖索引，哪怕没有WHERE子句。

换句话说，possible_keys揭示了哪一个索引能**有助于高效地进行查找**，而key显示的是优化采用哪一个索引可以**最小化查询成本**。

#### key_len列

该列显示了MySQL在索引里使用的**字节数**。如果MySQL正在使用的只是索引里的某些列，那么就可以用这个值来算出具体是哪些列。当我们计算列的使用情况时，务必把字符列中的**字符集**也考虑进去。

在utf8字符集中，每个字符最多占用3个字节。

MySQL并不总显示一个索引真正使用了多少。例如，如果对一个前缀模式匹配执行LIKE查询，它会显示列的完全宽度正在被使用。

key_len列显示了**在索引字段中可能的最大长度，而不是表中数据使用的实际字节数**。换言之，key_len通过查找表的定义而被计算出，而不是表中的数据。

#### ref列

这一列显示了之前的表在key列记录的索引中查找值所用的**列或常量**。

```sql
MySQL [demo]> EXPLAIN \
    -> SELECT * FROM food AS food \
    -> INNER JOIN food_food_base_rela AS rela ON food.`food_id`=rela.`food_id` AND rela.`food_id`=416 \
    -> INNER JOIN food_base AS base ON rela.`food_base_id`=base.`food_base_id`;
+----+-------------+-------+------------+--------+------------------------------+-------------+---------+------------------------------------+------+----------+-------+
| id | select_type | table | partitions | type   | possible_keys                | key         | key_len | ref                                | rows | filtered | Extra |
+----+-------------+-------+------------+--------+------------------------------+-------------+---------+------------------------------------+------+----------+-------+
|  1 | SIMPLE      | food  | NULL       | const  | PRIMARY                      | PRIMARY     | 4       | const                              |    1 |   100.00 | NULL  |
|  1 | SIMPLE      | rela  | NULL       | ref    | idx_food_id,idx_food_base_id | idx_food_id | 4       | const                              |   12 |   100.00 | NULL  |
|  1 | SIMPLE      | base  | NULL       | eq_ref | PRIMARY                      | PRIMARY     | 4       | demo.rela.food_base_id |    1 |   100.00 | NULL  |
+----+-------------+-------+------------+--------+------------------------------+-------------+---------+------------------------------------+------+----------+-------+
3 rows in set, 1 warning (0.00 sec)
```

#### rows列

这一列是MySQL估计为了找到所需的行而要读取的行数。这个数字是内嵌循环关联计划里的循环数目。也就是说它不是MySQL认为它最终要从表里读取出来的行数，而是MySQL为了找到符合查询的每一点上标准的那些行而必须读取的行的平均数。

根据表的统计信息和索引的选用情况，这个估算可能很不精确。要记住这个数字是MySQL认为它要检查的行数，而不是结果集里的行数。同时也要认识到有很多优化手段，例如关联缓冲区和缓存，无法影响到行数的显示。MySQL可能不必真的读所有它估计到的行，它也不知道任何关用操作系统或硬件缓存的信息。

#### filtered列

它显示的是针对表里符合某个条件（WHERE子句或联接条件）的记录数的百分比所做的一个悲观估算。如果你把rows列和这个百分比相乘，就能看到MySQL估算它将和查询计划里前一个表关联的行数。通常优化器只有在使用ALL、index、range和index_merge访问方法时才会使用这一估算。最大值为100，意味着没有行过滤发生。

#### Extra列

这一列包含的是不适合在其他列显示的额外信息。

常见的最重要的值如下：

- `Using index`：此值表示MySQL将使用**覆盖索引**，以避免访问表。不要把覆盖索引和index访问类型弄混了。
- `Using where`：这意味着MySQL服务器将**在存储引擎检索行后再进行过滤**。许多WHERE条件里涉及索引中的列，当（并且如果）它读取索引时，就能被存储引擎检验，因此不是所有带WHERE子句的查询都会显示`Using where`。有时`Using where`的出现就是一个暗示：查询可受益于不同的索引。
- `Using temporary`：MySQL在对查询结果排序时会使用一个临时表。
- `Using filesort`：MySQL会对结果使用一个**外部索引排序**，而不是按索引次序从表里读取行。MySQL有两种文件排序算法，**两种方式都可以在内存或磁盘上完成**。EXPLAIN不会告诉你MySQL将使用哪一种文件排序，也不会告诉你排序会在内存里还是磁盘上完成。
- `Range checked for each record(index map:N)`：这个值意味着没有好用的索引，新的索引将在联接的每一行上重新估算。N是显示possible_keys列中索引的位图，并且是冗余的。

## 参考

- [高性能MySQL（第三版）]
