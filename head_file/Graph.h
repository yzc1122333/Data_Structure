#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "Object.h"
// #include "Pointer.h"
#include "LinkStack.h"
#include "Exception.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "DynamicArray.h"
#include "Sort.h"



namespace YzcLib{
/*
邻接链表的边的数据类型
*/
template<typename E>
struct Edge: public Object{
	int b;	//起始顶点
	int e;	//邻接顶点
	E data;	//权值

	Edge(int i = -1, int j = -1){
		b = i;
		e = j;
	}

	Edge(int i, int j, const E& value){
		b = i;
		e = j;
		data = value;
	}

	//比较操作符重载，== != 主要比两个端点,无需加入data的比较
	bool operator == (const Edge<E>& obj){
		return (b == obj.b) && (e == obj.e) &&\
				(data == data);
	}

	bool operator != (const Edge<E>& obj){
		return !((*this) == obj );
	}

	bool operator < (const Edge<E>& obj){
		return data < obj.data;
	}

	bool operator > (const Edge<E>& obj){
		return data > obj.data;
	}

};



/*
图：
	定义：图是由顶点集合及顶点之间的关系集合组成的一种数据结构
		Graph = (V,E)
		V = {x | x 属于某个数据对象} 是顶点的又穷非空集合

		E = {（x,y） | x,y 属于V} 是顶点之间关系的有穷集合

	无向边
		顶点和边之间没有方向
		(x, y), (y, x)意义相同，表示x和y之间有连接
	无向图
		图中任意两个顶点之间的边均是无向边

	有向边
		顶点x和y之间的边有方向，
		<x, y>, <y, x> 意义不同
		<x, y>表示从x连接到y，x为尾，y为头
		<y, x>表示从y连接到x，y为尾，x为头
	有向图
		图中任意两个顶点之间的边均是有向边
	
	邻接顶点
		无向图
			如果(x, y) 属于E，则称x和y互为邻接
		有向图
			如果<x, y> 属于E，则称顶点x邻接顶点y

	度：
		顶点v的度是和v相关联的边的数目
			入度：以v为头的边的数目
			出度：以v为尾的边的数目
	权：
		与图的边相关的数据元素叫权
		权常用来表示图中顶点之间的距离或者消耗

图是一种非线性数据结构
*/
template <typename V, typename E>
class Graph: public Object{
protected:
	template<typename T>
	DynamicArray<T>* ToArray(LinkQueue<T>& q);

	void _visit(int i, DynamicArray<bool>& visited, LinkQueue<int>& q);

	int _find(Array<int>& p, int v);
public:
	enum MaxOrMin{
		Max,
		Min
	};
	virtual V GetVertex(int i) = 0;
	virtual bool GetVertex(int i, V& value) = 0;

	virtual bool SetVertex(int i, const V& value) = 0;
	
	virtual SharedPointer<Array<int> > GetAdjacent(int i) = 0;

	virtual bool IsAdjacent(int i, int j) = 0;

	virtual E GetEdge(int i, int j) = 0;
	virtual bool GetEdge(int i, int j, E& value) = 0;

	virtual bool SetEdge(int i, int j, const E& value) = 0;

	virtual bool RemoveEdge(int i, int j) = 0;

	virtual int VCount() = 0;

	virtual int ECount() = 0;

	virtual int OD(int i) = 0;

	virtual int ID(int i) = 0;

	virtual int TD(int i);

	SharedPointer<Array<int> > BFS(int i);

	SharedPointer<Array<int> > DFS(int i);
	//重载的递归版本
	void DFS(int i, SharedPointer<Array<int> >& sa);
	//判断是否为无向图
	bool IsUndirected();
	SharedPointer<Array<Edge<E> > > Prim(const E& LIMIT, const MaxOrMin model = Min);

	SharedPointer< Array<Edge<E>>> GetUndirectedEdge();

	SharedPointer< Array<Edge<E>>> Kruskal( const MaxOrMin model = Min);

	SharedPointer< Array<int> > Dijkstra(int i, int j, const E& LIMIT);

	SharedPointer< Array<int> > Floyd(int x, int y, const E& LIMIT);
};

template <typename V, typename E>
int Graph<V, E>::TD(int i){
	return OD(i) + ID(i);
}

//两个模板不能合并template <typename V, typename E， typename T>
template <typename V, typename E>
template<typename T>
DynamicArray<T>* Graph<V, E>::ToArray(LinkQueue<T>& q){
	DynamicArray<T>* rst  = new DynamicArray<T>(q.Length());
	if(rst != NULL){
		int i = 0;
		while(q.Length() > 0){
			(*rst)[i] = q.Front();
			q.Remove();
			i++;
		}	
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "no memory to create ret obj...");
	}
	return rst;

}

/*
MatrixGraph vs LinkGraph
MatrixGraph适用于内存资源富足的场合（性能较好）
ListGraph适用于内存资源受限的场合（节省空间）

*/

/*
图的遍历
	从图的某一个顶点出发，沿着一些边访问图中的其他顶点，使得每个顶点最多被访问一次

	注意：从某个顶点出发进行遍历，不一定能访问到图中所有顶点

	图的遍历方式
		广度优先
			以树层次遍历的思想对图进行遍历
		深度优先
			以叉树先序遍历的思想对图进行遍历
*/

/*
广度优先遍历
	原料：class LinkQueue<T>
	步骤：
		1.将顶点压入队列，对顶点做标记
		2.队头顶点v弹出，将未被标记的邻接顶点压入队列
		3.判断队列是否为空

因为返回值默认带有顺序，所以用数组最好
*/
template <typename V, typename E>
SharedPointer<Array<int> > Graph<V, E>::BFS(int i){
	DynamicArray<int>* rst = NULL;

	if((i >= 0) && (i < VCount())){
		LinkQueue<int> q;
		LinkQueue<int> r;
		DynamicArray<bool> visited(VCount());
		//初始化动态数组，将默认值赋false
		for(int i = 0; i < visited.Length(); i++){
			visited[i] = false;
		}

		q.Add(i);
		visited[i] = true;
		
		while(q.Length() > 0){
			int temp = q.Front();

			SharedPointer<Array<int> > ae= GetAdjacent(temp);

			q.Remove();

			for(int i = (r.Add(temp), 0); i< ae->Length(); i++){
				if(visited[(*ae)[i]] == false){
					q.Add((*ae)[i]);
					visited[(*ae)[i]] = true;
				}

			}
		}
		rst = ToArray(r);
	}
	else{
		THROW_EXCEPTION(InvalidParameterException,  "index i is invalid...");
	}
	return rst;
}

/*
深度优先遍历
只适用于树的遍历方式
	原料：class LinkStack<T>
	步骤：
		1.将顶点压入栈列，对顶点做标记
		2.栈顶点v弹出，将未被标记的邻接顶点压入队列
		3.判断栈是否为空

对于图而言上述方法不适用，因为后边的节点可能也会连接前边的vertex，如果再插入的时候标记，这些点是插入不进去的，违背了深度遍历回退的本质。所以介绍一更加普遍的算法
	原料：LinkStack<T>
	步骤：
		将起始点压入占中
		弹出栈顶顶点v，判断是否已标记（标记：转2，未标记转3）
		标记顶点v，并将其顶点v的邻接顶点压入栈中
		判断是否为空（非空：转2，空：结束）


因为返回值默认带有顺序，所以用数组最好
*/
template <typename V, typename E>
SharedPointer<Array<int> > Graph<V, E>::DFS(int i){
	DynamicArray<int>* rst = NULL; 
	if((i >= 0) && ( i < VCount())){

		DynamicArray<bool> visited(VCount());
		LinkQueue<int> rq;
		LinkStack<int> s;

		for(int i = 0; i < visited.Length(); i++){
			visited[i] = false;
		}
		
		s.Push(i);
		
		while(s.Size() > 0){

			int temp = s.Top();
			s.Pop();

			if(visited[temp] == false){
				
				SharedPointer<Array<int>> ad = GetAdjacent(temp);

				visited[temp] = true;
				rq.Add(temp);

				for(int i = (ad->Length() - 1); i >= 0; i--){
						s.Push((*ad)[i]);
				}
			}

		}
		rst = ToArray(rq);

	}
	else{
		THROW_EXCEPTION(InvalidParameterException,  "index i is invalid...");
	}

	return rst;
}

template <typename V, typename E>
void Graph<V, E>::_visit(int i, DynamicArray<bool>& visited, LinkQueue<int>& q){
	if(visited[i] == false){
		q.Add(i);
		visited[i] = true;
		
		SharedPointer<Array<int>> sa= GetAdjacent(i);
		
		for(int k = 0; k < sa->Length(); k++){
			_visit((*sa)[k], visited, q);
		}
	}
	

}

template <typename V, typename E>
void Graph<V, E>::DFS(int i, SharedPointer<Array<int> >& sa){
	if((i >= 0) && ( i < VCount())){
		LinkQueue<int> q;

		DynamicArray<bool> visited(VCount());

		for(int i = 0; i < visited.Length(); i++){
			visited[i] = false;
		}

		_visit(i, visited, q);
		
		sa = ToArray(q);
	}
	else{
		THROW_EXCEPTION(InvalidParameterException,  "index i is invalid...");
	}
	
}


template <typename V, typename E>
bool Graph<V, E>::IsUndirected(){
	//图中一个点都没有的情况，是无向图，返回true
	bool rst = true;
	//只要又一条边不满足要求，那么就不是无向图
	for(int i= 0; rst && i < VCount(); i++){
		for(int j = 0; rst && j < VCount(); j++){
			//如果（i， j）是一条边，那么检测（j, i）是否也是一条边，并且权值相同

			if(IsAdjacent(i, j)){
				rst = rst && (IsAdjacent(j, i)) &&(GetEdge(i, j) == GetEdge(j, i));
			}
		}
	}
	return rst;
}
/*
最小生成树：
	如何再图中选择n-1条边使得n个顶点间两两可达，并且n-1条边的权值之和最小

要求：
	仅使用图中n-1条边连接图中的n个顶点
	不能使用产生回路的边
	个边上的权值综合达到最小

Prim算法：
	1.选择某一个顶点v0作为起始顶点，使得T={v0}，F={v1,v2,...vn},E = {}
	2.每次选择一条边，这条边是所有(u,v)中权值最小的一条边，且u属于T，v属于F
	3.修改T，E，F
		T= T +{v}， F= F-{v}， E= E+{(u，v)}
	4.当F != NULL时，且(u,v)存在，转2，否则结束

	如果T集合到F集合中同一顶点的连接有多条，那么选择权值最小的连接

算法原料：
	Array<bool>	标记所属结合（T or F）
	Array<E> 记录T到F集合中的最小权值
	Array<int> 记录Array<E>中权值的对应顶点
	Queue<Edge>记录最小生成树的边

注意：
	最小生成树仅对无向图有意义
	必须判断图对象是否能够看作无向图


Prim算法通过顶点的动态标记寻找最小生成树
利用Prim算法的思想关键也能寻找图的最大生成树
Prim的本质是每次从未被连入树中的点寻找到达已经被连接好的树的最小距离

prim证明：
	图G，nv个顶点，ne条边，最小生成树Tmin，prim生成树T0。
	cost(Tmin) <= cost(T0)		(最小生成树定义)

	T0 边按权重由小到大排序 ek1, ek2, ....,ekn
	Tmin 边按权重由小到大排序 eg1, eg2, ....,egn

	两棵树从小到达比较权重，设第一个属于T0，但是不属于Tmin,但属于T0的边ed1,连接该边的两个顶点为(vs,ve1)
	不属于Tmin,但属于T0的边ed1,连接该边的两个顶点为(vs,ve1)

	两条边起点相同，由Prim算法性质可知
		w(ed2) >= w(ed1)
	此时，在Tmin中删除ed2，添加ed1，边的数量和顶点数量均不变，且不存在环，因此得到新的生成树满足 cost(Tmin) >= cost(Tnew)

	相当于断掉一条权值大的边，环一条权值小的边上去

	又因为Tmin是MST，所以 cost(Tmin) = cost(Tnew)

	以此类推， cost(Tmin) = cost(T0)
	T0s是最小生成树


又两个集合
T集合就是生成树中点的集合
F集合就是没有连接到树的点的集合

*/
template <typename V, typename E>
SharedPointer<Array<Edge<E>>> Graph<V, E>::Prim(const E& LIMIT, MaxOrMin model ){
	LinkQueue<Edge<E> > rst;
	//检测是否为无向图
	if(IsUndirected()){
		//记录顶点中的所属集合（T，F）
		DynamicArray<bool> mark(VCount());
		//记录cost权值对应顶点
		DynamicArray<int> adjVex(VCount());
		//记录T集合到F集合的对应顶点，即没链接到树上的点的集合到树的距离,NULL代表空，还没有访问过的点
		DynamicArray<E> cost(VCount());

		for(int i = 0; i < VCount(); i++){
			mark[i] = false;
			adjVex[i] = -1;
			cost[i] = LIMIT;
		}
		//初始化T集合，放入一个点到T集合，默认是索引为0个点。
		int initv = 0;
		mark[initv] = true;
		SharedPointer<Array<int>> aj = GetAdjacent(initv);
		//初始化adjVex和cost矩阵，即将和索引为0相连的点，更新到表中
		for(int i = 0; i < aj->Length(); i++){
			cost[(*aj)[i]] = GetEdge(initv, (*aj)[i]);
			adjVex[(*aj)[i]] = initv;
		}

		//从F集合将点逐个移动到T集合
		bool end = false;
		for(int i = 0; !end && i < VCount(); i++){
			//寻找不在树中，且离树最近的点,标记为k,若k = -1，则证明没有相连的点，无法构成树
			int k = -1;
			E temp = LIMIT;
			for(int j = 0; j < VCount();j++ ){
				if(!mark[j] && (model ? (cost[j] < temp) : (cost[j] > temp))){
					temp = cost[j];
					k = j;
				}
			
			}
			//若没有找到点，证明查找完毕
			end = (k == -1);
			if(!end){
				//将k点从F集合移到T集合
				mark[k] = true;
				//将(adjVex[i], k),权值为*cost[k]的边加入结果队列
				rst.Add(Edge<E>(adjVex[k], k, cost[k]));
				//更新各个点到树的距离，adjVex[]，cost[]
				aj = GetAdjacent(k);
				
				for(int j = 0; j < aj->Length(); j++){
					E value = GetEdge(k,(*aj)[j]);
					if(!mark[(*aj)[j]] && (model ? (value < cost[(*aj)[j]]) : (value > cost[(*aj)[j]]))){
						cost[(*aj)[j]] = value;
						adjVex[(*aj)[j]] = k;
					}
				}
			}
		}

	}
	//无向图抛异常
	else{
		THROW_EXCEPTION(InvalidOperationException, "Prim operation is for undirected graph only ...");
	}

	//一棵完整的生成树，#edge = #vertex - 1，如果队列长度不是 #vertex - 1，那么表示无法生成生成树
	if(rst.Length() != (VCount() - 1)){
		THROW_EXCEPTION(InvalidOperationException, "No enough edge for prim operation...");
	}

	return ToArray(rst);
}


/*

最小生成树Kruskal
	最小生成树特征：
		选取的边是图中权值较小的边
		所有边连接后不够成回路
	Kruskal算法主要是选取n-1条边，使得权值最小

	Kruskal算法核心步骤：
		1. 定义前驱标记数组，Array<int> p(vCount())
		2. 获取当前图的所有节点，并存储于edge数组中
		3. 对于数组edges按照权值进行排序
		4. 利用p数组在edges数组中选择前n-1个不构成回路的边	

	技巧：前驱标记数组：
		定义数组： Array<int>p(vCount());
		数组元素意义：
			p[n]表示顶点n在边的连接通路商的另一个端点
		前驱数组，记录一个node的前驱，直至前驱数组的两端（-1 作为标记）表示两端是否联通

	
	prim算法以顶点为核心寻找最小生成树
	kruskal算法以边为核心寻找最小生成树
	Kruskal算法中的关键是前驱数组的使用
	前驱数组用于判断新选择的边是否会造成回路
	如果从前驱数组能够查询到同样的端点，那么就意味着有回路
	前驱数组
	Kruskal算法本质就是寻找最小的前N-1条不构成回路的边，作为最小生成树
*/

/*
仅适用于无向图，因为无向图可以看作是对称连接的有向图，所以只需考虑矩阵的某一半即可
*/
template <typename V, typename E>
SharedPointer< Array<Edge<E>>> Graph<V, E>::GetUndirectedEdge(){
	if(!IsUndirected()){
		THROW_EXCEPTION(InvalidOperationException, "This function is for undirected graph only...");
	}
	LinkQueue<Edge<E>> rst;
	for(int i = 0; i < VCount(); i++){
		for(int j = i; j < VCount(); j++){
			if(IsAdjacent(i, j)){
				rst.Add(Edge<E>(i, j, GetEdge(i, j)));
			}
		}
	}
	return ToArray(rst);
}

//通过前驱数组寻找连通的各个vertex的顶点
template <typename V, typename E>
int Graph<V, E>::_find(Array<int>& p, int v){
	//-1代表到达顶点
	while(p[v] != -1){
		v = p[v];
	}
	return v;
}


template <typename V, typename E>
SharedPointer< Array<Edge<E>>> Graph<V, E>::Kruskal(const MaxOrMin model){
	// 得到无向图所有的边， 在GetUndirectedEdge()函数中检测是否为无向图，若不是抛异常
	SharedPointer< Array<Edge<E>>> edge = GetUndirectedEdge();
	LinkQueue<Edge<E>> rst;
	//前驱标记数组
	DynamicArray<int> p(VCount());
	for(int i = 0; i < p.Length(); i++){
		p[i] = -1;
	}

	//使用数组类排序函数
	Sort::Shell_Sort(*edge, model);
	for(int i = 0; i < edge->Length() && rst.Length() < VCount() ; i++){
		//通过前驱数组寻找顶点
		int h = _find(p, (*edge)[i].b);
		int t = _find(p, (*edge)[i].e);
		//查看端点是否相同，不相同则构成环
		if(h != t){
			rst.Add((*edge)[i]);
			//更新前驱标记数组,记录t点的前驱为h
			p[t] = h;
		}
	}

	if(rst.Length() !=  VCount() - 1){
		THROW_EXCEPTION(InvalidOperationException, "No enough edge for prim operation...");
	}

	return ToArray(rst);
}


/*
寻最短路：
	给定一个带有权的有向图G与起始顶点V， 求从V到G中其他顶点的最短路径（每条边上都有存在意义的权值）
最短路Dijkstra算法
	按照路径长度的递增次序，逐步产生最短路。
	首先求出长度最短的一条最短路， 再参照它求出长度次短的一条最短路径，以此类推，直到从起始顶点v到其他各顶点最短路全部求出为止

准备工作：
	辅助数组：Array<E> dist;
		dist[i]表示当前从起始顶点v0到顶点vi的路径长度
	初始设置：
		若从起始顶点v0到vi有边，dist[i]为该边的权值
		若从起始顶点v0到vi无边， dist[i] = 无穷


Dijkstra算法步骤
	1.初始化： S<-{v0}
			  dist[j]<-Edge[0][j], j = 1,2,...,n-1
	2.找出最小路径值所对应的顶点K
		dist[k] = min{dist[i]} i属于 V(点集)/S
		S<-S并{K} //标记k顶点进入S集合
	3.对于每一个i属于V/S修改（寻找新的最短路径）：
		dist[i]<-min{dist[i], dist[k] + Edge[k][i]}
	4.若 S== V，算法结束，否则转2

Dijkstra算法精髓：
	S集合内的顶点是已经找到最短路的顶点
	v0到w的最短路只能通过S集合内的顶点
	dist[w]可能改变
		if(dist[u] + Edge[u, w] < dist[w]){
			dist[w] = dist[u] + Edge[u, w];
		}

记录最短路径上的各个顶点
	定义辅助数组Array<int> path;
	path[i]表示当前路径上顶点i的前驱顶点
	初始化：path = {-1}
	修改
	if(dist[u] + Edge[u, w] < dist[w]){
		dist[w] = dist[u] + Edge[u, w];
		path[w] = u;
	}

Dijkstra寻求最短路径是基于递推的思想完成的
起始顶点到其他各个顶点的最短路径通过动态推导得到
未标记顶点的最短路径只能由已标记顶点计算得到
算法的最终结果是起始顶点到其他个各顶点的最短路径
*/

/*
LIMIT是
	求最短路径：极大值
	求最长路径：极小值
按顺序返回vertex
*/
template<typename V, typename E>
SharedPointer< Array<int> > Graph<V, E>::Dijkstra(int i, int j, const E& LIMIT){
	LinkQueue<int> rst;
	LinkStack<int>	s;

	//检测端点合法性
	if((i >= 0) && (i < VCount()) &&  (j >= 0) && ( j < VCount())){
		DynamicArray<int> path(VCount());
		DynamicArray<E> dist(VCount());
		//标记是否已进入集合S
		DynamicArray<bool> mark(VCount());

		//初始化数组：
		for(int k = 0; k < VCount(); k++){
			
			path[k] = -1;
			//将和i点相连的边提取出，更新前驱数组
			dist[k] = IsAdjacent(i, k)? (path[k] = i, GetEdge(i, k)) :  LIMIT;
			mark[k] = false;
		}

		//初始化S集合
		//将i点放入集合中
		mark[i] = true;

		//循环迭代，寻找i到k点的最小路径
		for(int k = 0; k < VCount(); k++){
			//通过前驱数组寻找到i到h的最小路径
			int minp = -1;
			E te = LIMIT;
			for(int h = 0; h < VCount(); h++){
				if(!mark[h] && dist[h] < te){
					te = dist[h];
					minp = h;
				}
			}

			if(minp == -1) break;

			//将新的节点插入S集合中
			mark[minp] = true;

			//找到下一个点的最短路，更新加入新点的前驱数组
			for(int h = 0; h < VCount(); h++){
				if(!mark[h] && IsAdjacent(minp, h)){
					E e = GetEdge(minp, h);
					if(dist[minp] + e < dist[h]){
						dist[h] = dist[minp] + e;
						path[h] = minp;
					}
				}
			}
		}

		s.Push(j);
		for(int k = path[j]; k != -1;k = path[k] ){
			s.Push(k);
		}

		while(s.Size() > 0){
			rst.Add(s.Top());
			s.Pop();
		}

	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "Index<i,j> is invalid...");
	}

	if(rst.Length() < 2){
		THROW_EXCEPTION(ArithmeticException, "Index<i,j> is invalid...");
	}

	return ToArray(rst);
}


/*
Dijkstra执行一次，可以算出从起点到其他顶点的最短路径
对于Dijkstra算法，若想算任意两点之间的最短路径
需要对图中每个点都执行一次Dijkstra算法，总共要执行N次

对求任意两点最短距离的问题，Floyd算法也可以求解，并且相对简单

Floyd算法
	定义一个n阶方阵序列
		A(-1), A(0), ..., A(n-1)
	其中：
		A(-1)[i][j] = Edge[i][j]
		A(k)[i][j] = min{A(k-1)[i][j], A(k-1)[i][k] + A(k-1)[k][j]}

	n阶方阵的意义：
	A(-1)[i][j]:vi 到vj的权值，没有中间点，本质就是邻接矩阵
	A(0)[i][j]: vi到vj的路径长度，路径的中间顶点为0
	A(1)[i][j]:	vi到vj的路径长度，路径的中间顶点可能为0或1

	......

	A(k)[i][j]:vi到vj的路径长度，路径的中间顶点编号不大于k

	......

	A(n-1)[i][j]: vi到vj的最短路径长度

	Floyed算法精髓

	A(-1)定义为邻接矩阵，则：
		A(0), ...,A(n-1)通过中转顶点逐一递推得到
	A(k)矩阵中元素的更新：
		A(k)[i][j] = min{A(k - 1)[i][j], A(k-1)[i][k] + A(k-1)[k][j]}
	A矩阵的推导就是最短路径的推导
		A[i][j]为i到j的路径值，在推导过程中逐渐减小
	

	Floyd算法的实现
		初始化
			本质：使用邻接矩阵初始化A(-1)
		矩阵推导：
			本质：使用中转顶点逐步推导最短路径


	定义赋值矩阵：
		int path[N][N];
		path[i][j]表示i到j的路径上所经过的第一个顶点
		初始化path[i][j] = -1 or path[i][j]=j;
		修改：
			if((dist[i][k] + dist[k][j]) < dist[i][j]){
				dist[i][j] = dist[i][k] + dist[i][k];
				path[i][j] = path[i][k]
			}

Floyd算法通过递推过程逐步球的所有顶点间的最短路径
Floyd算法本质是通过中转顶点寻找更短路径
邻接矩阵是最短路径推导的起始矩阵
邻接矩阵记录了最短路径上的各个顶点

*/
/*
返回值为i到j的最短path， LIMIT是E的最大值（正无穷）
*/
template<typename V, typename E>
SharedPointer< Array<int> > Graph<V, E>::Floyd(int x, int y, const E& LIMIT){
	LinkQueue<int> rst;

	//检测i，j的合法性
	if((x >= 0)&& (x < VCount()) && (y >= 0) && (y < VCount())){
		//定义初始化A(0)矩阵，即邻接矩阵，最短距离矩阵
		DynamicArray<DynamicArray<E>> dist(VCount());
		DynamicArray<DynamicArray<E>> path(VCount());

		//DynamicArray默认初始化长度为0,对dist和path进行resize操作
		for(int k = 0; k < VCount(); k++){
			dist[k].resize(VCount());
			path[k].resize(VCount());
		}

		//path初始化为-1，距离初始化为邻接矩阵
		for(int i= 0; i < VCount(); i++){
			for(int j = 0; j < VCount(); j++){
				path[i][j] = -1;
				dist[i][j] = IsAdjacent(i, j)? (path[i][j] = j, GetEdge(i, j)): LIMIT;
			}
		}
		

		for(int k = 0; k < VCount(); k++){
			for(int i = 0; i < VCount(); i++){
				for(int j = 0; j < VCount(); j++){
					if((dist[i][j] != LIMIT && dist[i][k] + dist[k][j] < dist[i][j]) ||\
					 (dist[i][j] == LIMIT && dist[i][k] < LIMIT && dist[k][j] < LIMIT)){
						dist[i][j] = dist[i][k] + dist[k][j];
						path[i][j] = path[i][k];
					}
				}
			}
		}
		
		//将最小path放到队列中,退出条件，没有路继续走，或者到达终点
		while(x != -1 && x != y){
			//将x更新为x到y的路径上，第一个到达的点，不断迭代，找到x

			rst.Add(x);
			x = path[x][y];
		}

		//如果到达终点，将终点加入到队列中
		if(x == y){
			rst.Add(x);
		}
		// for(int i = 0; i < VCount(); i++){
		// 	for(int j = 0; j < VCount(); j++){
		// 		cout<<dist[i][j]<<" ";
		// 	}
		// 	cout<<endl;
		// }

	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "Index<i,j> is invalid...");
	}
cout<<rst.Length()<<endl;
	//长度小于2，不构成边，即没有最小路径
	if(rst.Length() < 2){
		THROW_EXCEPTION(ArithmeticException, "Index<i,j> is invalid...");
	}

	return ToArray(rst);
}

/*
template< typename V, typename E >
Graph<V, E>& GraphEasy()
{
    static MatrixGraph<4, V, E> g;

    g.SetEdge(0, 1, 1);
    g.SetEdge(1, 0, 1);

    g.SetEdge(0, 2, 3);
    g.SetEdge(2, 0, 3);

    g.SetEdge(1, 2, 1);
    g.SetEdge(2, 1, 1);

    g.SetEdge(1, 3, 4);
    g.SetEdge(3, 1, 4);

    g.SetEdge(2, 3, 1);
    g.SetEdge(3, 2, 1);

    return g;
}


template< typename V, typename E >
Graph<V, E>& GraphMiddle()
{
    static ListGraph<V, E> g(9);

    g.SetEdge(0, 1, 10);

    g.SetEdge(1, 2, 50);

    g.SetEdge(0, 4, 100);

    g.SetEdge(0, 3, 30);

    g.SetEdge(2, 4, 10);

    g.SetEdge(3, 2, 20);
	g.SetEdge(3, 4, 60);
    return g;
}

template< typename V, typename E >
Graph<V, E>& GraphComplex()
{
    static ListGraph<V, E> g(9);

    g.SetEdge(0, 1, 10);
    g.SetEdge(1, 0, 10);

    g.SetEdge(0, 5, 11);
    g.SetEdge(5, 0, 11);

    g.SetEdge(1, 2, 18);
    g.SetEdge(2, 1, 18);

    g.SetEdge(1, 8, 12);
    g.SetEdge(8, 1, 12);

    g.SetEdge(1, 6, 16);
    g.SetEdge(6, 1, 16);

    g.SetEdge(2, 3, 22);
    g.SetEdge(3, 2, 22);

    g.SetEdge(2, 8, 8);
    g.SetEdge(8, 2, 8);

    g.SetEdge(3, 8, 21);
    g.SetEdge(8, 3, 21);

    g.SetEdge(3, 6, 24);
    g.SetEdge(6, 3, 24);

    g.SetEdge(3, 7, 16);
    g.SetEdge(7, 3, 16);

    g.SetEdge(3, 4, 20);
    g.SetEdge(4, 3, 20);

    g.SetEdge(4, 5, 26);
    g.SetEdge(5, 4, 26);

    g.SetEdge(4, 7, 7);
    g.SetEdge(7, 4, 7);

    g.SetEdge(5, 6, 17);
    g.SetEdge(6, 5, 17);

    g.SetEdge(6, 7, 19);
    g.SetEdge(7, 6, 19);

    return g;
}



int main() {
	// Graph<int, int>& g = GraphEasy<int, int>();
	Graph<int, int>& g = GraphMiddle<int, int>();
    // SharedPointer< Array< Edge<int> > > sa =g.Kruskal(Graph<int, int>::Max);
	SharedPointer< Array<int> > sa = g.Floyd(0, 4, 65535);

	// SharedPointer< Array<int> > sa = g.Dijkstra(0, 4, 65535);
    int w = 0;

    // for(int i=0; i<sa->Length(); i++)
    // {
    //     w += (*sa)[i].data;

    //     cout << (*sa)[i].b << " " << (*sa)[i].e << " " << (*sa)[i].data << endl;
    // }
    for(int i=0; i<sa->Length(); i++)
    {
        cout << (*sa)[i]<< endl;
    }
    cout << "Weight: " << w << endl;

    // Graph<int, int>& g = GraphComplex<int, int>();

    // SharedPointer< Array< Edge<int> > > sa = g.Kruskal(Graph<int, int>::Max);

    // int w = 0;

    // for(int i=0; i<sa->Length(); i++)
    // {
    //     w += (*sa)[i].data;

    //     cout << (*sa)[i].b << " " << (*sa)[i].e << " " << (*sa)[i].data << endl;
    // }

    // cout << "Weight: " << w << endl;


	return 0;
*/

}

#endif