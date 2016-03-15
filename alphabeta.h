#include "common.h"
#include "board.h"

struct alphabeta_move{
	alphabeta_move * parent;
	Move * parent_move;
	int value;
};
