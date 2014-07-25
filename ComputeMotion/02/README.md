実行方法
---
```sh
$ ./analyze.py 100 200 80 10 -s 2
```

ヘルプ
--
```sh
$ ./analyze.py -h
usage: analyze.py [-h] [--sparseness SPARSENESS] l1 l2 r1 r2

Analyze about motion of parallel link

positional arguments:
  l1                    l1
  l2                    l2
  r1                    r1
  r2                    r2

optional arguments:
  -h, --help            show this help message and exit
  --sparseness SPARSENESS, -s SPARSENESS
                        The sparseness when analyzing points and theta.
                        Default is 1 and this means x moving is per 1mm.
```

実行結果(例)
--
```
$ ./analyze.py 100 200 80 10 -s 1
```
![](can_move.png)

