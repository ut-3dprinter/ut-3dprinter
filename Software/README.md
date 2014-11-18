Gcodeから線形補間した移動経路へ
====
* `gcode2points.py`
* Gcodeの軌道を動画に、最終作品を画像に出力するスクリプト。

```bash
$ python gcode2points.py --help # ヘルプ
$ python gcode2points.py gcode_files/pyramid_50_50_binary.gcode --nframe 100
```

出力画像
---
![pyramid_50_50_binary.png](https://raw.githubusercontent.com/ut-3dprinter/ut-3dprinter/master/Software/final_outputs/pyramid_50_50_binary.png)


Gcodeから経路補完しそれをリンクの腕の角度へ
---
* `gcode2thetas.py`
* Gcodeをアームの角度に変換するスクリプト。

```bash
$ python gcode2thetas.py --help # ヘルプ
$ python gcode2thetas.py gcode_files/pyramid_50_50_binary.gcode 96 162 80 12
```


出力datファイル
---
`pyramid_50_50_binary.gcode`からの出力  
[thetas_pyramid_50_50_binary.dat](https://raw.githubusercontent.com/ut-3dprinter/ut-3dprinter/master/Software/dat_files/thetas_pyramid_50_50_binary.dat)

datファイルの値の型は`short int`です。
値を確認するためには`c_readtest.c`を利用してください。

```bash
$ # Output [theta1,theta2,theta3]
$ make clean
$ make
$ c_readtest dat_files/thetas_pyramid_50_50_binary.dat
4814 5118 4877
4811 5092 4863
4828 5087 4868
4843 5082 4874
4850 5078 4883
....
```
