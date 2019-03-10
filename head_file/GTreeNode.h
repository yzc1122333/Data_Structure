#ifndef __GTREENODE_H__
#define __GTREENODE_H__

#include "TreeNode.h"

namespace YzcLib{
/*
问题：free如何判断node所在空间
问题分析：
	单凭内存地址河南准确判断具体的存储区域，内存是可配置的
	只有堆空间的内存需要主动释放(delete),释放其他空间可能会产生无法想象的后果
	清除操作时只需要对堆空间的节点进行释放
解决方法：工厂模式（只有一个工厂的简单工厂）
	1.在GTreeNode中增加保护成员变量 m_flag
		m_flag标志着对象是否存储于堆空间中
	2.在GTreeNode中的operator new 重载为保护成员函数，使外部无法调用
	3.提供工厂方法（静态成员函数）GTreeNode<T>* NewNode()
	4.在工厂方法中new 新节点并将m_flag设置为true
*/

template <typename T>
class GTreeNode: public TreeNode<T>
{

public:


	//因为通用树可以有任意多后继，所以需要用链表来装一个节点的子树
	LinkList<GTreeNode<T>*> child;

	//工厂方法,m_flag赋值
	static GTreeNode<T>* NewNode();
	
	bool GetFlag();
	
};



template <typename T>
GTreeNode<T>* GTreeNode<T>:: NewNode(){
	GTreeNode<T>* rst = new GTreeNode<T>;
	//如果没申请成功，就没有m_flag,所以申请失败就不需要做处理
	if(rst){
		rst->m_flag = true;
	}
	return rst;
}

//通过GetFlag()判断是否在堆空间
template <typename T>
bool GTreeNode<T>:: GetFlag(){
	return this->m_flag;
}



}

#endif