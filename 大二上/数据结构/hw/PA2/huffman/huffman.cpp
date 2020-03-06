#include <cstdio>

//Huffman ���еĽڵ�
class Node {
public:
    int f = 0, r = 0; //freqency (of the char) and rank (of the node in the tree)
    //NB: rank is also the length of path
    Node *lc = nullptr, *rc = nullptr;
    Node *par = nullptr;

    ~Node(){
        if (lc) delete lc;
        if (rc) delete rc;
    }

    //���������
    void showPath(){
        if (r == 0) return;
        par->showPath();
        printf("%d", isLc() ? 0 : 1);
    }

    //�˽ڵ��Ƿ�һ������
    bool isLc(){
        return par->lc == this;
    }

    void incrR(){
        if (lc && rc) { 
            lc->incrR();
            rc->incrR();
        }
        r++;
    }
};

class HuffTree {
public:
    Node* nodes[26]; //��ʵ���ַ���Ӧ�ģ�Ҷ�ӣ��ڵ�
    Node* heap[30]; //ֱ�������鼴�ɣ���Ϊ�������Ҫ��ܼ�ª
    int heapK = 0; //�ѵĴ�С

    //�ƽ���
    void pushHeap(Node* n){
        for (int i = 0; i < heapK; i++){
            if (n->f > heap[i]->f){
                for (int j = heapK; j > i; j--){
                    heap[j] = heap[j-1];
                }
                heap[i] = n;
                heapK++;
                return;
            }
        }
        heap[heapK++] = n;
    }

    //�ϲ������ڵ�
    void merge(){
        Node* a = heap[heapK-2];
        Node* b = heap[heapK-1];
        Node* tmp = new Node;
        tmp->lc = a;
        tmp->rc = b;
        a->par = tmp;
        b->par = tmp;
        tmp->f = a->f + b->f;
        a->incrR(); //���ϲ��Ľڵ��rank++
        b->incrR();
        tmp->r = 0;
        heapK-=2;
        //�ϲ���������ƽ�����
        pushHeap(tmp);
    }

    //���ÿ���ַ��ı����
    void showAns(){
        int sum = 0;
        for (int i = 0; i < 26; i++){
            sum += nodes[i]->f * nodes[i]->r;
        }
        printf("%d\n", sum);
        for (int i = 0; i < 26; i++){
            if (nodes[i]->f) {
                printf("%c ", 'a' + i);
                nodes[i]->showPath();
                printf("\n");
            }
        }
    }
};

int main(){
    int len = 0; //the length of the string
    int existChar[26] = {}; //all different existing characters stored in a stack
    int existK = 0; //num of different characters existing in the string
    char c = getchar();
    HuffTree ht;

    for (int i = 0; i < 26; i++){
        ht.nodes[i] = new Node;
    }

    //getting input
    while ('a' <= c && c <= 'z'){
        if (ht.nodes[c - 'a']->f == 0) {
            existChar[existK++] = c - 'a';
        }
        ht.nodes[c - 'a']->f++;
        len++;
        c = getchar();
    }

    if (existK == 1){ //if string contains only one type of character
        //ֻ��һ���ַ�ʱ�����������д���
        printf("%d\n", len);
        printf("%c 0", existChar[0] + 'a');
    }
    else {
        for (int i = 0; i < 26; i++){
            if (ht.nodes[i]->f) ht.pushHeap(ht.nodes[i]); //������Ҷ���ƽ�����
        }
        //���� huffman ��
        while (ht.heapK > 1) {
            ht.merge();
        }
        ht.showAns();
    }

    //�ͷſռ�
    delete ht.heap[0];

    return 0;
}
