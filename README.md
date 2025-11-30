# Minishell

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Build Status](https://github.com/msakatav/minishell/actions/workflows/ci.yml/badge.svg)
![Norminette](https://img.shields.io/badge/norminette-passing-success)
![Repo Size](https://img.shields.io/github/repo-size/msakatav/minishell)
![Code Size](https://img.shields.io/github/languages/code-size/msakatav/minishell)

Bashにインスパイアされたシンプルなシェル実装です。このプロジェクトは42のカリキュラムの一部です。

## 概要

Minishellは、シェルの主要な機能を再現したコマンドラインインタープリタです。ユーザーはコマンドの実行、環境変数の管理、パイプやリダイレクトなどの機能を使用できます。

## 機能

- **コマンド実行**: バイナリおよびシェルコマンドを実行します。
- **ビルトインコマンド**:
  - `echo` (`-n` オプション対応)
  - `cd` (相対パス、絶対パス、`-` による移動)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **リダイレクト**:
  - `<` (入力リダイレクト)
  - `>` (出力リダイレクト)
  - `>>` (追記出力リダイレクト)
  - `<<` (ヒアドキュメント)
- **パイプ**: パイプ (`|`) を使用してコマンドを接続します。
- **環境変数**: `$` を使用して変数を展開します。
- **終了ステータス**: `$?` を使用して最後のコマンドの終了ステータスを確認します。
- **シグナルハンドリング**: `Ctrl-C`、`Ctrl-D`、`Ctrl-\` を適切に処理します。
- **コマンド履歴**: 上下矢印キーでコマンド履歴を操作できます。

## 使用可能関数

このプロジェクトで使用されている外部関数の一覧については、[こちら](ALLOWED_FUNCTIONS.md)を参照してください。

## 設計・実装ドキュメント

各機能の実装ロジックや疑似コードは [pseudo_code](./pseudo_code) ディレクトリにまとめています。

## 要件

- GCC または Clang
- Make
- Readline ライブラリ

## インストール

リポジトリをクローンし、プロジェクトをコンパイルします：

```bash
git clone https://github.com/msakatav/minishell.git
cd minishell
make
```

## 使用方法

シェルを起動します：

```bash
./target/minishell
```

シェル内では、bashと同様にコマンドを入力できます：

```bash
minishell$ ls -l | grep src
minishell$ echo "Hello World" > file.txt
minishell$ cat < file.txt
```

## テスト

テストを実行するには：

```bash
make test
```

## 著者

- msakata
