#!/bin/bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=.suppression ./target/minishell <<EOF
cat << END
^C
exit
EOF
