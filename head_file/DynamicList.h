#ifndef __DYNAMICLIST_H__
#define __DYNAMICLIST_H__

#include "Exception.h"
#include "SeqList.h"
/*
动态链表（容量可变的链表）：
	申请连续空间作为顺序存储空间
	动态设置顺序存储空间大小
注意：需要保证重置顺序存储空间时的异常安全性
设计要点：
1.函数异常安全
	不泄露任何资源
	不允许破坏数据
2.函数异常的基本保证
	如果抛出异常
		对象的任何成员仍能保持有效状态（对象的状态要配套，避免对象的状态只有部分更新了）
		没有数据的破坏及资源的泄露
*/

namespace YzcLib{
template<typename T>
class DynamicList: public SeqList<T>{
protected:
	unsigned int capacity;
public:
	DynamicList(const unsigned int N);
	unsigned int Capacity() const;
	//重新设置顺序存储空间大小
	void resize(const unsigned int N);
	//动态申请，需要释放内存
	~DynamicList();
};
/*
动态申请内存
成功-赋初值
不成功-抛异常
*/
template<typename T>
DynamicList<T>::DynamicList(const unsigned int N){
	T* array = new T[N];//确保只有申请成功才会改变this->m_array的值，所以先申请一个临时变量array。
	if(array != NULL){//内存申请成功，初始化
		this->m_array = array;
		capacity = N;
		this->length = 0;
	}
	else{//内存申请失败
		THROW_EXCEPTION(NotEnoughMemoryException,"There isn't enough memory to initial DynamicList");
	}
}
template<typename T>
unsigned int DynamicList<T>::Capacity() const{
	return capacity;
}




template<typename T>
void DynamicList<T>::resize(const unsigned int N){
	//容量一样，直接返回,容量不同进入if语句，重新分配内存
	if (N != capacity){

		T* array = new T[N];
		if(array != NULL){
			unsigned int l = (N > this->length)?this->length:N;
			for (int i = 0; i < l; i++){
				//可能在赋值的时候抛出异常,直接返回。对象中的内容不更新。
				//可能会发生array的内存泄漏，但是无法避免，泛指类型T的赋值失败是用库者的管辖范畴。
				array[i] = this->m_array[i];
			}
			T* temp = this->m_array;//delete[]可能抛异常中断返回，因此先用temp保存this->m_array,等对象的成员都赋值完毕再delete[] temp

			//成员赋值不会抛出异常
			this->m_array = array;
			capacity = N;
			this->length = l;

			//最后delete，即使抛出异常，成员的值已经全部更新，异常安全得到保障
			delete[] temp;
		}
		else{//内存申请失败，抛出异常
					THROW_EXCEPTION(NotEnoughMemoryException,"There isn't enough memory to resize DynamicList");
		}

	}
	
	
}

template<typename T>
DynamicList<T>::~DynamicList(){
	delete this->m_array;
}




//Test code
/*
	DynamicList<int> dl = 5;
	
	for (int i = 0; i < dl.Capacity(); i++){
		dl.Insert(i,i);
	}
	for (int i = 0; i < dl.Capacity(); i++){
		cout<<dl[i]<<endl;
	}
	cout<<"#############"<<endl;
	for (int i = 0; i < dl.Length(); i++){
		dl[i] *= dl[i];
	}

	for (int i = 0; i < dl.Length(); i++){
		cout<<dl[i]<<endl;
	}
	cout<<"#############"<<endl;
	try{
		dl[5] = 5;
	}
	//const主要保证const和非const对象都可以传参，若没有const对象，则只能传参非const对象。
	catch(const Exception& e){
		cout<< e.GetMessage()<<endl;
		cout<< e.GetLocation()<<endl;
	}
	dl.resize(6);
	cout<<dl.Capacity()<<endl;
	try{
		dl.Insert(5,5);
		cout<<dl[5]<<endl;
	}
	//const主要保证const和非const对象都可以传参，若没有const对象，则只能传参非const对象。
	catch(const Exception& e){
		cout<< e.GetMessage()<<endl;
		cout<< e.GetLocation()<<endl;
	}
	dl.resize(3);
	for (int i = 0; i < dl.Capacity(); i++){
		dl.Insert(i,i);
	}
	for (int i = 0; i < dl.Length(); i++){
		cout<<dl[i]<<endl;
	}
	return 0;
*/
//result
/*
0
1
2
3
4
#############
0
1
4
9
16
#############
Parameter i is invalid ...
./head_file/SeqList.h:110
6
5
0
1
4
*/
}

#endif