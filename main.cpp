#include "./head_file/Data_structure.h"
#include <cstring>
using namespace YzcLib;

/*
最长不下降序列
由n个数组组成的数列，记为a[1], a[2], ...,a[n],若存在i1 < i2 < ...<im,st.a[i1] <= a[i2] <= a[i3] <= ...<= a[im],则称为长度为m的不下降序列

问题建模
	使用数列中的元素和元素之间的关系建立图模型
		图中顶点附加值为对应的数列元素值
		图中的边按照如下方式建立
			当数列中某个元素与后继元素存在不下降关系时：
				从该元素的顶点到后继元素对应的顶点存在一条有向边
				边的权值固定为1
	
	最长不下降问题==>求图中的最多顶点路径（路径上顶点数目最多）


	解决思路
		以每个顶点为起始顶点寻找局部最多顶点路径
		寻找全局最多顶点的路径

	局部最多顶点路径寻找思路
		获取当前顶点v的邻接顶点
		以各个邻接顶点为起始顶点求解最多顶点路径
		将找到的最大路径+1，作为该点最大路径

	寻找局部顶点数最多的路径
		search_max_path(v, count, path, mark)
			因为要使用递归的方式进行，所以每一个参数都要传递
			以v作为起始顶点寻找最多顶点路径
			count记录经过的最多顶点数
			path记录最多顶点路径上经过的第一个点
			mark记录最多顶点路径是否找到


注：有可能顶点v的最大路径有多条，所以，为了全部找到，path用链表来记录
*/
int Search_Max_Path(Graph<int, int> &g, int v, Array<bool> &mark, Array<int> &count, Array<LinkList<int> > &path){
	int rst = 0;

	SharedPointer<Array<int> > aj= g.GetAdjacent(v);

	for(int i = 0; i < aj->Length(); i++){
		int count_path = 0;

		if(!mark[(*aj)[i]]){
			count_path = Search_Max_Path(g, (*aj)[i], mark, count, path);
		}
		else{
			count_path = count[(*aj)[i]];
		}

		if(count_path > rst) rst = count_path;
	}

	

	for(int i = 0; i < aj->Length(); i++){
		if(count[(*aj)[i]] == rst){
			path[v].Insert((*aj)[i]);
		}
	}

	rst++;
	count[v] = rst;
	mark[v] = true;

	return rst;
}


int Search_Max_Path(Graph<int, int>& g, Array<bool>& mark, Array<int>& count, Array<LinkList<int> >& path){
	int rst = 0;

	for(int i = 0; i < g.VCount(); i++){
		if(!mark[i]){
			int max_path_v = Search_Max_Path(g, i, mark, count, path);
			if(rst < max_path_v) rst = max_path_v;
		}
		
		
	}

	return rst;
}

void Init_Array(Array<int>& count,Array<bool>& mark){
	for(int i = 0; i < count.Length(); i++){
		count[i] = 0;
	}

	for(int i = 0; i < mark.Length(); i++){
		mark[i] = false;
	}
}


SharedPointer<Graph<int, int> > CreateGraph(int* a, int len){
	Graph<int, int>* rst = new ListGraph<int, int>(len);

	for(int i = 0; i < len; i++){
		rst->SetVertex(i, a[i]);
	}

	for(int i = 0; i < len; i++){
		for(int j = i + 1; j < len; j++){
			if(a[i] <= a[j]){
				rst->SetEdge(i, j, 1);
			}
		}
	}

	return rst;
}

void Print_Path(int* a, int v, Array<LinkList<int> >& path, LinkList<int>& p){
	p.Insert(a[v]);

	if(path[v].Length() > 0){
		for(path[v].Move(0); !path[v].End(); path[v].Next()){
			Print_Path(a, path[v].Current(), path, p);
		}
	}
	else{
		cout<<"Element: ";
		for(p.Move(0); !p.End(); p.Next()){
			cout<<p.Current()<<" ";
		}
		cout<<endl;
	}



	p.Remove(p.Length() - 1);
}

void Print_Max_Path(int* a, int max_path, Array<int>& count, Array<LinkList<int> >& path){
	LinkList<int> l;
	for(int i =0; i < count.Length(); i++){
		if(max_path == count[i]){
			Print_Path(a, i, path, l);
		}
	}
	
}

/*
长度为len的int型数组
*/


void Solution(int* a, int len){

	int max_path = 0;

	DynamicArray<bool> mark(len);
	DynamicArray<int> count(len);
	DynamicArray<LinkList<int> > path(len);

	SharedPointer<Graph<int, int> > g = CreateGraph(a, len);

	Init_Array(count, mark);

	max_path = Search_Max_Path(*g, mark, count, path);

	cout<<"Len"<<max_path<<endl;

	Print_Max_Path(a, max_path, count, path);

}

int main() {

	int a[] = {1, 2, 3, 5, 4};
	Solution(a, sizeof(a)/sizeof(*a));
	return 0;
}