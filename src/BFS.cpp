#include "BFS.hpp"

BFS::BFS(int r, int c,int d , std::vector<std::pair<int,int>> p) {
	row = r;
	col = c;
	dist = d;
	path = p;
}
BFS::~BFS(){}

std::vector<std::pair<int, int>> BFS::arrowBFS(std::vector<std::vector<int>> map, BFS start, std::vector<std::vector<bool>> visited) {
	return start.path;
}