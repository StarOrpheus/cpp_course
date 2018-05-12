[![Build 
Status](https://travis-ci.org/StarOrpheus/cpp_course.svg?branch=master)](https://travis-ci.org/StarOrpheus/cpp_course)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/15350/badge.svg?flat=1)](https://scan.coverity.com/projects/starorpheus-cpp_course)

### Build for big_integer:
From /big_integer:

```
cmake -D CMAKE_BUILD_TYPE:STRING=Release .
cmake --build . -- -j2
./big_integer_testing
```