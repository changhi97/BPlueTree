#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>   
#include <dirent.h>     
#include <locale.h>
#include <math.h>
#include <wchar.h>

#define degree 300
#define hashKey 543
typedef struct Document {
    int freq;
    int fileNumber;
    struct Document* next;
}Document;

typedef struct Word {
    wchar_t* word;
    int documentLen;
    Document* docuList;
}Word;

typedef struct Node {
    Word** key;   //key값을 저장할 배열
    struct Node** child;  //자식노드를 가르킬 포인터 배열
    int keyLen;    //key값을 저장한 배열의 길이 
    int childLen;    //자식노드를 가르킬 포인터 배열의 길이
    struct Node* next; //리프노드를 리스트형식으로 나타내기위한 포인터
    struct Node* prev; //단방향 리스트리도 중간에 값을 삽입해야 하기떄문에 prev가 필요하다
    bool leaf;      //노드의 리프여부를 판단(값은 리프에만 저장된다)
}Node;

typedef struct BPlusTree {
    struct Node* currentNode;  
    struct Node* head;  
}BPlusTree;

typedef struct Bucket {
    char* fileName;
    int fileNumber;
    struct Bucket* next;
}Bucket;

typedef struct HashMap {
    Bucket* buckets;
    int currentNum;
    int allBucketCnt;
}HashMap;

//BPTREE
BPlusTree create_BPTree();
Node* create_Node();//BPTree를 구성하는 Node를 만드는 함수
bool search(Node* node, wchar_t* data);
Document* search_v2(Node* node, wchar_t* data);//BPTree에 해당 데이터가 있는지 판단하는 함수
Document* search_v3(Node* node, wchar_t* data);//BPTree에 해당 데이터가 있는지 판단하는 함수
void print_leaf(BPlusTree* tree);//BPTree의 leaf노드만 출력하는 함수
void print_all(BPlusTree* tree);//BPTree의 인터널,리프노드를 모두 출력하는 함수
void insert_key(BPlusTree* tree, wchar_t* data,int fNum);//BPTree에 값을 삽입하기전 분할여부를 판단
void insert_leaf(Node* node, wchar_t* data, int fNum);//BPTree에 실제로 값을 삽입하는 함수
void split_child(Node* node_x, int index);//BPTree의 노드를 분할시키는 과정
Word* create_Word(wchar_t* data);
Document* create_Document(int fNum);

//FILE
void file_read(BPlusTree* tree, char* route, int fNum);//파일명을 매개변수로 받아 파일을 읽는 함수
void folder_traversal(BPlusTree* tree,HashMap* map, char* name,int *cnt);//모든 폴더를 순회해서 파일을 가져온다
void parse_file(BPlusTree* tree, wchar_t* file,int fNum);//파일을 단어별로 파싱
int is_dir(char* d_name);//파일, 폴더를 구분하기위한 함수(0 폴더, 1 html txt 파일,-1 예외)

//HASHMAP
HashMap create_HashMap();//HashMap을 만드는 함수
Bucket* create_Bucket(int fNameLen);//HashMap을 구성할 Bucket생성하는 함수
int map_add(HashMap* map, char* fName, int fNum);//HashMap에 추가하는 함수
void map_print(HashMap* map);//HashMap에 있는 요소를 모두 출력하는 함수
int hashFunction(double key);    //해시
