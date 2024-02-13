#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// Performs a topological sort of the graph using an iterative approach.
// It uses a stack to keep track of the nodes in topological order.
void topological_sort(int node, vector<vector<int>>& adjList, vector<bool>& visited, stack<int>& order) {
    stack<pair<int, int>> s;
    s.push(make_pair(node, 0));

    while (!s.empty()) {
        pair<int, int> current = s.top();
        s.pop();
        int cur = current.first;
        int idx = current.second;

        if (idx == 0) {
            visited[cur] = true;
        }

        for (; idx < static_cast<int>(adjList[cur].size()); idx++) {
            int neighbor = adjList[cur][idx];
            if (!visited[neighbor]) {
                s.push(make_pair(cur, idx + 1));
                s.push(make_pair(neighbor, 0));
                break;
            }
        }

        if (idx == static_cast<int>(adjList[cur].size())) {
            order.push(cur);
        }
    }
}


// Calculates the longest path in a directed acyclic graph (DAG). 
// It uses a topological sort of the SCCs and dynamic programming to compute the longest path.
int longest_path(vector<vector<int>>& componentsGraph, int startNode) {
    int numNodes = componentsGraph.size();
    vector<int> dp(numNodes, 0);

    // Perform topological sort to get the order of nodes
    stack<int> order;
    vector<bool> visited(numNodes, false);
    for (int i = 0; i < numNodes; i++) {
        if (!visited[i]) {
            topological_sort(i, componentsGraph, visited, order);
        }
    }

    // Initialize longest path to each node
    dp[startNode] = 0;

    // Iterate over the nodes in topological order
    while (!order.empty()) {
        int node = order.top();
        order.pop();

        for (int neighbor : componentsGraph[node]) {
            dp[neighbor] = max(dp[neighbor], dp[node] + 1);
        }
    }

    // Find the maximum value in dp array
    return *max_element(dp.begin(), dp.end());
}


// Implements an iterative depth-first search (DFS) to fill the order stack with nodes in reverse topological order.
void dfs(int node, vector<vector<int>>& adjList, vector<bool>& visited, stack<int>& order) {
    stack<pair<int, int>> s;
    s.push(make_pair(node, 0));

    while (!s.empty()) {
        pair<int, int> current = s.top();
        s.pop();
        int cur = current.first;
        int idx = current.second;

        if (idx == 0) {
            visited[cur] = true;
        }

        for (; idx < static_cast<int>(adjList[cur].size()); idx++) {
            int neighbor = adjList[cur][idx];
            if (!visited[neighbor]) {
                s.push(make_pair(cur, idx + 1));
                s.push(make_pair(neighbor, 0));
                break;
            }
        }

        if (idx == static_cast<int>(adjList[cur].size())) {
            order.push(cur);
        }
    }
}


// Performs a DFS on the transpose graph to identify strongly connected components (SCCs). It assigns each node to its corresponding SCC.
void dfs_scc(int node, vector<vector<int>>& transposeAdjList, vector<bool>& visited, vector<int>& component, int componentNumber) {
    stack<pair<int, int>> s;
    s.push(make_pair(node, 0));

    while (!s.empty()) {
        pair<int, int> current = s.top();
        s.pop();
        int cur = current.first;
        int idx = current.second;

        if (idx == 0) {
            visited[cur] = true;
            component[cur] = componentNumber;
        }

        for (; idx < static_cast<int>(transposeAdjList[cur].size()); idx++) {
            int neighbor = transposeAdjList[cur][idx];
            if (!visited[neighbor]) {
                s.push(make_pair(cur, idx + 1));
                s.push(make_pair(neighbor, 0));
                break;
            }
        }
    }
}


// Finds the strongly connected components in the graph and returns a vector of vectors (components), where each vector represents a strongly connected component. Kosaraju algorithm.
vector<vector<int>> findSCCs(vector<vector<int>>& adjList, vector<vector<int>>& transposeAdjList, int numNodes) {
    vector<bool> visited(numNodes, false);
    stack<int> order;

    // Perform DFS to fill the order stack
    for (int i = 0; i < numNodes; i++) {
        if (!visited[i]) {
            dfs(i, adjList, visited, order);
        }
    }

    // Transpose the graph
    vector<int> component(numNodes, -1);
    vector<vector<int>> components;

    // Reset visited array
    fill(visited.begin(), visited.end(), false);

    // Perform DFS on the transpose graph using the order from the stack
    int componentNumber = 0;
    while (!order.empty()) {
        int node = order.top();
        order.pop();

        if (!visited[node]) {
            vector<int> currentComponent;
            dfs_scc(node, transposeAdjList, visited, component, componentNumber++);
        }
    }

    // Create components graph
    components.resize(componentNumber);
    for (int i = 0; i < numNodes; i++) {
        for (int neighbor : adjList[i]) {
            if (component[i] != component[neighbor]) {
                components[component[i]].push_back(component[neighbor]);
            }
        }
    }

    return components;
}


/*
The overall logic is to first find the strongly connected components in the graph using Kosaraju's algorithm.
Then, it transforms the graph into a new graph where each SCC becomes a single node.
We do this because of the simplifying assumption: individuals who know each other directly or indirectly (cycles) become infected instantly.
Finally, it calculates the longest path in the transformed graph, which corresponds to the longest path in the original graph.
The iterative approach in DFS and topological sort ensures efficient traversal of the graph.
*/
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int number_of_people, number_of_relationships;

    cin >> number_of_people >> number_of_relationships;

    // Create adjacency lists with n vertices, for the original graph and its transpose
    vector<vector<int>> adjacencyList(number_of_people);
    vector<vector<int>> transposeAdjList(number_of_people);

    // Read and populate the adjacency lists
    for (int i = 0; i < number_of_relationships; i++) {
        int x, y;

        cin >> x >> y;

        // Adjust indices to start from 0
        // (we use a 0-based index for adjacency lists, assuming that individuals are 1-indexed in the input)
        x--;
        y--;

        adjacencyList[x].push_back(y);
        transposeAdjList[y].push_back(x);
    }

    // Find strongly connected components
    vector<vector<int>> components = findSCCs(adjacencyList, transposeAdjList, number_of_people);

    // Now 'components' contains the SCCs of the graph

    // Find the longest path in the components graph
    // Assuming 0 is the starting node
    int longestPathLength = longest_path(components, 0);
    cout << longestPathLength << endl;

    return 0;
}
