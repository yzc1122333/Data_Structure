#ifndef __MATRIXGRAPH_H__
#define __MATRIXGRAPH_H__

#include "Graph.h"
#include "Exception.h"
#include "DynamicArray.h"

/*
基本思想：
	用一维数组存储顶点：描述顶点相关的数据
	用二维数组存储边：描述顶点间的关系和权

邻接矩阵法：
	设图A = (V, E)是一个有n各顶点的图， 图的邻接矩阵为Edge[n][n],则：
						w, w权值，i和j连接
		Edge[i][j] = 
						空， i==j， 或i和j不连接
	注：解决工程问题时，习惯于对图中每个节点进行编号，当不需要全职时， 取w非空表示节点间有连接

	性质：
		无向图邻接矩阵是对称的
		有向图邻接矩阵可能不对称

	直接用数组表示顶点集和边集
		V m_vertexes[N]
		E m——edges[N][N]
		int m_ecount;
	问题：
		构造效率低下：
			图对象初始化时， 频繁调用顶点类型和边类型的构造函数
		空间使用率低下：
			图对象占大量空间，而大多数空间处于闲置状态
		无法表示空值
			无法用同一的方式表示边为空的情形

	使用指针数组表示顶点集和边集
	问题解决：
		构造效率：
			初始化图像时，只需要将数组原属赋值为空，无需调用构造函数
		空间使用率：
			顶点数据原属和边数据元素在需要的时候动态创建
		空值的表示
			任意的顶点类型和边类型都使用NULL表示空值

	使用指针数组进行数据存储->提高效率


注：图的节点可以为空，为空的节点也可以连接node
*/			

namespace YzcLib{

template<int N, typename V, typename E>
class MatrixGraph: public Graph<V, E>{
protected:
	V* m_vertexes[N];
	E* m_edges[N][N];
	int m_ecount;

	#define CHECKBOUND(i) ((i >= 0) && (i < VCount()))

public:
	MatrixGraph();

	V GetVertex(int i);

	bool GetVertex(int i, V& value);

	bool SetVertex(int i, const V& value);
	
	SharedPointer<Array<int>> GetAdjacent(int i);

	E GetEdge(int i, int j);
	bool GetEdge(int i, int j, E& value);

	bool SetEdge(int i, int j, const E& value);

	bool RemoveEdge(int i, int j);

	bool IsAdjacent(int i, int j);

	int VCount();

	int ECount();

	int OD(int i);

	int ID(int i);

	~MatrixGraph();
};
/*
初始化，顶点集边集都置空，边的数量为0
*/
template<int N, typename V, typename E>
MatrixGraph<N, V, E>::MatrixGraph(){
	for(int i = 0; i < VCount(); i++){
		m_vertexes[i] = NULL;
		for(int j = 0; j < VCount(); j++){
			m_edges[i][j] = NULL;
		}
	}
	m_ecount = 0;
}	

template<int N, typename V, typename E>
V MatrixGraph<N, V, E>::GetVertex(int i){
	V rst;
	//只有一个返回值，若是编号不在范围内只能抛异常
	if(!GetVertex(i, rst)){
		THROW_EXCEPTION(InvalidParameterException , "Paranmeter  i is not of range");
	}
	return rst;
}

/*
为i号位置的node赋值为value
*/
template<int N, typename V, typename E>
bool MatrixGraph<N, V, E>::GetVertex(int i, V& value){
	bool rst = CHECKBOUND(i);
	//若编号不在范围内，返回false
	if(rst){
		//检测node[i]是否有值，没有赋值则抛异常
		if(m_vertexes[i] != NULL){
			value = *m_vertexes[i];
		}
		else{
			THROW_EXCEPTION(InvalidParameterException , "No value assigned to this vertex...");
		}
	}

	return rst;
}

/*
如果node是第一次赋值（指针为NULL），node需要申请内存空间
如果之前node已经有值，则无需申请，直接使用上一次的内存空间，在内存上做修改即可
*/
template<int N, typename V, typename E>
bool MatrixGraph<N, V, E>::SetVertex(int i, const V& value){
	bool rst = CHECKBOUND(i);
	//out of range ->return false
	//需要注意，V类类型赋值的时候可能抛异常，注意异常安全，先用一个临时变量进行申请内存和赋值，若赋值没有抛出异常，将临时变量的值在赋值到该节点上
	if(rst){	
		V* temp = m_vertexes[i];
		if(temp == NULL){
			temp = new V;
		}
		//内存申请是否成功，失败抛异常
		if(temp != NULL){
			*temp = value;
			m_vertexes[i] = temp;
		}
		else{
			THROW_EXCEPTION(InvalidParameterException, "No memory to  store new vertex value...");
             }
		}
	return rst;
}

template<int N, typename V, typename E>
SharedPointer<Array<int>> MatrixGraph<N, V, E>::GetAdjacent(int i){
	DynamicArray<int>* rst = NULL;
	if(CHECKBOUND(i)){
		//先循环一遍确定数组大小，然后再循环一遍对数组赋值
		//如果不先确定数组大小，每次都要重新申请内存空间，都要将内存空间的东西进行拷贝，效率很低
		int len = 0;
		for(int j = 0; j < VCount(); j++){
			(m_edges[i][j] != NULL) && (len++);
		}

		rst = new DynamicArray<int>(len);

		if(rst != NULL){
			for(int j = 0, k = 0; j < VCount(); j++){
			//将与之相连的顶点j写入数组
				(m_edges[i][j] != NULL) && (rst->Set(k++, j));
			}
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException, "No memory to create new ret object...");
		}
	}
	else{
		THROW_EXCEPTION(InvalidParameterException , "Paranmeter  i is not of range");
	}
	return rst;
}

template<int N, typename V, typename E>
E MatrixGraph<N, V, E>::GetEdge(int i, int j){
	E rst;
	if(!GetEdge(i, j, rst)){
		THROW_EXCEPTION(InvalidParameterException, "Index <i, j> is invalid...");
	}
	return rst;
}

template<int N, typename V, typename E>
bool MatrixGraph<N, V, E>::GetEdge(int i, int j, E& value){
	bool rst = CHECKBOUND(i) && CHECKBOUND(j);
	if(rst){
		if(m_edges[i][j] != NULL){
			value = *(m_edges[i][j]);
		}
		else{
			THROW_EXCEPTION(InvalidParameterException , " No value assigned to this edge...");
		}
	}
	return rst;
}


template<int N, typename V, typename E>
bool MatrixGraph<N, V, E>:: SetEdge(int i, int j, const E& value){
	bool rst = CHECKBOUND(i) && CHECKBOUND(j);
	if(rst){
		E* temp = m_edges[i][j];
	
		if(temp == NULL){
			temp = new E;
		}
		//申请成功，或者之前已经成功申请过内存
		if(temp != NULL){
			//若赋值不会出现异常安全问题，更新状态
			*temp = value;
			(m_edges[i][j] == NULL) && (m_ecount++);
			m_edges[i][j] = temp;
		}
		else{
			THROW_EXCEPTION(InvalidParameterException, "No memory to store new edge value...");
		}
	}	
	return rst;
}

template<int N, typename V, typename E>
bool MatrixGraph<N, V, E>:: RemoveEdge(int i, int j){
	bool rst = CHECKBOUND(i) && CHECKBOUND(j);
	/*delete可能抛异常，如果直接使用m_edge[i][j]的话
		会先delete，然后将指针置为NULL，delete抛异常后，指针无法置空，导致异常不安全。

		解决方法，将指针的值设置给一个临时变量，就可以先将指针置空再delete
	*/
	if(rst){
		E* toDel = m_edges[i][j];
		//最后delete temp，就可以将m_edges[i][j]赋值为NULL
		m_edges[i][j] = NULL;
		if(toDel != NULL){

			m_ecount--;
			delete toDel;
		}

	}

	return rst;
}

/*
有向图 i邻接j，<i, j> i是头，j是尾
*/
template<int N, typename V, typename E>
bool MatrixGraph<N, V, E>::IsAdjacent(int i, int j){
	return CHECKBOUND(i) && CHECKBOUND(j) && m_edges[i][j] != NULL; 
}

template<int N, typename V, typename E>
int MatrixGraph<N, V, E>:: VCount(){
	return N;
}

template<int N, typename V, typename E>
int MatrixGraph<N, V, E>:: ECount(){
	return m_ecount;
}

/*
每一行代表该点的出度
计算出度 <=>将第i行所有不是 NULL 的个数
*/
template<int N, typename V, typename E>
int MatrixGraph<N, V, E>:: OD(int i){
	int rst = 0;
	if(!CHECKBOUND(i)){
		THROW_EXCEPTION(InvalidParameterException , "Paranmeter  i is not of range");
	}
	for(int j = 0; j < VCount(); j++){
		if(m_edges[i][j] != NULL){
			rst++;
		}
	}
	return rst;
}

/*
每一列代表该点的入度
计算出度 <=>将第i列所有不是 NULL 的个数
*/
template<int N, typename V, typename E>
int MatrixGraph<N, V, E>:: ID(int i){
	int rst = 0;
	if(!CHECKBOUND(i)){
		THROW_EXCEPTION(InvalidParameterException , "Paranmeter  i is not of range");
	}
	for(int j = 0; j < VCount(); j++){
		if(m_edges[j][i] != NULL){
			rst++;
		}
	}
	return rst;
}


/*
将指针所指空间全部释放，因为delete NULL编译器不会做任何操作，所以无需担心点和边为NULL的情况

*/
template<int N, typename V, typename E>
MatrixGraph<N, V, E>::~MatrixGraph(){
	for(int i = 0; i < VCount(); i++){
		delete m_vertexes[i];
		for(int j = 0; j < VCount(); j++){
			delete m_edges[i][j];
		}
	}
	m_ecount = 0;
}

/*
	MatrixGraph<3, int, int> g;
	g.SetEdge(0, 1, 1);
	g.SetEdge(1, 0, 2);
	g.SetEdge(1, 2, 3);

	cout<<"VCount = "<<g.VCount()<<endl;
	cout<<"eCount = "<<g.ECount()<<endl;
	cout<<"ID(1) = "<<g.ID(1)<<endl;
	cout<<"OD(1) = "<<g.OD(1)<<endl;
	cout<<"TD(1) = "<<g.TD(1)<<endl;

	cout<<"W(0, 1) = "<<g.GetEdge(0, 1)<<endl;
	cout<<"W(1, 0) = "<<g.GetEdge(1, 0)<<endl;	
	cout<<"W(1, 2) = "<<g.GetEdge(1, 2)<<endl;

	SharedPointer<Array<int> > aj = g.GetAdjacent(1);

	for(int i = 0; i < aj->Length(); i++){
		cout<<(*aj)[i]<<endl;
	}

	cout<<endl;

	cout<<"Delete Edge: "<<endl;

	g.RemoveEdge(0, 1);

	cout<<"ECOUNT = "<<g.ECount()<<endl;

	g.SetVertex(0, 100);

	cout<< "V(0)" << g.GetVertex(0)<<endl;
	cout<<"W(0, 1)"<<g.GetEdge(0, 1)<<endl;
*/

}

#endif