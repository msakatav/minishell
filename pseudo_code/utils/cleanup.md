# cleanup.c 疑似コード

## 関数: cleanup_data
- **概要**:
  - シェルのデータ構造体と関連リソースを解放する。
- **引数**:
  - `data`: シェルの主要データ構造体
- **処理**:
  1. `data` が `NULL` なら何もしない。
  2. `data->cmds` があれば `free_cmds` で解放する。
  3. `data->env` があれば `free_env` で解放する。
  4. `stdin_backup` が有効 (`!= -1`) なら閉じる。
  5. `stdout_backup` が有効 (`!= -1`) なら閉じる。
