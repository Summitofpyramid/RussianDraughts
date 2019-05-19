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
#include <unordered_map>
#include "Piece.cpp"

using namespace std;

class board{
    vector<vector<int>> positions=vector<vector<int>>(8, vector<int>(8, 0));
public:
    unordered_map<int,piece*> dark_normal, light_normal;
    unordered_map<int,piece*> dark_king, light_king;
    board(){
        for(int i=0;i<8;++i){
            for(int j=0;j<8;++j){
                if(((i&1)==0&&(j&1)==1)||
                   ((i&1)==1&&(j&1)==0))
                    positions[i][j] = 1;
                
                if(i<3&&positions[i][j])
                    dark_normal[8*i+j] = new piece(i,j,true,false);
                else if(i>4&&positions[i][j])
                    light_normal[8*i+j] = new piece(i,j,true,true);
            }
        }
    }

};

#endif
