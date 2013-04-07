#ifndef GEOM_H
#define GEOM_H

struct Point {
    double x, y;
};

struct Vec {
    double x, y, z;
};

// Basic arithmetic:

Vec operator+(Vec a, Vec b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Vec operator+=(Vec & a, Vec b) {
    return (a = a + b);
}

Vec operator-(Vec a, Vec b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
Vec operator-=(Vec & a, Vec b) {
    return (a = a - b);
}

Vec operator*(Vec a, double scalar) {
    return {a.x * scalar, a.y * scalar, a.z * scalar};
}
Vec operator*(double scalar, Vec a) {
    return {a.x * scalar, a.y * scalar, a.z * scalar};
}
Vec operator*=(Vec & a, double scalar) {
    return (a = a * scalar);
}

Vec operator/(Vec a, double scalar) {
    return {a.x / scalar, a.y / scalar, a.z / scalar};
}
Vec operator/(double scalar, Vec a) {
    return {a.x / scalar, a.y / scalar, a.z / scalar};
}
Vec operator/=(Vec & a, double scalar) {
    return (a = a / scalar);
}

#endif // GEOM_H
