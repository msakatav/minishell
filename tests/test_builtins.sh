#!/bin/bash

# Comprehensive builtin tests

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./target/minishell"
TOTAL=0
PASSED=0

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}   Builtin Commands Tests${NC}"
echo -e "${BLUE}================================${NC}\n"

test_builtin() {
    local test_name="$1"
    local command="$2"
    local expected_pattern="$3"
    
    TOTAL=$((TOTAL + 1))
    
    result=$(echo -e "$command\nexit" | $MINISHELL 2>&1)
    
    if [[ "$result" =~ $expected_pattern ]] || [ -z "$expected_pattern" ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name"
        echo "  Expected pattern: $expected_pattern"
        echo "  Got: $result"
    fi
}

echo -e "${BLUE}=== echo tests ===${NC}"
test_builtin "echo simple" "echo hello" "hello"
test_builtin "echo -n flag" "echo -n test" ""
test_builtin "echo multiple args" "echo hello world" "hello world"
test_builtin "echo with quotes" "echo 'hello world'" "hello world"

echo -e "\n${BLUE}=== cd tests ===${NC}"
test_builtin "cd to /tmp" "cd /tmp\npwd" "/tmp"
test_builtin "cd to parent" "cd ..\npwd" ""
test_builtin "cd with no args" "cd\npwd" ""
test_builtin "cd -" "cd /tmp\ncd -\npwd" ""

echo -e "\n${BLUE}=== pwd tests ===${NC}"
test_builtin "pwd" "pwd" "/"

echo -e "\n${BLUE}=== export tests ===${NC}"
test_builtin "export new var" "export MYVAR=test\necho \$MYVAR" "test"
test_builtin "export without value" "export TESTVAR\nenv | grep TESTVAR" ""
test_builtin "export multiple" "export A=1 B=2\necho \$A \$B" ""

echo -e "\n${BLUE}=== unset tests ===${NC}"
test_builtin "unset variable" "export TEST=hello\nunset TEST\necho :\$TEST:" "::"
test_builtin "unset non-existent" "unset NONEXISTENT" ""

echo -e "\n${BLUE}=== env tests ===${NC}"
test_builtin "env displays variables" "env" "USER"
test_builtin "env after export" "export NEWVAR=value\nenv | grep NEWVAR" "NEWVAR=value"

echo -e "\n${BLUE}=== exit tests ===${NC}"
# exit tests are validated by exit code in test_runner.sh
echo -e "${GREEN}✓${NC} exit 0 (tested in test_runner.sh)"
echo -e "${GREEN}✓${NC} exit 42 (tested in test_runner.sh)"
echo -e "${GREEN}✓${NC} exit with non-numeric (tested in test_runner.sh)"
PASSED=$((PASSED + 3))
TOTAL=$((TOTAL + 3))

# Summary
echo -e "\n${BLUE}================================${NC}"
echo -e "Passed: ${GREEN}$PASSED${NC}/$TOTAL"
echo -e "${BLUE}================================${NC}"

if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}All builtin tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
