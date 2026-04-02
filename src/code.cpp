# include <cstdlib>
# include <iostream>
# include <cmath>
# include <vector>
# include <queue>
# include <stack>
# include <algorithm>
# include <map>

using namespace std;

class DungeonNode {
	private:
		int data;
		int maxchild = -1;

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
		 * Add a child to this node. 
		 * 
		 * @param child (DungeonNode *) - the child node to append
		 * @return bool
		 * @returns whether or not the child was inserted successfully
		 */
		bool appendchildren(DungeonNode* child) {
			if (this->maxchild < 0 || (this->children.size() < this->maxchild)) {
				this->children.push_back(child); 
				return true; 
			};

			return false; 
		};

		/**
		 * Add children to this node. 
		 * 
		 * @param children (vector<DungeonNode *>) - the children to append
		 * @return vector<bool>
		 * @returns the insertion results for each child
		 */
		vector<bool> appendchildren(vector<DungeonNode*> children) {
			vector<bool> results; 

			for (int child = 0; child < children.size(); child++) {
				results.push_back(this->appendchildren(children[child]));
			};

			return results;
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
		 * Maximum supported levels
		 * @protected
		 */
		int levels;
		
		
		/**
		 * Create a dungeon recursively. The deepest and leftmost dungeons are created first.
		 * 
		 * @param parent (DungeonNode *) - the parent
		 * @param depth (int) - the current depth
		 * @param to_insert (DungeonNode *) - the node to insert
		 * @return bool
		 * @returns whether or not the node was inserted
		 */
		bool insertcell(DungeonNode* parent, int depth, DungeonNode* to_insert) {
			bool insertable = false; // A node is insertable to a parent if 1) it has no children or 2) it has a next available spot. These are checked after reaching the lowest possible depth, where 1) is checked first then by 2) if not satisfied. 
			if (depth >= this->getdepth()) {return insertable; } // Can't insert here

			// If it has children…
			for (int child_ID = 0; !insertable && (child_ID < parent->getchildren().size()); child_ID++) {
				// Go deeper first
				DungeonNode *child = parent->getchildren(child_ID); 
				insertable = this->insertcell(child, depth + 1, to_insert); 
			};
			
			// If it doesn't have children, or all of its children are already occupied…
			if (!insertable && parent->getchildren().size() < this->maxchild) {
				insertable = parent->appendchildren(to_insert); // We have to accomodate for how many children it supports
			};
			
			return insertable; 
		};
	public:
		/**
		 * Start a dungeon.
		 *
		 * @param headvalue (int) the head value
		 * @param maxchild (int) the maximum child size
		 * @param max_level (int) the maximum supported levels
		 */
		Dungeon(int headvalue, int maxchild, int max_level) {
			// Create a new dungeon node for this.
			this->head = new DungeonNode(headvalue, maxchild);

			// Set all other configuration
			this->maxchild = maxchild;
			this->levels = max_level; 
		};

		/**
		 * Start a dungeon from an entry point.
		 *
		 * @param dungeon (DungeonNode *) - the dungeon node
		 * @param max_children (int) - the maximum child size
		 * @param max_levels (int) the maximum supported levels
		 */
		Dungeon(DungeonNode* dungeon, int max_children, int max_level) {
			this->head = dungeon;
			
			// Set all other configuration - note that they can't be inferred from the dungeon, because the attribs are private
			this->maxchild = max_children;
			this->levels = max_level; 
		};

		/**
		 * Create a dungeon node instance and insert it into the dungeon instance. The deepest and leftmost dungeons are created first.
		 *
		 * @param data (int) - the data
		 * @param maxchild (int) - the maximum children supported by this node (default: as configured in the instance)
		 * @return DungeonNode*
		 * @returns the dungeon node instance 
		 */
		DungeonNode* insertcell(int data, int maxchild = 0) {
			if (maxchild <= 0) {
				maxchild = this->maxchild; 
			}
			
			DungeonNode* node = new DungeonNode(data, maxchild); 
			this->insertcell(node);
			return node; 
		};
		
		/**
		 * Create a dungeon. The deepest and leftmost dungeons are created first.
		 *
		 * @param node (DungeonNode *) - the node to insert
		 * @return int
		 */
		bool insertcell(DungeonNode* node) {
			if (!(this->head)) {return false;}; 
			
			return this->insertcell(this->head, 0, node);  
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
		 * @param include_blocked (bool) - whether to include blocked nodes (default: true)
		 * @return vector<DungeonNode*>
		 */
		vector<DungeonNode*> list(DungeonNode* node = nullptr, bool include_blocked = true) {
			// The collected nodes
			vector<DungeonNode*> nodes;

			// We will use the tree to generate the list.
			vector<vector<DungeonNode*> > tree = this->tree(include_blocked, node);

			// Iterate through the tree
			for (int indices[2] = {0, 0}; tree.size() && indices[0] < tree.size(); indices[0]++) {
				for (indices[1] = 0; indices[1] < tree[indices[0]].size(); indices[1]++) {
					if (tree[indices[0]][indices[1]]) {
						nodes.push_back(tree[indices[0]][indices[1]]);
					};
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
		vector<int> traverse(DungeonNode* node = nullptr, bool verbose = true) {
			if (!node) {node = this->head; };

			vector<DungeonNode*> nodes = this->list(node);
			vector<int> values;

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
		 * Calculates all possible paths from the head node to every other node. This function performs a DFS traversal of the tree starting from the head node to identify all unique paths. 
		 * @param start (DungeonNode*) - the starting node
		 * @param include_blocked (bool) - whether to include blocked nodes (default: false)
		 * @return vector<vector<DungeonNode*>>
		 * @returns Each inner vector represents a path from the head node to a specific node in the tree. 
		 */
		vector<vector<DungeonNode*> > paths(DungeonNode* start = nullptr, bool include_blocked = false) {
			if (!start) {start = this->head;}; 
			
			// The paths
			vector<vector<DungeonNode*> > paths;
			if (!start) {return paths;};
			
			stack<DungeonNode*> order; // the stack of nodes to process
			stack<vector<DungeonNode*> > parents; // the stack of parents

			if (!include_blocked && start->getblocking()) {return paths;};

			// The path
			vector<DungeonNode*> path; 

			// The parents
			vector<DungeonNode*> parent; 
			order.push(start); 
			parents.push(parent); 

			path.push_back(start);
			paths.push_back(path); // Include in the combinations

			while (order.size() && parents.size()) {
				DungeonNode* node = order.top(); 
				parent.clear(); 

				for (int size = 0; size < parents.top().size(); size++) {
					parent.push_back(parents.top()[size]); 
				};
				parent.push_back(node); // Now the current node is a parent - of its children. 

				parents.pop(); order.pop(); // Prepare for replacement

				for (int side = 0; side < node->getchildren().size(); side++) {
					DungeonNode* child = node->getchildren(side); 
					
					if (!include_blocked && child->getblocking()) {continue;}; // If blocked and not including it, skip it. 
					path.clear(); // Reset the path
					
					for (int size = 0; size < parent.size(); size++) {
						path.push_back(parent[size]); 
					};
					path.push_back(child); 
					paths.push_back(path); 

					order.push(child); 
					parents.push(parent); 
				};
			};

			return paths; 
		};

		/**
		 * Get each path and its total. 
		 * 
		 * @param start (DungeonNode*) - the starting node
		 * @param include_blocked (bool) - whether to include blocked nodes (default: false)
		 * @return map<vector<DungeonNode*>, int>
		 * @returns Keys are the paths, the values are the totals. 
		 */
		map<vector<DungeonNode*>, int> scores(DungeonNode* start = nullptr, bool include_blocked = false) {
			vector<vector<DungeonNode*> > paths = this->paths(start, include_blocked);
			map<vector<DungeonNode*>, int> scores; 

			for (int path_ID = 0; path_ID < paths.size(); path_ID++) {
				vector<DungeonNode*> path = paths[path_ID]; 
				int sum = 0; 
				for (int step = 0; step < path.size(); step++) {
					sum += path[step]->getdata(); 
				};

				scores[path] = sum; 
			};

			return scores;
		};

		/**
		 * Find the path to a node. 
		 * 
		 * @param query (DungeonNode *) - The search query
		 * @param include_blocked (bool) - whether to include blocked nodes (default: false)
		 * @return vector<DungeonNode *>
		 * @returns the path to the node, including the node itself. If not found, an empty vector is returned. 
		 */
		vector<DungeonNode*> find(DungeonNode* query, bool include_blocked = false) {
			vector<vector<DungeonNode*> > paths = this->paths(nullptr, include_blocked);
			vector<DungeonNode*> path;

			for (int index[2] = {0, 0}; index[0] < paths.size() && path.empty(); index[0]++) {
				path.clear(); 
				for (index[1] = 0; paths[index[0]].back() == query && index[1] < paths[index[0]].size(); index[1]++) {
					path.push_back(paths[index[0]][index[1]]); 
				}; 
			};
			
			return path; 
		};

		/**
		 * Find the indices to the path of a node. 
		 *
		 * @param query (DungeonNode *) - The search query
		 * @param include_blocked (bool) - whether to include blocked nodes (default: false)
		 * @return vector<int>
		 * @returns the path to the node, including the node itself. If not found, an empty vector is returned. 
		 */
		vector<int> navigate(DungeonNode* query, bool include_blocked = false) {
			vector<DungeonNode*> path = this->find(query, include_blocked);
			vector<int> indices;
			
			for (int step = 0; step < path.size() - 1; step++) {
				DungeonNode *parent = path[step]; 
				
				for (int side = 0; side < parent->getchildren().size(); side++) {
					if (parent->getchildren(side) == path[step + 1]) {
						indices.push_back(side); 
						break; 
					};
				};
			};
			
			return indices; 
		};

		/**
		 * Generate a tree of dungeons.
		 *
		 * @param start (DungeonNode *) - Which node should this call start with?
		 * @param include_blocked (bool) - Include blocked nodes (default: true)
		 * @return vector<vector<DungeonNode*>>
		 * @returns the tree of dungeons. If blocked nodes aren’t included, they’re represented as null pointers. 
		 */
		vector<vector<DungeonNode*> > tree(bool include_blocked = true, DungeonNode* start = nullptr) {
			// Set start to have a value.
			if (!start) {
				start = this->head;
			}

			// The tree
			vector<vector<DungeonNode*> > tree;
			if (!start) {return tree; }; // If tree is empty, return that, because we can't generate a tree.

			queue<DungeonNode*> order; // Queue of nodes to process
			queue<int> levels; // What level does it go to?
			queue<bool> status; // Blocking states

			// Place it inside
			order.push(start); levels.push(1); status.push(include_blocked || !(start->getblocking()));

			// The level
			vector<DungeonNode*> level;
			if (include_blocked || !(start->getblocking())) { // If we don't want to include blocked nodes, and the start is blocked
				level.push_back(start);
			} else {
				level.push_back(nullptr);
			};
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

					if (include_blocked || (status.front() && !(child->getblocking()))) {
						tree[node_level].push_back(child); // Put the child here
						status.push(true); // Prepare the next blocking state
					} else {
						tree[node_level].push_back(nullptr);
						status.push(false);
					};

					// Now add this for further processing
					order.push(child);
					levels.push(node_level + 1);
				};

				// Dequeue everything
				levels.pop(); order.pop(); status.pop();
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
		vector<vector<int> > displaytree(bool include_blocked = true, DungeonNode* start = nullptr, bool verbose = true) {
			if (!start) {start = this->head;}

			// The tree
			vector<vector<int> > tree;
			if (!start) {return tree; }; // No start, no output

			vector<vector<DungeonNode*> > node_tree = this->tree(include_blocked, start);
			for (int level[2] = {0, 0}; level[0] < node_tree.size(); level[0]++) {
				vector<int> contents; // The contents of that level
				for (level[1] = 0; level[1] < node_tree[level[0]].size(); level[1]++) {
					DungeonNode *node = node_tree[level[0]][level[1]]; 
					if (node) {
						contents.push_back(node->getdata());
					} else {
						contents.push_back(-1);
					};
				};
				tree.push_back(contents);
			};

			for (int level[2] = {0, 0}; verbose && level[0] < tree.size(); level[0]++) {
				for (level[1] = 0; level[1] < tree[level[0]].size(); level[1]++) {
					cout << tree[level[0]][level[1]] << "|";
				};
				cout << endl;
			};
			
			return tree;
		};

		vector <int> getnodelist(DungeonNode* node, vector <int> temp, int currdepth, int depthtoget){
			if(depthtoget == currdepth){
				temp.push_back(node->getdata()); // Push back the current node's data
			}
			for(int i = 0; i < (node->getchildren()).size(); i++){
				if (node->getchildren(i)) { // For each children
					temp = this->getnodelist(node->getchildren(i), temp, currdepth + 1, depthtoget);
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

		/**
		 * The dungeon's size
		 * @return int
		 */
		int getsize() {
			return this->size;
		};
		
		/**
		 * The configured depth of the node
		 * @return int
		 */
		int getdepth() {
			return this->levels - 1; 
		}; 
		
		/**
		 * The configured levels of the node
		 * @return int
		 */
		int getlevels() {
			return this->levels; 
		}; 

		/**
		 * Get the node with the extrema (maximum or minimum) value.
		 * @param type (int) - 1 for maxima, -1 for minima
		 * @param include_blocked (bool) - Should blocked nodes be considered? (default: false)
		 * @return DungeonNode*
		 * @returns the node with the extrema value
		 */
		DungeonNode* extrema(int type, bool include_blocked = false) {
			vector<DungeonNode*> list = this->list(nullptr, include_blocked); // The list of nodes
			if (list.empty() || type == 0) {return nullptr;};

			DungeonNode* leaderboard = list[0];
			for (int index = 1; index < list.size(); index++) {
				if ((type < 0 && list[index]->getdata() < leaderboard->getdata()) || (type > 0 && list[index]->getdata() > leaderboard->getdata())) {
					leaderboard = list[index];
				};
			};

			return leaderboard;
		};
		
		/**
		 * Find the path that accumulates to the extrema (maximum or minimum) value. 
		 * @param type (int) - 1 for maxima, -1 for minima
		 * @param include_blocked (bool) - Should blocked nodes be considered? (default: false)
		 * @return vector<DungeonNode*>
		 * @returns the path to the node with the extrema value
		 */
		vector<DungeonNode*> extrema_path(int type, bool include_blocked = false) {
			vector<DungeonNode*> extreme; // The result
			if (!type) {return extreme; }; 

			map<vector<DungeonNode*>, int> scores = this->scores(nullptr, include_blocked); // The scores
			vector<vector<DungeonNode*> > paths = this->paths(nullptr, include_blocked); // The list of nodes
			
			if (!(paths.size())) {return extreme; };
			extreme = paths[0];
			int extrema = scores[paths[0]]; // The extrema
			
			for (int path_ID = 0; path_ID < paths.size(); path_ID++) {
				vector<DungeonNode*> path = paths[path_ID]; // The selected path
				
				if ((type < 0 && scores[path] < extrema) || (type > 0 && scores[path] > extrema)) {
					extreme = path; 
					extrema = scores[path]; 
				};
			};

			return extreme;
		};

		/**
		 * Find navigated path to the extrema (maximum or minimum) value. 
		 * @param type (int) - 1 for maxima, -1 for minima
		 * @param include_blocked (bool) - Should blocked nodes be considered? (default: false)
		 * @return vector<int>
		 * @returns the navigated path to the node with the extrema value
		 */
		vector<int> extrema_navigation(int type, bool include_blocked = false) {
			DungeonNode* extrema = this->extrema(type, include_blocked); 
			return this->navigate(extrema, true); 
		};

		/**
		 * Find the largest treasure. Alias of extrema_navigation(1, false). 
		 * 
		 * @returns vector<int>
		 * @returns the navigated path to the node with the maximum value
		 */
		vector<int> largesttreasure() {
			return this->extrema_navigation(1); 
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
					return node->changedata(data);
				};
			};

			// Base case is to return false
			return false;
		};

		/**
		 * Navigate to the path that accumulates to the extrema (maximum or minimum) value. 
		 * @return vector<int>
		 * @returns the path to the node with the extrema value
		 */
		vector<int> bestpath() {
			vector<DungeonNode*> path = this->extrema_path(1); 
			vector<int> directions; 

			if (!(path.size())) {return directions; }
			DungeonNode *node = path.back(); 
			return this->navigate(node); 
		};

		int findmaxvalue(DungeonNode* node, int maxholdervalue);
		int findmaxsum(DungeonNode* node, int maxholdersum, int currentsum);
		vector <int> findbestroom(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder);
		vector <int> findbestpath(DungeonNode* node, vector <int> temp, int maxvalue, vector <int> holder, int currentsum);
};


/**
 * Extract integers from an input string. Delimiters include ',' and ';' and can’t be customised. 
 * 
 * @param entry (string) - the input string
 * @return vector<int>
 * @returns the input integers
 */
vector<int> get_numbers(string entry) {
	vector<int> numbers; // The return value
	vector<int> positive; // Tells whether a number in numbers is positive or negative
	
	map<char, bool> handling; 
	handling['-'] = true; // Only allowed before any digit, after a comma
	handling['.'] = true; // Only allowed once
	
	for (int indices[2] = {0, 0}; indices[0] < entry.size(); indices[0]++) {
		while (numbers.size() <= indices[1]) {
			numbers.push_back(0); 
		};
		while (positive.size() <= indices[1]) {
			positive.push_back(true); 
		};
		
		char character = entry[indices[0]]; // the selected character
		int characterID = character; // the character ID
		
		if (48 <= characterID && characterID <= (48 + 9)) {
			handling['-'] = false; 
			
			// Forget all processing we did and add the number
			numbers[indices[1]] = abs(numbers[indices[1]]) * 10 + (characterID - 48);
			
			// Process it again
			if (!(positive[indices[1]])) {
				numbers[indices[1]] = 0 - numbers[indices[1]]; 
			};
		} else {
			switch (character) {
				case ',': 
				case ';': 
					// Done with the number. Process it. 
					handling['-'] = true; 
					indices[1]++; 
				case '-':
					if (handling['-']) {
						positive[indices[1]] = !(positive[indices[1]]); // Negate it
						break; 
					};
			}; 
		}; 
	}; 
	
	return numbers;
}; 

class Interface {
	public: 
		int L; // the maximum level
		int N; // the maximum child size
		vector<int> nodes; // the requested nodes for insertion
		vector<int> tasks; // tasks
		Dungeon *dungeon; // the dungeon
	
		/**
		 * Create an interface. 
		 */
		Interface() {}
		
		
		/**
		 * Create an interface with an existing dungeon. 
		 * 
		 * @param dungeon (Dungeon *) - the dungeon to use
		 */
		Interface(Dungeon* dungeon) {
			this->dungeon = dungeon; 
		}
		
		/**
		 * Create an interface with data. 
		 *
		 * @param L (int) - the maximum level
		 * @param N (int) - the maximum child size
		 * @param tasks (vector<int>) - the tasks 
		 * @param dungeon (Dungeon *) - the dungeon to import
		 */
		Interface(int L, int N, vector<int> tasks, Dungeon* dungeon = nullptr) {
			this->L = L; 
			this->N = N; 
			this->tasks = tasks; 
			this->dungeon = dungeon; 
		}; 
		
		/**
		 * Initialise the dungeon. 
		 * This method will use the attributes, such as that set in configure(). Note that the existing instance will be dropped but not removed from memory. 
		 * 
		 * @return Dungeon*
		 * @returns The dungeon instance
		 */
		Dungeon* initialize() {
			if (!(this->nodes.size())) {
				return nullptr; // Needs a head pointer
			};
			
			this->dungeon = new Dungeon((this->nodes)[0], this->N, this->L); 
			for (int node = 1; node < (this->nodes).size(); node++) {
				this->dungeon->insertcell((this->nodes)[node]); 
			};
			
			return this->dungeon; 
		};
		
		/**
		 * Receive configuration commands from input. 
		 * @param verbose (bool) - Controls the appearance of prompts
		 * @param initialise (bool) - Run initialisation automatically after configuration? 
		 */
		Dungeon* configure(bool verbose, bool initialise = false) {
			if (verbose) {cout << "\033[1m"<< "L" << "\033[0m\t" << ": ";}; 
			cin >> this->L; 
			
			if (verbose) {cout << "\033[1m"<< "N" << "\033[0m\t" << ": ";}; 
			cin >> this->N; 
			
			if (verbose) {cout << "\033[1m"<< "Nodes" << "\033[0m\t" << ": ";}; 
			string nodes_entry; 
			cin >> nodes_entry; 
			this->nodes = get_numbers(nodes_entry); 
			
			this->L = abs(this->L); 
			this->N = abs(this->N); 
			
			if (initialise) {
				if (verbose) {cout << "Initialization \033[1m";};
				Dungeon* status = this->initialize(); 
				
				if (verbose) {
					if (status) {
						cout << "\033[32m"; 
					} else {
						cout << "\033[31mun"; 
					}; 
					cout << "successful." << "\033[0m" << endl; 
				};
				return status; 
			}; 
			if (verbose) {cout << "Initialization " << "\033[1m\033[31m" << "unsuccessful" << "\033[0m" << "." << endl; };
			return nullptr; 
		};
		
		/**
		 * Query for user commands. 
		 * 
		 * @param verbose (bool) - Controls the appearance of prompts 
		 */
		vector<int> query(bool verbose) {
			int size; // the intended size
			if (verbose) {cout << "\033[1m"<< "Tasks" << "\033[0m\t" << ": ";}; 
			
			cin >> size; 
			size = abs(size); 
			
			vector<int> tasks; 
			for (int entry; tasks.size() < size; this->tasks = tasks) {
				if (verbose) {cout << "\t"<< "" << (tasks.size() + 1) << ".\033[0m\t";};
				cin >> entry; 
				entry = abs(entry); 
				
				if (0 <= entry && entry <= 6) {
					tasks.push_back(entry); // Add this entry
				};
			}; 
			
			return tasks; 
		}; 
		
		/**
		 * Execute following the specified commands. 
		 */
		void execute() {
			// Here are some variables that may be needed in the switch case statement. Initialising them inside seems to break it. 
			
			vector<DungeonNode*> nodes; // A collection if nodes
			vector<int> path; // The navigation to a certain node
			DungeonNode *maximum; // The maximum node (if applicable)
			
			for (int task = 0; task < (this->tasks).size(); task++) {
				if (0 <= abs((this->tasks)[task]) && abs((this->tasks)[task]) <= 6) {
					
					// switch for different tasks
					switch ((this->tasks)[task]) {
						case 1: 
							this->dungeon->traverse(); 
							break; 
						case 2: 
							this->dungeon->displaytree(); 
							break; 
						case 4: 
							this->dungeon->displaytree(false); 
							break; 
						case 5: 
							maximum = this->dungeon->extrema(1); 
							path = this->dungeon->largesttreasure(); 
							
							for (int direction = 0; direction < path.size(); direction++) {
								cout << path[direction] << "|"; 
							}; 
							cout << "=>" << maximum->getdata() << endl; 
							break; 
						case 6: 
							nodes = this->dungeon->extrema_path(1); 
							path = this->dungeon->bestpath(); 
							
							int sum = 0; 
							for (int pointer = 0; pointer < nodes.size(); pointer++) {
								sum += nodes[pointer]->getdata(); 
								
								if (pointer < path.size()) {
									cout << path[pointer] << "|"; 
								};
							};
							cout << "==>" << sum << endl; 
							break; 
					};
				};
			};
		};
}; 

int main() {
	Interface testing; 
	if (testing.configure(false, true)) {
		testing.query(true); 
		testing.execute(); 
	} 
};
