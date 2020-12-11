#include "LinearL.h"

int insert_seq_list(seq_list list, data_type data, int position)
{
    if (list.length == MAXSIZE)
    {
        return -1;
    }
    if (position < 1 || position > list.length + 1)
    {
        return -1;
    }

    for (int j = list.length; j >= position; j--)
    {
        list.data[j] = list.data[j - 1];
    }
    list.data[position - 1] = data;
    list.length++;
    
    return 1;
}

int delete_seq_list(seq_list list, int position)
{
    if (position < 1 || position > list.length)
    {
        return -1;
    }

    for (int j = position; j < list.length; j++)
    {
        list.data[j - 1] = list.data[j];
    }
    list.length--;

    return 1;
}

int locate_seq_list(seq_list list, data_type position)
{
    int i = 0;
    while ((i < list.length) && (list.data[i].name != position.name))
    {
        i++;
    }
    
    if (i < list.length)
    {
        return i + 1;
    } 
    else
    {
        return 0;
    }
}

/* 单链表操作集合 */

link_list initial_link_list()
{
    link_list head;  // 头指针

    head = malloc(sizeof(node)); // 动态构建一结点，它是头结点
    head->next = NULL;
    return head;
}

int length_link_list(link_list head)
{
    node * p = head;  // p是工作指针，初始时p指向头结点
    int count = 0;  // 计数器初始值
    while (p->next != NULL)  // 判断是否为尾结点
    {
        p = p->next;  // 指针移动到下一个结点
        count++;
    }
    return count;
}

node * get_link_list(link_list head, int i)
{
    node * p;  // p是工作指针
    p = head->next; // 初始时p指向头结点
    int c = 1;
    while (c < i && p != NULL)
    {
        p = p->next;
        c++;
    }
    
    if (c == i)
    {
        return p;
    }
    else
    {
        return NULL;
    }
}

int locate_link_list(link_list head, data_type x)
{
    node * p = head;
    p = p->next;
    int index = 0;
    while ((p != NULL) && (p->data != x))
    {
        index++;
        p = p->next;        
    }
    
    if (p != NULL)
    {
        return i + 1;
    }
    else
    {
        return 0;
    }
    
}

void insert_link_list(link_list head, data_type x, int i)
{
    node * p;  // 工作指针
    node * q;  // 第 i-1 个结点的指针
    // 首先查找第 i-1 个结点
    if (i == 1)
    {
        q = head;
    } 
    else
    {
        q = get_link_list(head, i - 1);  // 查找第 i-1 个数据元素的结点
    }
    
    if (q == NULL)
    {
        exit("找不到插入的位置");
    }
    else
    {
        p = malloc(sizeof(node));  // 生成新结点
        p->data = x;
        p->next = q->next;  // 新结点指针域指向 *q 的后继结点
        q->next = p;  // 修改 *q 的链域
    }
}

void delete_link_list(link_list head, int i)
{
    node * p;  // 工作指针
    node * q;  // 第 i-1 个结点的指针

    if (i == 1)
    {
        q = head;
    }
    else
    {
        q = get_link_list(head, i - 1);  // 查找待删除结点的直接前驱
    }
    
    if ((q != NULL) && (q->next != NULL))
    {
        p = q->next;  // p指向待删除结点
        q->next = p->next;  // 移出待删除结点
        free(p);  // 释放已移出结点p的空间
    }
    else
    {
        exit("找不到要删除的结点");
    }
    
}
