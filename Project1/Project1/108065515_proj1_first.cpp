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
	vector<vector<int>> l;
	vector<vector<int>> r;
public:
	block(int x, int y, vector<vector<int>> s) :x(x), shift(y), shape(s) {
		int shapew = s[0].size();
		int shapeh = s.size();
		// ��
		for (int j = 0, i = 0; j < shapew; j++) {
			while (true) {
				if (s[i][j] > 0) {
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
		// ��
		for (int i = 0, j = 0; i < shapeh; i++) {
			while (true) {
				if (s[i][j] > 0) {
					int tmp[] = { i,j };
					vector<int> v1;
					v1.assign(tmp, tmp + 2);
					l.push_back(v1);
					j = 0;
					break;
				}
				else
					j++;
			}
		}
		// �k
		for (int i = 0, j = shapew - 1; i < shapeh; i++) {
			while (true) {
				if (s[i][j] > 0) {
					int tmp[] = { i,j };
					vector<int> v1;
					v1.assign(tmp, tmp + 2);
					r.push_back(v1);
					j = shapew - 1;
					break;
				}
				else
					j--;
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
	void out(fstream& fout) {
		for (int i = h - 1; i > -1; i--) {
			for (int j = 0; j < w;j++) {
				fout << matrix[i][j];
				cout << matrix[i][j];
				if (j != w - 1) {
					cout << " ";
					fout << " ";
				}
			}
			fout << endl;
			cout << endl;
		}
	}
	int refresh() {
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
				else if (matrix[i] == null || i == h - 1) {
					check = true;
					return i;
				}
			}
		}
	}
	bool newblock(block b, int top) {
		int index = b.x - 1;
		int i = top;
		bool shifted = false;
		bool check = false;
		vector<vector<int>> bottom = b.bottom;
		int bottomsize = bottom.size();
		while (i >= 0) {
			for (int k = 0; k < bottomsize; k++) {
				int below = i - 1 + bottom[k][0];
				below = (below >= 0) ? below : 0;
				int cur = index + bottom[k][1];
				if (cur > w-1) return 1;
				if (matrix[below][cur] > 0) {
					if (b.shift != 0 && !shifted) {
						// �P�_��O�_�W�X���
						if (index + b.shift > w - 1 || index + b.shift + b.shape[0].size() - 1 > w - 1)
							return 1;
						// �P�_��O�_�|�����ê
						// �k
						if (b.shift > 0) {
							for (int n = 1; n <= b.shift; n++) {
								for (int r = 0; r < b.r.size();r++) {
									if (matrix[i + b.r[r][0]][index + n + b.r[r][1]] > 0)
										return 1;
								}
							}
						}
						// ��
						else {
							for (int n = 1; n <= abs(b.shift); n++) {
								for (int l = 0; l < b.l.size();l++) {
									if (matrix[i + b.l[l][0]][index - n + b.l[l][1]] > 0)
										return 1;
								}
							}
						}
						// ��sindex
						index += b.shift;
						shifted = true;
						break;
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
					if (b.shift != 0 && !shifted) {
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


int main(int argc, char* argv[]) {
	fstream input;
	fstream output;
	input.open("108065515_proj1.data", ios::in);
	//input.open(argv[1], ios::in);
	output.open("108065515_proj1.final", ios::out);
	string line;
	int m, n;
	string type;
	int index, shift;

	vector<vector<int>> blocktype;

	// �إ߹�Ӫ�
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
		int top = 0;
		while (getline(input, line)) {
			input >> type >> index >> shift;
			if (!type.compare("End"))
				break;
			//cout << type << "\t" << index << "\t" << shift << endl;
			blocktype = block_list[type];
			block b = block(index, shift, blocktype);
			bool e1 = game.newblock(b, top);
			top = game.refresh();
			bool e2 = game.check();
			if (e1 || e2) {
				cout << "GG" << endl;
				return 1;
			}
			//game.out();
		}
		input.close();
		cout << "Final:" << endl;
		game.out(output);
		output.close();

		return 0;
	}
}

