Caltech CS2 Assignment 9: Othello

See [assignment9_part1.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part1.html) and [assignment9_part2.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part2.html)

When creating my AI, I had a lot of struggles trying to understand its thought and how to optimize its strategy given that I did not have the time/memory to 
brute force the game. I separated my do_move function into two parts, depending on how far into the game it was. Approximately the first half of the game was 
played by a heuristic obtained online (sourced in the function), which valued the mobility of moves over how many discs my player had in the beginning of the game, 
as well as prioritizing getting corners). I also experimented with having it deprioritize moves next to corners, thinking of those as 'weak' positions, but it turned out 
that this actually made he performance of my AI decrease, likely because it prohibited it from making moves that were far more benefiical in the other categories. 
What I ended up using as a deprioritizer were moves that allowed the opponent to take the corner immediately after my move, because not all moves next to the
corner allow the opponent to make such a move. This change allowed me to win slighly more of the time.  

After this stage, my AI begins using a search tree to find the move that provided the most optimal result of a minmax tree given the depth. I found that this could
often backfire on me because my heuristic at this point was simply disc count. But since my AI (until the last few moves) cannot go all the way to the end, it is possible
that the moves it makes are actually suboptimal and can give the opponent the advantage in the next few moves as my discs become theirs. I remade my minmax 
algorithm (originally a series of several loops with manually adjusted depth) into one where depth was a parameter. Because I only run this minimax function at the
end, it does not take too long to run as the space of possible plays has been reduced dramatically. (alpha-beta pruning was implemented, but there is likely 
something wrong with it as it actually does not cause as much success as the minmax function, so that was kept due to the increased win chance)

In experiments to improve my AI without making drastic changes to my code, I have tweaked the value at which my do_move function switches from the heuristic to the 
alpha beta function. It was originally set at 40, which allowed my heuristic + minmax function to beat SimplePlayer a majority of the time (20/21) in the first set, but
when playing against the stronger AI, and being subsequently destroyed, it was clear that I had to change this value. I found that extending the amount of time that
the heuristic had to play improved my AI in two ways. First, it gave it more it more time to find corners, and it also aided the alpha beta function by reducing the
amount of moves it had to calculate. (the more squares already filled, the less potential moves it has to calculate) However, this system is still not perfect, but after
testing, I am able to beat the ConstantTimePlayer a majority of the time, about 3/4 of the time when going second, 1/2 -> 2/3 when going first.) 

My main goals when creating/optimizing this AI were to use this dual strategy as a way of "adapting" my AI to change its strategy during different parts of the game. I 
was not, in the end, able to achieve a great AI, but I did attempt a good amount of strategies, such as changing my heuristics to see if I could allow for a better set of
moves to be performed. I tried changing where the switch occurred, and while I found a general area where my AI performed better (heuristics for first 45-55  moves), 
it still was not to the point where I would like it to be. 

If I had more time to improve my AI, I would likely do an overhaul of the heuristics scoring throughout the entire thing. I originally stuck with disc count as it was the 
easiest thing to keep track of, but it became clear that I need more than this to judge the board at all states of the game. If given the chance to redo it, I would write a 
heuristics function optimized for disc count, mobility, but also something I did not already implement- stability. I believe a large reason of my AI's current failure 
lies in its lack of being able to judge if a move is stable, which makes it very vulnerable to losing all of its discs very quickly. I would also go back and figure out where 
my alpha-beta is returning results that do not appear optimal and extend the search tree in the last half of the game (the search tree portion) to search the entire 
last part of the game using alpha-beta pruning, as well as "saving" this book so it can simply reference this to finish the game. 