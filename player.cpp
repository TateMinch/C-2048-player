#include <iostream>
#include "board.h"

using namespace std;

#include "player.h"

/**************************************************************************//**
 * This function is the constructor for the player class and initializes a 
 * 2d array called weights
 *
 *****************************************************************************/
Player::Player()
{
	//nested for loop to loop through every element
	//in a weights 2d array that is equibalent size
	//to game board
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		//initializes each element of weights array
			weights[i][j] = 16-((4*i)+j+1);
}

/**************************************************************************//**
 * This function determines the move with the highest score and returns it to
 * be made
 *
 *
 * @param[in] b - the current board
 *
 * @return - The determined best move to be made
 *****************************************************************************/
ValidMove Player::makeMove(const Board b)
{
	//initializes bestMove to NONE and bestScore 
	//to the lowest possible value 
	ValidMove bestMove = NONE;
	double bestScore = numeric_limits<double>::lowest();
	
	//loops through each of the possible moves on the board
	for (ValidMove move : {DOWN, LEFT, RIGHT, UP})
	{
		//calculates score with the calculateScore function
		double score = calculateScore(b, move);
		//if score is better than best score
		if (score > bestScore)
		{
			//bestScore is replaced with current move in loops score
			bestScore = score;
			//bestMove is replaced with current move in loop
			bestMove = move;
		}
	}
	//returns bestMove after loop is fully executed and 
	//the best score for each move has been found.
	return bestMove;
}

/**************************************************************************//**
 * This function calculates the score for the provided move
 *
 *
 * @param[in] b - the current board
 * @param[in] move - the move which score must be calculated for
 *
 * @return - the value obtained from generateScore function with a new board 
 * sent in
 *****************************************************************************/
double Player::calculateScore(const Board b, ValidMove move)
{
	//creates new copy of current board
	Board newBoard(b);
	//if the move is not valid, return lowest value
	if (!newBoard.checkMove(move))
		return numeric_limits<double>::lowest();

	//returns score from generateScore function with the
	//copy of the board sent in
	return generateScore(newBoard, 0, 2);
}

/**************************************************************************//**
 * This function generates the score of the current board
 *
 * @param[inout] b - copy of current board
 * @param[in] currentDepth - the depth of the board
 * @param[in] depthLimit - the maximum depth
 *
 * @return - the total calculated score
 *****************************************************************************/
double Player::generateScore(const Board b, int currentDepth, int depthLimit)
{
	//creates another copy of current board
	Board board(b);
	//if current depth is equal to depthLimit then game is over
	if (currentDepth==depthLimit)
		return calculateFinalScore(board);

	//initializes totalScore to 0
	double totalScore = 0;
	
	//nested for loop to access every element on the board copy
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//creates another copy of board
			Board newBoard(b);
			//if the cell value is 0
			if (newBoard.board[i][j] == 0)
			{
				//sets value to 2
				newBoard.board[i][j] = 2;
				//calculates move score with calculateMoveScore function
				double moveScore = calculateMoveScore(newBoard, currentDepth, depthLimit);
				//increments totalScore by 0.9 times moveScore
				totalScore += (0.9 * moveScore);
				//sets cell value to 4
				newBoard.board[i][j] = 4;
				//calculates moveScore again with new cell value on board copy
				moveScore = calculateMoveScore(newBoard, currentDepth, depthLimit);
				//increments totalScore by 0.1 times recalculated moveScore
				totalScore += (0.1 * moveScore);
			}
		}
	}
	//returns totalScore
	return totalScore;
}

/**************************************************************************//**
 * This function calulates the score after a specific move is made
 *
 *
 * @param[in] b - the current copy of the board
 * @param[in] currentDepth - the current depth of the board
 * @param[in] depthLimit - the maximum depth allowed 
 *
 * @return - the best possible score after all moves
 *****************************************************************************/
double Player::calculateMoveScore(const Board b, int currentDepth, int depthLimit)
{
	//initializes bestScore to lowest possible value
	double bestScore = numeric_limits<double>::lowest();
	//for loop to loop through every possible move on board copy
	for (ValidMove move : {DOWN, LEFT, RIGHT, UP})
	{
		//creates another copy of the board
		Board newBoard(b);
		//if the move is valid
		if (newBoard.checkMove(move))
		{
			//calculate score with generateScore function and current depth incremented
			double score = generateScore(newBoard, currentDepth+1, depthLimit);
			//if score is greater than bestScore
			if (score > bestScore)
				//replace bestScore with the score value
				bestScore = score;
		}
	}
	//return bestScore after completion of for loop
	return bestScore;
}

/**************************************************************************//**
 * This function calculates the final score
 *
 *
 * @param[in] b - current copy of board
 *
 * @return - final score
 *****************************************************************************/
double Player::calculateFinalScore(const Board b)
{
	//initializes variables to 0
	double score = 0;
	int empty = 0;
	int merges = 0;
	int sum = 0;
	int location = 0;

	//for loop that increments 4 times
	for (int i = 0; i < 4; i++)
	{
		//if the first two values in the row are equal or the second and third are equal and the second value does not equal 0 
		if ((b.board[i][0] == b.board[i][1] || b.board[i][1] == b.board[i][2]) && b.board[i][1] != 0)
			//increment merges
			merges++;
		//if the third and fourth values are equal and the first and second values are equal or the second and third are not equal and 
		//the second value does not equal 0
		if (b.board[i][2] == b.board[i][3] && (b.board[i][0] == b.board[i][1] || !(b.board[i][1] == b.board[i][2])) && b.board[i][2] != 0)
		//increment merges
			merges++;
		if ((b.board[0][i] == b.board[1][i] || b.board[1][i] == b.board[2][i]) && b.board[1][i] != 0)
			merges++;
		if (b.board[2][i] == b.board[3][i] && (b.board[0][i] == b.board[1][i] || !(b.board[1][i] == b.board[2][i])) && b.board[2][i] != 0)
			merges++;
		//for loop that loops 4 times
		for (int j = 0; j < 4; j++)
		{
			//sum is incremented by the value of the current cell
			sum += b.board[i][j];
			//if the cell value is equal to 0
			if (b.board[i][j] == 0)
				//empty is incremented by 1
				empty++;
			//location is incremented by the equivalent weight value times the value of the board cell
			location += weights[i][j] * b.board[i][j];
		}
	}
	//score is incremented by 370 times the empty variable
	score += 370 * empty;
	//score is incremented by 700 times the merges variable
	score += 700 * merges;
	//score is decremented by 11 times the sum variable
	score -= 11 * sum;
	//score is incremented by the location variable
	score += location;
	
	//score is returned
	return score;
}