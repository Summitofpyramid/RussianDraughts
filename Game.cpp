//
//  game.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-16.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//
#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include "Board.cpp"
#include "Node.cpp"

class game{
    board *B;
    bool isLightTurnToMove;
    bool endGame;
    vector<vector<int>> dirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
    const vector<string>  names= {"Dark", "Light"};

public:
    game(){
        isLightTurnToMove = true;
        endGame = false;
        B = new board();
    }
    ~game(){
        delete B;
    };
    
    void printPositions(){
        cout<<"DM: dark man pieces, DK: dark king, LM: light man pieces, L1: light king pieces"<<endl;
        cout<<"the index of the top-left of the board is [0,0], and increases from left to right\n";
        cout<<"and top to bottom!"<<endl;
        cout<<"   ";
        for(int c=0;c<8;++c)
            cout<<c<<"  ";
        cout<<endl;
        for(int r=0; r<8;++r){
            cout<<r<<"  ";
            for(int c=0;c<8;++c){
                cout<<B->positions[r][c]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    
    void start(){
        cout<<"Initial state of the board: "<<endl;
        printPositions();
        while(!endGame){
            int idx = isLightTurnToMove;
            cout<<"It is " + names[idx] + " player's turn to move."<<endl;
            vector<int> chosen = selectAPiecetoMove();
            // in case the player has selected wrong pieces
            while(chosen.empty()){
                chosen = selectAPiecetoMove();
            }
            movePiece(chosen);
            isLightTurnToMove = !isLightTurnToMove;
            gameStatusCheck();
            printPositions();
        }
    }
    
    vector<int> selectAPiecetoMove(){
        int x, y;
        do{
            cout<<"Please input the row of the piece to move!"<<endl;
            cin>>x;
            if(x<0||x>=8)
                cout<<"the row should be in the range [0, 7]";
        }while(x<0||x>=8);
        
        do{
            cout<<"Please input the col of the piece to move!"<<endl;
            cin>>y;
            if(y<0||y>=8)
                cout<<"the col should be in the range [0, 7]";
        }while(y<0||y>=8);
        
        if(isLightTurnToMove){
            if(B->positions[x][y]!="LM" || B->positions[x][y]=="LK"){
                cout<<"The chosen piece is not a valid light piece!"<<endl;
                return {};
            }
            
        }else{
            if(B->positions[x][y]!="DM" || B->positions[x][y]=="DK"){
                cout<<"The chosen piece is not a valid dark piece!"<<endl;
                return {};
            }
        }
        return {x,y};
    }

    

    bool insideTheBoard(int x, int y){
        return x>=0&&x<8&&y>=0&&y<8;
    }
    
    bool isEmpty(int x, int y){
        return B->positions[x][y]!="";
        
    }
    
    bool belongToDifferentPlayer(int x, int y, int nextX, int nextY){
        return B->positions[nextX][nextY]!=SPACE && B->positions[x][y][0]!=B->positions[nextX][nextY][0];
    }
    
    
    bool canAKingCapture(int x, int y){
        for(auto dir:dirs){
            int next_x = x;
            int next_y = y;

            while(insideTheBoard(next_x+dir[0], next_y+dir[1])&&isEmpty(next_x+dir[0], next_y+dir[1])){
                next_x +=dir[0];
                next_y +=dir[1];
            }
            int next_of_next_x = next_x+2*dir[0];
            int next_of_next_y = next_y+2*dir[1];
            
            if(belongToDifferentPlayer(x,y,next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAKingMove(int x, int y){
        return canAPieceShift(x, y)||canAKingCapture(x, y);
    }
    
    bool canAManCapture(int x, int y){
        for(auto dir:dirs){
            int next_x = x+dir[0];
            int next_y = y+dir[1];
            int next_of_next_x = x+2*dir[0];
            int next_of_next_y = y+2*dir[1];
            if(belongToDifferentPlayer(x, y, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAPieceShift(int x, int y){
        for(auto dir:dirs){
            int next_x = x+dir[0];
            int next_y = y+dir[1];
            if(insideTheBoard(next_x, next_y)&&isEmpty(next_x, next_y))
                return true;
        }
        return false;
    }
    
    bool canANormalPieceMove(int x, int y){
        return canAPieceShift(x, y)||canAManCapture(x, y);
    }
    
    int inputDirectionIndex(int x, int y){
        int dir_index=-1;
        if(B->positions[x][y][1]=='M'){
            if(B->positions[x][y][0]=='L'){
                do{
                    cin>>dir_index;
                    if(dir_index==2||dir_index==3)
                        cout<<"The direction index of light men piece can be only 0 or 1"<<endl;
                }while(dir_index==2||dir_index==3);
            }else if(B->positions[x][y][0]=='D'){
                do{
                    cin>>dir_index;
                    if(dir_index==0||dir_index==1)
                        cout<<"The direction index of dark men piece can be only 2 or 3"<<endl;
                }while(dir_index==0||dir_index==1);
            }
        }else if(B->positions[x][y][1]=='K'){
            cout<<"Please input the direction index of this King!";
            cin>>dir_index;
        }
        return dir_index;
    }
    
    void manOrKingShift(int x, int y, int new_x, int new_y){
        int index = 8*x+y;
        int new_index = 8*new_x+new_y;
        if(B->positions[x][y][1]=='M'){
            if(B->positions[x][y][0]=='L'){
                B->light_normal.erase(index);
                if(new_x==0){
                    B->light_king.insert(new_index);
                }else{
                    B->light_normal.insert(new_index);
                }
            }else if(B->positions[x][y][0]=='D'){
                B->dark_normal.erase(index);
                if(new_x==7){
                    B->dark_king.insert(new_index);
                }else{
                    B->dark_normal.insert(new_index);
                }
            }
        }else if(B->positions[x][y][1]=='K'){
            if(B->positions[x][y][0]=='L'){
                B->light_normal.erase(index);
                B->light_normal.insert(new_index);
            }else if(B->positions[x][y][0]=='D'){
                B->dark_normal.erase(index);
                B->dark_normal.insert(new_index);
            }
        }
        B->positions[new_x][new_y] = B->positions[x][y];
        B->positions[x][y] = SPACE;
        
    }
    
    void manCapture(int x0, int y0, int x, int y){
        int original_index = 8*x0+y0;
        int next_index = 8*(x0+x)+y0+y;
        int next_next_index = 8*(x0+2*x)+y0+2*y;
        string tmp = B->positions[x0][y0];

        if(B->positions[x0][y0][0]=='L'){
            B->light_normal.erase(original_index);
            if(B->dark_normal.count(next_index))
                B->dark_normal.erase(next_index);
            else
                B->dark_king.erase(next_index);
            if(x0+2*x== 0){
                B->positions[x0+2*x][y0+2*x] = "LK";
                B->light_king.insert(next_next_index);
            }else{
                B->positions[x0+2*x][y0+2*x] = tmp;
                B->light_normal.insert(next_next_index);
            }
            
        }else{
            B->dark_normal.erase(original_index);
            if(B->light_normal.count(next_index))
                B->light_normal.erase(next_index);
            else
                B->light_king.erase(next_index);
            
            if(x0+2*x== 7){
                B->positions[x0+2*x][y0+2*x] = "DK";
                B->dark_king.insert(next_next_index);
            }else{
                B->positions[x0+2*x][y0+2*x] = tmp;
                B->dark_normal.insert(next_next_index);
            }
        }
        B->positions[x0][y0] = SPACE;
        B->positions[x0+x][y0+x] = tmp;
        
    }
    
    void kingCapture(int x0, int y0, int x, int y){
        int original_index = 8*x0+y0;
        int capture_x = x0, capture_y=y0;
        while(insideTheBoard(capture_x+x, capture_y+y)&&isEmpty(capture_x+x, capture_y+y)){
            capture_x+=x;
            capture_y+=y;
        }
        // see if the one after
        if(!insideTheBoard(capture_x+x, capture_y+y)||!isEmpty(capture_x+x, capture_y+y))
            return;
        int next_index = 8*capture_x+capture_y;
        B->positions[capture_x][capture_y]=SPACE;
        int index_next_next = 8*(capture_x+x)+capture_y+y;
        if(B->positions[x0][y0][0]=='L'){
            if(B->dark_normal.count(next_index))
                B->dark_normal.erase(next_index);
            else
                B->dark_king.erase(next_index);
            B->light_king.erase(original_index);
            B->light_king.insert(index_next_next);
        }else{
            if(B->light_normal.count(next_index))
                B->light_normal.erase(next_index);
            else
                B->light_king.erase(next_index);
            B->dark_king.erase(original_index);
            B->dark_king.insert(index_next_next);
        }
    }
    


    void movePiece(vector<int> p){
        int dir_index = -1;
        cout<<"---------Please input the index of direction--------"<<endl;
        cout<<"0: left-up, 1: right-up, 2: left-down, 3: right-down"<<endl;
        if(B->positions[p[0]][p[1]][1]=='M'){
            if(canAManCapture(p[0], p[1])){
                while(canAManCapture(p[0], p[1])){
                    dir_index = inputDirectionIndex(p[0], p[1]);
                    manCapture(p[0], p[1], dirs[dir_index][0], dirs[dir_index][1]);
                }
            }else if(canAPieceShift(p[0], p[1])){
                dir_index = inputDirectionIndex(p[0], p[1]);
                int new_x = p[0]+dirs[dir_index][0];
                int new_y = p[1]+dirs[dir_index][1];
                manOrKingShift(p[0], p[1], new_x, new_y);
            }
        }else{
            if(canAKingCapture(p[0], p[1])){
                while((canAKingCapture(p[0], p[1]))){
                    dir_index = inputDirectionIndex(p[0], p[1]);
                    kingCapture(p[0], p[1], dirs[dir_index][0], dirs[dir_index][1]);
                }
            }
            else if(canAPieceShift(p[0], p[1])){
                int next_x, next_y;
                do{
                    dir_index = inputDirectionIndex(p[0], p[1]);
                    cout<<"Please input the moving distance along this direction!"<<endl;
                    int dis;
                    cin>>dis;
                    next_x = p[0] + dis*(dirs[dir_index][0]);
                    next_y = p[1] + dis*(dirs[dir_index][1]);
                }while(!insideTheBoard(next_x, next_y)||!isEmpty(next_x, next_y));
                manOrKingShift(p[0], p[1], next_x, next_y);
            }
        }
    }
    
    void gameStatusCheck(){
        bool light_nor = false;
        bool light_que = false;
        bool dark_nor = false;
        bool dark_que = false;
        //check if light or dark has no pieces
        if (
            (B->light_normal.empty()&&B->light_king.empty())||
            (B->dark_normal.empty()&&B->dark_king.empty())
            ){
            endGame = true;
            return;
        }
        // check if all light piece has no place to move
        for(auto ind:B->light_normal){
            if(canANormalPieceMove(ind/8, ind%8)){
                light_nor = true;
                break;
            }
            
        }
        
        for(auto ind:B->light_king){
            if(canAKingMove(ind/8, ind%8)){
                light_que = true;
                break;
            }
        }
        
        if(!(light_nor||light_que)){
            endGame = true;
            return;
        }
        // check if all dark piece has no place to move
        for(auto ind:B->dark_normal){
            if(canANormalPieceMove(ind/8, ind%8)){
                dark_nor = true;
                break;
            }
        }
        
        for(auto ind:B->dark_king){
            if(canAKingMove(ind/8, ind%8)){
                dark_que = true;
                break;
            }
        }
        
        if(!(dark_nor||dark_que)){
            endGame = true;
            return;
        }

    }
    
    
//     findAllPossibleMove(int k, board* b, bool isLightTurnToMove){
//        while
//    }

};

#endif
