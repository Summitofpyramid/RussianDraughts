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
    board B;
    bool isLightTurnToMove;
    bool endGame;
    vector<vector<int>> dirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
    const vector<string>  names= {"Dark", "Light"};

public:
    game(){
        isLightTurnToMove = true;
        endGame = false;
    }
    
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
                cout<<B.positions[r][c]<<" ";
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
            movePiece(B,chosen);
            isLightTurnToMove = !isLightTurnToMove;
            gameStatusCheck(B);
            printPositions();
        }
    }
    
    void startAuto(){
        cout<<"Initial state of the board: "<<endl;
        printPositions();
        int totalMoves = 3;
        node root = node();
        findAllPossibleMove(totalMoves, root, isLightTurnToMove);
        int dep = 0;
        getTreeDepth(root,0,dep);

        cout<<"The tree depth is: "<<dep<<endl;
    }
    
    void getTreeDepth(node root, int cur, int &depth){
        depth = max(depth, cur);
 
        for(auto n:root.children){
            cout<<n.first<<endl;
            getTreeDepth(n.second, cur+1, depth);
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
            if(B.positions[x][y]!="LM" || B.positions[x][y]=="LK"){
                cout<<"The chosen piece is not a valid light piece!"<<endl;
                return {};
            }
            
        }else{
            if(B.positions[x][y]!="DM" || B.positions[x][y]=="DK"){
                cout<<"The chosen piece is not a valid dark piece!"<<endl;
                return {};
            }
        }
        return {x,y};
    }

    bool insideTheBoard(int x, int y){
        return x>=0&&x<8&&y>=0&&y<8;
    }
    
    bool isEmpty(board S, int x, int y){
        return S.positions[x][y]==SPACE;
        
    }
    
    bool belongToDifferentPlayer(board S,int x, int y, int nextX, int nextY){
        return insideTheBoard(nextX, nextY)&&
               S.positions[nextX][nextY]!=SPACE &&
                S.positions[x][y][0]!=S.positions[nextX][nextY][0];
    }
    
    
    bool canAKingCapture(board S, int x, int y){
        for(auto dir:dirs){
            int next_x = x;
            int next_y = y;

            while(insideTheBoard(next_x+dir[0], next_y+dir[1])&&isEmpty(S,next_x+dir[0], next_y+dir[1])){
                next_x +=dir[0];
                next_y +=dir[1];
            }
            int next_of_next_x = next_x+2*dir[0];
            int next_of_next_y = next_y+2*dir[1];
            
            if(belongToDifferentPlayer(S,x,y,next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(S, next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAKingMove(board S,int x, int y){
        return canAPieceShift(S, x, y)||canAKingCapture(S, x, y);
    }
    
    bool canAManCapture(board S, int x, int y){
        for(int j = 0;j<4;++j){
            if((j==2||j==3)&&(S.positions[x][y]=="LM"))
                continue;
            if((j==0||j==1)&&(S.positions[x][y]=="DM"))
                continue;
            int next_x = x+dirs[j][0];
            int next_y = y+dirs[j][1];
            int next_of_next_x = x+2*dirs[j][0];
            int next_of_next_y = y+2*dirs[j][1];
            if(belongToDifferentPlayer(S,x, y, next_x, next_y)&&
               insideTheBoard(next_of_next_x,next_of_next_y)&&
               isEmpty(S,next_of_next_x, next_of_next_y))
                return true;
        }
        return false;
    }
    
    bool canAPieceShift(board S, int x, int y){
        for(int j = 0;j<4;++j){
            if((j==2||j==3)&&(S.positions[x][y]=="LM"))
                continue;
            if((j==0||j==1)&&(S.positions[x][y]=="DM"))
                continue;
            int next_x = x+dirs[j][0];
            int next_y = y+dirs[j][1];
            if(insideTheBoard(next_x, next_y)&&isEmpty(S,next_x, next_y))
                return true;
        }
        return false;
    }
    
    bool canANormalPieceMove(board S,int x, int y){
        return canAPieceShift(S, x, y)||canAManCapture(S, x, y);
    }
    
    int inputDirectionIndex(int x, int y){
        int dir_index=-1;
        if(B.positions[x][y][1]=='M'){
            if(B.positions[x][y][0]=='L'){
                do{
                    cin>>dir_index;
                    if(dir_index==2||dir_index==3)
                        cout<<"The direction index of light men piece can be only 0 or 1"<<endl;
                }while(dir_index==2||dir_index==3);
            }else if(B.positions[x][y][0]=='D'){
                do{
                    cin>>dir_index;
                    if(dir_index==0||dir_index==1)
                        cout<<"The direction index of dark men piece can be only 2 or 3"<<endl;
                }while(dir_index==0||dir_index==1);
            }
        }else if(B.positions[x][y][1]=='K'){
            cout<<"Please input the direction index of this King!";
            cin>>dir_index;
        }
        return dir_index;
    }
    
    board manOrKingShift(board S, int x, int y, int new_x, int new_y){
        int index = 8*x+y;
        int new_index = 8*new_x+new_y;
        if(S.positions[x][y][1]=='M'){
            if(S.positions[x][y][0]=='L'){
                S.light_normal.erase(index);
                if(new_x==0){
                    S.light_king.insert(new_index);
                }else{
                    S.light_normal.insert(new_index);
                }
            }else if(S.positions[x][y][0]=='D'){
                S.dark_normal.erase(index);
                if(new_x==7){
                    S.dark_king.insert(new_index);
                }else{
                    S.dark_normal.insert(new_index);
                }
            }
        }else if(S.positions[x][y][1]=='K'){
            if(S.positions[x][y][0]=='L'){
                S.light_normal.erase(index);
                S.light_normal.insert(new_index);
            }else if(B.positions[x][y][0]=='D'){
                S.dark_normal.erase(index);
                S.dark_normal.insert(new_index);
            }
        }
        S.positions[new_x][new_y] = B.positions[x][y];
        S.positions[x][y] = SPACE;
        return S;
    }
    
    board manCapture(board S,int x0, int y0, int x, int y){
        int original_index = 8*x0+y0;
        int next_index = 8*(x0+x)+y0+y;
        int next_next_index = 8*(x0+2*x)+y0+2*y;
        string tmp = S.positions[x0][y0];

        if(S.positions[x0][y0][0]=='L'){
            S.light_normal.erase(original_index);
            if(S.dark_normal.count(next_index))
                S.dark_normal.erase(next_index);
            else
                S.dark_king.erase(next_index);
            if(x0+2*x== 0){
                S.positions[x0+2*x][y0+2*x] = "LK";
                S.light_king.insert(next_next_index);
            }else{
                S.positions[x0+2*x][y0+2*x] = tmp;
                S.light_normal.insert(next_next_index);
            }
            
        }else{
            S.dark_normal.erase(original_index);
            if(S.light_normal.count(next_index))
                S.light_normal.erase(next_index);
            else
                S.light_king.erase(next_index);
            
            if(x0+2*x== 7){
                S.positions[x0+2*x][y0+2*x] = "DK";
                S.dark_king.insert(next_next_index);
            }else{
                S.positions[x0+2*x][y0+2*x] = tmp;
                S.dark_normal.insert(next_next_index);
            }
        }
        S.positions[x0][y0] = SPACE;
        S.positions[x0+x][y0+x] = tmp;
        return S;
    }
    
    board kingCapture(board S, int x0, int y0, int x, int y){
        int original_index = 8*x0+y0;
        int capture_x = x0, capture_y=y0;
        while(insideTheBoard(capture_x+x, capture_y+y)&&isEmpty(S,capture_x+x, capture_y+y)){
            capture_x+=x;
            capture_y+=y;
        }
        // see if the one after
        if(!insideTheBoard(capture_x+x, capture_y+y)||!isEmpty(S,capture_x+x, capture_y+y))
            return S;
        int next_index = 8*capture_x+capture_y;
        S.positions[capture_x][capture_y]=SPACE;
        int index_next_next = 8*(capture_x+x)+capture_y+y;
        if(S.positions[x0][y0][0]=='L'){
            if(S.dark_normal.count(next_index))
                S.dark_normal.erase(next_index);
            else
                S.dark_king.erase(next_index);
            S.light_king.erase(original_index);
            S.light_king.insert(index_next_next);
        }else{
            if(S.light_normal.count(next_index))
                S.light_normal.erase(next_index);
            else
                S.light_king.erase(next_index);
            S.dark_king.erase(original_index);
            S.dark_king.insert(index_next_next);
        }
        return S;
    }
    
    void movePiece(board &S, vector<int> p){
        int dir_index = -1;
        cout<<"---------Please input the index of direction--------"<<endl;
        cout<<"0: left-up, 1: right-up, 2: left-down, 3: right-down"<<endl;
        if(S.positions[p[0]][p[1]][1]=='M'){
            if(canAManCapture(S,p[0], p[1])){
                while(canAManCapture(S,p[0], p[1])){
                    dir_index = inputDirectionIndex(p[0], p[1]);
                    S = manCapture(S,p[0], p[1], dirs[dir_index][0], dirs[dir_index][1]);
                }
            }else if(canAPieceShift(S,p[0], p[1])){
                dir_index = inputDirectionIndex(p[0], p[1]);
                int new_x = p[0]+dirs[dir_index][0];
                int new_y = p[1]+dirs[dir_index][1];
                S = manOrKingShift(S,p[0], p[1], new_x, new_y);
            }
        }else{
            if(canAKingCapture(S,p[0], p[1])){
                while((canAKingCapture(S,p[0], p[1]))){
                    dir_index = inputDirectionIndex(p[0], p[1]);
                    S = kingCapture(S,p[0], p[1], dirs[dir_index][0], dirs[dir_index][1]);
                }
            }
            else if(canAPieceShift(S,p[0], p[1])){
                int next_x, next_y;
                do{
                    dir_index = inputDirectionIndex(p[0], p[1]);
                    cout<<"Please input the moving distance along this direction!"<<endl;
                    int dis;
                    cin>>dis;
                    next_x = p[0] + dis*(dirs[dir_index][0]);
                    next_y = p[1] + dis*(dirs[dir_index][1]);
                }while(!insideTheBoard(next_x, next_y)||!isEmpty(S,next_x, next_y));
                S = manOrKingShift(S,p[0], p[1], next_x, next_y);
            }
        }
    }
    
    void gameStatusCheck(board S){
        bool light_nor = false;
        bool light_que = false;
        bool dark_nor = false;
        bool dark_que = false;
        //check if light or dark has no pieces
        if (
            (B.light_normal.empty()&&B.light_king.empty())||
            (B.dark_normal.empty()&&B.dark_king.empty())
            ){
            endGame = true;
            return;
        }
        // check if all light piece has no place to move
        for(auto ind:B.light_normal){
            if(canANormalPieceMove(S,ind/8, ind%8)){
                light_nor = true;
                break;
            }
            
        }
        
        for(auto ind:B.light_king){
            if(canAKingMove(S,ind/8, ind%8)){
                light_que = true;
                break;
            }
        }
        
        if(!(light_nor||light_que)){
            endGame = true;
            return;
        }
        // check if all dark piece has no place to move
        for(auto ind:B.dark_normal){
            if(canANormalPieceMove(S,ind/8, ind%8)){
                dark_nor = true;
                break;
            }
        }
        
        for(auto ind:B.dark_king){
            if(canAKingMove(S,ind/8, ind%8)){
                dark_que = true;
                break;
            }
        }
        
        if(!(dark_nor||dark_que)){
            endGame = true;
            return;
        }

    }
    
    bool canAManMoveWithDir(board b, int x, int y, int dirx, int diry){
        int next_x = x+dirx;
        int next_y = y+diry;
        return insideTheBoard(next_x, next_y)&&
        b.positions[next_x][next_y]==SPACE;
    }
    
    bool canAManCaptureWithDir(board b, int x, int y, int dirx, int diry){
        int next_x = x+dirx;
        int next_y = y+diry;
        return insideTheBoard(next_x, next_y)&&
            belongToDifferentPlayer(b,x, y, next_x, next_y)&&
            insideTheBoard(next_x+dirx,next_y+diry)&&
            isEmpty(B, next_x+dirx, next_y+diry);
    }
    
    bool canAKingCaptureWithDir(board S, int x, int y, int dirx, int diry){
        int next_x = x;
        int next_y = y;
        
        while(insideTheBoard(next_x+dirx, next_y+diry)&&isEmpty(S,next_x+dirx, next_y+diry)){
            next_x +=dirx;
            next_y +=diry;
        }
        int next_of_next_x = next_x+dirx;
        int next_of_next_y = next_y+diry;
        
        return belongToDifferentPlayer(S,x,y,next_x, next_y)&&
           insideTheBoard(next_of_next_x,next_of_next_y)&&
           isEmpty(S, next_of_next_x, next_of_next_y);
    }
    
    bool canAKingShiftWithDir(board S, int distance, int r, int c, int dirx, int diry){
        int next_x = r+dirx, next_y = c+diry;
        while(isEmpty(S, next_x, next_y)&&insideTheBoard(next_x, next_y)){
            next_x+=dirx;
            next_y+=diry;
        }
        return next_x == r+distance*dirx;
    }
    
    void shiftingHelper(node &root,string path, board S, int r, int c, int dirx, int diry){
        if(!insideTheBoard(r, c)||!insideTheBoard(r+dirx, c+diry))
            return;
        if(S.positions[r][c][1]=='M'){
            
            if(!canAPieceShift(S, r, c)){
                root.children[path] = node(S);
                return;
            }
            string added = "-"+to_string(r+dirx)+to_string(c+diry);
            root.children[path] = manOrKingShift(S, r, c, r+dirx, c+diry);
            
        }else if(S.positions[r][c][1]=='K'){
            if(!canAPieceShift(S, r, c)){
                root.children[path] = node(S);
                return;
            }
            for(auto dir:dirs){
                int next_x = r, next_y = c;
                
                while(isEmpty(S, next_x+dirx, next_y+diry)&&insideTheBoard(next_x+dirx, next_y+diry)){
                    next_x+=dirx;
                    next_y+=dirx;
                    string added="-"+to_string(next_x)+to_string(next_y);
                    root.children[added] = node(S);
                }
            }
        }
    }
    
    void captureHelper(node &root,string path, board S, int r, int c, int dirx, int diry){
        if(S.positions[r][c][1]=='M'){
            if(!canAManCaptureWithDir(S,r,c,dirx,diry)){
                root.children[path] = node(S);
                return;
            }
            string added = ":"+to_string(r+2*dirx)+to_string(c+2*diry);
            for(int j=0;j<4;++j){
                if((j==2||j==3)&&(S.positions[r][c]=="LM"))
                    continue;
                if((j==0||j==1)&&(S.positions[r][c]=="DM"))
                    continue;
                captureHelper(root,path+added,S,r+2*dirx, c+2*diry,dirs[j][0],dirs[j][1]);
            }
        }
        else if(S.positions[r][c][1]=='K'){
            if(!canAKingCaptureWithDir(S,r,c,dirx,diry)){
                root.children[path] = node(S);
                return;
            }
            int next_x = r;
            int next_y = c;
            
            while(insideTheBoard(next_x+dirx, next_y+diry)&&isEmpty(S,next_x+dirx, next_y+diry)){
                next_x +=dirx;
                next_y +=diry;
            }
            int next_of_next_x = next_x+dirx;
            int next_of_next_y = next_y+diry;
            string added =":"+to_string(next_of_next_x)+to_string(next_of_next_y);
            cout<<added<<endl;
            for(auto dir:dirs){
                captureHelper(root,path+added,S,r+2*dirx, c+2*diry,dir[0],dir[1]);
            }
        }
        
    }
    
    void findAllPossibleMove(int k, node &root, bool &isLightTurnToMove){
        board curBoard = root.b;
        string prefix = "";
        if(k==0)
            return;
        gameStatusCheck(root.b);
        if(endGame){
            return;
        }
        
        if(isLightTurnToMove){
            // light player's turn to move
            bool pieceMoved = false;
            
            for(auto ind:curBoard.light_normal){
                
                int row = ind/8, col = ind%8;
                if(!canANormalPieceMove(curBoard,row, col))
                    continue;
                
                prefix=to_string(row)+to_string(col);
                
                if(canAManCapture(curBoard,row, col)){
                    pieceMoved = true;
                    for(auto dir:dirs){
                        captureHelper(root,prefix,curBoard,row,col,dir[0],dir[1]);
                    }
                }
            }
            
            for(auto ind:curBoard.light_king){
                int row = ind/8, col = ind%8;
                if(!canAKingMove(curBoard, row, col))
                    continue;
                pieceMoved = true;
                prefix=to_string(row)+to_string(col);
                if(canAKingCapture(curBoard, row, col)){
                    pieceMoved = true;
                    for(auto dir:dirs){
                        captureHelper(root,prefix,curBoard,row,col,dir[0],dir[1]);
                    }
                }
                
            }
            
            if(!pieceMoved){
                for(auto ind:curBoard.light_normal){
                    int row = ind/8, col = ind%8;
                    if(!canAPieceShift(curBoard, row, col))
                        continue;
                    pieceMoved = true;
                    prefix=to_string(row)+to_string(col);
                    for(int j=0;j<4;++j){
                        if((j==2||j==3)&&(root.b.positions[row][col]=="LM"))
                            continue;
                        if((j==0||j==1)&&(root.b.positions[row][col]=="DM"))
                            continue;
                        shiftingHelper(root,prefix,root.b,row,col,dirs[j][0],dirs[j][1]);
                    }
                }
                
                
                for(auto ind:curBoard.light_king){
                    board tmp = curBoard;
                    int row = ind/8, col = ind%8;
                    if(!canAPieceShift(curBoard, row, col))
                        continue;
                    pieceMoved = true;
                    prefix=to_string(row)+to_string(col);
                    for(auto dir:dirs){
                        shiftingHelper(root,prefix,root.b,row,col,dir[0],dir[1]);
                    }
                }
                
                
            }
            
            
        }else{
            // dark player's turn to move
            bool pieceMoved = false;
            
            for(auto ind:curBoard.dark_normal){
                
                int row = ind/8, col = ind%8;
                if(!canANormalPieceMove(curBoard,row, col))
                    continue;
                
                prefix=to_string(row)+to_string(col);
                
                if(canAManCapture(curBoard,row, col)){
                    pieceMoved = true;
                    for(auto dir:dirs){
                        captureHelper(root,prefix,curBoard,row,col,dir[0],dir[1]);
                    }
                }
            }
            
            if(!pieceMoved){
                for(auto ind:curBoard.dark_king){
                    int row = ind/8, col = ind%8;
                    if(!canAKingMove(curBoard, row, col))
                        continue;

                    prefix=to_string(row)+to_string(col);
                    if(canAKingCapture(curBoard, row, col)){
                        pieceMoved = true;
                        for(auto dir:dirs){
                            captureHelper(root,prefix,curBoard,row,col,dir[0],dir[1]);
                        }
                    }
                    
                }
            }
            
            if(!pieceMoved){
                for(auto ind:curBoard.dark_normal){
                    int row = ind/8, col = ind%8;
                    if(!canAPieceShift(curBoard, row, col))
                        continue;
                    prefix+=to_string(row)+to_string(col);
                    for(int j=0;j<4;++j){
                        if((j==2||j==3)&&(root.b.positions[row][col]=="LM"))
                            continue;
                        if((j==0||j==1)&&(root.b.positions[row][col]=="DM"))
                            continue;
                        shiftingHelper(root,prefix,root.b,row,col,dirs[j][0],dirs[j][1]);
                    }
                }
                
                for(auto ind:curBoard.dark_king){
                    board tmp = curBoard;
                    int row = ind/8, col = ind%8;
                    if(!canAPieceShift(curBoard, row, col))
                        continue;
                    
                    for(auto dir:dirs){
                        shiftingHelper(root,prefix,root.b,row,col,dir[0],dir[1]);
                    }
                }
            }
            
        }
            
        isLightTurnToMove = !isLightTurnToMove;
        // depth first traverse children
        for(auto son: root.children){
            findAllPossibleMove(k-1, son.second, isLightTurnToMove);
        }
        
    }
};

#endif
