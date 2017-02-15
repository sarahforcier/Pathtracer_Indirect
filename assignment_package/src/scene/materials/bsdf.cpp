#include "bsdf.h"
#include <warpfunctions.h>

BSDF::BSDF(const Intersection& isect, float eta /*= 1*/)
//TODO: Properly set worldToTangent and tangentToWorld
    : worldToTangent(),
      tangentToWorld(),
      normal(isect.normalGeometric),
      eta(eta),
      numBxDFs(0),
      bxdfs{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
    UpdateTangentSpaceMatrices(isect.tangent, isect.bitangent, normal);
}


void BSDF::UpdateTangentSpaceMatrices(const Normal3f& n, const Vector3f& t, const Vector3f b)
{
    //TODO: Update worldToTangent and tangentToWorld based on the normal, tangent, and bitangent
    tangentToWorld = Matrix3x3(t, b, n);
    worldToTangent = glm::inverse(tangentToWorld);
}

Color3f BSDF::f(const Vector3f &woW, const Vector3f &wiW, BxDFType flags /*= BSDF_ALL*/) const
{
    //TODO
    Color3f sum = Color3f(0.f);
    for (int i = 0; i < numBxDFs; i++) {
        if (bxdfs[i]->MatchesFlags(flags)) {
            Vector3f woT = worldToTangent * woW;
            Vector3f wiT = worldToTangent * wiW;
            sum += bxdfs[i]->f(woT, wiT);
        }
    }
    return sum;
}

Color3f BSDF::Sample_f(const Vector3f &woW, Vector3f *wiW, const Point2f &xi,
                       float *pdf, BxDFType type, BxDFType *sampledType) const
{
    // Use the input random number _xi_ to select
    // one of our BxDFs that matches the _type_ flags.
    std::vector<BxDF*> arr;
    for (int i = 0; i < numBxDFs; i++) {
        if (bxdfs[i]->MatchesFlags(type)) arr.push_back(bxdfs[i]);
    }
    int index = int(xi.x)%arr.size();
    BxDF* selection = bxdfs[index];

    // rewrite the random number contained in _xi_ to another number
    Point2f u = Point2f(xi.x * numBxDFs - index, xi.y);

    // Convert woW and wiW into tangent space and pass them to
    // the chosen BxDF's Sample_f (along with pdf).
    // Store the color returned by BxDF::Sample_f and convert
    // the _wi_ obtained from this function back into world space.
    Vector3f woT = worldToTangent * woW;
    Vector3f wiT;
    *pdf = 0;
    selection->Sample_f(woT,&wiT,u,pdf,sampledType);
    *wiW = tangentToWorld * wiT;

    // Iterate over all BxDFs that we DID NOT select above (so, all
    // but the one sampled BxDF) and add their PDFs to the PDF we obtained
    // from BxDF::Sample_f, then average them all together.
    *pdf = Pdf(woW, *wiW, type);

    // Finally, iterate over all BxDFs and sum together the results of their
    // f() for the chosen wo and wi, then return that sum.
    return f(woW, *wiW, type);
}


float BSDF::Pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const
{
    //TODO
    int num = 0;
    float sum = 0.f;
    for (int i = 0; i < numBxDFs; i++) {
        if (bxdfs[i]->MatchesFlags(flags)) {
            num++;
            Vector3f woT = worldToTangent * woW;
            Vector3f wiT = worldToTangent * wiW;
            sum += bxdfs[i]->Pdf(woT, wiT);
        }
    }
    return sum;
}

Color3f BxDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &xi,
                       Float *pdf, BxDFType *sampledType) const
{
    //TODO
    if (sampledType) *sampledType = type;
    Vector3f w = WarpFunctions::squareToHemisphereUniform(xi);
    Vector3f temp;
    *wi = w;
    *pdf = Pdf(wo, w);
    return f(wo,w);
}

// The PDF for uniform hemisphere sampling
float BxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return SameHemisphere(wo, wi) ? Inv2Pi : 0;
}
