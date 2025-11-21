# main.c 疑似コード

## 関数: init_data
- **引数**:
  - `data`: シェルの主要データを保持する構造体へのポインタ
  - `envp`: 環境変数の文字列配列
- **処理**:
  1. `init_env(envp)` を呼び出し、環境変数を初期化して `data->env` に格納する。
  2. コマンドリスト `data->cmds` を `NULL` に初期化する。
  3. 終了ステータス `data->exit_status` を `0` に初期化する。
  4. 標準入力のバックアップ `data->stdin_backup` を `-1` に初期化する。
  5. 標準出力のバックアップ `data->stdout_backup` を `-1` に初期化する。

## 関数: process_line
- **引数**:
  - `line`: 入力されたコマンドライン文字列
  - `data`: シェルの主要データ構造体
- **処理**:
  1. `line` が `NULL` または空文字の場合、処理を終了する。
  2. `line` の先頭から空白文字をスキップする。
  3. 空白スキップ後に文字がない場合、処理を終了する。
  4. `add_history(line)` を呼び出し、コマンド履歴に追加する。
  5. `lexer(line, data)` を呼び出し、トークン列を生成する。
     - トークン生成に失敗した場合、処理を終了する。
  6. `parser(tokens)` を呼び出し、トークン列を解析してコマンド構造体 `data->cmds` を生成する。
  7. `free_tokens(tokens)` を呼び出し、トークン列のメモリを解放する。
  8. コマンド構造体 `data->cmds` が生成された場合:
     - `executor(data->cmds, data)` を呼び出し、コマンドを実行する。
     - `free_cmds(data->cmds)` を呼び出し、コマンド構造体のメモリを解放する。
     - `data->cmds` を `NULL` にリセットする。

## 関数: shell_loop
- **引数**:
  - `data`: シェルの主要データ構造体
- **処理**:
  1. 無限ループを開始する:
     - `readline(PROMPT)` を呼び出し、ユーザーからの入力を受け取る。
     - 入力が `NULL` (EOF) の場合:
       - 標準エラー出力に "exit" を出力する。
       - ループを抜ける。
     - シグナル `SIGINT` を受信していた場合 (`g_signal_received == SIGINT`):
       - 終了ステータス `data->exit_status` を `130` に設定する。
       - シグナルフラグ `g_signal_received` を `0` にリセットする。
     - `process_line(line, data)` を呼び出し、入力行を処理する。
     - `free(line)` を呼び出し、入力行のメモリを解放する。

## 関数: main
- **引数**:
  - `argc`: 引数の数 (未使用)
  - `argv`: 引数の配列 (未使用)
  - `envp`: 環境変数の配列
- **処理**:
  1. `argc`, `argv` を void キャストして未使用警告を抑制する。
  2. `init_data(&data, envp)` を呼び出し、データを初期化する。
  3. `setup_signals()` を呼び出し、シグナルハンドラを設定する。
  4. `shell_loop(&data)` を呼び出し、メインループを開始する。
  5. `cleanup_data(&data)` を呼び出し、データのクリーンアップを行う。
  6. `rl_clear_history()` を呼び出し、readline の履歴をクリアする。
  7. `data.exit_status` を返して終了する。
