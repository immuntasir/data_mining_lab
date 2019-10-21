#include <bits/stdc++.h>
using namespace std;


int MOD; //MOD value for hash function
int bucket_max_size; //maximum bucket size possible 
int number_of_bit;//number of bit to compare

//key Type to work with
struct Key_Type{
	int id; //integer is key type here
};

//Bucket / bin
struct Bucket{
	int local_bit; //local bit 
	vector<Key_Type>key;//value to save
	struct Bucket *next_bucket;
};


int global_bit; //global bit to express how many bits are needed to represent main table
vector<Bucket*>bucket_address_table; 
deque<deque<int>>corresponding_bit_address;




//hash function
int hash_function(Key_Type object){
	int remainder = object.id%MOD;
	return remainder;
}

void init(){
	//variable start 
	global_bit=1;
	//memory allocate
	bucket_address_table.push_back(new Bucket());
	bucket_address_table.push_back(new Bucket());
	//complete for bucket 0
	bucket_address_table[0]->local_bit=1;
	bucket_address_table[0]->next_bucket= NULL;
	//complete for bucket 1
	bucket_address_table[1]->local_bit=1;
	bucket_address_table[1]->next_bucket = NULL;
	//address making completeness
	deque<int>temp;
	temp.push_back(0);
	corresponding_bit_address.push_back(temp);
	temp[0]=1;
	corresponding_bit_address.push_back(temp);
	return;
}

void bucket_overflow_chain(struct Bucket *target,Key_Type object){
	if((int)target->key.size()<bucket_max_size) {
		target->key.push_back(object);
		return;
	}
	else {
		if(target->next_bucket == NULL) {
			Bucket *one = new Bucket();
			one->local_bit = target->local_bit;
			target->next_bucket = one;
		}
		bucket_overflow_chain(target->next_bucket,object);
	}
	return;
}

void insertion(Key_Type object){
	int rem = hash_function(object);
	int row = rem>>(number_of_bit-global_bit);
	struct Bucket *target = bucket_address_table[row];
	if((int)target->key.size()<bucket_max_size) {
		target->key.push_back(object);
		return;
	}
	else{
		
		//if multiple address is taking the same place first split that
		vector<int>same_place;
		for(int i=0;i<(int)bucket_address_table.size();i++){
			if(bucket_address_table[i] == target){
				same_place.push_back(i);
			}
		}
		if(same_place.size() > 1) {
			//just local space elaborate
			Bucket *one = new Bucket();
			one->local_bit=target->local_bit+1;
			one->next_bucket=NULL;
			Bucket *two = new Bucket();
			two->local_bit=target->local_bit+1;
			two->next_bucket = NULL;
			for(int i=0;i<(int)target->key.size();i++){
				if(i==0){
					one->key.push_back(target->key[0]);
				}
				else{
					rem = hash_function(target->key[i]);
					int choice = rem>>(number_of_bit-one->local_bit);
					if(choice == (hash_function(one->key[0])>>(number_of_bit-one->local_bit))){
					one->key.push_back(target->key[i]);
				}
				else{
					two->key.push_back(target->key[i]);
				}
			}
		  }
		  int base = hash_function(one->key[0])>>(number_of_bit-one->local_bit);
			for(int i=0;i<(int)same_place.size();i++){
				int v=same_place[i];
				if((v>>(global_bit-one->local_bit)) == base){
					bucket_address_table[v]=one;
				}
				else{
					bucket_address_table[v]=two;
				}
			}
			insertion(object);
		}
		else if(same_place.size() == 1 && global_bit == number_of_bit) {
			//need to create chain
			bucket_overflow_chain(target,object);
			return;
		}
		else{
			//need to elaborate
			vector<Bucket*>new_list;
			global_bit++;
			for(int i=0;i<(int)bucket_address_table.size()*2;i++){
				int idx = (i>>1);
				new_list.push_back(bucket_address_table[idx]);
			}
			bucket_address_table = new_list;
			vector<int>change_row;
			for(int i=0;i<(int)bucket_address_table.size();i++){
				if(bucket_address_table[i] == target){
					change_row.push_back(i);
					bucket_address_table[i]= NULL;
				}
			}
			Bucket *one = new Bucket();
			one->local_bit=target->local_bit+1;
			one->next_bucket = NULL;
			Bucket *two = new Bucket();
			two->local_bit=target->local_bit+1;
			two->next_bucket = NULL;
			//division
			for(int i=0;i<(int)target->key.size();i++){
				if(i==0){
					one->key.push_back(target->key[0]);
				}
				else{
					rem = hash_function(target->key[i]);
					int choice = rem>>(number_of_bit-one->local_bit);
					if(choice == (hash_function(one->key[0])>>(number_of_bit-one->local_bit))){
					one->key.push_back(target->key[i]);
				}
				else{
					two->key.push_back(target->key[i]);
				}
			}
		  }
			int base = hash_function(one->key[0])>>(number_of_bit-one->local_bit);
			for(int i=0;i<(int)change_row.size();i++){
				int v=change_row[i];
				if((v>>(global_bit-one->local_bit)) == base){
					bucket_address_table[v]=one;
				}
				else{
					bucket_address_table[v]=two;
				}
			}
			insertion(object);
		}
	}
	return;
}

map<struct Bucket*,int>M;

string make_string(int number){
	string s="";
	while(number != 0) {
		int rem = number % 2;
		number /= 2;
		char ch = (char)(rem+'0');
		s=ch+s;
	}
	while((int)s.size()<global_bit) {
		s="0"+s;
	}	
	return s;
}


void print_function(){
	M.clear();
	for(int i=0;i<(int)bucket_address_table.size();i++){
		cout<<"Address: "<< i<<" (" <<make_string(i) << ") : ";
		if(M[bucket_address_table[i]] == 0) {
			M[bucket_address_table[i]]=1;
			cout<<"Hash prefix = ( "<<bucket_address_table[i]->local_bit<<" ) Value = ( ";
			for(int j=0;j<(int)bucket_address_table[i]->key.size();j++){
				cout<<bucket_address_table[i]->key[j].id<<" ";
			} 
			cout<<" ) " ;
			struct Bucket *target = bucket_address_table[i];
			while(target->next_bucket != NULL) {
				cout<< "( ";
				for(int j=0;j<(int)target->next_bucket->key.size();j++){
					cout<<target->next_bucket->key[j].id<<" ";
				}
				cout<<" ) ";
				target = target->next_bucket;
			}
			cout << endl;
		}
		else{
			cout<<"--------"<<endl;
		}
	}
	M.clear();
	return;
}

bool search(Key_Type object){
	int rem = hash_function(object);
	int row = rem>>(number_of_bit-global_bit);
	if((int)(bucket_address_table.size()-1)<row) return false;
	struct Bucket* target = bucket_address_table[row];
	while(true){
		for(int i=0;i<(int)target->key.size();i++){
			if(target->key[i].id == object.id) {
				return true;
			}
		}
		if(target->next_bucket != NULL) {
			target=target->next_bucket;
		}
		else{
			return false;
		}
	}
}

Bucket *first_bucket;


void print_menu () {
    cout<< "__________________________________" << endl;
    cout<<"1. Insert"<<endl;
	cout<<"2. Print"<<endl;
	cout<<"3. Delete"<<endl;
	cout<<"4. Search"<<endl;
	cout<<"0. Exit"<<endl;
	cout<< "__________________________________" << endl;
}
void process(){
	Key_Type object;
	int total_input = 0;
	first_bucket = new Bucket();
	first_bucket->local_bit = 0;
	bool moved=false;
	while(1){
		print_menu();
		int choice;
		scanf("%d",&choice);
		if(choice == 1) {
			cout<<"Enter the number: ";
			int number;
			cin>>number;
			object.id=number;
			
			total_input++;
			if(total_input<=bucket_max_size) {
				first_bucket->key.push_back(object);
			}
			else{
				if(moved == false) {
					//means the keys which remains in empty address hasn't been moved
					moved=true;
					for(int i=0;i<(int)first_bucket->key.size();i++){
						insertion(first_bucket->key[i]);
					}
					insertion(object);
				}
				else{
					insertion(object);
				}
			}
		}
		else if(choice == 2) {
			if(total_input<=bucket_max_size) {
				cout<<"Address: None" ;
				cout<<"Hash prefix = ( " << 0 <<" ) Value = ( ";
				for(int i=0;i<(int)first_bucket->key.size();i++){
					cout<<first_bucket->key[i].id<<" ";
				}
				cout<<" ) " << endl;
			}
			else{
				print_function();
			}
		}
		else if (choice == 3) {
		    printf("Not implemented yet.\n");
		}
		else if(choice == 4) {
			printf("Search not implemented yet.\n");
		}
		else if(choice == 0) {
			break;
		}
	}
}

int main(void){
	printf("Max number of bits: ");
	cin>>number_of_bit;
	MOD = 1<<number_of_bit;
	
	printf("Size of bucket: ");
	cin>>bucket_max_size;
	
	init(); 
	process();
	
	return 0;
}

