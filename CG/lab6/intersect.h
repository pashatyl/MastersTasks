#ifndef INTERSECT_H
#define INTERSECT_H

#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>

const int LEFT_END = -1;
const int RIGHT_END = 1;
const int TOP_END = -2;
const int BOTTOM_END = -2;

const int VERTICAL = 0;
const int  HORIZONTAL = 1;

struct Point{
    double x;
    double y;
    int type;
    int line_ind;

    Point() {}
    Point(double x, double y) : x(x), y(y){};
    bool operator< (const Point p2) const {
        return x <= p2.x;
    }
};

struct Line{
    int type;
    Line() {};
    Line(Point p1, Point p2){
        if (p1.x == p2.x){
            type = VERTICAL;
        }
        if (p1.y == p2.y){
            type = HORIZONTAL;
        }
        if (p1.x < p2.x || p1.y < p2.y){
            first_point = p1;
            second_point = p2;
        } else {
            first_point = p2;
            second_point = p1;
        }
        first_point.type = LEFT_END;
        second_point.type = RIGHT_END;

        if (p1.y < p2.y){
            _first_point = p1;
            _second_point = p2;
        } else {
            _first_point = p2;
            _second_point = p1;
        }
        _first_point.type = TOP_END;
        _second_point.type = BOTTOM_END;
    }

    Point first_point;
    Point second_point;
    Point _first_point;
    Point _second_point;

    bool operator< (const Line& s1) const {
        return _second_point.y > s1._second_point.y;
    }
};

std::vector<std::pair<int, int>> get_lines_intersections(std::vector<Line>);

#endif // INTERSECT_H
