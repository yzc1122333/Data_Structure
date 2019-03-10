#ifndef __STRING_H__
#define __STRING_H__

#include "Object.h"
//ostream, istream
#include <iostream>

/*
历史遗留问题：
	c语言不支持真正意义上的字符串
	c语言用字符串数组和以为函数实现字符串操作
	c语言不支持自定义类型，因此无法获得字符串类型
从c到C++的进化引入了自定义类型
在C++中可以通过类完成字符串类型的转换，但是C++为了兼容C语言没有官方的字符串类，都是各个厂商自己提供的字符串类。因此有必要提供自己的字符串类

字符串类是工程设计必不可少的组件
字符串中应包含常用字符串操作
	--增
	--删
	--改
	--查

实现注意事项：
	无缝实现Sting对象于char* 字符串的互操作
	操作符重载函数需要考虑是否支持const版本
	通过c语言中的字符串函数实现Sting的成员函数

注意事项：
	对于空指针：
	如果const char* s;需要检测指针是否为空，因为c语言的很多库函数是不带 为空检测的
	s = s?s:"";

	对于单个字符：
	c语言库很多函数都是通过'\0'来检测中止的，但是函数本身并没有是否为字符串的检测，因此，对于单个字符，需要手动加上结束符，使字符变成字符串
	c = {c,'\0'};

	对于C++的输出，如果类型是const char*,那么，cout会自动输出指针所指字符串，因此对于想得到string内容的函数，返回值是const char*。
	如果想得到字符串的指针，需要用static_cast将其转换成void*，然后用cout输出。

	字符串类中的字符串都是分布在堆空间上的
*/
using namespace std;
namespace YzcLib{

class String: public Object{
protected:
	char* str;
	unsigned int length;

	//构造函数不能够复用，复用会导致产生临时对象，因此采用重新定义一个函数，在每个构造函数里调用，以此达到函数复用
	void _init(const char* s);
	bool _equal(const char* s1, const char* s2) const;

/*
朴素字符串查找算法：
while(i <= slen && j <= tlen){
	sum++;
	if(s[i - 1] == t[j - 1]){//如果相等，则继续比较后边字符串
		i++;
		j++;
	}
	else{
		i = j + 2;	//i 回退到上一轮开始比较的下一个字符
		j = 1;		//j 回退到第1个字符
	}
}
字符串字串查找，暴力搜寻算法复杂度较大，因此考虑KMP算法，这样能保证在线性时间段内查找到子串
KMP算法：
	朴素算法的优化线索：
		s -> a b c d e ... '\0'
		p -> a b c z '\0'

		因为 p[a] != p[b] 且 p[b] == s[b]；
		所以 怕[a] != s[b]
		结论，子串p右移一位比较没有意义
	一些发现：
		匹配右移时的右移位数与字串本身相关， 与目标串无关
		移动位数 = 以匹配的字符数 - 对应的部分匹配值
		任意的字串都存在一个唯一的部分匹配表(内部保存对应位的部分匹配值)

部分匹配表的计算
	前缀：除了最后一个字符以外，一个字符串的全部头部组合
	后缀：除了第一个字符串以外， 一个字符串的全部尾部组合
	部分匹配值：前缀和后缀最长共有元素的长度
	例子：
		1  2  3  4  5  6  7
		A  B  C  D  A  B  D
		0  0  0  0  1  2  0
	如果第七位匹配失败(前6位匹配成功)->查表PMT[6]->右移位数为：6 - PMT[6] = 6-2 = 4

	为了方便计算机计算，采用动态规划的方式计算部分匹配表
	实现关键：
		PMT[1] = 0 (下标为0的元素匹配值为0)
		从2个关键字开始递推（从下表为1的字符开始递推）
		假设PMT[n] = PMT[n - 1] + 1(最长共有元素)
		假设不成立，PMT[n]在PMT[n-1]的基础上减小

			k = PMT[n - 1]
			t[j-k+1], t[j-k+2], ... , t[j - 1], t[j]
			t[1]		t[2]	...   t[k - 1]	t[k]

			当t[k] != t[j],k' = PMT[PMT[k - 1] -1] = PMT[k-1], 进行回退(k < k')
			t[j-k+1], t[j-k+2], ... , t[j-k'+1], t[j-k'+2], ..., t[j - 1], t[j]
										t[1]	    t[2]	 ... t[k' - 1] t[k']

			如果k'依旧不匹配,在寻找k'' = PMT[PMT[PMT[k - 1] - 1] - 1] = PMT[k' - 1]
			t[j-k'+1], t[j-k'+2], ... , t[j-k''+1], t[j-k''+2], ..., t[j - 1], t[j]
										   t[1]	      t[2]	    ... t[k'' - 1] t[k'']			

KMP算法的实现：
				   i
		s -> a b c d e ... '\0'
		p -> a b c z '\0'
				   j

		下标i处匹配失败，前j位匹配成功
		查表PMT[j - 1](因为当前字符不匹配，要寻找前一个元素的前后缀相同的元素，然后去比对前一个前后缀相同的元素是否匹配，如果不匹配再向前寻找)
		p右移位数(j - PMT[j - 1])[也可以看成是j进行回退，j = j - (j - LL) = LL = PMT[j - 1],j回退到PMT[j - 1]]
		
*/

	//制作部分匹配表
	int* _make_pmt(const char* p, int* pmt, unsigned int len) const;
	int _kmp(const char* p) const;

public:
//构造函数
String();
String(const char* s);
String(const String& s);
String(const char c);

//获取string内容
const char* Str() const;
//获取string长度
unsigned int Length()const;

/*
输入输出操作符重载
因为如果使用cout，cin进行输入输出，需要保证，输出的类型是ostream和istream,因此要以友元的方式重载>>和<<
为了能够链式操作，返回值类型为ostream& 和istream&
*/

friend ostream& operator <<(ostream& out,const String& s);
friend istream& operator >>(istream& in, const String& s);

/*
操作符重载，每一个操作符都要实现两个版本
	一个是string和string的
	一个是string和const char*的（为了兼容c语言用法）
*/

bool operator == (const String& s) const;
bool operator == (const char* s) const;

bool operator != (const String& s) const;
bool operator != (const char* s) const;

bool operator > (const String& s) const;
bool operator > (const char* s) const;

bool operator < (const String& s) const;
bool operator < (const char* s) const;

bool operator >= (const String& s) const;
bool operator >= (const char* s) const;

bool operator <= (const String& s) const;
bool operator <= (const char* s) const;

String& operator += (const String& s);
String& operator += (const char* s);

String operator + (const String& s) const;
String operator + (const char* s) const;

String& operator = (const String s);
String& operator = (const char* s);

//重载数组访问操作符，访问指定下标
//当i取值范围不合法时，抛出异常合法范围在[0, length)
char& operator [](unsigned int i);
char operator [](unsigned int i) const;

//一组函数，判断是否指定字符串开始或结束
bool StartWith(const char* s) const;
bool StartWith(const String& s) const;
bool EndOf(const char* s) const;
bool EndOf(const String& s) const;

//在指定位置插入字符串
String& Insert(int i, const char* s);
String& Insert(int i, const String& s);

//在指定位置删除指定长度
//i + len k可能会大于字符串长度（length），对于这种情况，只删除[i, length- 1]的部分
//i的输入范围是[0, length - 1],否则不做任何操作，这样方便其他函数重载复用(Remove函数重载，operator-重载)
String& Remove(int i, unsigned int len);

//去掉字符串两端的空白字符
String& Trim();

//字符串所有子串操作
//子串查找
int IndexOf(const char* s) const;
int IndexOf(const String& s) const;

/*
在字符串中将指定的子串删除
	1.根据KMP在目标字符串中查找子串的位置
	2.通过子串位置和子串长度进行删除
*/
String& Remove(const char* s);
String& Remove(const String& s);

/*
字符串的减法操作定义（operator -）
	使用Remove实现字符串间的减法操作
		字符串自身不被修改
		返回产生的新串
*/
String operator -(const char* s);
String operator -(const String& s);

String operator -=(const char* s);
String operator -=(const String& s);

/*
字符串中的子串替换
*/
String& Replace(const char* t, const char* s);
String& Replace(const String& t, const char* s);
String& Replace(const char* t, const String& s);
String& Replace(const String& t, const String& s);

/*
从字符串中创建子串
	以 i 为起点提取长度为 len 的子串
	子串返回一个新串，不会改变字符串本身状态
*/
String Sub(int i, unsigned int len) const;



~String();
};



}

/*
Test Code:
	int array[] = {3,3,1,2,5,2,4,5};
	Sort::Quick_Sort(array, 8, Sort::INCREASING);

	for(int i = 0; i < 8; i++){
		cout<<array[i]<<endl;
	}

	Sort::Quick_Sort(array, 8, Sort::DECREASING);

	for(int i = 0; i < 8; i++){
		cout<<array[i]<<endl;
	}
*/

#endif