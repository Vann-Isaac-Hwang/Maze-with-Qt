#include "maze.h"

maze::maze(int row,int column):row(row),column(column) {

    // Allocate memory to cells
    cell=new struct cell*[row];
    for (int i=0;i<row;i++){
        cell[i] = new struct cell[column];
    }

    // Random generator use time as seed
    srand(time(0));

    // Initialize starting row, column and destination row, column
    st_col=0,st_row=0,des_col=column-1,des_row=row-1;

    // Initialize walls
    for (int i=0;i<row;i++)
    {
        for (int j=0;j<column;j++)
        {
            cell[i][j].l=0;//1 for connected
            cell[i][j].r=0;
            cell[i][j].u=0;
            cell[i][j].d=0;
        }
    }

    SetAllUnvisited();
}

void maze::SetAllUnvisited()
{
    // Set all cells unvisited.
    for(int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            cell[i][j].visited=0;
        }
    }
}

int maze::getRand(int min,int max)
{
    return (rand() % (max-min+1))+min;
}

int maze::ContiJudge(int r, int c) // l:0, r:1, u:2, d:3, Stop:4.
{
    int dir[4], size=0;
    int l_flag=1,r_flag=1,u_flag=1,d_flag=1;
    // Edge Circumstances or visited.
    if (r==0 || cell[r-1][c].visited) u_flag=0;
    if (r==row-1 || cell[r+1][c].visited) d_flag=0;
    if (c==0 || cell[r][c-1].visited) l_flag=0;
    if (c==column-1 || cell[r][c+1].visited) r_flag=0;

    if (l_flag==1) dir[size++]=0;
    if (r_flag==1) dir[size++]=1;
    if (u_flag==1) dir[size++]=2;
    if (d_flag==1) dir[size++]=3;

    int sum=l_flag+r_flag+u_flag+d_flag; // Ways
    // Can't continue
    if (sum==0) return 4;
    // Can continue, select one randomly
    int op=getRand(0,size-1);

    return dir[op];
}

void maze::breakWallOnly(int r, int c, int direction)
{
    // std::cout<<r<<" "<<c<<" break ";
    switch (direction) {
    case 0:
        // std::cout<<"l"<<std::endl;
        cell[r][c].l=1;
        cell[r][c-1].r=1;
        break;
    case 1:
        // std::cout<<"r"<<std::endl;
        cell[r][c].r=1;
        cell[r][c+1].l=1;
        break;
    case 2:
        // std::cout<<"u"<<std::endl;
        cell[r][c].u=1;
        cell[r-1][c].d=1;
        break;
    case 3:
        // std::cout<<"d"<<std::endl;
        cell[r][c].d=1;
        cell[r+1][c].u=1;
        break;
    default:
        std::cout<<"Unknown Direction (breakWall)!"<<std::endl;
        break;
    }
}

void maze::breakWall(int r, int c, int direction)
{
    // std::cout<<r<<" "<<c<<" break ";
    switch (direction) {
    case 0:
        // std::cout<<"l"<<std::endl;
        cell[r][c].l=1;
        cell[r][c-1].r=1;
        cell[r][c-1].visited=1;
        cur_column--;
        break;
    case 1:
        // std::cout<<"r"<<std::endl;
        cell[r][c].r=1;
        cell[r][c+1].l=1;
        cell[r][c+1].visited=1;
        cur_column++;
        break;
    case 2:
        // std::cout<<"u"<<std::endl;
        cell[r][c].u=1;
        cell[r-1][c].d=1;
        cell[r-1][c].visited=1;
        cur_row--;
        break;
    case 3:
        // std::cout<<"d"<<std::endl;
        cell[r][c].d=1;
        cell[r+1][c].u=1;
        cell[r+1][c].visited=1;
        cur_row++;
        break;
    default:
        std::cout<<"Unknown Direction (breakWall)!"<<std::endl;
        break;
    }
}

void maze::Go(int r, int c)
{
    int direction;
    // Get direction
    int dir[4], size=0;
    int l_flag=1,r_flag=1,u_flag=1,d_flag=1;
    // Edge Circumstances or visited.
    if (r==0 || cell[r-1][c].visited) u_flag=0;
    if (r==row-1 || cell[r+1][c].visited) d_flag=0;
    if (c==0 || cell[r][c-1].visited) l_flag=0;
    if (c==column-1 || cell[r][c+1].visited) r_flag=0;

    // Add available directions to the list dir
    if (l_flag==1) dir[size++]=0;
    if (r_flag==1) dir[size++]=1;
    if (u_flag==1) dir[size++]=2;
    if (d_flag==1) dir[size++]=3;
    // Number of available directions
    int sum=l_flag+r_flag+u_flag+d_flag;

    if (sum==0) return;// Can't continue, quit

    else // Can continue, select one randomly
    {
        // Reorder all the available directions
        std::random_shuffle(dir, dir+size);
        for (int op=0;op<size;op++)
        {
            // Select a random direction to break wall each time
            direction=dir[op];

            switch (direction) {
            case 0: // Left
                if (cell[r][c-1].visited) continue;
                // std::cout<<"l"<<std::endl;
                cell[r][c].l=1;
                cell[r][c-1].r=1;
                cell[r][c-1].visited=1;
                Go(r, c-1);
                break;
            case 1: // Right
                if (cell[r][c+1].visited) continue;
                // std::cout<<"r"<<std::endl;
                cell[r][c].r=1;
                cell[r][c+1].l=1;
                cell[r][c+1].visited=1;
                Go(r, c+1);
                break;
            case 2: // Up
                if (cell[r-1][c].visited) continue;
                // std::cout<<"u"<<std::endl;
                cell[r][c].u=1;
                cell[r-1][c].d=1;
                cell[r-1][c].visited=1;
                Go(r-1, c);
                break;
            case 3: // Down
                if (cell[r+1][c].visited) continue;
                // std::cout<<"d"<<std::endl;
                cell[r][c].d=1;
                cell[r+1][c].u=1;
                cell[r+1][c].visited=1;
                Go(r+1, c);
                break;
            default:
                std::cout<<"Unknown Direction (breakWall)!"<<std::endl;
                break;
            }
        }
    }
}

void maze::mazeGenDFS()
{
    // Set all cells unvisited.
    SetAllUnvisited();
    //Initialize current position.
    cur_row=st_row, cur_column=st_col;
    cell[cur_row][cur_column].visited=1;
    // Call recursive function to break walls randomly
    Go(cur_row,cur_column);
}

void maze::mazeGenPrim()
{
    // Set all cells unvisited.
    SetAllUnvisited();
    //Store all edges in pair
    struct edge {
        int c0,r0,c1,r1,f;
    } *pair;
    int size=(row-1)*column+(column-1)*row;
    pair=new struct edge[size];
    int k=0;
    for(int i=0;i<row;i++)
    {
        for (int j=0;j<column-1;j++)
        {
            pair[k].r0=i;
            pair[k].c0=j;
            pair[k].r1=i;
            pair[k].c1=j+1;
            pair[k].f=0;
            k++;
        }
    }
    for(int i=0;i<column;i++)
    {
        for (int j=0;j<row-1;j++)
        {
            pair[k].r0=j;
            pair[k].c0=i;
            pair[k].r1=j+1;
            pair[k].c1=i;
            pair[k].f=0;
            k++;
        }
    }

    // Reorder the data in pair randomly
    std::random_shuffle(pair, pair+size);

    // Set all cells unvisited.
    for(int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            cell[i][j].visited=0;
        }
    }

    // Initialize current position
    cur_row=st_row, cur_column=st_col;
    cell[cur_row][cur_column].visited=1;

    while (1)
    {
        for (int i=0;i<size;i++) // For each edge
        {
            if (pair[i].f==1) continue; // The edge had been considered before
            if (cell[pair[i].r0][pair[i].c0].visited+cell[pair[i].r1][pair[i].c1].visited==1) // Only one vertex of the edge is visited
            {
                // Set the two verteces of the edge to be visited
                cell[pair[i].r0][pair[i].c0].visited=1;
                cell[pair[i].r1][pair[i].c1].visited=1;
                // Set the edge to be visited
                pair[i].f=1;
                // Break corresponding wall between the two vertices of the edge
                if (pair[i].r0<pair[i].r1) breakWall(pair[i].r0, pair[i].c0, 3);
                else if (pair[i].r0>pair[i].r1) breakWall(pair[i].r0, pair[i].c0, 2);
                else if (pair[i].c0<pair[i].c1) breakWall(pair[i].r0, pair[i].c0, 1);
                else if (pair[i].c0>pair[i].c1) breakWall(pair[i].r0, pair[i].c0, 0);
            }
        }
        // Judge if all verteces are in the tree
        int f=1; // f=1: All cells are visited, maze is built
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (cell[i][j].visited==0) // There's cell which is not visited
                {
                    f=0;
                    break;
                }
            }
        }
        if (f==1) break;
    }
}

void maze::mazeGenKruskal()
{
    /* Method description
     * In this algorithm, the air pressure of cells are regarded different,
    select two adjacent cells, if their pressures are different, break the wall between them,
    change all the cells whose pressure is the same as the higher one, to the same as the lower one,
    when all the cells' pressure are same, a minimum spanning tree is created with the Kruskal's algorithm.*/

    // Set all cells' pressure to be different
    int pressure=1;
    for(int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            cell[i][j].visited=pressure++;
            // std::cout<<pressure<<std::endl;
        }
    }

    while (1)
    {
        // Get a random position(x,y) and a random direction(s)
        int x=getRand(0,row-1);
        int y=getRand(0,column-1);
        int s=getRand(0,3);

        // if the position pointed by (x,y) is outside the maze, just change it to the opposite direction
        if (x==0&&s==2) s=3;
        if (x==row-1&&s==3) s=2;
        if (y==0&&s==0) s=1;
        if (y==column-1&&s==1) s=0;

        // The position pointed by (x,y) is (x1,y1), get x1,y1
        int x1=x,y1=y;
        switch (s) {
        case 0:
            x1=x,y1=y-1;
            break;
        case 1:
            x1=x,y1=y+1;
            break;
        case 2:
            x1=x-1,y1=y;
            break;
        case 3:
            x1=x+1,y1=y;
            break;
        default:
            std::cout<<"Wrong direction!"<<std::endl;
            break;
        }

        // If (x1,y1) is visited, choose another position and direction
        if (cell[x][y].visited==cell[x1][y1].visited) continue;

        // (x1,y1) is not visited, break the corresponding wall
        if (x<x1) breakWallOnly(x, y, 3);
        else if (x>x1) breakWallOnly(x, y, 2);
        else if (y<y1) breakWallOnly(x, y, 1);
        else if (y>y1) breakWallOnly(x, y, 0);

        // Change all the cells whose pressure is the same as the higher one, to the same as the lower one
        // Get low and high pressure
        int low=cell[x][y].visited, high=cell[x1][y1].visited;
        if (low>high){
            int mid=low;
            low=high,high=mid;
        }
        // Refresh pressure
        for(int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (cell[i][j].visited==high)
                    cell[i][j].visited=low;
            }
        }

        int f=1; // f=1: Maze is built
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (cell[i][j].visited!=1)
                {
                    f=0; // There's a cell whose pressure is not the same as the starting point
                    goto outside;
                }
            }
        }
        outside:
        if (f==1) break;
    }
}

void maze::mazeSolveDFS()
{
    // Set all cells unvisited.
    SetAllUnvisited();

    // Reset route (Route serves as stack there)
    if (Route!=NULL) Route=NULL;
    Route=new Position[row*column];
    int size=0;

    // Initialize current position
    cur_row=st_row, cur_column=st_col;
    cell[st_row][st_col].visited=1;
    int f=0; // f=1: Solution found
    Route[0].r=cur_row, Route[0].c=cur_column;
    size++;
    while (1)
    {
        // Select a direction
        int direction;
        if (cell[cur_row][cur_column].l==1&&cell[cur_row][cur_column-1].visited==0) direction=0; // Left
        else if (cell[cur_row][cur_column].r==1&&cell[cur_row][cur_column+1].visited==0) direction=1; // Right
        else if (cell[cur_row][cur_column].u==1&&cell[cur_row-1][cur_column].visited==0) direction=2; // Up
        else if (cell[cur_row][cur_column].d==1&&cell[cur_row+1][cur_column].visited==0) direction=3; // Down
        else direction=4; // No way to go

        if (direction==4) { // No way
            size--; // Pop out the last position in the route
            if (size<=0) break; // No element left in the stack
            // Move backward
            cur_row=Route[size-1].r;
            cur_column=Route[size-1].c;
        }
        else // Move ahead
        {
            switch (direction) {
            case 0: // Left
                cur_column--;
                break;
            case 1: // Right
                cur_column++;
                break;
            case 2: // Up
                cur_row--;
                break;
            case 3: // Down
                cur_row++;
                break;
            default:
                break;
            }
            // Sign current point visited
            cell[cur_row][cur_column].visited=1;
            // Push current point into the stack of route
            Route[size].r=cur_row, Route[size].c=cur_column, size++;

            if (cur_row==des_row&&cur_column==des_col) // Destination reached
            {
                f=1;
                break;
            }
        }
    }
    if (f)
    {
        RouteLen=size;
        std::cout<<"Solved"<<std::endl;
    }
    else
        std::cout<<"Solution not found"<<std::endl;
}

void maze::recurPrint(int r, int c)
{
    if (r==st_row&&c==st_col) // Trace back to the starting point
    {
        Route[RouteLen].r=r, Route[RouteLen].c=c, RouteLen++;
        // std::cout<<r<<","<<c<<"->";
    }
    else // Not trace back to the starting point yet
    {
        recurPrint(cell[r][c].lastr, cell[r][c].lastc);
        Route[RouteLen].r=r, Route[RouteLen].c=c, RouteLen++;
        // std::cout<<r<<","<<c<<"->";
    }
}

void maze::mazeSolveBFS()
{
    // Set all cells unvisited.
    for(int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            cell[i][j].visited=0;
        }
    }

    // Create a queue to perform BFS
    Position *Queue=new Position[row*column];
    int head=0, tail=0;
    // Enqueue starting point
    Queue[tail].r=st_row, Queue[tail].c=st_col, tail++;
    // Initialize current point to the starting point
    cur_row=st_row, cur_column=st_col;
    // Visited starting point
    cell[cur_row][cur_column].visited=1;

    int f=0; // f=1: Found solution

    while (head!=tail)
    {
        // Enque all adjacent positions
        if (cell[cur_row][cur_column].l==1&&cell[cur_row][cur_column-1].visited==0) // Leftside is not wall and is not visited
        {
            // Record for trace back
            cell[cur_row][cur_column-1].lastr=cur_row;
            cell[cur_row][cur_column-1].lastc=cur_column;
            // Mark
            cell[cur_row][cur_column-1].visited=1;
            // Enqueue
            Queue[tail].r=cur_row;
            Queue[tail].c=cur_column-1;
            tail++;
        }
        if (cell[cur_row][cur_column].r==1&&cell[cur_row][cur_column+1].visited==0) // Rightside is not wall and is not visited
        {
            // Record for trace back
            cell[cur_row][cur_column+1].lastr=cur_row;
            cell[cur_row][cur_column+1].lastc=cur_column;
            // Mark
            cell[cur_row][cur_column+1].visited=1;
            // Enqueue
            Queue[tail].r=cur_row;
            Queue[tail].c=cur_column+1;
            tail++;
        }
        if (cell[cur_row][cur_column].u==1&&cell[cur_row-1][cur_column].visited==0) // Upside is not wall and is not visited
        {
            // Record for trace back
            cell[cur_row-1][cur_column].lastr=cur_row;
            cell[cur_row-1][cur_column].lastc=cur_column;
            // Mark
            cell[cur_row-1][cur_column].visited=1;
            // Enqueue
            Queue[tail].r=cur_row-1;
            Queue[tail].c=cur_column;
            tail++;
        }
        if (cell[cur_row][cur_column].d==1&&cell[cur_row+1][cur_column].visited==0) // Downside is not wall and is not visited
        {
            // Record for trace back
            cell[cur_row+1][cur_column].lastr=cur_row;
            cell[cur_row+1][cur_column].lastc=cur_column;
            // Mark
            cell[cur_row+1][cur_column].visited=1;
            // Enqueue
            Queue[tail].r=cur_row+1;
            Queue[tail].c=cur_column;
            tail++;
        }

        // Dequeue
        head++;
        cur_row=Queue[head].r;
        cur_column=Queue[head].c;

        // If destination is reached, quit
        if (Queue[head-1].r==des_row&&Queue[head-1].c==des_col)
        {
            std::cout<<"Solution Found"<<std::endl;
            f=1;
            break;
        }
        else
            std::cout<<"Solution not found"<<std::endl;
        // std::cout<<head<<" "<<tail<<std::endl;
    }

    // Print out route
    if (f)
    {
        // Reset route record
        if (Route!=NULL) Route=NULL;
        Route=new Position[row*column];
        RouteLen=0;
        // Print and record the route recursively
        recurPrint(cell[des_row][des_col].lastr, cell[des_row][des_col].lastc);
        Route[RouteLen].r=des_row, Route[RouteLen].c=des_col, RouteLen++; // Record the last position
    }

}

void maze::mazeSave(std::string filename)
{
    file = new log2txt(filename+".iskmaz");
    file->clear();
    file->log_n(std::to_string(row)+","+std::to_string(column));
    for (int i=0;i<row;i++)
    {
        for (int j=0;j<column;j++)
        {
            // Write to file, first line is the size of maze (row,column), and the following lines are cells' connectivities(row,column,left,right,up,down)
            file->log_n(std::to_string(i)+","+std::to_string(j)+","+std::to_string(cell[i][j].l)+","+std::to_string(cell[i][j].r)+","+std::to_string(cell[i][j].u)+","+std::to_string(cell[i][j].d));
        }
    }
}
