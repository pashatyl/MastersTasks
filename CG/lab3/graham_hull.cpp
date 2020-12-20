#include "graham_hull.h"
#include "Predicates.h"

graham_hull::graham_hull(std::vector<point> const& points) {
    this->points = points;
    exact::Init();
}

bool graham_hull::compare_polar(point &p1, point &p2) {
    int orientation = exact::orient2d(
                points[0].to_double_ptr(),
                p1.to_double_ptr(),
                p2.to_double_ptr());
    return orientation < 0;
}

int orient2dPoint(point &p1, point &p2, point &p3) {
    return exact::orient2d(
                p1.to_double_ptr(),
                p2.to_double_ptr(),
                p3.to_double_ptr());
}

void graham_hull::calculate_graham_hull() {
    std::partial_sort(points.begin(), points.begin() + 1, points.end(),
                      [](point const& p1, point const& p2) -> bool {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    });
    std::sort(points.begin() + 1, points.end(),
              [this](point &p1, point &p2) -> bool { return compare_polar(p1, p2); });
    border.clear();
    border.push_back(0);
    border.push_back(1);
    for (size_t i = 2; i < points.size(); ++i) {
        while (orient2dPoint(
                   points[border[border.size()-1]],
                   points[border[border.size()-2]],
                   points[i]) < 0) {
            border.pop_back();
        }
        border.push_back(i);
    }
}
