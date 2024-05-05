#include <cstdio>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

#define max 110  //最大顶点个数
#define INF 0xfffff    //权值上限

int n;  //顶点数

struct node     //顶点节点
{
    int vlue;     
    int key;
    int parent;
    friend bool operator<(node a, node b)   //因要实现最小堆，按升序排列，因而需要重载运算符，重定义优先级，以小为先
    {
        return a.key > b.key;
    }
};

#include <iostream>



void Init(int **G)  //输入图
{
    cout << "cin:v v w 000to end"<<endl;
    int i, j;
    int v;
    scanf_s("%d%d%d", &i, &j, &v);
    while (i || j || v)
    {
        G[i][j] = v;
       // G[j][i] = v;  //有向图
        scanf_s("%d%d%d", &i, &j, &v);
    }
    cout << endl;
}


void Dijkstra(int **G, int s)    //Dijkstra算法，传入源顶点
{
    priority_queue<node> q; //最小优先队列
    node d[max];      //结点集
    bool visited[max]; //用于判断顶点是否已经在最短路径树中

    for (int i = 1; i <= n; i++) //初始化
    {
        d[i].vlue = i;
        d[i].key = INF;           //估算距离置INF
        d[i].parent = -1;         //每个顶点都无父亲节点
        visited[i] = false;     //都未找到最短路
    }
    d[s].key = 0;                 //源点到源点最短路权值为0
    q.push(d[s]);               //压入队列中
    while (!q.empty())           //队列空说明完成了操作
    {
        node cd = q.top();      //取最小顶点
        q.pop();
        int u = cd.vlue;
        if (visited[u])   
            continue;
        visited[u] = true;

        cout << "visit:" << u<<" dist:"<<d[u].key<<" parent:"<< d[u].parent<< endl;

        for (int i = 1; i < n;i++) {           
            if (!visited[i] && d[i].key>d[u].key+G[d[u].vlue][i])
            {
                d[i].parent = u;
                d[i].key = d[u].key + G[d[u].vlue][i];
                q.push(d[i]);
             //   cout <<"加入"<< d[i].key << endl;
            }
        }
    }
}

int main()
{
    printf("Num of v:\n");
    cin>>n;

    n++;

    int** G = (int**) new int* [n];
    for (int i = 0; i < n; i++)
    {
        G[i] = new int[n];
    }
    //初始化
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            G[i][j] = INF;
           // cout << G[i][j] << " ";
        }
        cout << endl;
    }

    Init(G);//读入
    //输出图
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < n; j++)
        {
            if (G[i][j] != INF)
                cout << setw(2) << G[i][j] << " ";
            else
                cout << setw(2) << 0 << " ";
        }
        cout << endl;
    }

    Dijkstra(G,1);


    for (int i = 0; i < n; i++)
    {
        delete[] G[i];
    }
    delete G;
    return 0;
}
/* 测试用例输入

5
1 2 10
1 5 5
2 3 1
2 5 2
3 4 4
4 3 6
4 1 7
5 2 3
5 3 9
5 4 2
0 0 0

5
1 2 3
1 5 1
2 3 2
3 4 1
5 2 1
5 3 2
5 4 3
0 0 0

*/
