# include <iostream>
# include <vector>
# include <queue>
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

		/**
		 * The blocked state
		 * @private
		 */
		bool blocked = false;
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
		 * @return bool
		 * @returns Was the node's data updated successfully?
		 */
		int changedata(int data) {
			this->data = data;
			return (this->data == data);
		};

		/**
		 * Get the blocking state, which may be defined automatically (by setting data to -1) or manually (by using setblocking).
		 * @return bool
		 * @returns the blocking state: True if blocked, false otherwise
		 */
		bool getblocking() {
			return (this->blocked || this->data == -1);
		}

		/**
		 * Set the blocking state.
		 * @return bool
		 * @returns Update success state
		 */
		bool setblocking(bool status) {
			this->blocked = status;
			return (this->getblocking() == status);
		}

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

		/**
		 * Maximum supported children
		 * @private
		 */
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
		 * List all nodes.
		 *
		 * @param node (DungeonNode *) - a reference node
		 * @return vector<DungeonNode*>
		 */
		vector<DungeonNode*> list(DungeonNode* node = nullptr) {
			// The collected nodes
			vector<DungeonNode*> nodes;

			// We will use the tree to generate the list.
			vector<vector<DungeonNode*>> tree = this->tree(node);

			// Iterate through the tree
			for (int indices[2] = {0, 0}; tree.size() && indices[0] < tree.size(); indices[0]++) {
				for (indices[1] = 0; indices[1] < tree[indices[0]].size(); indices[1]++) {
					nodes.push_back(tree[indices[0]][indices[1]]);
				};
			};

			return nodes;
		};

		/**
		 * Traverse through the dungeons.
		 *
		 * @param node (DungeonNode *) - a reference node. By default, the reference is the head.
		 * @param verbose (bool) - Should the returned output be logged?
		 * @return vector<int*>
		 */
		vector<int> traverse(DungeonNode* node = nullptr, bool verbose = false) {
			if (!node) {node = this->head; };

			vector<DungeonNode*> nodes = this->list(node);
			vector<int> values = {};

			// Convert from nodes to values
			for (int size = 0; size < nodes.size(); size++) {
				values.push_back(nodes[size]->getdata());
			};

			// Sort the values
			sort(values.begin(), values.end());

			if (verbose) {
				for (int node_ID = 0; node_ID < values.size(); node_ID++) {
					cout << values[node_ID] << "|";
				};
				cout << endl;
			};

			return values;
		};

		/**
		 * Generate a tree of dungeons.
		 *
		 * @param start (DungeonNode *) - Which node should this call start with?
		 * @return vector<vector<DungeonNode*>>
		 */
		vector<vector<DungeonNode*>> tree(DungeonNode* start = nullptr) {
			// Set start to have a value.
			if (!start) {
				start = this->head;
			}

			// The tree
			vector<vector<DungeonNode*>> tree;
			if (!start) {return tree; }; // If tree is empty, return that, because we can't generate a tree.

			queue<DungeonNode*> order; // Queue of nodes to process
			queue<int> levels; // What level does it go to?

			// Place it inside
			order.push(start);
			levels.push(1);

			// The level
			vector<DungeonNode*> level = {start};
			tree.push_back(level);

			// While the order isn't empty, we deal with it
			while (!(order.empty())) {
				DungeonNode* node = order.front(); // The node
				int node_level = levels.front(); // The current level
				vector<DungeonNode*> children = node->getchildren(); // The children of that node

				// Is the size correct? If not, let's fix it. For example, at `node_level` 1, the size is still 1, so we need to add by 1
				while (tree.size() < (node_level + 1)) {
					level.clear();
					tree.push_back(level);
				};

				for (int index = 0; index < children.size(); index++) { // It's chronological so we can assume (hopefully) that the order at which the children are inserted also are
					DungeonNode *child = children[index]; // The child

					tree[node_level].push_back(child); // Put that child here

					// Now add this for further processing
					order.push(child);
					levels.push(node_level + 1);
				};

				// Dequeue everything
				levels.pop();
				order.pop();
			};

			return tree;
		};

		/**
		 * Display the tree.
		 * @param start (DungeonNode *) - starting node
		 * @param include_blocked (bool) - Show blocked nodes?
		 * @param verbose (bool) - Display the nodes in output?
		 * @return vector<vector<int>>
		 * @returns the tree’s numerical representation
		 */
		vector<vector<int>> displaytree(DungeonNode* start = nullptr, bool include_blocked = true, bool verbose = true) {
			if (!start) {start = this->head;}

			// The tree
			vector<vector<int>> tree;
			if (!start) {return tree; }; // No start, no output

			if (include_blocked) {
				vector<vector<DungeonNode*>> node_tree = this->tree(start);
				for (int level[2] = {0, 0}; level[0] < node_tree.size(); level[0]++) {
					vector<int> contents; // The contents of that level
					for (level[1] = 0; level[1] < node_tree[level[0]].size(); level[1]++) {
						contents.push_back(node_tree[level[0]][level[1]]->getdata());
					};
					tree.push_back(contents);
				};
			} else {
				// Use a custom variation of that function

				queue<DungeonNode*> order; // Queue of nodes to process
				queue<int> levels; // What level does it go to?
				queue<bool> status; // Blocking states

				// Place it inside
				order.push(start); levels.push(1); status.push(!(start->getblocking()));

				// The level
				vector<int> level = {start->getdata()};
				if (!(start->getblocking())) {level[0] = -1;};
				tree.push_back(level);

				while (!(order.empty())) {
					DungeonNode* node = order.front(); // The node
					int node_level = levels.front(); // The current level
					vector<DungeonNode*> children = node->getchildren(); // The children of that node

					// Correct the size
					while (tree.size() < (node_level + 1)) {
						level.clear();
						tree.push_back(level);
					};

					for (int index = 0; index < children.size(); index++) {
						DungeonNode *child = children[index]; // The child

						if (status.front() && !(child->getblocking())) {
							tree[node_level].push_back(child->getdata()); // Put the child here
							status.push(true); // Prepare the next blocking state
						} else {
							tree[node_level].push_back(-1);
							status.push(false);
						};

						// Now add this for further processing
						order.push(child);
						levels.push(node_level + 1);
					};

					// Dequeue everything
					levels.pop();
					order.pop();
					status.pop();
				};
			};


			if (verbose) {
				for (int level[2] = {0, 0}; level[0] < tree.size(); level[0]++) {
					for (level[1] = 0; level[1] < tree[level[0]].size(); level[1]++) {
						cout << tree[level[0]][level[1]] << "|";
					};
					cout << endl;
				};
			};

			return tree;
		};

		vector <int> getnodelist(DungeonNode* node, vector <int> temp, int currdepth, int depthtoget){
			if(depthtoget == currdepth){
				temp.push_back(node->data); // Push back the current node's data
			}
			for(int i = 0; i < (node->getchildren()).size(); i++){
				if (node->getchildren(i)) { // For each children
					temp = this->getnodelist(node->children[i], temp, currdepth + 1, depthtoget);
				};
			};
			return temp;
		};

		vector <int> getnodelisthelper(DungeonNode* node, vector <int> temp, int maxdepth){
			for(int i = 0; i <= maxdepth; i++) {
				temp = getnodelist(node, temp, 0, i); // Merge the provided vector for each depth
			}
    		return temp;
		};


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

		/**
		 * Edit a node.
		 *
		 * @param node (DungeonNode *) - the node
		 * @param data (int) the data
		 */
		bool editnode(DungeonNode* node, int data) {
			return node->changedata(data);
		};

		/**
		 * Edit a node.
		 * @param path (vector<int>) - the node’s path
		 * @param data (int) the data
		 * @return bool
		 * @returns Was the data updated successfully?
		 */
		bool editnode(vector<int> path, int data) {
			DungeonNode *node = this->head;
			for (int indices[2] = {0, 0}; this->head && path.size() && (indices[0] < path.size()); indices[0]++) {
				indices[1] = path[indices[0]];

				if ((node->getchildren()).size() < indices[1]) {
					// That's when we have a problem
					return false;
				};

				// Update the node
				node = node->getchildren(indices[1]);

				// If this is the last element, we must edit it.
				if (indices[0] == (path.size() - 1)) {
					return this->editnode(node, data);
				};
			};

			// Base case is to return false
			return false;
		};

		int findmaxvalue(DungeonNode* node, int maxholdervalue);
		int findmaxsum(DungeonNode* node, int maxholdersum, int currentsum);
		vector <int> findbestroom(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder);
		vector <int> findbestpath(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder, int currentsum);
};

int main() {

};
