1. 访问修饰符 访问修饰符 访问修饰符 public,private,protected,以及不写（默认）时的区别？ 

    适用范围<访问权限范围越小，安全性越高>

　　　　  访问权限   类   包  子类  其他包

　　　　  public     ∨   ∨    ∨     ∨          （对任何人都是可用的）

　　　　 protect    ∨   ∨   ∨     ×　　　 （继承的类可以访问以及和private一样的权限）

　　　　 default    ∨   ∨   ×     ×　　　 （包访问权限，即在整个包内均可被访问）

　　　　 private    ∨   ×   ×     ×　　　 （除类型创建者和类型的内部方法之外的任何人都不能访问的元素）

2. int和 Integer有什么区别？

Java是面向对象的编程语言，一切都是对象，但是为了编程的方便还是引入了基本数据类型，为了能够将这些基本数据类型当成对象操作，Java为每一个基本数据类型都引入了对应的包装类型（wrapper class），int的包装类就是Integer，从Java 5开始引入了自动装箱/拆箱机制，使得二者可以相互转换。

3. &和&&的区别？

&&：短路逻辑与，当&&两边的表达式结果都为true时整体才为true，并且有短路的作用即当第一个表达式结果为false时第二个表达式将不再执行。

&：逻辑与或者位运算符，当&作为逻辑与时和&&的作用一样但是没有短路的作用即当第一个表达式为false时第二个表达式也会运算，但对整体结果没有影响，当&的两边为整数  时，&作为位运算符存在，计算方式：将两个整数转换为二进制数，再取两个整数的最低4位进行运算，当二进制数都为1时计算结果才为1，否则都为0。

4. Java中用最有效率的方法算出2 乘以8 等於几?

因为将一个数左移n 位，就相当于乘以了2 的n 次方，那么，一个数乘以8 只要将其左移3 位即可，而位运算cpu 直接支持的，效率最高，所以，2 乘以8 等於几的最效率的方法是2 << 3。


1. 是否可以继承 String类？

不能被继承，因为String类有final修饰符，而final修饰的类是不能被继承的。

修饰类：当用final修饰一个类时，表明这个类不能被继承。final类中的成员变量可以根据需要设为final，但是要注意final类中的所有成员方法都会被隐式地指定为final方法。
修饰方法：使用final修饰方法的原因有两个。第一个原因是把方法锁定，以防任何继承类修改它的含义；第二个原因是效率。在早期的Java实现版本中，会将final方法转为内嵌调用。但是如果方法过于庞大，可能看不到内嵌调用带来的任何性能提升。在最近的Java版本中，不需要使用final方法进行这些优化了。
修饰变量：对于被final修饰的变量，如果是基本数据类型的变量，则其数值一旦在初始化之后便不能更改；如果是引用类型的变量，则在对其初始化之后便不能再让其指向另一个对象。虽然不能再指向其他对象，但是它指向的对象的内容是可变的。

6. 抽象类（ abstract class）和接口（ interface）有什么异同？

抽象类方式中，抽象类可以拥有任意范围的成员数据，同时也可以拥有自己的非抽象方法，
但是接口方式中，它仅能够有静态、不能修改的成员数据（但是我们一般是不会在接口中使用成员数据），同时它所有的方法都必须是抽象的。
在某种程度上来说，接口是抽象类的特殊化。
对子类而言，它只能继承一个抽象类（这是java为了数据安全而考虑的），但是却可以实现多个接口。

语法规范区别：
1）接口不能有构造方法，抽象类可以有。
2）接口不能有方法体，抽象类可以有。
3）接口不能有静态方法，抽象类可以有。
4）在接口中凡是变量必须是public static final，而在抽象类中没有要求。

7. 打印昨天的当前时刻

```java
    Calendar calendar = Calendar.getInstance();
    calendar.add(Calendar.DATE, -1);
    System.out.println(calendar.getTime());
    System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(calendar.getTimeInMillis()));
```

8. try{}里有一个return语句，那么紧跟在这个 try后的 finally{}里的代码会不被执行，什 么时候被执行，在 return前还是后？

finally语句是在try的return语句执行之后，return返回之前执行。


9. 列出一些你常见的运行时异常

- ArithmeticException（算术异常）
- ClassCastException （类转换异常）
- IllegalArgumentException （非法参数异常）
- IndexOutOfBoundsException （下标越界异常）
- NullPointerException （空指针异常）
- SecurityException （安全异常）


10. 阐述ArrayList、Vector、LinkedList的存储性能和特性？

ArrayList 和Vector他们底层的实现都是一样的，都是使用数组方式存储数据，此数组元素数大于实际存储的数据以便增加和插入元素，它们都允许直接按序号索引元素，但是插入元素要涉及数组元素移动等内存操作，所以索引数据快而插入数据慢。

Vector中的方法由于添加了synchronized修饰，因此Vector是线程安全的容器，但性能上较ArrayList差，因此已经是Java中的遗留容器。

LinkedList使用双向链表实现存储（将内存中零散的内存单元通过附加的引用关联起来，形成一个可以按序号索引的线性结构，这种链式存储方式与数组的连续存储方式相比，内存的利用率更高），按序号索引数据需要进行前向或后向遍历，但是插入数据时只需要记录本项的前后项即可，所以插入速度较快。            

Vector属于遗留容器（Java早期的版本中提供的容器，除此之外，Hashtable、Dictionary、BitSet、Stack、Properties都是遗留容器），已经不推荐使用，但是由于ArrayList和LinkedListed都是非线程安全的，如果遇到多个线程操作同一个容器的场景，则可以通过工具类Collections中的synchronizedList方法将其转换成线程安全的容器后再使用（这是对装潢模式的应用，将已有对象传入另一个类的构造器中创建新的对象来增强实现）。

11. 获得一个类的类对象有哪些方式？

方法1：类型.class，例如：String.class
方法2：对象.getClass()，例如：”hello”.getClass()
方法3：Class.forName()，例如：Class.forName(“java.lang.String”)

12. 在 Linux中查看日志使用哪个命令? 编辑文件使用什么命令?查看进程使用什么命令 ?

13. synchronized实现原理？

参考[深入理解Java并发之synchronized实现原理](https://blog.csdn.net/javazejian/article/details/72828483)