# lexer_parse.c 疑似コード

## 関数: extract_part
- **引数**:
  - `input`: 入力文字列
  - `i`: 現在のインデックスへのポインタ
  - `quote_char`: クォート文字を格納するポインタ
- **処理**:
  1. `'` の場合: `quote_char` に `'` を設定し、`handle_quotes` を呼び出して返す。
  2. `"` の場合: `quote_char` に `"` を設定し、`handle_quotes` を呼び出して返す。
  3. その他: `quote_char` に `0` を設定し、`handle_word` を呼び出して返す。

## 関数: append_part
- **引数**:
  - `result`: 結果文字列へのポインタ
  - `part`: 追加する部分文字列
- **処理**:
  1. `result` と `part` を結合し、新しい文字列を `result` に格納する。
  2. 古い `result` と `part` を解放する。

## 関数: protect_spaces
- **引数**:
  - `str`: 文字列
- **処理**:
  1. 文字列中の空白文字（スペース、タブ、改行など）を対応する特殊文字（ASCII 1-6）に置換する。

## 関数: process_part
- **引数**:
  - `part`: 部分文字列
  - `quote`: クォートタイプ
  - `data`: シェルの主要データ構造体
- **処理**:
  1. クォートがシングルクォートでない場合:
     - `expand_variables` で変数を展開する。
     - 元の `part` を解放する。
     - クォートがダブルクォートの場合、`protect_spaces` を呼び出して空白を保護する。
     - 展開後の文字列を返す。
  2. シングルクォートの場合:
     - `protect_spaces` を呼び出して空白を保護する。
     - そのまま返す。

## 関数: build_combined_word
- **引数**:
  - `input`: 入力文字列
  - `i`: 現在のインデックスへのポインタ
  - `quote_type`: クォートタイプを格納するポインタ
  - `data`: シェルの主要データ構造体
- **処理**:
  1. 結果文字列 `result` を空文字で初期化する。
  2. `has_unquoted` フラグを `0` に初期化する。
  3. `quote_type` を `0` に初期化する。
  4. 区切り文字（`is_separator`）以外が続く間ループする:
     - `extract_part` で部分文字列を抽出する。
     - `process_part` で変数展開と空白保護を行う。
     - クォートタイプを判定し、`has_unquoted` フラグや `quote_type` を更新する。
     - `append_part` で結果に結合する。
  5. `has_unquoted` が真なら `quote_type` を `0` に設定する。
  6. `result` を返す。
