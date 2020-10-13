#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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
		int shapelen = shape[0].size();
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
		vector<vector<int>> tmp(m + 4, vector<int>(n));
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
	bool newblock(block b) {
		int index = b.x - 1;
		int i = h;
		bool shifted = false;
		bool check = false;
		vector<vector<int>> bottom = b.bottom;
		int bottomsize = bottom.size();
		while (i >= 0) {
			for (int k = 0; k < bottomsize; k++) {
				int below = i - 1 + bottom[k][0];
				below = (below >= 0) ? below : 0;
				int cur = index + bottom[k][1];
				if (matrix[below][cur] > 0) {
					if (b.shift != 0 and !shifted) {
						// 判斷橫移合法性
						if (index + b.shift > w - 1 || index + b.shift + b.shape[0].size() - 1 > w - 1)
							return 1;
						for (int n = 1; n <= b.shift; n++) {
							if (matrix[i][index + n] > 0)
								return 1;
						}
						// 更新index
						index += b.shift;
						shifted = true;
						break;
					}
					else if (b.shape.size() + i > h - 1) {
						return 1;
					}
					else {
						for (int z = 0; z < b.shape.size(); z++) {
							for (int x = 0; x < b.shape[0].size(); x++) {
								if (b.shape[z][x] > 0)
									matrix[i + z][index + x] = b.shape[z][x];
							}
						}
						return 0;
					}
				}
				else if (i == 0) {
					if (b.shift != 0 and !shifted) {
						for (int z = 0; z < b.shape.size(); z++) {
							for (int x = 0; x < b.shape[0].size(); x++) {
								if (b.shape[z][x] > 0)
									matrix[i + z][index + b.shift + x] = b.shape[z][x];
							}
						}
					}
					else {
						for (int z = 0; z < b.shape.size(); z++) {
							for (int x = 0; x < b.shape[0].size(); x++) {
								if (b.shape[z][x] > 0)
									matrix[i + z][index + x] = b.shape[z][x];
							}
						}
					}
					return 0;
				}
				else if (k == bottomsize - 1) {
					check = true;
				}
			}
			if (check) {
				i--;
				check = false;
			}
		}
	}
	bool check() {
		vector<int> null(w, 0);
		if (matrix[h] != null)
			return 1;
		else
			return 0;
	}

private:
	int h;
	int w;
	vector<vector<int>> matrix;
};

void build(vector<vector<int>>& vec, string type)
{
	vector<vector<int>> tmp{ { 1, 1, 1 },{ 2, 2, 2 } };
	vec = tmp;
}

int main(int argc, char* argv[]) {
	FILE* solution_file;
	solution_file = fopen("108065515_proj1.final", "w");
	fstream input;
	input.open("3.data", ios::in);

	string line;
	int m, n;
	string type;
	int index, shift;

	vector<vector<int>> blocktype;

	// 建立對照表
	map<string, vector<vector<int>>> block_list;
	block_list["T1"] = { {0,1,0},{1,1,1} };
	block_list["T2"] = { {0,1},{1,1},{0,1} };
	block_list["T3"] = { {1,1,1},{0,1,0} };
	block_list["T4"] = { {1,0},{1,1},{1,0} };
	block_list["L1"] = { {1,1},{1,0},{1,0} };
	block_list["L2"] = { {1,0,0},{1,1,1} };
	block_list["L3"] = { {0,1},{0,1},{1,1} };
	block_list["L4"] = { {1,1,1},{0,0,1} };
	block_list["J1"] = { {1,1},{0,1},{0,1} };
	block_list["J2"] = { {1,1,1},{1,0,0} };
	block_list["J3"] = { {1,0},{1,0},{1,1} };
	block_list["J4"] = { {0,0,1},{1,1,1} };
	block_list["S1"] = { {1,1,0},{0,1,1} };
	block_list["S2"] = { {0,1},{1,1},{1,0} };
	block_list["Z1"] = { {0,1,1},{1,1,0} };
	block_list["Z2"] = { {1,0},{1,1},{0,1} };
	block_list["I1"] = { {1},{1},{1},{1} };
	block_list["I2"] = { {1,1,1,1} };
	block_list["O"] = { {1,1},{1,1} };

	if (!input.is_open()) {
		cout << "Error:Failed to open input file." << endl;
		return 1;
	}
	else {
		input >> m >> n;
		background game = background(m, n);
		while (getline(input, line)) {
			input >> type >> index >> shift;
			if (!type.compare("End")) {
				break;
			}
			cout << type << "\t" << index << "\t" << shift << endl;
			blocktype = block_list[type];
			block b = block(index, shift, blocktype);
			bool e1 = game.newblock(b);
			bool e2 = game.check();
			if (e1 || e2) {
				cout << "GG" << endl;
				break;
			}
			game.out();
			game.refresh();
		}
		input.close();
		game.out();
	}
}

