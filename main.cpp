#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <tgmath.h>
#include <string>
#include <chrono>
#include "mcts.h"
#include "reversi.h"

using namespace std;

int main() {
	cout << ("Let's play some Reversi with a MCTS AI!!") << endl << endl;
	
	// Game setup
	Reversi game;
	game.displayBoard();
	int turn = 1;
	string playerInput;
	string MCTS;
	int ai = 2;
	int numPlayouts;
	int player = 1;
	string playerSymbol = "";

	cout << "Type 1 or 2 for Player 1 or 2 to play!"<< endl << "Or type 'analysis' to see analysis of the 2 MCTS AIs: ";
	cin >> playerInput;
	while (!(playerInput[0] - '0' == 1 || playerInput[0] - '0' == 2 || playerInput.compare("analysis") == 0)) {
		cout << "Invalid input. Please try again: ";
		cin >> playerInput;
	}
	// Input checking: player selection
	if (playerInput[0] - '0' == 1) {
		player = 1;
		playerSymbol = "X";
		ai = 2;
	}
	else if (playerInput[0] - '0' == 2) {
		player = 2;
		playerSymbol = "O";
		ai = 1;
	}
	else if (playerInput.compare("analysis") == 0) {
		analysisMCTS();
	}

	cout << "This program uses an AI that employs Monte Carlo Tree Search (MCTS)." << endl
		<< "Please choose the version of MCTS (pure/ucb1): ";
	cin >> MCTS;

	// Input checking: MCTS selection
	while (!(MCTS.compare("pure") == 0 || MCTS.compare("ucb1") == 0)) {
		cout << "Invalid input. Please choose the version of MCTS (pure/ucb1): ";
		cin >> MCTS;
	}
	if (MCTS.compare("pure") == 0) {
		cout << "Pure MCTS uses totally random playouts to find the best move." << endl << endl;
	}
	if (MCTS.compare("ucb1") == 0) {
		cout << "UCB1 MCTS uses non-random playouts based on the UCB1 heuristics to find the best move." << endl << endl;
	}


	cout << "Select the number of playouts the AI will perform at each turn (recommended 500): ";
	cin >> numPlayouts;

	// Input checking: numPlayouts
	while (!cin.good())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
		cout << "Invalid input. Select the number of playouts the AI will perform at each turn (recommended 500): ";
		cin >> numPlayouts;
		cout << endl;
	}

	game.displayBoard();
	// Game loop
	while (!game.isGameFinish()) {

		//Player's turn
		if (turn == player) {
			if (game.getAllValidMoves(player).size() == 0) {
				cout << "You have no valid moves. You pass the turn." << endl << endl;
			}
			else {
				cout << "Your turn. You are " << playerSymbol << ". Make a move (xy): ";
				cin >> playerInput;
				pair<int, int> playerMove = make_pair(playerInput[1] - '0', playerInput[0] - '0');
				while (!game.isValidMove(player, playerMove)) {
					cout << "Invalid move! Make a move (xy): ";
					cin >> playerInput;
					playerMove = make_pair(playerInput[1] - '0', playerInput[0] - '0');
				}
				game.makeMove(player, playerMove);
				game.displayBoard();
			}
		}

		//AI's turn
		else if (turn == ai) {
			if (game.getAllValidMoves(ai).size() == 0) {
				cout << "AI has no valid moves. AI passes the turn." << endl << endl;
			}
			else {
				auto startTime = chrono::high_resolution_clock::now();
				pair<int, int> aiMove;
				if (MCTS.compare("pure") == 0) {
					aiMove = pureMCTS(game, ai, numPlayouts);
				}
				if (MCTS.compare("ucb1") == 0) {
					aiMove = ucbMCTS(game, ai, numPlayouts);
				}
				auto endTime = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
				cout << "AI's turn. Ai choses move: " << aiMove.second << aiMove.first
					<< ". Time taken by AI to do " << numPlayouts << " playouts: "
					<< duration << " milliseconds." << endl;
				game.makeMove(ai, aiMove);
				game.displayBoard();
			}
		}


		//switch turn
		if (turn == player){
			turn = ai;
		}
		else if (turn == ai) {
			turn = player;
		}
	}
	if (game.checkPlayersPoints() == player) {
		cout << "YOU WIN! GREAT JOB!!" << endl << "Press Enter to exit";
		cin;
	}
	else if (game.checkPlayersPoints() == ai) {
		cout << "YOU LOSE!! BETTER LUCK NEXT TIME..." << endl << "Press Enter to exit";
		cin >> playerInput;
	}

	return 0;
}