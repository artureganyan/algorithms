#include "utils.h"

namespace test_utils {

void test_is_balanced_search_tree()
{
    struct TreeNode
    {
        int       val;
        TreeNode* left;
        TreeNode* right;
    };

    // Trivial
    ASSERT( is_balanced_search_tree<TreeNode>(
                nullptr
            ) == false );

    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1}
            ).data()) == true );

    // Duplicated nodes allowed
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{1},
                    nullptr
                }
            ).data(), true) == true );

    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    nullptr,
                    new TreeNode{1}
                }
            ).data(), true) == true );

    // Duplicated nodes not allowed
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{1},
                    nullptr
                }
            ).data(), false) == false );

    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    nullptr,
                    new TreeNode{1}
                }
            ).data(), false) == false );

    // Wrong order
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{2},
                    nullptr
                }
            ).data()) == false );

    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    nullptr,
                    new TreeNode{0}
                }
            ).data()) == false );

    // Correct order
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{0},
                    nullptr
                }
            ).data()) == true );

    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    nullptr,
                    new TreeNode{2}
                }
            ).data()) == true );

    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{2,
                    new TreeNode{1},
                    new TreeNode{3}
                }
            ).data()) == true );

    // Correct + wrong order
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{2,
                    new TreeNode{1},
                    new TreeNode{1}
                }
            ).data()) == false );

    // Wrong order found on deeper level
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{2,
                    new TreeNode{1},
                    new TreeNode{3,
                        new TreeNode{1},
                        nullptr
                    }
                }
            ).data()) == false );

    // Correct order + correct delta in subtrees' height
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{2,
                    new TreeNode{1},
                    new TreeNode{4,
                        new TreeNode{3},
                        nullptr
                    }
                }
            ).data()) == true );

    // Correct order + wrong delta in subtrees' height
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{2,
                    new TreeNode{1},
                    new TreeNode{4,
                        new TreeNode{3},
                        new TreeNode{5,
                            nullptr,
                            new TreeNode{6}
                        }
                    }
                }
            ).data()) == false );

    // Correct order + correct delta in subtrees' height, for more levels
    ASSERT( is_balanced_search_tree(TemporaryTree<TreeNode>(
                new TreeNode{2,
                    new TreeNode{1,
                        new TreeNode{0},
                        nullptr
                    },
                    new TreeNode{4,
                        new TreeNode{3},
                        new TreeNode{5,
                            nullptr,
                            new TreeNode{6}
                        }
                    }
                }
            ).data()) == true );
}

int main()
{
    test_is_balanced_search_tree();

    return 0;
}

}
