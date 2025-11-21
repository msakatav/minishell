# Minishell Test Suite

このディレクトリには、minishellの品質を保証するための包括的なテストが含まれています。

## テストの種類

### 1. test_runner.sh
基本的な機能テストとbash互換性テスト
- 基本コマンド (echo, pwd, env)
- ビルトインコマンド (cd, export, unset, exit)
- 変数展開
- パイプ
- リダイレクション
- heredoc
- エラーハンドリング
- クォート処理
- エッジケース

### 2. test_builtins.sh
ビルトインコマンドの詳細テスト
- echo (オプション、引数)
- cd (様々なパス、-, エラーケース)
- pwd
- export (単一、複数、値なし)
- unset
- env
- exit (終了コード)

### 3. test_parser.sh
パーサーとレキサーのテスト
- クォート処理
- 変数展開
- パイプのパース
- リダイレクションのパース
- エッジケース

## テストの実行方法

### すべてのテストを実行
```bash
make test
```

### 個別のテストを実行
```bash
./tests/test_runner.sh      # 機能テスト
./tests/test_builtins.sh    # ビルトインテスト
./tests/test_parser.sh      # パーサーテスト
```

## CI/CD統合

これらのテストはGitHub Actionsで自動的に実行されます。
`.github/workflows/ci.yml`を参照してください。

### サポートされているプラットフォーム
- Ubuntu (Linux)
- MacOS

## テスト結果の見方

- ✓ (緑) - テスト成功
- ✗ (赤) - テスト失敗
- ~ (黄) - 出力が異なるが許容範囲
- ⊘ (黄) - スキップされたテスト

## 新しいテストの追加

各テストスクリプトに新しいテストケースを追加できます：

```bash
run_test "テスト名" "実行するコマンド\nexit" 期待される終了コード
```

## 必要な依存関係

### Linux (Ubuntu)
- make
- gcc
- libreadline-dev

### MacOS
- make
- gcc (Xcode Command Line Tools)
- readline (Homebrew経由)
