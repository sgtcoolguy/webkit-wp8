/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "MoveOnly.h"
#include <wtf/Vector.h>

namespace TestWebKitAPI {

TEST(WTF_Vector, Basic)
{
    Vector<int> intVector;
    EXPECT_TRUE(intVector.isEmpty());
    EXPECT_EQ(0U, intVector.size());
    EXPECT_EQ(0U, intVector.capacity());
}

TEST(WTF_Vector, Iterator)
{
    Vector<int> intVector;
    intVector.append(10);
    intVector.append(11);
    intVector.append(12);
    intVector.append(13);

    Vector<int>::iterator it = intVector.begin();
    Vector<int>::iterator end = intVector.end();
    EXPECT_TRUE(end != it);

    EXPECT_EQ(10, *it);
    ++it;
    EXPECT_EQ(11, *it);
    ++it;
    EXPECT_EQ(12, *it);
    ++it;
    EXPECT_EQ(13, *it);
    ++it;

    EXPECT_TRUE(end == it);
}

TEST(WTF_Vector, OverloadedOperatorAmpersand)
{
    struct Test {
    private:
        Test* operator&();
    };

    Vector<Test> vector;
    vector.append(Test());
}

TEST(WTF_Vector, AppendLast)
{
    Vector<unsigned> vector;
    vector.append(0);

    // FIXME: This test needs to be run with GuardMalloc to show the bug.
    for (size_t i = 0; i < 100; ++i)
        vector.append(const_cast<const unsigned&>(vector.last()));
}

TEST(WTF_Vector, InitializerList)
{
    Vector<int> vector = { 1, 2, 3, 4 };
    EXPECT_EQ(4U, vector.size());

    EXPECT_EQ(1, vector[0]);
    EXPECT_EQ(2, vector[1]);
    EXPECT_EQ(3, vector[2]);
    EXPECT_EQ(4, vector[3]);
}

TEST(WTF_Vector, Reverse)
{
    Vector<int> intVector;
    intVector.append(10);
    intVector.append(11);
    intVector.append(12);
    intVector.append(13);
    intVector.reverse();

    EXPECT_EQ(13, intVector[0]);
    EXPECT_EQ(12, intVector[1]);
    EXPECT_EQ(11, intVector[2]);
    EXPECT_EQ(10, intVector[3]);

    intVector.append(9);
    intVector.reverse();

    EXPECT_EQ(9, intVector[0]);
    EXPECT_EQ(10, intVector[1]);
    EXPECT_EQ(11, intVector[2]);
    EXPECT_EQ(12, intVector[3]);
    EXPECT_EQ(13, intVector[4]);
}

TEST(WTF_Vector, ReverseIterator)
{
    Vector<int> intVector;
    intVector.append(10);
    intVector.append(11);
    intVector.append(12);
    intVector.append(13);

    Vector<int>::reverse_iterator it = intVector.rbegin();
    Vector<int>::reverse_iterator end = intVector.rend();
    EXPECT_TRUE(end != it);

    EXPECT_EQ(13, *it);
    ++it;
    EXPECT_EQ(12, *it);
    ++it;
    EXPECT_EQ(11, *it);
    ++it;
    EXPECT_EQ(10, *it);
    ++it;

    EXPECT_TRUE(end == it);
}

TEST(WTF_Vector, MoveOnly_UncheckedAppend)
{
    Vector<MoveOnly> vector;

    vector.reserveInitialCapacity(100);
    for (size_t i = 0; i < 100; ++i) {
        MoveOnly moveOnly(i);
        vector.uncheckedAppend(std::move(moveOnly));
        EXPECT_EQ(0U, moveOnly.value());
    }

    for (size_t i = 0; i < 100; ++i)
        EXPECT_EQ(i, vector[i].value());
}

TEST(WTF_Vector, MoveOnly_Append)
{
    Vector<MoveOnly> vector;

    for (size_t i = 0; i < 100; ++i) {
        MoveOnly moveOnly(i);
        vector.append(std::move(moveOnly));
        EXPECT_EQ(0U, moveOnly.value());
    }

    for (size_t i = 0; i < 100; ++i)
        EXPECT_EQ(i, vector[i].value());

    for (size_t i = 0; i < 16; ++i) {
        Vector<MoveOnly> vector;

        vector.append(i);

        for (size_t j = 0; j < i; ++j)
            vector.append(j);
        vector.append(std::move(vector[0]));

        EXPECT_EQ(0U, vector[0].value());

        for (size_t j = 0; j < i; ++j)
            EXPECT_EQ(j, vector[j + 1].value());
        EXPECT_EQ(i, vector.last().value());
    }
}

TEST(WTF_Vector, MoveOnly_Insert)
{
    Vector<MoveOnly> vector;

    for (size_t i = 0; i < 100; ++i) {
        MoveOnly moveOnly(i);
        vector.insert(0, std::move(moveOnly));
        EXPECT_EQ(0U, moveOnly.value());
    }

    EXPECT_EQ(vector.size(), 100U);
    for (size_t i = 0; i < 100; ++i)
        EXPECT_EQ(99 - i, vector[i].value());

    for (size_t i = 0; i < 200; i += 2) {
        MoveOnly moveOnly(1000 + i);
        vector.insert(i, std::move(moveOnly));
        EXPECT_EQ(0U, moveOnly.value());
    }

    EXPECT_EQ(200U, vector.size());
    for (size_t i = 0; i < 200; ++i) {
        if (i % 2)
            EXPECT_EQ(99 - i / 2, vector[i].value());
        else
            EXPECT_EQ(1000 + i, vector[i].value());
    }
}

TEST(WTF_Vector, MoveOnly_TakeLast)
{
    Vector<MoveOnly> vector;

    for (size_t i = 0; i < 100; ++i) {
        MoveOnly moveOnly(i);
        vector.append(std::move(moveOnly));
        EXPECT_EQ(0U, moveOnly.value());
    }

    EXPECT_EQ(100U, vector.size());
    for (size_t i = 0; i < 100; ++i)
        EXPECT_EQ(99 - i, vector.takeLast().value());

    EXPECT_EQ(0U, vector.size());
}

TEST(WTF_Vector, VectorOfVectorsOfVectorsInlineCapacitySwap)
{
    Vector<Vector<Vector<int, 1>, 1>, 1> a;
    Vector<Vector<Vector<int, 1>, 1>, 1> b;
    Vector<Vector<Vector<int, 1>, 1>, 1> c;

    EXPECT_EQ(0U, a.size());
    EXPECT_EQ(0U, b.size());
    EXPECT_EQ(0U, c.size());

    Vector<int, 1> x;
    x.append(42);

    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    
    Vector<Vector<int, 1>, 1> y;
    y.append(x);
    
    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    EXPECT_EQ(1U, y.size());
    EXPECT_EQ(1U, y[0].size());
    EXPECT_EQ(42, y[0][0]);
    
    a.append(y);

    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    EXPECT_EQ(1U, y.size());
    EXPECT_EQ(1U, y[0].size());
    EXPECT_EQ(42, y[0][0]);
    EXPECT_EQ(1U, a.size());
    EXPECT_EQ(1U, a[0].size());
    EXPECT_EQ(1U, a[0][0].size());
    EXPECT_EQ(42, a[0][0][0]);
    
    a.swap(b);

    EXPECT_EQ(0U, a.size());
    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    EXPECT_EQ(1U, y.size());
    EXPECT_EQ(1U, y[0].size());
    EXPECT_EQ(42, y[0][0]);
    EXPECT_EQ(1U, b.size());
    EXPECT_EQ(1U, b[0].size());
    EXPECT_EQ(1U, b[0][0].size());
    EXPECT_EQ(42, b[0][0][0]);
    
    b.swap(c);

    EXPECT_EQ(0U, a.size());
    EXPECT_EQ(0U, b.size());
    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    EXPECT_EQ(1U, y.size());
    EXPECT_EQ(1U, y[0].size());
    EXPECT_EQ(42, y[0][0]);
    EXPECT_EQ(1U, c.size());
    EXPECT_EQ(1U, c[0].size());
    EXPECT_EQ(1U, c[0][0].size());
    EXPECT_EQ(42, c[0][0][0]);
    
    y[0][0] = 24;

    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    EXPECT_EQ(1U, y.size());
    EXPECT_EQ(1U, y[0].size());
    EXPECT_EQ(24, y[0][0]);
    
    a.append(y);

    EXPECT_EQ(1U, x.size());
    EXPECT_EQ(42, x[0]);
    EXPECT_EQ(1U, y.size());
    EXPECT_EQ(1U, y[0].size());
    EXPECT_EQ(24, y[0][0]);
    EXPECT_EQ(1U, a.size());
    EXPECT_EQ(1U, a[0].size());
    EXPECT_EQ(1U, a[0][0].size());
    EXPECT_EQ(24, a[0][0][0]);
    EXPECT_EQ(1U, c.size());
    EXPECT_EQ(1U, c[0].size());
    EXPECT_EQ(1U, c[0][0].size());
    EXPECT_EQ(42, c[0][0][0]);
    EXPECT_EQ(0U, b.size());
}

} // namespace TestWebKitAPI
