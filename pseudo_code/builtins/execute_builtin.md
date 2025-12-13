# execute_builtin.c 疑似コード

## 関数: execute_builtin
- **概要**:
  - コマンド名に対応するビルトイン関数を呼び出す。
- **引数**:
  - `cmd`: 実行するコマンド構造体
  - `data`: シェルの主要データ構造体
- **処理**:
  1. コマンドが無効、または引数がない場合、`1` を返す。
  2. コマンド名 (`cmd->args[0]`) に応じて対応するビルトイン関数を呼び出す:
     - "echo" -> `builtin_echo`
     - "cd" -> `builtin_cd`
     - "pwd" -> `builtin_pwd`
     - "export" -> `builtin_export`
     - "unset" -> `builtin_unset`
     - "env" -> `builtin_env`
     - "exit" -> `builtin_exit`
  3. いずれにも該当しない場合、`1` を返す。
