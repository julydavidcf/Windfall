#include <vector>
#include <utility> 
#include <queue>
class BFS {
public:
	int row;
	int col;
	int dist;
	std::vector<std::pair<int,int>> path;

	BFS(int r, int c, int d, std::vector<std::pair<int,int>> p);
	~BFS();

	std::vector<std::pair<int, int>> arrowBFS(std::vector<std::vector<int>> map, BFS start, std::vector<std::vector<bool>> visited);
};