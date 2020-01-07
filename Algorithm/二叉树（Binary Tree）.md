# 二叉树（Binary Tree）

## 二叉树的定义和性质

### 定义

二叉树是n（n>=0）个结点的有限集合，它的每个结点至多只有两棵子树。它或者是空集，或者是由一个根结点及两棵互不相交的分别称作这个根的左子树和右子树的二叉树组成。

从二叉树的定义看出，它是递归的。

### 性质

性质1 在二叉树的第`i`层上至多有 `2^(i-1)`个结点（i>=1）

性质2 深度为`k`的二叉树至多有`2^k-1`个结点（k>=1）

性质3 对任何一棵二叉树`T`，若其终端结点（叶子结点）数为`n0`，度数为2的结点数为`n2`，则`n0=n2+1`

***满二叉树***：一棵深度为`k`且有`2^k-1`个结点的二叉树称为满二叉树。特点是每一层上的结点数都达到最大值，不存在度数为1的结点，且所有叶子结点都在第`k`层上。

***完全二叉树***：若一个深度为`k`的二叉树，其前`k-1`层是一个满二叉树，而最下面一层（即第k层）上的结点都集中在该层最左边的若干位置上，则称此二叉树为完全二叉树。

满二叉树一定是完全二叉树，但完全二叉树则不一定是满二叉树。

性质4 具有`n`个结点的完全二叉树的深度为`logn`向下取整的值加上1。

## 二叉树的运算

### 二叉树的遍历

所谓遍历，是指沿着某条搜索路径（线）周游二叉树，依次对树中每个结点访问且仅访问一次。

#### 递归遍历算法

前序遍历（Preorder Traversal）二叉树的递归定义：

    若二叉树非空，则依次进行操作：访问根节点，前序遍历左子树，前序遍历右子树。

中序遍历（Inorder Traversal）二叉树的定义：

    若二叉树非空，则依次进行操作：中序遍历左子树，访问根结点，中序遍历右子树。

后序遍历（Postorder Traversal）二叉树的定义：

    若二叉树非空，则依次进行操作：后序遍历左子树，后序遍历右子树，访问根结点。

遍历算法中的递归终止条件是二叉树为空。

代码如下：

```java
    /**
     * 前序遍历二叉树
     * @param treeNode
     */
    static void Preorder(TreeNode<String> treeNode) {
        if (treeNode != null) {
            System.out.println(treeNode.getVal());
            Preorder(treeNode.getLeft());
            Preorder(treeNode.getRight());
        }
    }

    /**
     * 中序遍历二叉树
     * @param treeNode
     */
    static void Inorder(TreeNode<String> treeNode) {
        if (treeNode != null) {
            Inorder(treeNode.getLeft());
            System.out.println(treeNode.getVal());
            Inorder(treeNode.getRight());
        }
    }

    /**
     * 后序遍历二叉树
     * @param treeNode
     */
    static void Postorder(TreeNode<String> treeNode) {
        if (treeNode != null) {
            Postorder(treeNode.getLeft());
            Postorder(treeNode.getRight());
            System.out.println(treeNode.getVal());
        }
    }

```



