#include "triangle.h"

#include <algorithm>

namespace ed
{

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    : v1(v1)
    , v2(v2)
    , v3(v3)
{
    computeAABB();
}

glm::vec3 Triangle::normal() const
{
    glm::vec3 edge1 = v2.position - v1.position;
    glm::vec3 edge2 = v3.position - v1.position;
    return glm::normalize(glm::cross(edge1, edge2));
}

glm::vec3 Triangle::normal(const glm::vec3& point) const
{
    glm::vec3 barycentric = barycentricCoordinates(point);
    return v1.normal * barycentric.x +
           v2.normal * barycentric.y +
           v3.normal * barycentric.z;
}

glm::vec2 Triangle::uv(const glm::vec3& point) const
{
    glm::vec3 barycentric = barycentricCoordinates(point);
    return v1.uv * barycentric.x +
           v2.uv * barycentric.y +
           v3.uv * barycentric.z;
}

glm::vec3 Triangle::barycentricCoordinates(const glm::vec3& point) const
{
    const glm::vec3& p1 = v1.position;
    const glm::vec3& p2 = v2.position;
    const glm::vec3& p3 = v3.position;

    float det = (p1.x - p3.x) * (p2.y - p3.y) + (p3.x - p2.x) * (p1.y - p3.y);

    float u   = (point.x - p3.x) * (p2.y - p3.y) + (p3.x - p2.x) * (point.y - p3.y);
    float v   = (point.x - p3.x) * (p3.y - p1.y) + (p1.x - p3.x) * (point.y - p3.y);

    u /= det;
    v /= det;

    float w = 1.0f - u - v;

    return glm::vec3(u, v, w);
}

AABB Triangle::aabb() const
{
    return mBox;
}

void Triangle::computeAABB()
{
    glm::vec3 min = v1.position;
    glm::vec3 max = v1.position;

    float minX = std::min(v2.position.x, v3.position.x);
    float minY = std::min(v2.position.y, v3.position.y);
    float minZ = std::min(v2.position.z, v3.position.z);

    min.x = std::min(min.x, minX);
    min.y = std::min(min.y, minY);
    min.z = std::min(min.z, minZ);

    float maxX = std::max(v2.position.x, v3.position.x);
    float maxY = std::max(v2.position.y, v3.position.y);
    float maxZ = std::max(v2.position.z, v3.position.z);

    max.x = std::max(max.x, maxX);
    max.y = std::max(max.y, maxY);
    max.z = std::max(max.z, maxZ);

    mBox = AABB(min, max);
}

} // namespace ed
