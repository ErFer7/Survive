#include "../include/vector2D.h"

#include <math.h>

Vector2D CreateVector2D(int x, int y)
{
    Vector2D result;

    result.x = x;
    result.y = y;

    return result;
}

Vector2Df CreateVector2Df(float x, float y)
{
    Vector2Df result;

    result.x = x;
    result.y = y;

    return result;
}

Vector2D VectorFloat2Int(Vector2Df v)
{
    Vector2D result;

    result.x = (int)v.x;
    result.y = (int)v.y;

    return result;
}

Vector2Df VectorInt2Float(Vector2D v)
{
    Vector2Df result;

    result.x = (float)v.x;
    result.y = (float)v.y;

    return result;
}

Vector2D AddVector(Vector2D v, Vector2D w)
{
    Vector2D result;

    result.x = v.x + w.x;
    result.y = v.y + w.y;

    return result;
}

Vector2Df AddVectorf(Vector2Df v, Vector2Df w)
{
    Vector2Df result;

    result.x = v.x + w.x;
    result.y = v.y + w.y;

    return result;
}

Vector2D SubVector(Vector2D v, Vector2D w)
{
    Vector2D result;

    result.x = v.x - w.x;
    result.y = v.y - w.y;

    return result;
}

Vector2Df SubVectorf(Vector2Df v, Vector2Df w)
{
    Vector2Df result;

    result.x = v.x - w.x;
    result.y = v.y - w.y;

    return result;
}

Vector2D MultiplyVector(Vector2D v, Vector2D w)
{
    Vector2D result;

    result.x = v.x * w.x;
    result.y = v.y * w.y;

    return result;
}

Vector2Df MultiplyVectorf(Vector2Df v, Vector2Df w)
{
    Vector2Df result;

    result.x = v.x * w.x;
    result.y = v.y * w.y;

    return result;
}

Vector2D MultiplyVectorByScalar(Vector2D v, int a)
{
    Vector2D result;

    result.x = v.x * a;
    result.y = v.y * a;

    return result;
}

Vector2Df MultiplyVectorfByScalar(Vector2Df v, float a)
{
    Vector2Df result;

    result.x = v.x * a;
    result.y = v.y * a;

    return result;
}

float Distancef(Vector2Df v, Vector2Df w)
{
    return sqrtf(powf((v.x - w.x), 2.0f) + powf((v.y - w.y), 2.0f));
}

float Distance(Vector2D v, Vector2D w)
{
    Vector2Df vf = VectorInt2Float(v);
    Vector2Df wf = VectorInt2Float(w);

    return Distancef(vf, wf);
}
