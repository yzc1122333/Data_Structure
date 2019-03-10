
#include "./../head_file/String.h"
//malloc,free
#include <cstdlib>

#include <cstring>
#include "./../head_file/Exception.h"



namespace YzcLib{

void String::_init(const char* s){
	//使用了strdup意味着使用了malloc，也就是说在string类中，不会有new delete，而使用malloc和free
	//字符串类中的字符串都是分布在堆空间上的	
	str = strdup(s);

	if(str != NULL ){
		length = strlen(str);
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException,"No memory to create String object ...");
	}

}

String::String(){
	_init("");
}

String::String(const char* s){
	_init(s?s:"");
}
String::String(const String& s){
	_init(s.str);
}
String::String(const char c){

	//不能直接_init({c, '\0'}),因为{c, '\0'}不能唯一确定数组类型，可以是char[], short[], int[]，甚至是结构体 
	char s[] = {c, '\0'};
	_init(s);
}

//获取string内容
const char* String::Str() const{
	return str;
}
//获取string长度
unsigned int String::Length()const{
	return length;
}

/*
友元函数，不能加类作用域限制，写成
ostream& String::operator <<(ostream& out, String& s)
加了String::会被认为是类成员函数，会将this指针作为第一个参数进行传递
const String& s必须加const属性
因为编译器会对临时对象尽可能的添加const属性
cout<<(s - "cde"),(s - "cde")没有后续操作的话会被默认为一个const对象
但是如果有后续非const的操作，(s - "cde").Insert(0, "yzc")，(s - "cde")被视为非const的
*/
ostream& operator <<(ostream& out, const String& s){
	out<<s.str;
	return out;
}

istream& operator >>(istream& in, const String& s){
	in>>s.str;
	return in;
}
/*
操作符重载，每一个操作符都要实现两个版本
	一个是string和string的
	一个是string和const char*的
	对于单个字符 char的操作符，并不重载。
之所以要重载const char*版本操作符，因为不重载，const char*就会先调用类型转换构造函数，其中的strdup和strlen的复杂度为O(n)，但是很多操作并用不到length操作，没有必要创建字符串类就可以完成。并且减少了很多开销。因此对const char*进行重载。

对于单个字符，调用类型转换构造函数开销并不是很大，可以接受，因此不单独重载单个字符版本的函数
*/

/*
字符串所有的比较操作都是基于strcmp进行的。
strcmp: This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs until the characters differ or until a terminating null-character is reached.

int strcmp(const char *str1, const char *str2)
	str1 − This is the first string to be compared.
	str2 − This is the second string to be compared.
Return Value:
	if Return value < 0 then it indicates str1 is less than str2.
	if Return value > 0 then it indicates str2 is less than str1.
	if Return value = 0 then it indicates str1 is equal to str2.
*/

bool String::operator == (const String& s) const{
	return (strcmp(str, s.Str()) == 0);
}
bool String::operator == (const char* s) const{
	return (strcmp(str, (s? s : "")) == 0);
}

bool String::operator != (const String& s) const{
	return !(*this == s);
}
bool String::operator != (const char* s) const{
	return !(*this == s);
}

bool String::operator > (const String& s) const{
	return (strcmp(str, s.Str()) > 0);
}
bool String::operator > (const char* s) const{
	return (strcmp(str, (s? s : "")) > 0);
}

bool String::operator < (const String& s) const{
	return (strcmp(str, s.Str()) < 0);
}
bool String::operator < (const char* s) const{
	return (strcmp(str, (s? s : "")) < 0);
}

bool String::operator >= (const String& s) const{
	return (strcmp(str, s.Str()) >= 0);
}
bool String::operator >= (const char* s) const{
	return (strcmp(str, (s? s : "")) >= 0);
}

bool String::operator <= (const String& s) const{
	return (strcmp(str, s.Str()) <= 0);
}
bool String::operator <= (const char* s) const{
	return (strcmp(str, (s? s : "")) <= 0);
}


//返回值为引用，方便链式操作
//为了方便复用，在operator + (const String& s) const内部调用operator + (const char* s)
String String::operator + (const String& s) const{
	return *this + s.Str();
}


String String::operator + (const char* s) const{
	//string类型不能是char*，必须是const char*.因为字符串都是const char*类型，根据左数右指，指针所指向数值是不能变的，如果string 类型是char*,复制之后指针所指向内容变得可变了

	//在String作用域内部，不受private，protected限制。private和protected主要是限制外部作用域的。因此在String作用域内部申请的变量，可以直接访问其私有成员
	String rst;

	unsigned int len = strlen(s? s : "");
	//const char*指针只能赋值给被const修饰的指针，确保指针所指数值不必那，但是被赋值的指针不需要有const属性
	char* new_str = reinterpret_cast<char*>(malloc(length + len + 1));

	if(new_str){
		strcpy(new_str, str);
		strcat(new_str, s? s : "");

		//malloc and free function never throw the exception
		//因此可以将free放在状态更新的前边，依旧满足异常安全
		free(rst.str);

		rst.str = new_str;
		rst.length = strlen(rst.str);
		
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException,"No memory to add string value ...");
	} 
	return rst;
}


//必须要*this = ，不然this的值不会发生改变
String& String::operator += (const String& s){
	return (*this = *this + s.str);
}
String& String::operator += (const char* s) {
	return (*this = *this + s);
}

//返回s.str，而不是s.Str(),因为String内部可以直接访问private属性的成员
String& String::operator = (const String s){
	return (*this = s.str);
}

String& String::operator = (const char* s){
	//检测成员str是否和s相等，若相等则无需赋值
	if( str != s){

		char* new_str = strdup(s? s : "");

		if(new_str){

			free(str);

			str = new_str;

			length = strlen(str);

		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No memory to assign new String value ...");
		}
	}
	return *this;
}

String::~String(){
	free(str);
}


char& String::operator [](unsigned int i){
	if( i < length){
		return str[i];
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter is invalid ...");
	}
}

char String::operator [](unsigned int i) const{
	//可以通过强制类型转换到非const类，调用类中的一些非const成员。当转换到非const成员之后，对于该成员将不会进行const的性质检测（不改变成员的值）
	return const_cast<String&>(*this)[i];
}



bool String::_equal(const char* s1, const char* s2) const{
	bool rst = false;
	unsigned int i = 0;
	while(1){
		if(s2[i] == '\0'){
			rst = true;
			break;
		}
		else if(s1[i] == '\0' || s2[i] != s1[i]){
			break;
		}

		i++;
	}
	return rst;
}

bool String::StartWith(const char* s) const{
	bool rst = (s != NULL);
	if(rst){
		rst = _equal(str, s);
	}
	return rst;
}
bool String::StartWith(const String& s) const{
	return StartWith(s.str);
}
bool String::EndOf(const char* s) const{
	bool rst = (s != NULL);
	if(rst){
		unsigned int len = strlen(s);
		rst = (len <= length)&&_equal(str + length - len, s);
	}
	return rst;
}
bool String::EndOf(const String& s) const{
	return EndOf(s.str);
}	

//在指定位置插入字符串
//需要进行位置合法性检测，不合法抛异常
//需要进行指针的非空检测，若为空，不做任何操作
//i可以理解为第几个位置插入，也可以理解为，插入的位置前边有几个元素，方便变成。
String& String::Insert(int i, const char* s){
	//插入操作取值范围为[0, length],0代表在最前边插入，length代表在尾部插入
	if((i >= 0)&&( i <= length)){
		if((s != NULL)&&(*s != '\0')){
			int len = strlen(s);
			char* new_str = reinterpret_cast<char*>(malloc(length + len + 1));
			if(new_str){
				//分成三块区域，插入前的区域，待插入的区域，插入后的区域
				//要注意new_str也要随着增加，不然相当于重复在new_str上赋值三次，而不是在三个不同的区域赋值
				strncpy(new_str, str, i);
				strcpy(new_str + i, s);
				strcpy(new_str + i + len, str + i);

				str = new_str;
				length = strlen(str);
			}
			else{
				THROW_EXCEPTION(NotEnoughMemoryException, "No memeory to insert string value ...");
			}
		}
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter is invalid ...");
	}
	return *this;
}

String& String::Insert(int i, const String& s){
	return Insert(i, s.str);
}

String& String::Remove(int i, unsigned int len){
	if((i >= 0)&&( i < length)){
		//将i+len的元素向前移动len位。k代表位移次数，最终字符串长度变成 i + k
		unsigned int k = 0;
		while((i + len + k) < length){
			str[i + k] = str[i + k + len];
			k++;
		}
		str[i + k] = '\0';
		length = i + k;
	}
	return *this;
}


//去掉字符串两端的空白字符
String& String::Trim(){
	//start，end本别代表两端第一个不为空格的位置
	unsigned int start = 0;
	unsigned int end = length - 1;

	while(str[start] == ' ') start++;
	while(str[end] == ' ') end--;
	cout<<start<<endl;
	cout<<end<<endl;
	#define LENGTH end - start + 1

	char* new_str = reinterpret_cast<char*>(malloc(LENGTH + 1));

	if(new_str){
		strncpy(new_str, str + start, LENGTH);
		free(str);
		length = LENGTH;
		str = new_str;		
	}

	return *this;
}

// String& String::Trim(){
// 	//start，end本别代表两端第一个不为空格的位置
// 	unsigned int start = 0;
// 	unsigned int end = length - 1;

// 	while(str[start] == ' ') start++;
// 	while(str[end] == ' ') end--;

// 	#define LENGTH end - start + 1

// 	if(start == 0){
// 		str[LENGTH] = '\0';
// 		length = LENGTH;
// 	}
// 	else{
// 		for(int i = 0; i < LENGTH; i++){
// 			str[i] = str[start + i];
// 		}
// 		str[LENGTH] = '\0';
// 		length = LENGTH;
// 	}

// 	return *this;
// }

/*
		PMT[1] = 0 (下标为0的元素匹配值为0)
		从2个关键字开始递推（从下表为1的字符开始递推）
		假设PMT[n] = PMT[n - 1] + 1(最长共有元素)
		假设不成立，PMT[n]在PMT[n-1]的基础上减小
*/
//使用之前，要保证p ！= NULL， pmt ！= NULL， pmt的长度要和len对应
int* String::_make_pmt(const char* p, int* pmt, unsigned int len) const{   //O(m)
	//检测pmt是否为NULL
	int *rst = pmt ? pmt : NULL;
	unsigned int ll = 0;
	if(p != NULL){
		//保证len>0,进入循环
		for(int i = 0; i < len; i++){
			if(i == 0){
				pmt[i] = 0;
			}
			else{
				//pmt中的值为，前后缀最大公共长度
				//退出条件 1)前后缀最大公共长度为0 2)寻找到对应的最大公共长度
				while(ll && p[ll] != p[i]){
					ll = pmt[ll - 1];
				// cout<<pmt[ll - 1]<<endl;
				}
				//如果当前字符相等，最大前后缀公共长度 + 1
				if(p[ll] == p[i]){
					ll++;	
				}
				//寻找到前后缀最大公共长度，赋值给pmt table
				pmt[i] = ll;
			}
		}
	}
	else{
		THROW_EXCEPTION(InvalidParameterException, "The p is NULL, it is invalid for making pmt table ...");
	}
	return rst;
}

//返回值为字串所在位置,若不存在，则返回-1
int String::_kmp(const char* p) const{					//O(m+n)
	//防止p是空指针
	unsigned int pl = p? strlen(p): 0;
	unsigned int sl = length;
	int rst = -1;
	//abcd一定不是abc的子串，因此要求sl >= pl
	if(pl > 0 && sl >= pl){
		int* pwt = new int[pl];
		pwt = _make_pmt(p, pwt, pl);
		if(pwt){
			int j = 0;
			for(int i = 0; i < sl; i++){
				//匹配不i成功，回退,直至匹配成功或者发现无匹配停止
				while(j && p[j] != str[i]){
					j = pwt[j - 1];
				}
				//若匹配成功，匹配长度+1
				if(p[j] == str[i]){
					j++;
				}

				//匹配的长度和输入字符串长度相等
				if(j == pl){
					rst = i - j + 1;
					break;
				}
			}
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No memeory to find sub string ...");
		}
		
		delete [] pwt;
	}
	return rst;
}

int String::IndexOf(const char* s) const{
	return _kmp(s);
}
int String::IndexOf(const String& s) const{
	return _kmp(s.str);
}


String& String::Remove(const char* s){
	return Remove(_kmp(s), s?strlen(s) : 0);
}

String& String::Remove(const String& s){
	return Remove(_kmp(s.str), s.length);
}

String String::operator -(const char* s){
	//因为减法不会影响原字符串，会返回一个新的字符串，所以调用拷贝构造，构造一个和原字符串相同的字符串进行Remove操作
	return String(*this).Remove(s);
}

String String::operator -(const String& s){
	return String(*this).Remove(s.str);
}
/*
对于 +=， -=这种操作，可以返回 this->func, 也可以返回 *this = *this -/+ s。因为重载的 + - 操作不会改变原对象，需要通过赋值函数将重载的 +/-操作赋值给原对象 *this
*/ 
String String::operator -=(const char* s){
	return Remove(s);
}	

String String::operator -=(const String& s){
	return Remove(s.str);
}



String& String::Replace(const char* t, const char* s){
	int loc = _kmp(t);
	if(loc >= 0){
		Remove(t);
		Insert(loc,s);
	}
	return *this;
}

String& String::Replace(const String& t, const char* s){
	return Replace(t.str, s);
}

String& String::Replace(const char* t, const String& s){
	return Replace(t, s.str);
}

String& String::Replace(const String& t, const String& s){
	return Replace(t.str, s.str);
}



String String::Sub(int i, unsigned int len) const{
	if((i >= 0)&&( i < length)){
		/*
		需要对超过字符串长度的部分进行处理
		即（i + len） >= length的部分忽视掉
		*/
		len = ((i + len) < length)? len : length - i;

		char* new_str = reinterpret_cast<char*>(malloc(len + 1));
		if(new_str){
			strncpy(new_str, str + i, len);
			return String(new_str);
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No enough memory to generate a sub string ...");
		}
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
	}

}

}

/*
	cout<<"test_1 begin..."<<endl;
	String s("123");
	//用括号初始化，括号不能为空，不然会被误认为是函数
	String s1;
	cout<<s<<endl;

	// cin>>s;
	// cout<<s<<endl;

	s = 'D';
	cout<<s<<endl;

	s1 = "YZC";
	cout<<s1<<endl;
	cout<<s1.Str()<<endl;
	cout<<s1.Length()<<endl;
	String s2(s1 + s1);
	cout<<"s2 = "<<s2<<endl;
	cout<<endl;
	s = s1+ s;
	cout<<"s = "<<s<<endl;
	cout<<endl;


	cout<<(s1 == s)<<endl;
	cout<<(s1 != s)<<endl;
	cout<<(s1 >= s)<<endl;
	cout<<(s1 > s)<<endl;
	cout<<(s1 <= s)<<endl;
	cout<<(s1 < s)<<endl;

	cout<<endl;
	
	cout<<(s1 == "YZC")<<endl;
	cout<<(s1 != "YZ")<<endl;
	cout<<(s1 >= "YZ")<<endl;
	cout<<(s1 > "YZ")<<endl;
	cout<<(s1 <= "YZZZ")<<endl;
	cout<<(s1 < "YZZZ")<<endl;

	const String s3(s2);
	for(int i = 0; i < s3.Length(); i++){
		cout<<s3[i]<<endl;
	}

	s = s1;
	cout<<s<<endl;

	cout<<"the next########"<<endl;
	cout<<s1.StartWith("YZ")<<endl;
	cout<<s1.StartWith("Z")<<endl;
	cout<<s1.EndOf("YZ")<<endl;
	cout<<s1.EndOf("ZC")<<endl;

	String t1;
	t1.Insert(0, "YZC");
	t1.Insert(3, "sui");
	t1.Insert(3, "wan");

	cout<<t1<<endl;
	cout<<t1.Length()<<endl;
	
	t1 = "   abc    ";
	cout<<t1.Trim().Insert(0, "YZC").Insert(t1.Length(), "wansui")<<endl;
*/ 

/*
PMT table生成代码 Test code
// const char* s = "ABCDABD";
const char* s = "ababax";
int a[strlen(s)] = {0};
int* pmt = _make_pmt(s, a, strlen(s));
cout<<pmt<<endl;
for(int i = 0; i < strlen(s); i++){
	cout<<i<<" : "<<a[i]<<endl;
}
*/

/*
KMP Test code
	String s;

	s = "abcde";
	cout<<s._kmp( "cde")<<endl;

	s = "ababax";
	cout<<s._kmp( "ba")<<endl;


	s = "ababax";
	cout<<s._kmp( "ax")<<endl;
	

	s = "ababax";
	cout<<s._kmp("")<<endl;


	s = "ababax";
	cout<<s._kmp( "ababax")<<endl;

	s = "ababax";
	cout<<s._kmp( "ababaxy")<<endl;
*/
/*
子串Test code
	String s;

	s = "abcde";
	cout<<s.IndexOf( "cde")<<endl;
	cout<<(s - "cde").Insert(0, "yzc")<<endl;
	cout<<endl;

	s = "ababax";
	cout<<s.IndexOf( "ba")<<endl;
	cout<<(s - "ba")<<endl;
	cout<<endl;

	s = "ababax";
	cout<<s.IndexOf( "ax")<<endl;
	cout<<(s - "ax")<<endl;
	cout<<endl;

	s = "ababax";
	cout<<s.IndexOf("")<<endl;
	cout<<(s - "")<<endl;
	cout<<endl;

	s = "ababax";
	cout<<s.IndexOf( "ababax")<<endl;
	cout<<(s - "ababax")<<endl;
	cout<<endl;

	s = "ababax";
	cout<<s.IndexOf( "ababaxy")<<endl;
	cout<<(s - "ababaxy")<<endl;
	cout<<endl;

	cout<<s.Remove(1, 4)<<endl;
	cout<<s.Length()<<endl;

	s = "ababax";
	cout<<s.Remove("bab")<<endl;
	cout<<s.Length()<<endl;
	
	cout<<endl;
	cout<<endl;
	cout<<endl;
	s = "abcde";
	cout<<s.IndexOf( "cde")<<endl;
	s -="cde";
	cout<<(s)<<endl;
	cout<<endl;

	cout<<"Replace"<<endl;
	s = "abcde";
	s.Replace("cde", "yzc");
	cout<<s<<endl;

	cout<<"###Sub###"<<endl;
	s = "ababax";
	cout<<s.Sub(3, 2)<<endl;
	cout<<s.Sub(3, 10)<<endl;
	cout<<s.Sub(-1, 10)<<endl;
	return 0;
*/