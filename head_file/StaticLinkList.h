#ifndef __STATICLINKLIST_H__
#define __STATICLINKLIST_H__

#include "LinkList.h"
/*
单链表的缺陷
--触发条件
	长时间使用单链表对象频繁增加和删除元素
--可能结果
	堆空间产生大量的内存碎片，当有一大片内存需要申请空间的时候，没有足够大的堆空间片段，系统需要进行内存管理，导致系统运行缓慢

静态链表：
静态链表是对对数据元素最大个数固定的情况，对单链表的一种优化
设计思路
在单链表的内部增加一片预留空间，所有空间都在这片空间中动态创建和动态销毁
顺序表 + 单链表 = 静态单链表

静态链表
data域用于存储
next域用于存储下一个元素在数组中的地址（也可以是下标）
*/

//关于程序内存分配的问题
/*
静态链表而言，静态链表中的成员数组都和该对象存储在一起，即数组成员变量不会new一个新的区域去存储。
静态链表中有两个大数组，很占内存，因此建议静态链表放到堆上去。
*/
namespace YzcLib{
//因为是静态的，所以长度N需要定义时指定
template<typename T, unsigned int N>
class StaticLinkList: public LinkList<T>{
protected:
//因为要用到LinkList<T>::Node,变量名相对繁杂，因此通过typedef对变量进行重命名
//typedef方式的重命名对于模板只能在模板类内部使用，模板类外部使用的时候，编译器在第一次编译的时候会忽略泛指类型参数，因此会找不到与之匹配的参数
//只能在类的内部使用，不能在类的外部调用，编译器无法自主推导
	typedef typename LinkList<T>::Node Node;
	struct SNode: public Node{
		//静态链表需要对每一个Node分配内存并且调用构造函数对其初始化。因此需要重载placement new。
		//palcement new主要的用途是在已经分配好的指定内存上构建对象，适用于已经分配空间，但是没有调用构造函数初始化对象的情况。
		//注意：不能使用delete对对象进行销毁，但是需要显式调用析构函数，销毁对象
		void* operator new (size_t size, void* loc){
			return loc;
		}
	};
	unsigned char space[sizeof(Node)*N];
	//标志对应的空间是否被使用，true已被用，false没有使用。
	bool is_used[N];
	//重写create函数，构造Node
	Node* _create();
	//重写destroy函数
	void _destroy(Node* n);
public:
	StaticLinkList();
	unsigned int Capacity();
	//对于析构函数，只要是基类中析构函数是虚的，那么后边的子类无论是否写virtual关键字，默认虚属性被继承。
	//对于静态链表，析构函数需要重写，主要原因是，destroy函数需要使用子类的，所以需要在子类中调用clear函数。
	//子类调用clear函数后，子类中的SNode都被清除，头指针指向NULL，因此~StaticLinkList()调用结束后，进入~LinkList()，~LinkList()中的clear函数发现头指针指向NULL，直接退出。
	//另外，如果不重写~StaticLinkList()，默认调用LinkList的destroy函数，导致placement new所申请的空间被释放而报错
	~StaticLinkList();

};

template<typename T, unsigned int N>
typename StaticLinkList<T,N>::Node* StaticLinkList<T,N>::_create(){
	SNode* rst = NULL;
	for(int i = 0; i < Capacity(); i++){

		if(is_used[i] == false){
			rst = reinterpret_cast<SNode*>(space) + i;
			//通过placement new调用SNode的构造函数。
			rst = new(rst)SNode;
			is_used[i] = true;
			break;
		}
	}
	return rst;
}

template<typename T, unsigned int N>
void StaticLinkList<T,N>::_destroy(Node* n){
	SNode* m_space = reinterpret_cast<SNode*>(n);
	SNode* m_SNode = dynamic_cast<SNode*>(n);
	for(int i = 0; i<N; i++){
		if(m_SNode == m_space + i){
			is_used[i] = false;
			//palcement new需要手工调用析构函数
			m_SNode->~SNode();
			break;
		}
	}
	// cout<<"~StaticLinkList()"<<endl;
}

template<typename T, unsigned int N>
StaticLinkList<T,N>::StaticLinkList(){
	for(int i = 0; i < N; i++){
		is_used[i] = false;
	}
}

template<typename T, unsigned int N>
unsigned int StaticLinkList<T,N>::Capacity(){
	return N;
}

template<typename T, unsigned int N>
StaticLinkList<T,N>::~StaticLinkList(){
	this->Clear();
	cout<<"~StaticLinkList()"<<endl;
}


//Test code
/*
	StaticLinkList<int,5> list;
	for(int i = 0; i < 5; i++){
		list.Insert(i);
	}
	cout<<list.Length()<<endl;
	
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}

	list.Remove(0);
	cout<<list.Length()<<endl;
	
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}

	try{
		list.Insert(6);
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
	}
	cout<<endl;

	
result:
5
0
1
2
3
4
~Object()
4
1
2
3
4

0x7fffcd98d998
~Object()
~Object()
~Object()
~Object()
~Object()
~StaticLinkList()
~LinkList()2
0
~LinkList()1
~Object()
~Object()
*/

}

#endif