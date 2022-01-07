#include "nim.h"
#include <stdio.h>
#include <stdlib.h>

//function to create a new board
int *new_board(int board_size) {

    int *array;

    array = malloc(sizeof(int)*board_size); //mallocing an array with appropriate size

    if(array==NULL) {  //checking if the malloc fails

	fprintf(stderr, "Problem with malloc\n");
	exit(1);
    }

    return array; //returning the array

}

//function to create a nim_hash array
struct node* mk_nim_hash( int max_hash, int board_size, int *start_board) {

    struct node *array; //variable needed

    array = malloc(sizeof(struct node)*max_hash); //mallocing enough space for the array

    if(array==NULL) { //checking if the malloc fails

	fprintf(stderr, "Problem with malloc\n");
	exit(1);
    }

    for (int i=0; i<max_hash; i++ ){ //initializing the attributes for every index of the array

	array[i].moves = -1;
	array[i].move = NULL;
	array[i].nimsum = -1;
	array[i].board = hash2board(board_size, start_board, i);

    }

    return array; //returning the finished array

}

//function to free the array board
void free_board(int *board) {

    free(board);
}

//function to free the nim_hash array
void free_nim_hash(int max_hash, struct node*nim_hash) {

    for (int i=0; i<max_hash; i++) { //looping through the array

	free(nim_hash[i].move); //freeing the move array
	free(nim_hash[i].board);//freeing the board array
    }

    free(nim_hash); //freeing the nim_hash array
}

//function to create a board from command line arguments
int *board_from_argv(int board_size, char **argv) {

    int *board_array; //variable needed
    board_array =new_board(board_size); //creating the board

    for(int i=0; i <board_size; i++) { //looping through the command line arguments

	board_array[i] = atoi(argv[i]); //initializing the array with the appropriate number of matches
    }

    return board_array; //returning the board array

}

//function to copy a board
int *copy_board(int board_size, int*board) {

    int *board_copy; //variable needed

    board_copy = new_board(board_size); //creating a new board with the same size as the original

    for(int i=0; i<board_size; i++) { //looping through the original array and copying the contents into the new board

	board_copy[i] = board[i];

    }

    return board_copy; //returning the board copy
}

//checking if the game is over
int game_over(int board_size, int *board) {

    int result=0; //variable needed

    for(int i=0; i <board_size; i++) { //looping through the board to get the total number of matches

	result += board[i];

    }

    if(result ==1 ){ //checking if there is only one match left

	return 1;

    }

    else {

	return 0;

    }
}

//function to join graph with all possible moves available from a board
void join_graph(struct node*nim_hash, int hash, int board_size,int *start_board) {

    int totalMoves = 0; //variables needed
    int rowMatches =0;
    int *tempBoard;
    int newHash =0;
    int k=0;

    if(nim_hash[hash].moves==-1) { //checking if the move has been computed

	for(int i=0; i<board_size; i++) { //looping through the board

	    totalMoves += nim_hash[hash].board[i]; //getting the total number of moves

	}

	nim_hash[hash].moves = totalMoves; //changing the moves attribute

	nim_hash[hash].nimsum = compute_nimsum(board_size, nim_hash[hash].board); //calling the compute nimsum to find the nimsum of the move

	nim_hash[hash].move = malloc(sizeof(struct move)*totalMoves); //mallocing enough space for the move array

	for(int i=0; i<board_size; i++) { //outer loop to determine the row we are in

	    rowMatches = nim_hash[hash].board[i]; //getting the number of matches in that row

	    for(int j=1; j<=rowMatches; j++) { //looping through all number of matches in the row

		tempBoard= copy_board(board_size, nim_hash[hash].board); //copying the board we are working with
		nim_hash[hash].move[k].row =i; //assigning the number of rows in the move array
		nim_hash[hash].move[k].matches = j; //assigning the number of matches in the move array
		tempBoard[i] = tempBoard[i] - j; //applying the move to the temp board
		newHash = board2hash(board_size, start_board, tempBoard); //getting the new hash variable from the temp board
		nim_hash[hash].move[k].hash=newHash; //assigning the new hash in the move array
		free(tempBoard); //freeing the temp board variable
		join_graph(nim_hash, newHash, board_size, start_board); //calling the recursion on each destination node
		k++; //incrementing the index of the move array

	    }
	}

    }

    else {

	return; //ignore if the move has been computed

    }

}

//function to calculate the nim sum of a given board
int compute_nimsum(int board_size, int *board) {

    int nimsum = 0; //variables needed
    int counter =0;

    for (int i=0; i <board_size; i++) { //looping through the board and finding the bitwise xor

	nimsum = nimsum ^ board[i];

    }

    for (int i=0; i<board_size; i++) { //looping through the board again

	if( board[i] >1) { //checking if the number of matches is greater than 1

	    counter++;

	}

    }

    if( nimsum != 0) { //checking if the nimsum value is greater than 0

	nimsum =1; //assign it to 0

    }

    if( counter == 0) { //checking if there are no values in board that are greater than 1

	return !nimsum; //negate the result and return

    }

    else {

	return nimsum; //return the original result

    }
}
