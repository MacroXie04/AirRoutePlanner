#ifndef TEST_QUEUE_H
#define TEST_QUEUE_H

#include <igloo/igloo.h>
#include <Queue.h>

using namespace igloo;

// =============================================================================
// Queue Tests
// Tests for FIFO (First-In-First-Out) queue implementation
// =============================================================================
Context(QueueTests) {
    Queue<int> q;

    void SetUp() {
        q = Queue<int>();
    }

    Spec(InitiallyEmpty) {
        Assert::That(q.isEmpty(), IsTrue());
        Assert::That(q.size(), Equals(0));
    }

    Spec(EnqueueIncreasesSize) {
        q.enqueue(10);
        Assert::That(q.size(), Equals(1));
        Assert::That(q.isEmpty(), IsFalse());
    }

    Spec(DequeueReturnsFirstIn) {
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        Assert::That(q.dequeue(), Equals(10));
        Assert::That(q.dequeue(), Equals(20));
        Assert::That(q.dequeue(), Equals(30));
    }

    Spec(PeekReturnsWithoutRemoving) {
        q.enqueue(10);
        q.enqueue(20);
        Assert::That(q.peek(), Equals(10));
        Assert::That(q.size(), Equals(2));
    }

    Spec(FIFOOrder) {
        for (int i = 1; i <= 5; i++) {
            q.enqueue(i);
        }
        for (int i = 1; i <= 5; i++) {
            Assert::That(q.dequeue(), Equals(i));
        }
    }

    Spec(EmptyAfterAllDequeued) {
        q.enqueue(10);
        q.dequeue();
        Assert::That(q.isEmpty(), IsTrue());
    }
};

#endif // TEST_QUEUE_H

