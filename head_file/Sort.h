#ifndef __SORT_H__
#define __SORT_H__

#include "Object.h"
/*
Sort类中的排序函数

Insertion_Sort(T a[],unsigned int  len,bool model );

Selection_Sort(T a[],unsigned int  len,bool model )

Bubble_Sort(T a[],unsigned int  len, bool model )

Shell_Sort(T a[], unsigned int len, bool model)

Quick_Sort(T a[],unsigned int len, bool model );

Merge_Sort(T a[],unsigned int len, bool model )
*/
/*
排序的一般定义：排序是计算机内部经常进行的一种操作，其目的是将一组“无序”的数据元素调整为有序的数据元素
排序的数学意义
	假设包含n个数据元素的序列为：{R1,R2, ... , Rn}，其相应的关键字序列为：{K1, K2, ...Kn}

	这些关键字之间可以进行比较，即存在一个关系 Kp1 <= Kp2 <= ... <= Kpn
	按此固有顺序将上式记录序列重新安排为{Rp1, Rp2, ..., Rpn}的操作成为排序

排序的稳定性：
	如果在序列中有两个数据元素 Ri, Rj, 它们的关键字Ki = Kj， 且在排序之前，对象Ri排在Rj前面；如果排序之后，对象Ri仍在对象Rj的前面，则成这个排序算法是稳定的，否则称这个排序是不稳定的
Stable Sorting Algorithms:
	Insertion Sort
	Merge Sort
	Bubble Sort
	Tim Sort
	Counting Sort
Unstable Sorting Algorithms
	Heap Sort
	Selection Sort
	Shell Sort
	Quick Sort


稳定性的意义
1、如果只是简单的进行数字的排序，那么稳定性将毫无意义。
2、如果排序的内容仅仅是一个复杂对象的某一个数字属性，那么稳定性依旧将毫无意义（所谓的交换操作的开销已经算在算法的开销内了，如果嫌弃这种开销，不如换算法好了？）
3、如果要排序的内容是一个复杂对象的多个数字属性，但是其原本的初始顺序毫无意义，那么稳定性依旧将毫无意义。
4、除非要排序的内容是一个复杂对象的多个数字属性，且其原本的初始顺序存在意义，那么我们需要在二次排序的基础上保持原有排序的意义，才需要使用到稳定性的算法，例如要排序的内容是一组原本按照价格高低排序的对象，如今需要按照销量高低排序，使用稳定性算法，可以使得想同销量的对象依旧保持着价格高低的排序展现，只有销量不同的才会重新排序。（当然，如果需求不需要保持初始的排序意义，那么使用稳定性算法依旧将毫无意义）。



多关键字排序：为了保证排序的稳定性（不出现排序的二义性），不同关键字需要考虑相互之间的优先级顺序

只需要在单关键字的比较操作进行一些处理，就可以将单关键字操作升级为多关键字比较
以两个关键字(Key1, Key2)为例
struct Test: public Object
{
	int key1; 	//(high priority)
	int Key2;	//(low priority)

	Test(int k1, int k2){
		key1 = k1;
		key2 = k2;
	}
	//多关键字比较操作
	bool operator == ( const Test& t){
		return (key1 == t.key1)&&( key2 == t.key2);
	}
	//!=与==是互逆的，因此采用 == 取反
	bool operator !=( const Test& t){
		return !(*this == t);
	}

	//利用短路规则，如果(key1 < t.key1)，就不会对key2进行比较。如果(key1 == t.key1)，那么对key2进行比较
	bool operator < ( const Test& t){
		return (key1 < t.key1) ||((key1 == t.key1)&&(key2 < t.key2));
	}

	//<与>=是互逆的，因此采用 < 取反,得到 >=
	bool operator >=( const Test& t){
		return !(*this < t);
	}

	//利用短路规则，如果(key1 < t.key1)，就不会对key2进行比较。如果(key1 == t.key1)，那么对key2进行比较
	bool operator > ( const Test& t){
		return (key1 > t.key1) ||((key1 == t.key1)&&(key2 > t.key2));
	}

	//> 与 <= 是互逆的，因此采用 > 取反,得到 <=
	bool operator <=( const Test& t){
		return !(*this > t);
	}

排序的关键操作
	比较：任意两个元素通过比较操作确定先后顺序
	交换：数据元素之间需要交换才能得到预期结果

排序的审判：
	时间性能
		关键性能差异体现在比较和交换的数量
	辅助存储空间
		为了完成排序所需要的额外存储空间
		必要时可以 空间换时间
		对于硬件空间受限的情况（嵌入式开发），需要考虑时间换空间，尽可能小的使用辅助存储空间
	算法的实现复杂性
		过于复杂的算法可能影响可读性和维护性

排序稳定性是选择排序算法的因素之一
多关键字和单关键字排序本质无区别
排序的时间性能是区分排序算法好坏的主要因素

}


*/


namespace YzcLib{

class Sort: public Object{
private:
	//由于Sort类的对象构造函数都设置成私有的，因此sort对象是无法构造的，只能使用其静态函数
	Sort();
	Sort(const Sort&);
	Sort& operator= (const Sort&);

	//对于某些复杂的类，交换才是主要的负担
	template< typename T>
	static void _swap(T&a, T& b);

	// template< typename T>
	// static void Merge_Sort(T a[], T helper[], unsigned int begin, unsigned int end, bool model);

	// template< typename T>
	// static void Merge_Sort(T a[], T helper[], unsigned int begin, unsigned int mid, unsigned int end, bool model);
	template< typename T>
	static void Merge(T a[], T helper[], unsigned int begin, unsigned int mid, unsigned int end, bool model = true);

	template< typename T>
	static void MergePass(T a[], T helper[], unsigned int gap, unsigned int len, bool model = true);
	//快速排序中将序列分成大于和小于基准的部分
	template< typename T>
	static int _partition(T a[], unsigned int begin, unsigned int end, bool model = true);
	//重载Quick_Sort是因为在递归的过程中不止需要长度信息，还需要位置信息
	template< typename T>
	static void Quick_Sort(T a[], unsigned int begin, unsigned int end, bool model );
public:
	enum{ INCREASING = true,
		   DECREASING = false};
	/*
	Insertion Sort
	插入排序的基本思想
		当插入第i(i >= 1)个元素时， 前面的V[0], V[1], ..., V[i - 1]已经排好顺序； 这时用V[i]的关键字与V[i - 1], V[i - 2], ..., V[0]的关键字进行比较，找到V[i]插入，原来位置上的对象向后顺移

	第i次插入排序

					___________________________
					| 将R[i]按照顺序插入有序序列 |
					|						   |
			R0, R1, ...,R[i - 1] 		|	  R[i], ..., R[n]
			有序序列R[0:i - 1]		 	 |      无序序列R[i:n]

			R0, R1, ...,R[i - 1], R[i] 	|  R[i+1], ..., R[n]
				有序序列R[0:i]		 	 |      无序序列R[i + 1:n]	
	插入排序每次将第i个元素插入前边 i - 1 个已排元素中
	时间复杂度为O(n^2)

	对于基本有序的序列只需要很少的时间就可以排序完毕
	*/
	template< typename T>
	static void Insertion_Sort(T a[],unsigned int  len,bool model = true);
	/*
	Selection Sort
	选择排序的基本思想
		每次（例如第i次，i = 0,1,..,n-2）从后面n-i个待排的数据元素中选出关键字最小的元素，作为有序元素序列第i个元素
		**若所在位置处的元素已经是最小的，就无需交换（尽量减小交换次数）
		第i次选择排序：
			R0, R1, ...,R[i - 1] 	|   R[i], R[i + 1]， ..., R[n]
			有序序列R[0:i - 1]		 |      无序序列R[i:n]
									|
									|	从中选出关键字最小的元素
									|
			R0, R1, ...,R[i - 1], 		R[i] 	|  R[i+1], ..., R[n]
				有序序列R[0:i]		 |      无序序列R[i + 1:n]		
	选择排序每次选择未排元素中最小的
	选择排序时不稳定的排序法，插入排序时稳定的排序方法
	时间复杂度为O(n^2)				
	*/
	template< typename T>
	static void Selection_Sort(T a[],unsigned int  len,bool model = true);
	/*
	Bubble Sort
	每次从后向前进行（假设为第i次）,j = n -1; n - 2; ...; i; 两两比较V[j - 1] 和 V[j]的关键字；如发生逆序，则交换V[j - 1] 和 V[j]

	冒泡排序设置exchange标志，用来表示是否发生交换。
	当待排序队列已经是有序的状态，只需要循环一次，就可以结束
	减少了循环次数
	冒泡排序是一种稳定排序
	算法复杂度O(1+2+...+n) = O((1+n)*n/2) = O(n^2)
	*/
	template< typename T>
	static void Bubble_Sort(T a[],unsigned int  len, bool model = true);

	/*
	Shell Sort
	希尔排序基本思想：
		将待排序序列划分为若干组，在每一组内进行插入排序，以使整个序列基本有序，然后在对整个序列进行插入排序

	希尔排序实例：
		将n个数据元素分组成d个子序列
			{R[1], R[1+d], R[1+2d], ..., R[1+kd]}
			{R[2], R[2+d], R[2+2d], ..., R[2+kd]}

			...

			{R[d], R[2d], R[3d], ..., R[(k + 1)d]}

		其中， d成为增量， 它的值在排序过程中从大到小逐渐缩小，知道最后一次增量减少到 1

	希尔排序是一种不稳定排序法
	不同的增量取值会得到不同的算法复杂度
	使用Hibbard增量的希尔排序的最坏情形运行时间为θ(N^3/2)

	增量			复杂度
	n/(2^l)			  O(n^2)
	2^k - 1			  O(n^3/2) [平均复杂度，最好是O(nlongn), 最坏O(n^2)]
	(2^i)*(3^j)		  O(n(logn)^2)

	步长为2^k - 1时
	最好情况整个序列已经有序，此时
	complexity = n +			k = 1
				 3*(n/3) + 		k = 2
				 7*(n/7) +		k = 3
				  ... 

	因为要确保 2^k - 1 < n
	所以 k < log(n + 1)
	每一次n个元素都要遍历一次，只不过可能在不同的组里
	一共执行k次，所以complexity = O(nlog(n+1))

	最坏情况是完全逆序的情况，此时
	complexity = n^2 +			k = 1
				 3*(n/3)^2 + 		k = 2
				 7*(n/7)^2 +		k = 3
				  ... 

	complexity = n^2 *(1 + 1/3 + 1/7 + ... + 1/(2^k - 1))
	< n^2 * (1 + 1/2 + 1/4 + 1/16 + ... + 1/2^(k - 1))
	=n^2 * 2
	=O(n^2)

	可以证明复杂度平均值在最好最坏之间是O(n^1.5),但是严格证明非常难，因此略过
	*/
	template< typename T>
	static void Shell_Sort(T a[], unsigned int len, bool model = true);

	/*
	归并排序的基本思想
		将两个或两个以上的有序序列合成一个新的有序序列
		有序序列
			V[0] ... V[m] 和 V[m+1] .. V[n-1]
						|    |
						|    |
						|    |
					   \|    |/
					    \    /
						 \  /
						  \/
					V[0]  ... V[n - 1]
		注意：V[0] ... V[m] 和 V[m+1] .. V[n-1]必须是有序的才行
		空间复杂度为O(n),需要一定辅助空间
		时间复杂度为O(nlogn),因为被不断的从中间拆分(如果n是偶数，每次是前n/2，后n/2。 如果n是奇数， 分组方式是前 n/2 + 1,后 n/2). 
		因此一共分组归并了k次（k 满足 2^k >= n 且 2^(k - 1) < n）
		=> k = ceil(log(n)), 
		每次n个元素都要进行并归，每次的复杂度是O(n)
		所以总共的时间复杂度是log(n) * O(n) = O(nlong(n))

		归并排序是一种稳定排序
	*/
	template< typename T>
	static void Merge_Sort(T a[],unsigned int len, bool model = true);
	/*
	快速排序是对冒泡排序的一种改进,与冒泡排序都属于交换排序类


	快速排序的基本思想
		任取序列中的某个数据元素作为基准将整个序列划分为左右两个
			左侧子序列所有元素都小于或等于基准元素
			右测子序列所有元素都大于基准元素
			基准元素排在这两个个子序列中间
		分别对着两个子序列重复进行划分，知道所有数据元素都排在相对应位置上。整个过程可以递归进行，以此达到整个数据变成有序序列
	
					 无序的元素序列
						|    |
						|    |
						|    |
					   \|    |/
					    \    /
						 \  /
						  \/
			无序子序列(1) | 基准 | 无序子序列(2)
				<= 基准    = 基准   >基准
				   \               /
					分别进行快速排序
		
		基准可以有很多中选择方法，最简单的就是选择左侧或右侧的第一个元素.

		然后将整个序列比基准小的元素放在一边，将大于基准的放在基准的另一边，等于基准的放到一组。

		基本步骤（以从小到大排序为例，从大到小反之）：
			1.选区基准点,一般为最左边或最右边的数（以最左边为例，右边反之）
			2.将序列最左边的数标识为左标记，最右边的数标识为右标记。

			#########准备工作完毕#########

			3.基准在左边，因此让右标记向左移动，直到遇到比基准小的数停止
			4.右标记停止移动后，左标记向右移动，遇到比基准大的值停止移动
			5.做右标记的值进行交换
			6.回到步骤3继续移动，直至左右标记重合停止

			############################
			7.将基准值与左右标记重合位置的值进行交换

			异常步骤可以保证基准左边都 <= 基准
							  右边都 >基准

			############################
			8.对左右大于基准和小于等于基准的两部分进行递归，直至最终完成排序


		由于关键字的比较和交换是跳跃进行的，快速排序是一种不稳定排序

		算法复杂度分析：

		最好情况：
		每一次所选的基准恰好是中位数,此时树的两边划分的很均匀，这颗树的深度为floor(log(n)) + 1,对于这颗递归树仅需要执行floor(log(n))次

		此时
						5
					   / \
					  /   \
				     /     \
					2       7
				   / \      / \
				  /   \    /   \
				 1     4  6     8
				 	  /          \
					3             9
		
		最坏的情况:
		待排序的序列为正序或者逆序，每一次划分只得到一个比上一次划分少一个的记录的子序列，另一个为空，两种情况如下图：
			1                                  9
			 \                                /
			  2                              8
			   \                            /
			    3                          7
				 \                        /
				  4                      6
    			   \                    /
    			    5                  5
					 \                / 
					  6              4
					   \            /
					    7          3
						 \        /
						  8      2
						   \    /
						    9  1
	此时一共需要执行n-1次，每次需要处理的元素减一
	complexity = (n - 1) + (n - 2) + ...+ 1
			   = O((n - 1 + 1)*(n - 1) / 2)
			   = O(n^2)

	相比归并排序，快排最大的优点是空间复杂度为O(1)
	*/
	template< typename T>
	static void Quick_Sort(T a[],unsigned int len, bool model = true);

	/*
	异常的排序函数都只支持原生数组，但是这个库中有数组类，
	因此从道理上将也应该支持，因此需要将排序函数进行重载支持数组类
	数组类的排序函数
	*/
	template< typename T>
	static void Insertion_Sort(Array<T>& a, bool model = true);

	template< typename T>
	static void Selection_Sort(Array<T>& a, bool model = true);

	template< typename T>
	static void Bubble_Sort(Array<T>& a, bool model = true);

	template< typename T>
	static void Shell_Sort(Array<T>& a, bool model = true);

	template< typename T>
	static void Merge_Sort(Array<T>& a, bool model = true);


	template< typename T>
	static void Quick_Sort(Array<T>& a, bool model = true);
};

template< typename T>
void Sort::_swap(T& a, T&b){
	T temp  = a;
	a = b;
	b = temp;
}

template< typename T>
void Sort:: Selection_Sort(T a[], unsigned int len, bool model){
	unsigned int minormax;
	for(int j = 0; j < len; j++){
		minormax = j;
		//从j+1开始，j没必要和j自己比
		for(int i = j + 1; i < len; i++){
			//尽量减少交换，所以是>,而不是>=
			if(model? a[minormax] > a[i] : a[minormax] < a[i]){
				minormax = i;
			}
		}
		//_swap 返回值是void,所以用逗号表达式，返回一个1，确保能够通过类型检测
		(minormax != j) && (_swap(a[minormax],a[j]), 1);
	}
}


template< typename T>
void Sort::Insertion_Sort(T a[], unsigned int len, bool model){
	for(int i = 0; i<len - 1; i++){
		//更新i+1 个元素的信息
		int k = i+1;
		T temp = a[k];
		//将第i+1个元素插入到前i个有序序列中，只有一个元素时不用排序，没有等于号，等于不交换，尽量减少交换次数
		for(int j = k; j > 0 && (model ? (temp < a[j - 1]) : (temp > a[j - 1])); j--){
			a[j] = a[j - 1];
			k = j-1;
		}
		if(k != (i + 1))  a[k] = temp;
	}

}

/*
以从小到大为例：
对于第i次冒泡排序的目的是将无需的序列中最小的元素放到i号位置
从最后一个元素向前遍历
	如果遇到更小的元素，直接放弃当前元素使用更小的
		不发生交换，exchange = 0
	如果下一个元素比当前元素大，将当前元素与下一个元素交换
		发生交换 exchange = 1
*/
template< typename T>
void Sort::Bubble_Sort(T a[], unsigned int len, bool model){
	bool  exchange  = true;
	for(int i = 0; i < len; i++){
		exchange  = false;
		for(int j = len - 1; j > i; j-- ){
			if(model ? (a[j] < a[j - 1]) :(a[j] > a[j - 1])){
				//如果第j个元素比第j-1个元素更符合条件，那么第j个元素要和j-1个元素换位
				//如果第j-1个元素更优，那么不动，下一轮j--之后，用j-1的元素继续和前边元素比较
				_swap(a[j], a[j - 1]);
				exchange = true;
			}
		}
	}
}	


template< typename T>
void Sort::Shell_Sort(T a[], unsigned int len, bool model){
	int d = len;
	do{
		//采用/3 + 1的增量方式，但是不一定很好，具体详细证明还没有研究出来
		d = d/3+1;
		//间隔为d的插入排序
		//每组第一个无需排序，因为每组只有一个元素，只有一种顺序，所以从第二个元素开始排序，即i = d，开始
		//每组每次递增i+d，但是d组，每组都要递增，最终变成i++。
		//每组抽一个元素排完序，然后再抽出每组下一个继续排序。
		for(int i = d; i < len; i++){
			//将要插入的元素拿出
			int k = i;
			T temp = a[k];
			//对于希尔排序的每组第0个元素是[0, d-1],因此shell排序的j>(d-1),就对应着插入排序的j>0
			for(int j = k; j > (d-1) && (model ?(temp < a[j - d]) :(temp > a[j - d])); j -= d){
				a[j] = a[j - d];
				k = j - d;
			}
			if(k != i) a[k] = temp;
		}
	//当步长减到1，就停止循环
	}while(d > 1);
}

// template< typename T>
// void Sort::Merge_Sort(T a[], T helper[], unsigned int begin, unsigned int mid, unsigned int end, bool model){
// 	//需要两个计数器，计数两组数组中以排序的个数
// 	int i = begin;
// 	int j = mid + 1;
// 	//helper下标计数器
// 	int k = begin;
// 	while((i <= mid )&&( j <= end) ){
// 		if(model ? a[i] < a[j] : a[i] > a[j]){
// 			//在使用索引的时候执行++，可以省掉两行i++; k++; 的代码，更加简洁
// 			helper[k++] = a[i++];
// 		}
// 		else{
// 			helper[k++] = a[j++];
// 		}
// 	}

// 	//在使用索引的时候执行++，可以省掉两行i++; k++; 的代码，更加简洁
// 	while( i <= mid ){
// 		helper[k++] = a[i++];
// 	}
// 	while( j <= end ){
// 		helper[k++] = a[j++];
// 	}

// 	//将辅助空间拍好顺序的序列拷贝回原数组，进行下一次排序
// 	//如果不拷贝回去，那么下一次就得用a[]来做辅助数组排序
// 	//要注意k是int， begin 是unsigned int，因此两者比较的时候k会先隐式转换成unsigned int，再与begin比较，但是这样要确保k不能取到负数，因为负数转换成unsigned int是一个很大的正数。
// 	//在int 和 unsigned int一起做运算的时候，要留意int是否可能是负数，是负数要单独考虑
// 	while(k > begin ){
// 		k--;
// 		a[k] = helper[k];
// 	}
// }

// template< typename T>
// void Sort::Merge_Sort(T a[], T helper[], unsigned int begin, unsigned int end, bool model){
// 	//begin 永远小于等于end， 当 begin = end(即只剩下一个元素)时退出递归，为了增加可读性，iff begin < end 时， 执行递归
// 	if(begin < end){
// 		unsigned int mid = (begin + end)/2;
// 		Merge_Sort(a, helper, begin, mid, model);
// 		Merge_Sort(a, helper, mid + 1, end, model);
// 		//并归排序合并
// 		Merge_Sort(a, helper, begin, mid, end, model);
// 	}
// }


// template< typename T>
// void Sort::Merge_Sort(T a[], unsigned int len, bool model){
// //申请辅助空间
// 	T* helper = new T [len];
// 	if(helper){
// 		//对应的归并算法,通过对Merge_Sort重载完成
// 		Merge_Sort(a, helper, 0, len - 1, model);
// 	} 
// 	delete[] helper;
// }

template< typename T>
void Sort::Merge(T a[], T helper[], unsigned int begin, unsigned int mid, unsigned int end, bool model){
	//需要两个计数器，计数两组数组中以排序的个数
	int i = begin;
	int j = mid + 1;
	//helper下标计数器
	int k = begin;
	while((i <= mid )&&( j <= end) ){
		if(model ? a[i] < a[j] : a[i] > a[j]){
			//在使用索引的时候执行++，可以省掉两行i++; k++; 的代码，更加简洁
			helper[k++] = a[i++];
		}
		else{
			helper[k++] = a[j++];
		}
	}

	//在使用索引的时候执行++，可以省掉两行i++; k++; 的代码，更加简洁
	while( i <= mid ){
		helper[k++] = a[i++];
	}
	while( j <= end ){
		helper[k++] = a[j++];
	}
}


template< typename T>
void  Sort::MergePass(T a[], T helper[], unsigned int gap, unsigned int len, bool model){
	int i = 0;
	while((i + 2*gap - 1)< len){
		//将每组有gap个元素的组两两归并，第一组[i, i+gap - 1],第二组[i+gap，i + 2*gap - 1];
		Merge(a, helper, i, i + gap - 1, i + 2*gap - 1, model);
		//i更新到下一组的起始位置处
		i += 2*gap;

	}

	//若剩下的元素不足两组的时候需要分开讨论
	//若所生的元素个数大于一组，且不足两组的，将其划分成两组，一组[i, i + gap -1],依旧是gap个，另一组[i + gap, len -1],不足gap个，这两组进行归并
	if((i + gap - 1)< len){
		Merge(a, helper, i, i + gap - 1, len - 1, model);
	}
	else{
	//若所剩元素的数量小于等于一组（gap个元素），无法归并，拷贝到辅助空间中，等待下一次排序
		while(i < len){
			helper[i] = a[i];
			i++;
		}
	}
}
/*
核心思想：从起始位置算起先每一个分一组排序，
				然后每两个分一组排序，然后
				然后每4个分一组排序
				然后8个
				。。。
*/
template< typename T>
void Sort::Merge_Sort(T a[], unsigned int len, bool model){
	T* helper = new T [len];
	if(helper){
		int k = 1;
		while(k <= len){
			/*
				先以helper为辅助空间对a进行排序
				然后再以a为辅助空间对helper的序列进行排序
				这样一次while循环处理两次，确保最终排好序的序列在a上，而不在helper上
				相比while里每次只处理一次（每次处理完都要将helper上的内容拷贝回a）减少了很多开销，确保最后拍好的序列在a上。
			*/
			MergePass(a, helper, k, len, model);
			k *= 2;
			MergePass(helper, a, k, len, model);
			k *= 2;
		}

	} 
	delete[] helper;
}

// template< typename T>
// int Sort::_partition(T a[], unsigned int begin, unsigned int end, bool model){
// 	unsigned int base = begin;
// 	unsigned int r = end;
// 	unsigned int l = begin;
// 	//基准为左边第一个数
// 	int pivot = begin;
// 	//寻找比base大的数停止
// 	while(1){

// 		//右标记向左移动，当遇到小于或等于基准的数停止
// 		while( r > begin  && (model ? (a[base] < a[r]) : (a[base] > a[r]))){r--;}
// 		//如果左右基准重合，（右基准可能会超过做基准，此时左基准在左右基准相交位置），将基准和重合位置元素进行交换，退出循环
// 		if(r <= l){
// 			_swap(a[pivot], a[l]);
// 			pivot = l;
// 			break;
// 		}
// 		//向左移动，当遇到大于基准的值停止移动
// 		while( l < end  && (model ? (a[base] >= a[l]) : (a[base] <= a[l]))){l++;}
// 		//如果左右基准重合，（左基准可能会超过做基准，此时右基准在左右基准相交位置），将基准和重合位置元素进行交换，退出循环
// 		if(r <= l){
// 			_swap(a[pivot], a[r]);
// 			pivot = r;
// 			break;
// 		}
// 		//如果左右基准没有相交，左右基准元素进行交换
// 		_swap(a[r], a[l]);
// 	}
// 	return pivot;
// }

template< typename T>
int Sort::_partition(T a[], unsigned int begin, unsigned int end, bool model){
	//基准为左边第一个数
	int pivot = begin;
	//若输入为一个空序列，则无需排序直接退出
	while(begin < end){
		//右标识向左移动,遇到小于等于基准的数停下
		//begin = 0的时候，（begin ，end都是无符号的）end--就是负数了，所以begin > end
		while((begin < end) && (model ? (a[end] > a[pivot]) : (a[end] < a[pivot]))){end --;}
		//基准和小于等于基准的元素换位置
		_swap(a[begin], a[end]),pivot = end;
		//左标识从左向右移动，遇到大于基准的数停下
		while((begin < end) && (model ? (a[begin] <= a[pivot]): (a[begin] >= a[pivot]))){begin++;}
		//基准和大于基准的元素换位置
		_swap(a[begin], a[end]),pivot = begin;
	}
	return pivot;
}

template< typename T>
void Sort::Quick_Sort(T a[], unsigned int begin, unsigned int end, bool model){
	if(begin < end){
		int pivot = _partition(a, begin, end, model);

		//无符号数要做非负检查。
		//若基准元素的值就是序列的最小值（begin所在位置）要特殊处理，因为begin， end都是无符号数，要考虑prvot - 1 可能小于0的情况
		Quick_Sort(a, begin, (pivot == begin) ? begin : pivot - 1 , model);
		Quick_Sort(a, pivot + 1, end, model);
	}	
}
template< typename T>
void Sort::Quick_Sort(T a[], unsigned int len, bool model){
	Quick_Sort(a, 0, len - 1, model);
}



//数组类排序


template< typename T>
void Sort::Insertion_Sort(Array<T>& a, bool model){
	Insertion_Sort(a.GetArray(), a.Length(), model);
}

template< typename T>
void Sort::Selection_Sort(Array<T>& a, bool model){
	Selection_Sort(a.GetArray(), a.Length(), model);
}

template< typename T>
void Sort::Bubble_Sort(Array<T>& a, bool model ){
	Bubble_Sort(a.GetArray(), a.Length(), model);
}

template< typename T>
void Sort::Shell_Sort(Array<T>& a, bool model){
	Shell_Sort(a.GetArray(), a.Length(), model);
}

template< typename T>
void Sort::Merge_Sort(Array<T>& a, bool model){
	Merge_Sort(a.GetArray(), a.Length(), model);
}


template< typename T>
void Sort::Quick_Sort(Array<T>& a, bool model ){
	Quick_Sort(a.GetArray(), a.Length(), model);
}




// 问题分析
// 	排序过程中不可避免的需要进行交换操作
// 	交换操作的本质为数据元素间的相互复制
// 	当数据元素体积较大时，交换操作消耗时间巨大

// 解决方案：代理模式
// 	1.为待排序数据设置代理对象
// 	2.对代理对象所组成的序列进行排序
// 	3.需要访问有序数据元素时，通过访问代理序列完成
// 		因此被代理的对象所有成员都能够通过代理访问到
// 代理模式的使用有效避开大对象交换时的耗时操作

// 代理模式本质就是用用指针操作间接操作被代理的类，类的赋值从拷贝整个类变成拷贝一个指针，进而减少了赋值交换的开销

// 代理模式对于小对象时没有意义的，因为使用代理模式需要更多的空间资源，并且增大了程序的复杂程度，因此对于小对象性价比不高

// 给出一个例子

// #include <ctime>
// using namespace YzcLib;

// struct Test: public  Object{
//     int id;
//     int data1[1000];
//     int data2[500];

//     bool operator < (const Test& obj){
//         return id < obj.id;
//     }
//     bool operator > (const Test& obj){
//         return id > obj.id;
//     }
//     bool operator <= (const Test& obj){
//         return id <= obj.id;
//     }

//     bool operator >= (const Test& obj){
//         return id >= obj.id;
//     }
// };


// class TestProxy: public Object{
// protected:
//     Test* m_pTest;
// public:
//     //原始对象能够完成的功能，代理函数也要能做
//     int id(){
//         return m_pTest->id;
//     }

//     int* data1(){
//         return m_pTest->data1;
//     }

//     int* data2(){
//         return m_pTest->data2;
//     }

//     //代理函数可以请出委托者
//     Test& test() const{
//         return *m_pTest;
//     }

//     //原始对象的比较操作功能，代理类也要具备
//     //代理类比较本质是原始对象比较

//     bool operator < (const TestProxy& obj){
//         return test() < obj.test();
//     }
//     bool operator > (const TestProxy& obj){
//         return test() > obj.test();
//     }
//     bool operator <= (const TestProxy& obj){
//         return test() <= obj.test();
//     }

//     bool operator >= (const TestProxy& obj){
//         return test() >= obj.test();
//     }

//     Test& operator = (Test& test){
//         m_pTest = &test;
//         return test;
//     }

// };

// Test t[1000];
// TestProxy pt[1000];

// int main() {
//     for(int i = 0; i < 1000; i++){
//         t[i].id = i;
//         pt[i] = t[i];
//     }

//     clock_t begin = 0;
//     clock_t end = 0;

//     begin = clock();
//     Sort::Bubble_Sort(t, 1000, Sort::DECREASING);
//     end = clock();
//     //17000 - 19000个时间单位
//     cout<<"Time:"<<(end - begin)<<endl;

//     begin = clock();
//     Sort::Bubble_Sort(pt, 1000, Sort::DECREASING);
//     end = clock();
//     //不到一个时间单位
//     cout<<"Time:"<<(end - begin)<<endl;
    
//     //使用代理模式不到一个时间单位，速度远比原始类要快得多

// 	/*
// 	深浅拷贝是对于有指针成员而言的
// 	浅拷贝是将对象A中指针成员的值直接拷贝给B对象指针成员
// 	深拷贝是将指针成员所指的区域，也要开辟新的内存，拷贝一遍
// 	*/
// 	/*
// 	因为对于Test对象，数组是和Test分配在一个空间中的，所以Test赋值即使时浅拷贝，依旧要把数组中的每个元素拷贝一遍，这个过程消耗巨大
// 	*/


}




#endif