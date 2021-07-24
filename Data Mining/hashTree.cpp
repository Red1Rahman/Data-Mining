#include <bits/stdc++.h>
using namespace std;
unsigned int size = 3,bitSize=3;

struct local_bucket
{
	vector <int> value;
	int index;
	local_bucket *nextBucket;
	local_bucket(){
		nextBucket = NULL;
	}
}*local;
int globalIndex;
vector < local_bucket* > pointer;



void menu()
{
	printf("Enter 1 to insert a value\nEnter 2 to print\nEnter 0 to exit\n");
}
void insert(int x)
{
	//cout<<"Global Index : "<<globalIndex<<" x : "<<x<<endl;
	int choose = x % (1<<bitSize);
	//cout<<"Choose : "<<choose<<endl;
	choose = choose >> (size - globalIndex);
	//cout<<"Choose : "<<choose<<endl;
	local_bucket *cur = pointer[choose];
	cur->value.push_back(x);
	cout<<x<<" "<<choose<<" "<<cur->value.size()<<endl;
	if(cur->value.size() > size)
	{

		if(globalIndex == cur->index)
		{
			vector < local_bucket* > newpointer;
			vector <int> tmpvalue;
			globalIndex++;
			//cout<<globalIndex<<" "<<size<<endl;
			if(globalIndex > size)
			{
				cout<<cur<<" "<<cur->nextBucket<<"...."<<endl;
				while(cur->nextBucket!=NULL)
				{
					cout<<cur<<" "<<cur->nextBucket<<"...."<<endl;
					cur->value.pop_back();
					cur = cur->nextBucket;
					cur->value.push_back(x);
					if(cur->value.size()<=size)
					{
						break;
					}
				}
				if(cur->value.size()>size){
					cur->value.pop_back();
					cur->nextBucket = new local_bucket();
					cout<<"Setting next bucket to: "<<cur->nextBucket<<endl;
					cur->nextBucket->value.push_back(x);
				}
				globalIndex--;
				return;
			}
			for(int i = 0;i< (1<<globalIndex);i++)
			{
				newpointer.push_back(0);
			}
			for(unsigned int i = 0;i<pointer.size();i++)
			{
				if(i!=choose){
					newpointer[2*i] = pointer[i];
					newpointer[2*i+1] = pointer[i];
				}
				else
				{
					newpointer[2*i] = new local_bucket;
					newpointer[2*i+1]= new local_bucket;
					newpointer[2*i]->index = pointer[i]->index+1;
					newpointer[2*i+1]->index = pointer[i]->index + 1;
					for(unsigned int j = 0;j<pointer[i]->value.size();j++)
					{
						tmpvalue.push_back(pointer[i]->value[j]);
					}

				}
			}
			pointer = newpointer;
			for(unsigned int j = 0 ; j< tmpvalue.size();j++)
			{
				insert(tmpvalue[j]);
			}
		}
		else
		{
			local_bucket *newbucket1 = new local_bucket;
			local_bucket *newbucket2 = new local_bucket;
			newbucket1->index = pointer[choose]->index + 1;
			newbucket2->index = pointer[choose]->index + 1;
			vector <int> tmpValue = pointer[choose]->value;
			pointer[choose]->value.clear();
			int lastPrefix = choose>>(globalIndex - newbucket1->index+1);
			cout<<lastPrefix<<endl;
			pointer[2*lastPrefix] = newbucket1;
			pointer[2*lastPrefix+1] = newbucket2;
			for(unsigned int i = 0;i< tmpValue.size();i++)
			{
				insert(tmpValue[i]);
			}

		}
	}
}
void print()
{
	map <int,local_bucket*> revlb;
	map <local_bucket*,int> lb;
	int cnt = 0;

	cout<<globalIndex<<"|"<<endl;
	for(unsigned int i = 0;i<pointer.size();i++)
	{
		if(lb.find(pointer[i])==lb.end())
		{
			cnt++;
			lb[pointer[i]]=cnt;
			revlb[cnt] = pointer[i];
		}
		cout<<i<<" : "<<lb[pointer[i]]-1<<endl;
	}
	for(int i = 1;i<=cnt;i++)
	{
		if(revlb[i]==0)continue;
		cout<<revlb[i]->index<<" | "<<i-1<<": ";
		for(unsigned int j = 0;j<revlb[i]->value.size();j++)
		{
			cout<<" "<<(revlb[i])->value[j];

		}
		local_bucket *cur = revlb[i];
        while(cur->nextBucket!=NULL)
        {
            cur = cur->nextBucket;
            cout<<"-> ";
            for(unsigned int j = 0;j<cur->value.size();j++)
            {
                cout<<" "<<cur->value[j];
            }
        }
        cout<<endl;
	}
}
int main()
{
	int ch,x;
    printf("Enter bit size: ");
    scanf("%d",&bitSize);
	printf("Enter the bucket size: \n");
	scanf("%d",&size);
	globalIndex  = 0;
	local = new local_bucket;
	local->index = 0;

	pointer.push_back(local);
	while(true)
	{
		menu();
		scanf("%d",&ch);
		if(ch==1)
		{
			scanf("%d",&x);
			insert(x);
		}
		else if(ch ==2)
		{
			print();
		}
		else if(ch==0)
		{
			break;
		}
	}
	return 0;

}
