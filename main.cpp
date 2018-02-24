#include "Game.h"
int main(int argc, char **argv){
   Game game;
   while(!game.IsDone()){
     game.HandleInput();
     game.Update();
     game.Render();
   }
   return 0;
}
