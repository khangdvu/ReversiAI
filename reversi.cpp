#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include "reversi.h"


Reversi::Reversi() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			board[x][y] = 0;
		}
	}
	board[3][3] = 1;
	board[4][3] = 2;
	board[4][4] = 1;
	board[3][4] = 2;
}

Reversi::~Reversi() {
	return;
}

void Reversi::displayBoard() {
	string numberLine = "     0   1   2   3   4   5   6   7  ";
	string rowLine = "   |---|---|---|---|---|---|---|---|";
	string emptySquare = " |  ";
	cout << numberLine << endl << rowLine << endl;
	for (int x = 0; x < 8; x++) {
		cout << " " << x;
		for (int y = 0; y < 8; y++) {
			if (board[x][y] == 0) {
				cout << emptySquare;
			}
			
			else if (board[x][y] == 1) {
				cout << " | " << "X";
			}
			else if (board[x][y] == 2) {
				cout << " | " << "O";
			}
		}
		cout << " |" << endl << rowLine << endl;
	}
	cout << endl;
}

vector<pair<int, int>> Reversi::getAllValidMoves(int player) {
	vector< pair <int, int> > possibleMoves;
	pair<int, int> move;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			move = make_pair(x, y);
			if (isValidMove(player, move)) {
				possibleMoves.push_back(move);
			}
		}
	}
	return possibleMoves;
}


bool Reversi::isValidMove(int player, pair<int, int> move) {
	//The 8 possible direction coordinates: 
	//{up-left, up, up-right,left, right, down-left, down, down-right}
	int xCoordinates[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
	int yCoordinates[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };

	int otherPlayer;
	if (player == 1) {
		otherPlayer = 2;
	}
	else {
		otherPlayer = 1;
	}

	if ((board[move.first][move.second]) != 0) {
		return false;
	}

	int turnedTiles = 0;
	int possible_turnedTiles;

	for (int i = 0; i < 8; i++) {
		int x = move.first + xCoordinates[i];
		int y = move.second + yCoordinates[i];
		possible_turnedTiles = 0;
		while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == otherPlayer) {
			possible_turnedTiles++;
			x += xCoordinates[i];
			y += yCoordinates[i];
		}
		if (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == player) {
			turnedTiles += possible_turnedTiles;
		}
	}
	if (turnedTiles > 0) {
		return true;
	}
	else {
		return false;
	}
}

int Reversi::checkPlayersPoints() {
// return 1 or 2 if player1 or player2 has more tiles
// return 0 if draw
	int player1 = 0;
	int player2 = 0;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (board[x][y] == 1) {
				player1++;
			}
			else if (board[x][y] == 2) {
					player2++;
			}
		}
	}
	if (player1 > player2) {
		return 1;
	}
	else if (player1 < player2) {
		return 2;
	}
	else {
		return 0;
	}
}

bool Reversi::isGameFinish() {
	if (getAllValidMoves(1).size() == 0 && getAllValidMoves(2).size() == 0 ) {
		return true;
	}
	return false;
}

void Reversi::makeMove(int player, pair<int, int> move) {
	if (isValidMove(player, move)) {
		board[move.first][move.second] = player;
	}

	//The 8 possible direction coordinates: 
	//{up-left, up, up-right,left, right, down-left, down, down-right}
	int xCoordinates[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
	int yCoordinates[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };

	int otherPlayer;
	if (player == 1) {
		otherPlayer = 2;
	}
	else {
		otherPlayer = 1;
	}

	int turnedTiles = 0;
	vector< pair<int, int> > possibleTiles;

	for (int i = 0; i < 8; i++) {
		int x = move.first + xCoordinates[i];
		int y = move.second + yCoordinates[i];
		possibleTiles.clear();
		while (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == otherPlayer) {
			possibleTiles.push_back(make_pair(x, y));
			x += xCoordinates[i];
			y += yCoordinates[i];
		}
		if (x >= 0 && x < 8 && y >= 0 && y < 8 && board[x][y] == player) {
			for (auto pair : possibleTiles) {
				board[pair.first][pair.second] = player;
			}
		}
	}
	return;
}

