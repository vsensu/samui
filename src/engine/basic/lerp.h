#pragma once

template <typename T>
static T lerp(T a, T b, T t)
{
    return a + t * (b - a);
}

template <typename T>
static T inverse_lerp(T a, T b, T value)
{
    if (a == b) return a;

    return (value - a) / (b - a);
}

static float map_to(float newmin, float newmax, float originmin, float originmax,
                   float value)
{
    return lerp(newmin, newmax, inverse_lerp(originmin, originmax, value));
}