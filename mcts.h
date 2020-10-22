#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include "reversi.h"

using namespace std;

class Node {
public:
	pair<int, int> move;
	int plays;
	int wins;
	int losses;
	int draws;
	vector<Node*> children;
	Node* parent;
public:
	Node(Node* parent, pair<int, int> move);
	~Node();


};

//delete tree recursively
void recursiveDestructor(Node* root);

//simulate one playout using random moves
void randomPlayout(Reversi game, int player, Node* root);

//simulate one playout using UCB1 heuristics
void ucbPlayout(Reversi game, int player, Node* root, int totalPlays);

//populate a level of tree with appropriate nodes based on available moves
void populateChildren(vector<pair<int,int>> allValidMoves, int player, Node* parent);

//back propagates the three with result of a playout
void updateTree(Node* node, int result); 

//return best possible move after num_playouts playouts using random moves
pair<int, int> pureMCTS(Reversi game, int player, int num_playouts);

//return best possible move after num_playouts playouts using UCB1 heuristics
pair<int, int> ucbMCTS(Reversi game, int player, int num_playouts);

//Simulate games between the pureMCTS agent and ucbMCTS agent
void analysisMCTS();