// Author: Przemysław Bągard

#ifndef GENERATOR_H
#define GENERATOR_H

using namespace EnvelopeGeometry;

// default multipler for generating points coordinates
const float defMultipler = 20.0f;

// generate value in the range [0, 1]
inline float randomPercent()
{
    return (float)rand()/(float)RAND_MAX;
}

// generate point coordinates
PKT prand(const float& multipler = defMultipler)
{
    return Punkt(randomPercent(), randomPercent(), randomPercent())*multipler;
}

// random situation
inline void generateRandom(PKT& A, PKT& B, PKT& C, PKT& vec, PKT& D, PKT& E, PKT& F, float& rr)
{
    A = prand();
    B = prand();
    C = prand();
    while(vec == PKT0)
        vec = prand();
    D = prand();
    E = prand();
    F = prand();
    rr = randomPercent()*defMultipler;
}

// random situation, where second object points are moved by vector
inline void generateRandomMove(PKT& A, PKT& B, PKT& C, PKT& vec, PKT& D, PKT& E, PKT& F, float& rr)
{
    A = prand();
    B = prand();
    C = prand();
    while(vec == PKT0)
        vec = prand();
    D = prand()+Punkt(30,20,20);
    E = prand()+Punkt(30,20,20);
    F = prand()+Punkt(30,20,20);
    rr = randomPercent()*defMultipler;
}

// random situation, where object has movement vector with small length
inline void generateRandomSmallVec(PKT& A, PKT& B, PKT& C, PKT& vec, PKT& D, PKT& E, PKT& F, float& rr)
{
    A = prand(3.0f);
    B = prand(3.0f);
    C = prand(3.0f);
    while(vec == PKT0)
        vec = prand();
    vec = setLengthToVector(vec);
    PKT move = prand(10.0f) - Punkt(5,5,5);
    D = move+prand(3.0f);
    E = move+prand(3.0f);
    F = move+prand(3.0f);
    rr = randomPercent()*2.0f;
}

// random situation, where object has movement vector with very small length
// (movement of character in real-time - with 50 fps, character moves by vec*0.02f )
inline void generateRandomVerySmallVec(PKT& A, PKT& B, PKT& C, PKT& vec, PKT& D, PKT& E, PKT& F, float& rr)
{
    A = prand(3.0f);
    B = prand(3.0f);
    C = prand(3.0f);
    while(vec == PKT0)
        vec = prand();
    vec = setLengthToVector(vec, 0.05f);
    PKT move = prand(10.0f) - Punkt(5,5,5);
    D = move+prand(3.0f);
    E = move+prand(3.0f);
    F = move+prand(3.0f);
    rr = randomPercent()*2.0f;
}

#endif // GENERATOR_H
