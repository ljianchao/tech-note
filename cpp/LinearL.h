/*
 * 顺序表（线性表的顺序存储）
 * 
 */

#ifndef _STRUCTURE_LINEARL_H_INCLUDED_
#define _STRUCTURE_LINEARL_H_INCLUDED_

// 定义数组最大容量
#define MAXSIZE 100

// 定义结点的类型
typedef struct
{
    int num;
    char name[8];
    char sex[2];
    int age;
    int score;
} data_type;

// 定义顺序表
typedef struct
{
    data_type data[MAXSIZE];  // 存放数据的数组
    int length;  // 线性表的实际长度
} seq_list;

// 定义单链表
typedef struct
{
    data_type data;  // 数据域
    struct node * next;  // 指针域
} node, * link_list;

// 定义双向循环链表
struct dbnode
{
    data_type data_type;
    struct dbnode * prior, * next;
};
typedef struct dbnode * dbpointer;
typedef dbpointer db_link_list;


// 在顺序表的第i（1<=position<=n+1）个元素之前，插入一个新元素x
int insert_seq_list(seq_list list, data_type data, int position);

// 将线性表的第i（1<=position<=n）个数据元素删除
int delete_seq_list(seq_list list, int position);

// 查出线性表L中值等于position的结点序号的最小值
int locate_seq_list(seq_list list, data_type position);

/* 单链表操作集合 */

// 初始化单链表
link_list initial_link_list();

// 获取表长
int length_link_list(link_list head);

// 获取指定位置元素
node * get_link_list(link_list head, int i);

// 按指查找
int locate_link_list(link_list head, data_type x);

// 将给定值为 x 的元素插入到链表 head 的第 i 个结点之前
void insert_link_list(link_list head, data_type x, int i);

// 删除链表 head 第 i 个结点
void delete_link_list(link_list head, int i);

#endif /* _STRUCTURE_LINEARL_H_INCLUDED_ */
