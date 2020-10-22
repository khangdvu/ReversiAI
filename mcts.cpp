#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <tgmath.h>
#include <chrono>
#include "mcts.h"
#include "reversi.h"

using namespace std;


Node::Node(Node* parent, pair<int, int> move) : parent(parent), move(move) {
	plays = 0;
	wins = 0;
	losses = 0;
	draws = 0;
}

Node::~Node() {
	return;
}

void recursiveDestructor(Node* root) {
	// recursively destroy the tree
	for (auto child : root->children) {
		recursiveDestructor(child);
	}
	delete(root);
}

void randomPlayout(Reversi game, int player, Node* root) {
	//one instance of a playout
	int turn = player;
	Node* currentNode = root;
	while (!game.isGameFinish()) {
		auto allValidMoves = game.getAllValidMoves(turn);
		// Populate currentNode with children from Valid moves if not done yet
		if ((*currentNode).children.size() != allValidMoves.size()) {
			populateChildren(allValidMoves, turn, currentNode);
		}
		// Make a random move
		if (allValidMoves.size() != 0) {
			int random = rand() % allValidMoves.size();
			auto move = allValidMoves[random];
			for (auto child : currentNode->children) {
				if (child->move == move) {
					currentNode = child;
				}
			}
			game.makeMove(turn, move);
			}
		if (turn == 1) {
			turn = 2;
		}
		else if (turn == 2) {
			turn = 1;
		}
	}
	if (game.checkPlayersPoints() == player) {
		updateTree(currentNode, 1);
	}
	else if (game.checkPlayersPoints() != player) {
		updateTree(currentNode, 2);
	}
	else if (game.checkPlayersPoints() == 0) {
		updateTree(currentNode, 0);
	}
}

void ucbPlayout(Reversi game, int player, Node* root, int totalPlays) {
	//one instance of a playout
	int turn = player;
	Node* currentNode = root;
	while (!game.isGameFinish()) {
		auto allValidMoves = game.getAllValidMoves(turn);
		// Populate currentNode with children from Valid moves if not done yet
		if ((*currentNode).children.size() != allValidMoves.size()) {
			populateChildren(allValidMoves, turn, currentNode);
		}
		// 
		if (allValidMoves.size() != 0) {
			pair<int, int> move;
			double bestUcbValue = 0.0;
			Node* chosenChild = NULL;
			for (auto child : currentNode->children) {
				if (child->plays == 0) {
					currentNode = child;
					move = child->move;
					chosenChild = NULL;
					break;
				}
				double childWins = double(child->wins);
				double childPlays = double(child->plays);
				// UCB1 heuristics: maximize 
				// Xj + sqrt(2log(n)/Tj(n))
				//where Xj = win average; n = #plays total across all moves; Tj(n) = #plays of current move
				double childUcbValue = (childWins / childPlays) + sqrt((2 * log(totalPlays))/childPlays);

				if (childUcbValue > bestUcbValue) {
					move = child->move;
					bestUcbValue = childUcbValue;
					chosenChild = child;
				}
			}
			if (chosenChild != NULL) {
				currentNode = chosenChild;
			}

			game.makeMove(turn, move);
		}
		if (turn == 1) {
			turn = 2;
		}
		else if (turn == 2) {
			turn = 1;
		}
	}
	if (game.checkPlayersPoints() == player) {
		updateTree(currentNode, 1);
	}
	else if (game.checkPlayersPoints() != player) {
		updateTree(currentNode, 2);
	}
	else if (game.checkPlayersPoints() == 0) {
		updateTree(currentNode, 0);
	}
}

void populateChildren(vector<pair<int,int>> allValidMoves, int player, Node* parent) {
	for (auto move : allValidMoves) {
		Node* node = new Node(parent, move);
		(*parent).children.push_back(node);
	}
	return;
}

void updateTree(Node* node, int result) {
	// backpropagation phase: update result up the tree
	// Result == 1 : win
	// Result == 2 : loss
	// Result == 0 : draw
	Node* currentNode = node;
	while (currentNode != NULL) {
		currentNode->plays++;
		if (result == 1) {
			currentNode->wins++;
		}
		else if (result == 2) {
			currentNode->losses++;
		}
		else if (result == 0) {
			currentNode->draws++;
		}
		currentNode = currentNode->parent;
	}
}

pair<int, int> pureMCTS(Reversi game, int player, int num_playouts) {
	// returns move that is "best" based on (wins/plays)
	Node* root = new Node(NULL, make_pair(10, 10));
	srand(time(NULL));
	for (int i = 0; i < num_playouts; i++) {
		randomPlayout(game, player, root);
	}

	pair<int, int> bestMove = make_pair(10,10);
	float bestAverage = 0.0;
	float currentAverage;
	for (auto child : root->children) {
		currentAverage = (float(child->wins) / float(child->plays));
		if (currentAverage > bestAverage) {
			bestMove = child->move;
			bestAverage = currentAverage;
		}
	}

	recursiveDestructor(root);
	return bestMove;
}

pair<int, int> ucbMCTS(Reversi game, int player, int num_playouts) {
	// returns move that is "best" based on (wins/plays)
	Node* root = new Node(NULL, make_pair(10, 10));
	for (int i = 0; i < num_playouts; i++) {
		ucbPlayout(game, player, root, i);
	}

	pair<int, int> bestMove = make_pair(10, 10);
	float bestAverage = 0.0;
	float currentAverage;
	for (auto child : root->children) {
		currentAverage = (float(child->wins) / float(child->plays));
		if (currentAverage > bestAverage) {
			bestMove = child->move;
			bestAverage = currentAverage;
		}
	}

	recursiveDestructor(root);
	return bestMove;
}

void analysisMCTS() {
	int pureAI = 1;
	int ucbAI = 2;
	int numPlayouts = 300;
	int pureAIWins = 0;
	int ucbAIWins = 0;

	cout << endl << "Here are the simluated games between pureMCTS AI vs ucb1MCTS AI." << endl
		<< "Each AI will perform 300 playouts per move." << endl
		<< "pureMCTS ai will go first for 20 games, then ucb1MCTS AI will go first the subsequent 20 games." << endl << endl;
	for (int i = 0; i < 20; i++){
		cout << "Simulating Game " << i <<" where pureAI goes first...";
		pureAI = 1;
		ucbAI = 2;
		Reversi game;
		int turn = 1;
		// Game loop
		while (!game.isGameFinish()) {

			//Player's turn
			if (turn == pureAI) {
				pair<int, int> aiMove;
				aiMove = pureMCTS(game, ucbAI, numPlayouts);
				game.makeMove(ucbAI, aiMove);
			}

			//AI's turn
			else if (turn == ucbAI) {
				pair<int, int> aiMove;
				aiMove = ucbMCTS(game, ucbAI, numPlayouts);
				game.makeMove(ucbAI, aiMove);
			}
			
			//switch turn
			if (turn == pureAI) {
				turn = ucbAI;
			}
			else if (turn == ucbAI) {
				turn = pureAI;
			}
		}
		if (game.checkPlayersPoints() == pureAI) {
			pureAIWins++;
			cout << " pureAI Wins." << endl;
		}
		else if (game.checkPlayersPoints() == ucbAI) {
			ucbAIWins++;
			cout << " ucbAI Wins." << endl;
		}
	}
	for (int i = 0; i < 20; i++) {
		cout << "Simulating Game " << i << " where ucbAI goes first...";
		pureAI = 2;
		ucbAI = 1;
		Reversi game;
		int turn = 1;
		// Game loop
		while (!game.isGameFinish()) {

			//Player's turn
			if (turn == pureAI) {
				pair<int, int> aiMove;
				aiMove = pureMCTS(game, ucbAI, numPlayouts);
				game.makeMove(ucbAI, aiMove);
			}

			//AI's turn
			else if (turn == ucbAI) {
				pair<int, int> aiMove;
				aiMove = ucbMCTS(game, ucbAI, numPlayouts);
				game.makeMove(ucbAI, aiMove);
			}

			//switch turn
			if (turn == pureAI) {
				turn = ucbAI;
			}
			else if (turn == ucbAI) {
				turn = pureAI;
			}
		}
		// Check for winner
		if (game.checkPlayersPoints() == pureAI) {
			pureAIWins++;
			cout << " pureAI Wins." << endl;
		}
		else if (game.checkPlayersPoints() == ucbAI) {
			ucbAIWins++;
			cout << " ucbAI Wins." << endl;
		}
	}
	   	  
}