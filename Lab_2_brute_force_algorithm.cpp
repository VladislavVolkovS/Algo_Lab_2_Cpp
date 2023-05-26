#include<iostream>
#include <vector>

class Rectangle {
public:
	int x1, y1, x2, y2;
	Rectangle(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

int main()
{
	int n;
	std::cin >> n; //number of rectangles
	std::vector<Rectangle> rectangles;
	for (int i = 0; i < n; i++) {
		int x1, y1, x2, y2;
		std::cin >> x1 >> y1 >> x2 >> y2;
		rectangles.emplace_back(x1, y1, x2, y2);
	}
	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		int count = 0;
		for (int i = 0; i < n; i++) {
			if (x >= rectangles[i].x1 && x < rectangles[i].x2 && y >= rectangles[i].y1 && y < rectangles[i].y2) {
				count++;
			}
		}
		std::cout << count << ' ';
	}
	return 0;
}
