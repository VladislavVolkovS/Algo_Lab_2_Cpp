#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>

//printing of map
void PrintMap(const std::vector<std::vector<int>>& Map) {
    for (auto& i : Map) {
        for (auto& j : i) {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }
}

//compressing of coordinates(method with unordered map)
void Compress(std::vector<int>& x, std::vector<int>& y, std::unordered_map<int, int>& mx, std::unordered_map<int, int>& my) {
    std::sort(x.begin(), x.end());
    std::sort(y.begin(), y.end());
    for (auto& ix : x) {
        if (!mx.count(ix)) {
            mx[ix] = (int)mx.size();
        }
    }
    for (auto& iy : y) {
        if (!my.count(iy)) {
            my[iy] = (int)my.size();
        }
    }
}

//binary search of element <= target (left binary search)
int binary_search(const int& target, const std::vector<int>& mas) {
    int left = -1, right = (int)mas.size();
    while (right > left + 1) {
        int middle = (left + right) / 2;
        if (mas[middle] <= target) {
            left = middle;
        }
        else if (mas[middle] > target) {
            right = middle;
        }
    }
    return mas[left];
}

//function which return target point(pair <x, y>)
std::pair<int, int> FindPosition(const int& x_target, const int& y_target, const std::vector<int>& x, const std::vector<int>& y, std::unordered_map<int, int>& mx, std::unordered_map<int, int>& my) {
    return std::make_pair(mx[binary_search(x_target, x)], my[binary_search(y_target, y)]);
}

class Rectangle {
public:
    int x1, y1, x2, y2;
    Rectangle(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

int main() {
    std::vector<int> x;//x - coordinates
    std::vector<int> y;//y - coordinates
    std::unordered_map<int, int> mx;//compressed x - coordinates
    std::unordered_map<int, int> my;// compressed y - coordinates
    int n; // number of rectangles
    std::cin >> n;
    if (n == 0) return 0;
    std::vector<Rectangle> Rectangles;
    Rectangles.reserve(n);
    x.reserve(2 * n);
    y.reserve(2 * n);
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        Rectangles.emplace_back(x1, y1, x2, y2);
        x.push_back(x1);
        x.push_back(x2);
        y.push_back(y1);
        y.push_back(y2);
    }
    Compress(x, y, mx, my); // coordinate compression method with unordered_map

    std::vector<std::vector<int>> Map(my.size(), std::vector<int>(mx.size(), 0));
    for (int id = 0; id < n; id++) {
        for (int i = my[Rectangles[id].y1]; i < my[Rectangles[id].y2]; i++) {
            for (int j = mx[Rectangles[id].x1]; j < mx[Rectangles[id].x2]; j++) {
                Map[i][j]++; // creating map of compressed rectangles
            }
        }
    }
    int m; // number of requests
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        std::pair<int, int> target;
        std::cin >> target.first >> target.second;
        if (target.first < x[0] || target.second < y[0]) {
            std::cout << 0 << '\n';
        }
        else {
            target = FindPosition(target.first, target.second, x, y, mx, my);
            std::cout << Map[target.second][target.first] << ' ';
        }
    }
    return 0;
}