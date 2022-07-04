#include "deadlock_detector.h"
#include "common.h"
#include <iostream>

/// this is the function you need to (re)implement
///
/// parameter edges[] contains a list of request- and assignment- edges
///   example of a request edge, process "p1" resource "r1"
///     "p1 -> r1"
///   example of an assignment edge, process "XYz" resource "XYz"
///     "XYz <- XYz"
///
/// You need to process edges[] one edge at a time, and run a deadlock
/// detection after each edge. As soon as you detect a deadlock, your function
/// needs to stop processing edges and return an instance of Result structure
/// with edge_index set to the index that caused the deadlock, and dl_procs set
/// to contain with names of processes that are in the deadlock.
///
/// To indicate no deadlock was detected after processing all edges, you must
/// return Result with edge_index = -1 and empty dl_procs[].
///

class FastGraph {
    public:
        std::vector<std::vector<int>> adj_list;
        std::vector<int> out_counts;
} graph;

Result detect_deadlock(const std::vector<std::string> & edges)
{
    Result result;
    Word2Int w2i;
    std::vector<std::string> i2w;
    int size = 0;

    for(int i = 0; i < (int)edges.size(); i++){
        //Process edges and populate the adj_list
        std::vector<std::string> edge = split(edges.at(i));
        edge.at(2) = edge.at(2) + "*";
        int process = w2i.get(edge[0]);
        int resource = w2i.get(edge[2]);
        if(process >= (int)i2w.size()){
            i2w.push_back(edge.at(0));
        }
        if(resource >= (int)i2w.size()){
            i2w.push_back(edge.at(2));
        }
        size = std::max(size, std::max(process, resource));
        graph.adj_list.resize(size+1);
        graph.out_counts.resize(size+1);

        if(edge.at(1) == "->"){
            graph.adj_list.at(resource).push_back(process);
            graph.out_counts.at(process)++;
        }
        if(edge.at(1) == "<-"){
            graph.adj_list.at(process).push_back(resource);
            graph.out_counts.at(resource)++;
        }

        std::vector<int> out = graph.out_counts;
        std::vector<int> zeros;

        //Find all nodes in the graph with outdegree==0.
        for(int i = 0; i < (int)out.size(); i++){
            if(out.at(i) == 0){
                zeros.push_back(i);
            }
        }

        //Remove nodes with an outdegree==0 from the adj_list.
        while(!zeros.empty()){
            int n = zeros.back();
            zeros.pop_back();
            for(int n2 : graph.adj_list.at(n)){
                out.at(n2)--;
                if(out.at(n2) == 0){
                    zeros.push_back(n2);
                }
            }
        }
        //Search for a cycle.
        for(int i = 0; i < (int)out.size(); i++){
            if(out.at(i) != 0 && i2w.at(i).back() != '*'){
                result.dl_procs.push_back(i2w.at(i));
            }
        }

        if(!result.dl_procs.empty()){
            result.edge_index = i;
            return result;
        }
    }

    result.edge_index = -1;
    return result;
}