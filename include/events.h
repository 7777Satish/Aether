#ifndef EVENTS_H
#define EVENTS_H

#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "utils.h"

void handleMouseScroll(int x, int y);
void leftDeleteChar();
void insertChar(char c);
void createNewline();

#endif