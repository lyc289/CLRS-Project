#pragma once
#include <build_in_progress/HL/dynamic/PLL_dynamic.h>
//v1, v2之间权值减少
void WeightDecreaseMaintenance_improv_step1(int v1, int v2, weightTYPE w_new, vector<vector<two_hop_label_v1>>* L, PPR_type* PPR, std::vector<affected_label>* CL,
	ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {
	//CL:{first, second, dis}:要更新的两个标签
	for (int sl = 0; sl < 2; sl++) {
		if (sl == 1) {
			swap(v1, v2);
		}
		for (auto it : (*L)[v1]) {//遍历v1的所有标签, 当前为it
			if (it.vertex <= v2) {
				results_dynamic.emplace_back(pool_dynamic.enqueue([it, v2, L, PPR, w_new, CL] {

					auto query_result = graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, it.vertex, v2); // query_result is {distance, common hub}// 原距离, 从3到2(1作为hub)
					if (query_result.first > it.distance + w_new) {//需要减小, 加入CL:{terminal, source, dist}
						mtx_595_1.lock();
						CL->push_back(affected_label{ v2 , it.vertex, it.distance + w_new });
						mtx_595_1.unlock();
					}
					else {
						auto search_result = search_sorted_two_hop_label((*L)[v2], it.vertex);//从3到2, 1作为hub
						if (search_result > it.distance + w_new && search_result != MAX_VALUE) {//v1作为hub, 查询v2和v1标签中另一点的距离需要更新
							mtx_595_1.lock();
							CL->push_back(affected_label{ v2, it.vertex, it.distance + w_new });
							mtx_595_1.unlock();
						}
						if (query_result.second != it.vertex) {
							mtx_5952[v2].lock();
							PPR_insert(*PPR, v2, query_result.second, it.vertex);
							mtx_5952[v2].unlock();
						}
						if (query_result.second != v2) {
							mtx_5952[it.vertex].lock();
							PPR_insert(*PPR, it.vertex, query_result.second, v2);
							mtx_5952[it.vertex].unlock();
						}
					}

					return 1; }));
			}
		}
	}

	for (auto&& result : results_dynamic) {
		result.get();
	}
	std::vector<std::future<int>>().swap(results_dynamic);
}

void DIFFUSE(graph_v_of_v_idealID& instance_graph, vector<vector<two_hop_label_v1>>* L, PPR_type* PPR, std::vector<affected_label>& CL,
	ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {

	/*TO DO 1*/
}

void WeightDecreaseMaintenance_improv(graph_v_of_v_idealID& instance_graph, graph_hash_of_mixed_weighted_two_hop_case_info_v1& mm, int v1, int v2, weightTYPE w_old, weightTYPE w_new,
	ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {

	std::vector<affected_label> CL;
	WeightDecreaseMaintenance_improv_step1(v1, v2, w_new, &mm.L, &mm.PPR, &CL, pool_dynamic, results_dynamic);//获取CL, 更新PPR

	DIFFUSE(instance_graph, &mm.L, &mm.PPR, CL, pool_dynamic, results_dynamic);
}