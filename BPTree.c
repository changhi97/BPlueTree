#include "BPTree.h"

BPlusTree create_BPTree() {
    BPlusTree BPTree;
    BPTree.currentNode = create_Node();
    BPTree.currentNode->leaf = true;
    BPTree.head = BPTree.currentNode;
    return BPTree;
}

Node* create_Node() {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = (Word**)malloc(sizeof(Word*) * (2 * degree - 1));
    node->child = (Node**)malloc(sizeof(Node*) * (2 * degree));//Node*?
    node->keyLen = 0;
    node->childLen = 0;
    node->next = NULL;
    node->prev = NULL;
    node->leaf = false;
    return node;
}

Word* create_Word(wchar_t* data) {
    Word* wordNode=(Word*)malloc(sizeof(Word));
    wordNode->word = (wchar_t*)malloc(wcslen(data)*sizeof(wchar_t));
    wcscpy(wordNode->word, data);
    wordNode->documentLen = 0;
    return wordNode;
}

Document* create_Document(int fNum) {
    Document* docu = (Document*)malloc(sizeof(Document));
    docu->freq = 1;
    docu->fileNumber = fNum;
    docu->next = NULL;
    return docu;
}
void insert_key(BPlusTree* tree, wchar_t* data,int fNum) {
    Node* temp = tree->currentNode;
    if (temp->keyLen == (2 * degree - 1)) {
        Node* newNode = create_Node();
        tree->currentNode = newNode;
        tree->currentNode->leaf = false;
        tree->currentNode->child[tree->currentNode->childLen] = temp;
        tree->currentNode->childLen++;

        split_child(tree->currentNode, 0);
        insert_leaf(tree->currentNode, data,fNum);
    }
    else {  
        insert_leaf(temp, data,fNum);
    }
}


//트리를 재귀호출하면서 리프노드를 찾아가서 key값을 넣는다
//그 과정에서 node의 분할도 같이 발생할수 있다.
void insert_leaf(Node* node, wchar_t* data,int fNum) {
    
    int i = node->keyLen - 1;  //노드의 마지막부분을 의미

    if (node->leaf) {   //노드가 리프일경우 실제 값 삽입
        while (i >= 0) {  //삽입전 같은 단어가 있는지 확인한다.         
            if (wcscmp(data, node->key[i]->word) == 0) {
                Document* docuTemp = node->key[i]->docuList;
                while (docuTemp->next) {
                    if (docuTemp->fileNumber == fNum) {
                        docuTemp->freq++;
                        return;
                    }
                    docuTemp = docuTemp->next;
                }
                docuTemp->next = create_Document(fNum);
                node->key[i]->documentLen++;
                return;
            }
            i--;
        }
        i = node->keyLen - 1;
        //같은 단어가 없으면 새로추가          
        while (i >= 0 && wcscmp(data, node->key[i]->word) < 0) {  
            node->key[i + 1] = node->key[i];
            i--;
        }
        i++;
        node->keyLen++;
        node->key[i] = create_Word(data);
        node->key[i]->documentLen++;
        node->key[i]->docuList = create_Document(fNum);

    }
    else {
        while (i >= 0 && wcscmp(data, node->key[i]->word) < 0) {
            i--;
        }
        i++;
        Node* temp = node->child[i];
        if (temp->keyLen == (2 * degree - 1)) {
            split_child(node, i);
            insert_leaf(node, data,fNum);
        }
        else {
            insert_leaf(node->child[i], data,fNum); 
        }
    }
}

//노드를 분할하는 함수 새로운 노드가 생성된다.
void split_child(Node* nodeX, int index) {
    Node* nodeY = nodeX->child[index];  
    Node* nodeZ = create_Node();

    if (nodeY->leaf) {
        nodeZ->leaf = nodeY->leaf;
        nodeZ->keyLen = degree; //key값을 절반 넘긴다

        // 리스트 연결
        if (nodeY->next != NULL) {
            nodeZ->next = nodeY->next;
            nodeY->next->prev = nodeZ;
        }
        nodeY->next = nodeZ;
        nodeZ->prev = nodeY;

        //분할되는 노드에 키값을 새로운 노드로 이동(기존의 노드의 반만 이동시킨다.)
        for (int j = 0; j < degree; j++) {
            nodeZ->key[j] = nodeY->key[j + degree - 1];
        }
        nodeY->keyLen = degree - 1;

        //키값을 옮기듯이 노드의 포인터도 옮긴다.
        // 다만 포인터배열은 key배열보다 1만큼 크다 
        nodeY->childLen = degree - 1;
        nodeZ->childLen = degree;
        for (int j = 0; j < degree + 1; j++) {
            nodeZ->child[j] = nodeY->child[j + degree - 1];
        }
    }
    else {
        //y와 z는 같은 레벨
        nodeZ->leaf = nodeY->leaf; 
        nodeZ->keyLen = degree - 1;
        nodeY->next = nodeZ;
        nodeZ->prev = nodeY;

        for (int j = 0; j < degree - 1; j++) {
            nodeZ->key[j] = nodeY->key[j + degree];
        }
        nodeY->keyLen = degree - 1;

        nodeY->childLen = degree;
        nodeZ->childLen = degree;
        for (int j = 0; j < degree; j++) {
            nodeZ->child[j] = nodeY->child[j + degree];
        }
    }

    for (int j = nodeX->keyLen - 1; j >= index; j--)
    {
        nodeX->key[j + 1] = nodeX->key[j];
    }
    nodeX->key[index] = nodeY->key[degree - 1];
    nodeX->keyLen++;

    for (int j = nodeX->childLen - 1; j > index; j--)
    {
        nodeX->child[j + 1] = nodeX->child[j];
    }
    nodeX->child[index + 1] = nodeZ;
    nodeX->childLen++;
}


bool search(Node* node, wchar_t* data) {
    int i = 0;
    while (i < node->keyLen) {
        if (wcscmp(node->key[i]->word, data)  >= 0) //같으면 오른쪽
            break;
        i++;
    }
    if (node->leaf && wcscmp(node->key[i]->word, data)==0) {
        return true;
    }
    else {
        if (wcscmp(node->key[i]->word, data)==0) i++;
        if (!node->leaf) {
            search(node->child[i], data);
        }
        else {
            return false;
        }
    }
}


Document* search_v2(Node* node, wchar_t* data) {
    int i = node->keyLen - 1;  //노드의 마지막부분을 의미

    if (node->leaf) {   //노드가 리프일경우 실제 값 삽입
        while (i >= 0) {  //삽입전 같은 단어가 있는지 확인한다.         
            if (wcscmp(data, node->key[i]->word) == 0) {
                Document* docuTemp = node->key[i]->docuList;
                Document* maxWord = docuTemp;
                while (docuTemp) {
                    if (maxWord->freq < docuTemp->freq) {
                        maxWord = docuTemp;
                    }
                    docuTemp = docuTemp->next;
                }
                return maxWord;
            }
            i--;
        }
        return NULL;
    }
    else {
        while (i >= 0 && wcscmp(data, node->key[i]->word) < 0) {
            i--;
        }
        i++;
        search_v2(node->child[i], data);
    }
}
Document* search_v3(Node* node, wchar_t* data) {
    while (!node->leaf) {
        node = node->child[0];
    }
    while (node) {
        for (int i = 0; i < node->keyLen; i++) {
            if (wcscmp(data, node->key[i]->word) == 0) {
                Document* docuTemp = node->key[i]->docuList;
                Document* maxWord = docuTemp;
                while (docuTemp) {
                    if (maxWord->freq < docuTemp->freq) {
                        maxWord = docuTemp;
                    }
                    docuTemp = docuTemp->next;
                }
                return maxWord;
            }
        }
        node = node->next;
    }
    return NULL;

}
void print_leaf(BPlusTree* tree) {
    Node* node = tree->head;
    while (!node->leaf) {
        node = node->child[0];
    }
    while (node) {
        for (int i = 0; i < node->keyLen; i++) {
            wprintf(L"%s | ", node->key[i]->word);
        }
        printf("\n\n\n");
        node = node->next;
    }

}

void print_all(BPlusTree* tree) {
    Node* node = tree->head;
    while (node) {
        Node* temp = node->child[0];
        while (node) {
            for (int i = 0; i < node->keyLen; i++) {
                wprintf(L"%s\n", node->key[i]->word);
            }
            node = node->next;
        }
        printf("\n");
        node = temp;
    }
}






