/**
 * @file predicats.hpp
 * @brief Defines some predicats.
 * @author MLTCRBN
 */
#ifndef PREDICATS_HPP_INCLUDED
#define PREDICATS_HPP_INCLUDED

#include "struct_predicats.hpp"


/**
 * @brief Check if \p v in inside \p t.
 * @param[in] v The vertex   to test.
 * @param[in] t The triangle to test.
 * @return false if \pv isn't on \p t, true otherwise.
 */
bool isInThisTriangle(const Pvertex3D& v, const Ptriangle3D& t);


/**
 * @brief Decide if \p s is inside the surrounding circle of the "triangle" compound of
 * \p p, \p q, \p r, in a counterclockwise order.
 * @param[in] p The first  point of the triangle.
 * @param[in] q The second point of the triangle.
 * @param[in] r The thirs  point of the triangle.
 * @return true if \p s is inside the surrounding triangle.
 */
bool isInSurroundingCircle(const Pvertex3D& p, const Pvertex3D& q, const Pvertex3D& r, const Pvertex3D& s);

/**
 * @brief Compute the center of the surrounding circle of \p t .
 * @param[in] t The triangle which you want the center of the surrounding circle.
 * @return The center of the surrounding circle in 2D (z = 0).
 */
Pvertex3D centerSurroundingCircle2D(const Ptriangle3D& t);

/**
 * @brief Check if the triangle made with <b>v1, v2, v3</b> is well oriented.
 * @param v1 The first  vertex to check with.
 * @param v2 The second vertex to check with.
 * @param v3 The third  vertex to check with.
 * @return true if it's well oriented, false otherwise.
 */
bool isWellOriented(const Pvertex3D& v1, const Pvertex3D& v2, const Pvertex3D& v3);

/**
 * @brief Check if \b tr is considered as a poor quality triangle, that means with an angle inferior to 
 * \b angleThreshold.
 * @param[in] tr             The triangle to test.
 * @param[in] angleThreshold The threshold for quality evaluation.
 * @return true if \b tr gots a bad angle, false otherwise.
 */
bool isPoorQuality(const Ptriangle3D& tr, double angleThreshold);

/**
 * @brief Check if \b test is inside a circle of diametral made by a-->b
 * @param a The first  point to get the diametral.
 * @param b The second point to get the diametral.
 * @param t The vertex to test.
 * @return true if \b test is inside
 */
bool isInCircleOfDiametral(const Pvertex3D& a, const Pvertex3D& b, const Pvertex3D& t);

/**
 * @brief Compute the barycentre of \b triangle.
 * @param triangle The triangle you wanna get the barycentre.
 * @return This centre as a vertex.
 */
Pvertex3D barycentre(const Ptriangle3D& triangle);

#endif
