// Author: Przemysław Bągard

#ifndef TESTS_H
#define TESTS_H

#include "Collision.h"
#include "CollisionSystem.h"
#include <time.h>
#include "assert.h"
#include "generator.h"

using namespace EnvelopeGeometry;

/// collision detection test
/// 1 000 000 calls of collision test; every function call add up to test time
#define TEST_BEGIN() { \
    clock_t collisionTimeStart; \
    clock_t collisionTimeEnd; \
    clock_t diff = 0; \
    for (int l1 = 0; l1 < 1000000; ++l1) {


#define TEST_TIME() collisionTimeStart = clock();

#define TEST_END() collisionTimeEnd = clock(); \
    diff += ((collisionTimeEnd - collisionTimeStart)*1000)/CLOCKS_PER_SEC; \
    } \
    std::cout << diff  << "\n"; \
}

/// pre-filter test
/// - time of pre-filter function calls
/// - quantity of objects accepted by pre-filter test
/// - time of pre-filter function calls + collision detection call time for accepted objects
/// 1 000 000 calls of collision test; every function call add up to test time
#define TEST_FILTER_BEGIN() { \
    clock_t collisionTimeFilterStart; \
    clock_t collisionTimeFilterEnd; \
    clock_t collisionTimeDetectionStart; \
    clock_t collisionTimeDetectionEnd; \
    int filtred = 0; \
    clock_t diffFilter = 0; \
    clock_t diffDetected = 0; \
    bool ok = false; \
    for (int l1 = 0; l1 < 1000000; ++l1) {


#define TEST_FILTER_TIME() collisionTimeFilterStart = clock();

#define TEST_FILTER_DETECTION() collisionTimeFilterEnd = clock(); \
    if(ok) { \
        ++filtred; \
        collisionTimeDetectionStart = clock();

#define TEST_FILTER_END() collisionTimeDetectionEnd = clock(); \
    } \
    diffFilter += ((collisionTimeFilterEnd - collisionTimeFilterStart)*1000)/CLOCKS_PER_SEC; \
    if(ok) diffDetected += ((collisionTimeDetectionEnd - collisionTimeDetectionStart)*1000)/CLOCKS_PER_SEC; \
    } \
    std::cout << diffFilter << "\ttotal:\t" << diffFilter+diffDetected << "\tdetected:\t" << filtred << "\n"; \
}





void testMain(void (*generate)(PKT&, PKT&, PKT&, PKT&, PKT&, PKT&, PKT&, float&))
{
    PKT A, B, C, vec, D, E, F;
    float rr;

    std::cout << "\nPointPoint:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangePointPoint(A, vec,D, rr);
    TEST_END();

    std::cout << "\nPointSegment:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangePointSegment(A, vec, Segment(D,E), rr);
    TEST_END();

    std::cout << "\nPointTriangle:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangePointTriangle(A, vec, Triangle(D,E,F), rr);
    TEST_END();

    std::cout << "\nSegmentPoint:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangeSegmentPoint(Segment(A,B), vec, D, rr);
    TEST_END();

    std::cout << "\nSegmentSegment:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangeSegmentSegment(Segment(A,B), vec, Segment(D,E), rr);
    TEST_END();

    std::cout << "\nSegmentTriangle:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangeSegmentTriangle(Segment(A,B), vec, Triangle(D,E,F), rr);
    TEST_END();

    std::cout << "\nTrianglePoint:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangeTrianglePoint(Triangle(A,B,C), vec, D, rr);
    TEST_END();

    std::cout << "\nTriangleSegment:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangeTriangleSegment(Triangle(A,B,C), vec, Segment(D,E), rr);
    TEST_END();

    /// count how many times object is on collision trajectory
    int count = 0;
    std::cout << "\nTriangleTriangle:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    count += (int)EnvelopeCollide::Functions::pierceRangeTriangleTriangle(Triangle(A,B,C), vec, Triangle(D,E,F), rr).onCollisionTrajectory;
    TEST_END();
    std::cout << "\nTriangleTriangleIle:\t\t" << count;
}

void testDeprecated(void (*generate)(PKT&, PKT&, PKT&, PKT&, PKT&, PKT&, PKT&, float&))
{
    PKT A, B, C, vec, D, E, F;
    float rr;

    std::cout << "\nTriangleTriangleDeprecated:\t";
    TEST_BEGIN();
    generate(A, B, C, vec, D, E, F, rr);
    TEST_TIME();
    EnvelopeCollide::Functions::pierceRangeTriangleTriangleDeprecated(Triangle(A,B,C), vec, Triangle(D,E,F), rr);
    TEST_END();
}

void testFilters(void (*generate)(PKT&, PKT&, PKT&, PKT&, PKT&, PKT&, PKT&, float&))
{
    PKT A, B, C, vec, D, E, F, A2, B2, C2, vec2;
    float rr;

    // generate data for first object
    generate(A, B, C, vec, D, E, F, rr);
    float rr2 = rr;

    std::cout << "\n";

    // intersected planes (a) - parallel to movement vector (similar to OBB)
    {
        // preprocessing
        Projector proj =  EnvelopeCollide::Functions::makeCubeProjectorFromMoveVector(vec);
        PKT min = Punkt(INFINITY, INFINITY, INFINITY);
        PKT max = -min;
        EnvelopeCollide::Functions::getProjectorMinMax(proj, A, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, B, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, C, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, A+vec, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, B+vec, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, C+vec, min, max);

        // in every test
        std::cout << "\nTafle (a) - FilterTriangleAndExecute:\t";
        TEST_FILTER_BEGIN();
        generate(A2, B2, C2, vec2, D, E, F, rr);
        TEST_FILTER_TIME() ok = EnvelopeCollide::Functions::checkMoveVectorProjectorFilterTriangle(proj, min, max, rr, Triangle(D, E, F));
        TEST_FILTER_DETECTION();
        EnvelopeCollide::Functions::pierceRangeTriangleTriangle(Triangle(A,B,C), vec, Triangle(D,E,F), rr);
        TEST_FILTER_END();
    }

    // intersected planes (b) - parallel to coordinate system (similar to AABB)
    {
        // preprocessing
        rr = rr2;
        Projector proj =  Projector(PKT0, Punkt(1.0f, 0.0f, 0.0f), Punkt(0.0f, 1.0f, 0.0f), Punkt(0.0f, 0.0f, 1.0f));//EnvelopeCollide::Functions::makeCubeProjectorFromMoveVector(vec);
        PKT min = Punkt(INFINITY, INFINITY, INFINITY);
        PKT max = -min;
        EnvelopeCollide::Functions::getProjectorMinMax(proj, A, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, B, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, C, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, A+vec, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, B+vec, min, max);
        EnvelopeCollide::Functions::getProjectorMinMax(proj, C+vec, min, max);

        // in every test
        std::cout << "\nTafle (b) - FilterTriangleAndExecute:\t";
        TEST_FILTER_BEGIN();
        generate(A2, B2, C2, vec2, D, E, F, rr);
        TEST_FILTER_TIME() ok = EnvelopeCollide::Functions::checkSimpleProjectorFilterTriangle(min, max, rr, Triangle(D, E, F));
        TEST_FILTER_DETECTION();
        EnvelopeCollide::Functions::pierceRangeTriangleTriangle(Triangle(A,B,C), vec, Triangle(D,E,F), rr);
        TEST_FILTER_END();
    }


    // sphere (a)
    {
        // preprocessing
        EnvelopeCollide::Mesh source1, source2;
        EnvelopeCollide::Mesh mesh1, mesh2;
        source1.type = EnvelopeCollide::TRIANGLE;
        source1.p[0] = A;
        source1.p[1] = B;
        source1.p[2] = C;
        source1.r = rr2/2.0f;
        EnvelopeCollide::Functions::createBoundingSphereSimple(mesh1, source1);

        // in every test
        std::cout << "\nSphere (a) - FilterTriangleAndExecute:\t";
        TEST_FILTER_BEGIN();
        generate(A2, B2, C2, vec2, D, E, F, rr);
        source2.type = EnvelopeCollide::TRIANGLE;
        source2.p[0] = D;
        source2.p[1] = E;
        source2.p[2] = F;
        source2.r = rr/2.0f;

        EnvelopeCollide::Functions::PierceRangeResult result;
        float rr4 = rr+rr2;
        TEST_FILTER_TIME()
                EnvelopeCollide::Functions::createBoundingSphereSimple(mesh2, source2);
                result = EnvelopeCollide::Functions::pierceRangePointPoint(mesh1.p[0], vec, mesh2.p[0],  mesh1.r + mesh2.r);
                ok = result.onCollisionTrajectory && result.vMinDistance <= 1.0f && result.vMaxDistance >= 0.0f;
        TEST_FILTER_DETECTION();
        EnvelopeCollide::Functions::pierceRangeTriangleTriangle(Triangle(A,B,C), vec, Triangle(D,E,F), rr4);
        TEST_FILTER_END();
    }

    // sphere (b) - minimal radius
    {
        // preprocessing
        EnvelopeCollide::Mesh source1, source2;
        EnvelopeCollide::Mesh mesh1, mesh2;
        source1.type = EnvelopeCollide::TRIANGLE;
        source1.p[0] = A;
        source1.p[1] = B;
        source1.p[2] = C;
        source1.r = rr2/2.0f;
        EnvelopeCollide::Functions::createBoundingSphere(mesh1, source1);

        // in every test
        std::cout << "\nSphere (b) - FilterTriangleAndExecute:\t";
        TEST_FILTER_BEGIN();
        generate(A2, B2, C2, vec2, D, E, F, rr);
        source2.type = EnvelopeCollide::TRIANGLE;
        source2.p[0] = D;
        source2.p[1] = E;
        source2.p[2] = F;
        source2.r = rr/2.0f;

        EnvelopeCollide::Functions::PierceRangeResult result;
        float rr4 = rr+rr2;
        TEST_FILTER_TIME()
                EnvelopeCollide::Functions::createBoundingSphere(mesh2, source2);
                result = EnvelopeCollide::Functions::pierceRangePointPoint(mesh1.p[0], vec, mesh2.p[0],  mesh1.r + mesh2.r);
                ok = result.onCollisionTrajectory && result.vMinDistance <= 1.0f && result.vMaxDistance >= 0.0f;
        TEST_FILTER_DETECTION();
        EnvelopeCollide::Functions::pierceRangeTriangleTriangle(Triangle(A,B,C), vec, Triangle(D,E,F), rr4);
        TEST_FILTER_END();
    }
}


#endif // TESTS_H
