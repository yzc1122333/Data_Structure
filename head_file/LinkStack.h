#ifndef __LINKSTACK_H__
#define __LINKSTACK_H__

#include "Stack.h"
// #include "LinkList.h"
#include "DualCircleList.h"
#include"Exception.h"

/*
StaticStack 与静态数组一样，实现是有缺陷的，当泛指类型为类类型是，需要大量的进行拷贝构造和析构工作，对于复杂的类对象是非常耗时的，因此考虑采用链式结构进行优化
链式栈的设计要点：
	--类模板， 抽象父类Stack的直接子类
	--在内部组合使用LinkList类， 实现栈的链式存储
	--只在单链表对象成员的头部操作

StaticStack在Clear是O(1),而LinkStack是O(N)
LinkStack对于大型类而言，不需要拷贝

**在单链表的头部进行操作能够实现非常高效的入栈和出栈操作,O(1)
	如果在尾部，每次都要循环，是O(N)
*/

namespace YzcLib{

template<typename T>
class LinkStack: public Stack<T>{
protected:
	DualCircleList<T> list;
public:

	void Push(const T& e);
	void Pop();
	T Top() const;
	unsigned int Size() const;
	void Clear();

};

template<typename T>
void LinkStack<T>::Push(const T& e){
	list.Insert(0, e);
}

template<typename T>
void LinkStack<T>::Pop(){
	if(list.Length() > 0){
		list.Remove(0);
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
	}
}

template<typename T>
T LinkStack<T>::Top() const{
	if(list.Length() > 0){
		return list.Get(0);
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
	}
}

template<typename T>
unsigned int LinkStack<T>::Size() const{
	return list.Length();
}

template<typename T>
void LinkStack<T>::Clear(){
	list.Clear();
}

/*
Test code:
	LinkStack<int> sstack;

	try{
		sstack.Top();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	try{
		sstack.Pop();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}
	
	for(int i = 0; i < 5; i++){
		sstack.Push(i);
	}

	try{
		sstack.Push(66);
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	while(sstack.Size() > 0){
		cout<<sstack.Top()<<endl;
		sstack.Pop();
	}
*/

/*
栈的应用：
符号匹配问题
	在C语言中有一些成对匹配出现的符号
	括号：(), [], {}, <>
	引号：‘’， “”
实现其成对匹配检测
算法思路
	--从第一个字符开始扫描
		当遇见普通字符时忽略
		当遇见左符号时压入栈
		当遇见右符号时弹出栈顶进行匹配
	--结束
		成功：所有字符扫描完毕，且栈为空
		失败：1.匹配实拍
			 2.所有字符扫描完毕但站非空
*/
bool Is_left(char c){
	return (c == '(')|| (c == '[')|| (c == '{')|| (c == '<'); 
 }

bool Is_right(char c){
	return (c == ')')|| (c == ']')|| (c == '}')|| (c == '>'); 
 }
//在英语中引号部分左右，但是成对，因此需要单独列出来处理
bool Is_quote(char c){
	 return (c == '\'')||(c == '\"');
 }

bool Is_match(char l, char r){
	return ((l == '(')&&(r == ')'))||\
			((l == '[')&&(r == ']'))||\
			((l == '{')&&(r == '}'))||\
			((l == '<')&&(r == '>'))||\
			((l == '\'')&&(r == '\''))||\
			((l == '\"')&&(r == '\"'));
}

//必须要有\0结束符，不然停不下来。。。，
bool Check(const char* s){
	int i = 0;
	const char* code = s? s : "";
	bool rst = true;

	LinkStack<char> stack;
	while(rst && code[i] != '\0'){
		if(Is_left(code[i])){
			stack.Push(code[i]);
		}
		else if(Is_right(code[i])){
			if(stack.Size()>0 && Is_match(stack.Top(), code[i])){
				stack.Pop();
			}
			else{
				rst = false;
			}
		}
		else if(Is_quote(code[i])){
			if(stack.Size() == 0 || !Is_match(stack.Top(), code[i])){
				stack.Push(code[i]);
			}
			else if(Is_match(stack.Top(), code[i])){
				stack.Pop();
			}
		}
		i++;
	}
	return rst && (stack.Size() == 0);
}

/*
Test code:

	cout<<(Check("ab\"\'{[(<c>)]}\n\'d\"")?"success":"false")<<endl;
	// Array.h中的一段程序
	cout<<(Check("namespace YzcLib{template<typename T>class Array: public Object{protected:T* m_array;public:virtual bool Set(unsigned int i, const T& e);virtual bool Get(unsigned int i, T& e)const;virtual unsigned int Length() const = 0;T& operator [] (const unsigned int i);T operator [] (const unsigned int i)const;};"))<<endl;
	return 0;
*/

}

#endif