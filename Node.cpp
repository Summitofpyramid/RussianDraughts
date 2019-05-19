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
    string moveNotation;
    board*  b;
    unordered_map<string, board*> children;
    node(){
        moveNotation = "";
        b = NULL;
    }
    node(string s, board* _b){
        moveNotation = s;
        b = _b;
    }
};


#endif
