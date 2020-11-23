#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>


using namespace std;
ifstream input;
ofstream output;

#define U (r - 1)
#define D (r + 1)
#define L (c - 1)
#define R (c + 1)

struct Node {
	int row;
	int col;
	int weight; // steps needed from root
	Node* parent; // used in shortest-path
	Node(int r, int c, int d) :row(r), col(c), weight(d), parent(0) {}
	Node operator=(Node* n) {
		this->row = n->row;
		this->col = n->col;
		this->weight = n->weight;
		this->parent = n->parent;
		return *this;
	}
};

class Robot {
private:
	Node* root;
	int row, col, battery;
	int** map; // visited
	bool** mapb; // bool map
	bool** mapb_;
	Node*** path;
	stack<Node*> last;
	queue<Node*> footprint;
public:
	Robot(int M, int N, int B, char** input) :row(M), col(N), battery(B) {
		/*
		2: wall
		1: visited
		0: unvisited
		T: reachable
		F: unreachable
		*/
		map = new int* [M];
		mapb = new bool* [M];
		mapb_ = new bool* [M];
		for (int i = 0; i < M; i++) {
			map[i] = new int[N];
			mapb[i] = new bool[N];
			mapb_[i] = new bool[N];
			for (int j = 0; j < N; j++) {
				if (input[i][j] == 'R') {
					map[i][j] = 1;
					mapb[i][j] = true;
					root = new Node(i, j, 0);

					last.push(root);
					footprint.push(root);
				}
				else if (input[i][j] == '1') {
					map[i][j] = 2;
					mapb[i][j] = false;
				}
				else {
					map[i][j] = 0;
					mapb[i][j] = true;
				}
			}
		}
		bfs_tree(root);
	}
	bool deadend(Node* tmp) {
		int r = tmp->row;
		int c = tmp->col;
		if (U >= 0 && !map[U][c]) return false;
		if (D <= row - 1 && !map[D][c]) return false;
		if (L >= 0 && !map[r][L]) return false;
		if (R <= col - 1 && !map[r][R]) return false;
		return true;
	};
	void copymap() {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				mapb_[i][j] = mapb[i][j];
			}
		}
	};
	void print() {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col;j++) {
				cout << map[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}
	Node*** bfs_tree(Node* root) {
		copymap();
		queue<Node*> bfs;
		path = new Node * *[row];
		for (int i = 0; i < row; i++) {
			path[i] = new Node * [col];
			for (int j = 0; j < col; j++) {
				path[i][j] = NULL;
			}
		}
		path[root->row][root->col] = root;
		path[root->row][root->col]->weight = 0;

		bfs.push(root);
		while (!bfs.empty()) {
			Node* cur = bfs.front();
			int r = cur->row;
			int c = cur->col;
			mapb_[r][c] = false;
			bfs.pop();
			if (U >= 0 && mapb_[U][c] == true) {
				Node* up = new Node(U, c, cur->weight + 1);
				mapb_[U][c] = false;
				up->parent = cur;
				path[U][c] = up;
				bfs.push(up);
			}
			if (D <= row - 1 && mapb_[D][c] == true) {
				Node* down = new Node(D, c, cur->weight + 1);
				mapb_[D][c] = false;
				down->parent = cur;
				path[D][c] = down;
				bfs.push(down);
			}
			if (L >= 0 && mapb_[r][L] == true) {
				Node* left = new Node(r, L, cur->weight + 1);
				mapb_[r][L] = false;
				left->parent = cur;
				path[r][L] = left;
				bfs.push(left);
			}
			if (R <= col - 1 && mapb_[r][R] == true) {
				Node* right = new Node(r, R, cur->weight + 1);
				mapb_[r][R] = false;
				right->parent = cur;
				path[r][R] = right;
				bfs.push(right);
			}
		}
		return path;
	};
	void sweep() {
		int b = battery;
		while (!done()) {
			if (b < 0)  break;
			print();
			Node* now = last.top();
			int r = now->row;
			int c = now->col;
			int w = now->weight;

			if (enough(now, b)) {
				Node* next;
				if (U >= 0 && !map[U][c]) {
					next = path[U][c];
					map[U][c] = 1;
					last.push(next);
					footprint.push(next);
					b--;
				}
				else if (D <= row - 1 && !map[D][c]) {
					next = path[D][c];
					map[D][c] = 1;
					last.push(next);
					footprint.push(next);
					b--;
				}
				else if (L >= 0 && !map[r][L]) {
					next = path[r][L];
					map[r][L] = 1;
					last.push(next);
					footprint.push(next);
					b--;
				}
				else if (R <= col - 1 && !map[r][R]) {
					next = path[r][R];
					map[r][R] = 1;
					last.push(next);
					footprint.push(next);
					b--;
				}
				else {
					while (deadend(last.top()) && last.size() > 1) {
						footprint.push(last.top());
						b--;
						last.pop();
					}
				}
			}
			else {
				back(path[now->row][now->col], root);
				b = battery;
				while (!last.empty())
					last.pop();
				last.push(root);
			}
		}
		back(path[last.top()->row][last.top()->col], root);
		cout << "Total: " << steps() << endl;
	};
	bool enough(Node* now, int b) {
		int r = now->row;
		int c = now->col;
		int wayhome = battery;
		if (U >= 0 && !map[U][c])
			wayhome = path[U][c]->weight;
		else if (D <= row - 1 && !map[D][c])
			wayhome = path[D][c]->weight;
		else if (L >= 0 && !map[r][L])
			wayhome = path[r][L]->weight;
		else if (R <= col - 1 && !map[r][R])
			wayhome = path[r][R]->weight;

		if (b - wayhome > 0) return true;
		else return false;
	};
	void back(Node* tmp, Node* root) {
		while (tmp != root) {
			tmp = tmp->parent;
			footprint.push(tmp);
			map[tmp->row][tmp->col] = 1;
		}
	};
	bool done() {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (!map[i][j]) return false;
			}
		}
		return true;
	};
	int steps() { return footprint.size() - 1; }
	void out(ofstream& outFile) {
		while (!footprint.empty()) {
			outFile << footprint.front()->row << " " << footprint.front()->col << endl;
			footprint.pop();
		}
	};
};

int main(int argc, char* argv[]) {
	if (argc != 2)
		input.open("Testcase/2/floor.data", ios::in);
	else {
		stringstream strin;
		strin << argv[1];
		input.open(strin.str());
	}
	output.open("final.path", ios::out);

	if (!input)	return 1;
	if (!output) return 1;

	int M, N, B;
	input >> M >> N >> B;

	if (M > 1000) return 1;
	else if (N > 1000) return 1;
	else if (B > 2147483647) return 1;

	char** map = new char* [M];
	for (int i = 0; i < M; i++) {
		map[i] = new char[N];
		for (int j = 0; j < N;j++) {
			input >> map[i][j];
		}
	}
	Robot robot(M, N, B, map);
	robot.sweep();
	output << robot.steps() << endl;
	robot.out(output);
	return 0;
}