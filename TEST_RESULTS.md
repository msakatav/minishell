# Minishell テスト結果

## ✅ 実装完了・動作確認済みの機能

### 基本機能
- [x] プロンプト表示 (readline使用)
- [x] コマンド履歴 (add_history使用)
- [x] 実行ファイルの検索と起動
  - PATH変数からの検索
  - 相対パス/絶対パスでの実行

### クォート処理
- [x] シングルクォート `'` - メタ文字を全て無視
- [x] ダブルクォート `"` - `$`のみ展開
- [x] 混在したクォート処理

### リダイレクト
- [x] `<` 入力リダイレクト
- [x] `>` 出力リダイレクト
- [x] `<<` heredoc (履歴に残らない)
- [x] `>>` 追記モード
- [x] 複数のリダイレクト

### パイプ
- [x] 単一パイプ `|`
- [x] 複数パイプの連結
- [x] パイプとリダイレクトの組み合わせ

### 環境変数
- [x] `$VAR` 形式の変数展開
- [x] `$?` 終了ステータスの展開
- [x] シングルクォート内では展開しない
- [x] ダブルクォート内では展開する

### ビルトインコマンド
- [x] `echo` (-n オプション対応、複数の -n も対応)
- [x] `cd` (相対/絶対パス、`cd -` 対応)
- [x] `pwd`
- [x] `export` (引数なしで一覧表示、変数の設定)
- [x] `unset`
- [x] `env`
- [x] `exit` (引数なし、数値引数、エラーハンドリング)

### エラー処理
- [x] コマンドが見つからない場合 (exit status: 127)
- [x] 権限エラー (exit status: 126)
- [x] ファイルが見つからない場合
- [x] ディレクトリが見つからない場合
- [x] 終了ステータスの正確な記録

### シグナル処理
- [x] Ctrl-C: 新しいプロンプトを表示
- [x] Ctrl-D: シェル終了
- [x] Ctrl-\: 何もしない (SIG_IGN)

## テストケース実行結果

### 基本コマンド
```bash
pwd                    # ✅ 動作
echo Hello World       # ✅ 動作
echo -n test           # ✅ 動作 (改行なし)
echo -nnn test         # ✅ 動作 (複数のn)
```

### 変数展開
```bash
export VAR=value       # ✅ 動作
echo $VAR              # ✅ 動作 (value)
echo '$VAR'            # ✅ 動作 ($VAR そのまま)
echo "$VAR"            # ✅ 動作 (value)
echo $?                # ✅ 動作 (終了ステータス表示)
unset VAR              # ✅ 動作
```

### リダイレクト
```bash
echo test > file.txt          # ✅ 動作
cat < file.txt                # ✅ 動作
echo append >> file.txt       # ✅ 動作
cat << EOF                    # ✅ 動作
```

### パイプ
```bash
ls | grep test                # ✅ 動作
echo a | cat | cat | cat      # ✅ 動作 (複数パイプ)
ls | head -3 | wc -l          # ✅ 動作
```

### ディレクトリ操作
```bash
cd /tmp                # ✅ 動作
pwd                    # ✅ 動作
cd -                   # ✅ 動作 (前のディレクトリに戻る)
cd /nonexistent        # ✅ エラー処理
```

### エラーハンドリング
```bash
nonexistent_command    # ✅ エラー表示 + exit status 127
cd /nonexistent        # ✅ エラー表示 + exit status 1
cat /nonexistent       # ✅ エラー表示 + exit status 1
exit abc               # ✅ エラー表示 + exit status 255
```

## 注意事項

1. **グローバル変数**: `g_signal_received` のみ使用 (シグナル番号格納用)
2. **メモリリーク**: readlineライブラリ自体のリークを除き、独自コードではリークなし
3. **許可関数**: subjectで指定された関数のみ使用
4. **Norminette**: 42のコーディング規約に準拠

## コンパイルとクリーン

```bash
make        # コンパイル
make clean  # オブジェクトファイル削除
make fclean # すべてのビルド成果物削除
make re     # 再コンパイル
```

## まとめ

42 School minishellプロジェクトのmandatory要件を全て実装・テスト完了しました。
基本的なシェル機能、ビルトインコマンド、リダイレクト、パイプ、変数展開、
シグナル処理など、すべての必須機能が正常に動作することを確認しました。
