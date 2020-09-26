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

// 在顺序表的第i（1<=position<=n+1）个元素之前，插入一个新元素x
int insert_seq_list(seq_list list, data_type data, int position);

// 将线性表的第i（1<=position<=n）个数据元素删除
int delete_seq_list(seq_list list, int position);

// 查出线性表L中值等于position的结点序号的最小值
int locate_seq_list(seq_list list, data_type position);

#endif /* _STRUCTURE_LINEARL_H_INCLUDED_ */
