#pragma once

namespace sb {

// Custom implementation because of frequent 'min' name clash
template <class T>
const T & minValue(const T & a, const T & b)
{
    return (b < a) ? b : a;
}

// Custom implementation because of frequent 'max' name clash
template <class T>
const T & maxValue(const T & a, const T & b)
{
    return (b > a) ? b : a;
}

} // namespace sb