//
//  Node.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-19.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//

#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h>
#include "Board.cpp"
#include <unordered_map>

class node{
public:
    board  b;
    bool hasWinner;
    bool tie;
    unordered_map<string, node> children;
    node(){
        hasWinner = false;
        tie = false;
    }
    node(board _b){
        b = _b;
        hasWinner = false;
        tie = false;
    }
};


#endif
