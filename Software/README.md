# Gcodeをパラレルリンクの腕の角度へ変換


## Gcodeから線形補間した移動経路
Gcodeから軌道を動画へ、最終作品を画像へ出力。

```bash
$ python gcode2points.py --help # ヘルプ
$ python gcode2points.py sample_stldata/DD_carriage_for_atom.gcode
```

### 3D Printed 画像

![3dprinted](3dprinted.png)


### 3D Printing 動画

`3dprinting.mp4`


## Gcode → 線形補間した経路 → パラレルリンクの腕の角度
Gcodeからパラレルリンクの腕の角度3つをCSVファイルへ出力。

```bash
$ python gcode2thetas.py --help # ヘルプ
$ python gcode2thetas.py sample_stldata/DD_carriage_for_atom.gcode 100 200 80 10
```

出力CSVは`output/thetas_DD_carriage_for_atom.csv`。
