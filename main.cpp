//
//  main.cpp
//  RussianDraughts
//
//  Created by Johnson Johnson on 2019-05-16.
//  Copyright © 2019 Johnson Johnson. All rights reserved.
//

#include <iostream>
#include "Game.cpp"

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    game *G = new game();
    G->startAuto();
    delete G;
    return 0;
}




