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
Changes from: **Makefile**
* Compiling with -O3

```
          91570.83 msec task-clock                       #    1.000 CPUs utilized
               272      context-switches                 #    2.970 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1640035      page-faults                      #   17.910 K/sec
      354252180112      cycles                           #    3.869 GHz
      356006248416      instructions                     #    1.00  insn per cycle
       88475451183      branches                         #  966.197 M/sec
        1268969129      branch-misses                    #    1.43% of all branches
     1771260900560      slots                            #   19.343 G/sec
      245487119829      topdown-retiring                 #     12.1% Retiring
      944672480298      topdown-bad-spec                 #     46.5% Bad Speculation
      214427872307      topdown-fe-bound                 #     10.6% Frontend Bound
      625096550090      topdown-be-bound                 #     30.8% Backend Bound

      91.574292490 seconds time elapsed

      88.936042000 seconds user
       2.636001000 seconds sys
```

## Optimization 02
Changes from: **myjoin2**
* The split function now avoids creating a `stringstream` and instead uses `find` and `substr`, which are generally faster for simple tokenization
* use `emplace_back` instead of `push_back`
    * Avoids the need for a temporary object and the associated copy/move operations

```
          80803.94 msec task-clock                       #    1.000 CPUs utilized
               598      context-switches                 #    7.401 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1639248      page-faults                      #   20.287 K/sec
      309084299501      cycles                           #    3.825 GHz
      213579862023      instructions                     #    0.69  insn per cycle
       58923782304      branches                         #  729.219 M/sec
        1254874785      branch-misses                    #    2.13% of all branches
     1545421497505      slots                            #   19.126 G/sec
      122384878769      topdown-retiring                 #      7.3% Retiring
      818164322208      topdown-bad-spec                 #     49.1% Bad Speculation
      161241514075      topdown-fe-bound                 #      9.7% Frontend Bound
      563809351491      topdown-be-bound                 #     33.9% Backend Bound

      80.810959796 seconds time elapsed

      78.385226000 seconds user
       2.419790000 seconds sys
```

## Optimization 03
Changes from: **myjoin3**
* in read file use move `std::move` to avoid copying the string

```
         89896.56 msec task-clock                       #    1.000 CPUs utilized
               701      context-switches                 #    7.798 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1639382      page-faults                      #   18.236 K/sec
      350438165814      cycles                           #    3.898 GHz
      350902746940      instructions                     #    1.00  insn per cycle
       86549362056      branches                         #  962.766 M/sec
        1259570748      branch-misses                    #    1.46% of all branches
     1752190829070      slots                            #   19.491 G/sec
      240437185859      topdown-retiring                 #     12.0% Retiring
     1298682614487      topdown-bad-spec                 #     64.8% Bad Speculation
      196237467886      topdown-fe-bound                 #      9.8% Frontend Bound
      269445087766      topdown-be-bound                 #     13.4% Backend Bound

      89.904439810 seconds time elapsed

      87.529556000 seconds user
       2.367825000 seconds sys
```

## Optimization 04
Changes from: **myjoin4**
* use `unordered_multimap` with pre-allocation

```
          40784.25 msec task-clock                       #    1.000 CPUs utilized
                79      context-switches                 #    1.937 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1485616      page-faults                      #   36.426 K/sec
      164409320397      cycles                           #    4.031 GHz
      283113929465      instructions                     #    1.72  insn per cycle
       60840899577      branches                         #    1.492 G/sec
         424482798      branch-misses                    #    0.70% of all branches
      822046601985      slots                            #   20.156 G/sec
      241683057726      topdown-retiring                 #     27.6% Retiring
      267568109665      topdown-bad-spec                 #     30.5% Bad Speculation
      104670619618      topdown-fe-bound                 #     11.9% Frontend Bound
      262803841891      topdown-be-bound                 #     30.0% Backend Bound

      40.785361442 seconds time elapsed

      38.616809000 seconds user
       2.168045000 seconds sys
```

## Optimization 05
Changes from: **myjoin5**
* reduce Scope of `std::string`
* reserve memory for `std::vector` in read_file
* remove now unused method `split`

```
          80412.02 msec task-clock                       #    1.000 CPUs utilized
               232      context-switches                 #    2.885 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1645374      page-faults                      #   20.462 K/sec
      300240187511      cycles                           #    3.734 GHz
      179815690592      instructions                     #    0.60  insn per cycle
       51034084124      branches                         #  634.657 M/sec
        1204060879      branch-misses                    #    2.36% of all branches
     1501200937555      slots                            #   18.669 G/sec
       94240379416      topdown-retiring                 #      5.9% Retiring
      606367437522      topdown-bad-spec                 #     37.9% Bad Speculation
      138535007821      topdown-fe-bound                 #      8.7% Frontend Bound
      759386955668      topdown-be-bound                 #     47.5% Backend Bound

      80.423237058 seconds time elapsed

      78.044555000 seconds user
       2.368016000 seconds sys
```

## Optimization 06
Changes from: **myjoin6**
* use `std::ostringstream buffer` to buffer the output and flush in one go

```
          92162.10 msec task-clock                       #    1.000 CPUs utilized
               336      context-switches                 #    3.646 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1647198      page-faults                      #   17.873 K/sec
      355328606344      cycles                           #    3.855 GHz
      350643328399      instructions                     #    0.99  insn per cycle
       87320951482      branches                         #  947.471 M/sec
        1260950124      branch-misses                    #    1.44% of all branches
     1776643031720      slots                            #   19.277 G/sec
      250834513509      topdown-retiring                 #     12.3% Retiring
      989346315702      topdown-bad-spec                 #     48.4% Bad Speculation
      216559251959      topdown-fe-bound                 #     10.6% Frontend Bound
      585356612416      topdown-be-bound                 #     28.7% Backend Bound

      92.166316228 seconds time elapsed

      89.319209000 seconds user
       2.843974000 seconds sys
```

## Optimization 07
Changes from: **myjoin6**
* skip loop iterations if no matches exist

```
          92287.24 msec task-clock                       #    1.000 CPUs utilized
               202      context-switches                 #    2.189 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1640686      page-faults                      #   17.778 K/sec
      358160233020      cycles                           #    3.881 GHz
      359175927986      instructions                     #    1.00  insn per cycle
       88969816133      branches                         #  964.053 M/sec
        1261444702      branch-misses                    #    1.42% of all branches
     1790801165100      slots                            #   19.405 G/sec
      249951232893      topdown-retiring                 #     12.4% Retiring
      849752709714      topdown-bad-spec                 #     42.0% Bad Speculation
      204926085107      topdown-fe-bound                 #     10.1% Frontend Bound
      717603878873      topdown-be-bound                 #     35.5% Backend Bound

      92.290239917 seconds time elapsed

      89.652470000 seconds user
       2.636013000 seconds sys
```

## Optimization 08
Changes from: **myjoin6**
* unroll the two inner-most loops by two

```
          93624.11 msec task-clock                       #    1.000 CPUs utilized
               274      context-switches                 #    2.927 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1640297      page-faults                      #   17.520 K/sec
      357420597379      cycles                           #    3.818 GHz
      359251111107      instructions                     #    1.01  insn per cycle
       89002831706      branches                         #  950.640 M/sec
        1257717413      branch-misses                    #    1.41% of all branches
     1787102986895      slots                            #   19.088 G/sec
      254169468221      topdown-retiring                 #     12.5% Retiring
      911072110966      topdown-bad-spec                 #     44.7% Bad Speculation
      204908950435      topdown-fe-bound                 #     10.1% Frontend Bound
      666861025357      topdown-be-bound                 #     32.7% Backend Bound

      93.627680183 seconds time elapsed

      90.989296000 seconds user
       2.635921000 seconds sys
```

## Optimization 09
Changes from: **myjoin7**
* parallelize reading of csv files

```
          96301.50 msec task-clock                       #    1.127 CPUs utilized
             16501      context-switches                 #  171.347 /sec
                22      cpu-migrations                   #    0.228 /sec
           1662615      page-faults                      #   17.265 K/sec
      374232105002      cycles                           #    3.886 GHz
      365825038555      instructions                     #    0.98  insn per cycle
       90688018377      branches                         #  941.709 M/sec
        1293212108      branch-misses                    #    1.43% of all branches
     1871160502735      slots                            #   19.430 G/sec
      154933392919      topdown-retiring                 #      7.9% Retiring
      770680071540      topdown-bad-spec                 #     39.3% Bad Speculation
      193549154799      topdown-fe-bound                 #      9.9% Frontend Bound
      842943841108      topdown-be-bound                 #     43.0% Backend Bound

      85.461477610 seconds time elapsed

      93.256800000 seconds user
       3.061662000 seconds sys
```

## Optimization 10
Changes from: **myjoin8**
* initialized unordered_multimap with reserved space

```
          39735.88 msec task-clock                       #    1.000 CPUs utilized
               320      context-switches                 #    8.053 /sec
                 0      cpu-migrations                   #    0.000 /sec
           1484155      page-faults                      #   37.351 K/sec
      160275805257      cycles                           #    4.034 GHz
      282568128452      instructions                     #    1.76  insn per cycle
       60721002347      branches                         #    1.528 G/sec
         423747670      branch-misses                    #    0.70% of all branches
      801379026285      slots                            #   20.168 G/sec
      213397832882      topdown-retiring                 #     21.3% Retiring
      571964638368      topdown-bad-spec                 #     57.2% Bad Speculation
      106778603361      topdown-fe-bound                 #     10.7% Frontend Bound
      107592879424      topdown-be-bound                 #     10.8% Backend Bound

      39.739857175 seconds time elapsed

      37.493178000 seconds user
       2.243831000 seconds sys
```