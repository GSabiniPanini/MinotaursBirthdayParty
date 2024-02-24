# MinotaursBirthdayParty

Proof of correctness:
The program runs without error. The minotaur maze doesnt finish until every guest has visited, and there is no real communication within guests. The second part functions at a baseline level using the suggested implementation. Not everyone visits the showroom, but everyone has the opportunity to do so

Efficiency:
The first part of the program runs in around O(N) time, the solution is largely limited by how long it will take for the random number generator to hit every guest.

The second part of the program runs in about O(N) time, for this part of the assignment there is not a need for every guest to visit the showroom, so the time is entirely restricted by how long the threads randomly sleep for. 

Experimental Evaluation:
For the first part of the assignment the solution I took is more than likely not the most ideal solution. I spent a lot of time thinking about how to make the solve using only the cupcake, and the replacing functionality. I think I was really on the line for the optimal solution, but until I could arrive at that optimal solution, I settled on a second best case. By assuming that the servants can be seen as they replace the cupcakes, and the condition that only one person is in the maze at a time, and will return before the next person even has the chance to request a new cupcake. No cupcake replacement will be missed in any case. My plan has one of the guests watch the servants and will count every time the servant leaves to replace. Each guest will only eat one cupcake total. If they are re-entered into the maze they will not eat the cupcake, but they can replace the cupcake. Once that chosen counter individual has counted N-1 cupcake replacements, they will signify to the minotaur that everyone has visited the maze.

For the second part of the solution I chose the 2nd option. Implementing a sign that has an availability on it seems the most optimal 
