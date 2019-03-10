#ifndef __DUALCIRCLELIST_H__
#define __DUALCIRCLELIST_H__

#include "DualLinkList.h"
#include "LinuxList.h"


/*
双向循环链表 = 通过 linux内核链表 + YzcLib
YzcLib中链表头节点时独立链表之外的，仅用于节点定位
实现思路
	--通过模板定义DualCircleList类，继承自DualLinkList类
	--在DualCircleList内部使用linux内核链表进行实现
	--使用struct list_head定义DualCircleList的头节点
	--特殊处理：循环遍历时忽略头节点
实现要点:
	--通过list_head进行目标定位(_position(i))
	--通过list_entry将list_head指针转换为目标节点指针
	--通过list_for_each 实现int find(const T& e)函数
	--遍历函数中的next()和pre()需要考虑跳过头节点
*/
/*
如果父类定义了_create()和_destroy()函数
不能使用父类的_create()和_destroy()函数。
因为多态只对成员函数而言，对于成员变量和私有成员类型不会有多态行为。
这也就意味在调用父类的函数(子类没有重写)调用的永远是父类的成员变量和自定义类型。
子类的成员变量和私有类行在调用子类的函数中会发生同名覆盖。
所以如果_create()只在父类定义了，只会return new Node中Node是父类的类型，并非双向循环链表的Node类型。因此直接用new和delete即可，不必重写_create()和_destroy()函数
*/

// offsetof和container_of的重定义,c++对于这两个宏会报警告，因为DualCircleList有继承关系，涉及到虚函数重写，不是standard layout type，在C++编译器这么做是危险的，而爆出警告。因此为了避免警告，自定义宏将其避让
//指针有可能是int，有可能是long，因此采用size_t类型进行运算，其会根据平台自动调整。
// #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define OFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)


// #define container_of123(ptr, type, member) ((type *)((char *)ptr - offsetof123(type,member)))
//一种通用的类型检测方式，保证类型的安全性，尽管在三目操作符中答案是确定的，但是编译器会检测(ptr)和&((type *)0)->member)是不是兼容的地址。
// #define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))
#define CONTAINER_0F(ptr, type, member) ((type*)((char*) (1?(ptr) : &((type*)0)->member) - OFFSETOF(type, member)))


/*

*/


namespace YzcLib{

template<typename T>
class DualCircleList: public DualLinkList<T>{
protected:
	struct Node: public Object{
		T value;
		list_head node;
	};

	list_head head;
	//链表许多操作都需要循环遍历，所以设置游标，提高效率
	list_head* cursor;
	//游标遍历模式
	bool model;

	list_head* _position(int i) const;
	//封装create和destroy为静态链表的实现做准备。
	//创建一个node，方便使用多态，在静态链表中重写create()
	int _mod(int i)const;
public:
	DualCircleList();
	//元素插入
	bool Insert(int i, const T& e); 
	//尾部插入元素
	bool Insert(const T& e); 
	//元素删除
	bool Remove(int i);				
	//获取目标位置元素的值
	bool Set(int i, const T& e);	
	//设置目标位置元素的值
	bool Get(int i, T& e)const;		
	//上一个Get使用起来不方便，对Get进行重载，遇到错误直接抛异常
	T Get(int i)const;	
	//获取目标元素的位置
	int Find(const T& e) const;
	//获取线性表的长度
	unsigned int Length() const;	
	//清空线性表
	//Clear函数使用父类的即可。在析构函数中，Clear()函数不会发生多态，只会调用当前类中的版本。所以正好调用子类重写的remove函数。
	//如果不显示声明，外界无法调用Clear(),因为父类的Clear函数在模板的第一次编译是不可见的，会在第一次编译报错，找不到Clear函数，因此可以考虑重载Clear()函数
	// void Clear();
	// 析构函数
	~DualCircleList();
	//游标的所有操作
	bool Move(int i, unsigned int step = 1,bool model = 0);//将游标定位到目标位置（初始化游标【起始位置，步长】）
	bool Next();//移动游标
	bool Pre();
	T Current();//获取游标所指向的数据元素
	bool End();//游标是否到达尾部（是否为空）

};

//使用前在外部检测合法性
//N个元素
//前 N/2+1 用next寻找，后 N/2 - 1 个用pre寻找
template<typename T>
list_head* DualCircleList<T>::_position(int i) const{

	list_head* current = const_cast<list_head*>(&head);
	if(i <= (this->length/2)){
		for(int k  = 0; k < i; k++){
			current = current->next;
		}
	}
	else{
		for(int k  = this->length - 1; k >= i - 1; k--){
			current = current->prev;
		}
	}


	return current;
}

template<typename T>
int DualCircleList<T>:: _mod(int i)const{
	return (this->length)?(i % this->length):0;
}

template<typename T>
DualCircleList<T>:: DualCircleList(){
	INIT_LIST_HEAD(&head);

	this->length = 0;

	cursor = NULL;
	this->step = 1;
	model = 0;
}

/*
链表：__ 1 __ 2 __ 3 __
i =   0    1    2   3
*/
template<typename T>
bool DualCircleList<T>::Insert(int i, const T& e){
	i = i%(this->length + 1);
	Node* node = new Node;
	bool rst = (i >= 0)&&( i <= this->length);

	if(rst){
		if(node){
			list_head* current = _position(i);
			node->value = e;
			__list_add(&(node->node), current, current->next);
			this->length ++;
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException,"No memory to insert new Element ...");
		}
	}


	return rst;
}

template<typename T>
bool DualCircleList<T>::Insert(const T& e){
	return Insert(this->length, e);
}

template<typename T>
bool DualCircleList<T>::Remove(int i){
	i = _mod(i);
	bool rst = (i >= 0)&&(i < this->length);
	if(rst){
		list_head* toDel = _position(i)->next;
		if(toDel == cursor){
			cursor = toDel->next;
		}
		list_del(toDel);
		this->length--;
		delete CONTAINER_0F(toDel, Node, node);
	}
	return rst;
}

template<typename T>
bool DualCircleList<T>::Set(int i, const T& e){
	i = _mod(i);
	bool rst = (i >= 0)&&(i < this->length);
	if(rst){
		CONTAINER_0F(_position(i)->next,Node, node)->value = e;
	}
	return rst;
}


template<typename T>
bool DualCircleList<T>::Get(int i, T& e)const{
	i = _mod(i);
	bool rst = (i >= 0)&&(i < this->length);
	if(rst){
		e = CONTAINER_0F(_position(i)->next, Node, node)->value;
	}
	return rst;
}

template<typename T>
T DualCircleList<T>::Get(int i)const{
	T rst;
	if(Get(i,rst)){
		return rst;
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i to get element ...");
	}
}

template<typename T>
int DualCircleList<T>::Find(const T& e) const{
	int rst = -1;
	//指针初始化为NULL，防止野指针
	list_head* slider = NULL;
	unsigned int i = 0;

	list_for_each(slider, &head){
		if(CONTAINER_0F(slider, Node, node)->value == e){
			rst = i;
			break;
		}
		i++;
	}
	return rst;
}
template<typename T>
unsigned int DualCircleList<T>::Length() const{
	return this->length;
}




template<typename T>
DualCircleList<T>::~DualCircleList(){
	this->Clear();
}


/*
head节点被跳过，从head->next开始
model0: 单链表模式，End标识为头节点
model1：循环链表模式， End标识为链表为空
*/
template<typename T>
bool DualCircleList<T>::Move(int i, unsigned int step , bool model){

	i = _mod(i);
	//要保证步长是大于0的
	bool rst = (i >= 0)&&(i < this->length)&&(step > 0);

	if(rst){
		cursor = _position(i)->next;
		this->step = step;
		this->model = model;
	}

	return rst;
}

template<typename T>
bool DualCircleList<T>::Next(){
	int i = 0;
	while( i < this->step && !End()){
		cursor = cursor->next;
		if(cursor == &head){
			continue;
		}
		i++;
	}
	if(!End() && cursor == &head){
		cursor = cursor->next;
	}
	return i == this->step;
}


template<typename T>
bool DualCircleList<T>::Pre(){
	int i = 0;
	while(i < this->step && !End()){
		cursor = cursor->prev;
		if(cursor == &head){
			continue;
		}
		i++;
	}

	if(!End() && cursor == &head){
		cursor = cursor->prev;
	}

	return i == this->step;
}

//空链表访问时报错
template<typename T>
T DualCircleList<T>::Current(){
	if(!End()){
		return CONTAINER_0F(cursor ,Node ,node)->value;
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
	}
}


template<typename T>
bool DualCircleList<T>:: End(){
	bool rst = (cursor == &head);
	if(model != 0){
		//对应两种情况，一种是游标没有初始化，另一种是链表为空
		rst = (cursor == NULL)||(this->length == 0);
	}
	return rst;
}

/*
Test code:
	DualCircleList<int> list;
	
	for(int i = 0; i < 5; i++){
		list.Insert(i);
		list.Insert(5);
	}
	cout<<endl;
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	cout<<endl;
	int i = 0;
	for(list.Move(0,1,1); !list.End(); list.Next()){
		if(i>8){
			break;
		}
		cout<<"list.End() = "<<list.End()<<endl;
		cout<<list.Current()<<endl;
		i++;
	}
	cout<<endl;
	for(list.Move(list.Length() - 1); !list.End(); list.Pre()){
		cout<<list.Current()<<endl;
	}
	cout<<endl;
	cout<<"begin"<<endl;
	list.Move(list.Length() - 1);
	while(! list.End()){
		if(list.Current() == 5){
			cout<<list.Current()<<endl;
			list.Remove(list.Find(list.Current()));
		}
		else{
			list.Pre();
		}
	}

	cout<<"end"<<endl;
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}
result:
new():size = 32
new():size = 32
new():size = 32
new():size = 32
new():size = 32
new():size = 32
new():size = 32
new():size = 32
new():size = 32
new():size = 32

0
5
1
5
2
5
3
5
4
5

list.End() = 0
0
list.End() = 0
5
list.End() = 0
1
list.End() = 0
5
list.End() = 0
2
list.End() = 0
5
list.End() = 0
3
list.End() = 0
5
list.End() = 0
4

5
4
5
3
5
2
5
1
5
0

begin
5
~Object()
delete:pointor = 0x561abd5ce2b0
5
~Object()
delete:pointor = 0x561abd5ce310
5
~Object()
delete:pointor = 0x561abd5ce370
5
~Object()
delete:pointor = 0x561abd5ce3d0
5
~Object()
delete:pointor = 0x561abd5ce430
end
0
1
2
3
4
~Object()
delete:pointor = 0x561abd5ce280
~Object()
delete:pointor = 0x561abd5ce2e0
~Object()
delete:pointor = 0x561abd5ce340
~Object()
delete:pointor = 0x561abd5ce3a0
~Object()
delete:pointor = 0x561abd5ce400
~Object()
~Object()
*/

}
#endif