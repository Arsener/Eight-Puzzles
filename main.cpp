#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
using namespace std;

/*
Number zero represents blank space, the goal status is:
0 1 2
3 4 5
6 7 8
So the number in node[i][j] should be (3 * i + j) and
the place where number n should stay is (n / 3 , n % 3).
The evaluation number h should be calculated in the way descried below:
(1) If node[i][j] == 0, pass;
(2) node[i][j] == n (n != 0), so h[n] = abs(i - n / 3) + abs(j - n % 3);
(3) sum(h[i]), i = 1, 2, ... , 8.
If the current status is:
1 4 0
7 5 2
3 6 8
the evaluation number h is 1 + 1 + 2 + 0 + 1 + 1 + 1 + 1 + 0 = 8.

The second way to calculate h is to count the number (without zero) that is not in its right position.
The evaluation number of the example above is 7.

The third way to calculate h is to make the matrix into a line, for example:
1 4 0
7 5 2
3 6 8
to:
1 4 0 7 5 2 3 6 8
0 1 2 3 4 5 6 7 8
Then calculate the sum of the distance that each number to its right location.
The evaluation number of the example above is 1 + 3 + 2 + 4 + 1 + 3 + 3 + 1 + 0 = 18.

3 6 2 5 1 4 8 7 0
*/

struct Node{
    int node[3][3];
    int h, g, f;
    int father;
};

vector<Node> open;
int minOpenEvaluate = 100;
vector<Node> close;
int minCloseEvaluate = 100;
stack<Node> solutionPath;
vector<Node>::iterator itor;
vector<int> test;
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};
const int METHOD = 2;

void inputNode(Node *n);
void outputNode(Node n);
int getEvaluate(Node n);
int evaluate(Node n);
int evaluate1(Node n);
int evaluate2(Node n);
bool compare(Node a, Node b);
int ifInOpen(Node n);
int ifInClose(Node n);
bool comp(const Node &a, const Node &b);
bool ifNoSolution(Node n);
int getFatherStatus(int fatherIndex);

int main()
{
    Node start;
    inputNode(&start);

    if(ifNoSolution(start)){
        cout << "This status has no solution.\n";
        return 0;
    }

    start.father = -1;
    start.h = getEvaluate(start);
    start.g = 0;
    start.f = start.g + start.h;
    if(start.f < minOpenEvaluate){
        minOpenEvaluate = start.f;
    }

    open.push_back(start);
    int terminateStatus = -2;

    while(!open.empty()){
        Node current = open[0];

        cout<<"---------------------------\n";
        outputNode(current);
        cout << "g:" << current.g << " --- h:" << current.h << endl;

        if(current.h == 0){
            terminateStatus = current.father;
            break;
        }

        itor = open.begin();
        open.erase(itor);
        close.push_back(current);
        if(current.f < minCloseEvaluate){
            minCloseEvaluate = current.f;
        }

        int zeroX, zeroY;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(!current.node[i][j]){
                    zeroX = i;
                    zeroY = j;
                    break;
                }
            }
        }

        int fatherStatus = getFatherStatus(current.father);
        int fatherX = fatherStatus / 10;
        int fatherY = fatherStatus % 10;

        for(int i = 0; i < 4; i++){
            Node tmp;
            int nextZeroX = zeroX + dx[i], nextZeroY = zeroY + dy[i];

            if(fatherStatus >= 0 && nextZeroX == fatherX && nextZeroY == fatherY){
                continue;
            }

            if(nextZeroX < 0 || nextZeroX > 2 || nextZeroY < 0 || nextZeroY > 2){
                continue;
            }

            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    if (i == zeroX && j == zeroY){
                        tmp.node[i][j] = current.node[nextZeroX][nextZeroY];
                    }
                    else if(i == nextZeroX && j == nextZeroY){
                        tmp.node[i][j] = 0;
                    }
                    else{
                        tmp.node[i][j] = current.node[i][j];
                    }
                }
            }

            tmp.g = current.g + 1;
            tmp.h = getEvaluate(tmp);
            tmp.f = tmp.g + tmp.h;

            cout<<"--------\n";
            outputNode(tmp);
            cout << "g:" << tmp.g << " --- h:" << tmp.h << endl;

            int openLoc = ifInOpen(tmp);
            int closeLoc = ifInClose(tmp);
            if(openLoc > -1 && tmp.f < minOpenEvaluate){
                open[openLoc].father = close.size() - 1;
                open[openLoc].f = tmp.f;
                open[openLoc].g = tmp.g;
                open[openLoc].h = tmp.h;
                minOpenEvaluate = tmp.f;
            }
            else if(closeLoc > -1 && tmp.f < minCloseEvaluate){
                tmp.father = close.size() - 1;
                close[closeLoc].father = close.size() - 1;
                close[closeLoc].f = tmp.f;
                close[closeLoc].g = tmp.g;
                close[closeLoc].h = tmp.h;
                minCloseEvaluate = tmp.f;
                open.push_back(tmp);
            }
            else if(openLoc == -1 && closeLoc == -1){
                tmp.father = close.size() - 1;
                open.push_back(tmp);
            }
        }

        sort(open.begin(), open.end(), comp);

    }

    while(terminateStatus != -1){
        solutionPath.push(close[terminateStatus]);
        terminateStatus = close[terminateStatus].father;
    }

    int step = 0;
    while(!solutionPath.empty()){
        cout << "---step" << step++ << ":---\n";
        outputNode(solutionPath.top());
        cout << "g:" << solutionPath.top().g << " --- h:" << solutionPath.top().h << endl;
//        if(solutionPath.top().father != -1){
//            cout << getFatherStatus(solutionPath.top().father) <<endl;
//        }
        solutionPath.pop();
    }

    cout << "---step" << step++ << ":---\n";
    cout << "0 1 2\n3 4 5\n6 7 8\n";

    return 0;
}

void inputNode(Node *n){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cin >> n->node[i][j];
        }
    }
}

void outputNode(Node n){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << n.node[i][j] << ' ';
        }
        cout << endl;
    }
}

bool compare(Node a, Node b){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if (a.node[i][j] != b.node[i][j]){
                return false;
            }
        }
    }

    return true;
}

int ifInOpen(Node n){
    for(int i = 0; i < open.size(); i++){
        if(compare(n, open[i])){
            return i;
        }
    }

    return -1;
}

int ifInClose(Node n){
    for(int i = 0; i < close.size(); i++){
        if(compare(n, close[i])){
            return i;
        }
    }

    return -1;
}

bool comp(const Node &a, const Node &b){
    if (a.f != b.f){
        return a.f < b.f;
    }
    else{
        return a.h < b.h;
    }
}

bool ifNoSolution(Node n){
    int p[8];
    int index = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(n.node[i][j]){
                p[index++] = n.node[i][j];
            }
        }
    }

    int reversePair = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < i; j++){
            if(p[j] > p[i]){
                reversePair++;
            }
        }
    }

    return reversePair % 2;
}

int getFatherStatus(int fatherIndex){
    if(fatherIndex < 0){
        return -1;
    }

    Node father = close[fatherIndex];
    int x = 0, y = 0;
    for(int i = 0; i <3; i++){
        for(int j = 0; j < 3; j++){
            if(!father.node[i][j]){
                x = i;
                y = j;
            }
        }
    }

    return x * 10 + y;
}

int getEvaluate(Node n){
    if (METHOD == 0){
        return evaluate(n);
    }
    else if(METHOD == 1){
        return evaluate1(n);
    }
    else if (METHOD == 2){
        return evaluate2(n);
    }
}

int evaluate(Node n){
    int eval = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j ++){
            if (i == 0 && j == 0){
                continue;
            }
            if (n.node[i][j] != 3 * i + j){
                eval++;
            }
        }
    }

    return eval;
}

int evaluate1(Node n){
    int h = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(n.node[i][j])
                h += abs(n.node[i][j] - 3 * i - j);
        }
    }

    return h;
}

int evaluate2(Node n){
    int h = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (n.node[i][j]){
                h += (abs(i - n.node[i][j] / 3) + abs(j - n.node[i][j] % 3));
            }
        }
    }

    return h;
}
























