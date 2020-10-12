#include <iostream>
#include <fstream>

#include <string>
#include <vector>
using namespace std;

class block {
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
		vector<vector<int>> tmp(m, vector<int>(n,1));
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
	b.out();
	cout << "-----------------" << endl;
	b.refresh();
	b.out();
	cout << "-----------------" << endl;
	Infile.close();
}
