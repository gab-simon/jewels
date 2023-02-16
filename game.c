#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "draw.h"
#include "utils.h"

void verifyBestScore(board_t *board)
{
    if (board->points > board->bestScore)
    {
        board->bestScore = board->points;
        freopen("./score", "w", board->essencials->score);
        fprintf(board->essencials->score, "%d", board->bestScore);
    }
}

candy_t *createCandy()
{
    candy_t *newCandy = malloc(sizeof(candy_t));
    if (!newCandy)
        return NULL;

    newCandy->drawned = 0;
    return newCandy;
}

slot_t *createSlot()
{
    slot_t *newSlot = malloc(sizeof(slot_t));
    if (!newSlot)
        return NULL;

    newSlot->candy = NULL;
    newSlot->left = NULL;
    newSlot->right = NULL;
    newSlot->up = NULL;
    newSlot->down = NULL;
    newSlot->xDisplayPos = 0;
    newSlot->yDisplayPos = 0;
    newSlot->xBoardPos = 0;
    newSlot->yBoardPos = 0;

    return newSlot;
}

mission_t *createMission()
{
    mission_t *newMission = malloc(sizeof(mission_t));
    if (!newMission)
        return NULL;

    srand(time(NULL));
    newMission->candyType = 1 + (rand() % 5);
    printf("%d\n", newMission->candyType);
    newMission->candiesDestroyed = 0;
    newMission->missionsCompleted = 0;

    return newMission;
}

board_t *createBoard()
{
    board_t *newBoard = malloc(sizeof(board_t));
    if (!newBoard)
        return NULL;

    // Inicialize variables
    newBoard->points = 0;
    newBoard->bestScore = 0;
    newBoard->essencials = NULL;
    newBoard->mission = createMission();
    newBoard->level = 1;

    for (int i = 0; i < BOARD_SIZE_X; i++)
    {
        for (int j = 0; j < BOARD_SIZE_Y; j++)
        {
            newBoard->slots[i][j] = createSlot();
            newBoard->slots[i][j]->candy = createCandy();
            newBoard->slots[i][j]->candy->type = CT_NONE;
            newBoard->slots[i][j]->xBoardPos = i;
            newBoard->slots[i][j]->yBoardPos = j;
            newBoard->slots[i][j]->xDisplayPos = 0;
            newBoard->slots[i][j]->yDisplayPos = 0;
        }
        newBoard->emptySlotsColumns[i] = 0;
    }

    for (int j = 0; j < BOARD_SIZE_Y; j++)
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            if (j == 0)
                newBoard->slots[i][j]->up = NULL;
            else
                newBoard->slots[i][j]->up = newBoard->slots[i][j - 1];
            if (j == BOARD_SIZE_Y - 1)
                newBoard->slots[i][j]->down = NULL;
            else
                newBoard->slots[i][j]->down = newBoard->slots[i][j + 1];
            if (i == 0)
                newBoard->slots[i][j]->left = NULL;
            else
                newBoard->slots[i][j]->left = newBoard->slots[i - 1][j];
            if (i == BOARD_SIZE_X - 1)
                newBoard->slots[i][j]->right = NULL;
            else
                newBoard->slots[i][j]->right = newBoard->slots[i + 1][j];
        }

    return newBoard;
}

// ---------- MISSION CONTROL FUNCTIONS ----------
void verifyMission(board_t *board)
{
    srand(time(NULL));
    if (board->mission->candiesDestroyed == 15)
    {
        board->mission->candiesDestroyed = 0;
        board->mission->candyType = 1 + (rand() % 5);
        board->mission->missionsCompleted++;
        board->points += 500;
        board->level += 1;
    }
}

void addMission(board_t *board, int candyType, int quantity)
{
    if (board->mission->candyType == candyType)
    {
        if (board->mission->candiesDestroyed + quantity > 15)
            board->mission->candiesDestroyed = 15;
        else
            board->mission->candiesDestroyed += quantity;
    }
    verifyMission(board);
}

// ---------- VERIFY MATCH FUNCTIONS ----------

int verifyLMatches(board_t *board)
{
    int candyType = 0;

    // Down-Right
    for (int i = 0; i < BOARD_SIZE_X - 2; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i][j + 1]->candy->type &&
                candyType == board->slots[i][j + 2]->candy->type &&
                candyType == board->slots[i + 1][j + 2]->candy->type &&
                candyType == board->slots[i + 2][j + 2]->candy->type &&
                candyType != 0)
            {
                board->slots[i][j]->candy->type = 0;
                board->slots[i][j + 1]->candy->type = 0;
                board->slots[i][j + 2]->candy->type = 0;
                board->slots[i + 1][j + 2]->candy->type = 0;
                board->slots[i + 2][j + 2]->candy->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->points += 1000;
                addMission(board, candyType, 5);
                return 1;
            }
        }

    // Down-Left
    for (int i = 2; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i][j + 1]->candy->type &&
                candyType == board->slots[i][j + 2]->candy->type &&
                candyType == board->slots[i - 1][j + 2]->candy->type &&
                candyType == board->slots[i - 2][j + 2]->candy->type &&
                candyType != 0)
            {
                board->slots[i][j]->candy->type = 0;
                board->slots[i][j + 1]->candy->type = 0;
                board->slots[i][j + 2]->candy->type = 0;
                board->slots[i - 1][j + 2]->candy->type = 0;
                board->slots[i - 2][j + 2]->candy->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i - 1]++;
                board->emptySlotsColumns[i - 2]++;
                board->points += 1000;
                addMission(board, candyType, 5);
                return 1;
            }
        }

    // Up-Right
    for (int i = 0; i < BOARD_SIZE_X - 2; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i + 2][j]->candy->type &&
                candyType == board->slots[i + 1][j]->candy->type &&
                candyType == board->slots[i][j + 1]->candy->type &&
                candyType == board->slots[i][j + 2]->candy->type &&
                candyType != 0)
            {
                board->slots[i + 2][j]->candy->type = 0;
                board->slots[i + 1][j]->candy->type = 0;
                board->slots[i][j]->candy->type = 0;
                board->slots[i][j + 1]->candy->type = 0;
                board->slots[i][j + 2]->candy->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->points += 1000;
                addMission(board, candyType, 5);
                return 1;
            }
        }

    // Up-Left
    for (int i = 2; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i - 2][j]->candy->type &&
                candyType == board->slots[i - 1][j]->candy->type &&
                candyType == board->slots[i][j + 1]->candy->type &&
                candyType == board->slots[i][j + 2]->candy->type &&
                candyType != 0)
            {
                board->slots[i - 2][j]->candy->type = 0;
                board->slots[i - 1][j]->candy->type = 0;
                board->slots[i][j]->candy->type = 0;
                board->slots[i][j + 1]->candy->type = 0;
                board->slots[i][j + 2]->candy->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i - 1]++;
                board->emptySlotsColumns[i - 2]++;
                board->points += 1000;
                addMission(board, candyType, 5);
                return 1;
            }
        }

    return 0;
}

int verifyVerticalMatches(board_t *board)
{
    int candyType = 0;

    // Verify 4 candies match VERTICAL
    for (int i = 0; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 3; j++)
        {

            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i][j + 1]->candy->type &&
                candyType == board->slots[i][j + 2]->candy->type &&
                candyType == board->slots[i][j + 3]->candy->type &&
                candyType != 0)
            {
                board->slots[i][j]->candy->type = 0;
                board->slots[i][j + 1]->candy->type = 0;
                board->slots[i][j + 2]->candy->type = 0;
                board->slots[i][j + 3]->candy->type = 0;
                board->emptySlotsColumns[i] += 4;
                board->points += 400;
                addMission(board, candyType, 4);
                return 1;
            }
        }
    // Verify 3 candies match VERTICAL
    for (int i = 0; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {

            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i][j + 1]->candy->type &&
                candyType == board->slots[i][j + 2]->candy->type &&
                candyType != 0)
            {
                board->slots[i][j]->candy->type = 0;
                board->slots[i][j + 1]->candy->type = 0;
                board->slots[i][j + 2]->candy->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->points += 300;
                addMission(board, candyType, 3);
                return 1;
            }
        }

    return 0;
}

int verifyHorizontalMatches(board_t *board)
{
    int candyType = 0;

    // Verify 4 candies match HORIZONTAL
    for (int i = 0; i < BOARD_SIZE_X - 3; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y; j++)
        {

            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i + 1][j]->candy->type &&
                candyType == board->slots[i + 2][j]->candy->type &&
                candyType == board->slots[i + 3][j]->candy->type &&
                candyType != 0)
            {
                board->slots[i][j]->candy->type = 0;
                board->slots[i + 1][j]->candy->type = 0;
                board->slots[i + 2][j]->candy->type = 0;
                board->slots[i + 3][j]->candy->type = 0;
                board->emptySlotsColumns[i]++;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->emptySlotsColumns[i + 3]++;
                board->points += 400;
                addMission(board, candyType, 4);
                return 1;
            }
        }
    // Verify 3 candies match HORIZONTAL
    for (int i = 0; i < BOARD_SIZE_X - 2; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y; j++)
        {

            candyType = board->slots[i][j]->candy->type;

            if (candyType == board->slots[i + 1][j]->candy->type &&
                candyType == board->slots[i + 2][j]->candy->type &&
                candyType != 0)
            {
                board->slots[i][j]->candy->type = 0;
                board->slots[i + 1][j]->candy->type = 0;
                board->slots[i + 2][j]->candy->type = 0;
                board->emptySlotsColumns[i]++;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->points += 300;
                addMission(board, candyType, 3);
                return 1;
            }
        }

    return 0;
}

int verifyMatch(board_t *board, int reset)
{
    if (verifyLMatches(board) || verifyVerticalMatches(board) || verifyHorizontalMatches(board))
    {
        if (!reset)
            verifyBestScore(board);
        return 1;
    }
    return 0;
}

// ---------- BOARD CONTROL FUNCTIONS ----------

void resetDrawned(board_t *board)
{
    for (int i = 0; i < BOARD_SIZE_X; i++)
        for (int j = 0; j < BOARD_SIZE_Y; j++)
            board->slots[i][j]->candy->drawned = 0;
}

void changePositions(slot_t *slot, int direction)
{

    candy_t *candyAux = slot->candy;

    // If the change is invalid, return
    if ((slot->left == NULL && direction == D_LEFT) || (slot->right == NULL && direction == D_RIGHT) ||
        (slot->up == NULL && direction == D_UP) || (slot->down == NULL && direction == D_DOWN))
        return;
    if (direction == D_LEFT)
    {
        slot->candy = slot->left->candy;
        slot->left->candy = candyAux;
    }
    else if (direction == D_RIGHT)
    {
        slot->candy = slot->right->candy;
        slot->right->candy = candyAux;
    }
    else if (direction == D_UP)
    {
        slot->candy = slot->up->candy;
        slot->up->candy = candyAux;
    }
    else if (direction == D_DOWN)
    {
        slot->candy = slot->down->candy;
        slot->down->candy = candyAux;
    }
}

void dragColumnDown(board_t *board, int column)
{

    int done = 0;
    slot_t *slotAux = NULL;

    // While there are empty spaces between candies, drag the column down
    while (!done)
    {
        done = 1;
        slotAux = board->slots[column][BOARD_SIZE_Y - 2];
        // Drag the column down by one unit
        while (slotAux)
        {
            if (slotAux->candy->type != CT_NONE && slotAux->down->candy->type == CT_NONE)
            {
                changePositions(slotAux, D_DOWN);
                done = 0;
            }
            slotAux = slotAux->up;
        }
        board->slots[column][0]->candy->type = 1 + (rand() % 5);
    }
}

void fillEmptySlots(board_t *board, int animation)
{

    // ANIMATION PART
    if (animation)
        fallJewels(board);
    // LOGIC PART
    for (int i = 0; i < BOARD_SIZE_X; i++)
    {
        // Verify if the column has empty slots
        if (board->emptySlotsColumns[i])
        {
            // Candy fall
            dragColumnDown(board, i);
            board->emptySlotsColumns[i] = 0;
        }
    }
}

void resetBoard(board_t *board)
{
    for (int j = 0; j < BOARD_SIZE_Y; j++)
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            board->slots[i][j]->candy->type = 1 + (rand() % 5);
            board->slots[i][j]->xDisplayPos = i * DISTANCE + OFFSET_X;
            board->slots[i][j]->yDisplayPos = (j) * DISTANCE + OFFSET_Y;
        }

    srand(time(NULL));

    while (verifyMatch(board, 1))
        fillEmptySlots(board, 0);
    board->points = 0;
    board->mission->candiesDestroyed = 0;
    board->mission->missionsCompleted = 0;
    board->mission->candyType = 1 + (rand() % 5);
    board->level = 0;
}

// Verify if a movement is valid
int validTransition(slot_t *slot, int direction)
{
    if ((direction == D_DOWN && slot->yBoardPos == 15) ||
        (direction == D_UP && slot->yBoardPos == 8) ||
        (direction == D_RIGHT && slot->xBoardPos == 7) ||
        (direction == D_LEFT && slot->xBoardPos == 0))
        return 0;
    return 1;
}
