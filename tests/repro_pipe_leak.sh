#!/bin/bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=.suppression ./target/minishell <<EOF
echo abc | echo abc
exit
EOF
