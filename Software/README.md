Gcodeから線形補間した移動経路へ
====
* `gcode2points.py`
* Gcodeの軌道を動画に、最終作品を画像に出力するスクリプト。
```
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
```
$ python gcode2thetas.py --help # ヘルプ
$ python gcode2thetas.py sample_stldata/DD_carriage_for_atom.gcode 100 200 80 10
```


出力CSV
---
`DD_carriage_for_atom.gcode`からの出力  
[thetas_DD_carriage_for_atom.csv](https://raw.githubusercontent.com/ut-3dprinter/ut-3dprinter/master/Software/output/thetas_DD_carriage_for_atom.csv)
```
theta1,theta2,theta3
0.64778159567699933,0.30318376719655998,0.485534949219672
0.64549945185215252,0.30088009193760218,0.48325687837283304
0.64644647128323007,0.29832354876547906,0.4834969728939148
0.64578115813232795,0.29851182768757428,0.48425837068447941
0.64012071887114763,0.30294078972811256,0.48903389524155949
....
```
