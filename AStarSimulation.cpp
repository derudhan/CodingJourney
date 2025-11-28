#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int x, y;
    bool isWalkable;
    char symbol;  // Visualisasi

    float gCost;
    float hCost;
    float fCost;
    Node* parent;

    Node() { gCost = 99999, fCost = 99999, parent = nullptr; }
};

class Grid {
   private:
    vector<vector<Node>> map;
    int width;
    int height;

    Node* startTile;
    Node* targetTile;

   public:
    Grid(int x, int y) {
        width = x;
        height = y;

        map.resize(height);

        for (int y = 0; y < height; y++) {
            map[y].resize(width);
            for (int x = 0; x < width; x++) {
                map[y][x].x = x;
                map[y][x].y = y;
                map[y][x].isWalkable = true;
                map[y][x].symbol = '-';
            }
        }
    };

    void SetTileStart(int x, int y) { startTile = SetTile(x, y, true, 'S'); };
    Node* GetTileStart() {
        if (startTile != nullptr) return startTile;
        return nullptr;
    };

    void SetTileTarget(int x, int y) { targetTile = SetTile(x, y, true, 'T'); };
    Node* GetTileTarget() {
        if (targetTile != nullptr) return targetTile;
        return nullptr;
    };

    void SetTileWalkable(int x, int y, bool walkable) {
        SetTile(x, y, walkable, walkable ? '-' : '#');
    };

    Node* GetTile(int x, int y) { return &map[y][x]; };

    vector<Node*> GetNeighbors(int x, int y) {
        vector<Node*> neighbors;
        if (!IsValidCoordinate(x, y)) return neighbors;

        if (IsValidCoordinate(x + 1, y) && map[y][x + 1].isWalkable)
            neighbors.push_back(&map[y][x + 1]);  // Right
        if (IsValidCoordinate(x - 1, y) && map[y][x - 1].isWalkable)
            neighbors.push_back(&map[y][x - 1]);  // Left
        if (IsValidCoordinate(x, y + 1) && map[y + 1][x].isWalkable)
            neighbors.push_back(&map[y + 1][x]);  // Top
        if (IsValidCoordinate(x, y - 1) && map[y - 1][x].isWalkable)
            neighbors.push_back(&map[y - 1][x]);  // Bottom

        return neighbors;
    };
    vector<Node*> GetNeighbors(Node* targetTile) {
        return GetNeighbors(targetTile->x, targetTile->y);
    }

    int GetDistance(int x1, int y1, int x2, int y2) {
        if (!IsValidCoordinate(x1, y1) || !IsValidCoordinate(x2, y2)) return -1;
        return abs(x1 - x2) + abs(y1 - y2);  // Manhattan Distance
    }
    int GetDistance(Node* startTile, Node* targetTile) {  // Overload
        return GetDistance(startTile->x, startTile->y, targetTile->x,
                           targetTile->y);
    }

    void DrawMap() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << map[y][x].symbol << " ";
            }
            cout << endl;
        }
    };

   private:
    Node* SetTile(int x, int y, bool walkable, char symbol) {
        map[y][x].isWalkable = walkable;
        map[y][x].symbol = symbol;
        return &map[y][x];
    }
    bool IsValidCoordinate(int x, int y) {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }
};

class AStar {
   private:
    Grid* grid;
    vector<Node*> openSet;
    vector<Node*> closeSet;

   public:
    AStar(Grid* g) { grid = g; }

    void FindPath() {
        Node* startTile = grid->GetTileStart();
        Node* targetTile = grid->GetTileTarget();

        // Init startTile
        startTile->gCost = 0;
        startTile->hCost = grid->GetDistance(startTile, targetTile);
        startTile->fCost = startTile->gCost + startTile->hCost;

        // Start finding path
        openSet.push_back(grid->GetTileStart());
        while (!openSet.empty()) {
            Node* currentNode = FindSmallestFCost(openSet);
            RemoveFromOpenSet(currentNode);
            closeSet.push_back(currentNode);  // Mark this node to be closed

            if (currentNode == targetTile) {
                cout << "Found the way!" << endl;
                PrintPath(GetPath(currentNode));
                return;
            }

            vector<Node*> neigbors = grid->GetNeighbors(currentNode);
            for (auto neighbor : neigbors) {
                if (NodeExistInSet(neighbor, closeSet)) continue;

                float newMovementCostToNeighbor =
                    currentNode->gCost +
                    grid->GetDistance(currentNode, neighbor);

                if (newMovementCostToNeighbor < neighbor->gCost) {
                    neighbor->gCost = newMovementCostToNeighbor;
                    neighbor->hCost = grid->GetDistance(neighbor, targetTile);
                    neighbor->fCost = neighbor->gCost + neighbor->hCost;
                    neighbor->parent = currentNode;

                    if (!NodeExistInSet(neighbor, openSet)) {
                        openSet.push_back(neighbor);
                    }
                }
            }
        }
    }

   private:
    Node* FindSmallestFCost(vector<Node*> nodes) {
        Node* currentNode = nodes[0];
        for (int i = 1; i < nodes.size(); i++) {
            if (nodes[i]->fCost < currentNode->fCost ||
                (nodes[i]->fCost == currentNode->fCost &&
                 nodes[i]->hCost < currentNode->hCost)) {
                currentNode = nodes[i];
            }
        }
        return currentNode;
    }

    // Find Node in OpenSet vector then erase it
    void RemoveFromOpenSet(Node* node) {
        auto new_end = remove(openSet.begin(), openSet.end(), node);
        openSet.erase(new_end);
    }

    // Check if targetNode is exist in specific set
    bool NodeExistInSet(Node* targetNode, vector<Node*> set) {
        return find(set.begin(), set.end(), targetNode) != set.end();
    }

    vector<Node*> GetPath(Node* node) {
        vector<Node*> nodes;
        Node* currentNode = node;
        while (currentNode->parent != nullptr) {
            if (currentNode != grid->GetTileTarget()) currentNode->symbol = 'o';
            nodes.push_back(currentNode);
            currentNode = currentNode->parent;
        }
        return nodes;
    }

    void PrintPath(vector<Node*> nodes) {
        for (int i = nodes.size() - 1; i >= 0; i--) {
            cout << "(" << nodes[i]->x << ", " << nodes[i]->y << ") ";
            (i == 0) ? cout << "" : cout << "> ";
        }
        cout << endl;
    }
};

int main() {
    // Setup Grid
    Grid grid = Grid(5, 5);

    grid.SetTileStart(0, 4);
    grid.SetTileTarget(4, 4);
    grid.SetTileWalkable(3, 0, false);
    grid.SetTileWalkable(1, 1, false);
    grid.SetTileWalkable(3, 1, false);
    grid.SetTileWalkable(1, 2, false);
    grid.SetTileWalkable(3, 3, false);
    grid.SetTileWalkable(3, 4, false);

    grid.DrawMap();
    // - - - # -
    // - # - # -
    // - # - - -  || Expected Output Grid
    // - - - # -
    // S - - # T

    // Checking Valid Neighbors (4 directions)
    Node* tile = grid.GetTile(1, 3);
    cout << "Total Valid Neighbors for (" + to_string(tile->x) + ", " +
                to_string(tile->y) + ") is "
         << grid.GetNeighbors(tile).size() << endl;

    // Checking Distance from Start to Target
    Node* startTile = grid.GetTileStart();
    Node* targetTile = grid.GetTileTarget();
    float distance = grid.GetDistance(startTile, targetTile);
    cout << "Distance from (" + to_string(startTile->x) + ", " +
                to_string(startTile->y) + ") to (" + to_string(targetTile->x) +
                ", " + to_string(targetTile->y) + ") is "
         << distance << endl;

    // A-Star
    AStar aStar = AStar(&grid);
    aStar.FindPath();
    grid.DrawMap();
    // - - - # -
    // - # - # -
    // - # o o o // Expected Output Path Grid
    // - - o # o
    // S o o # T
}