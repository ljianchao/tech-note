#include <stdio.h>

#include "LinearL.h"

int main()
{
    // 顺序表测试
    seq_list list;

    data_type student = {1, "Zsan", "2", 13, 100};
    list.data[0] = student;
    list.length = 1;

    printf("Student list is initialed. Info: \n");
    printf("num: %d, name: %s, sex: %s, age: %d, score: %d \n",
           list.data[0].num, list.data[0].name, list.data[0].sex, list.data[0].age, list.data[0].score);

    int position = 1;
    data_type new_student = {2, "Lisi", "2", 14, 99};
    insert_seq_list(list, new_student, position);

    int index = position - 1;
    printf("Inserted student. Info: \n");
    printf("num: %d, name: %s, sex: %s, age: %d, score: %d \n",
           list.data[index].num, list.data[index].name, list.data[index].sex, list.data[index].age, list.data[index].score);

    return 0;
}
