#include <bits/stdc++.h>
using namespace std;
#define MX1 100010
#define MX2 500
#define MIN_SUPPORT .2
int min_support;
int nxt[MX1][MX2];
int node[MX1];
bool created[MX1];
int identity[MX1];
int sz = 0;
int freq[MX1];
bool done[MX1];
void initialize_support() {
    sz = 0;
    memset(nxt, 0, sizeof nxt);
    memset(created, 0, sizeof created);
    memset(identity, -1, sizeof identity);
    memset(freq, 0, sizeof freq);
    memset(done, 0, sizeof done);
    memset(node, -1, sizeof node);
}

void add_itemset (vector <int> items, int id) {
    int cur = 0;
    int i = 0;
    for (int i=0; i<items.size(); i++) {
        int ch = items[i];        
        if (!created[nxt[cur][ch]]) {
            nxt[cur][ch] = ++sz;
            created[nxt[cur][ch]] = true;
        }
        cur = nxt[cur][ch];
        node[cur] = ch;
    }
    identity[cur] = id;
}

vector < vector <int> > transactions;

vector <int> process_input (string str) {
    vector <int> ret;
    int integer = 0;
    for (int i=0; i<str.size(); i++) {
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


vector < vector <int> > lkminus1;
vector < vector <int> > ck;
vector < vector <int> > lk;

map <string, int > M;

bool check_frequency (vector <int> transaction, int idx) {
    string str = "";    
    char str_temp[10000];
  
    for (int i=0; i<transaction.size(); i++) {
        if (idx != i) {
            sprintf(str_temp, "%d", transaction[i]);
            for (int j=0; str_temp[j] != '\0'; j++) str+=(str_temp[j]);            
            //str = str + str_temp;
            if (idx == transaction.size() - 1) {
                if (i < transaction.size() - 2) str = str + '-';
            }
            else {
                if (i < transaction.size() - 1) str = str +'-';
            }
        }
    }
    //cout << "Checking " << str <<  " " << transaction.size() << " " << idx <<  endl; 
    if (M.find(str) == M.end()) return 0;
    return 1;
}

int j_k, c_k, l_k;

vector < vector <int> > construct_ck () {
    //printf("here");
    //printf("%d %d\n", lkminus1.size(), lkminus1[0].size());
    vector < vector <int> > ret;
    M.clear();
    //printf("here");    
    char str_temp[10000];            
                
    for (int i=0; i<lkminus1.size(); i++) {
        string str = "";
        int sz = lkminus1[i].size();
        //printf("begin\n");  
        //cout << str << endl;      
        for (int j=0; j<lkminus1[i].size(); j++) {
            sprintf(str_temp, "%d", lkminus1[i][j]);
          //  printf("str_temp: %s\n", str_temp);
        
            for (int k=0; str_temp[k] != '\0'; k++) {
             //   printf("char: %c\n", str_temp[j]);             
               str += (str_temp[k]);            
            }            
            if (j < sz - 1) str = str + '-';
        }
        M[str] = 1;
        if (lkminus1[0].size() == 15) cout << str << endl;
        //printf("end\n");        
    }    
    //printf("here");
    for (int i=0; i<lkminus1.size(); i++) {
        for (int j=0; j<lkminus1.size(); j++) {
            if (i!=j) {
                int fl = 1;
                for (int k=0; k<lkminus1[i].size()-1; k++) {
                    if (lkminus1[i][k] != lkminus1[j][k]) {
                        fl = 0;
                    }
                }
                if (fl) {
                    vector < int > temp;
                    if (lkminus1[i][lkminus1[i].size() - 1] < lkminus1[j][lkminus1[j].size() - 1]) temp = lkminus1[i];
                    else temp = lkminus1[j];
                    temp.push_back(max(lkminus1[i][lkminus1[i].size() - 1], lkminus1[j][lkminus1[j].size() - 1]));
                    for (int i=0; i<temp.size(); i++) {
                        if (!check_frequency(temp, i)) fl = 0;
                    }
                    if (fl) {
                       c_k++;                         
                       ret.push_back(temp);                
                    }
                    j_k++;
                }
            }
        }
    }
    return ret;
}

void print (vector <int> ara) {
    printf("{");
    for (int i=0; i<ara.size(); i++) {
        printf("%d", ara[i]);
        if (i < ara.size()-1) printf(", ");
    }
    printf("}");
}

vector <int> current_transaction;
void increment_support (int node_idx, int trans_idx) {
    //if (trans_idx !=0 && node_idx != 0) printf("%d %d\n", node_idx, trans_idx);    
    if (identity[node_idx] != -1) {
        freq[identity[node_idx]]++;
        if (freq[identity[node_idx]] >= min_support && !done[identity[node_idx]]) {
            lk.push_back(ck[identity[node_idx]]);
            done[identity[node_idx]] = 1;        
        }        
        return;
    }
    if (trans_idx >= current_transaction.size()) return;
    
    int ch = current_transaction[trans_idx];
    if (created[nxt[node_idx][ch]]) {
        //printf("OmG\n");        
        increment_support(nxt[node_idx][ch], trans_idx+1); 
    }               
    
    increment_support(node_idx, trans_idx+1); 

    return;
}

int meta[100][6];

int main () {
    time_t St_time = clock(); 
    //freopen("../Dataset/chess.dat", "r", stdin);    
    freopen("../Dataset/mashroom.dat", "r", stdin);
	//freopen("../Dataset/pstar.dat", "r", stdin);
    //freopen("../Dataset/accidents.dat", "r", stdin);
    //freopen("../Dataset/t1.dat", "r", stdin);
    string str;
    while (getline(cin, str)) {
        transactions.push_back(process_input(str));    
    }
    
    //min_support = 1625;
    min_support = (int) (ceil(double(transactions.size()) * MIN_SUPPORT));
    
    int tot_freq = 0;
    int j_1=0, l_1=0;
    
    for (int i=0; i<transactions.size(); i++) {
        for (int j=0; j<transactions[i].size(); j++) {
           // printf("%d ", transactions[i][j]);
        }
        //printf("\n");
    }
    
    map <int, int> unique;
    for (int i=0; i<MX1; i++) {
        if (freq[i] != 0) j_1++;        
        if (freq[i] >= min_support) {
            vector <int> temp;
            temp.push_back(i);
            lk.push_back(temp);
            printf("{%d} ", i);       
        }
    }   
    printf("\n");
    printf("Total elements in frequent %d-itemsets: %d\n", 1, int(lk.size()));        
    tot_freq += lk.size();
    l_1 = lk.size();
    int max_k=1;
    while (lk.size() != 0) {
        lkminus1 = lk;
        j_k = 0;
        c_k = 0;
        ck  = construct_ck();
        if (ck.size() == 0) break;        
                
        printf("Total candidate %d-itemsets: %d\n", int(ck[0].size()), int(ck.size()));        
        
        initialize_support();        
        for (int i=0; i<ck.size(); i++) {
            add_itemset(ck[i], i);
        }

        lk.clear();
        
        for (int i=0; i<transactions.size(); i++) {
            current_transaction = transactions[i];
            increment_support(0, 0);
        }        
        printf("Minimum support count: %d\n", min_support);
        printf("Total number of nodes in trie: %d\n", sz);
        printf("Total number of frequent %d-itemsets: %d\n", int(lk[0].size()), int(lk.size()));        
        //printf("||| List of frequent %d-itemsets |||\n", lk[0].size());
        tot_freq += lk.size();
        
        meta[lk[0].size()][0] = j_k/2;
        meta[lk[0].size()][1] = c_k/2;
        meta[lk[0].size()][2] = lk.size();

        for (int i=0; i<lk.size(); i++) {
            //print(lk[i]);
            //printf("\n");        
        }        
        printf("\n");
        max_k++;
        lkminus1 = lk;
    }
    
    printf("Total number of frequent itemsets: %d\n", tot_freq);
    printf("k\tjoin\tprune\tlk\n");
    printf("L-1\t%d\t%d\t%d\n", j_1, j_1, l_1);
    meta[max_k+1][0] = j_1;
    meta[max_k+1][1] = j_1;
    meta[max_k+1][2] = l_1;    
    for (int i=2; i<=max_k+1; i++) {
        if (i<=max_k) printf("L-%d\t%d\t%d\t%d\n", i, meta[i][0], meta[i][1], meta[i][2]);
        else printf("Total\t%d\t%d\t%d\n", meta[i][0], meta[i][1], meta[i][2]);
        meta[max_k+1][0] += meta[i][0];
        meta[max_k+1][1] += meta[i][1];
        meta[max_k+1][2] += meta[i][2];    
    }   
    time_t Ed_time = clock();

    //double taken = (Ed_time - St_time)/1000000.0;
   // printf("Execution time: %ds\n", taken);    
    return 0;
}
