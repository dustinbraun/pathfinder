#include <iostream>

#include <nav2/query.hpp>

#include "profiler.hpp"

using namespace nav2;

uint16_t vertex_count = 86;

VertexData vertices[] = {
    { 47, 43, },
    { 48, 90, },
    { 101, 49, },
    { 97, 111, },
    { 147, 82, },
    { 151, 124, },
    { 194, 104, },
    { 206, 63, },
    { 237, 114, },
    { 263, 81, },
    { 272, 126, },
    { 311, 110, },
    { 285, 174, },
    { 319, 174, },
    { 270, 225, },
    { 323, 239, },
    { 245, 259, },
    { 291, 277, },
    { 231, 295, },
    { 276, 321, },
    { 220, 337, },
    { 258, 365, },
    { 169, 373, },
    { 197, 401, },
    { 129, 374, },
    { 133, 415, },
    { 62, 394, },
    { 69, 438, },
    { 26, 424, },
    { 338, 325, },
    { 356, 298, },
    { 358, 362, },
    { 415, 368, },
    { 431, 331, },
    { 454, 381, },
    { 477, 345, },
    { 535, 365, },
    { 539, 316, },
    { 597, 315, },
    { 574, 274, },
    { 621, 273, },
    { 617, 227, },
    { 586, 204, },
    { 550, 183, },
    { 577, 263, },
    { 572, 141, },
    { 516, 135, },
    { 468, 163, },
    { 454, 124, },
    { 405, 120, },
    { 419, 168, },
    { 367, 119, },
    { 366, 166, },
    { 419, 417, },
    { 442, 448, },
    { 389, 446, },
    { 393, 491, },
    { 426, 516, },
    { 474, 526, },
    { 495, 473, },
    { 533, 524, },
    { 570, 484, },
    { 580, 417, },
    { 534, 411, },
    { 592, 372, },
    { 366, 560, },
    { 384, 600, },
    { 320, 573, },
    { 308, 626, },
    { 271, 570, },
    { 242, 607, },
    { 236, 648, },
    { 187, 597, },
    { 146, 609, },
    { 137, 656, },
    { 110, 581, },
    { 160, 553, },
    { 139, 511, },
    { 100, 509, },
    { 113, 569, },
    { 124, 459, },
    { 293, 463, },
    { 251, 450, },
    { 271, 485, },
    { 319, 514, },
    { 257, 532, },
};

uint16_t face_count = 84;

FaceData faces[] = {
    { 0, 1, 2, },
    { 1, 3, 2, },
    { 3, 4, 2, },
    { 3, 5, 4, },
    { 5, 6, 4, },
    { 6, 7, 4, },
    { 6, 8, 7, },
    { 8, 9, 7, },
    { 8, 10, 9, },
    { 10, 11, 9, },
    { 10, 12, 11, },
    { 12, 13, 11, },
    { 12, 14, 13, },
    { 14, 15, 13, },
    { 14, 16, 15, },
    { 16, 17, 15, },
    { 16, 18, 17, },
    { 18, 19, 17, },
    { 18, 20, 19, },
    { 20, 21, 19, },
    { 20, 22, 21, },
    { 22, 23, 21, },
    { 22, 24, 23, },
    { 24, 25, 23, },
    { 24, 26, 25, },
    { 26, 27, 25, },
    { 26, 28, 27, },
    { 19, 29, 17, },
    { 29, 30, 17, },
    { 29, 31, 30, },
    { 31, 32, 30, },
    { 32, 33, 30, },
    { 32, 34, 33, },
    { 34, 35, 33, },
    { 34, 36, 35, },
    { 36, 37, 35, },
    { 36, 38, 37, },
    { 37, 38, 39, },
    { 38, 40, 39, },
    { 40, 41, 39, },
    { 41, 42, 39, },
    { 42, 43, 44, },
    { 42, 45, 43, },
    { 45, 46, 43, },
    { 46, 47, 43, },
    { 46, 48, 47, },
    { 48, 49, 47, },
    { 49, 50, 47, },
    { 49, 51, 50, },
    { 51, 52, 50, },
    { 51, 11, 52, },
    { 11, 13, 52, },
    { 32, 53, 34, },
    { 53, 54, 34, },
    { 53, 55, 54, },
    { 55, 56, 54, },
    { 56, 57, 54, },
    { 57, 58, 54, },
    { 58, 59, 54, },
    { 58, 60, 59, },
    { 60, 61, 59, },
    { 61, 62, 59, },
    { 62, 63, 59, },
    { 62, 64, 63, },
    { 64, 36, 63, },
    { 64, 38, 36, },
    { 57, 56, 65, },
    { 57, 65, 66, },
    { 65, 67, 66, },
    { 67, 68, 66, },
    { 67, 69, 68, },
    { 69, 70, 68, },
    { 70, 71, 68, },
    { 70, 72, 71, },
    { 72, 73, 71, },
    { 73, 74, 71, },
    { 73, 75, 74, },
    { 73, 76, 75, },
    { 76, 77, 75, },
    { 77, 78, 79, },
    { 77, 80, 78, },
    { 81, 82, 83, },
    { 81, 84, 83, },
    { 83, 85, 84, },
};

int main() {
    Mesh mesh;
    mesh.import(vertices, vertex_count, faces, face_count);

    Query query([&](const Mesh & mesh, const NodeArena & arena) {});

    Profiler profiler;
    int64_t duration = profiler.run(1000, [&]() {
        query.search(mesh, 2, Point(117, 83), 77, Point(142, 585));
    });

    std::cout << duration << " nanoseconds" << std::endl;


    return 0;
}