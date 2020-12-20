#ifndef KDNODE_H
#define KDNODE_H

#include <vector>

#include <QPoint>
#include <QRect>

class KDNode
{
private:
    QPoint point;
    bool horizontal;

    KDNode* left;
    KDNode* right;

    KDNode(std::vector<QPoint> points, bool horizontally);

    bool intersects(QRect* rect);
    bool isInside(QRect *rect);

    bool isInLeftSubtree(QRect *rect);
public:
    KDNode(std::vector<QPoint> points);
    ~KDNode();
    std::vector<QPoint> insideRectangle(QRect* rect);
};

#endif // KDNODE_H
