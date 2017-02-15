#include "naiveintegrator.h"

Color3f NaiveIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    //TODO
    Intersection isect;
    Color3f Le;
    Color3f color;
    if (scene.Intersect(ray, &isect)) {
        Vector3f woW = - ray.direction;
        Le = isect.Le(woW);
        if (depth < 1 || !isect.objectHit->GetMaterial()) {
            color = Le;
        } else {
            isect.ProduceBSDF();
            Vector3f wiW;
            Point2f xi = sampler->Get2D();
            float pdf;

            Color3f c = isect.bsdf->Sample_f(woW, &wiW, xi, &pdf);
            Color3f li = Li(isect.SpawnRay(glm::normalize(wiW)), scene, sampler, depth -1);
            color +=  Le + c * li * glm::dot(wiW, isect.normalGeometric)/pdf;
        }

    }
    return color;
}
