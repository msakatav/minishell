# executor.c 疑似コード

## 関数: executor
- **引数**:
  - `cmds`: コマンドリスト
  - `data`: シェルの主要データ構造体
- **処理**:
  1. コマンドリストが空の場合、何もせず終了する。
  2. `SIGINT` を無視 (`SIG_IGN`) するように設定する。
  3. 現在の標準入力と標準出力を `dup` で保存する (`saved_stdin`, `saved_stdout`)。
  4. コマンドが複数ある場合 (`cmds->next` が存在):
     - `execute_pipeline(cmds, data)` を呼び出す。
  5. コマンドが単一の場合:
     - `execute_cmd(cmds, data)` を呼び出す。
  6. 標準入力と標準出力を保存しておいたディスクリプタから復元する (`dup2`)。
  7. 保存用のディスクリプタを閉じる。
  8. `setup_signals` を呼び出し、シグナルハンドラを復元する。
