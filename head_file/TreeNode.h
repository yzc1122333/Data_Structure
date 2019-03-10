#ifndef __TREENODE_H__
#define __TREENODE_H__

#include "Object.h"
//TreeNode也是一个抽象父类
namespace YzcLib{

template <typename T>
class TreeNode: public Object{
protected:
	bool m_flag;
	/*
	C++中重载决议是在可访问性检查之前进行的，先找到重载函数位置，在看是否可以访问，如果不能就抛异常
	*/
	//new重载为保护成员函数
	void* operator new(size_t size) throw(){
		//如果想以函数的形式调用重载操作符，需要加上operator，不加默认是以操作符的形式调用，编译器会进行一些加工
		return Object::operator new(size);
	}
	//树的节点有孩子和双亲的指针，拷贝后可能出现问题，因此，禁止拷贝和赋值,如果需要拷贝或者赋值，就整棵树一起深拷贝
	
	TreeNode(const TreeNode<T>&);
	TreeNode<T>& operator = (const TreeNode<T>& );

public:
	T value;
	/*
	每个树中为什么包含指向前驱节点的指针？
		根节点->叶节点：非线性数据结构 （树）
		叶节点->根节点： 线性数据结构（链表）
	*/
	TreeNode<T>* parent;

	TreeNode();
	//为了让TreeNode成为抽象父类，虚构函数为纯虚析构函数
	virtual ~TreeNode() = 0;

};

template <typename T>
TreeNode<T>::TreeNode(){
	//初始化m_flag为false，如果在栈上申请成功，最终会将m_flag在NewNode()中赋值为true
	m_flag = false;
	parent = NULL;
}


//纯虚析构函数要给出实现
template <typename T>
TreeNode<T>::~TreeNode(){
	
}


}

#endif