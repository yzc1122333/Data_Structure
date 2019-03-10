#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "./../head_file/Exception.h"
using namespace std;
namespace YzcLib{
//itoa是windows中的程序，使用sprintf代替itoa，使其能够复用
char* itoa(int value,char* const s, int base){
	sprintf(s, "%d", value);
	return const_cast<char*>(s);
}

void Exception::init(const char* mes, const char* file, unsigned int line){
	//指针内容存储位置位置，存在可靠性问题，所以将其复制\栈上在返回给message。strdup使用了malloc，需要配套free
	//对于malloc在64-bit的电脑中最少申请32-bit的栈空间。malloc(0),malloc(1)都是会给出至少32-bit的空间出来，但是申请方无法得知其实际申请空间大小。但是free的时候会根据实际空间大小进行释放。
	//对于strdup函数里边用到了strlen,strlen默认字符串都是非空的，只有遇到'\0'才会停止。所以不能为NULL；
	message = mes?strdup(mes):NULL;
	if (file != NULL){
		char sl[10] = {0};
		itoa(line, sl, 10);
		//最后的+2，一个是：一个是'\0'
		//bugfix,这里不能用sizeof,sizeof只能做静态判断，不能做动态判断。
		// location = reinterpret_cast<char*>(malloc(sizeof(file)+sizeof(sl)+7));
		// 字符串动态判断要用strlen
		location = reinterpret_cast<char*>(malloc(strlen(file) + strlen(sl) + 2));
		//这里不能抛异常，因为所在类是异常父类，如果抛出异常会造成死循环：
		//1.Excepion内存申请抛出异常
		//2.构造另一个Exceotion,内存同样会有问题，再次抛出一样，进而变成一个死循环
		//因此在Exception类中内存·申请异常只会返回一个NULL指针。
		if (location != NULL){
			strcpy(location, file);
			strcat(location, ":");
			strcat(location, sl);
		}
	}
	else{
		location = NULL;
	}
}

Exception::Exception(const char* mes){
	//C++中NULL定义为0，c语言中是(void*)0
	init(mes,NULL,0);
}

Exception::Exception::Exception(const char* file, unsigned int line){
	init(NULL, file, line);
}

Exception::Exception(const char* mes, const char* file, unsigned int line){
	init(mes, file, line);
}

Exception::Exception(const Exception& excpt){
	//防止一个异常被析构而影响其他异常，拷贝构造函数采用深拷贝
	message = strdup(excpt.message);
	location = strdup(excpt.location);
}
	
Exception& Exception::operator = (const Exception& excpt){
	if (this != &excpt){
		message = strdup(excpt.message);
		location = strdup(excpt.location);
	}
	return *this;
}

char* Exception::GetMessage() const{
	return message;

}
char* Exception::GetLocation() const{
	return location;
}


Exception::~Exception(){
	free(message);
	free(location);
}


}