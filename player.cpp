#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    
    board = new Board();
	my_side = side;
	
	if (my_side == WHITE)
	{
		opponent_side = BLACK;
	}
	else
	{
		opponent_side = WHITE;
	}
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
     
    //have the opponent's move placed on the board before deciding anything
    board->doMove(opponentsMove, opponent_side);
   
   //generate list of valid moves
    std::vector<Move*> moves;
    
	for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move* move = new Move(i, j);
            if (board->checkMove(move, my_side))
            {
				moves.push_back(move);
			}
				
        }
    }
    
    board->doMove(moves[0], my_side);
    return moves[0];
    
}
