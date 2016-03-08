#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "common.h"

using namespace std;

class Board {
   
private:
    bitset<64> black;
    bitset<64> taken;    
       
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
      
public:
    Board();
    ~Board();
    Board *copy();
    
    //arrays of things
    /*Move corners [4] = {Move(0,0), Move(0, 7), Move(7,0), Move(7, 7)};
	Move adj_to_corners[12] = {Move(0, 1), Move(1, 0), Move(1, 1), 
							   Move(1, 7), Move(0, 6), Move(1, 6),
							   Move(7, 1), Move(6, 0), Move(6, 1),
							   Move(6, 7), Move(7, 6), Move(6, 6)};
	Move edges[16] = {Move(0, 2), Move(0, 3), Move(0, 4), Move(0, 5),
					  Move(2, 0), Move(3, 0), Move(4, 0), Move(5, 0),
					  Move(7, 2), Move(7, 3), Move(7, 4), Move(7, 5),
					  Move(2, 7), Move(3, 7), Move(4, 7), Move(5, 7)};
	*/
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    
    std::vector<Move*> return_moves(Side side);
    //Move* return_best_move_heuristic(Side side);
    Move* return_minmax_move(Side side);
};

struct minmax_move{
	Move * parent;
	int score;
	Board * board;
};
#endif
