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

flower_t *createCandy()
{
    flower_t *newCandy = malloc(sizeof(flower_t));
    if (!newCandy)
        return NULL;

    newCandy->shown = 0;
    return newCandy;
}

slot_t *createSlot()
{
    slot_t *newSlot = malloc(sizeof(slot_t));
    if (!newSlot)
        return NULL;

    newSlot->flower = NULL;
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

mission_t *createTask()
{
    mission_t *newTask = malloc(sizeof(mission_t));
    if (!newTask)
        return NULL;

    srand(time(NULL));
    newTask->flowersType = 1 + (rand() % 5);

    printf("%d\n", newTask->flowersType);

    newTask->flowersDestroy = 0;

    return newTask;
}

board_t *createBoard()
{
    board_t *newBoard = malloc(sizeof(board_t));
    if (!newBoard)
        return NULL;

    newBoard->points = 0;
    newBoard->bestScore = 0;
    newBoard->essencials = NULL;
    newBoard->tasks = createTask();
    newBoard->level = 1;

    for (int i = 0; i < BOARD_SIZE_X; i++)
    {
        for (int j = 0; j < BOARD_SIZE_Y; j++)
        {
            newBoard->slots[i][j] = createSlot();
            newBoard->slots[i][j]->flower = createCandy();
            newBoard->slots[i][j]->flower->type = FLOWER_NONE;
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

void verifyMission(board_t *board)
{
    srand(time(NULL));
    if (board->tasks->flowersDestroy == 15)
    {
        board->tasks->flowersDestroy = 0;
        board->tasks->flowersType = 1 + (rand() % 5);
        board->points += 1000;
        board->level += 1;
    }
}

void addTask(board_t *board, int flowersType, int quantity)
{
    if (board->tasks->flowersType == flowersType)
    {
        if (board->tasks->flowersDestroy + quantity > 15)
            board->tasks->flowersDestroy = 15;
        else
            board->tasks->flowersDestroy += quantity;
    }
    verifyMission(board);
}

// ---------- VERIFY MATCH FUNCTIONS ----------

int verifyLMatches(board_t *board)
{
    int flowersType = 0;

    // Down-Right
    for (int i = 0; i < BOARD_SIZE_X - 2; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i][j + 1]->flower->type &&
                flowersType == board->slots[i][j + 2]->flower->type &&
                flowersType == board->slots[i + 1][j + 2]->flower->type &&
                flowersType == board->slots[i + 2][j + 2]->flower->type &&
                flowersType != 0)
            {
                board->slots[i][j]->flower->type = 0;
                board->slots[i][j + 1]->flower->type = 0;
                board->slots[i][j + 2]->flower->type = 0;
                board->slots[i + 1][j + 2]->flower->type = 0;
                board->slots[i + 2][j + 2]->flower->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->points += 1000;
                addTask(board, flowersType, 5);
                return 1;
            }
        }

    // Down-Left
    for (int i = 2; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i][j + 1]->flower->type &&
                flowersType == board->slots[i][j + 2]->flower->type &&
                flowersType == board->slots[i - 1][j + 2]->flower->type &&
                flowersType == board->slots[i - 2][j + 2]->flower->type &&
                flowersType != 0)
            {
                board->slots[i][j]->flower->type = 0;
                board->slots[i][j + 1]->flower->type = 0;
                board->slots[i][j + 2]->flower->type = 0;
                board->slots[i - 1][j + 2]->flower->type = 0;
                board->slots[i - 2][j + 2]->flower->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i - 1]++;
                board->emptySlotsColumns[i - 2]++;
                board->points += 1000;
                addTask(board, flowersType, 5);
                return 1;
            }
        }

    // Up-Right
    for (int i = 0; i < BOARD_SIZE_X - 2; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i + 2][j]->flower->type &&
                flowersType == board->slots[i + 1][j]->flower->type &&
                flowersType == board->slots[i][j + 1]->flower->type &&
                flowersType == board->slots[i][j + 2]->flower->type &&
                flowersType != 0)
            {
                board->slots[i + 2][j]->flower->type = 0;
                board->slots[i + 1][j]->flower->type = 0;
                board->slots[i][j]->flower->type = 0;
                board->slots[i][j + 1]->flower->type = 0;
                board->slots[i][j + 2]->flower->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->points += 1000;
                addTask(board, flowersType, 5);
                return 1;
            }
        }

    // Up-Left
    for (int i = 2; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {
            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i - 2][j]->flower->type &&
                flowersType == board->slots[i - 1][j]->flower->type &&
                flowersType == board->slots[i][j + 1]->flower->type &&
                flowersType == board->slots[i][j + 2]->flower->type &&
                flowersType != 0)
            {
                board->slots[i - 2][j]->flower->type = 0;
                board->slots[i - 1][j]->flower->type = 0;
                board->slots[i][j]->flower->type = 0;
                board->slots[i][j + 1]->flower->type = 0;
                board->slots[i][j + 2]->flower->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->emptySlotsColumns[i - 1]++;
                board->emptySlotsColumns[i - 2]++;
                board->points += 1000;
                addTask(board, flowersType, 5);
                return 1;
            }
        }

    return 0;
}

int verifyVerticalMatches(board_t *board)
{
    int flowersType = 0;

    // 4 blocks VERTICAL
    for (int i = 0; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 3; j++)
        {

            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i][j + 1]->flower->type &&
                flowersType == board->slots[i][j + 2]->flower->type &&
                flowersType == board->slots[i][j + 3]->flower->type &&
                flowersType != 0)
            {
                board->slots[i][j]->flower->type = 0;
                board->slots[i][j + 1]->flower->type = 0;
                board->slots[i][j + 2]->flower->type = 0;
                board->slots[i][j + 3]->flower->type = 0;
                board->emptySlotsColumns[i] += 4;
                board->points += 450;
                addTask(board, flowersType, 4);
                return 1;
            }
        }
    // 3 blocks VERTICAL
    for (int i = 0; i < BOARD_SIZE_X; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y - 2; j++)
        {

            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i][j + 1]->flower->type &&
                flowersType == board->slots[i][j + 2]->flower->type &&
                flowersType != 0)
            {
                board->slots[i][j]->flower->type = 0;
                board->slots[i][j + 1]->flower->type = 0;
                board->slots[i][j + 2]->flower->type = 0;
                board->emptySlotsColumns[i] += 3;
                board->points += 300;
                addTask(board, flowersType, 3);
                return 1;
            }
        }

    return 0;
}

int verifyHorizontalMatches(board_t *board)
{
    int flowersType = 0;

    // 4 blocks HORIZONTAL
    for (int i = 0; i < BOARD_SIZE_X - 3; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y; j++)
        {

            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i + 1][j]->flower->type &&
                flowersType == board->slots[i + 2][j]->flower->type &&
                flowersType == board->slots[i + 3][j]->flower->type &&
                flowersType != 0)
            {
                board->slots[i][j]->flower->type = 0;
                board->slots[i + 1][j]->flower->type = 0;
                board->slots[i + 2][j]->flower->type = 0;
                board->slots[i + 3][j]->flower->type = 0;
                board->emptySlotsColumns[i]++;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->emptySlotsColumns[i + 3]++;
                board->points += 450;
                addTask(board, flowersType, 4);
                return 1;
            }
        }
    // 3 blocks HORIZONTAL
    for (int i = 0; i < BOARD_SIZE_X - 2; i++)
        for (int j = BOARD_SIZE_Y - 8; j < BOARD_SIZE_Y; j++)
        {

            flowersType = board->slots[i][j]->flower->type;

            if (flowersType == board->slots[i + 1][j]->flower->type &&
                flowersType == board->slots[i + 2][j]->flower->type &&
                flowersType != 0)
            {
                board->slots[i][j]->flower->type = 0;
                board->slots[i + 1][j]->flower->type = 0;
                board->slots[i + 2][j]->flower->type = 0;
                board->emptySlotsColumns[i]++;
                board->emptySlotsColumns[i + 1]++;
                board->emptySlotsColumns[i + 2]++;
                board->points += 300;
                addTask(board, flowersType, 3);
                return 1;
            }
        }

    return 0;
}

int checkMatch(board_t *board, int reset)
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

void restartDraw(board_t *board)
{
    for (int i = 0; i < BOARD_SIZE_X; i++)
        for (int j = 0; j < BOARD_SIZE_Y; j++)
            board->slots[i][j]->flower->shown = 0;
}

void swapPositions(slot_t *slot, int direction)
{

    flower_t *flowerAux = slot->flower;

    // If the change is invalid, return
    if ((slot->left == NULL && direction == DIR_LEFT) || (slot->right == NULL && direction == DIR_RIGHT) ||
        (slot->up == NULL && direction == DIR_UP) || (slot->down == NULL && direction == DIR_DOWN))
        return;
    if (direction == DIR_LEFT)
    {
        slot->flower = slot->left->flower;
        slot->left->flower = flowerAux;
    }
    else if (direction == DIR_RIGHT)
    {
        slot->flower = slot->right->flower;
        slot->right->flower = flowerAux;
    }
    else if (direction == DIR_UP)
    {
        slot->flower = slot->up->flower;
        slot->up->flower = flowerAux;
    }
    else if (direction == DIR_DOWN)
    {
        slot->flower = slot->down->flower;
        slot->down->flower = flowerAux;
    }
}

void dragColumnDown(board_t *board, int column)
{

    int done = 0;
    slot_t *slotSupport = NULL;

    // While there are empty spaces between candies, drag the column down
    while (!done)
    {
        done = 1;
        slotSupport = board->slots[column][BOARD_SIZE_Y - 2];
        // Drag the column down by one unit
        while (slotSupport)
        {
            if (slotSupport->flower->type != FLOWER_NONE && slotSupport->down->flower->type == FLOWER_NONE)
            {
                swapPositions(slotSupport, DIR_DOWN);
                done = 0;
            }
            slotSupport = slotSupport->up;
        }
        board->slots[column][0]->flower->type = 1 + (rand() % 5);
    }
}

void putSpacesVague(board_t *board, int animation)
{
    if (animation)
        fallFlowers(board);
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

void restartGame(board_t *board)
{
    for (int j = 0; j < BOARD_SIZE_Y; j++)
        for (int i = 0; i < BOARD_SIZE_X; i++)
        {
            board->slots[i][j]->flower->type = 1 + (rand() % 5);
            board->slots[i][j]->xDisplayPos = i * DISTANCE + OFFSET_X;
            board->slots[i][j]->yDisplayPos = (j) * DISTANCE + OFFSET_Y;
        }

    srand(time(NULL));

    while (checkMatch(board, 1))
        putSpacesVague(board, 0);
    board->points = 0;
    board->tasks->flowersDestroy = 0;
    board->tasks->flowersType = 1 + (rand() % 5);
    board->level = 0;
}

// Verify if a movement is valid
int checkSwap(slot_t *slot, int direction)
{
    if ((direction == DIR_DOWN && slot->yBoardPos == 15) ||
        (direction == DIR_UP && slot->yBoardPos == 8) ||
        (direction == DIR_RIGHT && slot->xBoardPos == 7) ||
        (direction == DIR_LEFT && slot->xBoardPos == 0))
        return 0;
    return 1;
}
