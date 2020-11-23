#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>


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
	Node* parent; // used in shortest-path
	Node(int r, int c, int d) :row(r), col(c), parent(0) {}
	Node operator=(Node* n) {
		this->row = n->row;
		this->col = n->col;
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
	}
	bool done() {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (!map[i][j]) return false;
			}
		}
		return true;
	};
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
};

int main(int argc, char* argv[]) {
	if (argc != 2)
		input.open("Testcase/1/floor.data", ios::in);
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

	return 0;
}