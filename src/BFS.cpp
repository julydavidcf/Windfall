#include "BFS.hpp"

using namespace std;

BFS::BFS(int r, int c,int d , std::vector<std::pair<int,int>> p) {
	row = r;
	col = c;
	dist = d;
	path = p;
}
BFS::~BFS(){}

std::vector<std::pair<int, int>> BFS::arrowBFS(std::vector<std::vector<int>> map, BFS start, std::vector<std::vector<bool>> visited) {
	queue<BFS> workQueue;
	workQueue.push(start);
	//printf("map 75 45is at %d, visited is %b\n", map[75][21], visited[75][21]);
	visited[start.row][start.col] = true;
	while (!workQueue.empty())
	{
		BFS curr = workQueue.front();
		workQueue.pop();
		//printf("current at %d %d, size is %d %d\n", curr.row, curr.col, map.size(), map[75].size());
		// reach destination
		if (map[curr.row][curr.col] == 9) {
			return curr.path;
			
		}
		//// test(add diagnals)+++++++++++++++++++++++++++++++++++++++++++++
		//if (curr.row - 1 >= 0 && curr.col - 1 >= 0 && visited[curr.row - 1][curr.col-1] == false) {
		//	//printf("before U\n");
		//	std::vector <std::pair<int, int>> new_path = curr.path;
		//	new_path.push_back(make_pair(curr.row - 1, curr.col-1));
		//	workQueue.push(BFS(curr.row - 1, curr.col-1, curr.dist + 1, new_path));
		//	visited[curr.row - 1][curr.col-1] = true;
		//	//printf("save U\n");
		//}
		//if (curr.row + 1 < map.size() && curr.col - 1 >=0  && visited[curr.row + 1][curr.col - 1] == false) {
		//	//printf("before U\n");
		//	std::vector <std::pair<int, int>> new_path = curr.path;
		//	new_path.push_back(make_pair(curr.row + 1, curr.col - 1));
		//	workQueue.push(BFS(curr.row + 1, curr.col - 1, curr.dist + 1, new_path));
		//	visited[curr.row + 1][curr.col - 1] = true;
		//	//printf("save U\n");
		//}
		//if (curr.row + 1 < map.size() && curr.col + 1 < map[0].size() && visited[curr.row + 1][curr.col+ 1] == false) {
		//	//printf("before U\n");
		//	std::vector <std::pair<int, int>> new_path = curr.path;
		//	new_path.push_back(make_pair(curr.row + 1, curr.col + 1));
		//	workQueue.push(BFS(curr.row + 1, curr.col + 1, curr.dist + 1, new_path));
		//	visited[curr.row + 1][curr.col + 1] = true;
		//	//printf("save U\n");
		//}
		//if (curr.row - 1 >= 0 && curr.col + 1 < map[0].size() && visited[curr.row - 1][curr.col + 1] == false) {
		//	//printf("before U\n");
		//	std::vector <std::pair<int, int>> new_path = curr.path;
		//	new_path.push_back(make_pair(curr.row - 1, curr.col + 1));
		//	workQueue.push(BFS(curr.row - 1, curr.col + 1, curr.dist + 1, new_path));
		//	visited[curr.row - 1][curr.col + 1] = true;
		//	//printf("save U\n");
		//}
		////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//move up
		if (curr.row - 1 >= 0 && visited[curr.row - 1][curr.col] == false) {
			//printf("before U\n");
			std::vector <std::pair<int, int>> new_path = curr.path;
			new_path.push_back(make_pair(curr.row - 1, curr.col));
			workQueue.push(BFS(curr.row - 1, curr.col, curr.dist + 1, new_path));
			visited[curr.row - 1][curr.col] = true;
			//printf("save U\n");
		}
		//move down
		if (curr.row + 1 < map.size() && visited[curr.row + 1][curr.col] == false) {
			//printf("before D\n");
			std::vector <std::pair<int, int>> new_path = curr.path;
			new_path.push_back(make_pair(curr.row + 1, curr.col));
			workQueue.push(BFS(curr.row + 1, curr.col, curr.dist + 1, new_path));
			visited[curr.row + 1][curr.col] = true;
			//printf("save D\n");
		}
		//move left
		if (curr.col - 1 >= 0 && visited[curr.row ][curr.col-1] == false) {
			//printf("before L\n");
			std::vector <std::pair<int, int>> new_path = curr.path;
			new_path.push_back(make_pair(curr.row, curr.col-1));
			workQueue.push(BFS(curr.row, curr.col-1, curr.dist + 1, new_path));
			visited[curr.row][curr.col-1] = true;
			//printf("save L\n");
		}
		//move right
		if (curr.col + 1 < map[0].size() && visited[curr.row][curr.col + 1] == false) {
			//printf("before R\n");
			std::vector <std::pair<int, int>> new_path = curr.path;
			new_path.push_back(make_pair(curr.row, curr.col + 1));
			workQueue.push(BFS(curr.row, curr.col + 1, curr.dist + 1, new_path));
			visited[curr.row][curr.col + 1] = true;
			//printf("save R\n");
		}

	}
	return start.path;
}