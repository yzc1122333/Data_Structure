#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include "List.h"
#include <iostream>

/*
链表:
基于链式存储机构的线性表（在物理内存中无相邻关系）
	分类:
	单链表:每个节点只包含后继信息
	循环链表:单链表的最后一个节点的直接后继为第一个节点
	双向链表:单链表中的节点包含直接前驱和后继的地址信息
链表中的基本元素：
	头节点（链表中的辅助节点，包含指向第一个数据元素的指针）
	数据节点（链表中代表数据元素的节点，表现形式为：[数据元素，地址]）
	尾节点（链表中的最后一个数据节点，包含的地址信息为空）
头节点在单链表中的意义：辅助数据元素的定位，方便插入和删除操作，因此，头节点不存储实际的数据元素


顺序表和单链表的比较：
对于内置类类型，顺序表和单链表的效率不相上下，顺序表效率略高
对于自定义类类型，顺序表在效率上低于单链表
效率的深度分析
--插入和删除
	顺序表：涉及大量数据对对象的复制操作
	单链表：只涉及指针操作，效率与数据对象无关
--数据访问
	顺序表：随机访问，可以直接定位数据对象
	单链表：顺序访问，必须从头访问，无法直接定位
工程开发中的选择
--顺序表
	数据类型相对简单，不涉及深拷贝
	数据元素相对稳定，访问操作远多于插入和删除操作
--单链表
	数据元素的类类型相对复杂，复制相对耗时
	数据元素不稳定，需要经常插入和删除，访问操作较少
*/

/*
LinkList设计要点：
	类模板，通过头节点访问后继节点
	定义内部节点类型Node， 用于描述数据域和指针域
	实现线性表的关键操作（增，删，改，查等）
	插入和删除要保证链表的完整性
*/
namespace YzcLib{

template<typename T>
class LinkList: public List<T>{
protected:
//定义内部类型
	//Node类型,使用自定义的new和delete，继承Object
	struct Node: public Object{
		T value;
		Node* next;
	};

	//头节点要单独定义类型，原因是，弱遇到用户自己构造的类有问题的情况，在insert环节报错，不要在构造list时报错，方便甩锅。。。
	//头节点要保证和Node有相同的内存分布，这样可以将Head直接转换成Node节点进行遍历
	//Head中的value是无用的，只是为了和Node保持一致类型
	//Head要加mutable，因为对于Get函数是有const属性的，在const函数中禁止一切对内部成员的修改。而将Head的地址赋值给临时变量的操作有可能会改变Head的值，所以编译器会报错。
	mutable struct: public Object {
		char reserved[sizeof(T)];
		Node* next;
	} head;
	//链表许多操作都需要循环遍历，所以设置游标，提高效率
	Node* cursor;
	//cursor的步长
	unsigned int step;

	unsigned int length;
	Node* _position(int i) const;
	//封装create和destroy为静态链表的实现做准备。StaticLinkList与LinkList的不同仅在于链表节点内存分配不同，因此将仅有的不同封装于父类和子类的虚函数中
	//创建一个node，方便使用多态，在静态链表中重写create()
	virtual Node* _create();
	//销毁一个node，方便使用多态，在静态链表中重写destroy()
	virtual void _destroy(Node* n);

	virtual void _reverse(Node* head);

	//浅拷贝禁用，防止多次Clear
	LinkList(const LinkList<T>& );
public:
	LinkList();
	//链表转置
	virtual void Reverse();
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
	//返回值不能是T&，因为没有输入T的地方，局部变量传不出来，申请堆内存，无法释放，因此返回值只能是T
	virtual T Get(int i)const;	
	//获取目标元素的位置
	virtual int Find(const T& e) const;
	//获取线性表的长度
	virtual unsigned int Length() const;	
	//清空线性表
	virtual void Clear();
	//析构函数
	~LinkList();
	//游标的所有操作
	virtual bool Move(int i, unsigned int step = 1);//将游标定位到目标位置（初始化游标【起始位置，步长】）
	virtual bool Next();//移动游标
	virtual T Current();//获取游标所指向的数据元素
	virtual bool End();//游标是否到达尾部（是否为空）

	//输出函数
	//友元函数不加作用域限制，本身就是类外部的函数，不需要传递this指针
	//打印效果 Head -> 0 -> 1 -> 2 -> 3 -> 4 -> NULL
	friend ostream& operator << (ostream& out, const LinkList<T>& l){
		Node* temp_cursor = reinterpret_cast<Node*>(&(l.head));
		out<<"Head";
		while( temp_cursor->next != NULL){
			temp_cursor = temp_cursor->next;
			out<<" -> "<<temp_cursor->value;
		}

		out<<" -> "<<"NULL";
		return out;
	}
};



//LinkList<T>::Node*前边要加typename，用来区分是静态变量还是自定义类类型，有typename是自定义类类型
//typename LinkList<T>::Node过长，为了方便进行重命名

//template<typename T> typedef typename LinkList<T>::Node Node;

//在链表中大量使用循环查询的方式，因此将循环查询功能封装成函数。
//因为position标识的是内部类型Node，所以应该是protected的，不应该被外部使用。
//返回值为算表头的第i个node,不算表头的第i-1个节点
//对于i的异常检测会在外部完成
template<typename T>
typename LinkList<T>::Node* LinkList<T>:: _position(int i) const{
	//因为head是mutable的所以这里head的地址可以在const修饰的函数中作为左值。
	Node* current = reinterpret_cast<Node*>(&head);
		for(int k = 0; k < i; k++){
			current = current->next;
		}		
	return current;
}

template<typename T>
typename LinkList<T>::Node* LinkList<T>:: _create(){
	return new Node;
}

template<typename T>
void LinkList<T>:: _destroy(Node* n){
	delete n;
}

template<typename T>
LinkList<T>:: LinkList(){
	head.next = NULL;
	length = 0;
	step = 1;
	cursor = NULL;
}


// __1__2__3__ 
//3个元素四个空，n个元素n+1个空，i标识第i个空
template<typename T>
bool LinkList<T>:: Insert(int i, const T& e){
	bool rst = (i >= 0)&&(i <= length);
	
	if (rst){
		Node* node = _create();
		//便利到要插入位置前一个节点。
		if(node){
			Node* current = _position(i);
			//执行插入操作
			node->value = e;
			node->next = current->next;
			current->next = node;
			length++;
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException,"Invalid parameter i to get element ...");
		}

		
	}
	return rst;
}

template<typename T>
bool LinkList<T>:: Insert(const T& e){
	return Insert(length,e);
}

template<typename T>
bool LinkList<T>:: Remove(int i){
	bool rst = (i >= 0)&&(i < length);
	if(rst){
		Node* current = _position(i);
		Node* toDel = current->next;
		//在删除元素之前，要保障游标不能指向删除的元素
		//如果删除的是游标所指向的元素，那么游标就指向后边的一个元素
		//若游标所指元素是最后一个元素，游标指向链表尾部的null
		//若游标已经为null，继续删除元素，游标随着链表尾部的null前移。
		if( cursor == toDel){
			cursor = toDel->next;
		}
		current->next = toDel->next;
		//length-- 一定要在delete前边，如果自定义类会在析构的时候抛出异常，delete后边的代码就无法执行，所以为了确保链表的异常安全性，链表的任何状态更新要在delete之前完成。
		//如果length--放在delete后边，就会造成当delete抛异常的时候，链表中已经删除了node，但是长度没有改变。链表的状态不配套。
		length--;
		_destroy(toDel);
	}
	return rst;
}

template<typename T>
bool LinkList<T>:: Set(int i, const T& e){
	bool rst = (i >= 0)&&(i < length);
	if(rst){
		_position(i)->next->value = e;
	}
	return rst;
}

template<typename T>
bool LinkList<T>:: Get(int i, T& e)const{
	bool rst = (i >= 0)&&(i < length);
	if(rst){
		e = _position(i)->next->value;
	}
	return rst;
}

template<typename T>
T LinkList<T>:: Get(int i)const{
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
int LinkList<T>:: Find(const T& e) const{
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
unsigned int LinkList<T>:: Length() const{
	return length;
}

/*
单向链表转置(递归版)
									guard = list->next;
				len(list) > 2 		ret = reverse(list->next); 
									guard->next = list;
									list->next = NULL;

reserve(list)	len(list) == 1  	list;

				NULL				list = NULL
*/
template<typename T>
void LinkList<T>::_reverse(Node* head){
	
	if(head->next == NULL){
		this->head.next = head;
	}
	else{
		Node* guard = head->next;
		_reverse(guard);
		guard->next = head;
		head->next = NULL;
	}

}


template<typename T>
void LinkList<T>::Reverse(){
	_reverse(_position(0)->next);
}



template<typename T>
void LinkList<T>:: Clear(){
	while(head.next){
		Node* toDel = head.next;
		head.next = toDel->next;
		//length状态要和链表状态一致，保障delete抛异常时的异常安全性
		length--;
		_destroy(toDel);
	}
	//如果delete抛异常，在delete后边抛异常的行为，导致长度和实际链表中node数不匹配，产生异常安全问题
	//length = 0;
}

template<typename T>
bool LinkList<T>::Move(int i, unsigned int step){
	bool rst = (i >= 0)&&( i <= length)&&(step > 0);
	if(rst){
		cursor = _position(i)->next;
		this->step = step;
	}
	return rst;
}

template<typename T>
bool LinkList<T>:: End(){
	return (cursor == NULL);
}

template<typename T>
bool LinkList<T>::Next(){
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
T LinkList<T>:: Current(){
	if(!End()){
		return cursor->value;
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
	}
}





template<typename T>
LinkList<T>::~LinkList(){
	Clear();
}

//Test code
/*
	class Test{
		int t;
		public:
			Test(){
				t = 0;
				throw 0;
			}
	};


	LinkList<int> list;
	LinkList<Test> list1;
	
	try{
		Test t;
		list1.Insert(t);
	}
	catch(int){
		cout<<"Test class throw an exception."<<endl;
	}
	
	for(int i = 0; i < 5; i++){
		list.Insert(i, i);
	}
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	for(int i = 5; i < 10; i++){
		list.Insert(i);
	}
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}


	cout<<"I*I"<<endl;
	for(int i = 0; i < list.Length(); i++){
		list.Set(i,i*i);
	}
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	cout<<"length = "<<list.Length()<<endl;


	list.Remove(0);
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	cout<<"length = "<<list.Length()<<endl;


	list.Remove(2);
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	cout<<"length = "<<list.Length()<<endl;


	list.Remove(list.Length()-1);
	for(int i = 0; i < list.Length(); i++){
		cout<<list.Get(i)<<endl;
	}
	cout<<"length = "<<list.Length()<<endl;
	cout<<"end"<<endl;
*/
/*result
Test class throw an exception.
new():size = 24
new():size = 24
new():size = 24
new():size = 24
new():size = 24
0
1
2
3
4
new():size = 24
new():size = 24
new():size = 24
new():size = 24
new():size = 24
0
1
2
3
4
5
6
7
8
9
I*I
0
1
4
9
16
25
36
49
64
81
length = 10
delete:pointor = 0x152bc20
1
4
9
16
25
36
49
64
81
length = 9
delete:pointor = 0x152bc80
1
4
16
25
36
49
64
81
length = 8
delete:pointor = 0x152c160
1
4
16
25
36
49
64
length = 7
*/

//Find() Test code
/*
	class Test:public Object{
		public:
		int t;
		
			Test(int i = 0){
				t = i;
			}

			bool operator == (const Test& obj){
				return t == obj.t;
			}
	};


	Test t1(1);
	Test t2(2);
	Test t3(3);
	LinkList<Test> list;
	LinkList<int> list1;
	
	list.Insert(t1);
	list.Insert(t2);
	list.Insert(t3);
	cout<<list.Find(t1)<<endl;
	cout<<list.Find(t2)<<endl;
	cout<<list.Find(t3)<<endl;
	cout<<endl;
	for(int i = 0; i < 5; i++){
		list1.Insert(i, i);
	}
	for(int i = 0; i < list1.Length(); i++){
		cout<<list1.Get(i)<<endl;
	}

	for(int i = 0; i < list1.Length(); i++){
		cout<<list1.Find(i)<<endl;
	}
*/
//result
/*
new():size = 32
new():size = 32
new():size = 32
0
1
2

new():size = 24
new():size = 24
new():size = 24
new():size = 24
new():size = 24
0
1
2
3
4
0
1
2
3
4
*/

//cursor Test code
/*
	LinkList<int> list;
	
	for(int i = 0; i < 5; i++){
		list.Insert(0, i);
	}
	cout<<"step = 1"<<endl;
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}
	cout<<"step = 3"<<endl;
	for(list.Move(0,3); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}

	try{
		list.Current();
	}
	catch(InvalidOperationException){
		cout<<"current is NULL"<<endl;
	}
*/
//result
/*
new():size = 24
new():size = 24
new():size = 24
new():size = 24
new():size = 24
step = 1
4
3
2
1
0
step = 3
4
1
current is NULL
*/
//create(),destroy() Test code
/*

	LinkList<int> list;
	
	for(int i = 0; i < 5; i++){
		list.Insert(0, i);
	}
	cout<<"step = 1"<<endl;
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}
	cout<<"step = 3"<<endl;
	for(list.Move(0,3); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}

	try{
		list.Current();
	}
	catch(InvalidOperationException){
		cout<<"current is NULL"<<endl;
	}
	cout<<endl;

	list.Clear();
	cout<<list.Length()<<endl;
	
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}
*/
//result
/*
new():size = 24
new():size = 24
new():size = 24
new():size = 24
new():size = 24
step = 1
4
3
2
1
0
step = 3
4
1
current is NULL

delete:pointor = 0x15600b0
delete:pointor = 0x1560090
delete:pointor = 0x1560070
delete:pointor = 0x1560050
delete:pointor = 0x1560030
0
*/
//游标完备性检验
/*
	LinkList<int> list;
	for(int i = 0; i<5; i++){
		list.Insert(i);
	}
	for(list.Move(0); !list.End(); list.Next()){
		if(list.Current() == 3){
			list.Remove(list.Find(list.Current()));
		}
	}
	cout<<endl;
	for(list.Move(0); !list.End(); list.Next()){
		cout<<list.Current()<<endl;
	}

result：
new():size = 24
new():size = 24
new():size = 24
new():size = 24
new():size = 24
delete:pointor = 0xefb090

0
1
2
4
delete:pointor = 0xefb030
delete:pointor = 0xefb050
delete:pointor = 0xefb070
delete:pointor = 0xefb0b0
*/

}

#endif