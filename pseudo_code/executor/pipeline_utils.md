# pipeline_utils.c 疑似コード

## 関数: count_cmds
- **引数**:
  - `cmds`: コマンドリスト
- **処理**:
  1. リスト内のコマンド数をカウントして返す。

## 関数: handle_parent_pipeline
- **引数**:
  - `prev_pipe`: 前のパイプのファイルディスクリプタ配列
  - `pipe_fd`: 現在のパイプのファイルディスクリプタ配列
  - `cmd`: 現在のコマンド構造体
- **処理**:
  1. 前のパイプが存在する場合 (`prev_pipe[0] != -1`):
     - `prev_pipe` の両端を閉じる。
  2. 次のコマンドが存在する場合 (`cmd->next`):
     - 現在のパイプ `pipe_fd` を `prev_pipe` に保存する。

## 関数: wait_for_children
- **引数**:
  - `cmd_count`: コマンドの総数
  - `last_pid`: 最後のコマンドのプロセスID
  - `data`: シェルの主要データ構造体
- **処理**:
  1. `signal_printed` フラグを `0` に初期化する。
  2. コマンドの数だけループする:
     - `wait` で子プロセスの終了を待つ。
     - プロセスがシグナルで終了し、かつ `signal_printed` が `0` の場合:
       - `SIGINT` なら改行を出力し、フラグを立てる。
       - `SIGQUIT` なら "Quit (core dumped)" を出力し、フラグを立てる。
     - 終了したプロセスが `last_pid` と一致する場合:
       - 正常終了なら終了ステータスを `data->exit_status` に保存。
       - シグナル終了なら `128 + シグナル番号` を保存。
