#ifndef EVENTS_H
#define EVENTS_H

#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "utils.h"
#include "completion.h"

void handleMouseScroll(int x, int y);
void moveCursorUp();
void moveCursorDown();
void moveCursorLeft();
void moveCursorRight();
void leftDeleteChar();
void insertChar(char c);
void insertString(const char* s);
void replaceWord(char* s);
void createNewline();

#endif