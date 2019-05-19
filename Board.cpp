//
//  Board.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-16.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//
#ifndef _BOARD_H
#define _BOARD_H


#include <stdio.h>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <string>

using namespace std;

#define SPACE "  "

class board{
public:
    vector<vector<string>> positions=vector<vector<string>>(8, vector<string>(8, SPACE));
    unordered_set<int> dark_normal, light_normal;
    unordered_set<int> dark_king, light_king;
    // in this version the board will be represented as a vector of vector
    // of string, where DM: dark man, DK: dark King, LM: light man, LK: light king
    
    board(){
        for(int i=0;i<8;++i){
            for(int j=0;j<8;++j){
                if(((i&1)==0&&(j&1)==1)||
                   ((i&1)==1&&(j&1)==0)){
                    if(i<3){
                        positions[i][j] = "DM";
                        dark_normal.insert(8*i+j);
                    }
                    else if(i>4){
                        positions[i][j] = "LM";
                        light_normal.insert(8*i+j);
                    }
                }
            }
        }
    }

};

#endif
