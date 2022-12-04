#include "BPTree.h"
void folder_traversal(BPlusTree* tree,HashMap* map, char* name,int * cnt) {
    DIR* info = opendir(name);
    struct dirent* entry;
    if (NULL != info) {
        while (entry = readdir(info)) { // 디렉토리 안에 있는 모든 파일과 디렉토리 출력          
            if (entry != NULL) {
                int chk = is_dir(entry->d_name);
                char route[256] = "";
                //strncpy(route, info->patt, strlen(info->patt) - 1);// *제거
                //strcat(route, entry->d_name);   //경로 재설정
                strncpy(route, name, strlen(name));
                strcat(route, "/");   //경로 재설정
                strcat(route, entry->d_name);
                if (chk == 0) {
                    folder_traversal(tree,map, route,cnt);
                }
                else if (chk > 0) {
                    int size = strlen(entry->d_name) - chk;
                    char* fName = (char*)malloc(size+1);
                    memset(fName, 0, size + 1);
                    strncpy(fName, entry->d_name, size);
                    int fNum = hashFunction(atof(fName));
                    fNum = map_add(map, fName, fNum);
                    if (fNum==0) //해시에 fName이 존재할경우
                        continue;
                    file_read(tree, route, fNum);   //파일 출력
                    free(fName);
                    *cnt = *cnt + 1;
                    if ((*cnt) % 100 == 0) {
                        printf("%d Files Compelete! \n", *cnt);
                        //printf(" - %s\n", route);
                    }
                }
            }
        }
        closedir(info);
    }
    else {
        printf("파일 경로명을 다시 확인해주세요\n");
        return;
    }
}

void file_read(BPlusTree* tree, char* route, int fNum) {
    int size = 1024;
    wchar_t line[1024];
    FILE* fp = fopen(route, "rt,ccs=UTF-8");
    setlocale(LC_ALL, "ko-KR");
    while (!feof(fp)) {
        wchar_t* pLine = fgetws(line, 1024, fp);
        if(pLine!=NULL)
           parse_file(tree, pLine, fNum);
    }
    fclose(fp);
}

void parse_file(BPlusTree* tree, wchar_t* file, int fNum) {
    wchar_t* removeStr =L" \n!\"#$%&'()*+,-./:;<>=?@[\\]^_'{|}~"; //아스키코드에 없는 특수기호가 존재하기에 반복문으로 일일이 찾는다
    wchar_t* next;
    wchar_t* ptr = wcstok(file, removeStr,&next);
    while (ptr){
        insert_key(tree, ptr, fNum);
        ptr = wcstok(NULL, removeStr,&next);
    }
}



