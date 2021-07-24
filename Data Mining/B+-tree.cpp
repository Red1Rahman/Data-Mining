#include <bits/stdc++.h>
using namespace std;
int dataInNode = 10,choice;

struct node
{
	int filled;
	string *data;
	node **pointer;
	node *parent;
	bool leaf;
}*newNode = NULL,*root = NULL,*curr = NULL;
map <node *,int> m;
bool cmp(string a, string b)
{
	return a<b;
}
bool cmpPointer(node *a,node *b)
{
	int k = 0;
	if(a!=NULL && b!=NULL){
		for(int i = 0;i<min(a->filled,b->filled);i++)
		{
			if(a->data[0] != b->data[0])
			{
				k = i;
				break;
			}
		}
		return cmp(a->data[k],b->data[k]);
	}
	if(a==NULL)return false;
	return true;
}
int cnt  = 1;
node *makeNode()
{
	newNode = new node;
	if(m.find(newNode)==m.end())
	{
		m[newNode] = cnt;
		cnt++;
	}
	newNode->filled = 0;
	newNode->data = new string[dataInNode+1];
	newNode->pointer = new node *[dataInNode+2];
	newNode->leaf = true;
	newNode->parent = NULL;
	for(int i = 0;i<=dataInNode;i++)
	{
		newNode->pointer[i] = NULL;
	}
	return newNode;
}

void menu()
{
	printf("Press 1 for insert\n");
	printf("Press 2 for print\n");
	printf("Press 3 for search\n");
	printf("Press 0 for quit\n");
}
void splitRoot()
{
	node *leftChild = makeNode();
	node *rightChild = makeNode();
	int mid = (dataInNode)/2;

	for(int i = 0;i<=mid;i++)
	{
		leftChild->data[i] = root->data[i];
		leftChild->filled++;
	}
	for(int i = mid+1;i<=dataInNode;i++)
	{
		rightChild->data[i - mid -1] = root->data[i];
		rightChild->filled++;
	}
	leftChild->parent = root;
	rightChild->parent = root;

	root->filled = 1;
	root->pointer[0] = leftChild;
	root->pointer[1] = rightChild;
	root->data[0] = rightChild->data[0];
	root->leaf = false;
}

void insertInNode(node *curr,string data)
{
	curr->data[curr->filled] = data;
	curr->filled++;
	sort(curr->data,curr->data+curr->filled,cmp);
}
void split(node *cur)
{
	if(cur->leaf == false)
	{
		cout<<"Entering non-leaf"<<endl;
		if(cur->parent == NULL)
		{
			cout<<"Root"<<endl;
			cur->parent = makeNode();
			cur->parent->leaf = false;
			cur->parent->pointer[0] = cur;
			root = cur->parent;
		}
		int mid = (dataInNode)/2;
		node *rightChild = makeNode();
		if(dataInNode%2)mid++;
		for(int i = 0,j = mid+1;j<cur->filled;i++,j++)
		{
			rightChild->data[i] = cur->data[j];
			rightChild->filled++;
		}
		for(int i = 0,j = mid+1;j<=cur->filled;i++,j++)
		{
			rightChild->pointer[i] = cur->pointer[j];
			rightChild->pointer[i]->parent = rightChild;
		}
		cur->parent->pointer[cur->parent->filled+1] = rightChild;
		rightChild->parent = cur->parent;
		cur->filled = mid;
		rightChild->leaf = false;
		insertInNode(cur->parent,cur->data[mid]);
	}
	else
	{
		cout<<"Entering leaf"<<endl;
		if(cur->parent == NULL)
		{
			cout<<"Root"<<endl;
			cur->parent = makeNode();
			cur->parent->leaf = false;
			cur->parent->pointer[0] = cur;
			root = cur->parent;
		}
		int mid = (dataInNode)/2;
		node *rightChild = makeNode();
		for(int i = 0,j = mid+1;j<cur->filled;i++,j++)
		{
			cout<<i<<" "<<j<<endl;
			rightChild->data[i] = cur->data[j];
			rightChild->filled++;
		}
		cur->parent->pointer[cur->parent->filled+1] = rightChild;
		rightChild->parent = cur->parent;
		cur->filled = mid+1;
		insertInNode(cur->parent,cur->data[mid+1]);
	}
	if(curr->parent!=NULL)
		sort(curr->parent->pointer,curr->parent->pointer+curr->parent->filled+1,cmpPointer);
}
vector <node *> level[10005];
int markLevel[1005];
void print(node *curr)
{
	for(int i = 0;i<1005;i++)level[i].clear();
	queue <node*> q;
	if(root!=NULL){
		q.push(root);
		markLevel[m[root]] = 0;
	}
	curr = root;
	while(!q.empty())
	{
		curr = q.front();
		level[markLevel[m[curr]]].push_back(curr);
		q.pop();
		for(int i = 0;i<= curr->filled;i++)
		{
				if(curr->pointer[i]!=NULL)
				{
					markLevel[m[curr->pointer[i]]] = markLevel[m[curr]]+1;
					q.push(curr->pointer[i]);
				}
		}
	}
	for(int i = 0;i<1005;i++)
	{
		if(level[i].size()==0)break;
		for(unsigned int j = 0;j <level[i].size();j++){
			curr = level[i][j];
			cout<<m[curr]<<" :";
			for(int i = 0;i< curr->filled;i++)
			{
				cout<<"["<<m[curr->pointer[i]]<<"]"<<curr->data[i];
			}
			cout<<"["<<m[curr->pointer[curr->filled]]<<"]";
			cout<<"\t";
		}
		printf("\n\n");
	}
	cout<<"\n\n";
}
void insert(string data)
{
	curr = root;
	int find;
	if(curr == NULL)
	{
		root = makeNode();
		curr = root;
		printf("Number of filled nodes after insertion %d\n",curr->filled);
		insertInNode(curr,data);
		return;
	}
	else if(curr->leaf == true && curr->filled >= dataInNode)
	{
		insertInNode(curr,data);
		splitRoot();
		return;
	}
	else
	{
		while(curr->leaf == false)
		{
		    printf("Searching for leaf\n");
			find = 0;
			if(curr->data[0]<=data)
			{
				for(int i = 0;i<(curr->filled);i++)
				{
					find = 1;
					if(data>= curr->data[i] && (i+1 == curr->filled ||curr->data[i+1]>data))
					{
						find = i + 1;
						break;
					}
				}
			}
			else find = 0;
			printf("Changing current node with pointer no %d with no overflow\n",find);
			curr = curr->pointer[find];
		}
		if(curr->filled < dataInNode)
		{
			insertInNode(curr,data);
			printf("Number of filled nodes after insertion %d\n",curr->filled);
		}
		else
		{
			insertInNode(curr,data);
			while(curr->filled > dataInNode)
			{
				//print(root);
				printf("Node overflow\n");
				split(curr);
				curr = curr->parent;
			}

            printf("Hi\n");
		}
	}

}
bool search(string data)
{
	int find;

	curr = root;
	while(curr->leaf == false)
		{
			find = 0;
			if(curr->data[0]<=data)
			{
				for(int i = 0;i<(curr->filled);i++)
				{
					find = 1;
					if(data>= curr->data[i] && (i+1 == curr->filled ||curr->data[i+1]>data))
					{
						find = i + 1;
						break;
					}
				}
			}else find = 0;
			printf("Changing current node with pointer no %d\n",find);
			curr = curr->pointer[find];
		}
		for(int i = 0;i<(curr->filled);i++)
		{
			if(curr->data[i] == data)
			{
				printf("Found in node no %d\n",i);
				return true;
			}
		}
		return false;

}
int main()
{
	printf("Data limit in a node :\n");
	int nPointers;
	scanf("%d",&nPointers);
	dataInNode = nPointers-1;

	string data;
	int n;
	while(true)
	{
		menu();
		scanf("%d",&choice);
		if(choice == 0)break;
		if(choice == 1)
		{
			getchar();
			printf("Input string: ");
			getline(cin,data);
			insert(data);
		}
		else if(choice == 2)
		{
			print(root);
		}
		else if(choice == 3)
		{
			getchar();
			printf("Input String: ");
			getline(cin,data);
			if(search(data))printf("Found\n");
			else printf("Not found\n");
		}
	}
	return 0;
}
