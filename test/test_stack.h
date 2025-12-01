#ifndef TEST_STACK_H
#define TEST_STACK_H

#include <igloo/igloo.h>
#include <Stack.h>

using namespace igloo;

// =============================================================================
// Stack Tests
// Tests for LIFO (Last-In-First-Out) stack implementation
// =============================================================================
Context(StackTests) {
    Stack<int> s;

    void SetUp() {
        s = Stack<int>();
    }

    Spec(InitiallyEmpty) {
        Assert::That(s.isEmpty(), IsTrue());
        Assert::That(s.size(), Equals(0));
    }

    Spec(PushIncreasesSize) {
        s.push(10);
        Assert::That(s.size(), Equals(1));
        Assert::That(s.isEmpty(), IsFalse());
    }

    Spec(PopReturnsLastIn) {
        s.push(10);
        s.push(20);
        s.push(30);
        Assert::That(s.pop(), Equals(30));
        Assert::That(s.pop(), Equals(20));
        Assert::That(s.pop(), Equals(10));
    }

    Spec(PeekReturnsWithoutRemoving) {
        s.push(10);
        s.push(20);
        Assert::That(s.peek(), Equals(20));
        Assert::That(s.size(), Equals(2));
    }

    Spec(LIFOOrder) {
        for (int i = 1; i <= 5; i++) {
            s.push(i);
        }
        for (int i = 5; i >= 1; i--) {
            Assert::That(s.pop(), Equals(i));
        }
    }

    Spec(EmptyAfterAllPopped) {
        s.push(10);
        s.pop();
        Assert::That(s.isEmpty(), IsTrue());
    }
};

#endif // TEST_STACK_H

