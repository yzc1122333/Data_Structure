//#include "./../Datastructure/head_file/Data_structure.h"
//#include <cstring>


/*
八皇后问题
    在一个8x8的国际象棋棋盘，有8个皇后，每个皇后占一个格；要求不出现相互攻击的现象（不能有两个皇后统一行，统一列，或同一对角线）

N皇后问题：
    N x N 的棋盘，N个皇后

关键数据结构的定义
    棋盘：二维数组(N+2)*(N+2)[2是为了方便计算，加了边界]
        0表示位置为空
        1表示皇后
        2表示边界
    位置：struct Pos
        struct Pos{
            int x;
            int y;
        }
    方向： 水平: (-1, 0); (1, 0)
          垂直：(0, -1); (0, 1)
          对角线: (-1, 1); (-1, -1); (1, -1); (1, 1)
          因为实现思路是每一行只能放一个皇后，并且逐行递增，因此只需检测三个方向(-1, -1); (-1 0); (-1, 1)
算法思路：
    1. 初始化： j = 1
    2. 初始化   i = 1
    3. 从第j行开始，恢复i的有效值，判断第i个位置
        a. 位置i可放皇后：标记位置(i,j), j++, 转步骤2
        b. 位置i不可放入皇后：i++，直至可放皇后（长度在棋盘内），转至步骤 a
        c. 当 i>8时， j--，转至步骤3
    结束：
        第8行有位置可放入皇后
*/

/*
enum {N = 3};
c语言：
    N 可以是 int或者 unsigned int，可根据赋值情况自行调整，但是一定占四个字节内存

    enum是真正的常量，和int所占空间相同，可以相互之间隐式转换（可以是负数）

    N 可以算作是标识，没有地址，&N会报错，但是sizeof(N) = 4。因此型必须是 int/unsigned int 类型。可以将enum {N = 3};中的N拿出来单独使用，近似为带 int/unsigned int 类型变量检测的宏定义。

    enum {N = 3};N已经被声明，后边变量不能重复声明N作为变量
C++：
    因为在C++语言中const可以被视作常量，所以对于部分编译器所有被const修饰的char，short， int都可以赋值给enum中的元素，但是复制后会隐式转换成int/unsigned int类型，sizeof(N) = 4
    部分编译器沿用c语言规则依旧不可以


*/
//size代表棋盘大小

//namespace YzcLib{

//template <unsigned int SIZE>
//class NQueen: public Object{
//private:
//    //N是加边界的棋盘大小，用这种方式代替红定义，增加程序安全性
//    enum {N = SIZE + 2};
//    //位置结构体
//    struct POS: Object{
//        POS(unsigned int px = 0, unsigned int py = 0):x(px), y(py){}
//        unsigned int x;
//        unsigned int y;
//    };

//    int chessboard[N][N];
//    POS direction[3]; //3个方向， 0=>左下，1=>正下方， 2=>右下
//    LinkList<POS> solution; //皇后在棋盘的坐标
//    int count;  //记录可行解的次数

//    //初始化函数
//    void _init(){

//        count = 0;

//        for(int i  = 0; i< N; i++){
//            for(int j = 0; j < N; j++){
//                chessboard[i][j] = 2;
//            }
//        }

//        for(int i  = 1; i< N - 1; i++){
//            for(int j = 1; j < N - 1; j++){
//                chessboard[i][j] = 0;
//            }
//        }

//        direction[0].x = -1;
//        direction[0].y = -1;
//        direction[1].x = 0;
//        direction[1].y = -1;
//        direction[2].x = 1;
//        direction[2].y = -1;
//    }



//    void print(){

//        cout<<"count = "<<count<<endl;
//        cout<<"Length = "<<solution.Length()<<endl;
//        for(solution.Move(0); !solution.End(); solution.Next()){
//            cout<<"("<<solution.Current().x<<", "<<solution.Current().y<<')'<<endl;
//        }

//        for( int i = 0; i< N; i++){
//            for(int j = 0; j < N; j++){
//                switch(chessboard[i][j]){
//                    case 0: cout<<' ';break;
//                    case 1: cout<<'#';break;
//                    case 2: cout<<'*';break;
//                }
//            }
//            cout<<endl;
//        }
//        cout<<endl;
//    }


//    bool _check(unsigned int x,unsigned int y, POS direct){
//        bool flag = true;
//        do{
//            x += direct.x;
//            y += direct.y;
//            flag = flag && (chessboard[x][y] == 0);
//        }while(flag);
//        //检测是否到达边界
//        return (chessboard[x][y] == 2);
//    }
//public:
//    void run(unsigned int y = 1){
//        POS loc(1,y);
//        if(loc.y <= SIZE){
//            for(loc.x = 1; loc.x <= SIZE; loc.x++){
//                if(_check(loc.x, loc.y, direction[0]) && \
//                        _check(loc.x, loc.y, direction[1])&& \
//                        _check(loc.x, loc.y, direction[2])){
//                    chessboard[loc.x][loc.y] = 1;
//                    solution.Insert(loc);
//                    run(loc.y + 1);
//                    //递归回退，回复现场
//                    chessboard[loc.x][loc.y] = 0;
//                    solution.Remove(solution.Length() - 1);
//                }
//            }
//        }
//        else{
//            count++;
//            print();
//        }
//    }


//    NQueen(){
//        _init();
//    }



//};
//}

