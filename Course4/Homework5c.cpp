/*
Coursera "C++ for C, Part B" Assignment #5 (second assignment in part B)
  by Chris Teslak, November 10, 2022

Instructions:

Draw a HEX board and determine a legal position and a winning position.
1.	Be able to draw the board using ASCII symbols and a given size, such as 7 by 7 or 11 by 11.
2.	Input a move and determine if a move is legal.
		The blue player must make a connected set of blue hexagons from east to west.
		The red player must do the same from north to south.
		At each turn a player chooses an unoccupied hexagon and gives it their color.
3.	Determine who won.
Implement Monte Carlo Hex move evaluation
4.  Objective: Generate an expert Hex move on an 11x 11 board. Be able to play against this program.
5.  Player should be able to interact with the program and choose its “color”
6.  Each legal move will be evaluated using  ~1000 or more trials. 
7.  The ratio: wins/trials are the AI’s metric for picking which next move to make.
8.  Efficiently determine within no more than 2 minutes what the computer's move is.

Your program should use a graph representation and treat the game as a path finding problem. Each internal node (hexagon) has six neighbors –
  so each would have 6 edges. The corners and the edges are special. A corner has either two or three neighbors and a non-corner edge has 4 neighbors.
The program should have a convenient interface for entering a move, displaying the board, and then making its own move.
The program should determine when the game is over and announce the winner.

*/

// Need time to seed the random number generator
#include <chrono>
//#include <sys/time.h> //gettimeofday()      ( NOTE: Use for METHOD 1 for calculating the value for srand(). Won't work in Visual Studio 2017. )

#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <queue>
#include <random>     // for std::default_random_engine()


using namespace std;
const bool VERBOSE = false;
const int INF = 99999999;

//========================================= GRAPH ======================================================================
/*
A potential partial interface definition for a Graph could be:
	Class Graph:
	V (G): returns the number of vertices in the graph
	E (G): returns the number of edges in the graph
	adjacent (G, x, y): tests whether there is an edge from node x to node y.
	neighbors (G, x): lists all nodes y such that there is an edge from x to y.
	add (G, x, y): adds to G the edge from x to y, if it is not there.
	delete (G, x, y): removes the edge from x to y, if it is there.
	get_node_value (G, x): returns the value associated with the node x.
	set_node_value( G, x, a): sets the value associated with the node x to a.
	get_edge_value( G, x, y): returns the value associated to the edge (x,y).
	set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v.
*/
// Reference material:
//  Multidimensional variable size array in C++
//  https://stackoverflow.com/questions/1946830/multidimensional-variable-size-array-in-c

/*
template <int>
iter_pair<int> make_range(std::pair<int, int> p) {
	return iter_pair<int>(p);
}
*/

class graph {
public:
	graph() {}                        // default constructor
	graph(size_t sz = 1);             // init constructor
	graph(graph& g);                  // copy constructor
	graph(size_t pmsz, int** pm);     // transfer constructor
	graph(string inpFile);            // file input constructor
	~graph();

	void printHex();
	void print();
	size_t gsize();  // returns the size of the graph
	int  gelem(int i, int j);// the element at the specified row and column
	void selem(int elementVal, int i, int j);// set the element at the specified row and column
	vector<pair<int, int>> possMoves(void);// return list of all empty cells
	bool is_winner(int row, int col);
	void checkBorders(int row, int col, vector<bool> &flags, char player);
	bool ValidMove(int row, int col);
	bool inBounds(int row, int col);
	void randFill();

private:
	void pad(int i);
	int** matrix;          // each element is a pointer to an array.
	size_t msize;
};


void graph::pad(int i) {
	for (int j = 0; j < i; ++j) cout << " ";
}


// Reference:
//  https://stackoverflow.com/questions/12885356/random-numbers-with-different-probabilities
//  https://www.cplusplus.com/reference/cstdlib/rand/
//  https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/#use-the-time-function-to-get-time-in-milliseconds-in-c%2b%2b

graph::graph(size_t sz) {       //init constructor

	msize = sz;
	matrix = new int*[sz]; // each element is a pointer to an array.

	for (size_t i = 0; i < sz; ++i)
		matrix[i] = new int[sz]; // build rows

	for (size_t i = 0; i < sz; ++i)
		for (size_t j = 0; j < sz; ++j)
			matrix[i][j] = 0;

}

graph::graph(graph &m)                            // copy constructor
{
	msize = m.msize;

	matrix = new int*[msize]; // each element is a pointer to an array.

	for (size_t i = 0; i < msize; ++i)
		matrix[i] = new int[msize]; // build rows

	for (size_t i = 0; i < msize; ++i)
		for (size_t j = 0; j < msize; ++j)
			matrix[i][j] = m.matrix[i][j];
}


graph::graph(size_t pmsz, int** pm)                        // transfer constructor
{
	msize = pmsz;
	matrix = new int*[msize]; // each element is a pointer to an array.
	for (size_t i = 0; i < msize; ++i) matrix[i] = new int[msize]; // build rows

	for (size_t i = 0; i < msize; ++i)
	{
		for (size_t j = 0; j < msize; ++j)
		{
			matrix[i][j] = pm[j][i];
		}
	}
}

graph::graph(const string inpFile) {               // constructor that can read in a graph from a file
//The file format will be an initial integer that is the node size of the graph and the further values will be integer triples: (i, j, cost).
	cout << "Opening:  <<<" << inpFile.c_str() << ">>>" << endl;
	ifstream data_file(inpFile);
	if (!data_file) {
		cout << "File NOT successfully opened." << endl;
		msize = 1;
		matrix = new int*[msize]; // each element is a pointer to an array.
	}
	else {
		cout << "File successfully opened." << endl;

		istream_iterator<int> start(data_file), finish, it;
		vector<int> data(start, finish);   // All the integers in the file are in this vector
		cout << "number of items in data: " << data.size() << endl;

		msize = data[0];

		matrix = new int*[msize]; // each element is a pointer to an array.
		for (size_t i = 0; i < msize; ++i) matrix[i] = new int[msize]; // create rows

		// populate the entire matrix with zeroes
		for (size_t i = 0; i < msize; ++i)
			for (size_t j = 0; j < msize; ++j)
				matrix[i][j] = 0;

		// grab three numbers at a time starting with element 1, i,j,cost
		// populate appropriate elements of matrix:  matrix[i][j]=cost
		for (size_t x = 1; x + 2 < data.size(); x = x + 3)
			matrix[data[x]][data[x + 1]] = data[x + 2];
	}
}

graph::~graph()
{
	//cout << "Graph Destructor, size = "<< msize << "." << endl;
	for (size_t i = 0; i < msize; ++i)
		delete matrix[i];

	delete matrix;
}

void graph::printHex() {
	cout << "Hex Game Board...................................." << endl;
	char symbol[3] = { '.', 'X', 'O' };
	int shift = 0;

	for (size_t i = 0; i < msize; ++i) {
		pad(shift++);
		for (size_t j = 0; j < msize; ++j) {
			cout << symbol[matrix[i][j]];
			if (j < msize - 1) cout << " - ";
		}
		cout << endl;
		if (i < msize - 1) {
			pad(shift++);
			for (size_t j = 0; j < msize; ++j) {
				cout << "\\";
				if (j < msize - 1) cout << " / ";
			}
			cout << endl;
		}
	}
	cout << endl;
}


void graph::print() {
	cout << "Graph:...................................." << endl;
	for (size_t i = 0; i < msize; ++i) {
		for (size_t j = 0; j < msize; ++j) {
			if (matrix[i][j] < 10)
				cout << "   " << matrix[i][j];
			else
				cout << "  " << matrix[i][j];
		}
		cout << endl;
	}
}


size_t graph::gsize() {
	return(msize);
}

int graph::gelem(int i, int j) {
	return(matrix[i][j]);
}

void graph::selem(int elementVal, int i, int j) {  // expect i,j = 0 .. msize-1
	matrix[i][j] = elementVal;
	//cout << "Setting matrix["<<i-1<<"]["<<j-1<<"] = "<< elementVal <<endl;
}


vector<pair<int, int>> graph::possMoves(void) {
	// retrun a list of all cells that are empty
	cout << "Now looking for all possible moves the computer can make." << endl;
	vector<pair<int, int>> validMoves;
	for (size_t i = 0; i < msize; ++i) {
		for (size_t j = 0; j < msize; ++j) {
			if (matrix[i][j] == 0) {
				validMoves.push_back(make_pair(i, j));  // Adds a row,col pair to the vector
			}
		}
	}
	return validMoves;
}


void graph::checkBorders(int row, int col, vector<bool> &flags, char player)
{
	if (player == 1) //Blue
	{
		if (col == 0)			flags[0] = true;
		if (col == msize - 1)	flags[1] = true;
	}
	else if (player == 2) //Red
	{
		if (row == 0)			flags[0] = true;
		if (row == msize - 1)	flags[1] = true;
	}
}

bool graph::ValidMove(int row, int col)
{
	return row >= 0 && row < msize && col >= 0 && col < msize && matrix[row][col] == 0;
}

bool graph::inBounds(int row, int col)
{
	return row >= 0 && row < msize && col >= 0 && col < msize;
}

bool graph::is_winner(int row, int col)
{
	const int surround[6][2] =
	{
		{-1, 0}, {-1, 1}, // top left, top right
		{0, -1}, {0, 1},  // left, right
		{1, -1}, {1, 0}   // buttom left, buttom right
	};

	char player = matrix[row][col];
	vector<bool> flags(2, false);

	vector<vector<bool> > visited(msize, vector<bool>(msize));
	queue<pair<int, int> > traces;
	traces.push(make_pair(row, col));
	visited[row][col] = true;

	while (!traces.empty())
	{
		auto top = traces.front();
		checkBorders(top.first, top.second, flags, player);
		traces.pop();

		for (int n = 0; n < 6; n++)
		{
			int curRow = top.first + surround[n][0];
			int curCol = top.second + surround[n][1];
			if (inBounds(curRow, curCol) && matrix[curRow][curCol] == player
				&& visited[curRow][curCol] == false)
			{
				visited[curRow][curCol] = true;
				traces.push(make_pair(curRow, curCol));
			}
		}
	}

	return flags[0] && flags[1];
}

void graph::randFill() {
	//long long seedVal_ll = chrono::high_resolution_clock::now().time_since_epoch().count(); // nanoseconds since your computer was booted.
	//long seedVal = (seedVal_ll / 1000L) % (long)2000000000; // to make it fit into a long that is converted to unsigned int
	//srand(seedVal);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);
	// Select player 
	for (int row = 0; row < msize; ++row)
		for (int col = 0; col < msize; ++col)
			matrix[row][col] = (rand() % 2) + 1;   // psuedo-random number in the range 1-2
}



//========================================================================================================================

bool doMoveHuman(graph &h, int playerNum) {
	bool isLegalmove = false;
	int cm_index;
	int row, col;  // 0..msize-1
	string player[2] = { "Blue","Red" };

	while (!isLegalmove) {
		cout << player[playerNum - 1].c_str() << " player, select a row: "; cin >> row;
		cout << player[playerNum - 1].c_str() << " player, select a col: "; cin >> col;

		// Determine if move is legal
		if (h.ValidMove(row, col)) {
			isLegalmove = true;
			cout << "\nPlayer " << playerNum << " moves to (" << row << ", " << col << ")." << endl;
			h.selem(playerNum, row, col);
		}
		else {
			cout << "Not a legal move. Try again.\n\n";
		}
	}
	cout << "Checking to see if (" << row << "," << col << ")is a winner...\n";
	return h.is_winner(row, col);
}


bool doMoveComputer(graph &h, int playerNum) {
	const bool verbose = false;
	vector<pair<int, int>> pm;
	vector<float> winProb;
	int cm_index;
	int row, col;  // 0..msize-1
	auto timeAtStartMove = std::chrono::high_resolution_clock::now();  // capture current time so we can measure how fast the algorithm is

	pm = h.possMoves();   // Get a list of all possible valid moves the computer can make (all empty cells)
	cout << "The size of the vector of all possible moves is " << pm.size() << ".\n";
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);
	int rmin = 0;
	int rmax = pm.size() - 1;

	// if it's the first move, just pick anywhere to start. Randomly select among all the available moves
	if (pm.size() == h.gsize()*h.gsize()) {
		int index = rand() % (rmax - rmin + 1) + rmin;   // psuedo-random number in the range rmin - rmax
		row = pm[index].first;
		col = pm[index].second;
	}
	else {
		for (int i = 0; i < pm.size(); ++i) {   // Got through all the available moves and calculate the likely outcome
			// Make a copy of the current game board
			class graph r(h);
			// add the proposed move
			r.selem(playerNum, pm[i].first, pm[i].second);
			// in a loop of 1000 iterations, randomly fill all remaining cells and determine the number of red wins and blue wins
			int blueCount = 0;
			int redCount = 0;
			auto timeAtStart = std::chrono::high_resolution_clock::now();  // capture current time so we can measure how fast the algorithm is
			for (int mc = 0; mc < 1000; ++mc) {
				// fills remaining cells with random players
				for (int index = rmin; index <= rmax; index++) {
					if (index != i)	r.selem(rand() % 2 + 1, pm[index].first, pm[index].second); // player number is random 1 or 2
				}
				// determine who wins
				// Assume that if blue isn't winner, then red is winner
				bool blueWins = false;
				for (int row = 0; row < r.gsize(); ++row) {
					if (r.gelem(row, 0) == 1) {  // if next top row cell is player 1
						blueWins = r.is_winner(row, 0);     // check if player 1 is winner
					}
					if (blueWins) break;
				}
				if (blueWins) blueCount++; else redCount++;
			}
			auto elapsed = std::chrono::high_resolution_clock::now() - timeAtStart;
			// after the loop, calculate the likelihood of win or loss = playerNum wins / 1000 and store in winProb (winProb.push_back(1);)
			float wp;
			if (playerNum == 1) wp = (float)blueCount / (blueCount + redCount); else wp = (float)redCount / (blueCount + redCount);
			winProb.push_back(wp);
			if (verbose) cout << "blueCount = " << blueCount << "\tredCount = " << redCount << "\twinProb = " << wp << "\t";
			long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
			if (verbose) cout << "elapsed time in microseconds = " << microseconds << endl; else cout << ". ";
		}
		// after all pm.size() iterations are done, determine the highest winProb and use as index into pm to get the coordinates of the best move.
		if (verbose) cout << "winProb has " << winProb.size() << " elements." << endl; else cout << endl;
		int bestMove = 1;
		float runningMax = 0;
		for (int index = 0; index < winProb.size(); ++index) {
			if (verbose) cout << winProb[index] << endl;
			if (winProb[index] > runningMax) {
				runningMax = winProb[index];
				bestMove = index;
			}
		}
		cout << "The best move is winProb[" << bestMove << "] = " << runningMax << endl;
		row = pm[bestMove].first;
		col = pm[bestMove].second;
	}
	std::cout << "\nPlayer " << playerNum << " moves to (" << row << ", " << col << ")." << endl;
	h.selem(playerNum, row, col);

	cout << "Checking to see if (" << row << "," << col << ") is a winner...\n";
	auto elapsed = std::chrono::high_resolution_clock::now() - timeAtStartMove;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "It took " << microseconds / 1000000 << " seconds for the comuter to make its move." << endl;
	return h.is_winner(row, col);
}



int main() {

	const int INF = 99999999;
	cout << "<<<<<<<<<<<<  The Game of Hex  >>>>>>>>>>>>" << endl;

	int  boardSize = 5;
	int  numPlayers = INF;
	int  player1Color = INF;
	int  row, col;   // 0 .. msize-1
	bool winPlayer1 = false;
	bool winPlayer2 = false;
	bool skipFirstPlr = true;

	cout << "Enter size of board: ";    cin >> boardSize;

	while (numPlayers < 0 || numPlayers>2) {
		cout << endl << "Enter number of players (1 or 2): "; cin >> numPlayers;
	}
	if (numPlayers) {
		while (player1Color < 0 || player1Color>2) {
			cout << endl << "Enter player 1 color: (1= Blue / East-West or 2= Red / North-South): "; cin >> player1Color;
		}
		cout << "Size of board: " << boardSize << " X " << boardSize << "." << endl;
		cout << "Number of players: " << numPlayers << "." << endl;
		cout << "Player 1 color is: "; if (player1Color == 1) cout << "Blue / East-West\n"; else cout << "Red / North-South\n";

		// Player 1 is always a human player.
		// Player 2 is human if numPlayers==2 and computer if numPlayers==2.
		// Blue player always makes the first move.

		class graph h(boardSize);              // Make a graph for the game board
		int playerNum;

		while ((!winPlayer1) && (!winPlayer2)) {

			// Blue player is up
			h.printHex();   // Print the playing field
			playerNum = 1;  // Blue player
			if ((numPlayers == 1) && (player1Color == 2)) {
				cout << "Blue player, computer making a move." << endl;
				winPlayer1 = doMoveComputer(h, playerNum);
			}
			else {
				cout << "Blue player, please make a move." << endl;
				winPlayer1 = doMoveHuman(h, playerNum);
			}
			if (winPlayer1)
				cout << " GAME OVER.  BLUE player has won!" << endl;
			else {
				h.printHex();   // Print the playing field
				playerNum = 2;  // Red player
				if ((numPlayers == 1) && (player1Color == 1)) {
					cout << "Red player, computer making a move." << endl;
					winPlayer2 = doMoveComputer(h, playerNum);
				}
				else {
					cout << "Red player, please make a move." << endl;
					winPlayer2 = doMoveHuman(h, playerNum);
				}
				if (winPlayer2)
					cout << " GAME OVER.  RED player has won!" << endl;
			}
		} // while
		h.printHex();   // Print the playing field
	}
	cout << endl << "Thank you for playing Hex." << endl;
}


