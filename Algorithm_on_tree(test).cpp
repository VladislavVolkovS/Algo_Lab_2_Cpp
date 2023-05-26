﻿#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
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
//segment which add to scanline 
class segment {
public:
    int y1, y2;
    int operation;
    segment() = default;
    segment(int _y1, int _y2, int _operation) : y1(_y1), y2(_y2), operation(_operation) {}
};

struct Node {
    Node* left; //link to left child
    Node* right; //link to right child
    std::pair<int, int> segment; //info about which segment this node covers
    int modify;
    Node(int _begin, int _end) : left(nullptr), right(nullptr), modify(0) {
        segment.first = _begin;
        segment.second = _end;
    }
    Node(Node* other) : left(other->left), right(other->right), modify(other->modify) {
        segment.first = other->segment.first;
        segment.second = other->segment.second;
    }
    ~Node() { delete left; delete right; }
};

//creating of segment tree(all modifies = 0 at the beginning)
Node* build_segment_tree(Node* root) {
    if (root->segment.first == root->segment.second) {
        return new Node(root->segment.first, root->segment.second);
    }
    int middle = root->segment.first + (root->segment.second - root->segment.first) / 2;
    root->left = build_segment_tree(new Node(root->segment.first, middle));
    root->right = build_segment_tree(new Node(middle + 1, root->segment.second));
    return root;
}

//creating of new version(add modifies) (with method copy path)
Node* create_version(Node* root, int begin, int end, int operation) {
    Node* tmp = new Node(root);
    if (tmp->segment.first == begin && tmp->segment.second == end) {
        tmp->modify += operation;
        return tmp;
    }
    if (tmp->segment.first > end || tmp->segment.second < begin) {
        return tmp;
    }
    if (tmp->left->segment.first <= begin && tmp->left->segment.second >= end) {
        tmp->left = create_version(tmp->left, begin, end, operation);
    }
    else if (tmp->right->segment.first <= begin && tmp->right->segment.second >= end) {
        tmp->right = create_version(tmp->right, begin, end, operation);
    }
    else {
        tmp->left = create_version(tmp->left, begin, tmp->left->segment.second, operation);
        tmp->right = create_version(tmp->right, tmp->right->segment.first, end, operation);
    }
    return tmp;
}

//compressing of coordinates (with using map)
void Compress(std::set<int>& x, std::set<int>& y, std::map<int, int>& mx, std::map<int, int>& my) {
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

//collect all modifies on path to target leaf
int get_number_of_intersection(int sum_of_modify, Node* root, int target) {
    if (root->segment.first == root->segment.second && root->segment.second == target) {
        return sum_of_modify + root->modify;
    }
    if (root->segment.first > target || root->segment.second < target) {
        return sum_of_modify;
    }
    if (root->left->segment.first <= target && root->left->segment.second >= target) {
        sum_of_modify += get_number_of_intersection(sum_of_modify, root->left, target);
        sum_of_modify += root->modify;
    }
    else if (root->right->segment.first <= target && root->right->segment.second >= target) {
        sum_of_modify += get_number_of_intersection(sum_of_modify, root->right, target);
        sum_of_modify += root->modify;
    }
    return sum_of_modify;
}

//left binary search(find element <= target)
int binary_search(const int& target, const std::vector<int>& mas) {
    int left = -1, right = (int)mas.size();
    while (right > left + 1) {
        int middle = (left + right) / 2;
        if (mas[middle] <= target) {
            left = middle;
        }
        else {
            right = middle;
        }
    }
    return mas[left];
}


int main() {
    //int n;
    //std::cin >> n; //number of rectangles
    std::vector<int> tests{ 2,4,8,16,32,64,128,256,512,1024,2048,4096 };
    for (auto& n : tests) {
        long long avg_time_of_preparing = 0;
        long long avg_time_of_request = 0;
        long long avg_time = 0;
        for (int num = 0; num < 100; num++) {
            auto start = std::chrono::steady_clock::now();
            auto start_of_preparing = start;
            if (n == 0) return 0;
            std::map<int, std::vector<segment>> matching; //map which contains info about segment on scanline 
            std::vector<Node*> roots; //vector of versions
            std::set<int> x;//x - coordinates
            std::set<int> y;//y - coordinates
            std::map<int, int> mx;//compressed x - coordinates
            std::map<int, int> my;// compressed y - coordinates
            for (int i = 0; i < n; i++) {
                /*int x1, y1, x2, y2;
                std::cin >> x1 >> y1 >> x2 >> y2;*/
                int x1 = 10 * i, y1 = 10 * i, x2 = 10 * (2 * n - i), y2 = 10 * (2 * n - i);
                matching[x1].push_back(segment(y1, y2, 1));
                matching[x2].push_back(segment(y1, y2, -1));
                x.insert(x1);
                x.insert(x2);
                y.insert(y1);
                y.insert(y2);
            }
            Compress(x, y, mx, my); // coordinate compression method with unordered_map
            Node* root = new Node(0, my.size() - 1);
            root = build_segment_tree(root);

            //creating versions
            for (auto& ix : x) {
                for (auto& seg : matching[ix]) {
                    root = create_version(root, my[seg.y1], my[seg.y2] - 1, seg.operation);
                }
                roots.push_back(root);
            }
            std::vector<int> new_x(x.begin(), x.end()); //new_x contains unique values
            std::vector<int> new_y(y.begin(), y.end()); //new_y contains unique values
            auto end_of_preparing = std::chrono::steady_clock::now();
            avg_time_of_preparing = avg_time_of_preparing + std::chrono::duration_cast<std::chrono::nanoseconds>(end_of_preparing - start_of_preparing).count();
            /*int m;
            std::cin >> m;*/ //number of requests
            int m = 10 * n;
            for (int i = 0; i < m; i++) {
                auto start_of_request = std::chrono::steady_clock::now();
                /*int x_target, y_target;
                std::cin >> x_target >> y_target;*/
                int x_target = f_pow(1009997 * i, 31, 2 * n), y_target = f_pow(1009993 * i, 31, 2 * n);
                if (x_target < new_x[0] || y_target < new_y[0]) {
                    //std::cout << 0 << '\n';
                }
                else {
                    //std::cout << get_number_of_intersection(0, roots[mx[binary_search(x_target, new_x)]], my[binary_search(y_target, new_y)]) << ' ';
                    int b = get_number_of_intersection(0, roots[mx[binary_search(x_target, new_x)]], my[binary_search(y_target, new_y)]);
                }
                auto end_of_request = std::chrono::steady_clock::now();
                avg_time_of_request = avg_time_of_request + (std::chrono::duration_cast<std::chrono::nanoseconds>(end_of_request - start_of_request).count());
            }
            auto end = std::chrono::steady_clock::now();
            avg_time = avg_time + std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }
        std::cout << n << ":\n";
        std::cout << avg_time_of_preparing / 100 << '\n';
        std::cout << avg_time_of_request / 100 << '\n';
        std::cout << avg_time / 100 << '\n';
        std::cout << '\n';
    }
    return 0;
}