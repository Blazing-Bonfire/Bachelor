#include <cstdio>
#include <iostream>

class Pos { 
public:
    int x, y; 
    Pos add(Pos b) { 
        return {x + b.x, y + b.y};
    }
};

Pos adj[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //�������ҵ��������
const int maxN = 1010, maxM = 1010; //m �� n ������
int n, m;

//���У�����BFS
class Q {
public:
    int *qx, *qy;
    int qa = 0, qb = 1; //���е�ǰ������ [qa, ..., qb), ע��������qb
    
    Q (int n){
        qx = new int[n];
        qy = new int[n];
    }

    void push(Pos p) { //insert
        qx[qb] = p.x; 
        qy[qb] = p.y;
        qb++;
    }
    Pos pop() { //remove
        qa++; 
        return {qx[qa-1], qy[qa-1]};
    }
    bool empty(){ //�����Ƿ��
        return qa == qb;
    }
};

int main(){
    int** mat = new int*[maxN];
    for (int i = 0; i < maxN; i++){
        mat[i] = new int[maxM];
    }
    bool vis[maxN][maxM]; //�Ƿ��ѷ���
    Q qu(maxN * maxM);
    long long int sum = 0;
    char line[1010];
    char c = '\n';

    //�洢���룬�Ƚϸ�����Ϊ��³����
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++){
        while (c != '1' && c != '0') c = getchar(); //�������з�'1','0'�ַ�
        int j = 0;
        while (c == '1' || c == '0'){
            if (c == '0'){
                mat[i][j] = 0;
                qu.push({i, j});
                vis[i][j] = true;
            }
            else {
                vis[i][j] = false;
                mat[i][j] = 1;
            } 
            c = getchar(); 
            j++;
        }
    }

    while (!qu.empty()){
        Pos cur = qu.pop();
        Pos next;
        // ѭ�����������ĸ�����
        for (int i = 0; i < 4; i++){
            next = cur.add(adj[i]);
            if ((next.x < 0 || n <= next.x || next.y < 0 || m <= next.y) || vis[next.x][next.y]) {
                //��next���Ƕ�ά�����е����� �� �ѷ��ʹ�next
                continue;
            }
            vis[next.x][next.y] = true;                 //��Ϊ�ѷ���
            mat[next.x][next.y] = mat[cur.x][cur.y] + 1; //����+1
            sum += mat[next.x][next.y];                 //������ܲ���
            qu.push(next);                              //�ƽ�����
        }
    }
    printf("%d\n", sum);

    //�ͷſռ�
    for (int i = 0; i < n; i++){
        delete[] mat[i];
    }
    delete[] mat;

    return 0;
}
