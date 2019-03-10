#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "Object.h"
#include "Exception.h"

/*
数组类
创建数组类代替原生数组
	包含长度信息
	能够主动发现越界访问
*/
/*
Array抽象类设计要点：
	抽象类模板，存储空间的位置和大小都由子类来完成
	重载数组操作符，加入索引合法性检测
	提供数组长度的抽象访问函数
	提供数组对象间的复制操作
*/

namespace YzcLib{
template<typename T>
class Array: public Object{
protected:
	T* m_array;
public:
	//get和set设置成虚函数，方便子类进行多态操作
	//对于位置索引采用的unsigned int类型，比int类型要好，如果输入一个负数，那么隐式类型转换为unsigned int后会是一个非常大的正数。
	virtual bool Set(unsigned int i, const T& e);
	virtual bool Get(unsigned int i, T& e)const;
	//Length相关信息都是在子类给出，但是Length是数组类必备的特征，所以这里只定义纯虚函数，不实现。
	virtual unsigned int Length() const = 0;
	//获取数组类首元素地址
	virtual T* GetArray() const;

	//数组操作符
	T& operator [] (const unsigned int i);
	T operator [] (const unsigned int i)const;
};

template<typename T>
bool Array<T>::Set(unsigned int i, const T& e){
	bool rst = i < Length();
	if (rst){
		m_array[i] = e;
	}
	return rst;
}

template<typename T>
bool Array<T>::Get(unsigned int i, T& e)const{
	bool rst = i < Length();
	if (rst){
		e = m_array[i];
	}
	return rst;
}


template<typename T>
T* Array<T>::GetArray() const{
	return m_array;
}

 


template<typename T>
T& Array<T>::operator [] (const unsigned int i){
	if(i < Length()){
		return m_array[i];
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException,"Parameter i is invalid...");
	}
}

template<typename T>
T Array<T>::operator [] (const unsigned int i) const{
	//借助上方的operator[],达到代码的复用
	return const_cast<Array<T>&>(*this)[i];
}

}



#endif