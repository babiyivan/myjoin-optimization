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
* reduce Scope of `std::string`
* reserve memory for `std::vector` in read_file
* remove now unused method `split`

```
109,042,856,561      cycles        

29.300540595 seconds time elapsed

26.262301000 seconds user
3.035803000 seconds sys
```

## myjoin6
(loop unrolling pt1?) 

Based on: **myjoin5**
**Changes**
* unroll the two inner-most loops by two
* use `std::ostringstream buffer` to buffer the output and flush in one go

```
105,060,222,941      cycles         

28.286133267 seconds time elapsed

25.009352000 seconds user
3.271653000 seconds sys
```

## myjoin7
(cycles got worse, only time improved, so you might want to base future optimizations on earlier ones rather than this one)

Based on: **myjoin6**
**Changes**
* parallelize reading of csv files

```
113,064,693,638      cycles         

27.552050027 seconds time elapsed

26.493009000 seconds user
4.223349000 seconds sys
```

## myjoin8

Based on **myjoin6**
Changes: Removed creation of map1, initialized unordered_multimap with reserved space

```
          26331.87 msec task-clock                       #    1.000 CPUs utilized          
               206      context-switches                 #    7.823 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
           2811832      page-faults                      #  106.784 K/sec                  
       97089778930      cycles                           #    3.687 GHz                    
       86860106024      instructions                     #    0.89  insn per cycle         
       19435077674      branches                         #  738.082 M/sec                  
         331870053      branch-misses                    #    1.71% of all branches        
      485448894650      slots                            #   18.436 G/sec                  
       61743309419      topdown-retiring                 #     12.0% Retiring              
      205601884792      topdown-bad-spec                 #     39.9% Bad Speculation       
       43159486450      topdown-fe-bound                 #      8.4% Frontend Bound        
      205020485087      topdown-be-bound                 #     39.8% Backend Bound         

      26.335056698 seconds time elapsed

      23.241372000 seconds user
       3.091650000 seconds sys
```