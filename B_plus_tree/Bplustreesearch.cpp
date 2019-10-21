#include <bits/stdc++.h>
using namespace std;
int order = 2;
int root = 0;
vector < vector < int > > nodes;
void create_node () {
    vector <int> node;
    node.clear();
    node.push_back(-1);
    nodes.push_back(node);
}
struct retval {
    int flag;
    int left_ptr;
    int right_ptr;
    int val;
    retval(){}
    retval(int _fl, int _l, int _r, int _val){
        flag = _fl;
        left_ptr = _l;
        right_ptr = _r;
        val = _val;
    }
};
void print_node (vector <int> ara) {
    for (int i=0; i<ara.size(); i++) {
        printf("%d | ", ara[i]);
    }
}
vector < int > nodes_level[32];
void dfs (int current_node, int depth) {
    nodes_level[depth].push_back(current_node);
    for (int i=0; i<nodes[current_node].size(); i+=2) {
        if (nodes[current_node][i] != -1)
            dfs(nodes[current_node][i], depth+1);
    }
}
void show_all_nodes () {
    for (int i=0; i<32; i++) nodes_level[i].clear();
    dfs(root, 0);
    for (int i=0; i<32; i++) {
        if (nodes_level[i].size() == 0) break;
        for (int j=0; j<nodes_level[i].size(); j++) {
            print_node(nodes[nodes_level[i][j]]);
            printf("\t");
        }

        printf("\n");
    }
}
retval insert_value_node (int current_node, retval tmp) {
    printf("HERE");
    printf("HERE");
    printf("HERE\n");
    printf("HERE\n");
    printf("HERE\n");
    int fl = 0, pos = -1;
    for (int i=0; i<nodes[current_node].size() && !fl; i+=2) {
        if (nodes[current_node][i] == tmp.left_ptr) {
            fl = 1;
            pos = i+1;
        }
    }
    if (fl) {
            nodes[current_node].insert(nodes[current_node].begin() + pos, tmp.right_ptr);
            nodes[current_node].insert(nodes[current_node].begin() + pos, tmp.val);
    }
    else {
      /*  if (nodes[current_node][0] == -1) {
            if (nodes[current_node].size() % 2 == 0) {
                nodes[current_node].push_back(-1);
            }
        }
        else {
            if (nodes[current_node].size() % 2 == 0) {
                create_node();
                nodes[current_node].push_back(nodes.size()-1);
                printf("Error\n");
            }
        }
        nodes[current_node].push_back(value); */
    }
    if (nodes[current_node].size() > order * 2 + 1) {
        printf("Here\n");
        int split_point = ((order + 1)/2) * 2 + 1;
        int split_val = nodes[current_node][split_point];
        vector <int> tmp;

        for (int i=split_point+1; i<nodes[current_node].size(); i++) {
            tmp.push_back(nodes[current_node][i]);
        }
        nodes[current_node].erase(nodes[current_node].begin() + split_point - 1, nodes[current_node].end());
        nodes.push_back(tmp);
        //nodes[current_node].push_back(nodes.size() - 1);
        return retval(1, current_node, nodes.size()-1, split_val);
    }
    else return retval(0, -1, -1, -1);
}
retval insert_value_node (int current_node, int value, int ptr) {
    int fl = 0, pos = -1;
    for (int i=1; i<nodes[current_node].size() && !fl; i+=2) {
        if (nodes[current_node][i] > value) {
            fl = 1;
            pos = i;
        }
    }
    if (fl) {
        if (ptr == -1) {
            nodes[current_node].insert(nodes[current_node].begin() + pos, -1);
            nodes[current_node].insert(nodes[current_node].begin() + pos, value);
        }
        else {
            nodes[current_node].insert(nodes[current_node].begin() + pos, value);
            nodes[current_node].insert(nodes[current_node].begin() + pos, ptr);
        }
    }
    else {
        if (nodes[current_node][0] == -1) {
            if (nodes[current_node].size() % 2 == 0) {
                nodes[current_node].push_back(-1);
            }
        }
        else {
            if (nodes[current_node].size() % 2 == 0) {
                create_node();
                nodes[current_node].push_back(nodes.size()-1);
                printf("Error\n");
            }
        }
        nodes[current_node].push_back(value);
    }
    if (nodes[current_node].size() > order * 2 + 1) {
        printf("Here\n");
        int split_point = ((order + 1)/2) * 2 + 1;
        int split_val = nodes[current_node][split_point];
        vector <int> tmp;
        if (ptr == -1) {
            tmp.push_back(-1);
            tmp.push_back(nodes[current_node][split_point]);
        }

        for (int i=split_point+1; i<nodes[current_node].size(); i++) {
            tmp.push_back(nodes[current_node][i]);
        }
        nodes[current_node].erase(nodes[current_node].begin() + split_point - 1, nodes[current_node].end());
        nodes.push_back(tmp);
        //nodes[current_node].push_back(nodes.size() - 1);


        return retval(1, current_node, nodes.size()-1, split_val);
    }
    else return retval(0, -1, -1, -1);
}
retval insert_value (int current_node, int value) {
    printf("Number of nodes: %d | root = %d (%d)\n", nodes.size(), root, current_node);
    //print_node(nodes[current_node]);
    printf("%d %d\n", current_node, nodes[current_node].size());
    printf("%d %d\n", current_node, nodes[current_node][0]);
    retval tmp;
    if (nodes[current_node][0] == -1) {
        // This is a leaf node
        tmp = insert_value_node(current_node, value, -1);
        if (tmp.flag == 1) {
            printf("Splitted\n");
            if (current_node == root) {
                create_node();
                root = nodes.size() - 1;
                nodes[root].clear();
                nodes[root].push_back(tmp.left_ptr);
                nodes[root].push_back(tmp.val);
                nodes[root].push_back(tmp.right_ptr);
                printf("New root: %d\n", root);
                //print_node(nodes[root]);
            }
            return tmp;
        }
        else return tmp;
    }
    else {
        int flag = 0;
        int pos = -1;
        for (int i=1; i<nodes[current_node].size() && !flag; i+=2) {
            if (nodes[current_node][i] > value) {
                flag = 1;
                pos = i-1;
            }
        }
        if (flag) {
            tmp = insert_value(nodes[current_node][pos], value);
        }
        else if (nodes[current_node].size() % 2 == 0) {
            printf("I am here.\n");
            if (nodes[current_node].size() % 2 == 0) {
                create_node();
                nodes[current_node].push_back(nodes.size()-1);
            }
            pos = nodes[current_node].size()-1;
            tmp = insert_value(nodes[current_node][pos], value);
        }
        else {
            tmp = insert_value(nodes[current_node][nodes[current_node].size()-1], value);
        }
        if (tmp.flag == 1) {
            printf("SHould instert.\n");
            print_node(nodes[current_node]);
            printf(" %d %d %d\n", tmp.left_ptr, tmp.val, tmp.right_ptr);
            tmp = insert_value_node(current_node, tmp);

        }
    }
    return tmp;
}
int insert_value (int value) {
    retval tmp = insert_value(root, value);
    if (tmp.flag == 1) {
        printf("root should be splitted.\n");
        create_node();
        root = nodes.size() - 1;
        nodes[root].clear();
        nodes[root].push_back(tmp.left_ptr);
        nodes[root].push_back(tmp.val);
        nodes[root].push_back(tmp.right_ptr);
        printf("New root: %d\n", root);
        //print_node(nodes[root]);

    }
}
void show_values() {
    for (int i=0; i<nodes.size(); i++) {
        print_node(nodes[i]);
        printf("\n");
    }
}
int init_tree () {
    nodes.clear();
    vector <int> root_node;
    root_node.clear();
    root_node.push_back(-1);
    root = 0;
    nodes.push_back(root_node);
}
int search_value (int current_node, int val) {
    int retval = 0;
    for (int i=1; i<nodes[current_node].size(); i++) {
        if (nodes[current_node][i] == val) return 1;
        else if (nodes[current_node][i] > val && nodes[current_node][i-1] != -1)
            retval |= search_value(nodes[current_node][i-1], val);
    }
    if (nodes[current_node].size() % 2 != 0 && nodes[current_node][nodes[current_node].size()-1] != -1)
        retval |= search_value(nodes[current_node][nodes[current_node].size()-1], val);
    return retval;
}
int main () {
    printf("Enter the order of the B+ tree: ");
    scanf("%d", &order);
    init_tree();
    bool isRunning = true;
    int choice = -1;
    int input;
    while (isRunning) {
        printf("Enter 1 to add a value.\n");
        printf("Enter 2 to delete a value.\n");
        printf("Enter 3 to print the values.\n");
        printf("Enter 4 to search for a value.\n");
        printf("Enter 0 to exit.\n");
        scanf("%d", &choice);

        if (choice == 1) {
            scanf("%d", &input);
            insert_value(input);
        }
        else if (choice == 2) {

        }
        else if (choice == 3) {
            show_all_nodes();
        }
        else if (choice == 4) {
            printf("Enter the value: ");
            int value;
            scanf("%d", &value);
            if (search_value(root, value)) printf("Found.\n");
            else printf("Not found.\n");
        }
        else {
            isRunning = false;
        }

    }
    return 0;
}
