#include "utility.h"



void writeFile();
void read();

std::vector<std::pair<string, int >> data;
std::vector<std::pair<string, std::string >> code_table;
std::vector<std::pair<string, std::string >> outData;


//Node Content
struct Node
{
	string* charr;  
	int freqt; 
	struct Node* left_child;
	struct Node* right_child;
};

//Create New Node. 
struct Node* newNode(string r, int f)
{
	struct Node* temp = (struct Node*) malloc(sizeof(struct Node));	
    temp->charr = &r;
	temp->freqt = f;
	temp->left_child = NULL; // left_child;
	temp->right_child = NULL; // right_child;
	
	return temp;
}

//MainHeap Struct
struct MainHeap
{
	int heapsize;
	struct Node **array;
};

//Create New MainHeap (Trivial One). 
struct MainHeap* newMainHeap(string C[], int C_size, int freq[])
  {

	struct MainHeap* temp = (struct MainHeap*) malloc(sizeof(struct MainHeap));
	temp->heapsize = C_size;
	temp->array = (struct Node**)malloc(temp->heapsize * sizeof(struct Node*));

	for (int i = 0; i < C_size; ++i)
	{
		//cout << "\n" <<freq[i]<<" \n  "<<C[i];
		//temp->array[i] = newNode(C[i], freq[i]);
		struct Node* Ntemp = (struct Node*) malloc(sizeof(struct Node));
		Ntemp->freqt = freq[i];
		Ntemp->left_child = NULL; // left_child;
		Ntemp->right_child = NULL; // right_child;
		Ntemp->charr = &C[i];
		//cout << *(Ntemp->charr)<<endl;
		temp->array[i] = Ntemp;
	}	
	return temp;
}

//MainHeapify
void MainHeapify(int i, MainHeap* temp)
{
	int samllest;
	if ((2 * i + 1) < temp->heapsize && temp->array[(2 * i + 1)]->freqt < temp->array[i]->freqt)
	{
		samllest = (2 * i + 1);
	}
	else
	{
		samllest = i;
	}
	
	if ((2 * i + 2) < temp->heapsize && temp->array[(2 * i + 2)]->freqt < temp->array[samllest]->freqt)
	{
		samllest = (2 * i + 2);
	}

	if (samllest != i)
	{
		struct Node* tnode = *(&temp->array[i]);
		*(&temp->array[i]) = *(&temp->array[samllest]);
		*(&temp->array[samllest]) = tnode;
		//cout << &(tnode->freqt) << endl;
		MainHeapify(samllest, temp);
	}
	
}
//Build MainHeap
struct MainHeap* BuildMainHeap(string C[], int C_size, int freq[])
{
	struct MainHeap *temp = newMainHeap(C, C_size, freq);
	//cout << (&temp->array[0]->charr);
	int i = floor(C_size / 2.0);
	for (; i >= 0; --i)
	{	
		MainHeapify(i, temp);
	}
	return temp;
}

//ExtractMin
struct Node *ExtractMin(MainHeap* temp)
{
	struct Node *min = (struct Node*) malloc(sizeof(struct Node));
	if (temp->heapsize == 0)
	{
		cout << "heap underflow\n";
	}
	else
	{
		min = temp->array[0];
		int x = temp->heapsize - 1;
		temp->array[0] = temp->array[x];
		temp->heapsize = x;
		MainHeapify(0, temp);	
	}
	return min;
}

void InsertMinHeap(struct MainHeap* root, struct Node *NewInternal)
{
	/*
	root->heapsize = root->heapsize + 1;
	root->array[root->heapsize -1] = NewInternal;
	MainHeapify(0, root);
	*/
	root->heapsize = root->heapsize + 1;
	int i = root->heapsize - 1;
	while (i && NewInternal->freqt < root->array[(i - 1) / 2]->freqt)
	{
		root->array[i] = root->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	root->array[i] = NewInternal;
}

void traverse(struct Node* root, string code = "")
{
	if (root->left_child)
	{
		traverse(root->left_child, code + '0');
		traverse(root->right_child, code + '1');
	}
	else
	{
		//create code table
		//cout << endl<<*(root->charr) << ":" << code << endl;
		code_table.push_back(make_pair(*(root->charr), code));
	}
}

void Huffman(string C[], int C_size, int freq[])
{
	struct MainHeap* root = BuildMainHeap(C, C_size, freq);
	struct Node *leftChild ;
	struct Node *RightChild ;
	struct Node *NewInternal;	

	for (int i = (root->heapsize); i >1; --i)
	{
		leftChild = (struct Node*) malloc(sizeof(struct Node));
		RightChild = (struct Node*) malloc(sizeof(struct Node));
		NewInternal = (struct Node*) malloc(sizeof(struct Node));
		
		leftChild = ExtractMin(root);
		RightChild = ExtractMin(root);				
		NewInternal->charr = '\0';
		NewInternal->freqt = (leftChild->freqt) + (RightChild->freqt);
		NewInternal->left_child = leftChild;
		NewInternal->right_child = RightChild;
		
		InsertMinHeap(root, NewInternal);
	}

	traverse(ExtractMin(root), "");
	
}

//Read Data
void read()
{
	ifstream openfile;
	string linee, str;
	int num;

	// Open the file and Check if it exists 
	openfile.open("freq.txt");
	if (openfile.fail())
	{
		std::cout << "No data file" << endl;
		exit(1);
	}

	// If file exist in the Dir start reading 
	else
	{
		while (!openfile.eof())
		{
			getline(openfile, linee);
			if (!(linee.empty()))
			{
				//Split the line;
				istringstream iss(linee);
				vector<string> tokens;
				copy(istream_iterator<string>(iss),
					istream_iterator<string>(),
					back_inserter(tokens));
				//Store the str and freq;
				num = atoi(tokens[1].c_str());
				data.push_back(make_pair(tokens[0], num));
			}
		}
	}
	openfile.close();
}


void writeFile()
{
	ofstream myfile;
	myfile.open("codetable.txt");
	for (int i = 0; i < outData.size(); ++i)
	{
		myfile << outData[i].first << " " << outData[i].second << "\n";
	}
	myfile.close();
}

void main()
{
	read();
	string C[127];
	int freq[127];

	for (int i = 0; i < data.size(); i++)
	{
		C[i] = (data[i].first);
		freq[i] =(data[i].second);
		outData.push_back(make_pair(data[i].first, " "));
	}

	//int C_size = sizeof(C) / sizeof(C[0]);
	int C_size = data.size();
	Huffman(C, C_size, freq);

	for (int i = 0; i < code_table.size(); i++)
	{
		cout << code_table[i].first << ":" << code_table[i].second<< endl;
	}


	for (int i = 0; i < code_table.size(); i++)
	{
		for (int j = 0; j < outData.size(); j++)
		{
			if (code_table[i].first == outData[j].first)
				outData[j].second = code_table[i].second;
		}
	}

	cout << endl << "******************************************************"<<endl;
	for (int i = 0; i < outData.size(); i++)
	{
		cout << outData[i].first << ":" << outData[i].second << endl;
	}

	writeFile();
	
		
}