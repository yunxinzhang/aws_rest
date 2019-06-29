#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <cstdio>
#include <string>
#include <set>
#include <queue>
#include <ctime>
using namespace std;
class Pos {
public:
	short x;
	short y;
	Pos() :x(-1), y(-1) {}
	Pos(short x, short y) :x(x), y(y) {}
	bool operator<(const Pos &p)const {
		if (x < p.x || (x == p.x&&y < p.y))
			return true;
		else
			return false;
	}
	bool operator==(const Pos &p)const {
		if (x == p.x  && y == p.y)
			return true;
		else
			return false;
	}
};

Pos dir[4] = { Pos(0,1), Pos(0,-1), Pos(1,0), Pos(-1,0) };
int uid = 0;
class Map {
public:
	Map(Map const & m) {
		//id = m.id;
		iter = m.iter;
		maze = m.maze;
		visited = m.visited;
		pre = m.pre;
		mv = m.mv;
		man = m.man;
		boxes = m.boxes;
		nson = m.nson;
		flags = m.flags;
	}
	static short row;
	static short col;
	static char max_left;
	static char max_right;
	static char max_up;
	static char max_down;
	static char flag_max_left;
	static char flag_max_right;
	static char flag_max_up;
	static char flag_max_down;
	static vector<string> res;
	//int id;
	char iter; // step record
	char nson;
	Pos man;
	Map * pre;
	vector<vector<char> > maze; // 记录可以经过的 =, 永远不能经过的 x, 移动箱子后可以经过的 b
	vector<vector<bool> > visited;
	string mv; // move info	
	set<Pos> boxes;
	set<Pos> flags;
	
	static map<set<Pos>, set<Pos> > state; // box --> man
	bool manHasPath(Pos p) {
		visited = vector<vector<bool> >(row, vector<bool>(col, false));
		return go(man, p);
	};
	bool win() {
		return boxes == flags;
	}
	// go使用前需要先初始化visited
	bool isLinked(Pos m, Pos p, set<Pos> newbox) {
		visited = vector<vector<bool> >(row, vector<bool>(col, false));
		return go(m, p, newbox);
	}
	bool go(Pos m, Pos p, set<Pos> newbox) {
		visited[m.x][m.y] = true;
		if (m.x == p.x&&m.y == p.y)return true;
		for (int i = 0; i < 4; ++i) {
			int newx = m.x + dir[i].x;
			int newy = m.y + dir[i].y;
			if (newx >= 0 && newx < maze.size() && newy >= 0
				&& newy < maze[0].size() && maze[newx][newy] != 'x'
				&& (newbox.find(Pos(newx, newy)) == newbox.end())
				&& !visited[newx][newy]) {
				Pos m2(newx, newy);
				if (go(m2, p, newbox))return true;
			}
		}
		return false;
	}
	bool go(Pos m, Pos p) {
		visited[m.x][m.y] = true;
		if (m.x == p.x&&m.y == p.y)return true;
		for (int i = 0; i < 4; ++i) {
			int newx = m.x + dir[i].x;
			int newy = m.y + dir[i].y;
			if (newx >= 0 && newx < maze.size() && newy >= 0
				&& newy < maze[0].size() && maze[newx][newy] != 'x'
				&& (boxes.find(Pos(newx, newy)) == boxes.end())
				&& !visited[newx][newy]) {
				Pos m2(newx, newy);
				if (go(m2, p))return true;
			}
		}
		return false;
	}
	Map(string path, int row = 10, int col = 10) {
		nson = 0;
		pre = NULL;
		//id = 0;
		mv = "";
		iter = 0;
		char c;
		this->row = row;
		this->col = col;
		visited = vector<vector<bool> >(row, vector<bool>(col, false));
		for (int i = 0; i < row; ++i) {
			vector<char> v;
			for (int j = 0; j < col; ++j) {
				c = path[i*col + j];
				v.push_back(c);
				if (c == '$') { // man
					man.x = i;
					man.y = j;
				}
				else if (c == 'f') { // flag
					flags.insert(Pos(i, j));
				}
				else if (c == 'b') { // box
					boxes.insert(Pos(i, j));
				}
				else if (c == 'o') { // flag & box
					flags.insert(Pos(i, j));
					//maze[i][j] = 'b';
					boxes.insert(Pos(i, j));
				}
				else if (c == 'y') { // flag & man
					flags.insert(Pos(i, j));
					//maze[i][j] = 'f';
					man.x = i;
					man.y = j;
				}
			}
			maze.push_back(v);
		}

		for (set<Pos>::iterator it = flags.begin(); it != flags.end(); ++it) {
			maze[it->x][it->y] = '=';
		}
		for (set<Pos>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
			maze[it->x][it->y] = 'b';
		}
		//cout << "man " << man.x << ", "<< man.y <<endl;
		maze[man.x][man.y] = '=';
		max_left = col; max_right = 0; max_up = row; max_down = 0;
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				if (maze[i][j] != 'x') {
					if (max_left > j) {
						max_left = j;
					}
					if (max_right < j) {
						max_right = j;
					}
					if (max_up > i) {
						max_up = i;
					}
					if (max_down < i) {
						max_down = i;
					}
				}
			}
		}
		flag_max_left = flag_max_right = flag_max_up = flag_max_down = 0;
		for (set<Pos>::iterator it = flags.begin(); it != flags.end(); ++it) {
			if (it->y == max_left) {
				++flag_max_left;
			}
			if (it->y == max_right) {
				++flag_max_right;
			}
			if (it->x == max_up) {
				++flag_max_up;
			}
			if (it->x == max_down) {
				++flag_max_down;
			}
		}
	}
	bool inState(set<Pos> s, Pos t) {
		if (state.find(s) == state.end())return false;
		else {
			set<Pos> manPos = state.at(s);
			for (set<Pos>::iterator it = manPos.begin(); it != manPos.end(); ++it) {
				if (isLinked(t, *it, s)) {
					return true;
				}
			}
			return false;
		}
	}

	//限制的条件不够充分	
	bool cantSolve() {
	
		int cnt_box_left = 0, cnt_box_up = 0, cnt_box_right = 0, cnt_box_down = 0;
		
		for (set<Pos>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
			if (it->y == max_left) cnt_box_left++;
			if (it->y == max_right) cnt_box_right++;
			if (it->x == max_up) cnt_box_up++;
			if (it->x == max_down) cnt_box_down++;
		}
		
		if ((cnt_box_down > flag_max_down) || (cnt_box_up > flag_max_up) || (cnt_box_left > flag_max_left) || (cnt_box_right > flag_max_right))return true;

		for (set<Pos>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
			int x1 = it->x + dir[0].x;
			int y1 = it->y + dir[0].y;

			int x2 = it->x + dir[1].x;
			int y2 = it->y + dir[1].y;

			int x3 = it->x + dir[2].x;
			int y3 = it->y + dir[2].y;

			int x4 = it->x + dir[3].x;
			int y4 = it->y + dir[3].y;
			if ((y1 >= col || maze[x1][y1] == 'x') && (x3 >= row || maze[x3][y3] == 'x') && flags.find(*it) == flags.end())return true;
			if ((y1 >= col || maze[x1][y1] == 'x') && (x4 < 0 || maze[x4][y4] == 'x') && flags.find(*it) == flags.end())return true;
			if ((y2 < 0 || maze[x2][y2] == 'x') && (x3 >= row || maze[x3][y3] == 'x') && flags.find(*it) == flags.end())return true;
			if ((y2 < 0 || maze[x2][y2] == 'x') && (x4 <0 || maze[x4][y4] == 'x') && flags.find(*it) == flags.end())return true;

		}
		return false;
	}
	void disp() {
		cout << "----------------" << endl;
		for (set<Pos>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
			cout << it->x << "\t" << it->y << endl;;
		}
	}
	void print(bool charpic = false) {
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				if (charpic && man.x == i && man.y == j) {
					cout << "m " ;
				}
				else
					cout << maze[i][j] << " ";
			}
			cout << endl;
		}
	}
	//~Map(){
	//		pre = NULL;
	//}
};
map<set<Pos>, set<Pos> > Map::state;
short Map::col;
short Map::row;
char Map::max_down;
char Map::max_left;
char Map::max_right;
char Map::max_up;
char Map::flag_max_down;
char Map::flag_max_left;
char Map::flag_max_right;
char Map::flag_max_up;
vector<string> Map::res;

int cntnew = 0, cntdel = 0;
// 广度优先
bool solve2(Map &m) {
	Map::state.clear();
	++m.iter;
	//m.print();
	queue<Map*> list;
	set<Pos> s;
	s.insert(m.man);
	m.state.insert(pair<set<Pos>, set<Pos> >(m.boxes, s));
	if (m.win()) {
		return true;
	}
	if (m.cantSolve()) { // 剪枝
		return false;
	}
	Map * mcopy = new Map(m);
	//mcopy->id = uid++;
	//cout << "new id " << mcopy->id << ", " << mcopy->mv << endl;
	++cntnew;
	list.push(mcopy);
	cout << "搜索中..." << endl;
	char step = 0;
	long tm1, tm2 = clock();
	while (list.size() > 0) {
		//cout << "List >> " << list.size() << endl;

		Map *mm = list.front();
		if (step != mm->iter) {
			tm1 = clock();
			cout << "第" << int(step) << "步" << (tm1 - tm2) << "ms" << endl;
		}
		step = mm->iter;
		list.pop();
		if (mm->state.find(mm->boxes) == mm->state.end()) {
			set<Pos> s;
			s.insert(mm->man);
			mm->state.insert(pair<set<Pos>, set<Pos> >(mm->boxes, s));
		}
		else {
			bool finded = false;
			set<Pos> s = mm->state.at(mm->boxes);
			for (set<Pos>::iterator it = s.begin(); it != s.end(); ++it) {
				if (mm->manHasPath(Pos(it->x, it->y))) {
					finded = true;
					break;
				}
			}
			if (!finded)
				mm->state.find(mm->boxes)->second.insert(mm->man);
		}
		//cout << "here 1" << endl;
		if (mm->win()) {
			cout << "solved" << endl;
			//cout << list.size() << endl;
			Map::res.clear();
			Map *mv = mm;
			Map::res.push_back(mv->mv);
			//cout << mv->mv << endl;
			while (mv->pre != NULL) {
				mv = mv->pre;
				Map::res.push_back(mv->mv);
			}
			if (mm != NULL) {
				Map* father = mm->pre;
				(father->nson)--;
				++cntdel;
				//cout << "----------------delete " << mm->id << ", " << mm->mv << endl;
				delete mm;
				if(father && father->nson==0)
					list.push(father);
				//cout << "father mv "<< father->mv << endl;
				mm = NULL;
			}
			Map * tfather;
			vector<Map*> tv;
			do {
				tv.clear();
//				cout << "tv ---- idx " << tv.size() << endl;
				while (!list.empty()) {
					Map * iter = list.front();
					if (iter != NULL) {
						tfather = iter->pre;
						//cout << "delete " << iter->id << ", " << iter->mv << endl;
						if (tfather != NULL)
							tfather->nson--;
						++cntdel;
						if (tfather && tfather->nson == 0)
							tv.push_back(tfather);
						delete iter;
						iter = NULL;
					}
					list.pop();
				}
				for (int i = 0; i<tv.size(); ++i) {
					list.push(tv[i]);
					//cout << "list " << tv[i]->mv << endl;
				}
			} while (tv.size() != 0);
			//cout << " tv " << tv.size() << endl;
/*
			for (int i = 0; i < m.row; ++i) {
				for (int j = 0; j < m.col; ++j) {
					cout << m.maze[i][j] << '\t';
				}
				cout << "\t<----  " << i;
				cout << endl;
			}
			cout << "结果如下： " << endl;
			for (int i = res.size() - 1; i >= 0; --i) {
				cout << res[i] << endl;
			}
*/
			Map::state.clear();
			return true;
		}
		//	cout << "here2" << endl;
		if (mm->cantSolve()) {
			Map * mv = mm->pre;
			++cntdel;
//			cout <<"cant solve >> "<< mm->mv << endl;
			//cout << "delete " << mm->id << endl;
			delete mm;

			mv->nson = mv->nson - 1;
			mm = NULL;
			while ((mv != NULL) && (mv->nson == 0)) {
				Map * father = mv->pre;
				++cntdel;
				//cout << "delete " << mv->id << endl;
				delete mv;
				--(father->nson);
				mv = father;
			}
			continue;
		}
		int newstepnum = 0;
		for (set<Pos>::iterator it = mm->boxes.begin(); it != mm->boxes.end(); ++it) {
			for (int j = 0; j < 4; ++j) {
				int newx = it->x + dir[j].x;
				int newy = it->y + dir[j].y;
				int manx = it->x - dir[j].x;
				int many = it->y - dir[j].y;

				if (newx >= 0 && newx < mm->maze.size() && newy >= 0
					&& newy < mm->maze[0].size() && mm->maze[newx][newy] != 'x'
					&& mm->maze[newx][newy] != 'b' && mm->maze[newx][newy] != 'o'
					&& manx >= 0 && manx < mm->maze.size() && many >= 0
					&& many < mm->maze[0].size() && mm->maze[manx][many] != 'x'
					&& mm->maze[manx][many] != 'b' && mm->maze[manx][many] != 'o'
					&& mm->manHasPath(Pos(manx, many))
					) {
					set<Pos> bpos = mm->boxes;
					bpos.erase(Pos(it->x, it->y));
					bpos.insert(Pos(newx, newy));
					Pos mpos(it->x, it->y);
					if (!mm->inState(bpos, mpos)) {
						Map *m2 = new Map(*mm);
						//m2->id = uid++;

						++cntnew;
						
						m2->iter++;
						char buff[20];
						sprintf(buff, "(%d,%d)-->(%d,%d); ", it->x + 1, it->y + 1, newx + 1, newy + 1);
						m2->mv = buff;
						m2->pre = mm;
						m2->maze[newx][newy] = 'b';
						m2->maze[it->x][it->y] = '=';
						m2->boxes = bpos;
						m2->man = mpos;
						
						if (!m2->cantSolve() && !m2->inState(bpos, mpos)) {
							++newstepnum;
							list.push(m2);
						}
						else {
							delete m2;
							m2 = NULL;
						}
						//else 
						//	delete m2;
					}
				}
			}

		}
		mm->nson = newstepnum;
		if (newstepnum == 0) {
			++cntdel;
			Map * mv = mm->pre;
			//cout << "delete " << mm->id << endl;
			delete mm;
			--(mv->nson);
			mm = NULL;
			while ((mv != NULL) && (mv->nson == 0)) {
				Map * father = mv->pre;
				++cntdel;
				//cout << "delete " << mv->id << endl;
				delete mv;
				--(father->nson);
				mv = father;
			}
		}

		//cout << "here7" << endl;
	}
	cout << "找不到答案" << endl;
	Map::state.clear();
	return false;
}
#include <string>
using namespace std;
extern "C"
int solve_push_box(int r, int c){
	ifstream fin("map_push_box.txt");
	if(!fin){
		return -1;
	}	
	string s;
	fin >> s;
	fin.close();
	if(s.size() <= 1){
		return -2;
	}
	//cout << "init" <<endl;
	Map m(s, r, c);
	//cout << "row" << m.row << " col " << m.col << " str " << s.size() <<endl;
	//m.print(true);
	bool cansolve = solve2(m);
	if(cansolve){
		string paht = "./push_box_res/" + s;
		ofstream out((paht.c_str()));
		for(int i=Map::res.size()-1; i > 0; --i){
			out << Map::res[i];
		}
		out.close();
		return 1;
	}
	return 0;	
}

int main() {
//	string path = "=fxxx==xxxo$==x==b=x==xxx";
//	Map m(path, 5, 5);
		string path = "===$===fbbfbbfbffbffb===x===";
		Map m(path, 4, 7);
	//int  a = 1;
	//int *ap = &a;
	//int *bp = &a;
	//cout << (ap == bp) << endl;

	m.print();
	solve_push_box(10, 10);
	set<Pos> ss;
	//ss.insert(Pos(1, 3));
	//ss.insert(Pos(2, 5));
	//cout << m.isLinked(Pos(1, 2), Pos(1, 1), ss) << endl;;
	//cout << m.isLinked(Pos(1, 2), Pos(1, 4), ss) << endl;;
	//cout << m.go(Pos(1, 2), Pos(1, 4)) <<endl;
	//Pos p(0, 0);
	//cout << m.man.x << "," << m.man.y << endl;
	//for (int i = 0; i< 1; ++i)
	//	solve2(m);
	//cout << cntdel << endl;
	//cout << cntnew << endl;

	//释放内存的操作缺少
	/*cout << m.manHasPath(p) << endl;
	map<set<Pos>, int>a;
	set<Pos>se;
	se.insert(p);
	a.insert(pair<set<Pos>, int>(se, 199));
	Pos q(0, 0);
	set<Pos>se2;
	se2.insert(q);
	cout << a.at(se2) << endl;*/
	getchar();
}
