#include "sphere.h"
#include <iostream>
#include <math.h>

float Sphere::Area() const
{
    //TODO later
}

void Sphere::ComputeTBN(const Point3f& P, Normal3f* nor, Vector3f* tan, Vector3f* bit) const
{
    Vector4f oN = Vector4f(glm::normalize(P), 0.f);
    *nor = glm::normalize(transform.invTransT() * glm::normalize(P));
    //TODO: Compute tangent and bitangent

    Vector3f oB = Vector3f(0.f, 1.f, 0.f);
    Vector3f t = glm::cross(oB, Vector3f(oN));
    Vector3f b = glm::cross(Vector3f(oN), t);
    *bit = Vector3f(glm::normalize(transform.T() * Vector4f(b,0.f)));
    *tan = Vector3f(glm::normalize(transform.T() * Vector4f(glm::cross(b, Vector3f(oN)),0.f)));
}

bool Sphere::Intersect(const Ray &ray, Intersection *isect) const
{
    //Transform the ray
    Ray r_loc = ray.GetTransformedCopy(transform.invT());

    float A = pow(r_loc.direction.x, 2.f) + pow(r_loc.direction.y, 2.f) + pow(r_loc.direction.z, 2.f);
    float B = 2*(r_loc.direction.x*r_loc.origin.x + r_loc.direction.y * r_loc.origin.y + r_loc.direction.z * r_loc.origin.z);
    float C = pow(r_loc.origin.x, 2.f) + pow(r_loc.origin.y, 2.f) + pow(r_loc.origin.z, 2.f) - 1.f;//Radius is 1.f
    float discriminant = B*B - 4*A*C;
    //If the discriminant is negative, then there is no real root
    if(discriminant < 0){
        return false;
    }
    float t = (-B - sqrt(discriminant))/(2*A);
    if(t < 0)
    {
        t = (-B + sqrt(discriminant))/(2*A);
    }
    if(t >= 0)
    {
        Point3f P = glm::vec3(r_loc.origin + t*r_loc.direction);
        InitializeIntersection(isect, t, P);
        return true;
    }
    return false;
}

Point2f Sphere::GetUVCoordinates(const Point3f &point) const
{
    Point3f p = glm::normalize(point);
    float phi = atan2f(p.z, p.x);
    if(phi < 0)
    {
        phi += TwoPi;
    }
    float theta = glm::acos(p.y);
    return Point2f(1 - phi/TwoPi, 1 - theta / Pi);
}
