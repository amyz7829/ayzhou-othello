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
   
    Move* move = NULL;
    std::cerr<<board->countWhite() + board->countBlack()<<std::endl;
    //early-mid game, use the heuristic. Late game begin to use the minmax tree
    if(board->countWhite() + board->countBlack() < 55)
    {
		move = board->return_best_move_heuristic(my_side);
	}
	else
	{
		move = board->return_minmax_move_better(my_side, 7);
		//move = board->return_alphabeta_move(board, true, my_side, -999, 999, 6)->parent_move;
	}
	
    board->doMove(move, my_side);
	
	return move;
    
}
