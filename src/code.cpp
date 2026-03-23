# include <iostream>
# include <vector>
# include <algorithm>

using namespace std;

class DungeonNode {
	private:
		int data;
		int maxchild;
		vector <DungeonNode*> children;
	public:
   		DungeonNode(int data, int maxchild);
     	DungeonNode* getchildren(int idx);
      	int getdata();
       	void changedata(int newdata);
        friend class Dungeon;
};

class Dungeon {
	private:
		int size; DungeonNode *head; int maxchild;
	public:
		Dungeon(int headvalue, int maxchild);
		int insertcell(int data, int depth, int done, DungeonNode* node);
		DungeonNode *gethead();
		void traverse(DungeonNode* node);
		void displayTree(DungeonNode* node, vector<int>);
		vector <int> getnodelist(DungeonNode* node, vector <int> temp, int currdepth, int depthtoget);
		vector <int> getnodelisthelper(DungeonNode* node, vector <int> temp, int maxdepth);
		vector <int> getnodelist2(DungeonNode* node, vector <int> temp, int currdepth, int depthtoget, int blocked);
		vector <int> getnodelisthelper2(DungeonNode* node, vector <int> temp, int maxdepth);
		int getsize();
		int getmaxchild();
		void editnode(DungeonNode* node, vector <int> path, int newdata);
		int findmaxvalue(DungeonNode* node, int maxholdervalue);
		int findmaxsum(DungeonNode* node, int maxholdersum, int currentsum);
		vector <int> findbestroom(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder);
		vector <int> findbestpath(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder, int currentsum);
};

int main() {

}
