# 线性表-顺序表-ArrayList

`ArrayList`底层采用数组实现，是具有顺序存储结构的线性表。

## 基本定义和字段

```java
public class ArrayList<E> extends AbstractList<E>
        implements List<E>, RandomAccess, Cloneable, java.io.Serializable
{
    private static final long serialVersionUID = 8683452581122892189L;

    /**
     * Default initial capacity.
     */
    private static final int DEFAULT_CAPACITY = 10;

    /**
     * Shared empty array instance used for empty instances.
     */
    private static final Object[] EMPTY_ELEMENTDATA = {};

    /**
     * Shared empty array instance used for default sized empty instances. We
     * distinguish this from EMPTY_ELEMENTDATA to know how much to inflate when
     * first element is added.
     */
    private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};

    /**
     * The array buffer into which the elements of the ArrayList are stored.
     * The capacity of the ArrayList is the length of this array buffer. Any
     * empty ArrayList with elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA
     * will be expanded to DEFAULT_CAPACITY when the first element is added.
     */
    transient Object[] elementData; // non-private to simplify nested class access

    /**
     * The size of the ArrayList (the number of elements it contains).
     *
     * @serial
     */
    private int size;

    /**
     * Constructs an empty list with the specified initial capacity.
     *
     * @param  initialCapacity  the initial capacity of the list
     * @throws IllegalArgumentException if the specified initial capacity
     *         is negative
     */
    public ArrayList(int initialCapacity) {
        if (initialCapacity > 0) {
            this.elementData = new Object[initialCapacity];
        } else if (initialCapacity == 0) {
            this.elementData = EMPTY_ELEMENTDATA;
        } else {
            throw new IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
        }
    }

    /**
     * Constructs an empty list with an initial capacity of ten.
     */
    public ArrayList() {
        this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
    }

    /**
     * Constructs a list containing the elements of the specified
     * collection, in the order they are returned by the collection's
     * iterator.
     *
     * @param c the collection whose elements are to be placed into this list
     * @throws NullPointerException if the specified collection is null
     */
    public ArrayList(Collection<? extends E> c) {
        elementData = c.toArray();
        if ((size = elementData.length) != 0) {
            // c.toArray might (incorrectly) not return Object[] (see 6260652)
            if (elementData.getClass() != Object[].class)
                elementData = Arrays.copyOf(elementData, size, Object[].class);
        } else {
            // replace with empty array.
            this.elementData = EMPTY_ELEMENTDATA;
        }
    }
}
```

字段解析：

- `DEFAULT_CAPACITY`：底层数组的默认初始化容量（值为10）；
- `EMPTY_ELEMENTDATA`：共享的空数组实例；
- `DEFAULTCAPACITY_EMPTY_ELEMENTDATA`：共享的空数组实例，用于`ArrayList`默认无参构造函数，创建一个空列表`ArrayList`。我们将此与`EMPTY_ELEMENTDATA`区别开来，在填充第一个元素时，会进行`elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA`的判断，以便进行数组的扩充。
- `elementData`：存储`ArrayList`的数据元素的数组缓冲区，`ArrayList`的容量大小为该数组缓冲区的长度。任何具有`elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA`的空`ArrayList`（即采用无参构造函数创建的ArrayList）在填充第一个元素时，都将扩充为容量为`DEFAULT_CAPACITY`（值为10）的新数组。
- `size`：`ArrayList`包含的元素数量。
