#include "Room.h"

#include <fstream>
#include <utility>

Room::Room(int roomId) : id(roomId) {


}

bool Room::LoadLevel(const std::string& filePath) {

  std::ifstream file(filePath);

  if (!file.is_open()) {
    return false;
  }

  // Read into temporary containers first.
  std::vector<std::string> loadedTiles;
  std::vector<Rectangle> loadedWalls;
  std::vector<Door> loadedDoors;

  std::string line;

  while (std::getline(file, line))
    {
        // Handle text files saved with Windows line endings.
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        loadedTiles.push_back(line);
    }

    // Reject an empty file or a failed read.
    if (file.bad() || loadedTiles.empty() || loadedTiles.front().empty())
    {
        return false;
    }

    const std::size_t columns = loadedTiles.front().size();

    for (std::size_t row = 0; row < loadedTiles.size(); ++row)
    {
        // Every row must have the same number of tiles.
        if (loadedTiles[row].size() != columns)
        {
            return false;
        }

        for (std::size_t column = 0; column < columns; ++column)
        {
            const char tile = loadedTiles[row][column];

            if (tile == '#') //hashtag for walls
            {
                loadedWalls.push_back(Rectangle{
                    static_cast<float>(column) * tileSize,
                    static_cast<float>(row) * tileSize,
                    static_cast<float>(tileSize),
                    static_cast<float>(tileSize)
                });
            }
            else if (tile >= '0' && tile <= '9') {

                loadedDoors.push_back(Door{
                  Rectangle{
                      static_cast<float>(column) * tileSize,
                      static_cast<float>(row) * tileSize,
                      static_cast<float>(tileSize),
                      static_cast<float>(tileSize)
                  },
                  tile - '0',
                  Vector2{80.0f, 80.0f}
                });
            }
        }
    }

    // Replace the room only after loading succeeds.
    tiles = std::move(loadedTiles);
    walls = std::move(loadedWalls);
    doors = std::move(loadedDoors);

    return true;

}

void Room::Draw() const {

  for(const Rectangle& wall : walls) {

    DrawRectangleRec(wall, RED);
  }

  for(const Door& door : doors) {

    DrawRectangleRec(door.bounds, GOLD);
  }

}


const std::vector<Rectangle>& Room::GetWalls() const
{
    return walls;
}

const std::vector<Door>& Room::GetDoors() const {

  return doors;
}