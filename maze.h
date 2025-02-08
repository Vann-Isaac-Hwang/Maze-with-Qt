#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include "log2txt.h"

class maze
{
public:
    maze(int row,int column);

    // Random number generation
    int getRand(int min, int max);

    // Maze Generation
    void SetAllUnvisited(); // Set all cells to be unvisited
    int ContiJudge(int r, int c); // Judge if there's available direction to go for the position, return 4 if not, else, randomly return a available direction
    void breakWall(int r, int c, int direction); // Break the wall and go to the new room
    void Go(int c, int r); // Recursive function, used to perform DFS maze generation
    void mazeGenDFS(); // Generate maze with DFS algorithm
    void mazeGenPrim(); // Generate maze with Prim algorithm
    void breakWallOnly(int r, int c, int direction); // Only break the wall
    void mazeGenKruskal(); // Generate maze with Kruskal algorithm

    // Maze Solution
    void mazeSolveDFS(); // Solve maze with DFS algorithm
    void recurPrint(int r, int c); // Print route recursively, used in mazeSolveBFS
    void mazeSolveBFS(); // Solve maze with BFS algorithm

    // Maze Save
    void mazeSave(std::string filename);

    // l,r,u,d for left,right,up,down side, 1: no wall, 0: blocked; visited: sign for visited; lastr,lastc: record last position of route in BFS solution
    struct cell
    {
        int l,r,u,d,visited,lastr,lastc;
    } **cell;
    int st_col,st_row,des_col,des_row;//Starting point and destination.

    // Route recorder
    struct Position{
        int c,r;
    }*Route;
    int RouteLen=0;

private:
    int row, column; // Maze size
    int cur_row, cur_column; // Current position
    log2txt *file; // Used to save maze to file
};

#endif // MAZE_H
