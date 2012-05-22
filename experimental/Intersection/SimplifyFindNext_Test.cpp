/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
 
#include "Simplify.h"

namespace SimplifyFindNextTest {

#include "Simplify.cpp"

} // end of SimplifyFindNextTest namespace

#include "Intersection_Tests.h"

static const SimplifyFindNextTest::Segment* testCommon(
        int winding, int startIndex, int endIndex,
        SkTArray<SimplifyFindNextTest::Contour>& contours,
        SimplifyFindNextTest::EdgeBuilder& builder, const SkPath& path) {
    SkTDArray<SimplifyFindNextTest::Contour*> contourList;
    makeContourList(contours, contourList);
    addIntersectTs(contourList[0], contourList[0], -1);
    if (contours.count() > 1) {
        SkASSERT(contours.count() == 2);
        addIntersectTs(contourList[0], contourList[1], -1);
        addIntersectTs(contourList[1], contourList[1], -1);
    }
    fixOtherTIndex(contourList);
    SimplifyFindNextTest::Segment& segment = contours[0].fSegments[0];
    SkPoint pts[2];
    double startT = segment.t(endIndex);
    segment.xyAtT(startT, &pts[0]);
    SimplifyFindNextTest::Segment* next = segment.findNext(winding,
            startIndex, endIndex);
    double endT = next->t(startIndex);
    next->xyAtT(endT, &pts[1]);
    SkASSERT(pts[0] == pts[1]);
    return next;
}

static void test(const SkPath& path) {
    SkTArray<SimplifyFindNextTest::Contour> contours;
    SimplifyFindNextTest::EdgeBuilder builder(path, contours);
    int winding = 0;
    int start = 0;
    int end = 1;
    testCommon(winding, start, end, contours, builder, path);
}

static void test(const SkPath& path, int start, int end) {
    SkTArray<SimplifyFindNextTest::Contour> contours;
    SimplifyFindNextTest::EdgeBuilder builder(path, contours);
    int winding = 0;
    testCommon(winding, start, end, contours, builder, path);
}

static void testLine1() {
    SkPath path;
    path.moveTo(2,0);
    path.lineTo(1,1);
    path.lineTo(0,0);
    path.close();
    test(path);
}

static void addInnerCWTriangle(SkPath& path) {
    path.moveTo(3,0);
    path.lineTo(4,1);
    path.lineTo(2,1);
    path.close();
}

static void addInnerCCWTriangle(SkPath& path) {
    path.moveTo(3,0);
    path.lineTo(2,1);
    path.lineTo(4,1);
    path.close();
}

static void addOuterCWTriangle(SkPath& path) {
    path.moveTo(3,0);
    path.lineTo(6,2);
    path.lineTo(0,2);
    path.close();
}

static void addOuterCCWTriangle(SkPath& path) {
    path.moveTo(3,0);
    path.lineTo(0,2);
    path.lineTo(6,2);
    path.close();
}

static void testLine2() {
    SkPath path;
    addInnerCWTriangle(path);
    addOuterCWTriangle(path);
    test(path, 0, 3);
}

static void testLine3() {
    SkPath path;
    addInnerCWTriangle(path);
    addOuterCWTriangle(path);
    test(path, 3, 0);
}

static void testLine4() {
    SkPath path;
    addInnerCWTriangle(path);
    addOuterCWTriangle(path);
    test(path, 3, 2);
}

static void (*tests[])() = {
    testLine1,
    testLine2,
    testLine3,
    testLine4,
};

static const size_t testCount = sizeof(tests) / sizeof(tests[0]);

static void (*firstTest)() = 0;
static bool skipAll = false;

void SimplifyFindNext_Test() {
    if (skipAll) {
        return;
    }
    size_t index = 0;
    if (firstTest) {
        while (index < testCount && tests[index] != firstTest) {
            ++index;
        }
    }
    bool firstTestComplete = false;
    for ( ; index < testCount; ++index) {
        (*tests[index])();
        firstTestComplete = true;
    }
}