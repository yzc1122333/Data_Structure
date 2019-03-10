#ifndef __LISTGRAPH_H__
#define __LISTGRAPH_H__

#include "Graph.h"
#include "LinkList.h"
#include "Exception.h"
#include "DynamicArray.h"

/*
邻接矩阵法残留问题：
	无法动态添加/删除顶点
	即使整个矩阵只占中四个字节，但对于较大的图（1000 节点）依旧空间紧张
		N = 1000，邻接矩阵就需要 1000 * 1000 大约4M空间
	
为了进一步提高空间使用率，可以考虑使用链表替换属猪，将邻接矩阵变成邻接链表

邻接链表法：
	图中所有顶点按编号存储于同一个链表中
	每一各顶点对应一个链表， 用于存储始发于该定点的边
	每一条边的信息包含：起点， 终点， 权值

	0 A -> (0, 1, 5) ->(0, 3, 6)
	  |
	1 B -> (1, 2, 8)
	  |
	2 C -> (2, 3, 2)
	  |
	3 D -> (3, 1, 9)

邻接链表法使用链表对图相关的数据进行存储
每一各顶点关联一个链表， 用于存储边相关的数据
所有顶点按照编号被组织再同一个链表中
邻接链表法实现的图能够支持动态添加/删除顶点

*/

namespace YzcLib{

template<typename V, typename E>
class ListGraph: public Graph<V, E>{
protected:
	struct Vertex: public Object{
		//为了节省空间，node可以没有value，需要对vertex赋值的时候再申请
		V* data;				//顶点数据元素值
		LinkList<Edge<E> > edge;	//邻接于该点的边

		//因为有指针，需要初始化，赋初值为NULL，防止野指针，所以需要构造函数

		Vertex():data(NULL){}
	};

	LinkList<Vertex*> m_list;

	#define CHECKBOUND(i) ((i >= 0) && (i < VCount()))
public:
	ListGraph(unsigned int n = 0);

	V GetVertex(int i);

	bool GetVertex(int i, V& value);

	bool SetVertex(int i, const V& value);
	
	bool IsAdjacent(int i, int j);

	SharedPointer<Array<int>> GetAdjacent(int i);

	E GetEdge(int i, int j);
	bool GetEdge(int i, int j, E& value);

	bool SetEdge(int i, int j, const E& value);

	bool RemoveEdge(int i, int j);

	int VCount();

	int ECount();

	int OD(int i);

	int ID(int i);

	//需要增加一些vertex相关的函数
	//增加新的节点，返回顶点编号，如果返回地址，用户就可以更改图的元素，破坏了封装性
	int AddVertex();
	//增加新顶点的同时附加数据元素
	int AddVertex(const V& value);
	//删除最近增加的顶点
	void RemoveVertex();

	~ListGraph();
};

//可以初始化点的个数
template<typename V, typename E>
ListGraph<V, E>::ListGraph(unsigned int n){
	for(int i = 0; i < n; i++){
		//构造函数加入的都是vertex的data为空的节点
		AddVertex();
	}
}

/*
在栈上增加一个新的vertex
插入失败返回-1
*/
template<typename V, typename E>
int ListGraph<V, E>::AddVertex(){
	Vertex* v = new Vertex;
	int rst = -1;
	if(v != NULL){
		//默认新的vertex插在最后
		//vertex是有编号的，所以按照编号逐一在末尾插入元素
		m_list.Insert(v) && \
		(rst = m_list.Length() - 1);
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create new vertex object...");
	}
	return rst;
}

/*
在栈上增加一个新的vertex，若申请成功，则赋初 value给vertex
*/
template<typename V, typename E>
int ListGraph<V, E>::AddVertex(const V& value){
	int i = AddVertex();
	int rst = -1;
	//vertex添加成功
	if(i >= 0){
		SetVertex(i, value) && \
		(rst = i);
	}
	return rst;
}

/*获取i位置vertex的值，vertex并不一定要有值，若vertex并没有赋值，则抛异常*/
template<typename V, typename E>
V ListGraph<V, E>::GetVertex(int i){
	V rst;

	if(!GetVertex(i, rst)){
		THROW_EXCEPTION(InvalidParameterException, "parameter i is invalid...");
	}

	return rst;
}

template<typename V, typename E>
bool ListGraph<V, E>::GetVertex(int i, V& value){
	bool rst = CHECKBOUND(i);
	if(rst){

		Vertex* v = m_list.Get(i);

		if(v->data != NULL){
			value = *(v->data);
		}
		else{
			THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex...");
		}
	}
	return rst;
}

/*
设置value的值
	若vertex是第一次使用，在栈上申请空间
	若vertex已经被使用过，直接修改栈空间的值
*/
template<typename V, typename E>
bool ListGraph<V, E>::SetVertex(int i, const V& value){
	bool rst = CHECKBOUND(i);
	if(rst){
		Vertex* v = m_list.Get(i);
		//为了确保异常安全，防止V*指针申请内存，但是data在赋值时抛出异常导致data值不正确，及状态不统一的情况出现，先对temp进行操作，最终没有异常的情况下赋值给v->data
		V* temp  = v->data;
		if(temp == NULL){
			temp = new V;
		}

		if(temp != NULL){
			*temp = value;
			v->data = temp;
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create new vertex value...");
		}
	}
	return rst;
}

/*
默认删除链表最后一个vertex
将其他vertex中tail为i的边删除
将vertex[i]的邻接链表删除
	先delete V*指针
	再delete vertex

remove所删除的是继承自Object的对象，因此delete是不抛异常的，remove就不会抛异常，所以唯一要考虑抛异常的地方在于 v->data,
因此要最后delete v->data;
*/
template<typename V, typename E>
void ListGraph<V, E>::RemoveVertex(){
	//若图中没有vertex，直接返回
	if(m_list.Length() > 0){
		int loc = m_list.Length() - 1;
		Vertex* v = m_list.Get(loc);
		//删除Vertex节点，其内部邻接链表会自动调用析构函数而释放
		//链表的remove函数自带检测该索引是不是在链表中，借助remove的返回值判断链表中是否有vertex
		if(m_list.Remove(loc)){
			for(int i = (m_list.Move(0), 0); !m_list.End(); i++, m_list.Next()){
				int index = m_list.Current()->edge.Find(Edge<E>(i, loc));

				if(index >= 0){
					m_list.Current()->edge.Remove(index);
				}
			}
			delete v->data;
			delete v;
		}
		else{
			THROW_EXCEPTION(InvalidOperationException, "No vertex in current graph...");
		}
	}
}

template<typename V, typename E>
bool ListGraph<V, E>::IsAdjacent(int i, int j){
	return CHECKBOUND(i) && \
	CHECKBOUND(j) && \
	(m_list.Get(i)->edge.Find(Edge<E>(i, j)) >= 0);
}

/*将vertex[i]的邻接矩阵全部赋值给动态数组*/
template<typename V, typename E>
SharedPointer<Array<int>> ListGraph<V, E>::GetAdjacent(int i){
	DynamicArray<int>* rst = NULL;

	if(CHECKBOUND(i)){

		Vertex* v = m_list.Get(i);
		rst = new DynamicArray<int>(v->edge.Length());

		if(rst){
			//循环将邻接链表中邻接顶点放到数组中
			for(int k = (v->edge.Move(0), 0); !v->edge.End(); v->edge.Next(), k++){
				rst->Set(k, v->edge.Current().e);
			}
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create ret object...");
		}
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "Index i is invalid...");
	}	
	return rst;
}

template<typename V, typename E>
E ListGraph<V, E>::GetEdge(int i, int j){
	E rst;

	if(!GetEdge(i, j, rst)){
		THROW_EXCEPTION(InvalidParameterException, "Edge<i,j> is invalid...");
	}

	return rst;
}
/*在vertex[i]的邻接链表中加入tail为j，权值为value的边*/
template<typename V, typename E>
bool ListGraph<V, E>::GetEdge(int i, int j, E& value){
	bool rst = CHECKBOUND(i) && CHECKBOUND(j);
	if(rst){
		int loc = m_list.Get(i)->edge.Find(Edge<E>(i, j));
		if(loc >= 0){
			value = m_list.Get(i)->edge.Get(loc).data;
		}
		else{
			THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge...");
		}
	}
	return rst;
}

/*
在vertex[i]的邻接链表中加入tail为j，权值为value的边
*/
template<typename V, typename E>
bool ListGraph<V, E>:: SetEdge(int i, int j, const E& value){
	bool rst = CHECKBOUND(i) && CHECKBOUND(j);

	if(rst){
	//邻接链表保存了一个边类型，而不是指针，所以Edge的异常安全性由链表来实现
		Vertex* v = m_list.Get(i);
		int loc = v->edge.Find(Edge<E>(i, j));

		if(loc >= 0){
			rst = v->edge.Set(loc, Edge<E>(i, j, value));
		}
		else{
			//与vertex不同，edge没有编号，因此在最前边插入，效率高
			rst = v->edge.Insert(0, Edge<E>(i, j, value));
		}
	}
	return rst;
}

/*
在vertex[i]的邻接链表中查找tail为j的边
*/
template<typename V, typename E>
bool ListGraph<V, E>:: RemoveEdge(int i, int j){
	bool rst = CHECKBOUND(i) && CHECKBOUND(j);

	if(rst){

		Vertex* v = m_list.Get(i);
		int loc = v->edge.Find(Edge<E>(i, j));
		/*
		若存在，则删除，若不存在，则直接返回，不做任何操作
		*/
		if(loc >= 0){
			rst = v->edge.Remove(loc);
		}
	}

	return rst;
}

/*
vertex数量 <=> 存储vertex链表的长度
*/
template<typename V, typename E>
int ListGraph<V, E>:: VCount(){
	return m_list.Length();
}

/*
边的数目 <=> 所有vertex邻接链表长度加合
*/
template<typename V, typename E>
int ListGraph<V, E>::ECount(){
	int rst = 0;

	for(m_list.Move(0); !m_list.End(); m_list.Next()){
		rst += m_list.Current()->edge.Length();
	}

	return rst;
}
/*
计算该vertex的邻接链表的长度
*/
template<typename V, typename E>
int ListGraph<V, E>:: OD(int i){
	int rst = 0;
	if(CHECKBOUND(i)){
		rst = m_list.Get(i)->edge.Length();
	}
	else{
		THROW_EXCEPTION(InvalidParameterException, "Index i is invalid...");
	}
	return rst;
}

/*
入度等价于计算
	其他vertex的邻接链表中出现第i个vertex的次数
	O(n^2)
*/
template<typename V, typename E>
int ListGraph<V, E>:: ID(int i){
	int rst = 0;
	if(CHECKBOUND(i)){
		for(m_list.Move(0);!m_list.End(); m_list.Next()){
			LinkList<Edge<E>>& edge = m_list.Current()->edge;

			for(edge.Move(0); !edge.End(); edge.Next()){
				if(edge.Current().e == i){
					rst++;
					break;
				}
			}
		}
	}
	else{
		THROW_EXCEPTION(InvalidParameterException, "Index i is invalid...");
	}
	return rst;
}


/*
vertex中有一个V*类型指针，所以执行析构函数的时候，每次都要将头节点取出，将V*类型指针释放，为了异常安全，要保证delete在状态更新之后执行
*/
template<typename V, typename E>
ListGraph<V, E>::~ListGraph(){
	while(m_list.Length() > 0){
		Vertex* v = m_list.Get(0);
		//删除掉链表中元素的空间
		m_list.Remove(0);
		//链表中无法识别所存储的element是指针还是变量，所以如果是指针，需要拿到外部进行delete
		//delete释放V*指针所指空间
		delete v->data;
		//释放vertex所占空间
		delete v;

	}
}

/*

	ListGraph<char, int> g;
	g.AddVertex('A');
	g.AddVertex('B');
	g.AddVertex('C');
	g.AddVertex('D');

	cout<<"VCOUNT = "<<g.VCount()<<endl;

	for(int i = 0; i < g.VCount(); i++){
		cout<<i<<" : "<<g.GetVertex(i)<<endl;
	}

	cout<<endl;

	// g.RemoveVertex();
	// g.RemoveVertex();
	// cout<<"VCOUNT = "<<g.VCount()<<endl;

	// for(int i = 0; i < g.VCount(); i++){
	// 	cout<<i<<" : "<<g.GetVertex(i)<<endl;
	// }

	g.SetEdge(0, 1, 5);
	g.SetEdge(0, 3, 5);
	g.SetEdge(1, 2, 8);
	g.SetEdge(2, 3, 2);
	g.SetEdge(3, 1, 9);

	cout<<"W(0, 1) = "<<g.GetEdge(0, 1)<<endl;
	cout<<"W(0, 3) = "<<g.GetEdge(0, 3)<<endl;
	cout<<"W(1, 2) = "<<g.GetEdge(1, 2)<<endl;
	cout<<"W(2, 3) = "<<g.GetEdge(2, 3)<<endl;
	cout<<"W(3, 1) = "<<g.GetEdge(3, 1)<<endl;
	
	cout<<"ECOUNT = "<<g.ECount()<<endl;
	// g.RemoveEdge(3, 1);
	// cout<<"ECOUNT = "<<g.ECount()<<endl;
	// cout<<"W(3, 1)"<<g.GetEdge(3, 1)<<endl;

	SharedPointer<Array<int> > aj = g.GetAdjacent(0);

	for(int i = 0; i < aj->Length(); i++){
		cout<<(*aj)[i]<<endl;
	}

	cout<<"ID(1) = "<<g.ID(1)<<endl; 
	cout<<"OD(1) = "<<g.OD(1)<<endl; 
	cout<<"TD(1) = "<<g.TD(1)<<endl; 

	g.RemoveVertex();

	cout<<"Ecount = "<<g.ECount()<<endl;
	cout<<"W(0, 1) = "<<g.GetEdge(0, 1)<<endl;
	cout<<"W(1, 2) = "<<g.GetEdge(1, 2)<<endl;
	// cout<<"W(0, 3) = "<<g.GetEdge(0, 3)<<endl;
	// cout<<"W(2, 3) = "<<g.GetEdge(2, 3)<<endl;
	// cout<<"W(3, 1) = "<<g.GetEdge(3, 1)<<endl;
*/

}

#endif