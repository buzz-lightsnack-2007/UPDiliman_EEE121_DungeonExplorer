# include <iostream>
# include <vector>
# include <algorithm>

using namespace std;

class DungeonNode {
	private:
		int data;
		int maxchild;

		/**
		 * The connected dungeons; size restricted by maxchild
		 * @private
		 */
		vector <DungeonNode*> children;
	public:
		/**
		 * Construct a new Dungeon Node object.
		 *
		 * @param data (int)
		 * @param maxchild (int) - the maximum number of children
		 */
		DungeonNode(int data, int maxchild) {
			this->data = data;
			this->maxchild = maxchild;
		};

		/**
		 * Construct a new Dungeon Node object.
		 *
		 * @param maxchild (int) - the maximum number of children
		 */
		DungeonNode(int maxchild) {
			this->maxchild = maxchild;
		};

		/**
		 * Get the associated children of this node.
		 *
		 * @param idx
		 * @return DungeonNode*
		 */
		DungeonNode* getchildren(int idx) {
			return this->children[idx];
		};

		/**
		 * Get the associated children of this node.
		 *
		 * @return vector<DungeonNode*>
		 */
		vector<DungeonNode*> getchildren() {
			return this->children;
		};

		/**
		 * Get this dungeon's data.
		 *
		 * @return int
		 */
		int getdata() {
			return this->data;
		};

		/**
		 * Replace this dungeon's data.
		 *
		 * @param data (int)
		 */
		void changedata(int data) {
			this->data = data;
		};

		friend class Dungeon;
};

class Dungeon {
	private:
		/**
		 * The dungeon's size
		 * @private
		 */
		int size;

		/**
		 * The dungeon's start
		 * @private
		 */
		DungeonNode *head;
		int maxchild;

	protected:
		/**
		 * The dungeon's depth
		 * @param node (DungeonNode *) - the node to start with
		 * @param offset (int) - the offset
		 * @return int
		 */
		int depth(DungeonNode* node, int offset = 0) {
			vector<DungeonNode*> children = node->getchildren();

			// If no children, that's it
			if (children.empty()) {return offset; };

			/**
			 * The depth results. Which one is higher?
			 */
			vector<int> results;

			// Else, for each children…
			for (int side = 0; side < children.size(); side++) {
				// We recursively attempt to find the maximum depth there
				DungeonNode* child = children[side];
				results.push_back(this->depth(child, offset + 1));
			};

			return *(max_element(results.begin(), results.end()));
		};

	public:
		/**
		 * Start a dungeon.
		 *
		 * @param headvalue (int) the head value
		 * @param maxchild (int) the maximum child size
		 */
		Dungeon(int headvalue, int maxchild) {
			// Create a new dungeon node for this.
			this->head = new DungeonNode(headvalue, maxchild);

			// Set all other configuration
			this->maxchild = maxchild;
		};

		/**
		 * Start a dungeon from an entry point.
		 * The dimensions will be inferred from the provided head node.
		 *
		 * @param dungeon (DungeonNode *) - the dungeon node
		 */
		Dungeon(DungeonNode* dungeon) {
			this->head = dungeon;
			this->maxchild = dungeon->maxchild;
		};

		/**
		 * Start a dungeon from an entry point.
		 *
		 * @param dungeon (DungeonNode *) - the dungeon node
		 * @param dimensions (int) - the maximum child size
		 */
		Dungeon(DungeonNode* dungeon, int dimensions) {
			this->head = dungeon;
			this->maxchild = dimensions;
		};

		/**
		 * Create a dungeon. The deepest and leftmost dungeons are created first.
		 *
		 * @param data -
		 * @param depth
		 * @param done
		 * @param node
		 * @return int
		 */
		int insertcell(int data, int depth, int done, DungeonNode* node) {
		};

		/**
		 * The dungeon's start
		 *
		 * @return DungeonNode*
		 */
		DungeonNode *gethead() {
			return this->head;
		};

		/**
		 * Traverse through the dungeons.
		 *
		 * @param node (DungeonNode *) - a reference node
		 * @param use_nodes (bool) - Fill this with any value to use this traversal
		 * @return vector<DungeonNode*>
		 */
		vector<DungeonNode*> traverse(DungeonNode* node, bool use_nodes) {
			// The collected nodes
			vector<DungeonNode*> nodes = {node};

			// The children nodes
			vector<DungeonNode*> children = node->getchildren(); // DON'T MERGE
			for (int side = 0; side < children.size(); side++) {
				DungeonNode* child = children[side];

				// The descendant nodes, for recursive insertion
				vector<DungeonNode*> descendants = this->traverse(child, true);
				for (int descendant = 0; descendant < descendants.size(); descendant++) {
					nodes.push_back(descendants[descendant]); // Copy it here
				}
			};

			return nodes;
		};

		/**
		 * Traverse through the dungeons.
		 *
		 * @param node (DungeonNode *) - a reference node. By default, the reference is the head.
		 * @return vector<int*>
		 */
		vector<int> traverse(DungeonNode* node = nullptr) {
			if (!node) {node = this->head; };

			vector<DungeonNode*> nodes = this->traverse(node, true);
			vector<int> values = {};

			// Convert from nodes to values
			for (int size = 0; size < nodes.size(); size++) {
				values.push_back(nodes[size]->data);
			};

			// Sort the values
			sort(values.begin(), values.end());
			return values;
		};

		// We have to do a BFS for this one.
		void displayTree(DungeonNode* node, vector<int>);
		vector <int> getnodelist(DungeonNode* node, vector <int> temp, int currdepth, int depthtoget);
		vector <int> getnodelisthelper(DungeonNode* node, vector <int> temp, int maxdepth);
		vector <int> getnodelist2(DungeonNode* node, vector <int> temp, int currdepth, int depthtoget, int blocked);
		vector <int> getnodelisthelper2(DungeonNode* node, vector <int> temp, int maxdepth);

		/**
		 * The dungeon's size
		 * @return int
		 */
		int getsize() {
			return this->size;
		};

		/**
		 * The dungeon's depth
		 * @param offset (int) - the offset
		 * @return int
		 */
		int depth(int offset = 0) {
			return this->depth(this->head, offset);
		};

		int getmaxchild();
		void editnode(DungeonNode* node, vector <int> path, int newdata);
		int findmaxvalue(DungeonNode* node, int maxholdervalue);
		int findmaxsum(DungeonNode* node, int maxholdersum, int currentsum);
		vector <int> findbestroom(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder);
		vector <int> findbestpath(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder, int currentsum);
};

int main() {

};
