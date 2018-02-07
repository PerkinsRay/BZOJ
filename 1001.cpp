#include <map>
#include <set>
#include <unordered_map>
#include<vector>
#include<queue>
#include<iostream>
struct Edge{
    int node;
    int distance;
    Edge(int _node, int _distance) :node(_node),distance(_distance){
 
    }
};
struct Node {
    int node;
    int distance;
    Node(int n, int d):node(n),distance(d) {
 
    }
    bool operator<(const Node& x)const  {
        return distance > x.distance;
    }
};
int dijkstra(std::vector<std::vector<Edge>>& Edges,int MaxNode,int First,int Last) {
    unsigned int *dis = new unsigned int[MaxNode];
    memset(dis, -1, MaxNode * sizeof(int));
    std::priority_queue<Node> queue;
    dis[First] = 0;
    queue.emplace(First, 0);
    while (!queue.empty()) {
        Node traveled = queue.top();
        queue.pop();
        if (traveled.node == Last) {
            return traveled.distance;
        }
        else if (traveled.distance > dis[traveled.node])
        {
            continue;
        }
        else {
            for (std::vector<Edge>::iterator i= Edges[traveled.node].begin(); i != Edges[traveled.node].end();++i) {
 
                if (dis[i->node] > (dis[traveled.node] + i->distance)) {
                    dis[i->node] = dis[traveled.node] + i->distance;
                    queue.push(Node(i->node, dis[i->node]));
                }
            }
        }
    }
    return 0;
 
}
 
void AddEdge(std::vector<std::vector<Edge>>&Edges,int i, int j, int v) {
    //std::cout <<"t::"<< i << " " << j << " "<<v<<std::endl;
    Edges[i].push_back(Edge(j, v));
    Edges[j].push_back(Edge(i, v));
}
 
int main()
{
    int  N, M,tmp;
    std::cin >> N >> M;
    std::vector<std::vector<Edge>> Edges((N-1)*(M-1)*2+2);
    for (int j = 0; j < M-1; j++) {
        std::cin >> tmp;
        AddEdge(Edges, 0, j+1, tmp);
    }
    for (int i = 1; i < N - 1; i++) {
        for (int j = 0; j < M-1; j++) {
            std::cin >> tmp;
            AddEdge(Edges, 2 * i*(M - 1) + j + 1, (2 * i - 1)*(M - 1) + j + 1, tmp);
        }
    }
    for (int j = 0; j < M-1; j++) {
        std::cin >> tmp;
        AddEdge(Edges, 2 * (N - 1)*(M - 1) + 1, (2 * (N - 1) - 1)*(M - 1) + j + 1, tmp);
    }
 
    for (int i = 0; i < N - 1; i++) {
        std::cin >> tmp;
        AddEdge(Edges, 1 + (M - 1)*(2 * i + 1), (N - 1)*(M - 1) * 2 + 1, tmp);
        for (int j = 1; j < M-1; j++) {
            std::cin >> tmp;
            AddEdge(Edges, 1 + j + (M - 1)*(2 * i + 1), j + (M - 1)*(2 * i), tmp);
        }
        std::cin >> tmp;
        AddEdge(Edges, M - 1 + (M - 1)*(2 * i),0, tmp);
    }
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < M - 1; j++) {
            std::cin >> tmp;
            AddEdge(Edges, 2 * i*(M - 1) + j + 1, (2 * i+1)*(M - 1) + j + 1, tmp);
        }
    }
    std::cout << dijkstra(Edges, (N - 1)*(M - 1) * 2 + 2, 0, 13);
}