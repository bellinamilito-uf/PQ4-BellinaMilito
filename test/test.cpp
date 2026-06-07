#include <catch2/catch_test_macros.hpp>
#include "GatorBST.h"

using namespace std;

TEST_CASE("Insert into empty tree", "[insert]") {
    GatorBST bst;
    REQUIRE(bst.Insert(10, "Alice") == true);
}

TEST_CASE("Insert multiple nodes", "[insert]") {
    GatorBST bst;
    REQUIRE(bst.Insert(10, "A"));
    REQUIRE(bst.Insert(5, "B"));
    REQUIRE(bst.Insert(15, "C"));
}

TEST_CASE("Insert duplicate UFID fails", "[insert]") {
    GatorBST bst;
    REQUIRE(bst.Insert(10, "A"));
    REQUIRE(bst.Insert(10, "B") == false);
}

TEST_CASE("Insert left and right children", "[insert]") {
    GatorBST bst;
    bst.Insert(10, "A");
    REQUIRE(bst.Insert(5, "B"));
    REQUIRE(bst.Insert(15, "C"));
}

// ------------------------------------------------------------
// SEARCH ID TESTS
// ------------------------------------------------------------

TEST_CASE("SearchID finds correct name", "[searchID]") {
    GatorBST bst;
    bst.Insert(10, "Alice");
    bst.Insert(20, "Bob");

    auto r = bst.SearchID(20);
    REQUIRE(r.has_value());
    REQUIRE(r.value() == "Bob");
}

TEST_CASE("SearchID missing UFID returns nullopt", "[searchID]") {
    GatorBST bst;
    bst.Insert(10, "A");
    REQUIRE_FALSE(bst.SearchID(999).has_value());
}

TEST_CASE("SearchID root and leaf", "[searchID]") {
    GatorBST bst;
    bst.Insert(10, "Root");
    bst.Insert(5, "Leaf");

    REQUIRE(bst.SearchID(10).value() == "Root");
    REQUIRE(bst.SearchID(5).value() == "Leaf");
}

// ------------------------------------------------------------
// SEARCH NAME TESTS
// ------------------------------------------------------------

TEST_CASE("SearchName returns sorted UFIDs", "[searchName]") {
    GatorBST bst;
    bst.Insert(30, "Sam");
    bst.Insert(10, "Sam");
    bst.Insert(20, "Sam");

    auto ids = bst.SearchName("Sam");
    REQUIRE(ids == vector<int>{10, 20, 30});
}

TEST_CASE("SearchName empty result", "[searchName]") {
    GatorBST bst;
    REQUIRE(bst.SearchName("Nobody").empty());
}

TEST_CASE("SearchName single match", "[searchName]") {
    GatorBST bst;
    bst.Insert(42, "X");
    auto ids = bst.SearchName("X");
    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == 42);
}

// ------------------------------------------------------------
// REMOVE TESTS
// ------------------------------------------------------------

TEST_CASE("Remove leaf node", "[remove]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");

    REQUIRE(bst.Remove(5));
    REQUIRE_FALSE(bst.SearchID(5).has_value());
}

TEST_CASE("Remove node with one child", "[remove]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(2, "C");

    REQUIRE(bst.Remove(5));
    REQUIRE_FALSE(bst.SearchID(5).has_value());
}

TEST_CASE("Remove node with two children (successor)", "[remove]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(15, "C");
    bst.Insert(12, "D");
    bst.Insert(18, "E");

    REQUIRE(bst.Remove(15));
    REQUIRE_FALSE(bst.SearchID(15).has_value());
}

TEST_CASE("Remove root node", "[remove]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(15, "C");

    REQUIRE(bst.Remove(10));
    REQUIRE_FALSE(bst.SearchID(10).has_value());
}

TEST_CASE("Remove non-existent UFID", "[remove]") {
    GatorBST bst;
    bst.Insert(10, "A");
    REQUIRE(bst.Remove(999) == false);
}


TEST_CASE("Inorder traversal sorted", "[traversal]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(15, "C");

    auto nodes = bst.TraverseInorder();
    REQUIRE(nodes.size() == 3);
    REQUIRE(nodes[0]->ufid == 5);
    REQUIRE(nodes[1]->ufid == 10);
    REQUIRE(nodes[2]->ufid == 15);
}

TEST_CASE("Preorder traversal correct order", "[traversal]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(15, "C");

    auto nodes = bst.TraversePreorder();
    REQUIRE(nodes.size() == 3);
    REQUIRE(nodes[0]->ufid == 10);
    REQUIRE(nodes[1]->ufid == 5);
    REQUIRE(nodes[2]->ufid == 15);
}

TEST_CASE("Postorder traversal correct order", "[traversal]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(15, "C");

    auto nodes = bst.TraversePostorder();
    REQUIRE(nodes.size() == 3);
    REQUIRE(nodes[0]->ufid == 5);
    REQUIRE(nodes[1]->ufid == 15);
    REQUIRE(nodes[2]->ufid == 10);
}

TEST_CASE("Traversal on empty tree returns empty vector", "[traversal]") {
    GatorBST bst;
    REQUIRE(bst.TraverseInorder().empty());
    REQUIRE(bst.TraversePreorder().empty());
    REQUIRE(bst.TraversePostorder().empty());
}

// ------------------------------------------------------------
// HEIGHT TESTS
// ------------------------------------------------------------

TEST_CASE("Height of empty tree is 0", "[height]") {
    GatorBST bst;
    REQUIRE(bst.Height() == 0);
}

TEST_CASE("Height updates after inserts", "[height]") {
    GatorBST bst;
    bst.Insert(10, "A");
    REQUIRE(bst.Height() == 1);

    bst.Insert(5, "B");
    REQUIRE(bst.Height() == 2);

    bst.Insert(15, "C");
    REQUIRE(bst.Height() == 2);
}

TEST_CASE("Height updates after removals", "[height]") {
    GatorBST bst;
    bst.Insert(10, "A");
    bst.Insert(5, "B");
    bst.Insert(15, "C");

    REQUIRE(bst.Height() == 2);

    bst.Remove(5);
    REQUIRE(bst.Height() == 2);

    bst.Remove(15);
    REQUIRE(bst.Height() == 1);
}
