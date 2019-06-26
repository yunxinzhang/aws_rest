#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
using namespace std;
typedef map<pair<int,int>, set<int> >::iterator CITER;
class Sudoku{
public:
	int matrix[9][9];
	map<pair<int, int>, set<int> > candidate;
	Sudoku(const Sudoku &ss) {
		candidate = ss.candidate;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				matrix[i][j] = ss.matrix[i][j];
			}
		}
	}
	Sudoku(string question) {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				matrix[i][j] = question[i*9+j] - '0';
			}							
		}
		init_candidate();
	}
	void init_candidate() {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (matrix[i][j] == 0) {
					set<int> s;
					for (int k = 1; k <= 9; ++k)s.insert(k);
					candidate.insert(pair<pair<int, int>, set<int> >(pair<int, int>(i, j), s));
				}						
			}				
		}
	}
	void print() {
		cout << "-------------" << endl;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				cout << matrix[i][j] << "\t";																
			}
			cout << endl;												
		}
	}
	bool win() {
		if (candidate.size() == 0)return true;
		bool flag = true;
		for (CITER it = candidate.begin(); it != candidate.end(); ++it) {
			if (it->second.size() > 1){
				return false;
			}																
		}
		return true;
	}
	int easyCheck(){
		for(int i=0; i<9; ++i){
			set<int> s;
			for(int j=0; j<9; ++j){
				if(matrix[i][j] != 0){
					if(s.find(matrix[i][j]) != s.end()){
						return false;
					}
					s.insert(matrix[i][j]);
				}
			}
		}
		for(int i=0; i<9; ++i){
			set<int> s;
			for(int j=0; j<9; ++j){
				if(matrix[j][i] != 0){
					if(s.find(matrix[j][i]) != s.end()){
						return false;
					}
					s.insert(matrix[j][i]);
				}
			}
		}
		for(int i=0; i<3; ++i){
		for(int j=0; j<3; ++j){
			set<int> s;
			for(int ii=0; ii<3; ++ii){
			for(int jj=0; jj<3; ++jj){
				if(s.find(matrix[i*3 +ii][j*3 +jj]) != s.end()){
						cout << "f" << 0 <<endl;
					return false;
				}
				if(matrix[i*3 +ii][j*3+jj] != 0)
				s.insert(matrix[i*3 +ii][j*3 + jj]);
			}
			}
		}
		}
		int cnt0s = 0;
		for(int i=0; i<=4; ++i){
			for(int j =0; j<=4; ++j){
				int cnt0 = 0;
				for(int ii=i; ii<i+5; ++ii){
				for(int jj=j; jj<j+5; ++jj){
					if(matrix[ii][jj] == 0)
						cnt0 += 1;
				}
				}
				if(cnt0 < 2){
					cnt0s += 1;		
				}
			}
		}
		if(cnt0s > 3){
			return -1;
		}
		return true;
	}
	void check(int x,int y) {// delete not possible situation
		
		// 行
		set<int> row;
		for (int i = 0; i < 9; ++i) {
			if (matrix[x][i] != 0) {
				candidate.at(pair<int, int>(x, y)).erase(matrix[x][i]);
			}
			if (matrix[x][i] == 0 && !(y == i)) {
				set<int> & s = candidate.at(pair<int, int>(x, i));
				for (set<int>::iterator it = s.begin(); it != s.end(); ++it )
					row.insert(*it);
			}
		}
		// can not be filled in other places
		set<int> & cs = candidate.at(pair<int, int>(x, y));
		for (set<int>::iterator it = cs.begin(); it != cs.end(); ++it) {
			if (row.find(*it) == row.end()) {
				candidate.at(pair<int, int>(x, y)).clear();
				candidate.at(pair<int, int>(x, y)).insert(*it);
				return;
			}
		}
		
		// 列
		set<int> col;
		for (int i = 0; i < 9; ++i) {
			if (matrix[i][y] != 0) {
				candidate.at(pair<int, int>(x, y)).erase(matrix[i][y]);
			}
			if (matrix[i][y] == 0 && !(x == i)) {
				set<int> & s = candidate.at(pair<int, int>(i, y));
				for (set<int>::iterator it = s.begin(); it != s.end(); ++it )
					col.insert(*it);
			}
		}
		for (set<int>::iterator it = cs.begin(); it != cs.end(); ++it) {
			if (col.find(*it) == col.end()) {
				candidate.at(pair<int, int>(x, y)).clear();
				candidate.at(pair<int, int>(x, y)).insert(*it);
				return;
			}
		}

		// 块
		int bigrow = x / 3;
		int bigcol = y / 3;
		set<int> block;
		for (int i = bigrow * 3; i < bigrow * 3 + 3; ++i) {
			for (int j = bigcol * 3; j < bigcol * 3 + 3; ++j) {
				if(matrix[i][j] != 0)
					candidate.at(pair<int, int>(x, y)).erase(matrix[i][j]);
				if (matrix[i][j] == 0 && !(x==i&&y==j)) {
					set<int> & s = candidate.at(pair<int, int>(i, j));
					for (set<int>::iterator it = s.begin(); it != s.end(); ++it )
						block.insert(*it);
				}
			}
		}
		for (set<int>::iterator it = cs.begin(); it != cs.end(); ++it) {
			if (block.find(*it) == block.end()) {
				candidate.at(pair<int, int>(x, y)).clear();
				candidate.at(pair<int, int>(x, y)).insert(*it);
				return;
			}
		}
		
	}
	// candidate中有set的size=1时，有空确定。确定后会引发其它反应
	bool update() {
		bool flag = false;
		set<pair<int, int> > temp;
		for (CITER it = candidate.begin(); it != candidate.end(); ++it) {
			if (it->second.size() == 1) {
				matrix[it->first.first][it->first.second] = *it->second.begin();
				// 删除无法遍历
				//candidate.erase(pair<int, int>(it.first.first, it.first.second));
				temp.insert(pair<int, int>(it->first.first, it->first.second));
				flag = true;
			}
		}
		for (set<pair<int, int> >::iterator it= temp.begin(); it!=temp.end(); ++it) {
			candidate.erase(*it);
		}
		return flag;
	}
	void reset() {
		do {
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if(matrix[i][j]==0)
						check(i, j);
				}
			}
		} while (update());		
	}
	bool isOK() {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (matrix[i][j] == 0) return false;
				if (matrix[i][j] != 0) {
					if (!isOK(i, j))return false;
				}
			}
		}
		return true;
	}
	bool isOK(int x ,int y) {
		for (int i = 0; i < 9; ++i) {
			if (matrix[x][i] == matrix[x][y] && i != y)return false;
		}
		for (int i = 0; i < 9; ++i) {
			if (matrix[i][y] == matrix[x][y] && i != x)return false;
		}
		int r = x / 3;
		int c = y / 3;
		for (int i = r * 3; i < r * 3 + 3; ++i) {
			for (int j = c * 3; j < c * 3 + 3; ++j) {
				if (matrix[i][j] == matrix[x][y] && !(x == i&&y == j))return false;
			}
		}
		return true;
	}
};

int  solve(Sudoku s, vector<vector<int> > & ans, int layer) {
	ans.clear();
//	cout << "layer>> " << layer <<endl;
	if (s.win() && s.isOK()) {
	//	cout << "result:" << endl;
		//s.print();		
		for(int i=0; i<9; ++i){
			vector<int> v;
			for(int j=0; j<9; ++j){
				v.push_back(s.matrix[i][j]); 
			}
			ans.push_back(v);
		}
		//cout << "copy over" <<endl;
		return true;
	}
	for (CITER it = s.candidate.begin(); it != s.candidate.end(); ++it) {
		for (set<int>::iterator init = it->second.begin(); init != it->second.end(); ++init) {
			Sudoku news(s);
			news.matrix[it->first.first][it->first.second] = *init;
			news.candidate.erase(pair<int, int>(it->first.first, it->first.second));
			news.reset();
			int res = solve(news, ans, layer+1);
			if ( res == 1) {
				return true;
			}
			else if( res == -1){
				return -1;
			}
			if(layer >= 17){
				return -1;// 递归太深
			}
		}
	}
	return false;// 非正常退出
}

/*
int main() {
	Sudoku sdk("matrix.txt");
	sdk.print();
	sdk.reset();
	vector<vector<int> > ans;
	solve(sdk, ans);
}
*/
#include <string>
using namespace std;
extern "C"
int solveSDK()
{
	//cout << 1 <<endl;
		ifstream fin("matrix.txt");
		string question;
		fin >> question;
		fin.close();
		if(question.size() != 81)
			return -1;
	 Sudoku sdk(question);
	 int k = sdk.easyCheck();
	 cout << "easy " << k<<endl;
	 if (k == 0)
		 return -1;
	 else if( k == -1)
		 return -2; //递归太多
	 sdk.reset();
	 string filename = "./sdk_result/" + question ;
	 vector<vector<int> > ans;
	cout <<"start solve " <<12 <<endl;
	int layer = 0;
	 int b = solve(sdk, ans, layer);
	cout << "end solve "<<13 <<endl;
	cout << b <<endl;
	 if(b == 1){

		 ofstream out(filename.c_str());
		 for(int i=0; i<9; ++i){
			 for(int j=0; j<9; ++j)
					 out << ans[i][j];
		 }
	 	out.close();
		return 1;
	 }else if( b == -1){
		 ofstream out("sdk_too_deep", ios::app);
		 out << question << "\n";
		 out.close();
	 	return -3;
	 }
	 return -1;
}
int main(){
	cout << solveSDK() <<endl;
}
