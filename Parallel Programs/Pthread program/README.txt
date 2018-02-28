Assignment for CIS 3090 

To run
-type make into your command line
-follow instructions for each program

Data
- The program is run on the command line and uses parameters to configure
- the size of the game and the number of threads.
- 
- The usage for the program is:
- 	./data <#-threads> <grid-size> <#-iterations> -d
- 
- <#-threads> is the number of threads the program will create to run
- 	the game. This is a required parameter.
- <grid-size> is one number that represents both the height and width
- 	of the grid. This is a required parameter.
- <#-iterations> is how many iterations of the game the program will
- 	run before exiting. This is a required parameter.
- -d This indicates that the program should print out the grid after each
- 	iteration. If the -d is present then the output should be displayed.
- 	If the -d is not present then the output is not displayed. Each new
- 	grid is calculated but the grid is never drawn on the screen.
- 	This is an optional parameter.
- 
- The parameters will always appear in this order.
- 
- For example:
- 	data 4 10 100
- will create 4 threads, a grid size of 10x10, run for 100 iterations, and
- never display the grid.

Task
This program also implements the Game of Life but instead of having
- all threads perform the same actions, each thread specializes in
- a different operation.
- 
- ./task <grid-size> <#-iterations> -d
- 
- <grid-size> is one number that represents both the height and width of the grid. This is a required parameter.
- <#-iterations> is how many iterations of the game the program will run before exiting. This is a required parameter.
- -d This indicates that the program should print out the grid after each iteration. 
- 
- One thread does nothing but counts the number of neighbours for each
- square in the grid. The location for each square is stored in
- a queue that indicates if the square should be occupied in the next
- iteration. There will need to be two queues. One indicating that the
- location should be occupied (live) for the next iteration and another
- indicating the location should be empty for the next iteration. 
- 
- A second thread should read through the occupied queue and update the
- next iteration game board. This should mark the location on the board
- as occupied.
- 
- A third thread thread should read through the empty queue and update the
- next iteration game board. This should mark the location on the board
- as unoccupied.
