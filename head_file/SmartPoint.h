#ifndef __SMARTPOINT_H__
#define __SMARTPOINT_H__
/*
1.指针生命周期结束时自动释放空间
2.一片空间最多只能由一个指针标识
3.杜绝指针运算和指针比较
4.只适用于保存在堆空间中的类
*/
/*
注：
1.动态申请失败，不会抛出异常，而是返回NULL，所以使用智能指针每次需要进行判断返回值是否合法。
*/
#include "Object.h"


namespace YzcLib{

template <typename T>
class SmartPoint : public Object{
private:
  T* point;

public:
  SmartPoint(T* p = NULL);
  SmartPoint(const SmartPoint<T>& sp);

  SmartPoint<T>& operator = (const SmartPoint<T>& sp);

  T* operator ->();
  T& operator *();

  bool IsNull();
  T* Get_Point();
  ~SmartPoint();
  
};



template <typename T>
SmartPoint<T>::SmartPoint(T* p ){
  point = p;
}

template <typename T>
SmartPoint<T>::SmartPoint(const SmartPoint<T>& sp){
  point = sp.point;
  const_cast<SmartPoint<T>&> (sp).point = NULL;
}


template <typename T>
SmartPoint<T>& SmartPoint<T>::operator = (const SmartPoint<T>& sp){
  if (this != &sp){
    delete point;
    point = sp.point;
    const_cast<SmartPoint<T>&> (sp).point = NULL;
  }
  return *this;
}

template <typename T>
T* SmartPoint<T>::operator ->(){
  return point;
}

template <typename T>
T& SmartPoint<T>::operator *(){
  return *point;
}

template <typename T>
T* SmartPoint<T>::Get_Point(){
  return point;
}

template <typename T>
bool SmartPoint<T>::IsNull(){
  return point == NULL;
}

template <typename T>
SmartPoint<T>::~SmartPoint(){
  delete point;
}
}
/*
class Test{
public:
  Test(){
    cout<<"Test()"<<endl;
  }
  ~Test(){
     cout<<"~Test()"<<endl;
  }
};

void SP_Test(){
  SmartPoint<Test> sp1 = new Test();
  SmartPoint<Test> sp2;

  cout<<"sp1 = "<<sp1.Get_Point()<<endl;
  cout<<"sp2 = "<<sp2.Get_Point()<<endl;

  sp2 = sp1;

  cout<<"*********************************"<<endl;  
  cout<<"sp1 = "<<sp1.Get_Point()<<endl;
  cout<<"sp2 = "<<sp2.Get_Point()<<endl;

}
*/

#endif