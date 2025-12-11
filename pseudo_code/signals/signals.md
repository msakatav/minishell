# signals.c 疑似コード

## グローバル変数
- `g_signal_received`: 受信したシグナル番号を保持する。初期値は `0`。

## 関数: signal_handler
- **引数**:
  - `signum`: 受信したシグナル番号
- **処理**:
  1. `g_signal_received` に `signum` を設定する。
  2. `SIGINT` (Ctrl+C) の場合:
     - 改行を出力する。
     - `rl_on_new_line` で readline の状態を更新する。
     - `rl_replace_line` で入力行をクリアする。
     - `rl_redisplay` でプロンプトを再表示する。

## 関数: signal_handler_heredoc
- **引数**:
  - `signum`: 受信したシグナル番号
- **処理**:
  1. `SIGINT` の場合:
     - `g_signal_received` に `signum` を設定する。
     - 改行を出力する。
     - 標準入力 (`STDIN_FILENO`) を閉じる。これにより `readline` が `NULL` を返し、ループが終了する。

## 関数: setup_signals
- **引数**: なし
- **処理**:
  1. `sigaction` 構造体 `sa` を初期化する。
  2. ハンドラとして `signal_handler` を設定する。
  3. シグナルマスクを空にする。
  4. フラグに `SA_RESTART` を設定する。
  5. `SIGINT` に対して `sigaction` を設定する。
  6. `SIGQUIT` (Ctrl+\) を無視 (`SIG_IGN`) するように設定する。
