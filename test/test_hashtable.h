#ifndef TEST_HASHTABLE_H
#define TEST_HASHTABLE_H

#include <igloo/igloo.h>
#include <HashTable.h>

using namespace igloo;

// =============================================================================
// HashTable Tests (Integer)
// Tests for hash table with integer keys
// =============================================================================
Context(HashTableIntTests) {
    HashTable<int> ht;

    void SetUp() {
        ht = HashTable<int>(10);
    }

    Spec(InsertAndSearch) {
        ht.insert(42);
        Assert::That(ht.search(42), IsTrue());
    }

    Spec(SearchMissingReturnsFalse) {
        ht.insert(42);
        Assert::That(ht.search(99), IsFalse());
    }

    Spec(MultipleInserts) {
        ht.insert(1);
        ht.insert(2);
        ht.insert(3);
        Assert::That(ht.search(1), IsTrue());
        Assert::That(ht.search(2), IsTrue());
        Assert::That(ht.search(3), IsTrue());
    }

    Spec(HandleCollisions) {
        // Insert values that map to the same bucket (depends on hash function)
        ht.insert(0);
        ht.insert(10);
        ht.insert(20);
        Assert::That(ht.search(0), IsTrue());
        Assert::That(ht.search(10), IsTrue());
        Assert::That(ht.search(20), IsTrue());
    }

    Spec(NegativeNumbers) {
        ht.insert(-5);
        ht.insert(-10);
        Assert::That(ht.search(-5), IsTrue());
        Assert::That(ht.search(-10), IsTrue());
    }
};

// =============================================================================
// HashTable Tests (String)
// Tests for hash table with string keys
// =============================================================================
Context(HashTableStringTests) {
    HashTable<std::string> ht;

    void SetUp() {
        ht = HashTable<std::string>(10);
    }

    Spec(InsertAndSearchString) {
        ht.insert("hello");
        Assert::That(ht.search("hello"), IsTrue());
    }

    Spec(SearchMissingString) {
        ht.insert("hello");
        Assert::That(ht.search("world"), IsFalse());
    }

    Spec(MultipleStrings) {
        ht.insert("apple");
        ht.insert("banana");
        ht.insert("cherry");
        Assert::That(ht.search("apple"), IsTrue());
        Assert::That(ht.search("banana"), IsTrue());
        Assert::That(ht.search("cherry"), IsTrue());
    }

    Spec(EmptyString) {
        ht.insert("");
        Assert::That(ht.search(""), IsTrue());
    }
};

#endif // TEST_HASHTABLE_H

