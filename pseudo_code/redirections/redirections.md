# redirections.c 疑似コード

## 関数: handle_input_redir
- **引数**:
  - `file`: 入力ファイル名
- **処理**:
  1. ファイルを読み込みモード (`O_RDONLY`) で開く。
  2. 失敗した場合:
     - エラーメッセージを出力する。
     - `-1` を返す。
  3. `dup2` でファイルディスクリプタを標準入力 (`STDIN_FILENO`) に複製する。
  4. 複製に失敗した場合、ファイルを閉じて `-1` を返す。
  5. ファイルを閉じて `0` を返す。

## 関数: handle_output_redir
- **引数**:
  - `file`: 出力ファイル名
  - `append`: 追記モードフラグ
- **処理**:
  1. フラグを設定する:
     - 基本: `O_WRONLY | O_CREAT`
     - `append` が真なら `O_APPEND` を追加。
     - 偽なら `O_TRUNC` を追加。
  2. ファイルを開く（権限 `0644`）。
  3. 失敗した場合:
     - エラーメッセージを出力する。
     - `-1` を返す。
  4. `dup2` でファイルディスクリプタを標準出力 (`STDOUT_FILENO`) に複製する。
  5. 複製に失敗した場合、ファイルを閉じて `-1` を返す。
  6. ファイルを閉じて `0` を返す。

## 関数: setup_redirections
- **引数**:
  - `redirs`: リダイレクトリスト
  - `data`: シェルの主要データ構造体
- **処理**:
  1. リダイレクトリストを走査するループ:
     - `TOKEN_REDIR_IN` の場合: `handle_input_redir` を呼び出す。
     - `TOKEN_REDIR_OUT` の場合: `handle_output_redir` (append=0) を呼び出す。
     - `TOKEN_REDIR_APPEND` の場合: `handle_output_redir` (append=1) を呼び出す。
     - `TOKEN_REDIR_HEREDOC` の場合:
       - `handle_heredoc` を呼び出し、ファイルディスクリプタを取得する。
       - 取得したディスクリプタを標準入力に `dup2` する。
       - ディスクリプタを閉じる。
     - いずれかの処理で失敗 (`< 0`) した場合、`-1` を返す。
     - 次のノードへ進む。
  2. `0` を返す。
