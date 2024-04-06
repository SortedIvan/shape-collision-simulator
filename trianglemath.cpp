#include <iostream>
#include <cmath>

float vectorLength(float x, float y);
float heronArea(float s, float side_a, float side_b, float side_c);

int main() {
    float a_x = 2;
    float a_y = 2;

    float b_x = 4;
    float b_y = 1.4;

    float c_x = 4;
    float c_y = 0.5;

    float p_x = 2.8;
    float p_y = 1.6;

    float a_c_x = c_x - a_x;
    float a_c_y = c_y - a_y;

    float a_b_x = b_x - a_x;
    float a_b_y = b_y - a_y;

    float c_b_x = c_x - b_x;
    float c_b_y = c_y - b_y;

    float a_c_length = vectorLength(a_c_x, a_c_y);
    float a_b_length = vectorLength(a_b_x, a_b_y);
    float c_b_length = vectorLength(c_b_x, c_b_y);

    float p_a_x = p_x - a_x;
    float p_a_y = p_y - a_y;

    float p_b_x = p_x - b_x;
    float p_b_y = p_y - b_y;

    float p_c_x = p_x - c_x;
    float p_c_y = p_y - c_y;

    float p_a_length = vectorLength(p_a_x, p_a_y);
    float p_b_length = vectorLength(p_b_x, p_b_y);
    float p_c_length = vectorLength(p_c_x, p_c_y);

    float perimeter1 = a_c_length + p_a_length + p_c_length;
    float perimeter2 = a_b_length + p_a_length + p_b_length;
    float perimeter3 = p_c_length + c_b_length + p_b_length;

    float area_1 = heronArea(perimeter1, a_c_length, p_a_length, p_c_length);
    float area_2 = heronArea(perimeter2, a_b_length, p_a_length, p_b_length);
    float area_3 = heronArea(perimeter3, p_c_length, c_b_length, p_b_length);

    float actualArea = heronArea(
        (a_c_length + a_b_length + c_b_length), a_c_length, a_b_length, c_b_length
    );

    std::cout << "point area: " << (area_1 + area_2 + area_3) << std::endl;
    std::cout << "actual area: " << actualArea << std::endl;

    // Introduce a margin of error
    float margin = 0.01;
    if (std::abs((area_1 + area_2 + area_3) - actualArea) <= margin) {
        std::cout << "Point lies inside the triangle." << std::endl;
    }
    else {
        std::cout << "Point does not lie inside the triangle." << std::endl;
    }

    return 0;
}


float vectorLength(float x, float y) {
    return std::sqrt((x * x) + (y * y));
}

float heronArea(float s, float side_a, float side_b, float side_c) {
    s = s / 2;
    return std::sqrt(
        s * (s - side_a) * (s - side_b) * (s - side_c)
    );
}
