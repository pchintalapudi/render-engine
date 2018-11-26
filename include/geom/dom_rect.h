//
// Created by prem on 11/26/2018.
//

#ifndef FEATHER_DOM_RECT_H
#define FEATHER_DOM_RECT_H
namespace dom {
    class DOMRect;
}

class dom::DOMRect {
public:

    DOMRect() : DOMRect(false) {}

    DOMRect(bool readOnly) : readOnly(readOnly), x(0), y(0), width(0), height(0) {}

    DOMRect(double x, double y, double width, double height) : readOnly(false), x(x), y(y), width(width),
                                                               height(height) {}

    DOMRect(bool readOnly, double x, double y, double width, double height) : readOnly(readOnly), x(x), y(y),
                                                                              width(width), height(height) {}

    DOMRect(DOMRect &other) : readOnly(other.readOnly), x(other.x), y(other.y), width(other.width),
                              height(other.height) {}

    inline bool isReadOnly() { return readOnly; }

    inline double getX() { return x; }

    inline bool setX(double x) {
        if (!readOnly) this->x = x;
        return !readOnly;
    }

    inline double getY() { return y; }

    inline bool setY(double y) {
        if (!readOnly) this->y = y;
        return !readOnly;
    }

    inline double getWidth() { return width; }

    inline bool setWidth(double width) {
        if (!readOnly) this->width = width;
        return !readOnly;
    }

    inline double getHeight() { return height; }

    inline bool setHeight(double height) {
        if (!readOnly) this->height = height;
        return !readOnly;
    }

    inline double getTop() { return y + (height < 0 ? height : 0); }

    inline double getLeft() { return x + (width < 0 ? width : 0); }

    inline double getBottom() { return y + (height > 0 ? height : 0); }

    inline double getRight() { return x + (width > 0 ? width : 0); }

private:
    bool readOnly;
    double x;
    double y;
    double width;
    double height;
};

#endif //FEATHER_DOM_RECT_H
