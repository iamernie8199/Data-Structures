#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
ifstream input;
ofstream output;

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
public:
	Robot(int M, int N, int B, char** input) :row(M), col(N), battery(B) {
		/*
		2: wall
		1: visited
		0: unvisited
		*/
		map = new int* [M];
		for (int i = 0; i < M; i++) {
			map[i] = new int[N];
			for (int j = 0; j < N; j++) {
				if (input[i][j] == 'R') {
					map[i][j] = 1;
					root = new Node(i, j, 0);
				}
				else if (input[i][j] == '1') {
					map[i][j] = 2;
				}
				else {
					map[i][j] = 0;
				}
			}
		}
	}
	bool done() {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (map[i][j] == 0) return false;
			}
		}
		return true;
	};
	bool Deadend(Node* tmp) {
		int r = tmp->row;
		int c = tmp->col;
		if (r - 1 >= 0 && map[r - 1][c] == 0) return false;
		if (r + 1 <= row - 1 && map[r + 1][c] == 0) return false;
		if (c - 1 >= 0 && map[r][c - 1] == 0) return false;
		if (c + 1 <= col - 1 && map[r][c + 1] == 0) return false;
		return true;
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