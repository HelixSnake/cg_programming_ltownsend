#pragma once
#include "Application.h"

struct Matrix4x4custom {
float a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3, d0, d1, d2, d3;
static Matrix4x4custom Identity();
static Matrix4x4custom MultiplyMbyM(Matrix4x4custom m1, Matrix4x4custom m2);
static Matrix4x4custom MultiplyMbyV(Matrix4x4custom m1, vec4 v1);
};