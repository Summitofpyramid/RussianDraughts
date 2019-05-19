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
#include "Piece.cpp"
#include "Node.cpp"
#include <unordered_map>

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
        cout<<"D0: dark normal pieces, D1: dark queen, L0: light normal pieces, L1: light queen pieces"<<endl;
        cout<<"the index of the top-left of the board is [0,0], and increases from left to right and top to bottom!"<<endl;
        cout<<"   ";
        for(int c=0;c<8;++c)
            cout<<c<<"  ";
        cout<<endl;
        for(int r=0; r<8;++r){
            cout<<r<<"  ";
            for(int c=0;c<8;++c){
                if(B->dark_normal.count(r*8+c))
                    cout<<"D0";
                else if(B->light_normal.count(r*8+c))
                    cout<<"L0";
                else if(B->dark_king.count(r*8+c))
                    cout<<"D1";
                else if(B->light_king.count(r*8+c))
                    cout<<"L1";
                else
                    cout<<"##";
                cout<<" ";
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
            piece* chosen = selectAPiecetoMove();
            // in case the player has selected wrong pieces
            while(!chosen){
                chosen = selectAPiecetoMove();
            }
            move(chosen);
            isLightTurnToMove = !isLightTurnToMove;
            gameStatusCheck();
            printPositions();
        }
    }
    
    piece* selectAPiecetoMove(){
        int x, y;
        cout<<"Please select a piece to move!"<<endl;
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
        
        int index = x*8+y;
        if(isLightTurnToMove){
            if(B->light_normal.count(index))
                return B->light_normal[index];
            else if(B->light_king.count(index))
                return B->light_king[index];
            else{
                cout<<"The chosen piece is not a valid light piece!"<<endl;
                return NULL;
            }
        }else{
            if(B->dark_normal.count(index))
                return B->dark_normal[index];
            else if(B->dark_king.count(index))
                return B->dark_king[index];
            else{
                cout<<"The chosen piece is not a valid dark piece!"<<endl;
                return NULL;
            }
        }
    }
    

    bool insideTheBoard(int x, int y){
        return x>=0&&x<8&&y>=0&&y<8;
    }
    
    bool isEmpty(int x, int y){
        int cur_index = 8*x+y;
        return !B->light_normal.count(cur_index)&&
        !B->light_king.count(cur_index)&&
        !B->dark_normal.count(cur_index)&&
        !B->dark_king.count(cur_index);
        
    }
    
    bool belongToDifferentPlayer(piece* cur, int nextX, int nextY){
        bool isLight = cur->getIsBelongingToLight();
        return (isLight&&(B->dark_normal.count(8*nextX+nextY)||B->dark_king.count(8*nextX+nextY)))||
                (!isLight&&(B->light_normal.count(8*nextX+nextY)||B->light_king.count(8*nextX+nextY)));
    }
    
    
    bool canAKingCapture(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];

            while(insideTheBoard(next_x, next_y)&&isEmpty(next_x, next_y)){
                next_x = cur->getX()+dir[0];
                next_y = cur->getY()+dir[1];
            }
            int next_of_next_x = cur->getX()+2*dir[0];
            int next_of_next_y = cur->getY()+2*dir[1];
            
            if(belongToDifferentPlayer(cur, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAKingShift(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];
            int next_of_next_x = cur->getX()+2*dir[0];
            int next_of_next_y = cur->getY()+2*dir[1];
            if(belongToDifferentPlayer(cur, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAKingMove(piece* cur){
        return canAKingShift(cur)||canAKingCapture(cur);
    }
    
    bool canAManCapture(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];
            int next_of_next_x = cur->getX()+2*dir[0];
            int next_of_next_y = cur->getY()+2*dir[1];
            if(belongToDifferentPlayer(cur, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAManShift(piece* cur){
        for(auto dir:dirs){
            int next_x = cur->getX()+dir[0];
            int next_y = cur->getY()+dir[1];
            if(insideTheBoard(next_x, next_y)&&isEmpty(next_x, next_y))
                return true;
        }
        return false;
    }
    
    bool canANormalPieceMove(piece* cur){
        return canAManShift(cur)||canAManCapture(cur);
    }
    
    int inputDirectionIndex(piece* current_piece){
        int dir_index;
        if(current_piece->getNormal()){
            if(current_piece->getIsBelongingToLight()){
                do{
                    cin>>dir_index;
                    if(dir_index==2||dir_index==3)
                        cout<<"The direction index of light men piece can be only 0 or 1"<<endl;
                }while(dir_index==2||dir_index==3);
            }else{
                do{
                    cin>>dir_index;
                    if(dir_index==0||dir_index==1)
                        cout<<"The direction index of dark men piece can be only 2 or 3"<<endl;
                }while(dir_index==0||dir_index==1);
            }
        }else{
            cout<<"Please input the direction index of this King!";
            cin>>dir_index;
        }
        return dir_index;
    }
    
    void manCapture(piece* &cur, int x, int y){
        int original_index = 8*cur->getX()+cur->getY();
        int next_index = 8*(cur->getX()+x)+cur->getY()+y;
        cur->setX(cur->getX()+2*x);
        cur->setY(cur->getY()+2*y);
        int next_next_index = 8*cur->getX()+cur->getY();
        if((cur->getIsBelongingToLight()&&cur->getX()==0)||
           (!cur->getIsBelongingToLight()&&cur->getX()==7))
            cur->changeToKing();

        if(cur->getIsBelongingToLight()){
            if(B->dark_normal.count(next_index))
                B->dark_normal.erase(next_index);
            else
                B->dark_king.erase(next_index);
            B->light_normal.erase(original_index);
            if(cur->getNormal())
                B->light_normal[next_next_index] = cur;
            else
                B->light_king[next_next_index] = cur;
        }else{
            if(B->light_normal.count(next_index))
                B->light_normal.erase(next_index);
            else
                B->light_king.erase(next_index);
            B->dark_normal.erase(original_index);
            if(cur->getNormal())
                B->dark_normal[next_next_index] = cur;
            else
                B->dark_king[next_next_index] = cur;
        }
    }
    
    void kingCapture(piece* cur, int x, int y){
        int original_index = 8*cur->getX()+cur->getY();
        int capture_x = cur->getX(), capture_y=cur->getY();
        while(insideTheBoard(capture_x+x, capture_y+y)&&isEmpty(capture_x+x, capture_y+y)){
            capture_x+=x;
            capture_y+=y;
        }
        // see if the one after
        if(!insideTheBoard(capture_x+x, capture_y+y)||!isEmpty(capture_x+x, capture_y+y))
            return;
        int next_index = 8*capture_x+capture_y;
        cur->setX(capture_x+x);
        cur->setY(capture_y+y);
        int index_next_next = 8*cur->getX()+cur->getY();
        if(cur->getIsBelongingToLight()){
            if(B->dark_normal.count(next_index))
                B->dark_normal.erase(next_index);
            else
                B->dark_king.erase(next_index);
            B->light_king.erase(original_index);
            B->light_king[index_next_next]=cur;
        }else{
            if(B->light_normal.count(next_index))
                B->light_normal.erase(next_index);
            else
                B->light_king.erase(next_index);
            B->dark_king.erase(original_index);
            B->dark_king[index_next_next]=cur;
        }
    }
    
    void manOrKingShift(piece* &current_piece, int new_x, int new_y){
        int index = 8*current_piece->getX()+current_piece->getY();
        current_piece->setX(new_x);
        current_piece->setY(new_y);
        int new_index = 8*current_piece->getX()+current_piece->getY();
        if(current_piece->getIsBelongingToLight()){
            B->light_normal.erase(index);
            B->light_normal[new_index] = current_piece;
        }else{
            B->dark_normal.erase(index);
            B->dark_normal[new_index] = current_piece;
        }
        
        if((current_piece->getIsBelongingToLight()&&current_piece->getX()==0)||
           (!current_piece->getIsBelongingToLight()&&current_piece->getX()==7))
            current_piece->changeToKing();
    }

    void move(piece* &current_piece){
        int dir_index = -1;
        cout<<"---------Please input the index of direction--------"<<endl;
        cout<<"0: left-up, 1: right-up, 2: left-down, 3: right-down"<<endl;
        if(current_piece->getNormal()){
            if(canAManCapture(current_piece)){
                while(canAManCapture(current_piece)){
                    dir_index = inputDirectionIndex(current_piece);
                    manCapture(current_piece, dirs[dir_index][0], dirs[dir_index][1]);
                }
            }else if(canAManShift(current_piece)){
                dir_index = inputDirectionIndex(current_piece);
                int new_x = current_piece->getX()+dirs[dir_index][0];
                int new_y = current_piece->getY()+dirs[dir_index][1];
                manOrKingShift(current_piece, new_x, new_y);
            }
        }else{
            if(canAKingCapture(current_piece)){
                while((canAKingCapture(current_piece))){
                    dir_index = inputDirectionIndex(current_piece);
                    kingCapture(current_piece, dirs[dir_index][0], dirs[dir_index][1]);
                }
            }
            else if(canAKingShift(current_piece)){
                int next_x, next_y;
                do{
                    dir_index = inputDirectionIndex(current_piece);
                    cout<<"Please input the moving distance along this direction!"<<endl;
                    int dis;
                    cin>>dis;
                    next_x = current_piece->getX() + dis*(dirs[dir_index][0]);
                    next_y = current_piece->getY() + dis*(dirs[dir_index][1]);
                }while(!insideTheBoard(next_x, next_y)||!isEmpty(next_x, next_y));
                manOrKingShift(current_piece, next_x, next_y);
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
        for(auto pair:B->light_normal){
            if(canANormalPieceMove(pair.second)){
                light_nor = true;
                break;
            }
            
        }
        
        for(auto pair:B->light_king){
            if(canAKingMove(pair.second)){
                light_que = true;
                break;
            }
        }
        
        if(!(light_nor||light_que)){
            endGame = true;
            return;
        }
        // check if all dark piece has no place to move
        for(auto pair:B->dark_normal){
            if(canANormalPieceMove(pair.second)){
                dark_nor = true;
                break;
            }
        }

        for(auto pair:B->dark_king){
            if(canAKingMove(pair.second)){
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
