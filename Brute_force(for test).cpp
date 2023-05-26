#include<iostream>
#include <vector>
#include <chrono>

int f_pow(int a, int power, int n)

{
	if (power == 0)

	{
		return 1;

	}
	if (power % 2 == 0)
	{
		return f_pow((a * a) % n, power / 2, n);
	}
	return (a * f_pow(a, power - 1, n) % n);

}
class Rectangle {
public:
	int x1, y1, x2, y2;
	Rectangle(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

int main()
{
	//int n;
	//std::cin >> n; //number of rectangles
	std::vector<int> tests{ 2,4,8,16,32,64,128,256,512,1024,2048,4096 };
	for (auto& n : tests) {
		long long avg_time_of_preparing = 0;
		long long avg_time_of_request = 0;
		long long avg_time = 0;
		for (int num = 0; num < 100; num++) {
			auto start_of_preparing = std::chrono::steady_clock::now();
			auto start = start_of_preparing;
			std::vector<Rectangle> rectangles;
			for (int i = 0; i < n; i++) {
				/*int x1, y1, x2, y2;
				std::cin >> x1 >> y1 >> x2 >> y2;*/
				int x1 = 10 * i, y1 = 10 * i, x2 = 10 * (2 * n - i), y2 = 10 * (2 * n - i);
				rectangles.emplace_back(x1, y1, x2, y2);
			}
			auto end_of_preparing = std::chrono::steady_clock::now();
			avg_time_of_preparing = avg_time_of_preparing + std::chrono::duration_cast<std::chrono::nanoseconds>(end_of_preparing - start_of_preparing).count();
			/*int m;
			std::cin >> m;*/
			int m = 10 * n;
			for (int i = 0; i <= m; i++) {
				auto start_of_request = std::chrono::steady_clock::now();
				/*int x, y;
				std::cin >> x >> y;*/
				int x = f_pow(1009997 * i, 31, 2*n) , y = f_pow(1009993 * i, 31, 2*n);
				int count = 0;
				for (int i = 0; i < n; i++) {
					if (x >= rectangles[i].x1 && x < rectangles[i].x2 && y >= rectangles[i].y1 && y < rectangles[i].y2) {
						count++;
					}
				}
				auto end_of_request = std::chrono::steady_clock::now();
				avg_time_of_request = avg_time_of_request + (std::chrono::duration_cast<std::chrono::nanoseconds>(end_of_request - start_of_request).count());
				//std::cout << count << ' ';
			}
			auto end = std::chrono::steady_clock::now();
			avg_time = avg_time + std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			//std::cout << '\n';
		}
		std::cout << n << ":\n";
		std::cout << avg_time_of_preparing / 100 << '\n';
		std::cout << avg_time_of_request / 100 << '\n';
		std::cout << avg_time / 100 << '\n';
		std::cout << '\n';
	}
	return 0;
}
