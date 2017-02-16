#include "lambertbrdf.h"
#include <warpfunctions.h>

Color3f LambertBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    return R * InvPi;
}

Color3f LambertBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                        Float *pdf, BxDFType *sampledType) const
{
    //TODO
    if (sampledType) *sampledType = type;
    *wi = WarpFunctions::squareToHemisphereCosine(u);
    *pdf = LambertBRDF::Pdf(wo, *wi);
    return LambertBRDF::f(wo,*wi);
}

float LambertBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    //return SameHemisphere(wo,wi) ? WarpFunctions::squareToHemisphereCosinePDF(wi):0;
    return WarpFunctions::squareToHemisphereCosinePDF(wi);
}
