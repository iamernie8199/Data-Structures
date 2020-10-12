#include <iostream>
#include <fstream>

#include <string>
#include <vector>
using namespace std;

class block {
	friend class background;
private:
	int x;
	int shift;
	vector<vector<int>> shape;
	vector<vector<int>> bottom;
public:
	block(int x, int y, vector<vector<int>> shape) :x(x), shift(y), shape(shape) {
		int i = 0;
		int shapelen = sizeof(shape[0]) / sizeof(shape[0][0]);
		for (int j = 0; j < shapelen; j++) {
			while (true) {
				if (shape[i][j] > 0) {
					int tmp[] = { i,j };
					vector<int> v1;
					v1.assign(tmp, tmp + 2);
					bottom.push_back(v1);
					i = 0;
					break;
				}
				else
					i++;
			}
		}

	}
	~block() {
		vector<vector<int>>().swap(shape);
		vector<vector<int>>().swap(bottom);
	};
};

class background {
public:
	background(int m, int n) : h(m), w(n) {
		vector<vector<int>> tmp(m, vector<int>(n));
		matrix = tmp;
	}
	~background() {
		vector<vector<int>>().swap(matrix);
	};
	void out() {
		for (int i = h - 1; i > -1; i--) {
			for (int j = 0; j < w;j++) {
				cout << matrix[i][j];
			}
			cout << endl;
		}
	}
	void refresh() {
		bool check = false;
		vector<int> full(w, 1);
		vector<int> null(w, 0);
		while (true) {
			for (int i = 0;i < h;i++) {
				if (matrix[i] == full) {
					matrix.erase(matrix.begin() + i);
					matrix.push_back(null);
					break;
				}
				else if (i == h - 1) {
					check = true;
					break;
				}
			}
			if (check)
				break;
		}
	}
	bool newblock(block* b) {
		int index = b->x - 1;
		int i = h;
		bool shifted = false;
		bool check = false;
		vector<vector<int>> bottom = b->bottom;
		int bottomsize = bottom.size();
		while (i >= 0) {
			for (int k = 0; k < bottomsize; k++) {
				int below = i - 1 + bottom[k][0];
				int cur = index + bottom[k][1];
				if (below > h - 1) {
					check = true;
					continue;
				}
				else if (matrix[below][cur] > 0) {
					if (b->shift != 0 and !shifted) {
						// 判斷橫移合法性
						if (index + b->shift > w - 1 || index + b->shift + b->shape[0].size() > w - 1)
							return 1;
						for (int n = 1; n < b->shift; n++) {
							if (matrix[i][index + n] > 0)
								return 1;
						}
						// 更新index
						index += b->shift;
						shifted = true;
						break;
					}
				}
			}
		}
	}

private:
	int h;
	int w;
	vector<vector<int>> matrix;
};

int main(int argc, char* argv[]) {
	ifstream Infile;
	Infile.open(argv[0], fstream::in);
	if (!Infile.is_open()) {
		cout << "Error:Failed to open input file." << endl;
		return 1;
	}
	background b = background(7, 5);
	b.refresh();
	b.out();
	Infile.close();
}
