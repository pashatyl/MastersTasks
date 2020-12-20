#include "node.h"
#include <algorithm>

bool compareOnX(QPoint const& p1, QPoint const& p2) {
    return p1.x() < p2.x();
}

bool compareOnY(QPoint const& p1, QPoint const& p2) {
    return p1.y() < p2.y();
}

KDNode::KDNode(std::vector<QPoint> points, bool horizontally) :
    left(nullptr), right(nullptr), horizontal(horizontally) {
    if (points.size() >= 2) {
        uint median = points.size() / 2;
        std::nth_element(points.begin(), points.begin() + median,
                         points.end(), horizontally ? compareOnX : compareOnY);
        this->point = points[median];

        std::vector<QPoint> leftPoints, rightPoints;
        for (QPoint const& point : points) {
            if (this->point != point) {
                if ((horizontally && point.x() >= this->point.x()) || (!horizontally && point.y() >= this->point.y())) {
                    rightPoints.push_back(point);
                } else {
                    leftPoints.push_back(point);
                }
            }
        }

        if (!leftPoints.empty()) {
            this->left = new KDNode(leftPoints, !horizontally);
        }

        if (!rightPoints.empty()) {
            this->right = new KDNode(rightPoints, !horizontally);
        }
    } else if (!points.empty()) {
        this->point = points[0];
    }
}

KDNode::KDNode(std::vector<QPoint> points): KDNode(points, true) {}

bool KDNode::intersects(QRect *rect) {
    bool isIntersectsVertical = this->horizontal && this->point.x() >= rect->left() && this->point.x() <= rect->right();
    bool isIntersectsHorizontal = !this->horizontal && this->point.y() >= rect->top() && this->point.y() <= rect->bottom();
    return isIntersectsVertical || isIntersectsHorizontal;
}

bool KDNode::isInLeftSubtree(QRect *rect) {
    bool isInLeftSubtreeVertical = this->horizontal && this->point.x() > rect->right();
    bool isInLeftSubtreeHorizontal = !this->horizontal && this->point.y() > rect->bottom();
    return isInLeftSubtreeVertical || isInLeftSubtreeHorizontal;
}

bool KDNode::isInside(QRect *rect) {
    return rect->contains(this->point);
}

void insertNodePoints(KDNode *node, QRect *rect, std::vector<QPoint> *points) {
    if (node != nullptr) {
        std::vector<QPoint> const results = node->insideRectangle(rect);
        if (results.size() != 0) {
            points->insert(points->end(), results.begin(), results.end());
        }
    }
}

std::vector<QPoint> KDNode::insideRectangle(QRect *rect) {
    std::vector<QPoint> results;
    if (this->intersects(rect)) {
        if (this->isInside(rect)) {
            results.push_back(this->point);
        }

        insertNodePoints(this->left, rect, &results);
        insertNodePoints(this->right, rect, &results);

    } else if (this->isInLeftSubtree(rect)) {
        insertNodePoints(this->left, rect, &results);
    } else {
        insertNodePoints(this->right, rect, &results);
    }
    return results;
}

KDNode::~KDNode() {
    delete left;
    delete right;
}
