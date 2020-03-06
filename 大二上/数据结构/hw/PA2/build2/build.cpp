#include <cstdio>

//the following class is directly copied from donList library, 
//made by Donny Chan 2019 (��Ӣ����Ҳ)
template<class T>
class ListNode {
public:
	ListNode<T>* prev = nullptr;
	ListNode<T>* next = nullptr;
	T _val;
public:
	ListNode() {}
	ListNode(T val) {
		_val = val;
	}
	T val() { return _val; }
};

template<class T> class List {
private:
	ListNode<T> *head;
	ListNode<T> *tail;
	int _size;
public:
	~List(){
		ListNode<T>* looper = head;
		while (looper != tail){
			looper = looper->next;
			delete looper->prev;
		}
		delete looper;
	}
	List() {
		head = new ListNode<T>;
		tail = new ListNode<T>;
		head->next = tail;
		tail->prev = head;
		_size = 0;
	};
	int size() { return _size; }
	ListNode<T>* getHead() { return head; }
	ListNode<T>* getTail() { return tail; }
	ListNode<T>* getFirst() { return head->next; }
	ListNode<T>* getLast() { return tail->prev; }
	T first() { return head->next->val();  }
	T at(int n) {
		ListNode<T>* ret = this->head->next;
		while (n--){
            if (ret->next == this->tail) break;
			ret = ret->next;
        }
		return ret->_val;
	}
	ListNode<T>* nodeAt(int n) {
		ListNode<T>* ret = head->next;
		while (n--){
			if (ret->next == this->tail) break;
            ret = ret->next;
        }
		return ret;
	}
	T set(T a, int n) {
		nodeAt(n)->_val = a;
		return a;
	}
	T insertB(T a, int n) {
		return insertB(a, nodeAt(n));
	}
	T insertB(T a, ListNode<T>* ptr) {
		ListNode<T>* nodeA = new ListNode<T>(a);
		nodeA->next = ptr;
		nodeA->prev = ptr->prev;
		ptr->prev->next = nodeA;
		ptr->prev = nodeA;
		_size++;
		return a;
	}
	T insertA(T a, int n) {
		return insertB(a, n + 1); //won't overflow because of the existence of tail
	}
	T insertA(T a, ListNode<T>* ptr) {
		return insertB(a, ptr->next); //won't overflow because of the existence of tail
	}
	T remove(ListNode<T>* n) {
		n->prev->next = n->next;
		n->next->prev = n->prev;
		T tmp = n->_val;
		_size--;
		return tmp;
	}
	T remove(int n) {
		return remove(nodeAt(n));
	}
	T pushBack(T a) {
		return insertB(a, tail);
	}
	T pushFront(T a) {
		return insertA(a, head);
	}
	T popFront() {
		return remove(head->next);
	}
	T popBack(){
		return remove(tail->prev);
	}
	int find(T t){
		int k = 0;
		for (ListNode<T>* looper = head->next; looper != tail; looper = looper->next){
			if (looper->val() == t) break;
			k++;
		}
		return k;
	}
	void clear(){
		ListNode<T>* looper = head->next;
		while (looper != tail){
			looper = looper->next;
			delete looper->prev;
		}
		head->next = tail;
		tail->prev = head;
	}
	void show() {
		ListNode<T>* looper = head->next;
		while (looper != tail) {
			printf("%d ", looper->_val);
			looper = looper->next;
		}
		printf("    end\n");
	}
};
// ���ϴ��������Ҹ��˱�д�Ŀ����������Ϊ�˲���ÿ�ζ�ѹ�������ļ���̫�鷳
// ���Կ��Կ�����������һЩ������δ����

const int maxN = 1000050, maxM = 20000; 

// �������³���ı�д
int max(int a, int b){
    return a > b ? a : b;
}

class Node{
public:
    int par = 0; //��int����ʾ���ڵ㣬��Ҫ�õ��ýڵ�ָ��õ���nodes[par]
    int h = 0; // == max(�˽ڵ������߶ȣ��ܵ��е����߶�)
    int size = 0; 
    List<int> ch;  //ͬ������int����ʾ�ڵ㣬����nodes[����ֵ]���õ��ýڵ��ָ��

    //����������������˽ڵ��ʼsize��h����С�ͣ�α���߶ȣ���Ȼ�����µݹ�
    //��ˣ�������ʱ�����ڵ�����������������ܸ���������ʼ��С�͸߶�
    int genSize();
    int genH();

	int getH();
    //get the id of the n'th child
    int getC(int n){
        return ch.at(n); // O(n)
    }

    //for updating size and h, but for h (height), some details has
    //to be taken care of separately in remove() and insert()
    void updateSize(int diff);
    void updateH(int self, int n);
};

//root is by default nodes[1]
Node* nodes[maxN];

//init the size of all nodes after construction
int Node::genSize(){
    size = 1;
    if (ch.size() != 0){
        for (ListNode<int>* i = ch.getFirst(); i != ch.getTail(); i = i->next){
            size += nodes[i->val()]->genSize();
        }
    }
    return size;
}

//init the height of all nodes after construction
int Node::genH(){
    if (ch.size() == 0) {	
		h = 0;
		return 0; 
	}

	//handle youngest son separately
    ListNode<int>* i = ch.getLast();
    nodes[i->val()]->genH();
    
    //loop from second youngest son to oldest son, inclusive
    for (i = ch.getLast()->prev; i != ch.getHead(); i = i->prev){
        //this child's h = max(next child's h, his oldest son's h + 1)
        nodes[i->val()]->h = max(nodes[i->next->val()]->h, nodes[i->val()]->genH());
    }
    return h = nodes[ch.getFirst()->val()]->h + 1;
}

//returns the real height of this node
int Node::getH(){
	if (ch.size() == 0) return 0;
	return nodes[ch.at(0)]->h + 1;
}

//update size of nodes recursively upwards
void Node::updateSize(int diff){
    size += diff;
    if (this != nodes[1]){
        nodes[par]->updateSize(diff);
    }
}

//update height of nodes recursively upwards
//nodes are handled by their parent
void Node::updateH(int self, int n){
    if (n == 1) return;
    
    // node n is the id of the child whose h got updated
    int idx = ch.find(n); // = index of node n
    if (idx < ch.size()){

    }
    if (idx < ch.size()){ // if n is a child (it wasn't removed)
        ListNode<int> *i = ch.nodeAt(idx); 
        nodes[n]->h = nodes[n]->getH();
        if (idx != ch.size() - 1){ //if the updated child is NOT the youngest child
            nodes[n]->h = max(nodes[n]->h, nodes[i->next->val()]->h );
        }

        //update the height of all older brothers, the time complexity will not be more than O(cost)
        for (i = i->prev; i != ch.getHead(); i = i->prev){
            nodes[i->val()]->h = max(nodes[i->val()]->getH(), nodes[i->next->val()]->h);
        }
    }
    if (self != 1)
        nodes[par]->updateH(par, self);
}

//when we know that the following input describes a node, call this method to get that node
//NB: this function will prompt user input in the terminal!
int getNode(){
    int pathLen;
    int cur = 1;
    scanf("%d", &pathLen);
    int* path = new int[pathLen];
    for (int i = 0; i < pathLen; i++){
        scanf("%d", &path[i]);
    }

    for (int i = 0; i < pathLen; i++){
        if (nodes[cur]->ch.size() <= path[i] || path[i] < 0) {
            break;
        }
        cur = nodes[cur]->ch.at(path[i]);
    }
    return cur;
}

//ע���˺�������������
int remove(){
    int rm = getNode(); //the node to be removed

    int idx = nodes[nodes[rm]->par]->ch.find(rm); 
    int par = nodes[rm]->par; //the node that we are going to give a children to
    nodes[par]->ch.remove(idx);
    nodes[par]->updateSize(-nodes[rm]->size);

    if (nodes[par]->ch.size() == 0){ 
        //the removed node is the only child OR oldest child
        nodes[nodes[par]->par]->updateH(nodes[par]->par, par); //start updating h from one generation above
    } else {
        //the removed node has older brothers, so update them
        int startFrom = nodes[par]->ch.at(idx - 1);
        nodes[par]->updateH(par, startFrom); 
    }
    return rm;
}

void insert(int n, int par, int idx){
    if (idx >= nodes[par]->ch.size()){
        nodes[par]->ch.pushBack(n);
    } else {
        nodes[par]->ch.insertB(n, idx);
    }
    nodes[n]->par = par; //THIS WAS WHAT CAUSED MY BUG FOR SO LONG

    //update size and h
    nodes[par]->updateSize(nodes[n]->size);
    //printf("updated size\n");

    nodes[par]->updateH(par, n);
    //printf("updated h\n");
}

int main(){
    int n, m;
    int numC, tmp;
    scanf("%d %d", &n, &m);

	for (int i = 1; i <= n; i++){
		nodes[i] = new Node;
	}

    //�洢����
    for (int i = 1; i <= n; i++){ //�ڵ���Ϊ[1, n]������
        scanf("%d", &numC);
        for (int j = 0; j < numC; j++){
            scanf("%d", &tmp);
            nodes[i]->ch.pushBack(tmp);
            nodes[tmp]->par = i;
        }
    }

    //��ʼ����α���߶Ⱥ͹�ģ
    nodes[1]->genSize();
    nodes[1]->genH();

    int op;
    for (int i = 0; i < m; i++){
        scanf("%d", &op);
        if (op == 0){ 
            //�ƶ�һ���ڵ�
            int rm = remove(); //����� updateH() �� updateSize() 
            int par = getNode(); 
            int idx;
            scanf("%d", &idx);
            insert(rm, par, idx); //Ҳ����� updateH() �� updateSize()
        }
        else if (op == 1){  
            //��ѯ�߶�
            int target = getNode();
            printf("%d\n", nodes[target]->getH());
        }
        else if (op == 2) { 
            //��ѯ��С
            int target = getNode();
            printf("%d\n", nodes[target]->size);
        }
    }

    for (int i = 1; i<= n; i++){
        delete nodes[i];
    }
    
}
