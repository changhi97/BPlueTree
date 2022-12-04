# 간단한 검색 엔진 구현
문서가 주어졌을 때 문서를 읽어서 그 속에 발생하는 단어를 분리하여 (단어, 문서번호, 빈도수)로 구성된 쌍을 생성한다. 이들을 검색이 용이한 자료구조인 BPlueTree로 구성하여 저장한다.
본 프로그램은 리눅스 환경에서 실행되며 Makefile로 컴파일된다.

# BPlusTree
BPlusTree는  같은 레벨의 모든 키값들이 정렬되어 있고, 같은 레벨의 Sibiling node는 연결리스트 형태로 이어져 있다.

# BPlusTree 특징
1. 데이터 노드의 자료는 정렬되어 있다.
2. 데이터 노드에서는 데이터가 중복되지 않다.
3. 모든 leaf node는 같은 레벨에 있다.
4. leaf node가 아닌 node의 키값의 수는 그 노드의 서브트리수보다 하나가 적다.
5. 모든 leaf node는 연결리스트로 연결되어 있다.

# 파일 설명
## 1. BPTree.c
BPlustTree를 만들기 위한 파일이다.
## 2. file.c
news2021에 있는 파일을 불러와 특수문자를 제외하고 단어별로 파싱을 진행한 후
BPTree에 넣는 과정을 수행하기 위한 파일이다.
## 3. filehashmap.c
news2021에 파일 명을 정수형으로 변환해주는 기능을 한다. 
news2021에 중복되는 파일 명들이 존재하기 때문에, hash를 이용한다.
## 4. main.c
tree와 hashmap을 생성하고 search 기능을 수행하는 파일이다.
## 5. bptree.h
해당 파일에는 tree를 구성하기 위한 BPlusTree, node, word, document 네개의 구조체와 
파일 중복을 방지를 위한 hash를 구성하는 hashmap과 bucket 구조체로 이루어져있다.
BPlusTree 구조체는 Node로 구성되어 있다. 
Node 구조체는 단어와 그 단어가 나타난 문서번호의 리스트를 저장하기 위한 것이다. 
Word구조체는 단어와 문서번호를 저장하기 위한 것이다.
document구조체는 문서 번호와 해당 빈도수를 저장하기 위한 것이다.
hashmap은 buckt구조체로 이루어져 있다.
bucket구조체는 hashmap에 해당하는 단어와 문서번호를 저장하는 것이다.
