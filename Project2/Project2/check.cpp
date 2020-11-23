#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
	ifstream floor, path;
	stringstream strfloor, strpath;
	if (argc != 2) strfloor << "floor.data";
	else strfloor << argv[1];

	floor.open(strfloor.str());
	path.open("final.path");
	
	if (!floor) return 1;
	if (!path) return 1;

	int M, N, B, ri, rj;
	floor >> M >> N >> B;
	//cout << M << ' ' << N << ' ' << B << '\n';

	if (M > 1000) return 1;
	else if (N > 1000) return 1;
	else if (B > 2147483647) return 1;

	char map[1000][1000];
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			floor >> map[i][j];
			if (map[i][j] == 'R') {
				ri = i;
				rj = j;
			}
		}
	}
	//cout << ri << ' ' << rj << endl;
	int curB = B;
	int lasti, lastj;
	int nowi = ri;
	int nowj = rj;
	bool firsttime = true;
	int step;
	int step_count = 0;
	path >> step;
	while (1) {
		lasti = nowi;
		lastj = nowj;

		if (!(path >> nowi >> nowj)) break;
		else step_count++;

		if ((nowi != ri) || (nowj != rj)) {
			curB--;
			if (curB <= 0) {
				cout << "RUN OUT OF BATTERY!!\n";
				return 1;
			}
		}
		else
			curB = B;

		if ((abs(nowi-lasti)+abs(nowj-lastj))>1) {
			cout << "JUMP STEP: (" << lasti << ',' << lastj << ") -> (" << nowi << ',' << nowj << ")!!\n";
			return 1;
		}
		else if (map[nowi][nowj] == '1') {
			cout << "PATH THROUGH WALL : (" << nowi << ',' << nowj << ")!!\n";
			return 1;
		}
		map[nowi][nowj] = 'D';
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] == '0') {
				cout << "SOME ELEMENTS ARE NOE CLEAN YET : (" << i << ',' << j << ")!!\n";
				return 1;
			}
		}
	}
	
	if (step != step_count-1){
		cout << "STEPS WRONG!!" << endl;
		cout << step << " " << step_count-1 << endl;
		return 1;
	}
	cout << "PASS!!CONGRATS!!\n";
	return 0;

}