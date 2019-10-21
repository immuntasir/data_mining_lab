#include <bits/stdc++.h>

using namespace std;

#define D(x)            cerr << #x " = " << x << '\n'
#define DBG             cerr << "Hi!" << '\n'

typedef string dt;

int N;
const int MAX_L = 100;

int nodeCount = 0;

int it = 0;

struct node {
	vector <dt> keys;
	vector <struct node*> pointers;
	struct node* parent;
	struct node* leftSibling;
	struct node* rightSibling;
	bool isLeaf;
	int id;
	node() {
        keys.reserve(N);
        pointers.reserve(N+1);
        rightSibling = leftSibling = NULL;
        isLeaf = false;
        id = ++it;
	}
};

struct node* root;

void printNode(node* nd) {
    cout << "(";
    for(int i=0;i<nd->keys.size();i++) {
        cout << " ";
        cout << nd->keys[i];
    }
    cout << ")";
}

vector <node*> nodes[MAX_L];

void dfs(node* nd,int level) {
    if(nd==NULL or nd->id==0) return;
    nodes[level].push_back(nd);
    for(int i=0;i<nd->pointers.size();i++) {
        dfs(nd->pointers[i],level+1);
    }
}

void printTree() {
    puts("\n----------");
    for(int i=0;i<MAX_L;i++) nodes[i].clear();
    dfs(root,0);
    for(int i=0;i<MAX_L;i++) {
        for(int j=0;j<nodes[i].size();j++) {
            if(j) printf("\t");
            printNode(nodes[i][j]);
        }
        if( (int)nodes[i].size() != 0 ) puts("");
    }
    puts("----------\n");
}

node *newLeafnode(dt val,node* par) {
    node* nd = new node();
    nd->isLeaf = true;
    nd->parent = par;
    nd->keys.push_back(val);
    return nd;
}

void insertInLeaf(node* leaf,dt val) {
    leaf->keys.push_back(val);
    sort(leaf->keys.begin(),leaf->keys.end());
}

node* newInternalNode(dt val,node* par,node* lp,node* rp) {
    node* nd = new node();
    nd->isLeaf = false;
    nd->parent = par;
    nd->keys.push_back(val);
    nd->pointers.push_back(lp);
    nd->pointers.push_back(rp);
    return nd;
}

void insertInInternalNode(node* nd,dt val,node* lp,node *rp){
    for(int i=0;i<(int)nd->pointers.size();i++) {
        if( nd->pointers[i] == lp ) {
            nd->keys.insert(nd->keys.begin()+i,val);
            nd->pointers.erase(nd->pointers.begin()+i);
            nd->pointers.insert(nd->pointers.begin()+i,rp);
            nd->pointers.insert(nd->pointers.begin()+i,lp);
            break;
        }
    }
    if( nd->keys.size() == N ) {
        int mid = N/2;
        int rs = mid+1;
        node* nnd = newInternalNode(nd->keys[rs],nd->parent,nd->pointers[rs],nd->pointers[rs+1]);

        nd->pointers[rs]->parent = nnd;
        nd->pointers[rs+1]->parent = nnd;

        for(int i=rs+1;i<nd->keys.size();i++) {
            nnd->keys.push_back(nd->keys[i]);
            nnd->pointers.push_back(nd->pointers[i+1]);
            nnd->pointers[i+1]->parent = nnd;
        }

        if( nd->parent == NULL ) {
            node *cur = newInternalNode(nd->keys[mid],NULL,nd,nnd);
            root = nd->parent = nnd->parent = cur;
        }
        else insertInInternalNode(nd->parent,nd->keys[mid],nd,nnd);


        int del = nd->keys.size() - mid;
        for(int i=0;i<del;i++) {
            nd->keys.pop_back();
            nd->pointers.pop_back();
        }
    }
}

node* findLeaf(dt val) {
    node* cur = root;
    while(cur->isLeaf == false) {
        int id = -1;
        for(int i=0;i<cur->keys.size();i++) {
            if( cur->keys[i] >= val ) {
                id = i;
                break;
            }
        }
        if( id == -1 ) cur = cur->pointers[ cur->pointers.size()-1 ];
        else if( cur->keys[id] == val ) cur = cur->pointers[id+1];
        else cur = cur->pointers[id];
    }
    return cur;
}

void Insert(dt val) {
    if(nodeCount == 0) {
        root = newLeafnode(val,NULL);
        nodeCount++;
        return;
    }
    node* leaf = findLeaf(val);

    insertInLeaf(leaf,val);

    if( leaf->keys.size() == N ) {

        node* nleaf = NULL;
        int cnt = 0;
        for(int i=(N+1)/2;i<leaf->keys.size();i++) {
            if( nleaf == NULL ) nleaf = newLeafnode( leaf->keys[i] , leaf->parent );
            else insertInLeaf( nleaf , leaf->keys[i] );
            cnt++;
        }
        for(int i=0;i<cnt;i++) leaf->keys.pop_back();

        if(leaf->parent==NULL) {
            node* cur = newInternalNode(nleaf->keys[0],NULL,leaf,nleaf);
            root = leaf->parent = nleaf->parent = cur;
        }
        else {
            insertInInternalNode(leaf->parent,nleaf->keys[0],leaf,nleaf);
        }
    }
}


void assignParent(node* nd) {
    if(nd==NULL) return;

    for(int i=0;i<nd->pointers.size();i++) {
        nd->pointers[i]->parent = nd;
    }

    for(int i=0;i<nd->pointers.size();i++) {
        assignParent(nd->pointers[i]);
    }
}

bool search(node* nd,dt val) {
    if(nd == NULL) return false;
    if(nd->isLeaf == true) {
        for(int i=0;i<nd->keys.size();i++) {
            if( nd->keys[i] == val ) return true;
        }
    }
    bool ret = false;
    for(int i=0;i<nd->pointers.size();i++) {
        ret |= search(nd->pointers[i],val);
    }
    return ret;
}


bool search(dt val) {
    node* cur = root;
    while(cur->isLeaf == false) {
        int id = -1;
        for(int i=0;i<cur->keys.size();i++) {
            if( cur->keys[i] >= val ) {
                id = i;
                break;
            }
        }
        if( id == -1 ) cur = cur->pointers[ cur->pointers.size()-1 ];
        else if( cur->keys[id] == val ) cur = cur->pointers[id+1];
        else cur = cur->pointers[id];
    }
    if(cur->isLeaf == true) {
        for(int i=0;i<cur->keys.size();i++) {
            if( cur->keys[i] == val ) {
                cout << val << " at node: " << cur->id << endl;
                return true;
            }
        }
    }

    return false;
}

bool tooFew(node* nd) {
    if(nd->isLeaf) return ( nd->keys.size() < (N/2) );
    else return ( nd->pointers.size() < ((N+1)/2) );
}

void deleteEntry(node* nd,dt val,node* p) {
    //cout << val << "--------------------" << (p?p->id:0) << endl;
    for(int i=0;i<nd->keys.size();i++) {
        if(nd->keys[i] == val){
            nd->keys.erase(nd->keys.begin()+i);
            break;
        }
    }
    for(int i=0;i<nd->pointers.size();i++) {
        if(nd->pointers[i] == p){
            nd->pointers.erase(nd->pointers.begin()+i);
            break;
        }
    }

    if( nd == root and nd->pointers.size() == 1){
        root = nd->pointers[0];
    }
    else if( tooFew(nd) ) {
        bool isPred = true; /// nd is predecessor of nnd
        node* nnd = NULL;

        if(nd->parent==NULL) { cout << "Some error while trying to delete root!" << endl; }

        dt key;
        int keyPos = -1;
        vector <node*> temp = nd->parent->pointers;
        for(int i=0;i<temp.size();i++) {
            if(temp[i]==nd) {
                if(i-1>=0) {
                    keyPos = i-1;
                    key = key = nd->parent->keys[i-1];
                    nnd = temp[i-1];
                    isPred = false;
                }
                else if(i+1<temp.size()) {
                    keyPos = i;
                    key = key = nd->parent->keys[i];
                    nnd = temp[i+1];
                    isPred = true;
                }
            }
        }
        if(nnd==NULL) { cout << "No Sibling!" << endl; }

        bool a = (!nd->isLeaf && nd->pointers.size() + nnd->pointers.size() <= N);
        bool b = (nd->isLeaf and nd->keys.size() + nnd->keys.size() <= N-1);

        //cout << nd->id << " " << nnd->id << endl;
        //cout << nd->parent->id << endl;

        if( a or b ) {
            if (isPred) swap(nd,nnd);

            if(!nd->isLeaf) {
                nnd->keys.push_back(key);
                for(int i=0;i<nd->keys.size();i++) nnd->keys.push_back(nd->keys[i]);
                for(int i=0;i<nd->pointers.size();i++) {
                    nnd->pointers.push_back(nd->pointers[i]);
                    nd->pointers[i]->parent = nnd;
                }
            }
            else {
                /// Any problem ?
                for(int i=0;i<nd->keys.size();i++) nnd->keys.push_back(nd->keys[i]);
                nd->keys.clear();
                nd->pointers.clear();
            }
            deleteEntry(nd->parent,key,nd);
        }
        else {
            //cout << nd->parent->id << " " << nd->id << " " << nnd->id  << endl;
            if(!isPred) {
                if(!nd->isLeaf) {
                    dt save = nnd->keys.back();
                    nnd->keys.pop_back();
                    node* temp = nnd->pointers.back();
                    nnd->pointers.pop_back();
                    nd->keys.insert(nd->keys.begin(),key);
                    nd->pointers.insert(nd->pointers.begin(),temp);
                    temp->parent = nd;
                    nd->parent->keys[keyPos] = save;
                }
                else {
                    printNode(nd->parent);
                    dt save = nnd->keys.back();
                    nnd->keys.pop_back();
                    nd->keys.insert(nd->keys.begin(),save);
                    nd->parent->keys[keyPos] = save;
                }
            }
            else {
                if(!nd->isLeaf) {
                    dt save = nnd->keys[0];
                    nnd->keys.erase(nnd->keys.begin());
                    node* temp = nnd->pointers[0];
                    nnd->pointers.erase(nnd->pointers.begin());
                    nd->keys.push_back(key);
                    nd->pointers.push_back(temp);
                    temp->parent = nd;
                    nd->parent->keys[keyPos] = nnd->keys[0];
                }
                else {
                    dt save = nnd->keys[0];
                    nnd->keys.erase(nnd->keys.begin());
                    nd->keys.push_back(save);
                    nd->parent->keys[keyPos] = nnd->keys[0];
                }
            }
        }
    }
}

void Delete(dt val) {
    node* leaf = findLeaf(val);
    D(leaf->id);
    deleteEntry(leaf,val,NULL);
}
void menu(){
	printf("\t\t\t||| Menu |||\n");
	cout<<"1. Insert"<<endl;
	cout<<"2. Delete"<<endl;
	cout<<"3. Print"<<endl;
	cout<<"4. Search"<<endl;
	cout<<"5. Exit"<<endl;
}
int main() {
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);

    /// V.insert(V.begin()+i,0); /// jake insert korba tar index i hobe
    /// V.erase(V.begin()+i); /// jar index i take delete korba
    printf("Enter the order of the tree: ");
    scanf("%d", &N);
    int cmd;
    dt data;
    while(true) {
        menu();
        cin >> cmd;
        if(cmd==0) {
            return 0;
        }
        else if(cmd==1) {
            if(root!=NULL) assignParent(root);
//            printf("Please enter the data to insert : ");
            cout << "Enter Key to insert: ";
            cin >> data;
            Insert(data);
        }
        else if(cmd==2) {
            //printf("How to delete ? :|\n");
            cout << "Enter Key to delete: ";
            cin >> data;
            if(search(data)) Delete(data);
            else puts("The input is invalid!");
        }
        else if(cmd==3) {
            printTree();
        }
        else if(cmd==4) {
//            printf("Please enter the data to search : ");
            cout << "Enter Key to search: ";
            cin >> data;
            if( search(data) /*or search(root,data)*/ ) {
                cout << "Found in the tree!" << endl;
            }
            else {
                cout << "Not found!" << endl;
            }
        }
    }
    return 0;
}

