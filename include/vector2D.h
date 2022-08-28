#pragma once

typedef struct
{
    float x;
    float y;
} Vector2Df;

typedef struct
{
    int x;
    int y;
} Vector2D;

Vector2D CreateVector2D(int x, int y);
Vector2Df CreateVector2Df(float x, float y);
Vector2D VectorFloat2Int(Vector2Df v);
Vector2Df VectorInt2Float(Vector2D v);
Vector2D AddVector(Vector2D v, Vector2D w);
Vector2Df AddVectorf(Vector2Df v, Vector2Df w);
Vector2D SubVector(Vector2D v, Vector2D w);
Vector2Df SubVectorf(Vector2Df v, Vector2Df w);
Vector2D MultiplyVector(Vector2D v, Vector2D w);
Vector2Df MultiplyVectorf(Vector2Df v, Vector2Df w);
Vector2D MultiplyVectorByScalar(Vector2D v, int a);
Vector2Df MultiplyVectorfByScalar(Vector2Df v, float a);
float Distancef(Vector2Df v, Vector2Df w);
float Distance(Vector2D v, Vector2D w);
