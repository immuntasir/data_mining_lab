#include <bits/stdc++.h>
using namespace std;
#define MX1 100010
#define MX2 101
#define MIN_SUPPORT .2
#define MAX 2500000
#define DEBUG 0

map <int, int> rank;
int freq[MX1];
int is_frequent[MX1];
int freq_itemset_count[MX2];
int min_support;
int sorted_items[MX1];
int sorted_index[MX1];
bool cmp (int a, int b) {
    if (freq[a] != freq[b]) return freq[a] > freq[b];
    return a < b;
}

vector < vector <int> > transactions;
vector <int> process_input (string str) {
    vector <int> ret;
    int integer = 0;
    for (int i=0; i<(int)str.size(); i++) {
        if (str[i] == ' ') {
            ret.push_back(integer);
            freq[integer]++;            
            integer = 0;
        }   
        else {
            integer *= 10;
            integer = integer + str[i] - '0';
        }
    }
    return ret;
}

int number_of_nodes = 0;
map <int, int> edge[MAX];
vector <int> edges[MAX];
int freq_node[MAX];
vector <int> pointers[MX1];
void add_transaction (vector <int> transaction) {
    int cur_node = 0;
    int sz = transaction.size();
    for (int i=0; i<sz; i++) {
        if (edge[cur_node].find(transaction[i]) == edge[cur_node].end()) {
            edge[cur_node][transaction[i]] = ++number_of_nodes;
            edges[cur_node].push_back(transaction[i]);            
            pointers[transaction[i]].push_back(number_of_nodes);
        }
        cur_node = edge[cur_node][transaction[i]];        
        freq_node[cur_node] += 1;
    }
}
void construct_fp_tree () {
    int sz = transactions.size();
    for (int i=0; i<sz; i++) {
        add_transaction(transactions[i]);
    }
}

vector <vector <int> > temp;
map <int, int> item_freq_cpt[MX1];

void clean_transactions () {
    vector <int> tmp;
    int sz = transactions.size();    
    for (int i=0; i<sz; i++) {
        for (int j=0; j<(int)transactions[i].size(); j++) {
            if (is_frequent[transactions[i][j]]) tmp.push_back(transactions[i][j]);
        }
        temp.push_back(tmp);
        int sz = tmp.size();
        for (int j=0; j<sz-1; j++) {
			for (int k=j+1; k<sz; k++) {
				item_freq_cpt[tmp[j]][tmp[k]]++;
				item_freq_cpt[tmp[k]][tmp[j]]++;
			}
		}
        tmp.clear();
    }
    transactions = temp;
    temp.clear(); 
}
int num_trees_ = 4213;
map < int, int > cptree[MAX];
vector <int> cptedges[MAX];
int freq_cpt[MAX];
int number_of_nodes_cpt = MX1; 
int num_trees = 0;
void add_conditional_pattern_base (int item, vector <int> pattern, int val) {
    num_trees++;
    int sz = pattern.size();
    int cur_node = item;
    if (DEBUG) {
		printf("%d -> adding %d with %d\n", item, sz, val);
		for (int i=0; i<sz; i++) printf("%d ", pattern[i]);
		printf("\n");
    }
    for (int i=0; i<sz; i++) {
		if (item_freq_cpt[item][pattern[i]] < min_support) continue; 
		if (cptree[cur_node].find(pattern[i]) == cptree[cur_node].end()) {
			cptree[cur_node][pattern[i]] = ++number_of_nodes_cpt;
			cptedges[cur_node].push_back(pattern[i]);
		}
		cur_node = cptree[cur_node][pattern[i]];
		freq_cpt[cur_node] += val;
	} 
}
vector <int> cur_path;
void find_conditional_pattern_bases (int cur_node, int node_idx) {
    if (DEBUG) printf("%d %d %d\n", cur_node, node_idx, (int)edges[cur_node].size());
    if (node_idx != -1) {
        add_conditional_pattern_base(node_idx, cur_path, freq_node[cur_node]);
        cur_path.push_back(node_idx);
    }
    int sz = edges[cur_node].size();
    for (int i=0; i<sz; i++) {
		if (DEBUG) printf("%d(%d). %d --> (%d) %d\n", i, (int)edges[cur_node].size(), cur_node, edges[cur_node][i], edge[cur_node][edges[cur_node][i]]);
        find_conditional_pattern_bases(edge[cur_node][edges[cur_node][i]], edges[cur_node][i]);
    }
    if (cur_path.size() != 0) cur_path.pop_back();
}   

map <int, int> cfptree[MAX];
vector <int> cfptree_edges[MAX];
int freq_cfptree[MAX];
int number_of_nodes_cfptree = MX1;

int number_of_patterns[MX1][MX2];
int number_of_patterns_complete[MX2];

void find_patterns (int cur_node, int node_idx, int depth) {
	if (DEBUG) printf("%d %d %d\n", cur_node, node_idx, depth);
	if (freq_cfptree[cur_node] >= min_support) {
		if (DEBUG) printf("Found pattern of length %d\n", depth+1);
		number_of_patterns[node_idx][depth+1]++;
		number_of_patterns_complete[depth+1]++;
	}
	else if (cur_node >= MX1) return;
	int sz = cfptree_edges[cur_node].size();
	if (DEBUG) printf("Number of different paths for %d (%d): %d\n", cur_node, node_idx, sz);
	for (int i=0; i<sz; i++) {
		if (DEBUG) printf("From %d (%d) with %d to %d\n", cur_node, node_idx, cfptree_edges[cur_node][i], cfptree[cur_node][cfptree_edges[cur_node][i]]);
		find_patterns(cfptree[cur_node][cfptree_edges[cur_node][i]], node_idx, depth+1);
	}
}
void init_cfptree () {
	for (int i=0; i<=number_of_nodes_cfptree; i++) {
		cfptree[i].clear();
		cfptree_edges[i].clear();
	}
	memset(freq_cfptree, 0, sizeof freq_cfptree);
	number_of_nodes_cfptree = 0;
}
int current_idx;
void construct_conditional_fp_tree (int node_1, int node_2, int val) {
	//printf("%d, %d: %d %d\n", node_2, current_idx, sorted_index[node_2], sorted_index[current_idx]);
	//if (sorted_index[node_2] > sorted_index[current_idx]) return;
	//if (DEBUG) printf("--------- %d -----------\n", node_2);
	//if (DEBUG) printf("%d %d %d\n", node_1, node_2, val);
	freq_cfptree[node_2] += val;
	int sz = cptedges[node_1].size();
	//if (DEBUG) printf("Number of paths: %d\n", sz);
	num_trees++;
	for (int i=0; i<sz; i++) {
		if (sorted_index[cptedges[node_1][i]] > sorted_index[current_idx]) continue;
		//if (DEBUG) printf("%d %d %d %d. From %d without %d to %d\n", node_1, node_2, val, i, node_2, cptedges[node_1][i], cptree[node_1][cptedges[node_1][i]]);
		construct_conditional_fp_tree(cptree[node_1][cptedges[node_1][i]], node_2, 0);
		//if (item_freq_cpt[current_idx].find(cptedges[node_1][i]) != item_freq_cpt[current_idx].end() && 
			//item_freq_cpt[current_idx][cptedges[node_1][i]] < min_support) continue;
		if (cfptree[node_2].find(cptedges[node_1][i]) == cfptree[node_2].end()) {
			//if (DEBUG) printf("Creating node for %d: %d\n", cptedges[node_1][i], number_of_nodes_cfptree + 1);
			cfptree[node_2][cptedges[node_1][i]] = ++number_of_nodes_cfptree;
			cfptree_edges[node_2].push_back(cptedges[node_1][i]);
		}
		
		

		//if (DEBUG) printf("%d %d %d %d. From %d with %d to %d\n", node_1, node_2, val, i, node_2, cptedges[node_1][i], cptree[node_1][cptedges[node_1][i]]);
		construct_conditional_fp_tree(cptree[node_1][cptedges[node_1][i]], cfptree[node_2][cptedges[node_1][i]], freq_cpt[cptree[node_1][cptedges[node_1][i]]]);
		
		
	}
}
void dfs (int cur_node) {
	if (DEBUG) printf("%d\n", cur_node);
	for (int i=0; i<(int)cfptree_edges[cur_node].size(); i++) {
		if (DEBUG) printf("%d. %d (%d)\n", i, cfptree_edges[cur_node][i], cfptree[cur_node][cfptree_edges[cur_node][i]]);
		dfs(cfptree[cur_node][cfptree_edges[cur_node][i]]);
	}
} 

void construct_conditional_fp_tree () {
	int last;
	for (int j=0; j<MX1; j++) {
		int i = sorted_items[j];
		if (!is_frequent[i]) {
			last = j - 1;
			break;
		}
	}
	for (int j=last; j>=0; j--) {
		int i = sorted_items[j];
		if (!is_frequent[i]) break;
		
		current_idx = i;
		if (DEBUG) printf("%d is a frequent item.\n", i);
		init_cfptree();
		construct_conditional_fp_tree(i, 0, 0);
		printf("Conditional fp-tree constructed for %d\n", i);
		if (DEBUG) printf("Total number of nodes in fp-tree for %d: %d\n", i, number_of_nodes_cfptree);
		/*
		for (int i=0; i<=number_of_nodes_cfptree; i++) printf("%d: %d (%d)\n", i, freq_cfptree[i], (int)cfptree_edges[i].size());
		for (int i=0; i<(int)cfptree_edges[0].size(); i++) {
			printf("edge  with 0 :%d\n", cfptree_edges[0][i]);
		}
		dfs(0);
		*/
		find_patterns(0, i, 0);
	
	}
	
	printf("Total number of FP-trees: %d\n", num_trees_);
}



int main () { 
    //freopen("../Dataset/small_dataset.dat", "r", stdin);
    //freopen("../Dataset/chess.dat", "r", stdin);    
    freopen("../Dataset/mashroom.dat", "r", stdin);
    //freopen("../Dataset/pstar.dat", "r", stdin);
    //freopen("../Dataset/accidents.dat", "r", stdin);
    //freopen("../Dataset/t1.dat", "r", stdin);
    string str;
    while (getline(cin, str)) {
		transactions.push_back(process_input(str));    
    }
	//min_support = 2;
    min_support = (int) (ceil(double(transactions.size()) * MIN_SUPPORT));
    
    
    int tot_freq = 0;
       
    for (int i=0; i<MX1; i++) {
		if (freq[i] >= min_support) {
            tot_freq++;
            is_frequent[i] = 1;  
        }
    }   
    printf("Total elements in frequent %d-itemsets: %d\n", 1, tot_freq);        
    for (int i=0; i<MX1; i++) sorted_items[i] = i;
    sort(sorted_items, sorted_items + MX1, cmp);
    for (int i=0; i<MX1; i++) sorted_index[sorted_items[i]] = i;
    if (DEBUG) for (int i=0; i<10; i++) printf("%d: %d\n", i, sorted_index[i]);
    clean_transactions();
	
    for (int i=0; i<(int)transactions.size(); i++) {
        sort(transactions[i].begin(), transactions[i].end(), cmp);
    }
	
    construct_fp_tree();
    printf("Construction of fp tree complete.\n");
	printf("Number of nodes: %d\n", number_of_nodes);
	
	find_conditional_pattern_bases(0, -1);
	printf("Search for conditional pattern bases complete.\n");
	
	construct_conditional_fp_tree();
	printf("Construction of conditional tree complete.\n");
	
	printf("Number of %d-frequent itemsets: %d\n", 1, tot_freq);
	for (int i=2; i<MX2; i++) {
		if (number_of_patterns_complete[i] == 0) break;
		printf("Number of %d-frequent itemsets: %d\n", i, number_of_patterns_complete[i]);
		tot_freq += number_of_patterns_complete[i];
	}
	printf("Total number of frequent itemsets: %d\n", tot_freq);
	/*
	*/
    return 0;
}
