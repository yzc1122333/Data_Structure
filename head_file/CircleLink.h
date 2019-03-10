#ifndef __CIRCLELINK_H__
#define __CIRCLELINK_H__

#include "LinkList.h"

/*
循环链表
--概念上
	任意数据元素都有一个前驱和一个后继
	所有的数据元素的关系构成了一个逻辑上的环
--实现上
	循环链表是一种特殊的单链表
	尾节点的指针域保存了首节点的地址

实现思路
	通过模板定义CircleList类，继承自LinkList类
	定义内部函数last_to_first(),用于将单链表首位相连
	特殊处理：首元素的插入和删除操作
		--插入位置为0时：
			头节点和尾节点均指向新节点
			新节点成为首节点插入链表
		--删除位置为0时：
			头节点和尾节点指向位置为1的节点
			安全销毁首节点
	重新实现：清空操作和遍历操作

*/

namespace YzcLib{
template<typename T>
class CircleList : public LinkList<T>
{
protected:
typedef typename LinkList<T>::Node Node;
/*
根据链表长度取模，长度为0时，返回0
*/
unsigned int _mod(unsigned int i)const;
//寻找链表最后一个元素
Node* _last()const;

void _last2first()const;

Node* last;

public:
	CircleList();
	//元素插入
	bool Insert(int i, const T& e); 
	//尾部插入元素
	//因为上边的Insert函数重写了，所以子类有Insert函数，会引发同名覆盖，即将父类的所有函数名为Insert的函数（包括重载的Insert函数）覆盖掉，所以需要在子类全部重写。
	virtual bool Insert(const T& e); 
	//删除元素
	bool Remove(int i);				
	//获取目标位置元素的值
	bool Set(int i, const T& e);	
	//设置目标位置元素的值
	bool Get(int i, T& e)const;		
	//上一个Get使用起来不方便，对Get进行重载，遇到错误直接抛异常
	//Get函数被同名覆盖了，因此需要重写
	T Get(int i)const;	
	//获取目标元素的位置
	int Find(const T& e) const;	
	//清空线性表
	void Clear();

	//游标的函数重载
	bool Move(int i, unsigned int step = 1);//将游标定位到目标位置（初始化游标【起始位置，步长】）
	bool End();//游标是否到达尾部（是否为空）


	~CircleList();
};

template<typename T>
unsigned int CircleList<T>::_mod(unsigned int i)const{
	return (this->length? i%(this->length):0);
}

template<typename T>
typename CircleList<T>::Node* CircleList<T>::_last()const{
	return this->_position(this->length);
}

template<typename T>
void CircleList<T>::_last2first()const{
	if(last){
		last->next = this->head.next;
	}
	
}

template<typename T>
CircleList<T>::CircleList():LinkList<T>(){
	last = NULL;
}

template<typename T>
bool CircleList<T>::Insert(int i, const T& e){
	//因为想调用父类单链表的Insert函数，所以进行归一化
	i = i%(this->length + 1);
	bool rst = LinkList<T>::Insert(i,e);

	if(rst && i == (this->length) - 1){
		last  = last?last->next: this->head.next;
	}

	if(rst && i == 0){
		_last2first();
	}
	

	
	return rst;
}

template<typename T>
bool CircleList<T>::Insert(const T& e){
	return Insert(this->length, e);
}


template<typename T>
bool CircleList<T>::Remove(int i){
	bool rst = false;
	i = _mod(i);
	//首节点单独讨论
	if( i == 0){
		if(this->length > 0){
			Node* toDel = this->head.next;
			this->head.next = toDel->next;
			//通过length来获得last,所以要先改变length。
			(this->length)--;
			_last2first();

			if(this->cursor == toDel){
				this->cursor = toDel->next;
			}
			this->_destroy(toDel);

			if(this->length == 0){
				this->head.next = NULL;
				this->cursor = NULL;
				last = NULL;
			}
			rst = true;
		}
		
	}
	else{
		rst = LinkList<T>::Remove(i);
		if(rst && (i >= this->length)){
			last = _last();
		}
	}

	return rst;
}

template<typename T>
bool CircleList<T>::Set(int i, const T& e){
	return LinkList<T>::Set(_mod(i),e);
}

template<typename T>
bool CircleList<T>::Get(int i, T& e)const{
	return LinkList<T>::Get(_mod(i),e);
}

template<typename T>
T CircleList<T>::Get(int i)const{
	return LinkList<T>::Get(_mod(i));
}

template<typename T>
int CircleList<T>::Find(const T& e) const{
/*错误案例:
	last()->next = NULL;
	//如果Find函数抛出异常，那么循环链表结构被破坏，因此不能这样写。
	rst = LinkList<T>::Find(e);
	_last2first();
*/
	Node* current = reinterpret_cast<Node*>(&(this->head));
	int rst = -1;
	for(int i = 0; i < this->length; i++){
		current = current->next;
		if(current->value == e){
			rst = i;
			break;
		}
	}
	return rst;
}

template<typename T>
void CircleList<T>::Clear(){
	/*错误案例:
	Clear如果抛异常，那么破坏了循环链表的性质，所以禁止如此操作
	if(this->m_length > 0){
		_last()->next = NULL;
		LinkList<T>::Clear();
	}
	*/
	//因为循环链表首元素删除和插入需要特殊处理，复杂度较大，因此一直Remove(1),最后首元素单独处理
	while(this->length > 1){
		Remove(1);
	}
	if(this->length == 1){
		Node* toDel = this->head.next;
		this->head.next = NULL;
		this->cursor = NULL;
		this->length = 0;
		this->_destroy(toDel);
	}
}


template<typename T>
CircleList<T>::~CircleList(){
	Clear();
}

template<typename T>
bool CircleList<T>:: Move(int i, unsigned int step){
	return LinkList<T>::Move(_mod(i), step);
}

//有问题
//只能够判断是否链表为空，为空时停止
//不推荐使用
template<typename T>
bool CircleList<T>:: End(){
	//对应两种情况，一种是游标没有初始化，另一种是链表为空
	return (this->length == 0) || (this->cursor == NULL);//(this->cursor == last);
}

/*
约瑟夫环问题
按顺序输出
n 总数
s 起始位置
m 步长
*/
void Josephus(unsigned int n, unsigned int start, unsigned int step){
	CircleList<int> cl;
	for(int i = 1; i <= n; i++){
		cl.Insert(i);
	}
	cl.Move(start-1, step - 1);

	while(cl.Length() > 0){
		cl.Next();
		cout<<cl.Current()<<endl;
		cl.Remove(cl.Find(cl.Current()));
	} 

}


}


#endif