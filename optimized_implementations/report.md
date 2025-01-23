## Original
```
535.027.663.415      cycles

131.808134958 seconds time elapsed

129.042447000 seconds user
2.759966000 seconds sys
```

## With -O3 when compiling
```
355,163,021,185      cycles

91.264964371 seconds time elapsed

88.205682000 seconds user
3.051781000 seconds sys
```

## myjoin2
Based on: **With -O3 when compiling**
**Changes:**
* in `split()`
* The split function now avoids creating a `stringstream` and instead uses `find` and `substr`, which are generally faster for simple tokenization
* use `emplace_back` instead of `push_back`
    * Avoids the need for a temporary object and the associated copy/move operations
```
312.277.715.826      cycles

92.564241486 seconds time elapsed

89.390281000 seconds user
3.167939000 seconds sys
```

## myjoin3
Based on: **myjoin2**
**Changes**
* in read file use move `std::move` to avoid copying the string

```
308,833,085,225      cycles

81.429837800 seconds time elapsed

78.271056000 seconds user
3.151800000 seconds sys
```

## myjoin4
Base on: **myjoin3**
**Changes**
* use `unordered_multimap` with pre-allocation

```
121,187,831,303      cycles

31.612314720 seconds time elapsed

28.487886000 seconds user
3.123987000 seconds sys
```

## myjoin5
Based on: **myjoin4**
**Changes**
* Reduce Scope of `std::string` and reserve memory for `std::vector` in read_file

```
109,042,856,561      cycles        

29.300540595 seconds time elapsed

26.262301000 seconds user
3.035803000 seconds sys
```