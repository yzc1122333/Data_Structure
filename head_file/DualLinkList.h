#ifndef __DUALLINKLIST_H__
#define __DUALLINKLIST_H__

#include "List.h"
/*
单链表另一个缺陷
--单向性
	智能从头结点开始高效的访问链表中的数据元素O(n)
--缺陷
	如果需要逆向访问单链表中的元素，效率将极其低下O(n^2)

设计思路：在单链表中增加一个指针pre，用于指向当前节点的前驱节点。

双向链表不是单链表，没有继承关系，DualLinkList继承自List
双向链表中的游标能够直接访问当前节点的前驱和后继

双向链表是线性表概念的最终实现(更贴近理论上的线性表)

DualLinkList和LinkList有很多相同的部分，因此DualLinkList是在LinkList上改进的。

有的库将DualLinkList设计成LinkList子类。

插入的时候头节点和尾节点要特殊处理。

*/
//因为没有双向链表的静态链表，所以可以不重载_create()和_destroy()

namespace YzcLib{

template<typename T>
class DualLinkList: public List<T>{
protected:
	struct Node: public Object{
		T value;
		Node* next;
		Node* pre;
	};

	mutable struct: public Object {
		char reserved[sizeof(T)];
		Node* next;
		Node* pre;
	} head;


	//链表许多操作都需要循环遍历，所以设置游标，提高效率
	Node* cursor;
	//cursor的步长
	unsigned int step;

	unsigned int length;
	Node* _position(int i) const;
	//封装create和destroy为静态链表的实现做准备。
	//创建一个node，方便使用多态，在静态链表中重写create()
	virtual Node* _create();
	//销毁一个node，方便使用多态，在静态链表中重写destroy()
	virtual void _destroy(Node* n);
public:
	DualLinkList();
	//元素插入
	virtual bool Insert(int i, const T& e); 
	//尾部插入元素
	virtual bool Insert(const T& e); 
	//元素删除
	virtual bool Remove(int i);				
	//获取目标位置元素的值
	virtual bool Set(int i, const T& e);	
	//设置目标位置元素的值
	virtual bool Get(int i, T& e)const;		
	//上一个Get使用起来不方便，对Get进行重载，遇到错误直接抛异常
	virtual T Get(int i)const;	
	//获取目标元素的位置
	virtual int Find(const T& e) const;
	//获取线性表的长度
	virtual unsigned int Length() const;	
	//清空线性表
	virtual void Clear();
	//析构函数
	~DualLinkList();
	//游标的所有操作
	virtual bool Move(int i, unsigned int step = 1);//将游标定位到目标位置（初始化游标【起始位置，步长】）
	virtual bool Next();//移动游标
	virtual bool Pre();
	virtual T Current();//获取游标所指向的数据元素
	virtual bool End();//游标是否到达尾部（是否为空）
};

//DualLinkList<T>::Node*前边要加typename，用来区分是静态变量还是自定义类类型，有typename是自定义类类型
//typename DualLinkList<T>::Node过长，为了方便进行重命名

//template<typename T> typedef typename DualLinkList<T>::Node Node;

//在链表中大量使用循环查询的方式，因此将循环查询功能封装成函数。
//因为position标识的是内部类型Node，所以应该是protected的，不应该被外部使用。
//返回值为算表头的第i个node,不算表头的第i-1个节点
//对于i的异常检测会在外部完成
template<typename T>
typename DualLinkList<T>::Node* DualLinkList<T>:: _position(int i) const{
	//因为head是mutable的所以这里head的地址可以在const修饰的函数中作为左值。
	Node* current = reinterpret_cast<Node*>(&head);
		for(int k = 0; k < i; k++){
			current = current->next;
		}		
	return current;
}

template<typename T>
typename DualLinkList<T>::Node* DualLinkList<T>:: _create(){
	return new Node;
}

template<typename T>
void DualLinkList<T>:: _destroy(Node* n){
	delete n;
	cout<<"~LinkList()"<<endl;
}


template<typename T>
DualLinkList<T>:: DualLinkList(){
	head.next = NULL;
	head.pre = NULL;
	length = 0;
	step = 1;
	cursor = NULL;
}


// __1__2__3__ 
//3个元素四个空，n个元素n+1个空，i标识第i个空
template<typename T>
bool DualLinkList<T>:: Insert(int i, const T& e){
	bool rst = (i >= 0)&&(i <= length);
	
	if (rst){
		Node* node = _create();
		//便利到要插入位置前一个节点。
		if(node){
			Node* current = _position(i);
			Node* next = current->next;
			//执行插入操作
			node->value = e;

			node->next = next;
			current->next = node;

			if(current != reinterpret_cast<Node*>(&head)){
				node->pre = current;
			}
			else{
				node->pre = NULL;
			}

			if(next != NULL){
				next->pre = node;
			}

			length++;
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException,"Invalid parameter i to get element ...");
		}

		
	}
	return rst;
}

template<typename T>
bool DualLinkList<T>:: Insert(const T& e){
	return Insert(length,e);
}

template<typename T>
bool DualLinkList<T>:: Remove(int i){
	bool rst = (i >= 0)&&(i < length);
	if(rst){
		Node* current = _position(i);
		Node* toDel = current->next;
		Node* next = toDel->next;
		//在删除元素之前，要保障游标不能指向删除的元素
		//如果删除的是游标所指向的元素，那么游标就指向后边的一个元素
		//若游标所指元素是最后一个元素，游标指向链表尾部的null
		//若游标已经为null，继续删除元素，游标随着链表尾部的null前移。
		if( cursor == toDel){
			cursor = toDel->next;
		}

		current->next = next;
		if(next != NULL){
			next->pre = current;
		}
		//length-- 一定要在delete前边，如果自定义类会在析构的时候抛出异常，delete后边的代码就无法执行，所以为了确保链表的异常安全性，链表的任何状态更新要在delete之前完成。
		//如果length--放在delete后边，就会造成当delete抛异常的时候，链表中已经删除了node，但是长度没有改变。链表的状态不配套。
		length--;
		_destroy(toDel);
	}
	return rst;
}

template<typename T>
bool DualLinkList<T>:: Set(int i, const T& e){
	bool rst = (i >= 0)&&(i < length);
	if(rst){
		_position(i)->next->value = e;
	}
	return rst;
}

template<typename T>
bool DualLinkList<T>:: Get(int i, T& e)const{
	bool rst = (i >= 0)&&(i < length);
	if(rst){
		e = _position(i)->next->value;
	}
	return rst;
}

template<typename T>
T DualLinkList<T>:: Get(int i)const{
	T e;
	if(Get(i,e)){
		return e;
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException,"Invalid parameter i to get element...");
	}

}

/*
寻找第一个值相等的元素
return -1： 若list中没有该元素
return >= 0: 数据元素在线性表中第一次出现的位置
*/
template<typename T>
int DualLinkList<T>:: Find(const T& e) const{
	int i = 0;
	int location = -1;
	Node* current = _position(0)->next;
	while(current){
		if(current->value == e){
			location = i;
			break;
		}
		else{
			current = current->next;
			i++;
		}
	}
	return location;
}

template<typename T>
unsigned int DualLinkList<T>:: Length() const{
	return length;
}

template<typename T>
void DualLinkList<T>:: Clear(){
	while(length > 0){
		Remove(0);
	}
}

template<typename T>
bool DualLinkList<T>::Move(int i, unsigned int step){
	bool rst = (i >= 0)&&( i <= length)&&(step > 0);
	if(rst){
		cursor = _position(i)->next;
		this->step = step;
	}
	return rst;
}


//对于双向链表，两头都有NULL，作为终点
template<typename T>
bool DualLinkList<T>:: End(){
	return (cursor == NULL);
}

template<typename T>
bool DualLinkList<T>::Next(){
	int i = 0;
	while((i<step)&&(!End())){
		cursor = cursor->next;
		i++;
	}
	//移动是否成功（是否移动了一个步长）
	//如果不到一个步长，就遇到了End()，那么移动只能被迫中止，此时返回异常（false）
	return (i == step);
}

template<typename T>
bool DualLinkList<T>:: Pre(){
	unsigned int i = 0;
	while((i < step) &&!End()){
		cursor = cursor->pre;
		i++;
	}
	//检测移动是否成功，还是被迫终止
	return (i == step);
}

template<typename T>
T DualLinkList<T>:: Current(){
	if(!End()){
		return cursor->value;
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
	}
}

template<typename T>
DualLinkList<T>::~DualLinkList(){
	Clear();
}


/*
Test code:
DualLinkList<int> list;
	
	for(int i = 0; i < 5; i++){
		list.Insert(i);
		list.Insert(5);
	}
	cout<<endl;
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	cout<<endl;
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
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

Result:
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
delete:pointor = 0x55f5dffb12b0
~DualLinkList()
5
~Object()
delete:pointor = 0x55f5dffb1310
~DualLinkList()
5
~Object()
delete:pointor = 0x55f5dffb1370
~DualLinkList()
5
~Object()
delete:pointor = 0x55f5dffb13d0
~DualLinkList()
5
~Object()
delete:pointor = 0x55f5dffb1430
~DualLinkList()
end
0
1
2
3
4
~Object()
~Object()
~Object()
delete:pointor = 0x55f5dffb1280
~DualLinkList()
~Object()
delete:pointor = 0x55f5dffb12e0
~DualLinkList()
~Object()
delete:pointor = 0x55f5dffb1340
~DualLinkList()
~Object()
delete:pointor = 0x55f5dffb13a0
~DualLinkList()
~Object()
delete:pointor = 0x55f5dffb1400
~DualLinkList()
~Object()
~Object()
*/

}
#endif