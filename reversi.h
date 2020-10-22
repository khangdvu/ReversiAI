#pragma once

using namespace std;

class Reversi {
	int board[8][8];
public:
	Reversi();
	~Reversi();

	//display board in user-friendly interface
	void displayBoard();
	
	//get a vector list of pairs containing the x- and y-coordinates of all possible moves
	vector<pair<int, int>> getAllValidMoves(int player);

	//check if a move is valid for player
	bool isValidMove(int player, pair<int, int> move);

	// return 1 or 2 if player1 or player2 has more tiles or 0 if draw
	int checkPlayersPoints(); 

	//update board by flipping over tiles from a move
	void makeMove(int player, pair<int, int> move);

	//check if game is finished
	bool isGameFinish();
};