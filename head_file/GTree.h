#ifndef __GTREE_H__
#define __GTREE_H__

#include "Tree.h"
#include "GTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"

/*
设计要点
	GTree 为通用树（general tree），每个节点可以包含任意多个后继节点
	GTreeNode 能包含任意多指向后继节点的指针
	实现树结构的所有操作（增 删 查 等）
*/

namespace YzcLib{
template <typename T>
class GTree: public Tree<T>{

	LinkQueue<GTreeNode<T>*> m_queue;

	//如果允许外部使用拷贝或者赋值函数，会导致clear释放出问题，通用树结构不适合支持拷贝或者赋值，如果需要拷贝赋值，需要进行深拷贝
	GTree(const GTree<T>& );
	GTree<T>& operator = (const GTree<T>& ); 

	GTreeNode<T>* _find(GTreeNode<T>* node, T value) const;
	GTreeNode<T>* _find(GTreeNode<T>* node, GTreeNode<T>* obj) const;
	
	//清除操作功
	void _free(GTreeNode<T>* node);

	SharedPointer<Tree<T>>  _remove(TreeNode<T>* node, bool isnode = true);

	int _count(GTreeNode<T>* node) const;
	int _height(GTreeNode<T>* node) const;
	int _degree(GTreeNode<T>* node) const;
public:
	GTree();
	GTree( const GTreeNode<T>* root);
	
	bool Insert(TreeNode<T>* node );
	bool Insert(const T& value, TreeNode<T>* parent);
	//将元素从树中删除，之所以返回时只能指针类型是因为Remove会将该节点以及该节点的子树一并删除，所以将所删除的子树返回，这样就可以对即将被删除的子树进行处理，比如只删树中某个节点
	SharedPointer<Tree<T> > Remove(const T& value);
	SharedPointer<Tree<T> > Remove(TreeNode<T>* node);

	//基于数据元素的查找
	GTreeNode<T>* Find(const T& value) const;
	//基于节点的查找
	GTreeNode<T>* Find(TreeNode<T>* node) const;
	//获取树的根节点
	GTreeNode<T>* Root() const;
	//获取树的度
	int Degree() const;
	//获取树的节点数
	int Count() const;
	//获取树的高度
	int Height() const;
	// 清空树中的元素
	void Clear();
	
	//层次遍历的迭代器
	bool Begin();
	bool End();
	bool Next();
	T Current();


	~GTree();
};


template <typename T>
GTree<T>::	GTree(){

}

template <typename T>
GTree<T>::GTree( const GTreeNode<T>* root){
	this->m_root = root;
}

/*
问题分析：
	树是非线性的，无法采用下标的形式定位数据元素
	每一个树节点都有唯一的前驱节点（父节点）
	因此，必须先找到前驱系欸但那，才能完成新节点的插入
要点：
	插入操作是构建树唯一操作
	执行插入时，必须指明节点间的父子关系
	插入操作必须正确处理指向父节点的指针
	插入数据元素时需要从堆空间中创建节点

	Insert(node)
	1.判断根节点是否为空（是否是一颗空树）
	2.空树:	将node设置为根节点(将其双亲设置为NULL)
		非空：寻找双亲是否在树中
	3.双亲若不在树中，无法插入，抛参数异常
		若双亲在树中，判断node是否已经存在，若不存在，插入node到child中，结束程序
*/

//node->parent = NULL ,在根节点插入元素，iff树是空树
template <typename T>
bool GTree<T>::Insert(TreeNode<T>* node ){
	bool rst = true;
	//node = NULL，node->parent,调用不了，会抛系统异常，所以需要判断一下
	if(node != NULL){
		//不是空树
		if(this->m_root != NULL){
				GTreeNode<T>* temp = Find(node->parent);
				if(temp != NULL){
					GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);
					//链表，找不到返回-1,若node已经存在于child中，无需重复插入
					if(temp->child.Find(n) < 0){
						temp->child.Insert(n);
					}

				}
				else{
					THROW_EXCEPTION(InvalidParameterException, "invalid parent treenode ...");
				}
		}//是空树，在根节点插入元素
		else if(node->parent == NULL){
			this->m_root = node;
		}
	}
	else{
		THROW_EXCEPTION(InvalidParameterException, "Node can not be NULL");
	}

	return rst;
}

/*
先在堆空间创建node
对node赋值
使用Insert(node)将node插入
*/
template <typename T>
bool GTree<T>::Insert(const T& value, TreeNode<T>* parent){
	//错误都通过抛异常处理，不通过返回值，带返回值只是为了重写父类虚函数，因此rst默认为true
	bool rst = true;
	GTreeNode<T>* node = GTreeNode<T>:: NewNode();
	if(node != NULL){
		node->value = value;
		node->parent = parent;
		
		Insert(node);
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memeory to create gtree node ...");
	}

	return rst;	
}

/*
删除的方式
	基于数据元素的删除
	基于节点的删除

删除操作成员函数的设计
	将被删除节点所代表的子树删除
	删除函数返回一颗堆空间中的树
	具体返回值为指向树的智能指针对象（可以自动释放内存）

实用的设计原则
	当需要从函数中返回堆中的对象时，使用智能指针（SharedPointer）作为韩式的返回值

删除步骤：
	remove(node, ret)
	1.为返回值 rst————子树 申请内存
	2.判断node是否为根节点
	3.  是根节点：root = NULL；
		不是根节点
			a.找到node的parent
			b.从parent的child链表中删除node
			c.将node的parent置空
	4.将node作为返回子树的根节点，返回

设计要点
	删除操作将目标节点所代表的子树转移
	删除操作必须完善处理父节点和子节点的关系
	删除操作的返回值为指向树的智能指针对象
	函数中返回堆中对象时，使用智能指针作为返回值，防止内存泄漏

*/
template <typename T>
SharedPointer<Tree<T>> GTree<T>::_remove(TreeNode<T>* node, bool isnode){
	//要先申请rst内存，在执行删除操作，不然数据已经从树中删除，内存申请异常，删除的数据就会永久丢失
	GTree<T>* rst = new GTree;
	GTreeNode<T>* n = isnode ? \
						Find(dynamic_cast<GTreeNode<T>*>(node)):\
						dynamic_cast<GTreeNode<T>*>(node);
	if(rst == NULL){
		THROW_EXCEPTION(NotEnoughMemoryException, "no memory to create gtree node...");
	}

	if(n == NULL){
		THROW_EXCEPTION(InvalidParameterException, "Can not find the node via parameter value ...");
	}
	
	if(Root() == node){
		this->m_root = NULL;
	}
	else{
		LinkList<GTreeNode<T>*>& l = dynamic_cast<GTreeNode<T>*>(n->parent)->child;

		l.Remove(l.Find(n));

		n->parent = NULL;
	}

	rst->m_root = node;

	m_queue.Clear();
	
	return rst;	
}

template <typename T>
SharedPointer<Tree<T>> GTree<T>:: Remove(const T& value){
	GTreeNode<T>* node = Find(value);
	return _remove(node, 0);
}

template <typename T>
SharedPointer<Tree<T>> GTree<T>:: Remove(TreeNode<T>* node){
	return _remove(node);
}

/*
树中数据元素和节点的查找
	基于数据元素值的查找
	基于节点的查找

基于数据元素值的查找
	功能定义：
	基于元素值查找
		find(node, value)
			在node为根节点的树中查找value所在的节点
						node->value == value 	    return node; 
	find(node, value)
						node->value != value;       find(node->child, value);  

	基于节点的查找
		find(node, obj)
			在node为根节点的树中查找是否存在obj节点
	在node为根节点的树中查找obj节点
						node->value == obj; 	    return node; 
	find(node, value)
						node->value != obj;       find(node->child, obj);  

以上是递归方式的思路
如果用非递归查找：
	使用层次遍历的方式：借用队列来实现整个树节点的访问操作
	从队列中取出node
	检测 node == node->value ? 是，返回，清空队列 : 不是，将子节点压入队列，为空的子节点不入队
	队列中下一个node出队
*/			

template <typename T>
GTreeNode<T>* GTree<T>::_find(GTreeNode<T>* node, T value)const{
	GTreeNode<T>* rst = NULL;
	//退出条件，子节点为空（链表为空）
	if(node != NULL){
		if(node->value == value){
			return node;
		}
		else{
			//循环查找，退出条件1.遍历完整个链表 2.找到目标节点
			for(node->child.Move(0); !node->child.End() && (rst == NULL); node->child.Next()){
				rst = _find(node->child.Current(), value);
			}
		}

	}
	return rst;
}

template <typename T>
GTreeNode<T>* GTree<T>::_find(GTreeNode<T>* node, GTreeNode<T>* obj) const{
	GTreeNode<T>* rst = NULL;
	/*
	递归退出条件 
	1.obj = NULL,树的节点为非空，空意味着没有节点,这样当obj为空的时候不必递归到树的最后一层为NULL的child的时候再返回
	2.node == NULL，如果node == NULL，就意味着为空树
	*/
	if(node != NULL && obj != NULL){
		if(node == obj){
			return node;
		}
		else{
			for(node->child.Move(0); !node->child.End() && (rst == NULL); node->child.Next()){
				rst = _find(node->child.Current(), obj);
			}
		}	
	}

	return rst;
}


template <typename T>
GTreeNode<T>* GTree<T>:: Find(const T& value) const{
	return _find( Root(), value);
}

template <typename T>
GTreeNode<T>* GTree<T>:: Find(TreeNode<T>* node) const{
	/*node是父类，父类转子类需要用dynamic_cast来转换，
	dynamic_cast具备类型检测
	如果父类指针本身是由子类转换而来，那么返回子类指针，
	如果父类指针本身就是父类，那么无法转换成子类，就会返回NULL
	Root返回的根节点是强制转换过的，所以使用Root函数，而不用this->root;
	*/

	return _find(Root(), dynamic_cast<GTreeNode<T>*>(node));

}

template <typename T>
GTreeNode<T>* GTree<T>:: Root() const{
	/*node是父类，父类转子类需要用dynamic_cast来转换，
	dynamic_cast具备类型检测
	如果父类指针本身是由子类转换而来，那么返回子类指针，
	如果父类指针本身就是父类，那么无法转换成子类，就会返回NULL
	*/
	return dynamic_cast<GTreeNode<T>*>(this->m_root);
}

/*
树的度
	获取node为根节点的树的度数

				node == NULL;	return 0;

count(node)		

				node != NULL	max(degree(node->child), node->child.length)
*/
template <typename T>
int GTree<T>:: _degree(GTreeNode<T>* node) const{
	int rst = 0;
	if(node != NULL){
		rst = node->child.Length();
		for(node->child.Move(0); !node->child.End(); node->child.Next()){
			int d = _degree(node->child.Current());
			//寻找最大子树高度，如果找到更大的就更新
			(d > rst)&&(rst = d);
		}

	}
	return rst;
}


template <typename T>
int GTree<T>:: Degree() const{
	return _degree(Root());
}

/*
树中节点的数目
	以node为根节点的树中统计节点数目

				node == NULL;				return 0;

count(node)		node->child.length = 0		return 1;

				node->child.length > 0		sum(count(node->child)) + 1
*/
//需要传递node，但是Count()函数在父类是纯虚函数，必须要在子类实现，所以重新定义一个函数进行递归
template <typename T>
int GTree<T>:: _count(GTreeNode<T>* node) const{
	int rst = 0;
	if(node != NULL){
		//如果存在子节点，需要加上child的节点数,如果没有child，无法进入循环
		for(node->child.Move(0); !node->child.End(); node->child.Next()){
			rst += _count(node->child.Current());
		}
		//node本身需要加1
		rst += 1;
	}
	return rst;
}


template <typename T>
int GTree<T>:: Count() const{
	return _count(Root());
}

/*
树的高度
	定义功能：获取node为根节点的树的高度

				node == NULL;				return 0;

count(node)		node->child.length = 0		return 1;

				node->child.length > 0		max(hight(node->child))

*/
template <typename T>
int GTree<T>:: _height(GTreeNode<T>* node) const{
	int rst = 0;
	if(node != NULL){
		//计算child的高度. 如果存在子节点，需要加上child的节点数,如果没有child，无法进入循环
		for(node->child.Move(0); !node->child.End(); node->child.Next()){
			int h = _height(node->child.Current());
			//寻找最大子树高度，如果找到更大的就更新
			(h > rst)&&(rst = h);
		}
		// 加上自身高度
		rst += 1;
	}
	return rst;
}


template <typename T>
int GTree<T>:: Height() const{
	return _height(Root());
}

/*
清楚操作功能定义
	清楚node为根节点的树
	释放树中每一个节点(从堆空间)
					node == NULL
	_free(node)
					node != NULL  free(node->child);
								  delete node;

清除操作，用于销毁树中每个节点
销毁节点时需要决定是否释放对应的内存空间
工厂模式可以用于定制堆空间中的节点
只有销毁定制节点的时候需要进行释放
*/
template <typename T>
void GTree<T>:: _free(GTreeNode<T>* node){
	//node为空————空树，删除完毕，退出循环
	if(node != NULL){
		for(node->child.Move(0); !node->child.End(); node->child.Next()){
				_free(node->child.Current());
		}
	}

		//检测当前节点是否在堆空间中，如果是，需要释放
		if(node->GetFlag()){
			delete node;
			cout<<"###"<<(node->value)<<endl;
		}
		else{
			cout<<"***"<<(node->value)<<endl;
		}
		m_queue.Clear();
}


template <typename T>
void GTree<T>:: Clear(){
	_free(Root());
	this->m_root = NULL;
}


//层次遍历迭代器
template <typename T>
bool GTree<T>::Begin(){
	bool rst = (this->m_root != NULL);
	//空树直接返回，不入队列
	if(rst){
		//先对队列进行清除，防止之前一轮队列没有被清除干净
		m_queue.Clear();
		m_queue.Add(Root());
	}
	return rst;
}

template <typename T>
bool GTree<T>:: End(){
	//remove和clear操作需要将m_queue清空，所以无需检测m_root是否为空
	return (m_queue.Length() == 0);
}

template <typename T>
bool GTree<T>:: Next(){
	bool rst = (!End());
	//如果队列不为空
	if(rst){
		GTreeNode<T>* node = m_queue.Front();

		m_queue.Remove();

		for(node->child.Move(0); !node->child.End(); node->child.Next()){
			m_queue.Add(node->child.Current());
		}
	}
	return rst;
}

template <typename T>
T GTree<T>:: Current(){
	if(!End()){
		return (m_queue.Front()->value);
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No value at current position... ");
	}
}



template <typename T>
GTree<T>::~GTree(){
	Clear();
}

/*
Test code
	GTree<char> t;
	GTreeNode<char>* node = NULL;

	t.Insert('A', NULL);

	node = t.Find('A');
	t.Insert('B', node);
	t.Insert('C', node);
	t.Insert('D', node);

	node = t.Find('B');
	t.Insert('E', node);
	t.Insert('F', node);

	node = t.Find('E');
	t.Insert('K', node);
	t.Insert('L', node);

	node = t.Find('C');
	t.Insert('G', node);

	node = t.Find('D');
	t.Insert('H', node);
	t.Insert('I', node);
	t.Insert('J', node);

	node = t.Find('H');
	t.Insert('M', node);


	const char* s = "KLFGMIJ"; //加const使长度确定，消除警告
	cout<<"###"<<endl;
	for(t.Begin(); !t.End(); t.Next()){
		cout<<t.Current()<<endl;
	}
	cout<<"###"<<endl;

	for(int i = 0; i < 7; i++){
		TreeNode<char>* n = t.Find(s[i]);
		while(n != NULL){
			cout<<n->value<<" ";
			n = n->parent;
		}
	cout<<endl;
	}
	cout<<"Degree = "<<t.Degree()<<endl;
	cout<<"Hight = "<<t.Height()<<endl;
	cout<<"Count = "<<t.Count()<<endl;

	SharedPointer<Tree<char> > n1 = t.Remove('D');
	SharedPointer<Tree<char> > n2 = t.Remove('B');

	cout<<"sub tree n1"<<endl;
	for(int i = 0; i < 7; i++){
		TreeNode<char>* n = n1->Find(s[i]);
		while(n != NULL){
			cout<<n->value<<" ";
			n = n->parent;
		}
	cout<<endl;
	}
	cout<<"Degree = "<<n1->Degree()<<endl;
	cout<<"Hight = "<<n1->Height()<<endl;
	cout<<"Count = "<<n1->Count()<<endl;


	cout<<"sub tree n2"<<endl;
	for(int i = 0; i < 7; i++){
		TreeNode<char>* n = n2->Find(s[i]);
		while(n != NULL){
			cout<<n->value<<" ";
			n = n->parent;
		}
	cout<<endl;
	}

	cout<<"Degree = "<<n2->Degree()<<endl;
	cout<<"Hight = "<<n2->Height()<<endl;
	cout<<"Count = "<<n2->Count()<<endl;

	cout<<"the rest of tree"<<endl;
	for(int i = 0; i < 7; i++){
		TreeNode<char>* n = t.Find(s[i]);
		while(n != NULL){
			cout<<n->value<<" ";
			n = n->parent;
		}
	cout<<endl;
	}
	cout<<"Degree = "<<t.Degree()<<endl;
	cout<<"Hight = "<<t.Height()<<endl;
	cout<<"Count = "<<t.Count()<<endl;
*/
}

#endif