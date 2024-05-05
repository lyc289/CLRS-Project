#pragma once
#include <iostream>
#include <chrono>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <graph_hash_of_mixed_weighted/two_graphs_operations/graph_hash_of_mixed_weighted_to_graph_v_of_v_idealID_2.h>
#include <build_in_progress/HL/dynamic/two_hop_labels_base.h>
bool PLL_dynamic_generate_PPR = true;

//从v_k开始的Dijkstra搜索
void PLL_thread_function_dij_mixed(int v_k, int N)
{
	if (PLL_throw_error) {
		return;
	}
	if (labal_size_595 > max_labal_size_595) {
		PLL_throw_error = true;
		throw reach_limit_error_string_MB;  // after catching error, must call terminate_procedures_595(), otherwise this PLL cannot be reused
	}
	if (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin_time_595).count() > max_run_time_nanoseconds_595) {
		PLL_throw_error = true;
		throw reach_limit_error_string_time;  // after catching error, must call terminate_procedures_595(), otherwise this PLL cannot be reused
	}

	//从q队列中pop一个点used_id
	mtx_595[max_N_ID_for_mtx_595 - 1].lock();
	int used_id = Qid_595.front();
	Qid_595.pop();
	mtx_595[max_N_ID_for_mtx_595 - 1].unlock();
	
	queue<int> P_changed_vertices;
	queue<int> T_changed_vertices;//存储v_k标签的辅助数组
	auto& Q_handles = Q_handles_dij_595[used_id];// vector<vector<boost::heap::fibonacci_heap<PLL_dynamic_node_for_sp>::handle_typ>>// PLL_dynamic_node_for_sp {v, priority}

	PLL_dynamic_node_for_sp node;
	boost::heap::fibonacci_heap<PLL_dynamic_node_for_sp> Q;
	two_hop_label_v1 xx;//{vertex, distance}

	node.vertex = v_k;
	node.priority_value = 0;
	Q_handles[v_k] = Q.push(node);//每个顶点维护一个斐波那契堆, 初始为{v_k, 0}
	P_dij_595[used_id][v_k] = 0;//当前以v_k为源点的图中各顶点的最短距离
	P_changed_vertices.push(v_k);

	//遍历v_k标签集合中的所有标签,将顶点加入T_changed_vertices, 距离加入T_dij_595[used_id][v]
	mtx_595[v_k].lock();
	int L_v_k_size = L_temp_595[v_k].size();
	for (int i = 0; i < L_v_k_size; i++) {
		int L_v_k_i_vertex = L_temp_595[v_k][i].vertex;
		T_dij_595[used_id][L_v_k_i_vertex] = L_temp_595[v_k][i].distance; 
		T_changed_vertices.push(L_v_k_i_vertex);
	}
	mtx_595[v_k].unlock();

	long long int new_label_num = 0;

	while (Q.size() > 0) {

		node = Q.top();
		Q.pop();
		int u = node.vertex;//修改u的标签集合
		//仅对编号更大的顶点u进行操作, 减少状态标记数组
		if (v_k <= u) { // this pruning condition is not in the original 2013 PLL paper// r(u)>=r(v)
			weightTYPE P_u = node.priority_value;// priority_value: dijkstra中的dist
			weightTYPE P_u_with_error = P_u + 1e-5;
			weightTYPE query_v_k_u = std::numeric_limits<weightTYPE>::max();
			int common_hub_for_query_v_k_u;
		//遍历label, 求v_k到u的距离最小值query_v_k_u
#ifdef _WIN32
			mtx_595[u].lock();
			auto L_u_size = L_temp_595[u].size(); // a vector<PLL_with_non_adj_reduction_sorted_label>
			mtx_595[u].unlock();
			for (int i = 0; i < L_u_size; i++) { //遍历u的每个标签
				mtx_595[u].lock();      // put lock in for loop is very slow, but it may be the only way under Windows
				weightTYPE dis = L_temp_595[u][i].distance + T_dij_595[used_id][L_temp_595[u][i].vertex];// L_temp_595中的数据一定存在, 如果不相连, T_dij_595值为DOUBLE_MAX
				mtx_595[u].unlock();
				if (query_v_k_u > dis) { query_v_k_u = dis; common_hub_for_query_v_k_u = L_temp_595[u][i].vertex; }
			} 
#else
			mtx_595[u].lock();
			auto L_u_size1 = L_temp_595[u].size(); // a vector<PLL_with_non_adj_reduction_sorted_label>
			for (int i = 0; i < L_u_size1; i++) {
				weightTYPE dis = L_temp_595[u][i].distance + T_dij_595[used_id][L_temp_595[u][i].vertex];   // dont know why this code does not work under Windows
				if (query_v_k_u > dis) { query_v_k_u = dis; common_hub_for_query_v_k_u = L_temp_595[u][i].vertex; }
			} //求query的值
			mtx_595[u].unlock();
#endif
			//if (query(v,u,L)>d(u))
			if (P_u_with_error < query_v_k_u) { // this is pruning, 但此时需要更新标签, 将{v_k, dist}加入u的标签集合中
				xx.vertex = v_k;
				xx.distance = P_u;

				//insert (v, d(u)) into L(u)
				mtx_595[u].lock();
				L_temp_595[u].push_back(xx);
				mtx_595[u].unlock();
				new_label_num++;

				/*下面是dij更新邻接点的过程，同时更新优先队列和距离*/
				int u_adj_size = ideal_graph_595[u].size();
				for (int i = 0; i < u_adj_size; i++) {//for each vertex x\in N(v)
					int adj_v = ideal_graph_595[u][i].first; // 邻接点
					weightTYPE ec = ideal_graph_595[u][i].second; 

					if (P_dij_595[used_id][adj_v] == std::numeric_limits<weightTYPE>::max()) { //尚未到达的点//if d(x)==NIL
						node.vertex = adj_v;//{v, priority}
						node.priority_value = P_u + ec;
						Q_handles[adj_v] = Q.push(node);
						P_dij_595[used_id][adj_v] = node.priority_value;
						P_changed_vertices.push(adj_v);
					}
					else {
						if (P_dij_595[used_id][adj_v] > P_u + ec) {//else if d(x)>d(u)+w(x,u)
							node.vertex = adj_v;
							node.priority_value = P_u + ec;
							Q.update(Q_handles[adj_v], node);
							P_dij_595[used_id][adj_v] = node.priority_value;
						}
					}
				}
			}
			else if (PLL_dynamic_generate_PPR) {//更新PPR//if (query(v,u,L)<=d(u))
				/* add v_k into PPR(u,common_hub_for_query_v_k_u), and add u into PPR(v_k,common_hub_for_query_v_k_u)*/
				if (common_hub_for_query_v_k_u != v_k) {
					mtx_595[u].lock();
					PPR_insert(PPR_595, u, common_hub_for_query_v_k_u, v_k);
					mtx_595[u].unlock();
				}
				if (common_hub_for_query_v_k_u != u) {
					mtx_595[v_k].lock();
					PPR_insert(PPR_595, v_k, common_hub_for_query_v_k_u, u);
					mtx_595[v_k].unlock();
				}
			}
		}
	}
	//将P_changed_vertices中的点在P_dij_595[used_id]中置为DOUBLE_MAX
	while (P_changed_vertices.size() > 0) {
		P_dij_595[used_id][P_changed_vertices.front()] = std::numeric_limits<weightTYPE>::max(); // reverse-allocate P values
		P_changed_vertices.pop();
	}
	//将T_changed_vertices中的点在T_dij_595[used_id]中置为DOUBLE_MAX
	while (T_changed_vertices.size() > 0) {
		T_dij_595[used_id][T_changed_vertices.front()] = std::numeric_limits<weightTYPE>::max(); // reverse-allocate T values
		T_changed_vertices.pop();
	}

	mtx_595[v_k].lock();
	vector<two_hop_label_v1>(L_temp_595[v_k]).swap(L_temp_595[v_k]); // swap释放vector中多余空间
	mtx_595[v_k].unlock();

	mtx_595[max_N_ID_for_mtx_595 - 1].lock();
	Qid_595.push(used_id);//猜测是防止其他线程中的该函数操作同一个used_id
	labal_size_595 = labal_size_595 + new_label_num;
	mtx_595[max_N_ID_for_mtx_595 - 1].unlock();
}

void PLL_dynamic(graph_hash_of_mixed_weighted& input_graph, int max_N_ID, int num_of_threads, graph_hash_of_mixed_weighted_two_hop_case_info_v1& case_info)
{
	//----------------------------------- step 1: initialization ------------------------------------------------------------------
	auto begin = std::chrono::high_resolution_clock::now();
	begin_time_595 = std::chrono::high_resolution_clock::now();
	max_run_time_nanoseconds_595 = case_info.max_run_time_seconds * 1e9;
	labal_size_595 = 0;
	max_labal_size_595 = case_info.max_labal_size;

	if (max_N_ID > max_N_ID_for_mtx_595) {
		cout << "max_N_ID > max_N_ID_for_mtx_595; max_N_ID_for_mtx_595 is too small!" << endl;
		exit(1);
	}
	mtx_595[max_N_ID_for_mtx_595 - 1].lock();
	if (this_parallel_PLL_PSL_is_running_595 == true) {
		cout << "the following parallel PLL code cannot be run parallelly, due to globel values" << endl;
		exit(1);
	}
	this_parallel_PLL_PSL_is_running_595 = true;
	PLL_throw_error = false;
	mtx_595[max_N_ID_for_mtx_595 - 1].unlock();

	L_temp_595.resize(max_N_ID);
	PPR_595.resize(max_N_ID);
	int N = input_graph.hash_of_vectors.size();
	/*change graphs*/
	ideal_graph_595 = graph_hash_of_mixed_weighted_to_graph_v_of_v_idealID_2(input_graph, max_N_ID);

	auto end = std::chrono::high_resolution_clock::now();
	case_info.time_initialization = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9; // s

	//----------------------------------------------- step 2: generate labels ---------------------------------------------------------------
	begin = std::chrono::high_resolution_clock::now();

	ThreadPool pool(num_of_threads);
	std::vector< std::future<int> > results; // return typename: xxx
	P_dij_595.resize(num_of_threads);
	T_dij_595.resize(num_of_threads);
	Q_handles_dij_595.resize(num_of_threads);
	queue<int>().swap(Qid_595);
	for (int i = 0; i < num_of_threads; i++) {//每个线程都维护各自的P_dij_595, T_dij_595, Q_handles_dij_595, 容量均为顶点数N
		P_dij_595[i].resize(N, std::numeric_limits<weightTYPE>::max());
		T_dij_595[i].resize(N, std::numeric_limits<weightTYPE>::max());
		Q_handles_dij_595[i].resize(N);
		Qid_595.push(i);//给每个任务赋予特定编号
	}
	int push_num = 0;//num_of_threads个线程对push_num个任务进行处理
	for (int v_k = 0; v_k < N; v_k++) {// 按照编号遍历每个点
		if (ideal_graph_595[v_k].size() > 0) {  // 不从孤立点出发 // std::vector<std::vector<std::pair<int, double>>>
			results.emplace_back(
				pool.enqueue([v_k, N] { // pass const type value j to thread; [] can be empty
					PLL_thread_function_dij_mixed(v_k, N);
					return 1; // return to results; the return type must be the same with results
					})
			);
			push_num++;
		}
	}
	for (auto&& result : results)
		result.get(); //all threads finish here
	results.clear();
	end = std::chrono::high_resolution_clock::now();
	case_info.time_generate_labels = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9; // s

	for (int i = 0; i < max_N_ID; i++) {
		sort(L_temp_595[i].begin(), L_temp_595[i].end(), compare_two_hop_label_small_to_large); // sort is necessary
	}

	case_info.L = L_temp_595;
	case_info.PPR = PPR_595;
	two_hop_clear_global_values();
}