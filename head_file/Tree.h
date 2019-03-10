#ifndef __TREE_H__
#define __TREE_H__

#include "Object.h"
#include "TreeNode.h"
/*
树是一种非线性的数据结构
树是由n(n >= 0)个节点组成的有限集合
	如果 n = 0, 成为空树
	如果 n > 0, 则：
		有一个特定的称之为根的节点
		根节点只有直接后继，但没有直接前驱
		除根以外的其他节点划分为m(m >= 0)个互补相交的有限集合T0, T1, ..., Tm-1,每个集合又是一棵树，并且称之为根的子树（sub tree），子树的本质就是一个树的指针
树中度的概念
	树的节点包含了一个数据及若干指向子树的分支
	节点拥有的子树数目成为节点的度
		度为0的节点称之为叶节点
		度不为0的节点称之为分支节点
	树的度定义为所有节点中度的最大值

树中的前驱和后继
	节点的直接后继成为该节点的孩子
		相应的，该节点称为孩子的双亲
	节点的孩子的孩子的孩子...称为该节点的子孙
		相应的，该节点称为子孙的祖先
	同一个双亲的孩子之间互称兄弟

树中节点的层次：									 ______
						A			------ 1层        |
					   / \							  |
					  /   \							  |	
				     /     \						  |
					B       C		------ 2层	  高度 = 4
				   / \     / \						  |
				  /   \   /   \						  |
				 D     E F     G	------ 3层        |
				 	  /          \					  |
					H             I	------ 4层     ___|___
树中的最大节点称之为树的深度或者高度

树的有序性
	如果树中节点的各子树从左向右是有次序的，子树间不能相互换位，增城该树为有序树，否则为无序树

森林的概念
	森林是由n(n >= 0)棵互不相交的树组成的集合



*/
namespace YzcLib{

template<typename T>
class Tree: public Object{
protected:
	TreeNode<T>* m_root;
public:
	Tree(){m_root = NULL;}
	//将元素插入树中
	virtual bool Insert(TreeNode<T>* node ) = 0;
	virtual bool Insert(const T& value, TreeNode<T>* parent) = 0;
	//将元素从树中删除，之所以返回时只能指针类型是因为Remove会将该节点以及该节点的子树一并删除，所以将所删除的子树返回，这样就可以对即将被删除的子树进行处理，比如只删树中某个节点
	virtual SharedPointer<Tree<T>> Remove(const T& value) = 0;
	virtual SharedPointer<Tree<T>> Remove(TreeNode<T>* node) = 0;

	virtual TreeNode<T>* Find(const T& value) const = 0;
	virtual TreeNode<T>* Find(TreeNode<T>* node) const = 0;
	//获取树的根节点
	virtual TreeNode<T>* Root() const = 0;
	//获取树的度
	virtual int Degree() const = 0;
	//获取树的节点数
	virtual int Count() const = 0;
	//获取树的高度
	virtual int Height() const = 0;
	// 清空树中的元素
	virtual void Clear()  = 0;
	/*树的层次遍历
	  	问题：如何按层次遍历通用树结构中的每一个数据元素

		当前事实：树是非线性的数据结构，树的节点没有固定的编号方式

		新的需求：为通用树结构提供方法，快速遍历每一个节点
	
		设计思路：
			在树中定义一个游标（GTreeNode<T>*）
			遍历开始前将游标指向根节点
			获取游标指向的数据元素
			通过节点中child成员移动游标

		提供一组遍历相关函数， 按层次访问树中的数据元素
		begin() 初始化，准备进行遍历访问
		next()	移动游标，指向下一个节点
		current() 获取游标所指向数据元素
		end()	判断游标是否到达尾部

		层次遍历算法
			原料：class LinkQueue<T>
			游标：LinkQueue<T>::Front();
			思想：
				begin()	将根节点压入队列中
				current() 访问队头指定元素
				next()	对头元素弹出，将对头元素的孩子压入队列
				end()	判断队列是否为空

	遍历成员函数是相互依赖，相互配合的关系
	遍历算法的核心为队列的使用
	*/
	virtual bool Begin() = 0;
	virtual bool End() = 0;
	virtual bool Next() = 0;
	virtual T Current() = 0;
};

}

#endif