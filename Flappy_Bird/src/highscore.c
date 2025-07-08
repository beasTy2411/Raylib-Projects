#include "highscore.h"
#include <stdio.h>

int LoadHighScore(const char *filename)
{
    int score = 0;
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        fscanf(file, "%d", &score);
        fclose(file);
    }
    return score;
}
void SaveHighScore(const char *filename, int score)
{
    FILE *file = fopen(filename, "w");
    if (file != NULL)
    {
        fprintf(file, "%d", score);
        fclose(file);
    }
}