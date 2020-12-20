#ifndef MODEL_H
#define MODEL_H

#include <QPainter>
#include <vector>

struct point {
    double x;
    double y;

    double* to_double_ptr() {
        return reinterpret_cast<double*>(this);
    }
};

class graham_hull
{
    std::vector<point> points;
    std::vector<int> border;

public:
    graham_hull(std::vector<point> const& points);
    void calculate_graham_hull();
    bool compare_polar(point &p1, point &p2);

    const std::vector<point>& get_points() {
        return points;
    }

    const std::vector<int>& get_border() {
        calculate_graham_hull();
        return border;
    }
};

#endif // MODEL_H
