#ifndef TEST_LINKEDLIST_H
#define TEST_LINKEDLIST_H

#include <igloo/igloo.h>
#include <LinkedList.h>

using namespace igloo;

// =============================================================================
// LinkedList Tests
// Tests for doubly-linked list implementation
// =============================================================================
Context(LinkedListTests) {
    LinkedList<int> list;

    void SetUp() {
        list = LinkedList<int>();
    }

    Spec(InitiallyEmpty) {
        Assert::That(list.size(), Equals(0));
    }

    Spec(AppendIncreasesSize) {
        list.append(10);
        list.append(20);
        Assert::That(list.size(), Equals(2));
    }

    Spec(AppendAddsToEnd) {
        list.append(10);
        list.append(20);
        list.append(30);
        Assert::That(list[0], Equals(10));
        Assert::That(list[2], Equals(30));
    }

    Spec(PrependAddsToFront) {
        list.append(20);
        list.prepend(10);
        Assert::That(list[0], Equals(10));
        Assert::That(list[1], Equals(20));
    }

    Spec(RemoveFirstReturnsAndRemoves) {
        list.append(10);
        list.append(20);
        int removed = list.removeFirst();
        Assert::That(removed, Equals(10));
        Assert::That(list.size(), Equals(1));
        Assert::That(list[0], Equals(20));
    }

    Spec(RemoveLastReturnsAndRemoves) {
        list.append(10);
        list.append(20);
        int removed = list.removeLast();
        Assert::That(removed, Equals(20));
        Assert::That(list.size(), Equals(1));
        Assert::That(list[0], Equals(10));
    }

    Spec(RemoveFromSingleElementList) {
        list.append(10);
        int removed = list.removeFirst();
        Assert::That(removed, Equals(10));
        Assert::That(list.size(), Equals(0));
    }

    Spec(RemoveLastFromSingleElementList) {
        list.append(10);
        int removed = list.removeLast();
        Assert::That(removed, Equals(10));
        Assert::That(list.size(), Equals(0));
    }

    Spec(IndexOutOfBoundsThrows) {
        list.append(10);
        AssertThrows(std::logic_error, list[5]);
    }

    Spec(RemoveFromEmptyThrows) {
        AssertThrows(std::logic_error, list.removeFirst());
    }

    Spec(RemoveLastFromEmptyThrows) {
        AssertThrows(std::logic_error, list.removeLast());
    }

    Spec(CopyConstructor) {
        list.append(10);
        list.append(20);
        LinkedList<int> copy(list);
        Assert::That(copy.size(), Equals(2));
        Assert::That(copy[0], Equals(10));
        Assert::That(copy[1], Equals(20));
    }

    Spec(AssignmentOperator) {
        list.append(10);
        list.append(20);
        LinkedList<int> other;
        other = list;
        Assert::That(other.size(), Equals(2));
        Assert::That(other[0], Equals(10));
    }

    Spec(MultipleOperations) {
        list.append(1);
        list.append(2);
        list.prepend(0);
        list.removeFirst();
        list.removeLast();
        Assert::That(list.size(), Equals(1));
        Assert::That(list[0], Equals(1));
    }
};

#endif // TEST_LINKEDLIST_H

