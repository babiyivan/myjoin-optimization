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
          25201.63 msec task-clock                       #    1.000 CPUs utilized          
                77      context-switches                 #    3.055 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
           1135196      page-faults                      #   45.045 K/sec                  
       90858019300      cycles                           #    3.605 GHz                    
       76347489653      instructions                     #    0.84  insn per cycle         
       17253171661      branches                         #  684.606 M/sec                  
         330981364      branch-misses                    #    1.92% of all branches        
      454290096500      slots                            #   18.026 G/sec                  
       59840582455      topdown-retiring                 #     12.6% Retiring              
      138959323635      topdown-bad-spec                 #     29.2% Bad Speculation       
       37496286559      topdown-fe-bound                 #      7.9% Frontend Bound        
      239809378117      topdown-be-bound                 #     50.4% Backend Bound         

      25.203453258 seconds time elapsed

      23.262874000 seconds user
       1.939906000 seconds sys
```

## myjoin9
(cycles AND time got worse)

Based on **myjoin8**
Changes: Implemented parallelized join processing (NOT reading, as in myjoin7). The join process is divided into chunks of data1, and each chunk is processed in parallel using threads. The results from each thread are stored in a shared buffer, which is combined and written to standard output at the end.

```
          28071.81 msec task-clock                       #    1.662 CPUs utilized          
               633      context-switches                 #   22.549 /sec                   
                 1      cpu-migrations                   #    0.036 /sec                   
           1252169      page-faults                      #   44.606 K/sec                  
       99157079419      cycles                           #    3.532 GHz                    
       82849195425      instructions                     #    0.84  insn per cycle         
       18883467832      branches                         #  672.684 M/sec                  
         356477459      branch-misses                    #    1.89% of all branches        
      495785397095      slots                            #   17.661 G/sec                  
       70261970355      topdown-retiring                 #     13.3% Retiring              
      142097405758      topdown-bad-spec                 #     26.9% Bad Speculation       
       44274296485      topdown-fe-bound                 #      8.4% Frontend Bound        
      271674085488      topdown-be-bound                 #     51.4% Backend Bound         

      16.886052383 seconds time elapsed

      25.630032000 seconds user
       2.443049000 seconds sys
```

## myjoin10
(cycles got worse)

Based on **myjoin8**
Changes: output buffer is flushed in batches rather than holding all results in memory at once.

```
          25663.01 msec task-clock                       #    1.000 CPUs utilized          
               220      context-switches                 #    8.573 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
           1127015      page-faults                      #   43.916 K/sec                  
       95875908602      cycles                           #    3.736 GHz                    
       81929455753      instructions                     #    0.85  insn per cycle         
       18520762167      branches                         #  721.691 M/sec                  
         342373324      branch-misses                    #    1.85% of all branches        
      479379543010      slots                            #   18.680 G/sec                  
       60937734141      topdown-retiring                 #     12.0% Retiring              
      206791175416      topdown-bad-spec                 #     40.7% Bad Speculation       
       41925412671      topdown-fe-bound                 #      8.2% Frontend Bound        
      198990584503      topdown-be-bound                 #     39.1% Backend Bound         

      25.666305244 seconds time elapsed

      24.036038000 seconds user
       1.628002000 seconds sys
```

## myjoin11
(cycles AND time got VERY much worse)

Based on **myjoin8**
Changes: sort the data and use binary search for lookups instead of hash-based lookups with unordered_multimap

```
          68849.70 msec task-clock                       #    1.000 CPUs utilized          
               239      context-switches                 #    3.471 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
            285993      page-faults                      #    4.154 K/sec                  
      280943563629      cycles                           #    4.081 GHz                    
      330645241038      instructions                     #    1.18  insn per cycle         
       93758298470      branches                         #    1.362 G/sec                  
        1574015847      branch-misses                    #    1.68% of all branches        
     1404717818145      slots                            #   20.403 G/sec                  
      255368844819      topdown-retiring                 #     17.1% Retiring              
      589430613888      topdown-bad-spec                 #     39.4% Bad Speculation       
      244251297838      topdown-fe-bound                 #     16.3% Frontend Bound        
      407115100343      topdown-be-bound                 #     27.2% Backend Bound         

      68.852913264 seconds time elapsed

      67.926810000 seconds user
       0.923983000 seconds sys
```

## myjoin12
(cycles AND time got worse)

Based on **myjoin8**
Changes: "intern" strings by replacing them with integer IDs

```
          36595.06 msec task-clock                       #    1.000 CPUs utilized          
               296      context-switches                 #    8.089 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
            458348      page-faults                      #   12.525 K/sec                  
      120568790850      cycles                           #    3.295 GHz                    
       66422471949      instructions                     #    0.55  insn per cycle         
       14979152724      branches                         #  409.322 M/sec                  
         264924182      branch-misses                    #    1.77% of all branches        
      602843954250      slots                            #   16.473 G/sec                  
       47717904112      topdown-retiring                 #      7.4% Retiring              
      186763421120      topdown-bad-spec                 #     29.0% Bad Speculation       
       33023428910      topdown-fe-bound                 #      5.1% Frontend Bound        
      377568690022      topdown-be-bound                 #     58.5% Backend Bound         

      36.599301269 seconds time elapsed

      35.500191000 seconds user
       1.095882000 seconds sys
```

## myjoin13
(cycles AND time got worse)

Based on **myjoin8**
Changes: Offload the output writing task to a separate thread to decouple it from the join process.

```
          33625.99 msec task-clock                       #    1.143 CPUs utilized          
           2668332      context-switches                 #   79.353 K/sec                  
               148      cpu-migrations                   #    4.401 /sec                   
           1127267      page-faults                      #   33.524 K/sec                  
      123938714878      cycles                           #    3.686 GHz                    
      105978002991      instructions                     #    0.86  insn per cycle         
       24443923073      branches                         #  726.935 M/sec                  
         399555699      branch-misses                    #    1.63% of all branches        
      619693574390      slots                            #   18.429 G/sec                  
       59465153220      topdown-retiring                 #      7.7% Retiring              
      491006814245      topdown-bad-spec                 #     63.5% Bad Speculation       
       47234863496      topdown-fe-bound                 #      6.1% Frontend Bound        
      175044492219      topdown-be-bound                 #     22.7% Backend Bound         

      29.406903436 seconds time elapsed

      26.725183000 seconds user
       8.954524000 seconds sys
```

## myjoin14

Based on **myjoin8**
Changes: use memory mapping (mmap).

```
          25179.73 msec task-clock                       #    1.000 CPUs utilized          
               116      context-switches                 #    4.607 /sec                   
                 0      cpu-migrations                   #    0.000 /sec                   
           1153978      page-faults                      #   45.830 K/sec                  
       92337135105      cycles                           #    3.667 GHz                    
       85061508834      instructions                     #    0.92  insn per cycle         
       19971205202      branches                         #  793.146 M/sec                  
         370403388      branch-misses                    #    1.85% of all branches        
      461685675525      slots                            #   18.336 G/sec                  
       64959065852      topdown-retiring                 #     13.1% Retiring              
      170190013722      topdown-bad-spec                 #     34.4% Bad Speculation       
       41399641094      topdown-fe-bound                 #      8.4% Frontend Bound        
      217772979619      topdown-be-bound                 #     44.1% Backend Bound         

      25.183806405 seconds time elapsed

      23.436986000 seconds user
       1.743775000 seconds sys
```

