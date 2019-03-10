#include<cstdlib>
#include <iostream>
using namespace std;
#include "./../head_file/Object.h"

namespace YzcLib{
void* Object::operator new(size_t size)throw(){
	// cout<<"new():size = "<<size<<endl;
	return malloc(size);
}

//new[]会比new多一些参数，所以要分开定义
void* Object::operator new[](size_t size)throw(){
	// cout<<"new[]():size = "<<size<<endl;
	return malloc(size);
}


/*
根据C和C++文档，delete NULL，free(NULL)都不会抛异常，什么事情都不做
*/
void Object::operator delete(void* p){
	// cout<<"delete:pointor = "<<p<<endl;
	free(p);
}

void Object::operator delete[](void* p){
	// cout<<"delete[]:pointor = "<<p<<endl;
	free(p);
}

bool Object::operator == (const Object& obj){
	return (this == &obj);
}
bool Object::operator != (const Object& obj){
	return (this != &obj);
}

//c++要求析构函数的纯虚函数也要给出实现。因为析构函数只要是虚的，子类就会一直相上调用，如果析构函数没有实现，就会出现问题————找不到父类析构函数。这个问题只有在定义对象的时候才会出现，定义对象指针本质是个指针，并不会出现构造析构的问题
//并不是只有纯虚构函数可以定义函数体，所有纯虚构函数都可以定义函数体。纯析构函数只限制对象的定义
//另外，纯虚函数必须在类声明外部实现，不能写入类声明中

Object::~Object(){
	// cout<<"~Object()"<<endl;
}

}