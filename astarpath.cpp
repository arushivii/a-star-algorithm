#include<bits/stdc++.h>
#include<windows.h>
HANDLE color=GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std;
#define ROW 15
#define COL 15
typedef pair<int, int> Pair;
typedef pair <double, pair<int, int> > pPair;
struct cell
{
	int parent_i, parent_j;
	double f, g, h;
};
bool isValid(int row, int col)
{
	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}
bool isUnBlocked(int grid[][COL], int row, int col)
{
	if (grid[row][col] == 1)
		return (true);
	else
		return (false);
}
bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}
double calculateHValue(int row, int col, Pair dest)
{
	return ((double)sqrt ((row-dest.first)*(row-dest.first) + (col-dest.second)*(col-dest.second)));
}
void tracePath(int grid[ROW][COL], cell cellDetails[ROW][COL], Pair dest)
{
    SetConsoleTextAttribute(color, 15);
	printf ("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;
	stack<Pair> Path;
	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col ))
	{
		Path.push (make_pair (row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}
	Path.push (make_pair (row, col));
	int c=0;
	while (!Path.empty())
	{
		pair<int,int> p = Path.top();
		Path.pop();
		if(c==0 || Path.empty())
        {
            cout<<"-> ";
            SetConsoleTextAttribute(color, 9);
            cout<<"("<<p.first<<","<<p.second<<") ";
            SetConsoleTextAttribute(color, 15);
            if(c==0)
            {
                grid[p.first][p.second]=3;
                SetConsoleTextAttribute(color, 10);
            }
            else
                grid[p.first][p.second]=4;
            c=1;
        }
        else
        {
            cout<<"-> ("<<p.first<<","<<p.second<<") ";
            grid[p.first][p.second]=2;
        }
	}
	SetConsoleTextAttribute(color, 12);
	cout<<"\n\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	for(int i=0;i<ROW;i++)
    {
        for(int j=0;j<COL;j++)
        {
            if(grid[i][j]==0)
                cout<<"|O";
            else if(grid[i][j]==2)
            {
                cout<<"|";
                SetConsoleTextAttribute(color, 10);
                cout<<"+";
                SetConsoleTextAttribute(color, 12);
            }
            else if(grid[i][j]==3)
            {
                cout<<"|";
                SetConsoleTextAttribute(color, 9);
                cout<<"S";
                SetConsoleTextAttribute(color, 12);
            }
            else if(grid[i][j]==4)
            {
                cout<<"|";
                SetConsoleTextAttribute(color, 9);
                cout<<"F";
                SetConsoleTextAttribute(color, 12);
            }
            else if(grid[i][j]==1)
                cout<<"| ";
        }
        cout<<"|\n";
    }
    cout<<"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
    SetConsoleTextAttribute(color, 15);
	return;
}
void aStarSearch(int grid[ROW][COL], Pair src, Pair dest)
{
	if (isValid (src.first, src.second) == false)
	{
	    SetConsoleTextAttribute(color, 14);
		printf ("\nSource is invalid\n");
		SetConsoleTextAttribute(color, 15);
		return;
	}
	if (isValid (dest.first, dest.second) == false)
	{
	    SetConsoleTextAttribute(color, 14);
		printf ("\nDestination is invalid\n");
		SetConsoleTextAttribute(color, 15);
		return;
	}
	if (isUnBlocked(grid, src.first, src.second) == false || isUnBlocked(grid, dest.first, dest.second) == false)
	{
	    SetConsoleTextAttribute(color, 14);
		printf ("\nSource or the destination is blocked\n");
		SetConsoleTextAttribute(color, 15);
		return;
	}
	if (isDestination(src.first, src.second, dest) == true)
	{
	    SetConsoleTextAttribute(color, 13);
		printf ("\nWe are already at the destination\n");
		SetConsoleTextAttribute(color, 15);
		return;
	}
	bool closedList[ROW][COL]={false};
	cell cellDetails[ROW][COL];
	int i, j;
	for (i=0; i<ROW; i++)
	{
		for (j=0; j<COL; j++)
		{
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;
	set<pPair> openList;
	openList.insert(make_pair (0.0, make_pair (i, j)));
	bool foundDest = false;
	while (!openList.empty())
	{
		pPair p = *openList.begin();
		openList.erase(openList.begin());
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;
		double gNew, hNew, fNew;
		//----------- 1st Successor (North) ------------
		if (isValid(i-1, j) == true)
		{
			if (isDestination(i-1, j, dest) == true)
			{
				cellDetails[i-1][j].parent_i = i;
				cellDetails[i-1][j].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf ("\n\nThe destination cell is found\n");
				tracePath (grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i-1][j] == false && isUnBlocked(grid, i-1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue (i-1, j, dest);
				fNew = gNew + hNew;
				if (cellDetails[i-1][j].f == FLT_MAX || cellDetails[i-1][j].f > fNew)
				{
					openList.insert( make_pair(fNew, make_pair(i-1, j)));
					cellDetails[i-1][j].f = fNew;
					cellDetails[i-1][j].g = gNew;
					cellDetails[i-1][j].h = hNew;
					cellDetails[i-1][j].parent_i = i;
					cellDetails[i-1][j].parent_j = j;
				}
			}
		}
		//----------- 2nd Successor (South) ------------
		if (isValid(i+1, j) == true)
		{
			if (isDestination(i+1, j, dest) == true)
			{
				cellDetails[i+1][j].parent_i = i;
				cellDetails[i+1][j].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf("\n\nThe destination cell is found\n");
				tracePath(grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i+1][j] == false && isUnBlocked(grid, i+1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i+1, j, dest);
				fNew = gNew + hNew;
				if (cellDetails[i+1][j].f == FLT_MAX || cellDetails[i+1][j].f > fNew)
				{
					openList.insert( make_pair (fNew, make_pair (i+1, j)));
					cellDetails[i+1][j].f = fNew;
					cellDetails[i+1][j].g = gNew;
					cellDetails[i+1][j].h = hNew;
					cellDetails[i+1][j].parent_i = i;
					cellDetails[i+1][j].parent_j = j;
				}
			}
		}
		//----------- 3rd Successor (East) ------------
		if (isValid (i, j+1) == true)
		{
			if (isDestination(i, j+1, dest) == true)
			{
				cellDetails[i][j+1].parent_i = i;
				cellDetails[i][j+1].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf("\n\nThe destination cell is found\n");
				tracePath(grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i][j+1] == false && isUnBlocked (grid, i, j+1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue (i, j+1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i][j+1].f == FLT_MAX || cellDetails[i][j+1].f > fNew)
				{
					openList.insert( make_pair(fNew, make_pair (i, j+1)));
					cellDetails[i][j+1].f = fNew;
					cellDetails[i][j+1].g = gNew;
					cellDetails[i][j+1].h = hNew;
					cellDetails[i][j+1].parent_i = i;
					cellDetails[i][j+1].parent_j = j;
				}
			}
		}
		//----------- 4th Successor (West) ------------
		if (isValid(i, j-1) == true)
		{
			if (isDestination(i, j-1, dest) == true)
			{
				cellDetails[i][j-1].parent_i = i;
				cellDetails[i][j-1].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf("\n\nThe destination cell is found\n");
				tracePath(grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i][j-1] == false && isUnBlocked(grid, i, j-1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j-1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i][j-1].f == FLT_MAX || cellDetails[i][j-1].f > fNew)
				{
					openList.insert( make_pair (fNew, make_pair (i, j-1)));
					cellDetails[i][j-1].f = fNew;
					cellDetails[i][j-1].g = gNew;
					cellDetails[i][j-1].h = hNew;
					cellDetails[i][j-1].parent_i = i;
					cellDetails[i][j-1].parent_j = j;
				}
			}
		}
		//----------- 5th Successor (North-East) ------------
        if (isValid(i-1, j+1) == true)
		{
			if (isDestination(i-1, j+1, dest) == true)
			{
				cellDetails[i-1][j+1].parent_i = i;
				cellDetails[i-1][j+1].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf ("\n\nThe destination cell is found\n");
				tracePath (grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i-1][j+1] == false && isUnBlocked(grid, i-1, j+1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i-1, j+1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i-1][j+1].f == FLT_MAX || cellDetails[i-1][j+1].f > fNew)
				{
					openList.insert( make_pair (fNew, make_pair(i-1, j+1)));
					cellDetails[i-1][j+1].f = fNew;
					cellDetails[i-1][j+1].g = gNew;
					cellDetails[i-1][j+1].h = hNew;
					cellDetails[i-1][j+1].parent_i = i;
					cellDetails[i-1][j+1].parent_j = j;
				}
			}
		}
		//----------- 6th Successor (North-West) ------------
		if (isValid (i-1, j-1) == true)
		{
			if (isDestination (i-1, j-1, dest) == true)
			{
				cellDetails[i-1][j-1].parent_i = i;
				cellDetails[i-1][j-1].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf ("\n\nThe destination cell is found\n");
				tracePath (grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i-1][j-1] == false && isUnBlocked(grid, i-1, j-1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i-1, j-1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i-1][j-1].f == FLT_MAX || cellDetails[i-1][j-1].f > fNew)
				{
					openList.insert( make_pair (fNew, make_pair (i-1, j-1)));
					cellDetails[i-1][j-1].f = fNew;
					cellDetails[i-1][j-1].g = gNew;
					cellDetails[i-1][j-1].h = hNew;
					cellDetails[i-1][j-1].parent_i = i;
					cellDetails[i-1][j-1].parent_j = j;
				}
			}
		}
		//----------- 7th Successor (South-East) ------------
		if (isValid(i+1, j+1) == true)
		{
			if (isDestination(i+1, j+1, dest) == true)
			{
				cellDetails[i+1][j+1].parent_i = i;
				cellDetails[i+1][j+1].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf ("\n\nThe destination cell is found\n");
				tracePath (grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i+1][j+1] == false && isUnBlocked(grid, i+1, j+1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i+1, j+1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i+1][j+1].f == FLT_MAX || cellDetails[i+1][j+1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair (i+1, j+1)));
					cellDetails[i+1][j+1].f = fNew;
					cellDetails[i+1][j+1].g = gNew;
					cellDetails[i+1][j+1].h = hNew;
					cellDetails[i+1][j+1].parent_i = i;
					cellDetails[i+1][j+1].parent_j = j;
				}
			}
		}
		//----------- 8th Successor (South-West) ------------
		if (isValid (i+1, j-1) == true)
		{
			if (isDestination(i+1, j-1, dest) == true)
			{
				cellDetails[i+1][j-1].parent_i = i;
				cellDetails[i+1][j-1].parent_j = j;
				SetConsoleTextAttribute(color, 13);
				printf("\n\nThe destination cell is found\n");
				tracePath(grid, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i+1][j-1] == false && isUnBlocked(grid, i+1, j-1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i+1, j-1, dest);
				fNew = gNew + hNew;
				if (cellDetails[i+1][j-1].f == FLT_MAX || cellDetails[i+1][j-1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i+1, j-1)));
					cellDetails[i+1][j-1].f = fNew;
					cellDetails[i+1][j-1].g = gNew;
					cellDetails[i+1][j-1].h = hNew;
					cellDetails[i+1][j-1].parent_i = i;
					cellDetails[i+1][j-1].parent_j = j;
				}
			}
		}
	}
	if (foundDest == false)
    {
        SetConsoleTextAttribute(color, 14);
		printf("\nFailed to find the Destination Cell\n");
		SetConsoleTextAttribute(color, 15);
    }
	return;
}
int main()
{
	int grid1[ROW][COL] =
	{
		{0,1,1,1,1,1,1,0,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,0,0,0,0,1,0,1,0,0,0,0,1,1},
		{1,1,0,1,1,1,1,0,1,1,1,1,0,1,1},
		{1,1,0,1,1,1,1,0,1,1,1,1,0,1,1},
		{1,1,0,1,1,1,1,0,1,1,1,1,0,1,1},
		{1,1,1,1,1,1,1,0,1,1,1,1,1,1,1},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{1,1,1,1,1,1,1,0,1,1,1,1,1,1,1},
		{1,1,0,1,1,1,1,0,1,1,1,1,0,1,1},
		{1,1,0,1,1,1,1,0,1,1,1,1,0,1,1},
		{1,1,0,1,1,1,1,0,1,1,1,1,0,1,1},
		{1,1,0,0,0,0,1,0,1,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,1,1,1,1,1,1,0,1,1,1,1,1,1,0}
	};
    int grid2[ROW][COL] =
	{
		{0,1,1,1,1,1,1,0,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,0,1,1,0,0,0,0,1,1,0,0,1,1},
		{1,1,0,0,1,1,1,1,1,1,0,0,1,1,1},
		{1,1,1,0,0,1,1,1,1,0,0,1,1,1,1},
		{1,1,1,1,0,0,1,1,0,0,1,1,0,1,1},
		{1,1,0,1,1,0,0,1,0,1,1,1,0,1,1},
		{0,1,0,1,1,1,1,0,1,1,1,1,0,1,0},
		{1,1,0,1,1,1,0,1,0,0,1,1,0,1,1},
		{1,1,0,1,1,0,0,1,1,0,0,1,1,1,1},
		{1,1,1,1,0,0,1,1,1,1,0,0,1,1,1},
		{1,1,1,0,0,1,1,1,1,1,1,0,0,1,1},
		{1,1,0,0,1,1,0,0,0,0,1,1,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,1,1,1,1,1,1,0,1,1,1,1,1,1,0}
	};
	int grid3[ROW][COL] =
	{
		{0,0,1,1,1,1,1,0,1,1,1,1,1,1,0},
		{1,0,1,1,1,1,1,0,1,1,1,1,0,0,0},
		{1,0,0,1,1,0,1,0,1,0,1,1,0,1,1},
		{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1},
		{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1},
		{1,1,0,0,1,1,0,0,0,1,1,0,0,1,1},
		{1,1,1,1,1,0,1,1,1,0,1,1,1,1,1},
		{0,0,0,1,1,0,1,0,1,0,1,1,0,0,0},
		{1,1,1,1,1,0,1,1,1,0,1,1,1,1,1},
		{1,1,0,0,1,1,0,0,0,1,1,0,0,1,1},
		{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1},
		{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1},
		{1,1,0,1,1,0,1,0,1,0,1,1,0,0,1},
		{0,0,0,1,1,1,1,0,1,1,1,1,1,0,1},
		{0,1,1,1,1,1,1,0,1,1,1,1,1,0,0}
	};
	int i,j,ch;
	SetConsoleTextAttribute(color, 15);
	cout<<"Enter grid map of your choice 1,2 or 3:\n\n";
	cout<<"1.\t\t\t\t\t2.\t\t\t\t\t3.\n";
	SetConsoleTextAttribute(color, 12);
	cout<<"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\t\t+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\t\t+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
	for(i=0;i<ROW;i++)
    {
        for(j=0;j<COL;j++)
        {
            if(grid1[i][j]==0)
                cout<<"|O";
            else
                cout<<"| ";
        }
        cout<<"|\t\t";
        for(j=0;j<COL;j++)
        {
            if(grid2[i][j]==0)
                cout<<"|O";
            else
                cout<<"| ";
        }
        cout<<"|\t\t";
        for(j=0;j<COL;j++)
        {
            if(grid3[i][j]==0)
                cout<<"|O";
            else
                cout<<"| ";
        }
        cout<<"|"<<endl;
    }
    cout<<"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\t\t+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\t\t+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n";
    SetConsoleTextAttribute(color, 11);
	cin>>ch;
	SetConsoleTextAttribute(color, 15);
	int x1,y1,x2,y2;
	cout<<"\nEnter STARTING x and y co-ordinates from 0 to 14\n";
	SetConsoleTextAttribute(color, 11);
	cin>>x1>>y1;
	SetConsoleTextAttribute(color, 15);
	cout<<"\nEnter FINISHING x and y co-ordinates from 0 to 14\n";
	SetConsoleTextAttribute(color, 11);
	cin>>x2>>y2;
	SetConsoleTextAttribute(color, 15);
	Pair src = make_pair(x1, y1);
	Pair dest = make_pair(x2, y2);
    if(ch==1)
        aStarSearch(grid1, src, dest);
    else if(ch==2)
        aStarSearch(grid2, src, dest);
    else if(ch==3)
        aStarSearch(grid3, src, dest);
	return(0);
}
