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
 * valid moves. Does not think ahead.
 */
  
/*Move* Board::return_best_move_heuristic(Side side)
{					  	
	Move* best_move = NULL;
	int best_move_score = -10;
	
	std::vector<Move*> moves = return_moves(side);
	
	for(int i = 0; i < moves.size(); i++)
	{
		//first check if the move is in a corner
		bool is_corner;
		
		for(int j = 0; j < 4; j++)
		{
			if(moves[i]->x == corners[j].x && moves[i]->y == corners[j].y)
			{
				is_corner = true;
			}
		}
		
		if(is_corner)
		{
			//corner is the best place! if a corner is chosen, the best move is the corner by default (score 3)
			best_move = moves[i];
			best_move_score = 3;
		}
		else
		{
			//if not in a corner, check if it's next to one
			bool is_adj_to_corner;
		
			for(int j = 0; j < 4; j++)
			{
				if(moves[i]->x == adj_to_corners[j].x && moves[i]->y == adj_to_corners[j].y)
				{
					is_adj_to_corner = true;
				}
			}
			
			if(is_adj_to_corner)
			{
				//we only want to pick a move adjacent to the corner if there are no other options, so give it score -1
				if (best_move_score < -1)
				{
					best_move_score = -1;
					best_move = moves[i];
				}
			}
			else
			{
				//edges are pretty good, just not as good as corners (score 2)
				bool is_edge;
		
				for(int j = 0; j < 4; j++)
				{
					if(moves[i]->x == edges[j].x && moves[i]->y == edges[j].y)
					{
						is_edge = true;
					}
				}
				if(is_edge)
				{
					if (best_move_score < 2)
					{
						best_move_score = 2;
						best_move = moves[i];
					}
				}
				else
				{
					//all other moves are just score 0
					if(best_move_score < 0)
					{
						best_move_score = 0;
						best_move = moves[i];
					}
				}
			}
		}
	}
	
	return best_move;
}*/			
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
 * Finds the best move by using the minmax algorithm. Goes down four levels
 * (your move -> opponent -> you ->opponent
 */ 
Move* Board::return_minmax_move(Side side)
{
	std::vector<Move*> my_moves = return_moves(side);
	std::vector<minmax_move*> first_level_moves;
	
	//construct tree of first level moves (after we make our move)
	for(int i = 0; i < my_moves.size(); i++)
	{
		minmax_move* move = new minmax_move();
		move->parent = my_moves[i];
		
		//create a new board with the move done on it to count the score
		Board * new_board = copy();
		new_board->doMove(my_moves[i], side);
		
		move->board = new_board;
		
		first_level_moves.push_back(move);
	}
	
	
	/*
	 * For each first level move, find all of its "children" moves. For each
	 * parent move, find the minimum child score, and reassign the score of the parent 
	 * to the minimum score of its children.
	 */ 
	for(int i = 0; i < first_level_moves.size(); i++)
	{	
		 std::vector<Move*> opponent_moves; //"children" moves
		 if(side == WHITE)
		 {
			 opponent_moves = first_level_moves[i]->board->return_moves(BLACK);
		 }
		 else
		 {
			 opponent_moves = first_level_moves[i]->board->return_moves(WHITE);
		 }
		 
		 int min_score = 999; // set impossibly high at first
		 
		 for(int j = 0; j < opponent_moves.size(); j++)
		 {
			 //create a new board with the move done on it to count the score
			 Board * new_board = first_level_moves[i]->board;
			 if(side == WHITE)
			 {
				new_board->doMove(opponent_moves[j], BLACK);
			 }
			 else
			 {
				 new_board->doMove(opponent_moves[j], WHITE);
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
			 
			  
			 if(score < min_score)
			 {
				 min_score = score;
			 }
			 /*std::cerr<<"Min score: ";
			  std::cerr<<min_score<<endl;
			  * */
		 }
		 
		 first_level_moves[i]->score = min_score;
	 }
	 
	 //find best move based on maxing the min_score
	 
	 Move * best_move = NULL;
	 int max_score = -999;
	 
	 for(int i = 0; i < first_level_moves.size(); i++)
	 {
		 /*std::cerr<<"Max score: ";
		 std::cerr<<max_score<<endl;
		 * */
		 if (first_level_moves[i]->score > max_score)
		 {
			 best_move = first_level_moves[i]->parent;
			 max_score = first_level_moves[i]->score;
		 }
	 }
	 
	 return best_move;
 }
	 
	 
	 

			 
			
		
		
