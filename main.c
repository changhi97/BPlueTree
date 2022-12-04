#define _CRT_SECURE_NO_WARNINGS
#include "BPTree.h"
int main() {
    char* root_name = "./news2021/";
    char* test_name = "./test";
    HashMap map = create_HashMap();
    BPlusTree tree = create_BPTree();
    int cnt = 0;
    folder_traversal(&tree,&map,root_name,&cnt);
    printf("=====Make B+Tree Success!=====");
    
    while (true) {
        int mode = 0;
        printf("============================================\n");
        printf("Search Word(1) / B+Tree Print(2) / EXIT(3)\n");
        printf("============================================\n");
        scanf("%d", &mode);
        switch (mode)
        {
        case 1: // Insert
            getchar();
            printf("Input Word :  ");
            wchar_t temp[100];
            wscanf(L"%s",temp);
            Document* result = search_v3(tree.head, temp);
            if (result != NULL) {
                int fre = result->freq;
                int fNum = result->fileNumber;
                wprintf(L"Word Name: %s\n", temp);
                printf("Max Count : %d(file number) Count : %d\n", fNum, fre);
            }
            else {
                printf("Search Fail... Words That Don't Exist!\n");
            }
            
            break;
        case 2: // Insert
            print_leaf(&tree);
            break;
        case 3:
            return 0;
        default:
            break;
        }
        
    }
    printf("=====END!=====");
}


