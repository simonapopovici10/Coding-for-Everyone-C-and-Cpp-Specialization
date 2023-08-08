#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>
#include <string>

using namespace std;

// a global random number generator
random_device rd;
mt19937 generator(rd());

// hex cell owner
enum class Owner {NONE, RED, BLUE};

// a row / col data holder - zero based
class RC {
  public:
    int row;
    int col;
    RC() {}
    RC(int row, int col) : row(row), col(col) {}
    bool operator==(const RC& other) const { return row == other.row && col == other.col; }
};

const int MAXN = 25;    // a small concession to efficiency

// model a Hex game board of size n
class HexBoard {
  private:
    int n;
    vector<vector<Owner>> board;
    Owner winner;

  public:
    HexBoard(int n) : n(n), board(n, vector<Owner>(n, Owner::NONE)), winner(Owner::NONE) {}

    int size() const { return n; }

    // return owner of this location
    Owner getNodeOwner(int row, int col) const { return board[row][col]; }

    // set owner of this location
    void setNodeOwner(int row, int col, Owner owner) {
      board[row][col] = owner;
      winner = Owner::NONE;     // winner needs to be recalculated if set
    }

    // find winner - boardIsFullHint is a perfromance hint - must be a winner if full
    Owner getWinner(bool boardIsFullHint = false) {
      bool visited[MAXN][MAXN]; // we'll just do a depth first search
      RC toDo[MAXN * MAXN];     // cells to process
      if (winner == Owner::NONE) {
        for (Owner owner: {Owner::RED, Owner::BLUE}) {
          if (boardIsFullHint && owner == Owner::BLUE)  // here'e the shortcut if hint is given
            return owner;
          for (int row = 0; row < n; ++row)
            for (int col = 0; col < n; ++col)
              visited[row][col] = false;
          int nToDo = 0;
          if (owner == Owner::RED) {                    // start with all of top row that are red
            for (int col = 0; col < n; ++col) {
              if (getNodeOwner(0, col) == owner) {
                toDo[nToDo++] = RC(0, col);
                visited[0][col] = true;
              }
            }
          } else {
            for (int row = 0; row < n; ++row) {         // start with left column that are blue
              if (getNodeOwner(row, 0) == owner) {
                toDo[nToDo++] = RC(row, 0);
                visited[row][0] = true;
              }
            }
          }
          while (nToDo > 0) {
            RC rc = toDo[--nToDo];
            static RC rcNeighbors[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 0}, {1, -1}, {0, -1}};
            for (RC& rcNeighbor: rcNeighbors) {
              RC rcTry(rc.row + rcNeighbor.row, rc.col + rcNeighbor.col);
              if (rcTry.row >= 0 && rcTry.col >= 0 && rcTry.row < n && rcTry.col < n) { // valid cell
                if (board[rcTry.row][rcTry.col] == owner && !visited[rcTry.row][rcTry.col]) {   // unvisited and right color
                  if ((owner == Owner::RED && rcTry.row == n - 1) || (owner == Owner::BLUE && rcTry.col == n - 1))
                    return winner = owner;      // done if we hit last row(red) or col(blue)
                  toDo[nToDo++] = rcTry;
                  visited[rcTry.row][rcTry.col] = true;
                }
              }
            }
          }
        }
      }
      return winner;    // might have already been calculated, could be NONE
    }

    // calculated shortest winning path - getWinner could be used but it is DFS and not always shortest
    vector<RC> getWinnerPath() {