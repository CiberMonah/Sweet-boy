if(EXISTS "/home/andrew/Private/sems/sweet_boy/rbtree/build/tests/rbtree_tests[1]_tests.cmake")
  include("/home/andrew/Private/sems/sweet_boy/rbtree/build/tests/rbtree_tests[1]_tests.cmake")
else()
  add_test(rbtree_tests_NOT_BUILT rbtree_tests_NOT_BUILT)
endif()
