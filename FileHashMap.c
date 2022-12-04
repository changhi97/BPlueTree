#include "BPTree.h"

int is_dir(char* d_name) {
    char* ext = strrchr(d_name, '.'); //파일명 뒤에서 부터 확장자 가져옴
    if (ext == NULL) {
        return 0; // 확장자가 없으면 폴더를 뜻함
    }
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".txt") == 0) {
        return strlen(ext); // 파일 탐색 시작
    }
    else {
        return -1; // html,txt를 제외한 파일은 처리하지않는다
    }
}

Bucket* create_Bucket(int fNameLen) {
    Bucket* bucket = (Bucket*)malloc(sizeof(Bucket));
    bucket->fileName = (char*)malloc(fNameLen);
    bucket->fileNumber = 0;
    bucket->next = NULL;
    return bucket;
}
HashMap create_HashMap() {
    HashMap map;
    map.buckets = (Bucket*)malloc(sizeof(Bucket)*hashKey+1);
    memset(map.buckets, 0, sizeof(Bucket) * hashKey+1);
    map.currentNum = 0;
    map.allBucketCnt = 0;
    return map;
}

int map_add(HashMap *map, char* fName,int fNum) {
    Bucket* bucket = create_Bucket(strlen(fName));
    memset(bucket->fileName, 0, strlen(fName) + 1);
    strncpy(bucket->fileName, fName, strlen(fName));
    map->allBucketCnt = map->allBucketCnt + 1;
    //printf("%s %d\n", bucket->fileName, bucket->fileNumber);
    Bucket* temp = map->buckets[fNum].next;
    if (temp == NULL) {
        map->currentNum = map->currentNum + 1;
        bucket->fileNumber = map->currentNum;
        map->buckets[fNum].next = bucket;
    }
    else {
        while (temp) {
            if (strcmp(temp->fileName, fName)==0)
                return 0;
            if (temp->next == NULL) {
                map->currentNum = map->currentNum + 1;
                bucket->fileNumber = map->currentNum;
                temp->next = bucket;
                break;
            }
            temp = temp->next;
        }
    }
    return map->currentNum;
}

bool map_contain(HashMap *map,char*fName) {
    Bucket* temp = map->buckets;
    while (temp) {
        if (strcmp(temp->fileName, fName)==0)
            return true;
        temp = temp->next;
    }
    return false;
}

void map_print(HashMap* map) {
    for (int i = 0; i < hashKey; i++) {
        Bucket* temp = map->buckets[i].next;
        if (temp == NULL)
            continue;
        printf("%d ▷  ", i);
        while (temp) {
            printf("%s - %d |", temp->fileName, temp->fileNumber);
            temp = temp->next;
        }
        printf("\n\n\n");
    }

}

int hashFunction(double key) {
    return fmod(key, (double)hashKey) + 1;
}
