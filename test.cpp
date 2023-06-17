#include <iostream>
#include <random>
#include <vector>

using namespace std;

struct Coord {
    int x, y;
};

class Map {
    int width, height;
    vector<vector<int>> map;

public:
    Map(int w, int h) : width(w), height(h), map(w, vector<int>(h, 0)) {}

    void generateRandomWall() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distX(0, width - 1);
        uniform_int_distribution<int> distY(0, height - 1);
        uniform_int_distribution<int> distLength(1, 5); // 벽의 길이 범위

        // 랜덤한 시작 위치 선택
        int startX = distX(gen);
        int startY = distY(gen);

        // 랜덤한 방향 선택 (1: 위쪽, 2: 오른쪽, 3: 아래쪽, 4: 왼쪽)
        int direction = distX(gen) % 4 + 1;

        // 랜덤한 길이 선택
        int length = distLength(gen);

        // 선택된 방향으로 벽 생성
        int x = startX;
        int y = startY;
        for (int i = 0; i < length; i++) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                map[x][y] = 1;
            }

            switch (direction) {
                case 1: // 위쪽
                    y--;
                    break;
                case 2: // 오른쪽
                    x++;
                    break;
                case 3: // 아래쪽
                    y++;
                    break;
                case 4: // 왼쪽
                    x--;
                    break;
            }
        }
    }

    void printMap() {
        for (const auto& row : map) {
            for (int cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int width = 10;
    int height = 10;

    Map map(width, height);
    map.generateRandomWall();
    map.generateRandomWall();
    map.generateRandomWall();
    map.printMap();

    return 0;
}
