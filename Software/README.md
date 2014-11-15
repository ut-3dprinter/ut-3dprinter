Gcodeから線形補間した移動経路へ
====
* `gcode2points.py`
* Gcodeの軌道を動画に、最終作品を画像に出力するスクリプト。

```bash
$ python gcode2points.py --help # ヘルプ
$ python gcode2points.py sample_stldata/DD_carriage_for_atom.gcode
```

出力画像
---
![3dprinted](https://raw.githubusercontent.com/ut-3dprinter/ut-3dprinter/master/Software/sample_stldata/test2.png)

出力動画
---
[test2.mp4](http://youtu.be/XC9xf_K4ywE)


Gcodeから経路補完しそれをリンクの腕の角度へ
---
* `gcode2thetas.py`
* Gcodeをアームの角度に変換するスクリプト。

```bash
$ python gcode2thetas.py --help # ヘルプ
$ python gcode2thetas.py sample_stldata/DD_carriage_for_atom.gcode 100 200 80 10
```


出力datファイル
---
`DD_carriage_for_atom.gcode`からの出力  
[thetas_DD_carriage_for_atom.dat](https://raw.githubusercontent.com/ut-3dprinter/ut-3dprinter/master/Software/output/thetas_DD_carriage_for_atom.dat)

datファイルの値の型は`short int`です。
値を確認するためには`c_readtest.c`を利用してください。

```bash
$ # theta1,theta2,theta3
$ make clean
$ make
$ c_readtest output/thetas_DD_carriage_for_atom.dat
4814 5118 4877
4811 5092 4863
4828 5087 4868
4843 5082 4874
4850 5078 4883
....
```
