# execute_cmd.c 疑似コード

## 関数: execute_external_cmd
- **引数**:
  - `cmd`: 実行するコマンド構造体
  - `data`: シェルの主要データ構造体
- **処理**:
  1. `find_executable` を呼び出し、コマンドの絶対パスを取得する。
  2. パスが見つからない場合:
     - エラーメッセージ "command not found" を出力する。
     - ステータス `127` で終了 (`exit`) する。
  3. 実行権限がない場合 (`access(path, X_OK) != 0`):
     - エラーメッセージ "Permission denied" を出力する。
     - パスのメモリを解放する。
     - ステータス `126` で終了 (`exit`) する。
  4. `env_to_array` を呼び出し、環境変数を配列形式に変換する。
  5. `execve` を呼び出し、コマンドを実行する。
  6. `execve` が失敗した場合:
     - エラーメッセージを出力する。
     - パスと環境変数配列のメモリを解放する。
     - ステータス `126` で終了 (`exit`) する。

## 関数: handle_child_process
- **引数**:
  - `cmd`: 実行するコマンド構造体
  - `data`: シェルの主要データ構造体
- **処理**:
  1. `setup_redirections` を呼び出し、リダイレクトを設定する。
     - 失敗した場合、ステータス `1` で終了 (`exit`) する。
  2. `SIGINT` と `SIGQUIT` のハンドラをデフォルト (`SIG_DFL`) に戻す。
  3. `execute_external_cmd` を呼び出し、外部コマンドを実行する。

## 関数: handle_parent_process
- **引数**:
  - `pid`: 子プロセスのPID
  - `data`: シェルの主要データ構造体
- **処理**:
  1. `waitpid` で子プロセスの終了を待つ。
  2. 子プロセスが正常終了した場合 (`WIFEXITED`)、終了ステータスを取得して `data->exit_status` に設定する。
  3. 子プロセスがシグナルで終了した場合 (`WIFSIGNALED`):
     - シグナル番号に `128` を加えた値を `data->exit_status` に設定する。
     - `SIGINT` の場合、改行を出力する。
     - `SIGQUIT` の場合、"Quit (core dumped)" を出力する。

## 関数: execute_cmd
- **引数**:
  - `cmd`: 実行するコマンド構造体
  - `data`: シェルの主要データ構造体
- **処理**:
  1. コマンドが無効な場合、処理を終了する。
  2. コマンドがビルトインの場合 (`is_builtin`):
     - `setup_redirections` を呼び出し、リダイレクトを設定する。
       - 失敗した場合、`data->exit_status` を `1` に設定して終了する。
     - `execute_builtin` を呼び出し、ビルトインコマンドを実行し、戻り値を `data->exit_status` に設定する。
     - 処理を終了する。
  3. `fork` を呼び出し、子プロセスを作成する。
  4. 子プロセス (`pid == 0`) の場合:
     - `handle_child_process` を呼び出す。
  5. 親プロセス (`pid > 0`) の場合:
     - `handle_parent_process` を呼び出す。
  6. `fork` に失敗した場合:
     - エラーメッセージを出力する。
