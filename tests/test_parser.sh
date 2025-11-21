#!/bin/bash

# Parser and lexer tests

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./target/minishell"
TOTAL=0
PASSED=0

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}   Parser/Lexer Tests${NC}"
echo -e "${BLUE}================================${NC}\n"

test_parse() {
    local test_name="$1"
    local command="$2"
    
    TOTAL=$((TOTAL + 1))
    
    # Just check if it doesn't crash
    echo -e "$command\nexit" | $MINISHELL > /dev/null 2>&1
    exit_code=$?
    
    if [ $exit_code -lt 128 ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name (crashed)"
    fi
}

echo -e "${BLUE}=== Quote handling ===${NC}"
test_parse "single quotes" "echo 'hello world'"
test_parse "double quotes" "echo \"hello world\""
test_parse "mixed quotes" "echo \"hello\"'world'"
test_parse "nested quotes" "echo \"it's a test\""
test_parse "quote concatenation" "echo hello'world'test"
test_parse "empty quotes" "echo '' \"\" ''"

echo -e "\n${BLUE}=== Variable expansion ===${NC}"
test_parse "simple variable" "echo \$USER"
test_parse "variable in quotes" "echo \"\$USER\""
test_parse "no expand in single" "echo '\$USER'"
test_parse "exit status" "echo \$?"
test_parse "invalid variable" "echo \$123"
test_parse "dollar with space" "echo \$ "

echo -e "\n${BLUE}=== Pipe parsing ===${NC}"
test_parse "simple pipe" "echo hello | cat"
test_parse "multiple pipes" "echo test | cat | cat | cat"
test_parse "pipe with quotes" "echo 'hello | world' | cat"

echo -e "\n${BLUE}=== Redirection parsing ===${NC}"
test_parse "output redirect" "echo test > /tmp/test.txt"
test_parse "input redirect" "cat < /tmp/test.txt"
test_parse "append redirect" "echo test >> /tmp/test.txt"
test_parse "multiple redirects" "cat < /tmp/in.txt > /tmp/out.txt"
test_parse "redirect with quotes" "cat > '/tmp/test file.txt'"

echo -e "\n${BLUE}=== Edge cases ===${NC}"
test_parse "empty input" ""
test_parse "only spaces" "   "
test_parse "multiple spaces" "echo    hello    world"
test_parse "tabs" "echo\thello"
test_parse "special chars" "echo !@#%^&*()"

# Cleanup
rm -f /tmp/test.txt /tmp/in.txt /tmp/out.txt /tmp/test\ file.txt

# Summary
echo -e "\n${BLUE}================================${NC}"
echo -e "Passed: ${GREEN}$PASSED${NC}/$TOTAL"
echo -e "${BLUE}================================${NC}"

if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}All parser tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
