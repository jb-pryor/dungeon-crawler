#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct Door {

  Rectangle bounds;
  int destinationPoint;
  Vector2 destinationSpawn;

};

class Room { //room probably needs index number for different rooms + a 2d vector map representing the 
  public:
    Room(int roomId); //room and then potentially some sort of 2d array to store the gamemap
    
    bool LoadLevel(const std::string& filePath);
    void Draw() const;

    const std::vector<Rectangle>& GetWalls() const;
    const std::vector<Door>& GetDoors() const;
  
  private:
    int id;
    static constexpr int tileSize = 40;

    std::vector<std::string> tiles;
    std::vector<Rectangle> walls;
    std::vector<Door> doors;
};