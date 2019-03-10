#ifndef __BTREENODE_H__
#define __BTREENODE_H__

#include "TreeNode.h"

namespace YzcLib{

template< typename T>
class BTreeNode: public TreeNode<T>{
public:
	BTreeNode<T>* left;
	BTreeNode<T>* right;

	//初始化左右节点
	BTreeNode();
	
	//工厂函数
	static BTreeNode<T>* NewNode();

	bool Flag();
};


template< typename T>
BTreeNode<T>::BTreeNode(){
	left = NULL;
	right = NULL;
}


template<typename T>
BTreeNode<T>* BTreeNode<T>:: NewNode(){
	BTreeNode<T>* rst = new BTreeNode<T>;
	//如果申请成功，堆空间标志 = true
	if(rst != NULL){
		rst->m_flag = true;
	}
	
	return rst;
}

template<typename T>
bool BTreeNode<T>:: Flag(){
	return this->m_flag;
}


}


#endif