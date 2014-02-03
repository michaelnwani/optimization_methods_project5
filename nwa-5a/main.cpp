// Project 5

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <queue>

using namespace std;

class maze
{
public:
    maze(ifstream &fin);
    
    int numRows(){return rows;};
    int numCols(){return cols;};
    
    void print(int,int,int,int);
    bool isLegal(int i, int j);
    
    void setMap(int i, int j, int n);
    int getMap(int i, int j) const;
    int getReverseMapI(int n) const;
    int getReverseMapJ(int n) const;
    bool findShortestPath1(int start, graph &g);
    bool findShortestPath2(int start, graph &g);
    void mapMazeToGraph(graph &g);
    void findPathRecursive(int i, int j, graph &g);
    void findPathNonRecursive(int i, int j, graph &g);
    
    
private:
    int rows; // number of rows in the maze
    int cols; // number of columns in the maze
    
    matrix<bool> value;
    matrix<int> map;
    
};


void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.
{
    map[i][j] = n;
}

int maze ::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
    return map[i][j];
}

int maze ::getReverseMapI(int n) const
// Return reverse mapping of node n to it's maze i value.
{
    for (int i = 0; i < map.rows(); i++){
        for (int j = 0; j < map.cols(); j++){
            if (map[i][j] == n){
                return i;
            }
        }
    }
}

int maze ::getReverseMapJ(int n) const
// Return reverse mapping of node n to it's maze j value.
{
    for (int i = 0; i < map.rows(); i++){
        for (int j = 0; j < map.cols(); j++){
            if (map[i][j] == n){
                return j;
            }
        }
    }
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
    fin >> rows;
    fin >> cols;
    
    char x;
    
    value.resize(rows,cols);
    for (int i = 0; i <= rows-1; i++)
        for (int j = 0; j <= cols-1; j++)
        {
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;
        }
    
  map.resize(rows,cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
    cout << endl;
    
    if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
        throw rangeError("Bad value in maze::print");
    
    if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
        throw rangeError("Bad value in maze::print");
    
    for (int i = 0; i <= rows-1; i++)
    {
        for (int j = 0; j <= cols-1; j++)
        {
            if (i == goalI && j == goalJ)
                cout << "*";
            else
                if (i == currI && j == currJ)
                    cout << "+";
                else
                    if (value[i][j])
                        cout << " ";
                    else
                        cout << "X";
        }
        cout << endl;
    }
    cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to occupy cell (i,j).
{
    if (i < 0 || i > rows || j < 0 || j > cols)
        throw rangeError("Bad value in maze::isLegal");
    
    return value[i][j];
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    int nodeOrder = 0;
    for (int i = 0; i <= value.rows()-1; i++){
        for (int j = 0; j <= value.cols()-1; j++){
            if (value[i][j] == 1){
                g.addNode();
                setMap(i, j, nodeOrder);
                nodeOrder++;
                
            }
        }
    }
    
    for (int i = 1; i <= value.rows()-1; i++){
        for (int j = 0; j <= value.cols()-1; j++){
            if(value[i-1][j] == 1 && value[i][j] == 1){
                g.addEdge(getMap(i-1, j), getMap(i, j));
            }
        }
    }
    
    for (int i = 0; i <= value.rows()-2; i++){
        for (int j = 0; j <= value.cols()-1; j++){
            if(value[i+1][j] == 1 && value[i][j] == 1){
                g.addEdge(getMap(i+1, j), getMap(i, j));
            }
        }
    }
   
    for (int i = 0; i <= value.rows()-1; i++){
        for (int j = 1; j <= value.cols()-1; j++){
            if(value[i][j-1] == 1 && value[i][j] == 1){
                g.addEdge(getMap(i, j-1), getMap(i, j));
            }
        }
    }
    
    for (int i = 0; i <= value.rows()-1; i++){
        for (int j = 0; j <= value.cols()-2; j++){
            if(value[i][j+1] == 1 && value[i][j] == 1){
                g.addEdge(getMap(i, j+1), getMap(i, j));
            }
        }
    }
    
    
}

bool maze::findShortestPath1(int start, graph &g){
    
    //Depth First Traversal
    
    if (getReverseMapI(start) == (rows-1) && getReverseMapJ(start) == (cols-1))
    {
        
        cout << "Solved through Depth First Traversal" << endl;
     
        return true;
        
    }
    
    // Mark the start node as visited.
   g.visit(start);
    
    int v = 0;
    
    
    // Keep looking for legal moves as long as there are more neighbors
    // to check.
    
    while (v < g.numNodes())
    {
        // if v is an unvisited neighbor of the start node, recurse.
        
        if (g.isEdge(start,v) && !g.isVisited(v)){
            findShortestPath1(v,g); //visiting v each time.
            print((rows-1), (cols-1), getReverseMapI(start), getReverseMapJ(start)); //printing our current location in the maze
            
        }
        
        v++;
    }
    
    return false; //Default, for if no path to the goal was found.
    
}

bool maze::findShortestPath2(int start, graph &g){
    
    //Breadth First Traversal
    //unvisit every node
    
    
    queue<int> node;
       
    g.clearVisit();
    
    
    
    //Start node gets pushed to the front of the queue
    node.push(start);
    g.visit(start);
    while (!node.empty()){
        start = node.front();

        //Every edge around this node gets searched and visited
        for (int v = 0; v < g.numNodes(); v++){
            if (g.isEdge(start,v) && !g.isVisited(v))
            {
            g.visit(v);
            node.push(v);
            print((rows-1), (cols-1), getReverseMapI(start), getReverseMapJ(v)); //printing our current location in the maze
                
            }
            if (getReverseMapI(start) == (rows-1) && getReverseMapJ(start) == (cols-1))
            {
                
                cout << "Solved through Breadth First Traversal" << endl;
                
                return true;
                
            }
            
        
        }
        //Once the edges around this node have been exhausted, the node gets popped from the front of the queue
        node.pop();
        //If the node becomes empty, it means every node has been exhausted/visited.

    }
    
    return false; //Default, for if no path to the goal was found.
}

/*void maze::findPathRecursive(int i, int j, graph &g){
    
    if (getMap(i, j) == getMap((rows-1), (cols-1))){
        cout << "\nRecursive Approach: \n";
        return print((rows-1), (cols-1), i, j);
    }
        
    if (g.isEdge(i, j+1)){
        findPathRecursive(i, j+1, g);
    }
    if (g.isEdge(i, j-1)){
        findPathRecursive(i, j-1, g);
    }
    if (g.isEdge(i-1, j)){
        findPathRecursive(i-1, j, g);
    }
    if (g.isEdge(i+1, j)){
        findPathRecursive(i+1, j, g);
    }
}

 */
/*void maze::findPathNonRecursive(int i, int j, graph &g){
    
    if (getMap(i, j) != getMap((rows-1), (cols-1))){
        if (g.isEdge(i, j+1) && g.isEdge(i, j)){
            j++;
        }
        if (g.isEdge(i, j-1) && g.isEdge(i, j)){
            j--;
        }
        if (g.isEdge(i-1, j) && g.isEdge(i, j)){
            i--;
        }
        if (g.isEdge(i+1, j) && g.isEdge(i, j)){
            i++;
        }
    }
    else
    {
        cout << "\nNonRecursive Approach: \n";
        return print((rows-1), (cols-1), i, j);
    }

}
 
 */
int main()
{
    
    ifstream fin;
    
    // Read the maze from the file. (Change to proper maze file location)
    string fileName = "/Users/michaelnwani/Desktopstuff/maze1.txt";
    
    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    
    try
    {
        graph g;
        while (fin && fin.peek() != 'Z')
        {
            maze m(fin);
            m.mapMazeToGraph(g);
            
          
            //m.findShortestPath1(0, g);
            m.findShortestPath2(0, g);
        }
        
        
    } 
    catch (indexRangeError &ex) 
    { 
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}
