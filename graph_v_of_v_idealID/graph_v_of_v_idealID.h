#pragma once
#include<vector>
#include<iostream>

/*this function only suits ideal vertex IDs: from 0 to V-1;

this function is for undirected and edge-weighted graph;

idealizing vertex IDs is about fast-hashing vertex IDs*/

#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted_binary_operations.h>

typedef std::vector<std::vector<std::pair<int, double>>> graph_v_of_v_idealID;

//增加e1到e2权重为ec的边
void graph_v_of_v_idealID_add_edge(graph_v_of_v_idealID& g, int e1, int e2, double ec) {
	 /*
	 Add the edges (e1,e2) and (e2,e1) with the weight ec
	 When the edge exists, it will update its weight.
	 */
	graph_hash_of_mixed_weighted_binary_operations_insert(g[e1], e2, ec);
	graph_hash_of_mixed_weighted_binary_operations_insert(g[e2], e1, ec);
}

//删除e1到e2的边
void graph_v_of_v_idealID_remove_edge(graph_v_of_v_idealID& g, int e1, int e2) {
	 /*
	 Remove the edges (e1,e2) and (e2,e1)
	 If the edge does not exist, it will do nothing.
	 */
	graph_hash_of_mixed_weighted_binary_operations_erase(g[e1], e2);
	graph_hash_of_mixed_weighted_binary_operations_erase(g[e2], e1);
}

//删除点v
void graph_v_of_v_idealID_remove_all_adjacent_edges(graph_v_of_v_idealID& g, int v) {
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
		graph_hash_of_mixed_weighted_binary_operations_erase(g[it->first], v);
	}
	std::vector<std::pair<int, double>>().swap(g[v]);
}

//返回是否存在e1到e2的边
bool graph_v_of_v_idealID_contain_edge(graph_v_of_v_idealID& g, int e1, int e2) {
	/*
	Return true if graph contain edge (e1,e2)
	*/
	return graph_hash_of_mixed_weighted_binary_operations_search(g[e1], e2);
}

//返回e1到e2的边权值
double graph_v_of_v_idealID_edge_weight(const graph_v_of_v_idealID& g, int e1, int e2) {
	/*
	Return the weight of edge (e1,e2)
	If the edge does not exist, return std::numeric_limits<double>::max()
	*/
	return graph_hash_of_mixed_weighted_binary_operations_search_weight(g[e1], e2);
}

//返回vertex的最小边权值
double graph_v_of_v_idealID_smallest_adj_edge_weight(graph_v_of_v_idealID& input_graph, int vertex) {
	/*
	Return the least weight edge from point vertex
	If point vertex contain no edge, return std::numeric_limits<double>::max()
	*/
	double smallest_adj_ec = std::numeric_limits<double>::max();
	for (int i = input_graph[vertex].size() - 1; i >= 0; i--) {
		double ec = input_graph[vertex][i].second;
		if (smallest_adj_ec > ec) {
			smallest_adj_ec = ec;
		}
	}
	return smallest_adj_ec;
}

//返回无向图中边的总数
long long int graph_v_of_v_idealID_total_edge_num(graph_v_of_v_idealID& g) {
	/*
	Returns the number of edges in the figure
	(e1,e2) and (e2,e1) will be counted only once
	*/
	int num = 0;
	for (auto it = g.begin(); it != g.end(); it++) {
		num = num + (*it).size();
	}
	return num / 2;
}

//计算图占用空间(MB)
double graph_v_of_v_idealID_total_RAM_MB(graph_v_of_v_idealID& g) {
	/*
	we assume that the edge weight type is float  ???
	*/
	double bit_num = g.size() * sizeof(std::vector <std::pair<int, double>>) + graph_v_of_v_idealID_total_edge_num(g) * sizeof(std::pair<int, double>); // two pointers for each vector
	return bit_num / 1024 / 1024;
}

//打印图
void graph_v_of_v_idealID_print(graph_v_of_v_idealID& g) {
	std::cout << "graph_v_of_v_idealID_print:" << std::endl;
	int size = g.size();
	for (int i = 0; i < size; i++) {
		std::cout << "Vertex " << i << " Adj List: ";
		int v_size = g[i].size();
		for (int j = 0; j < v_size; j++) {
			std::cout << "<" << g[i][j].first << "," << g[i][j].second << "> ";
		}
		std::cout << std::endl;
	}
	std::cout << "graph_v_of_v_idealID_print END" << std::endl;
}

//检查邻接表pair对是否按照顶点编号从小到大排列
bool graph_v_of_v_idealID_check_sort(graph_v_of_v_idealID& g) {

	/*this function checks whether adjacency lists are sorted*/

	int N = g.size();
	for (int xx = 0; xx < N; xx++) {
		for(int yy = g[xx].size() - 1; yy >= 0; yy--) {
			if (yy != 0 && g[xx][yy].first <= g[xx][yy - 1].first) {
				return false;
			}
		}
	}
	return true;
}

//测试
/*examples
---------------------------
#include <graph_v_of_v_idealID/graph_v_of_v_idealID.h>

int main()
{
	graph_v_of_v_idealID_example();
}
------------------------------------------

*/


void test_graph_v_of_v_idealID() {

	int N = 5;
	graph_v_of_v_idealID g(N);

	graph_v_of_v_idealID_add_edge(g, 1, 2, 0.5);
	graph_v_of_v_idealID_add_edge(g, 2, 4, 0.5);
	graph_v_of_v_idealID_add_edge(g, 1, 3, 0.5);
	graph_v_of_v_idealID_add_edge(g, 3, 4, 0.7);
	graph_v_of_v_idealID_add_edge(g, 1, 2, 0.7);

	graph_v_of_v_idealID_print(g);

}


void graph_v_of_v_idealID_example() {

	/*
	Create a complete graph of 10 nodes
	Weight of edge (u,v) and (v,u) equal to min(u,v)+max(u,v)*0.1
	*/
	using std::cout;
	int N = 10;
	graph_v_of_v_idealID g(N);

	/*
	Insert the edge
	When the edge exists, it will update its weight.
	*/
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			graph_v_of_v_idealID_add_edge(g, i, j, j + 0.1 * i); // Insert the edge(i,j) with value j+0.1*i
		}
	}

	/*
	Get the number of edges, (u,v) and (v,u) only be counted once
	The output is 45 (10*9/2)
	*/
	std::cout << graph_v_of_v_idealID_total_edge_num(g) << '\n';

	/*
	Get the least weight edge from point 3
	The output is 0.3, edge is (3,0)
	*/
	std::cout << graph_v_of_v_idealID_smallest_adj_edge_weight(g, 3) << '\n';

	/*
	Check if graph contain the edge (3,1) and get its value
	The output is 1 1.3
	*/
	std::cout << graph_v_of_v_idealID_contain_edge(g, 3, 1) << " " << graph_v_of_v_idealID_edge_weight(g, 3, 1) << '\n';

	/*
	Remove half of the edge
	If the edge does not exist, it will do nothing.
	*/
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			if ((i + j) % 2 == 1)
				graph_v_of_v_idealID_remove_edge(g, i, j);
		}
	}

	/*
	Now the number of edges is 20
	*/
	std::cout << graph_v_of_v_idealID_total_edge_num(g) << '\n';;

	/*
	Now the least weight edge from point 3 is 1.3
	Because edge(3,0) is removed
	*/
	std::cout << graph_v_of_v_idealID_smallest_adj_edge_weight(g, 3) << '\n';;

	/*
	Now the graph no longer contain the edge (3,0) and its value become std::numeric_limits<double>::max()
	*/
	std::cout << graph_v_of_v_idealID_contain_edge(g, 3, 0) << " " << graph_v_of_v_idealID_edge_weight(g, 3, 0) << '\n';


	graph_v_of_v_idealID_print(g); // print the graph
}

