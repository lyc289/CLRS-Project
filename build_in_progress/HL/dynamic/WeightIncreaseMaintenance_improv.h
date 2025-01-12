#pragma once

#include <queue>
#include <build_in_progress/HL/dynamic/PLL_dynamic.h>

void SPREAD1(graph_v_of_v_idealID& instance_graph, vector<vector<two_hop_label_v1>>* L,
	std::vector<affected_label>& al1, std::vector<pair_label>* al2, ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {

	/*TO DO 2*/

}

void SPREAD2(graph_v_of_v_idealID& instance_graph, vector<vector<two_hop_label_v1>>* L, PPR_type* PPR,
	std::vector<pair_label>& al2, std::vector<affected_label>* al3, ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {
	std::vector<pair_label>::iterator it;
	for(it=al2.begin();it!=al2.end();it++)//for each (x,y)属于AL2
	{
		std::vector<int> PPR_xyandy = PPR_retrieve(*PPR,it->first,it->second);
		PPR_xyandy.push_back(it->second);
		for(std::vector<int>::iterator ilter = PPR_xyandy.begin();ilter!=PPR_xyandy.end();ilter++)
		{
			if(instance_graph[*ilter].size()>instance_graph[it->first].size())//r(t)>r(x)
			{
				double d1 = 1e12;//inf
				for(std::vector<std::pair<int, double>>::iterator it2 = instance_graph[it->first].begin();it2!=instance_graph[it->first].end();it2++)
				{
					double d1_new =(*L)[it2->first][*ilter].distance+it2->second;
					if(d1_new<d1)
					{
						d1 =d1_new;
					}
				}// 计算d1(x,t)
				pair<weightTYPE, int> hc = Query2(it->first,*ilter);
				if(hc.first>d1)//不太了解为什么会产生报错，编译的时候看看？
				{
					(*al3).push_back(affected_label(it->first,*ilter,d1));
				}
				else
				{
					PPR_insert(*PPR,it->first,hc.second,*ilter);
					PPR_insert(*PPR,*ilter,hc.second,it->first);
				}
			}
			else
			{
				double d1 = 1e12;
				for(std::vector<std::pair<int, double>>::iterator it2 = instance_graph[*ilter].begin();it2!=instance_graph[*ilter].end();it2++)
				{
					double d1_new =(*L)[it2->first][it->first].distance+it2->second;
					if(d1_new<d1)
					{
						d1 =d1_new;
					}
				}
				pair<weightTYPE, int> hc = Query2(*ilter,it->first);
				if(hc.first>d1)
				{
					(*al3).push_back(affected_label(*ilter,it->first,d1));
				}
				else
				{
					PPR_insert(*PPR,it->first,hc.second,*ilter);
					PPR_insert(*PPR,*ilter,hc.second,it->first);
				}
			}	
		}//for each t属于PPR[x,y]并y
	}//遍历al2
	/*TO DO 3*/

}

void SPREAD3(graph_v_of_v_idealID& instance_graph, vector<vector<two_hop_label_v1>>* L, PPR_type* PPR, std::vector<affected_label>& al3,
	ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {

	/*TO DO 4*/

	int vex_num=instance_graph.size()/2;
	double *Dis=new double[vex_num];
	for (auto affected_l : al3)//{u,v,du}
	{
		results_dynamic.emplace_back(pool_dynamic.enqueue([affected_l, L, PPR, Dis, vex_num, instance_graph]{
		pair<weightTYPE,int> dis_vex= graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, affected_l.first, affected_l.second);
		if (dis_vex.first <= affected_l.dis)
		{
			mtx_5952[affected_l.first].lock();
			PPR_insert(*PPR, affected_l.first, dis_vex.second, affected_l.second);
			mtx_5952[affected_l.second].unlock();
			mtx_5952[affected_l.second].lock();
			PPR_insert(*PPR, affected_l.second, dis_vex.second, affected_l.first);
			mtx_5952[affected_l.second].unlock();
		}
		else
		{
			fill_n(Dis, vex_num, -1);
			Dis[affected_l.first]=affected_l.dis;
			PLL_dynamic_node_for_sp node={affected_l.first, affected_l.dis};//{vertex, val}
			boost::heap::fibonacci_heap<PLL_dynamic_node_for_sp> Q;
			vector<graph_hash_of_mixed_weighted_HL_PLL_v1_handle_t_for_sp> Q_handles(vex_num);
			Q_handles[affected_l.first] = Q.push(node);
			while (Q.size()>0)
			{
				node=Q.top();
				Q.pop();
				int x=node.vertex;
				weightTYPE dx=node.priority_value;
				(*L)[x][affected_l.second].distance=min((*L)[x][affected_l.second].distance, dx);
				for (auto xn:instance_graph[affected_l.second])
				{
					if (affected_l.second<xn.first)
					{
						if (Dis[xn.first]==-1)
						{
							Dis[xn.first]=graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc(*L, xn.first, affected_l.second);
						}
						double newVal=dx+graph_v_of_v_idealID_edge_weight(instance_graph, xn.first, x);
						if (Dis[xn.first]>newVal)
						{
							Dis[xn.first]=newVal;
							node={xn.first, Dis[xn.first]};
							bool f=true;//should insert
							for (auto it=Q.begin(); it!=Q.end();it++)
							{
								if ((*it).vertex==node.vertex)
								{
									Q.update(Q_handles[node.vertex], node);
									f=false;
									break;
								}
							}
							if (f) Q_handles[node.vertex]=Q.push(node);
						}
						else
						{
							mtx_5952[xn.first].lock();
							PPR_insert(*PPR, xn.first, dis_vex.second, affected_l.second);
							mtx_5952[xn.first].unlock();
							mtx_5952[affected_l.second].lock();
							PPR_insert(*PPR, affected_l.second, dis_vex.second, xn.first);
							mtx_5952[affected_l.second].unlock();
						}
					}
				}
			}
		}		
		return 1;}));
	}
	for (auto&& result : results_dynamic) result.get();
	std::vector<std::future<int>>().swap(results_dynamic);
}

void WeightIncreaseMaintenance_improv(graph_v_of_v_idealID& instance_graph, graph_hash_of_mixed_weighted_two_hop_case_info_v1& mm, int v1, int v2, weightTYPE w_old, weightTYPE w_new,
	ThreadPool& pool_dynamic, std::vector<std::future<int>>& results_dynamic) {

	std::vector<affected_label> al1, al3;
	std::vector<pair_label> al2;
	//获取al1
	for (auto it : mm.L[v1]) {
		if (it.vertex <= v2 && abs(search_sorted_two_hop_label(mm.L[v2], it.vertex) - it.distance - w_old) < 1e-5) {
			al1.push_back(affected_label(v2, it.vertex, it.distance + w_old));
		}
	}
	for (auto it : mm.L[v2]) {
		if (it.vertex <= v1 && abs(search_sorted_two_hop_label(mm.L[v1], it.vertex) - it.distance - w_old) < 1e-5) {
			al1.push_back(affected_label(v1, it.vertex, it.distance + w_old));
		}
	}

	//cout << "al1.size() " << al1.size() << endl;

	SPREAD1(instance_graph, &mm.L, al1, &al2, pool_dynamic, results_dynamic);
	SPREAD2(instance_graph, &mm.L, &mm.PPR, al2, &al3, pool_dynamic, results_dynamic);
	SPREAD3(instance_graph, &mm.L, &mm.PPR, al3, pool_dynamic, results_dynamic);

	//for (auto it : al2) {
	//	cout << "al2 " << it.first << " " << it.second << endl;
	//}
	//for (auto it : al3) {
	//	cout << "al3 " << it.first << " " << it.second << " " << it.dis << endl;
	//}
}

