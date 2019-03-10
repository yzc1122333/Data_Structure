#ifndef __STATICLIST_H__
#define __STATICLIST_H__
#include "SeqList.h"
/*
静态链表
	所谓静态即链表长度不再发生变化，链表长度是静止的
	使用原生数组作为顺序存储空间
	使用模板参数决定数组大小
*/

namespace YzcLib{
template<typename T, int N> //N为数组大小
class StaticList: public SeqList<T>{
protected:
	T space[N];
public:
	//主要对父类成员进行初始化
	StaticList(){
		//父类是模板可以用Base::来调用父类成员
		SeqList<T>::m_array = space;
		//父类是模板可以用this来调用父类成员
		this->length = 0;
	}
	//链表一旦创建，长度不可变，因此直接返回N的值即可
	unsigned int Capacity() const{
		return N;
	}
};

}

//test code
/*
	StaticList<int, 5> sl;
	
	for (int i = 0; i < sl.Capacity(); i++){
		sl.Insert(i,i);
	}
	for (int i = 0; i < sl.Capacity(); i++){
		cout<<sl[i]<<endl;
	}
	cout<<"#############"<<endl;
	for (int i = 0; i < sl.Length(); i++){
		sl[i] *= sl[i];
	}

	for (int i = 0; i < sl.Length(); i++){
		cout<<sl[i]<<endl;
	}
	cout<<"#############"<<endl;
	try{
		sl[5] = 5;
	}
	//const主要保证const和非const对象都可以传参，若没有const对象，则只能传参非const对象。
	catch(const Exception& e){
		cout<< e.GetMessage()<<endl;
		cout<< e.GetLocation()<<endl;
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
*/

#endif