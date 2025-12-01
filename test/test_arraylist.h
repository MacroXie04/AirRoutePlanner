#ifndef TEST_ARRAYLIST_H
#define TEST_ARRAYLIST_H

#include <igloo/igloo.h>
#include <ArrayList.h>

using namespace igloo;

// =============================================================================
// ArrayList Tests
// Tests for dynamic array implementation with automatic resizing
// =============================================================================
Context(ArrayListTests) {
    ArrayList<int> list;

    void SetUp() {
        list = ArrayList<int>();
    }

    Spec(InitiallyEmpty) {
        Assert::That(list.size(), Equals(0));
    }

    Spec(AppendIncreasesSize) {
        list.append(10);
        list.append(20);
        list.append(30);
        Assert::That(list.size(), Equals(3));
    }

    Spec(AppendAddsToEnd) {
        list.append(10);
        list.append(20);
        list.append(30);
        Assert::That(list[0], Equals(10));
        Assert::That(list[1], Equals(20));
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
        list.append(30);
        int removed = list.removeFirst();
        Assert::That(removed, Equals(10));
        Assert::That(list.size(), Equals(2));
        Assert::That(list[0], Equals(20));
    }

    Spec(RemoveLastReturnsAndRemoves) {
        list.append(10);
        list.append(20);
        list.append(30);
        int removed = list.removeLast();
        Assert::That(removed, Equals(30));
        Assert::That(list.size(), Equals(2));
        Assert::That(list[1], Equals(20));
    }

    Spec(SearchFindsExistingElement) {
        list.append(10);
        list.append(20);
        list.append(30);
        Assert::That(list.search(20), IsTrue());
    }

    Spec(SearchReturnsFalseForMissing) {
        list.append(10);
        list.append(20);
        Assert::That(list.search(99), IsFalse());
    }

    Spec(InsertAtIndex) {
        list.append(10);
        list.append(30);
        list.insert(1, 20);
        Assert::That(list.size(), Equals(3));
        Assert::That(list[0], Equals(10));
        Assert::That(list[1], Equals(20));
        Assert::That(list[2], Equals(30));
    }

    Spec(InsertAtBeginning) {
        list.append(20);
        list.append(30);
        list.insert(0, 10);
        Assert::That(list[0], Equals(10));
    }

    Spec(InsertAtEnd) {
        list.append(10);
        list.append(20);
        list.insert(2, 30);
        Assert::That(list[2], Equals(30));
    }

    Spec(FindMinReturnsSmallest) {
        list.append(30);
        list.append(10);
        list.append(20);
        Assert::That(list.findMin(), Equals(10));
    }

    Spec(IndexOutOfBoundsThrows) {
        list.append(10);
        AssertThrows(std::logic_error, list[5]);
    }

    Spec(NegativeIndexThrows) {
        list.append(10);
        AssertThrows(std::logic_error, list[-1]);
    }

    Spec(RemoveFromEmptyThrows) {
        AssertThrows(std::logic_error, list.removeFirst());
    }

    Spec(RemoveLastFromEmptyThrows) {
        AssertThrows(std::logic_error, list.removeLast());
    }

    Spec(InsertOutOfBoundsThrows) {
        AssertThrows(std::logic_error, list.insert(5, 10));
    }

    Spec(CopyConstructor) {
        list.append(10);
        list.append(20);
        ArrayList<int> copy(list);
        Assert::That(copy.size(), Equals(2));
        Assert::That(copy[0], Equals(10));
        Assert::That(copy[1], Equals(20));
    }

    Spec(AssignmentOperator) {
        list.append(10);
        list.append(20);
        ArrayList<int> other;
        other = list;
        Assert::That(other.size(), Equals(2));
        Assert::That(other[0], Equals(10));
    }

    Spec(CapacityGrowsOnInflate) {
        for (int i = 0; i < 10; i++) {
            list.append(i);
        }
        Assert::That(list.size(), Equals(10));
        Assert::That(list.getCapacity(), IsGreaterThan(10));
    }
};

#endif // TEST_ARRAYLIST_H

