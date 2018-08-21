emscripten で WebAssembly にした mruby で矩形の衝突判定を動かして、mruby寄りの実装とC寄りの実装の速度を比較する。

以下、macbook air ( i7 @ 1.7GHz ) と iPhone7 での参考記録

### 100 enemies / 1000 bullets

| browser       | all mruby | C with mrb_iv_get | almost C |
|---------------|-----------|-------------------|--------|
| Safari@iPhone | 922 msec  | 15 msec           | 3 msec |
| Safari@macOS  | 861 msec  |  9 msec           | 2 msec |
| Firefox       | 156 msec  |  7 msec           | 1 msec |

### 1000 enemies / 100 bullets

| browser       | all mruby | C with mrb_iv_get | almost C |
|---------------|-----------|-------------------|---------|
| Safari@iPhone | 822 msec  | 21 msec           | 24 msec |
| Safari@macOS  | 880 msec  | 27 msec           | 23 msec |
| Firefox       | 144 msec  |  8 msec           |  2 msec |

### 100 enemies / 100 bullets

| browser       | all mruby | C with mrb_iv_get | almost C |
|---------------|-----------|-------------------|--------|
| Safari@iPhone | 218 msec  |  4 msec           | 1 msec |
| Safari@macOS  | 216 msec  |  3 msec           | 2 msec |
| Firefox       |  44 msec  |  2 msec           | 1 msec |

### ネイティブで実行

| enemies | bullets | all mruby | C with mrb_iv_get | almost C |
|---------|---------|-----------|-------------------|----------|
|  100    | 1000    | 23.7 msec | 3.0 msec          | 0.3 msec |
| 1000    |  100    | 18.5 msec | 2.6 msec          | 0.4 msec |
|  100    |  100    |  5.2 msec | 0.8 msec          | 0.1 msec |
