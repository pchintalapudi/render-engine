//
// Created by prem on 12/13/2018.
//

#ifndef FEATHER_DOM_RECT_H
#define FEATHER_DOM_RECT_H
namespace feather {
    namespace dom {
        class DOMRectReadOnly {
        public:
            DOMRectReadOnly() = default;

            DOMRectReadOnly(double x, double y, double width, double height)
                    : x(x), y(y), width(width), height(height) {}

            inline double getX() const { return x; }

            inline double getY() const { return y; }

            inline double getWidth() const { return width; }

            inline double getHeight() const { return height; }

            inline double getTop() const { return height < 0 ? y + height : y; }

            inline double getRight() const { return width < 0 ? x : (x + width); }

            inline double getBottom() const { return height < 0 ? y : (y + height); }

            inline double getLeft() const { return width < 0 ? y + width : y; }

        protected:
            double x = 0, y = 0, width = 0, height = 0;
        };

        class DOMRect : public DOMRectReadOnly {
        public:

            DOMRect() : DOMRectReadOnly() {}

            DOMRect(double x, double y, double width, double height)
                    : DOMRectReadOnly(x, y, width, height) {}

            inline void setX(double x) { this->x = x; }

            inline void setY(double y) { this->y = y; }

            inline void setWidth(double width) { this->width = width; }

            inline void setHeight(double height) { this->height = height; }
        };
    }
}
#endif //FEATHER_DOM_RECT_H
