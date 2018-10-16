hnStat Project
==================

# Presentation

hnStat searches a TSV file according to commands and criteria passed on the command line.

``` bash
./hnStat [distinct|top N] [--from TIMESTAMP] [--to TIMESTAMP] [file]
```

The size of the provided data file is considered to fit the memory.  

The code respects the C++11 specification.  
It has been developed and tested with g++ 5.4.0 on Linux x86_64 (Ubuntu 16.04).

## Complexity analysis
### Distinct query
* The data structure used is an `std::unordered_set<std::string>`, with the query used as key.  
An `std::unordered_set` was selected because:  
    - Its keys are unique  
    - Sorting is not necessary  
    - Its `insert()` method provides an average case time complexity of O(1) since it is implemented as an array of buckets, and O(n) in the worst case, with n the number of elements in the set  
    - We then just need a call to `size()` to get the number of distinct queries  

* Since the complete file needs to be parsed, there is a O(n) time complexity, n being the number of lines in the datafile.  
* The space complexity is O(m), m being the number of queries with a timestamp within the range since these are the only insertions in the set. If no timestamps are provided, then O(n) is the space complexity.  

### Top X query
* The logic implemented for this type of query is:
    - Use an `std::unordered_map<std::string, std::uint64_t>`, with the query used as key, and its number of occurence in the file as value  
    - Insert every query in the timestamp range in this associative container  
    - Transfer all its elements in an `std::multimap<std::uint64_t, std::string>`, so that the queries become the value, and number of occurence the key, with multiple keys allowed. The comparison function has to be `std::greater<std::uint64_t>` instead of the default `std::less<std::uint64_t>` so that the sorting is done on the number of occurence in the decreasing order
* The time complexity is O(n) with n the number of lines in the file, since it is required to go through each of them
* The space complexity is O(m), with m the number of queries matching the timestamp range provided. If no timestamps are provided, then O(n) is the space complexity.  

For both queries, storing timestamps is unnecessary as we insert only the queries which timestamp matches the range provided on the command line. 

## Multithreading
If the provided file's size is larger than `QueryProcessor::min_thread_part_size`, the operation of reading the file is split among several threads.  

`QueryProcessor::min_thread_part_size` has a default value of 4096, and can be modified in the arguments of `QueryProcessor::QueryProcessor()`, defined in `QueryProcessor.hh`  

The maximum number of threads is the number of online CPU cores.  

## Conception & code architecture

![UML class diagram](https://raw.githubusercontent.com/jgagnaire/hnStat/master/UML_class_diagram.png)

# Quick Start

## Cloning sources

``` bash
git clone https://github.com/jgagnaire/hnStat.git
```

## Building
``` bash
make
```
or
``` bash
make hnStat
```

# Examples

## Disinct query
``` bash
./hnStat distinct ./test_file.tsv
199
```
``` bash
./hnStat distinct --from 1438387227 --to 1438387498 ./test_file.tsv
174
```
``` bash
./hnStat distinct --from 1438387225 ./test_file.tsv
178
```

## Top X query
``` bash
./hnStat top 3 ./test_file.tsv
http%3A%2F%2Fwww.cbsnews.com%2Fpictures%2Fnew-kurt-cobain-death-scene-photos%2F10%2F 2
akamai 1
%22http%3A%2F%2Fasianjournal.com%2Fnews%2Fnevadas-lieutenant-governor-a-supporter-of-education-a-better-economy-and-filipinos%2F%22 1
```
``` bash
./hnStat top 3 --to 1438387241 ./test_file.tsv
http%3A%2F%2Fwww.cbsnews.com%2Fpictures%2Fnew-kurt-cobain-death-scene-photos%2F10%2F 2
%22http%3A%2F%2Fasianjournal.com%2Fnews%2Fnevadas-lieutenant-governor-a-supporter-of-education-a-better-economy-and-filipinos%2F%22 1
%22http%3A%2F%2Fwww.myfoxhouston.com%2Fstory%2F29678749%2Fobama-to-extend-college-aid-grants-to-some-prison-inmates%22 1
```

# Unit tests

## Unit tests dependencies
The Google Test library is used to unit-test the code. Install it with:

``` bash
sudo apt-get install libgtest-dev
sudo apt-get install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
```
## Building the unit tests binary
At the root of the hnStat repository:
```bash
make tests_hnStat
```
## Executing unit tests:
``` bash
./tests_hnStat
```
