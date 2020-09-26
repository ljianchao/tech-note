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
