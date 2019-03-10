#ifndef __DYNAMICARRAY_H__
#define __DYNAMICARRAY_H__

#include "Array.h"
#define MAX_SIZE 0xFFFFFFFF
/*
DynamicArray设计要点
	动态确定内部数组空间大小
	实现函数返回数组长度
	拷贝构造和赋值操作
在DynamicArray的实现过程中，出现大量的重复性工作
考虑封装函数，代码复用
	init
		对象构造时的初始化操作
	copy
		在堆空间中申请新的内存，并执行拷贝操作
	update
		将指定的堆空间作为内部存储数组使用，更新各个成员


*/
namespace YzcLib{

template <typename T>
class DynamicArray: public Array<T>{
protected:
	unsigned int length;
	//第一个参数T* a可以在_init内部申请空间，但是对于函数而言，将T* a放在参数的位置更好，增强了函数的独立性。这样就避免了在函数内部申请内存，在外部释放。避免忘记释放内存
	void _init(T* a, unsigned int len);
	T* _copy(T* array, unsigned int len, unsigned int newlen = MAX_SIZE);
	void _update(T* a, unsigned int len);



public:

	DynamicArray(unsigned int len = 0);

	//拷贝构造和赋值
	DynamicArray(const DynamicArray<T>& obj);
	DynamicArray<T>& operator = (const DynamicArray<T>& obj);

	//重置数组大小
	void resize(unsigned int len);
	//长度获取
	unsigned int Length() const;

	//动态申请内存，需要释放，所以需要析构函数

	~DynamicArray();


};


template <typename T>
void DynamicArray<T>::_init(T* a, unsigned int len){
	if(a != NULL){
		this->m_array = a;
		this->length = len;
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create DynamicArray object...");
	}
}

/*
为了兼容resize，有两个长度参数
len	原数组长度
newlen 更新后的数组长度
将newlen的默认值设置为最大值，这样方便拷贝函数和赋值函数
如果是赋值操作或拷贝的话，有两种选择
1.len和newlen是一样的值
2.newlen的默认值设置为了最大值，只要不设置newlen，就会得到len的值。
*/
template <typename T>
T* DynamicArray<T>::_copy(T* array, unsigned int len, unsigned int newlen){
	T* a = new T[newlen];
	if (array != NULL){
		unsigned int l = (len < newlen)?len:newlen;
		for(int i = 0; i < l; i++){
			a[i] = array[i];
		}
	}
	return a;
}


template <typename T>
void DynamicArray<T>::_update(T* a, unsigned int len){
	if (a != NULL){
		T* temp = this->m_array;

		this->m_array = a;
		this->length = len;

		delete[] temp;
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create DynamicArray object...");
	}

}

template <typename T>
DynamicArray<T>::DynamicArray(unsigned int len){
	_init(new T[len], len);
	// T* array = new T[len];
	// if(array != NULL){
	// 	this->m_array = array;
	// 	this->length = len;
	// }
	// else{
	// 	THROW_EXCEPTION(NotEnoughMemoryException,"No memory to create DynamicArray object...");
	// }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& obj){
	_init(_copy(obj.m_array, obj.length),obj.Length());
	// T* array = new T[obj.Length()];
	// if(array != NULL){
	// 	for(int i = 0; i < obj.Length(); i++){
	// 		array[i] = obj[i];
	// 	}
	// 	this->m_array = array;
	// 	this->length = obj.Length();
	// }
	// else{
	// 	THROW_EXCEPTION(NotEnoughMemoryException,"No memory to create DynamicArray object...");
	// }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator = (const DynamicArray<T>& obj){
	if(this != &obj){
		_update(_copy(obj.m_array, obj.length),obj.Length());
		// T* array = new T[obj.Length()];
		// if(array != NULL){
		// 	for(int i = 0; i < obj.Length(); i++){
		// 		array[i] = obj[i];
		// 	}
		// 	T* temp = this->m_array;

		// 	this->m_array = array;
		// 	this->length = obj.Length();

		// 	delete[] temp;
		// }
		// else{
		// 	THROW_EXCEPTION(NotEnoughMemoryException,"No memory to create DynamicArray object...");
		// }
	}
	return *this;

}

template <typename T>
void DynamicArray<T>::resize(unsigned int len){
	if(len != this->length){
		_update(_copy(this->m_array, this->length, len), len);
		// T* array = new T[len];
		// if(array != NULL){
		// 	//选长度小的
		// 	unsigned int l = (len < this->length)?len:this->length;
		// 	for(int i = 0; i < l; i++){
		// 		array[i] = this->m_array[i];
		// 	}
		// 	T* temp = this->m_array;

		// 	this->m_array = array;
		// 	this->length = len;

		// 	delete[] temp;
		// }
		// else{
		// 	THROW_EXCEPTION(NotEnoughMemoryException,"No memory to create DynamicArray object...");
		// }
	}
}

template <typename T>
unsigned int DynamicArray<T>::Length()const{
	return length;
}

template <typename T>
DynamicArray<T>::~DynamicArray(){
	delete[] this->m_array;
}

//Test code
/*
	DynamicArray<int> da(5);

	for (int i = 0; i < da.Length(); i++){
		da[i] = i;
	}
	for (int i = 0; i < da.Length(); i++){
		cout<<da[i]<<endl;
	}

	cout<<endl;

	DynamicArray<int> da1 = da;

	for (int i = 0; i < da1.Length(); i++){
		cout<<da1[i]<<endl;
	}

	cout<<endl;
	da1.resize(3);

	for (int i = 0; i < da1.Length(); i++){
		cout<<da1[i]<<endl;
	}

	cout<<endl;
	DynamicArray<int> da2 = da1;

	for (int i = 0; i < da2.Length(); i++){
		cout<<da2[i]<<endl;
	}

	cout<<endl;
	DynamicArray<int> da3(8);
	da3 = da;
	cout<<"da3.Length = "<<da3.Length()<<endl;
	for (int i = 0; i < da3.Length(); i++){
		cout<<da3[i]<<endl;
	}
*/

//result
/*
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

0
1
2

0
1
2

da3.Length = 5
0
1
2
3
4
*/
}

#endif