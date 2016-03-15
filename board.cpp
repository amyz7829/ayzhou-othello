#include "board.h"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Given the current board, find the valid moves for the given
 * side.
 */
std::vector<Move*> Board::return_moves(Side side)
{
	std::vector<Move*> moves;
	for (int i = 0; i < 8; i++) 
	{
        for (int j = 0; j < 8; j++) 
		{
            Move* move = new Move(i, j);
            if (checkMove(move, side))
            {
				moves.push_back(move);
			}	
        }
    }
    return moves;
}

/*
 * A basic heuristic function that "ranks" the moves out of the current
 * valid moves. Does not look ahead. Uses disc count, legal moves count, 
 * and corner squares count. (found on cs.cornell.edu/~yuli/othello/othello.html)
 * 
 * Legal Moves count = how many moves you will have after opponent makes their move(presuming opponent makes move limiting our mobility)
 * Discs = 1
 * Legal Moves = 100
 * Corner Squares = 1000
 *
 * Higher the score the better the move!
 * However, if the move we make allows the opponent to take a corner, give that move a terrible score. (does not necessarily mean next to corner is bad- ONLY 
 * if it gives the opponent a valid move that lets them take the corner.
 */
  
Move* Board::return_best_move_heuristic(Side side)
{					  	
	std::vector<Move*> moves = return_moves(side);
	
	int best_score = -999;
	Move* best_move = NULL;
	
	for(int i = 0; i< moves.size(); i++)
	{
		int legal_move_count = 999; //takes min after opponent makes optimal move
		int disc_count;
		int total_score;
		bool isCorner = false;
		bool givesCorner = false;
		
		if(moves[i]->x == 0 && moves[i]->y == 0 ||moves[i]->x == 0 && moves[i]->y == 7||moves[i]->x == 7 && moves[i]->y == 0||moves[i]->x == 7 && moves[i]->y == 7)
		{
			isCorner = true;
		}
		
		Board * new_board = copy();
		new_board->doMove(moves[i], side);
		
		int white = new_board->countWhite();
		int black = new_board->countBlack();
		
		std::vector<Move*> opponent_moves;
		
		if(side == WHITE)
		{
			opponent_moves = new_board->return_moves(BLACK);
			disc_count = white - black;
		}
		else
		{
			opponent_moves = new_board->return_moves(WHITE);
			disc_count = black - white;
		}
		
		for(int j = 0; j < opponent_moves.size(); j++)
		{
			Board * new_board_2 = new_board->copy();
			
			if(opponent_moves[j]->x == 0 && opponent_moves[j]->y == 0 ||opponent_moves[j]->x == 0 && opponent_moves[j]->y == 7||opponent_moves[j]->x == 7 && opponent_moves[j]->y == 0||opponent_moves[j]->x == 7 && opponent_moves[j]->y == 7)
			{
				givesCorner = true;
			}
			
			if(side == WHITE)
			{
				new_board_2->doMove(opponent_moves[j], BLACK);
			}
			else
			{
				new_board_2->doMove(opponent_moves[j], WHITE);
			}
			
			if(new_board_2->return_moves(side).size() < legal_move_count)
			{
				legal_move_count = new_board_2->return_moves(side).size();
			}
		}
		
		total_score = legal_move_count * 10 + disc_count; 
		
		if(isCorner)
		{
			total_score += 1000;
		}
		
		if(givesCorner)
		{
			total_score -= 1000;
		}
		
		if(total_score > best_score)
		{
			best_move = moves[i];
			best_score = total_score;
		}
	}
	
	return best_move;
}		
		
/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

/*
 * A recursive function that passes the best move "up" using the alpha beta struct,
 * which stores the parent, initial move, alpha value, beta value, as well as the board state of that move. 
 * If the alpha value is greater than the beta value then cut off the branch. 
 */
 
alphabeta_move* Board::return_alphabeta_move(Board* board, bool myPlayer, Side side, int alpha, int beta, int depth)
{
	alphabeta_move* move = new alphabeta_move();
	if(depth == 0)
	{
		int black = board->countBlack();
		int white = board->countWhite();
		
		if(side == WHITE)
		{
			move->value = white - black;
		}
		else
		{
			move->value = black - white;
		}
		return move;
	}
	
	std::vector<Move*> child_moves = board->return_moves(side);
	
	if(child_moves.size() == 0)
	{
		int black = board->countBlack();
		int white = board->countWhite();
		
		if(side == WHITE)
		{	
			move->value = white - black;
		}
		else
		{
			move->value = black - white;
		}
		return move;
	}
	
	for(int i = 0; i < child_moves.size(); i ++)
	{
		Board* new_board = board->copy();
		
		move = return_alphabeta_move(new_board, myPlayer, side, alpha, beta, depth - 1);
		
		//record the move that was taken and the parent
		move->parent_move = child_moves[i];
		
		if(myPlayer)
		{
			if(move->value > alpha)
			{
				alpha = move->value;
			}
		}
		else
		{
			if(move->value < beta)
			{
				beta = move->value; 
			}
		}
		if (beta <= alpha)
		{
			break;
		}
	}
	return move;
}

/*
 * Finds the best move by using the minmax algorithm. Goes down four levels
 * (your move -> opponent -> you ->opponent
 */ 
 /*
Move* Board::return_minmax_move(Side side)
{
	std::vector<Move*> my_moves = return_moves(side);
	std::vector<minmax_move*> first_level_moves;
	
	//construct tree of first level moves (after we make our move)
	for(int i = 0; i < my_moves.size(); i++)
	{
		minmax_move* move = new minmax_move();
		move->parent = NULL;
		
		//create a new board with the move done on it to count the score
		Board * new_board = copy();
		new_board->doMove(my_moves[i], side);
		
		move->board = new_board;
		move->move = my_moves[i];
		
		first_level_moves.push_back(move);
	}
	
	
	/*
	 * For each first level move, find all of its "children" moves. 
	 */ 
	 
	//std::vector<minmax_move*> second_level_moves;
	//for(int i = 0; i < first_level_moves.size(); i++)
	//{	
		 //std::vector<Move*> opponent_moves; //"children" moves
		 //if(side == WHITE)
		 //{
			 //opponent_moves = first_level_moves[i]->board->return_moves(BLACK);
		 //}
		 //else
		 //{
			 //opponent_moves = first_level_moves[i]->board->return_moves(WHITE);
		 //}
		 
		
		 
		 //for(int j = 0; j < opponent_moves.size(); j++)
		 //{
			 ////create a new board with the move done on it to count the score
			 //Board * new_board = first_level_moves[i]->board;
			 //if(side == WHITE)
			 //{
				//new_board->doMove(opponent_moves[j], BLACK);
			 //}
			 //else
			 //{
				 //new_board->doMove(opponent_moves[j], WHITE);
			 //}
			 //minmax_move* move = new minmax_move();
			 //move->parent = first_level_moves[i];
			 //move->board = new_board;
			 //second_level_moves.push_back(move);
		 //}
	 //}
	 
	 ////now find the next level of moves (your turn!)
	
	//std::vector<minmax_move*> third_level_moves;
	//for(int i = 0; i < second_level_moves.size(); i++)
	//{	
		 //std::vector<Move*> my_moves; 
		 //if(side == WHITE)
		 //{
			 //my_moves = second_level_moves[i]->board->return_moves(WHITE);
		 //}
		 //else
		 //{
			 //my_moves = second_level_moves[i]->board->return_moves(BLACK);
		 //}
		 
		
		 
		 //for(int j = 0; j < my_moves.size(); j++)
		 //{
			 ////create a new board with the move done on it to count the score
			 //Board * new_board = second_level_moves[i]->board;
			 //if(side == WHITE)
			 //{
				//new_board->doMove(my_moves[j], WHITE);
			 //}
			 //else
			 //{
				 //new_board->doMove(my_moves[j], BLACK);
			 //}
			 //minmax_move* move = new minmax_move();
			 //move->parent = second_level_moves[i];
			 //move->board = new_board;
			 //third_level_moves.push_back(move);
		 //}
	 //}
	 
	///* 
	 //* Now that we've reached the last level we're searching, we'll find
	 //* the final "children" moves and find the minimum child score for each
	 //* "parent" move. We'll then move those scores "up" the table accordingly.
	 //*/
	  
	//for(int i = 0; i < third_level_moves.size(); i++)
	//{	
		 //std::vector<Move*> opponent_moves; //"children" moves
		 //if(side == WHITE)
		 //{
			 //opponent_moves = third_level_moves[i]->board->return_moves(BLACK);
		 //}
		 //else
		 //{
			 //opponent_moves = third_level_moves[i]->board->return_moves(WHITE);
		 //}
		 
		 //int min_score = 999; //start impossibly high
		 
		 //for(int j = 0; j < opponent_moves.size(); j++)
		 //{
			 ////create a new board with the move done on it to count the score
			 //Board * new_board = third_level_moves[i]->board;
			 //if(side == WHITE)
			 //{
				//new_board->doMove(opponent_moves[j], BLACK);
			 //}
			 //else
			 //{
				 //new_board->doMove(opponent_moves[j], WHITE);
			 //}
			 
			 //int black = new_board->countBlack();
			 //int white = new_board->countWhite();
			 //int score;
			 
			 //if(side == WHITE)
			 //{
				 //score = white - black;
			 //}
			 //else
			 //{
				 //score = black - white;
			 //}
			 
			 //if(score < min_score)
			 //{
				 //min_score = score;
			 //} 
		 //}
		 //third_level_moves[i]->score = min_score;
	 //}
	 
	 ///*
	  //* We are now going to find the "max" of these children of the 
	  //* second level because that is our best move. Set score of their 
	  //* parents to the highest score of their children
	  //*/
	  
	  //int max_score = -999;
	  
	  //for(int i = 0; i < third_level_moves.size(); i++)
	  //{
		  //if(third_level_moves[i]->score > max_score)
		  //{
			  //third_level_moves[i]->parent->score = max_score;
		  //}
	  //}
	  
	  ///*
	   //* We are now again finding the min through a similar process. 
	   //*/
	   
	   //int min_score = 999;
	   
	   //for(int i = 0; i < second_level_moves.size(); i++)  
	   //{
		   //if(second_level_moves[i]->score < min_score)
		   //{
			   //second_level_moves[i]->parent->score = min_score;
			//}
		//}
		
		///*
		 //* Final level! We'll finally figure out which of the first moves
		 //* we made had the best final result.
		 //*/
		 
		 //max_score = -999;
		 //Move* best_move = NULL;
		 //for(int i = 0; i < first_level_moves.size(); i++)
		 //{
			 //if(first_level_moves[i]->score > max_score)
			 //{
				 //best_move = first_level_moves[i]->move;
				 //max_score = first_level_moves[i]->score;
			 //}
		 //}
		 
		 //return best_move;
 //}
//*/
Move* Board::return_minmax_move_better(Side side, int depth)
{
	std::vector<std::vector<minmax_move*> > move_levels;
	
	//first set up the first level of moves to "get the ball rolling"
	std::vector<Move*> children_moves = return_moves(side);
	std::vector<minmax_move*> my_moves;
	
	for(int i = 0; i < children_moves.size(); i++)
	{
		minmax_move* move = new minmax_move();
		move->parent = NULL;
		
		Board * new_board = copy();
		new_board->doMove(children_moves[i], side);
		
		move->board = new_board;
		move->move = children_moves[i];
		
		my_moves.push_back(move);
	}
	
	move_levels.push_back(my_moves);
	
	
	for(int n = 1; n < depth - 1; n++)
	{
			if(n % 2 == 0)
			{
				std::vector<Move*> children_moves;
				std::vector<minmax_move*> my_moves;
				for(int i = 0; i < move_levels[n - 1].size(); i++)
				{
					if(side == WHITE)
					{
						children_moves = move_levels[n - 1][i]->board->return_moves(WHITE);
					}
					else
					{
						children_moves = move_levels[n - 1][i]->board->return_moves(BLACK);
					}
					
					for(int j = 0; j < children_moves.size(); j++)
					{
						//create new board so moves can be done to it
						Board * new_board = move_levels[n - 1][i]->board->copy();
						
						if(side == WHITE)
						{
							new_board->doMove(children_moves[j], WHITE);
						}
						else
						{
							new_board->doMove(children_moves[j], BLACK);
						}
						
						minmax_move* move = new minmax_move();
						move->parent = move_levels[n - 1][i];
						move->board = new_board;
						my_moves.push_back(move);
					}
				}
				move_levels.push_back(my_moves);
			}
			else
			{
				std::vector<Move*> children_moves;
				std::vector<minmax_move*> opponent_moves;
				for(int i = 0; i < move_levels[n - 1].size(); i++)
				{
					if(side == WHITE)
					{
						children_moves = move_levels[n - 1][i]->board->return_moves(BLACK);
					}
					else
					{
						children_moves = move_levels[n - 1][i]->board->return_moves(WHITE);
					}
					
					for(int j = 0; j < children_moves.size(); j++)
					{
						//create new board so moves can be done to it
						Board * new_board = move_levels[n - 1][i]->board->copy();
						
						if(side == WHITE)
						{
							new_board->doMove(children_moves[j], BLACK);
						}
						else
						{
							new_board->doMove(children_moves[j], WHITE);
						}
						
						minmax_move* move = new minmax_move();
						move->parent = move_levels[n - 1][i];
						move->board = new_board;
						opponent_moves.push_back(move);
					}
				}
				move_levels.push_back(opponent_moves);
			}
		}
		
		//start the ball rolling by calculating the scores at the bottom level
		for(int i = 0; i < move_levels[depth - 2].size(); i++)
		{
			std::vector<Move*> children_moves;
			if(depth % 2 == 0)
			{
				children_moves = move_levels[depth - 2][i]->board->return_moves(side);
			}
			else
			{
				if(side == WHITE)
				{
					children_moves = move_levels[depth - 2][i]->board->return_moves(BLACK);
				}
				else
				{
					children_moves = move_levels[depth - 2][i]->board->return_moves(WHITE);
				}
			}
			int min_score = -999;
			int max_score = 999;
			//only one f these is used
			for(int j = 0; j < children_moves.size(); j++)
			{
				Board* new_board = move_levels[depth - 2][j]->board->copy();
				if(side == WHITE)
				{
					new_board->doMove(children_moves[j], BLACK);
				}
				else
				{
					new_board->doMove(children_moves[j], WHITE);
				}
				
				int black = new_board->countBlack();
				int white = new_board->countWhite();
				int score;
				
				if(side == WHITE)
				{
					score = white - black;
				}
				else
				{
					score = black - white;
				}
				
				if(depth % 2 == 0)
				{
					if(score > max_score)
					{
						max_score = score;
					}
				}
				else
				{
					if(score < min_score)
					{
						min_score = score;
					}
				}
			}
			if(depth % 2 == 0)
			{
				move_levels[depth - 2][i]->score = max_score;
			}
			else
			{
				move_levels[depth - 2][i]->score = min_score;
			}
		}
					
		//now to bring it all back up to the top... joy
		for(int n = depth - 2; n > 0; n--)
		{
			//maxing level
			if(n % 2 == 0)
			{
				int max_score = -999;
				for(int i = 0; i < move_levels[n].size(); i++)
				{
					if(move_levels[n][i]->score > max_score)
					{
						move_levels[n][i]->parent->score = max_score;
					}
				}
			}
			else
			{
				int min_score = 999;
				for(int i = 0; i < move_levels[n].size(); i++)
				{
					if(move_levels[n][i]->score < min_score)
					{
						move_levels[n][i]->parent->score = min_score;
					}
				}
			}
		}
		
		//we're now going to choose which one of the original moves was best
		
		int max_score = -999;
		Move* best_move = NULL;
		
		for(int i = 0; i < move_levels[0].size(); i++)
		{
			if(move_levels[0][i]->score > max_score)
			{
				best_move = move_levels[0][i]->move;
				max_score = move_levels[0][i]->score;
			}
		}
		
		return best_move;
}
					
			
			
				
				
				
	 
	 

			 
			
		
		
