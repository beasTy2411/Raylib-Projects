#ifndef HIGHSCORE_H
#define HIGHSCORE_H

int LoadHighScore(const char *filename);
void SaveHighScore(const char *filename, int score);

#endif