# Single Optimizations
**Performance of each Optimization compared to Baseline**

## Original
```
535.027.663.415      cycles

131.808134958 seconds time elapsed

129.042447000 seconds user
2.759966000 seconds sys
```

## Optimization 01
Changes from: **Makefile**\
* Compiling with -O3
```
355,163,021,185      cycles

91.264964371 seconds time elapsed

88.205682000 seconds user
3.051781000 seconds sys
```

## Optimization 02
Changes from: **myjoin2**\
* The split function now avoids creating a `stringstream` and instead uses `find` and `substr`, which are generally faster for simple tokenization
* use `emplace_back` instead of `push_back`
    * Avoids the need for a temporary object and the associated copy/move operations
```

```

## Optimization 03
Changes from: **myjoin3**\
* in read file use move `std::move` to avoid copying the string

```

```

## Optimization 04
Changes from: **myjoin4**\
* use `unordered_multimap` with pre-allocation

```

```

## Optimization 05
Changes from: **myjoin5**\
* reduce Scope of `std::string`
* reserve memory for `std::vector` in read_file
* remove now unused method `split`

```

```

## Optimization 06
Changes from: **myjoin6**\
* use `std::ostringstream buffer` to buffer the output and flush in one go

```

```

## Optimization 07
Changes from: **myjoin6**\
* skip loop iterations if no matches exist

```

```

## Optimization 08
Changes from: **myjoin6**\
* unroll the two inner-most loops by two

```

```

## Optimization 09
Changes from: **myjoin7**\
* parallelize reading of csv files

```

```

## Optimization 10
Changes from: **myjoin8**\
* initialized unordered_multimap with reserved space

```

```