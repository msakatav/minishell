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

## 関数: handle_heredoc_redir
- **引数**:
  - `file`: ヒアドキュメントの一時ファイル名
- **処理**:
  1. ファイルを読み込みモード (`O_RDONLY`) で開く。
     - 失敗した場合、エラーメッセージを出力して `-1` を返す。
  2. `unlink` を呼び出し、ファイルを削除する（ファイルディスクリプタは有効なまま）。
  3. `dup2` でファイルディスクリプタを標準入力 (`STDIN_FILENO`) に複製する。
  4. ファイルディスクリプタを閉じる。
  5. 成功したら `0` を返す。

## 関数: handle_single_redirection
- **引数**:
  - `redir`: リダイレクト構造体
  - `data`: シェルの主要データ構造体
- **処理**:
  1. リダイレクトの種類に応じて処理を分岐する:
     - `TOKEN_REDIR_IN`: `handle_input_redir` を呼び出す。
     - `TOKEN_REDIR_OUT`: `handle_output_redir` (append=0) を呼び出す。
     - `TOKEN_REDIR_APPEND`: `handle_output_redir` (append=1) を呼び出す。
     - `TOKEN_REDIR_HEREDOC`: `handle_heredoc_redir` を呼び出す。
  2. いずれかの処理が失敗したら `-1` を返す。
  3. 成功したら `0` を返す。

## 関数: setup_redirections
- **引数**:
  - `redirs`: リダイレクトリスト
  - `data`: シェルの主要データ構造体
- **処理**:
  1. リダイレクトリストを走査するループ:
     - `handle_single_redirection` を呼び出す。
     - 失敗したら `-1` を返す。
     - 次のリダイレクトへ進む。
  2. すべて成功したら `0` を返す。
