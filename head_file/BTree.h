#ifndef __BTREE_H__
#define __BTREE_H__

#include "Tree.h"
#include "Pointer.h"
#include "BTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"
#include "DynamicArray.h"

/*
二叉树设计要点
	BTree为二叉树结构， 每个节点最多只有连个后继节点
	BTreeNode 只包含4个固定的公有成员
	实现树结构的所有操作（增删改查）
*/

namespace YzcLib{

template <typename T>
class BTree: public Tree<T>{
public:
	//三种典型遍历方式
	enum BTTraversal{
		PreOrder,
		InOrder,
		PostOrder,
		LevelOrder
	};

protected:
	LinkQueue<BTreeNode<T>*> m_queue;

	BTreeNode<T>* _find(BTreeNode<T>* node, const T& value) const;
	BTreeNode<T>* _find(BTreeNode<T>* node,  const BTreeNode<T>* obj) const;

	int _degree(BTreeNode<T>* node) const;
	int _count(BTreeNode<T>* node) const;
	int _height(BTreeNode<T>* node) const;

	void _free(BTreeNode<T>* node);
	SharedPointer<Tree<T>> _remove(BTreeNode<T>* node);

	void PreOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue);
	void InOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue);
	void PostOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue);
	void LevelOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue);

	void _traversal(LinkQueue<BTreeNode<T>*>& queue, BTTraversal order);

	BTreeNode<T>* _clone(const BTreeNode<T>* node) const;

	bool _equal(BTreeNode<T>* lt, BTreeNode<T>* rt) const;

	BTreeNode<T>* _add(const BTreeNode<T>* lt, const BTreeNode<T>* rt) const;

	BTreeNode<T>* _connect(LinkQueue<BTreeNode<T>*>& queue);
public:
	enum BTNodePos{
		ANY,
		LEFT,
		RIGHT
	};
	bool Insert(TreeNode<T>* node, BTNodePos pos);
	bool Insert(const T& value, TreeNode<T>* parent, BTNodePos pos);

	bool Insert(TreeNode<T>* node );
	bool Insert(const T& value, TreeNode<T>* parent = NULL);
	//将元素从树中删除
	SharedPointer<Tree<T>> Remove(const T& parent);
	SharedPointer<Tree<T>> Remove(TreeNode<T>* node);

	BTreeNode<T>* Find(const T& value) const;
	BTreeNode<T>* Find(TreeNode<T>* node) const;
	//获取树的根节点
	BTreeNode<T>* Root() const;
	//获取树的度
	int Degree() const;
	//获取树的节点数
	int Count() const;
	//获取树的高度
	int Height() const;
	// 清空树中的元素
	void Clear();

	//二叉树的层次遍历迭代器
	bool Begin();
	bool End();
	bool Next();
	T Current();

	//二叉树的先序，中序，后续遍历算法
	SharedPointer<Array<T> > Traversal(BTTraversal order);

	//二叉树的克隆操作
	SharedPointer<BTree<T>> Clone() const;

	//二叉树比较操作
	bool operator == (const BTree<T>& btree) const;
	bool operator != (const BTree<T>& btree) const;

	SharedPointer<BTree<T>> Add(const BTree<T>& btree) const;

	BTreeNode<T>* Thread(BTTraversal order);

	~BTree();
};

/*
二叉树
	通用树结构回顾
		双亲孩子表示法：
			每个节点都有一个指向其双亲的指针
			每个节点都有若干指向其孩子的指针
	另一种树形结构
		孩子兄弟表示法
			每个节点都有一个指向其第一个孩子的指针
			每个节点都有一个指向其第一个右兄弟的指针
		孩子兄弟表示法的特点：
			能够表示任意的树形结构
			每个节点都包含一个数据成员和两个指针成员
			孩子节点指针和兄弟节点指针构成了树杈
		孩子兄弟表示法能够将通用树转化为二叉树

	二叉树定义：
		二叉树是由n（n >= 0）个节点组成的有限集合，该集合或者为空，或者是由一个根节点加上两颗分别成为左子树和右子树的，互步相交的二叉树组成
				*
			  /   \
			 L     R
	二叉树五种形态：
	1    2       3       4     5
				 *        *    *
	空	 *	   /   \     /		\
			  L     R   L        R

特殊的二叉树
	满二叉树（Fully binary tree）
		如果二叉树中所有分直节点的度数都为2， 且叶子节点都在同一层次上，则称这类二叉树为满二叉树
	完全二叉树（Complete Binary Tree）
		如果一颗具有n个节点的高度为k的二叉树， 它的每一个节点都与高度为k的满二叉树中编号为1-n的节点一一对应，则称这颗树为完全二叉树（从上到下，从左到右编号）
	
	完全二叉树性质
		同样节点的二叉树，完全二叉树高度最小
		完全二叉树的叶节点一定出现在最下面两侧
			最底层的叶节点一定出现在左边
			倒数第二层的页节点一定出现在第二层
			完全二叉树中度为1的节点只有左孩子

	二叉树是最多只有两个孩子的树

	深层性质：
		1.在二叉树的第i层最多有 2^(i - 1)个节点（i >= 1）
			第一层 最多有 2^(1-1) = 1 个节点
			第二层 最多有 2^(2-1) = 2 个节点
			第一层 最多有 2^(3-1) = 4 个节点
			.........

		2.高度为k的二叉树最多有2^k - 1个节点（k>= 0）
			有一层， k = 1, 最多有 1 = 2^1 - 1 = 1 个节点
			有二层， k = 2, 最多有 1 + 2 = 2^2 - 1 = 3 个节点
			有二层， k = 2, 最多有 1 + 2 + 4 = 2^3 - 1 = 7 个节点

		前两个性质可以用数学归纳法证明

		3.任何一颗二叉树，如果其叶节点有n0个，度为2的非叶节点有n2个，则 n0 = n2 + 1
			proof：
				suppose 二叉树中度为1的节点有n1个，且总数目为n个
					则	n = n1 + n2 + n0
				二叉树的边数可以表示为：n-1 （树的边数等于节点数减一）
				也可以表示为：2*n2 + 1*n1
				   n - 1 = 2*n2 + 1*n1 
				=> n0 = n2 + 1 (带入 n = n1 + n2 + n0)
		4.具有n个节点的**完全二叉树**的高度为floor(log(n)) + 1
			proof：
				suppose 这个n个节点的完全二叉树高度为k，则
					2^(k - 1) - 1 < n <= 2^k - 1
				<=> 2^(k - 1) <= n < 2^k 			(n为整数)
				=>  k-1 <= log(n) < k				(两边取log)
				=>  k = floor(log(n)) + 1			（取整）
		5.一颗有n个节点的完全二叉树（高度为floor(log(n)) + 1）, 按层次对节点进行编号（从上到下，从左到右）， 对任意 i 有：
			i = 1；		i是二叉树的根
			i > 1； 	双亲节点为floor(i/2)
			2i <= n; 	节点i的左孩子为2i
			2i > n;  	节点无左孩子
			2i+ 1 <= n  节点i的右孩子为 2i+1
			2i + 1 > n  节点i无右孩子
*/

/*
需要考虑的问题：
	是否能够在二叉树的任意节点处插入子节点？
	是否需要指定新数据元素（新节点）的插入位置？

因为二叉树有左右两个分支，所以插入有位置问题，而且每个节点左右两个位置只能各插入一个元素
因此通过定义枚举类型来确定树的位置，并且要重载带有具体位置的插入函数
	枚举类型三个元素
	ANY			任意位置插入
	LEFT		左侧插入
	RIGHT		右侧插入


*/

//如果所在位置无法插入节点，则返回插入失败
//如果输入参数有问题，那么抛异常


template <typename T>
bool BTree<T>::Insert(TreeNode<T>* node, BTNodePos pos){
	bool rst = true;
	//node是空，无法插入，抛异常
	if(node == NULL){
		THROW_EXCEPTION(InvalidParameterException, "Parameter node can not be NULL");
	}
	//根节点
	if( Root() == NULL && node->parent == NULL){
		this->m_root = node;
	}
	else{
		BTreeNode<T>* np = Find(node->parent);
		if(np != NULL){
			//插入顺序，先左后右
			if( (pos == ANY || pos == LEFT) && np->left == NULL ){
				np->left = dynamic_cast<BTreeNode<T>*>(node);
			}
			else if( (pos == ANY || pos == RIGHT) && np->right == NULL ){
				np->right = dynamic_cast<BTreeNode<T>*>(node);
			}
			else{
				//如果插入的位置已经有元素，那么无法插入，返回false
				rst = false;
			}
			
		}
		else{
			THROW_EXCEPTION(InvalidParameterException, "invalid parent tree node ...");
		}
	}
	return rst;
}

template <typename T>
bool BTree<T>:: Insert(const T& value, TreeNode<T>* parent, BTNodePos pos){
	BTreeNode<T>* node =  BTreeNode<T>::NewNode();
	if(node != NULL){
		node->value = value;
		node->parent = parent;

		return Insert(node, pos);
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create new node ... ");
	}
}

template <typename T>
bool BTree<T>:: Insert(TreeNode<T>* node ){
	return Insert(node, ANY);
}


template <typename T>
bool BTree<T>:: Insert(const T& value, TreeNode<T>* parent){
	return Insert(value, parent, ANY);
}


/*
删除的方式
	基于数据元素的删除
	基于节点的删除

删除操作功能的定义
	将node为根节点的子树从原来的二叉树中删除
	ret作为子树返回（ret指向堆空间中的二叉树对象）

步骤：
	1.判断node的值是否合法，不合法抛异常
		node不为NULL
		node必须能够用Find函数找到
	2.为返回值的子树申请空间
	3.如果node是根节点，将树的根节点置空
		如果node不是根节点
			检查node是左子树还是右子树
			删除node节点
		
	4.将node的parent置空，将node作为返回值子树的根节点

*/
template <typename T>
SharedPointer<Tree<T>> BTree<T>::_remove(BTreeNode<T>* node){
	BTree<T>* rst = new BTree<T>;
	if(rst == NULL){
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create btreenode...");
	}
	//node为根节点
	if(node == Root()){
		this->m_root = NULL;
	}
	//node不是根节点
	else{
		BTreeNode<T>* np = dynamic_cast<BTreeNode<T>*>(node->parent);
		(np->left == node) ? np->left = NULL : np->right = NULL;
	}
	//清除node与树的联系
	node->parent = NULL;

	m_queue.Clear();

	rst->m_root = node;

	return rst;
}
template <typename T>
SharedPointer<Tree<T>> BTree<T>:: Remove(const T& value){
	BTreeNode<T>* n = Find(value);
	//若待删除的节点不在树中
	if(n != NULL){
		return _remove(n);
	}
	else{
		THROW_EXCEPTION(InvalidParameterException, "parameter value is invalid...");
	}
}

template <typename T>
SharedPointer<Tree<T>> BTree<T>:: Remove(TreeNode<T>* node){
	BTreeNode<T>* n = Find(node);
	if(n != NULL){
		return _remove(n);
	}
	else{
		THROW_EXCEPTION(InvalidParameterException, "parameter value is invalid...");
	}
}

/*
二叉树节点查找操作
	基于数据元素值的查找
	基于节点的查找


基于数据元素值的查找
					node->value == value	return node;
find(node, value)
					node->value != value	find(node->left, value)
											find(node->right, value)


*/	
template <typename T>
BTreeNode<T>* BTree<T>:: _find(BTreeNode<T>* node, const T& value) const{
	BTreeNode<T>* rst = NULL;
	if(node != NULL){
		if(node->value == value){
			rst = node;
		}
		else{
			(rst == NULL) && (rst = _find(node->left, value));
			(rst == NULL) && (rst = _find(node->right, value));
		}

	}
	return rst;
}

template <typename T>
BTreeNode<T>* BTree<T>:: _find(BTreeNode<T>* node, const BTreeNode<T>* obj) const{
	BTreeNode<T>* rst = NULL;
	if(node != NULL && obj != NULL){
		if(node == obj){
			rst = node;
		}
		else{
				(rst == NULL) && (rst = _find(node->left, obj));
				(rst == NULL) && (rst = _find(node->right, obj));
		}

	}
	return rst;
}

template <typename T>
BTreeNode<T>* BTree<T>:: Find(const T& value) const{
	return _find(Root(), value);
}


template <typename T>
BTreeNode<T>* BTree<T>:: Find(TreeNode<T>* node) const{
	return _find(Root(), dynamic_cast<BTreeNode<T>*>(node));
}

template <typename T>
BTreeNode<T>* BTree<T>:: Root() const{
	return dynamic_cast<BTreeNode<T>*>(this->m_root);
}

/*
获取树的度
	获取node为根节点的二叉树的度数
				node == NULL	return 0
degree(node)
				node != NULL    max(degree(node->left), degree(node->right), (!!node->left + !!node->right))

小技巧：
	二叉树当前节点的度的计算
	对地址进行两次取反操作，例如(!!node->left), 
		如果当前地址为空，两次取反为0
		如果当前地址非空，两次取反为1

	二叉树最大度为二，若某一个节点度为2，那么无需继续递归，直接返回即可，提高效率

*/

template <typename T>
int BTree<T>:: _degree(BTreeNode<T>* node) const{
	int rst = 0;
	if(node != NULL){
		//node的度
		rst = (!!node->left + !!node->right);
		//当前node的度小于2，进一步递归
		//进算完左子树node，依旧小于2，继续递归
		//左右节点处理思路类似，避免冗余，采用数组的方式定义子节点逐一遍历
		BTreeNode<T>* child[] = {node->left, node->right};
		for(int i = 0; (i < 2) && (rst < 2); i++)
		{
			int d = _degree(child[i]);

			if(rst < d){
				rst = d;
			}
		}
		
	}
	return rst;	
}

template <typename T>
int BTree<T>:: Degree() const{
	return _degree(Root());
}

/*
二叉树节点数目
	在node中计算node为根节点的二叉树中统计节点数目
				node == NULL 	return 0
count(node)
				node != NULL	count(node->left)+ count(node->right) + 1
*/
template <typename T>
int BTree<T>::_count(BTreeNode<T>* node) const{
	return (node != NULL) ? _count(node->left) + _count(node->right) + 1 : 0;
}

template <typename T>
int BTree<T>:: Count() const{
	return _count(Root());
}

/*
二叉树高度
	获取node为根节点的二叉树的高度
				node == NULL	return 0
_height(node)
				node != NULL	return max(_height(node->left), _height(node->right)) + 1;
*/
template <typename T>
int BTree<T>::_height(BTreeNode<T>* node) const{
	int rst = 0;
	if(node != NULL){
		int l = _height(node->left);
		int r = _height(node->right);

		rst = ((l > r) ? l: r) + 1;
	}
	return rst;
}

template <typename T>
int BTree<T>:: Height() const{
	return _height(Root());
}

/*
二叉树中的节点删除操作
	free(node)
	清除node为根节点的二叉树
	释放二叉树的每一个节点
				node == NULL return;
free(node)
				node != NULL free(node->left);
							 free(node->right);
							 delete node;

*/	
template <typename T>
void BTree<T>::_free(BTreeNode<T>* node){
	if(node != NULL){
		//删除左子树
		_free(node->left);
		// 删除右子树
		_free(node->right);
		//删除node节点,如果在堆空间申请，需要释放内存
		if(node->Flag()){
			cout<<"**##"<<node->value<<endl;
			delete node;
		}
		else{
			cout<<"**&&"<<node->value<<endl;
		}
		
		m_queue.Clear();
		
	}
}

template <typename T>
void BTree<T>:: Clear(){
	_free(Root());
	this->m_root = NULL;
}

//层次遍历迭代器
template <typename T>
bool BTree<T>:: Begin(){
	bool rst = (Root() != NULL);
	if(rst){
		//先对队列进行清除，防止上一次有残余。
		m_queue.Clear();
		m_queue.Add(Root());
	}
	return rst;
}


template <typename T>
bool BTree<T>:: End(){
	return m_queue.Length() == 0;
}

template <typename T>
bool BTree<T>:: Next(){
	bool rst = !End();
	if(rst){
		//取出当前节点值
		BTreeNode<T>* n = m_queue.Front();
		//当前节点出队
		m_queue.Remove();
		//左子树入队
		if(n->left != NULL){
			m_queue.Add(n->left);
		}
		// 右子树入队
		if(n->right != NULL){
			m_queue.Add(n->right);
		}

	}
	return rst;
}

template <typename T>
T BTree<T>:: Current(){
	//队列为空时，调用抛异常
	if(!End()){
		return m_queue.Front()->value;
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
	}
	
}
/*
二叉树的典型遍历顺序
	先序遍历（Pre-Order Traversal）
	中序遍历（In-Order Traversal）
	后序遍历 （Post-Order Traversal）

设计要点：
	不能与层次遍历函数冲突，必须设计新的接口函数
	算法执行完成后，能够方便的获得遍历结果
	遍历结果能够反应节点访问的先后次序

二叉树的典型遍历方式都是递归方式执行的
如果想使用非递归方式，需要用到deque
*/
/*
先序遍历（Pre-Order Traversal）
	二叉树为空
		无操作，直接返回
	二叉树不为空
		访问根节点数据元素
		先序遍历左子树
		先序遍历右子树

*/
template <typename T>
void BTree<T>::PreOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue){
	if(node != NULL){
		queue.Add(node);
		PreOrderTraversal(node->left, queue);
		PreOrderTraversal(node->right, queue);
	}
}
/*
中序遍历（In-Order Traversal）
	二叉树为空
		无操作直接返回
	二叉树不为空：
		中序遍历左子树
		访问根节点中的数据
		中序遍历右子树
*/
template <typename T>
void BTree<T>::InOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue){
	if(node != NULL){
		InOrderTraversal(node->left, queue);
		queue.Add(node);
		InOrderTraversal(node->right, queue);
	}

}
/*
后序遍历（Post-Order Traversal）
	二叉树为空
		无操作直接返回
	二叉树不为空：
		中序遍历左子树
		中序遍历右子树
		访问根节点中的数据
*/
template <typename T>
void BTree<T>::PostOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue){
	if(node != NULL){
		PostOrderTraversal(node->left, queue);
		PostOrderTraversal(node->right, queue);
		queue.Add(node);
	}
}

/*
层次遍历：
	1.将根节点压入队列
	2.访问对头元素指向的二叉树节点
	3.对头元素弹出，将对头元素的孩子压入队列
	4.判断队列是否为空（非空：转2，空：结束）
*/
template <typename T>
void BTree<T>::LevelOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue){
	LinkQueue<BTreeNode<T>*> temp;
	if(node == Root()){
		temp.Add(node);
	}

	while( temp.Length() > 0){
		BTreeNode<T>* n = temp.Front();
		if(n->left){
			temp.Add(n->left);
		}

		if(n->right){
			temp.Add(n->right);
		}
		
		queue.Add(n);
		temp.Remove();
	}

}

/*
根据参数order选择执行遍历算法 （先序，中序，后续）
返回值为堆中的数组对象（声明周期由智能指针管理）
数组元素的次序反映了遍历的先后次序
*/
template <typename T>
void BTree<T>::_traversal(LinkQueue<BTreeNode<T>*>& queue, BTTraversal order){
	switch(order){
		case PreOrder:
			PreOrderTraversal(Root(), queue);
			break;
		case InOrder:
			InOrderTraversal(Root(), queue);
			break;
		case PostOrder:
			PostOrderTraversal(Root(), queue);
			break;
		case LevelOrder:
			LevelOrderTraversal(Root(), queue);
			break;
		default:
			THROW_EXCEPTION(InvalidParameterException, "parameter order is invalid...");
			break;
	}
}

//如果子树为空，那么返回一个空数组类
template <typename T>
SharedPointer<Array<T> > BTree<T>::Traversal(BTTraversal order){
	LinkQueue<BTreeNode<T>*> queue;
	
	_traversal(queue, order);
	
	int len = queue.Length();

	DynamicArray<T>* rst = new DynamicArray<T>(len);
	if(rst != NULL){
		for(int i = 0; i < len; i++, queue.Remove()){
			rst->Set(i, queue.Front()->value);
		}
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "no memory to creat dynamicarray...");
	}
	return rst;
}
/*
问题：
	1.树中的指针域只有n-1个有值，n+1个都是空闲状态
	2.树的主要操作时查找操作，每一次查询树以某种顺序进行遍历，都需执行一次递归，这会导致效率较低

解决方法1：利用所有空的指针域
			若左边指针域为空，指向其前驱节点
			若右边指针域为空，指向其后继节点
			为了区分时指向孩子节点，还是指向前去后记，需要对左右指针各加一个标记
线索化二叉树的优势在于利用空闲的指针域，将遍历的速度提升到O(n), 一般的二叉搜索树是无法得到后续遍历的，因为后续遍历的顺序是左右中，这样导致最后回到根节点，根节点指针都指向孩子，无法跳跃到下一个状态。层次遍历对于线索化二叉树而言帮助不大


解决方法2：将二叉树转换为一个双向链表
	在二叉树构建好之后不会轻易改动，主要的用途是用于查询，无需做更多的插入删除操作。这种情况可以将二叉树以某种次序转换成双向链表，进行查找

	如果是中序转换成链表，二叉树的左右孩子指针不会发生改变。其他三种顺序，二叉树的结构会被破坏

本库采用方法2
线索化二叉树：
	将二叉树转换为双向链表的过程（非线性 -> 线性）
	能够反应某种二叉树的遍历次序（节点的先后访问次序
	）
		利用节点的right指针指向遍历中的后继节点
		利用节点的left指针指向遍历中的前驱节点

	思维过程：
		线索化能够反应遍历层次 -> 线索化算法必然域遍历算法相关 -> 如何在遍历时记录节点间访问次序 -> 使用队列（先进先出） -> 遍历结束后，队列记录了访问次序 -> 循环访问队列，连接队列中的节点

线索化可以堪称是将二叉树转换为双向链表的过程
线索坏操作节点之间的先后次序符合某种遍历次序
线索化操作将破坏原二叉树节点间父子关系
线索化之后二叉树将不在管理节点的生命期

*/

/*
队列连接算法的实现：
	1.第一个元素出队，left = NULL
	2.将出队元素的right指向队列首节点
	3.将队列首节点left指向刚刚出队元素
	4.循环2，3直至队列为空，退出
*/
template <typename T>
BTreeNode<T>* BTree<T>::_connect(LinkQueue<BTreeNode<T>*>& queue){
	BTreeNode<T>* rst = NULL;
	if(queue.Length() > 0){

		rst = queue.Front();
		rst->left = NULL;
		queue.Remove();
		BTreeNode<T>* n = rst;

		while(queue.Length() > 0){
			n->right = queue.Front();
			queue.Front()->left = n;
		 	n = queue.Front();	
			queue.Remove();
		}
		n->right = NULL;
	}
	return rst;
}


/*
线索化函数接口设计
	根据order选择线索化次序
	返回值线索化之后指向链表首节点的指针
	线索化执行结束之后对应的二叉树变为空树

流程：
	建立队列；
	按某种顺序遍历二叉树，并保存如队列
	将队列中的元素连接成双向链表
	将根节点置空
*/
template <typename T>
BTreeNode<T>* BTree<T>::Thread(BTTraversal order){
	LinkQueue<BTreeNode<T>*> queue;
	_traversal(queue, order);
	
	BTreeNode<T>* rst = _connect(queue);

	//清除树的状体
	m_queue.Clear();
	this->m_root = NULL;

	return rst;
}

/*
二叉树的克隆操作
	克隆拷贝当前树
	返回值为堆空间中的一颗新二叉树（与当前树相同）
				node == NULL; return NULL;
clone(node)
				node != NULL				n = NewNode();
											n->value = node->value;
											n->left = clone(node->left);
											n->right = clone(node->right);
*/
template <typename T>
BTreeNode<T>* BTree<T>::_clone(const BTreeNode<T>* node) const{
	BTreeNode<T>* rst = NULL;
	if(node != NULL){
		rst = BTreeNode<T>::NewNode();
		if(rst == NULL){
			THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create btreenode...");
		}
		rst->value = node->value;
		rst->left = _clone(node->left);
		rst->right = _clone(node->right);

		//若左右子树不为空，更新左右子树的parent
		if(rst->left != NULL){
			rst->left->parent = rst;
		}	

		if(rst->right != NULL){
			rst->right->parent = rst;
		}
	}
	return rst;
}

template <typename T>
SharedPointer<BTree<T>> BTree<T>::Clone() const{
	BTree<T>* rst = new BTree<T>;
	if(rst != NULL){
		rst->m_root = _clone(Root());
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create btree ...");
	}
	return rst;
}


/*
二叉树比较操作
	判断两棵二叉树是否相等

二叉树比较函数
	判断lh为根节点的二叉树与rh为根节点的二叉树是否相等

				lh == rh	return true;

				lh == 0 && rh != 0	return false
equal(lh, rh)
				lh != 0 && rh == 0  return false

				lh != 0 && rh != 0 	lh->value == rh->value &&
									equal(lh->left, rh->left) &&
									equal(lh->right, rh->right)
*/
template <typename T>
bool BTree<T>::_equal(BTreeNode<T>* lt, BTreeNode<T>* rt) const{
	bool rst = false;
	//如果两个node的地址万千相同，那么这以这两个node为根的子树本质就是同一棵树，必然相同
	//lt,rt同时为NULL，即为退出退出条件，返回true，因为两边都是空树
	if(lt == rt){
		rst = true;
	}
	//若两边子树都不为空，检查当前节点的value值，并且检查左子树和右子树是否相等
	else if(lt != NULL && rt != NULL){
		rst =  lt->value == rt->value &&\
			   _equal(lt->left, rt->left) &&\
			   _equal(lt->right, rt->right);
	}
	//若只有其中一颗树为空树，必然不相等，返回rst 默认值false
	return rst;
}
template <typename T>
bool BTree<T>::operator == (const BTree<T>& btree) const{
	return _equal(Root(), btree.Root());
}
template <typename T>
bool BTree<T>::operator != (const BTree<T>& btree) const{
	return !((*this) == btree);
}

/*
二叉树的加法操作
	将当前二叉树与参数btree中的数据元素在对应位置处相加
	返回值（相加结果）为堆空间中的一颗新二叉树

	将lh为根节点的二叉树与rh为根节点的二叉树相加

			lh == 0 && rh != 0	_clone(rh);

			lh != 0 && rh == 0  _clone(lh);
add(lh, rh)
			lh ==0 && rh == 0	 return (退出条件)

			lh != 0 && rh != 0	n = NewNode();
								n->value = lh->value + rh->value;
								n->left = add(lh->left, rh->left);
								n->right = add(lh->right, rh->right);


*/
template <typename T>
BTreeNode<T>* BTree<T>::_add(const BTreeNode<T>* lt, const BTreeNode<T>* rt) const{
	BTreeNode<T>* rst = NULL;
	//两棵子树，其中有一个是非空的，将非空的树的直接克隆到待相加的树上去
	if(lt == NULL && rt != NULL){
		rst = _clone(rt);
	}
	else if(lt != NULL && rt == NULL){
		rst = _clone(lt);
	}
	//若子树都是非空的，将当前节点值相加，左子树相加，右子树相加
	else if(lt != NULL && rt != NULL){
		rst = BTreeNode<T>::NewNode();
		if( rst != NULL){
			rst->value = lt->value + rt->value;
			rst->left = _add(lt->left, rt->left);
			rst->right = _add(lt->right, rt->right);

			//对子节点的parent赋值，因为子节点的递归函数中，无法获知自己的parent
			if(rst->left != NULL){
				rst->left->parent = rst;
			}			
			if(rst->right != NULL){
				rst->right->parent = rst;
			}
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create btreenode...");
		}
	}
	//左右子树都为空，退出递归
	return rst;
}

template <typename T>
SharedPointer<BTree<T>> BTree<T>::Add(const BTree<T>& btree) const{
	BTree<T>* rst = new BTree<T>;
	if(rst != NULL){
		rst->m_root = _add(Root(), btree.Root());
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create btree ...");
	}
	return rst;
}



template <typename T>
BTree<T>:: ~BTree(){
	Clear();
}

}


/*
Test Code
	BTree<int> bt;
	BTreeNode<int>* n = NULL;

	bt.Insert(1);
	n = bt.Find(1);
	bt.Insert(2, n);
	bt.Insert(3, n);

	n = bt.Find(2);
	bt.Insert(4, n);
	bt.Insert(5, n);

	n = bt.Find(3);
	bt.Insert(6, n);
	bt.Insert(7, n);

	n = bt.Find(4);
	bt.Insert(8, n);
	bt.Insert(9, n);

	n = bt.Find(5);
	bt.Insert(10, n);
	// cout<<"###Thread test begin###"<<endl;
	// BTreeNode<int>* n2 = bt.Thread(BTree<int>::LevelOrder);
	// while(n2 != NULL){
	// 	cout<<n2->value<<" ";
	// 	n2 = n2->right;
	// }
	// cout<<endl;
	// cout<<"###Thread test end###"<<endl;
	cout<<"###add test begin###"<<endl;
	BTree<int> bt1;
	BTreeNode<int>* n1 = NULL;
	bt1.Insert(0);
	n1 = bt1.Find(0);
	bt1.Insert(6, n1);
	bt1.Insert(2, n1);

	n1 = bt1.Find(2);
	bt1.Insert(7, n1);
	bt1.Insert(8, n1);

	SharedPointer<BTree<int> > spad = bt.Add(bt1);
	int b[] = {8, 9, 10, 13, 15};

	for(int i = 0; i < 5; i++)
	{
		TreeNode<int>* node = spad->Find(b[i]);
		while(node){
			cout<<node->value<<" ";
			node = node->parent;
		}
		cout<<endl;
	}
	SharedPointer<Array<int> > spaa = NULL;
	spaa = spad->Traversal(BTree<int>::PreOrder);
	cout<<"order begin"<<endl;

	for(int i = 0; i < spaa->Length(); i++){
		cout<<(*spaa)[i]<<" ";
	}
	cout<<endl;
	cout<<"order end"<<endl;
	cout<<"Degree = "<<spad->Degree()<<endl;
	cout<<"Height = "<<spad->Height()<<endl;
	cout<<"Count = "<<spad->Count()<<endl;

	
	cout<<"###add test end###"<<endl;

	n = bt.Find(6);
	bt.Insert(11, n, BTree<int>::LEFT);
	n = bt.Find(11);
	bt.Insert(12, n, BTree<int>::RIGHT);

	

	cout<<"############################"<<endl;
	int a[] = {8, 9, 10, 12, 7};
	SharedPointer<Array<int> > spa = NULL;
	spa = bt.Traversal(BTree<int>::LevelOrder);
	cout<<"order begin"<<endl;

	for(int i = 0; i < spa->Length(); i++){
		cout<<(*spa)[i]<<" ";
	}

	cout<<"order end"<<endl;

	cout<<"clone test begin"<<endl;
	SharedPointer<BTree<int> > spc = bt.Clone();
	for(int i = 0; i < 5; i++)
	{
		TreeNode<int>* node = spc->Find(a[i]);
		while(node){
			cout<<node->value<<" ";
			node = node->parent;
		}
		cout<<endl;
	}
	cout<<"Degree = "<<spc->Degree()<<endl;
	cout<<"Height = "<<spc->Height()<<endl;
	cout<<"Count = "<<spc->Count()<<endl;
	cout<<"clone test end"<<endl;
	cout<<endl;
	cout<<"== operator test :   "<<((*spc) == bt)<<endl;
	cout<<endl;
	for(bt.Begin(); !bt.End(); bt.Next()){
		cout<<bt.Current()<<endl;
	}

	for(int i = 0; i < 5; i++)
	{
		TreeNode<int>* node = bt.Find(a[i]);
		while(node){
			cout<<node->value<<" ";
			node = node->parent;
		}
		cout<<endl;
	}
	cout<<"Degree = "<<bt.Degree()<<endl;
	cout<<"Height = "<<bt.Height()<<endl;
	cout<<"Count = "<<bt.Count()<<endl;

	cout<<"############################"<<endl;
	SharedPointer<Tree<int> > sp = bt.Remove(3);
	for(sp->Begin(); !sp->End(); sp->Next()){
		cout<<sp->Current()<<endl;
	}
	
	for(int i = 0; i < 5; i++)
	{
		TreeNode<int>* node = sp->Find(a[i]);
		while(node){
			cout<<node->value<<" ";
			node = node->parent;
		}
		cout<<endl;
	}
	cout<<"Degree = "<<sp->Degree()<<endl;
	cout<<"Height = "<<sp->Height()<<endl;
	cout<<"Count = "<<sp->Count()<<endl;
	cout<<"== operator test :   "<<((*sp) == bt)<<endl;
	cout<<"############################"<<endl;
	for(bt.Begin(); !bt.End(); bt.Next()){
		cout<<bt.Current()<<endl;
	}

	for(int i = 0; i < 5; i++)
	{
		TreeNode<int>* node = bt.Find(a[i]);
		while(node){
			cout<<node->value<<" ";
			node = node->parent;
		}
		cout<<endl;
	}
	cout<<"Degree = "<<bt.Degree()<<endl;
	cout<<"Height = "<<bt.Height()<<endl;
	cout<<"Count = "<<bt.Count()<<endl;

// 面试题
// BTree<int>* createTree(){
// 	BTree<int>* bt = new BTree<int>;
// 	BTreeNode<int>* n = NULL;

// 	bt->Insert(0);
// 	n = bt->Find(0);
// 	bt->Insert(1, n);
// 	bt->Insert(2, n);

// 	n = bt->Find(1);
// 	bt->Insert(3, n);

// 	n = bt->Find(3);
// 	bt->Insert(6, n);

// 	n = bt->Find(2);
// 	bt->Insert(4, n);
// 	bt->Insert(5, n);

// 	n = bt->Find(4);
// 	bt->Insert(7, n);

// 	n = bt->Find(5);
// 	bt->Insert(8, n);
// 	return bt;
// }

// 
// 单度节点删除
// 	编写一个用于删除二叉树中所有单度节点
// 	要求：节点删除后，其唯一的自界定啊替代它的位置
// 				node == NULL 	return

// delodd1(node)	degree == 1  	parent = node->parent;
// 								parent->child = node->child;
// 								node->child->parent = parent;
// 								delodd1(node->child);
// 				degree == 2
// 					or			delodd1(node->left);delodd1(node->right);
// 				degree == 0
// 返回值为树的首节点

// */
// template<typename T>
// BTreeNode<T>* DelOdd1(BTreeNode<T>* node){
// 	BTreeNode<T>* rst = node;
// 	if(node != NULL){
// 		int degree = !!node->left + !!node->right;
// 		if(degree == 1){
// 			BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);
// 			BTreeNode<T>* child = (node->left != NULL)? node->left : node->right;
// 			if(parent != NULL){
// 				((parent->left == node)? parent->left : parent->right) = child;
// 			}
// 			child->parent = parent;
// 			DelOdd1(child);

// 			if(node->Flag()){
// 				delete node;
// 			}
// 			rst = child;
// 		}
// 		else{
// 			DelOdd1(node->left);
// 			DelOdd1(node->right);
// 		}
// 	}
// 	return rst;
// }


// /*
// 不用parent指针
// 	不用parent指针，涉及到在递归函数中改变node->child的值，因此必须要传指针引用，或者指针的指针

// 因为传递的就是树的节点指针的引用，所以不需要返回值，递归函数内部就会更改node的值
// */
// template<typename T>
// void DelOdd2(BTreeNode<T>*& node){
// 	if(node != NULL){
// 		int degree = !!node->right + !!node->left;
		
// 		if(degree == 1){
// 			BTreeNode<T>* child = ((node->left) ? node->left : node->right);

// 			if(node->Flag()){
// 				delete node;
// 			}
// 			//node的是指针引用，将child赋值非node
// 			node = child;
// 			//不能DelOdd2(child)，child只是一个临时变量，node是指针引用，可以将值传出来
// 			DelOdd2(node);
// 		}
// 		else{
// 			DelOdd2(node->left);
// 			DelOdd2(node->right);
// 		} 
// 	}
// }

// /*
// 中序线索话
// 	要求：
// 		不允许使用其他数据结构
// 	思路：
// 		使用辅助指针，在中序遍历时指向当前节点的前驱
// 		访问当前节点时，连接与前驱节点的先后次序
// */
// template<typename T>
// void InOrderThread(BTreeNode<T>* node, BTreeNode<T>*& pre){
// 	if(node != NULL){
// 		//递归函数更新pre，因此执行完递归函数，pre的值不再是传进来的值，而是递归函数更新出来的值，所以要用指针引用来传递参数
// 		InOrderThread(node->left, pre);
// 		node->left = pre;

// 		if(pre!=NULL){
// 			pre->right = node;
// 		}

// 		pre = node;

// 		InOrderThread(node->right, pre);
		
// 	}
// }

// template<typename T>
// BTreeNode<T>* InOrderThread(BTreeNode<T>* node){
// 	BTreeNode<T>* pre = NULL;
// 	InOrderThread(node, pre);
// 	while(node != NULL && node->left != NULL){
// 		node = node->left;
		
// 	}

// 	return node;
// }

// /*
// 中序线索化2
// 	中序遍历节点的次序恰好是节点的水平次序
// 		思路：
// 			使用辅助指针，指向转换后双向链表的头节点和尾节点
// 			根节点与左右子树转换的双向链表连接，成为完整双向链表
// 								node == NULL return
// inOrderThread(node, head, tail)
// 								inOrderThread(node->left, h, t);
// 								node->left = t;
// 								t->right = node;
// 								inOrderThread(node->right, h,t);
// 								node->right = h;
// 								h->left = node;
// */

// template<typename T>
// void InOrderThread2(BTreeNode<T>* node, BTreeNode<T>*& head, BTreeNode<T>*& tail){
// 	if(node != NULL){
// 		BTreeNode<T>* h = NULL;
// 		BTreeNode<T>* t = NULL;

// 		InOrderThread2(node->left, h, t);
		
// 		head = h ? h : node;

// 		node->left = t;

// 		if(t != NULL){
// 			t->right = node;
// 		}
		
// 		//计算右子树，需要重新赋值h，t
// 		h = NULL;
// 		t = NULL;
// 		InOrderThread2(node->right, h, t);

// 		tail = t ? t : node;

// 		node->right = h;

// 		if(h != NULL){
// 			h->left = node;
// 		}
	
// 	}
// }
// template<typename T>
// BTreeNode<T>* InOrderThread2(BTreeNode<T>* node){
// 	BTreeNode<T>* head =  NULL;
// 	BTreeNode<T>* tail = NULL;

// 	InOrderThread2(node, head, tail);

// 	return head;
// }

// int main() {
// 	// BTree<int>* bt = createTree();
// 	// BTreeNode<int>* n =  DelOdd1(bt->Root());
// 	// BTree<int> bt1;
// 	// bt1.Insert(n);
// 	// cout<<(n->left->value)<<endl;
// 	// cout<<(n->right->value)<<endl;
// 	// cout<<bt1.Root()<<endl;
// 	// SharedPointer<Array<int> > a = bt1.Traversal(BTree<int>::PreOrder);
// 	// for(int i = 0; i < a->Length(); i++){
// 	// 	cout<<(*a)[i]<<" ";
// 	// }
// 	// cout<<endl;

// 	// BTree<int>* bt = createTree();
// 	// BTreeNode<int>* node = bt->Root();


// 	// DelOdd2(node);
// 	// BTree<int> bt1;
// 	// bt1.Insert(node);

	
// 	// SharedPointer<Array<int> > a = bt1.Traversal(BTree<int>::PreOrder);

// 	// for(int i = 0; i < a->Length(); i++){
// 	// 	cout<<(*a)[i]<<" ";
// 	// }
// 	// cout<<endl;
// 	// cout<<a->Length()<<endl;
	
// 	// BTree<int>* bt = createTree();
// 	// BTreeNode<int>* node =InOrderThread(bt->Root());

// 	// while(node->right != NULL){
// 	// 	cout<<node->value<<endl;
// 	// 	node= node->right;
// 	// }

// 	BTree<int>* bt = createTree();
// 	BTreeNode<int>* node =InOrderThread2(bt->Root());

// 	while(node->right != NULL){
// 		cout<<"node = "<<node->value<<endl;
// 		node= node->right;
// 	}


// 	return 0;
// }

#endif