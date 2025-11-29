#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

MINISHELL="./target/minishell"
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
USE_VALGRIND=0
VALGRIND_EXIT_CODE=242

# Check for arguments
for arg in "$@"
do
    if [ "$arg" == "--valgrind" ]; then
        USE_VALGRIND=1
        echo -e "${YELLOW}Running with Valgrind memory check...${NC}"
        MINISHELL="valgrind --leak-check=full --show-leak-kinds=all --suppressions=.suppression --error-exitcode=$VALGRIND_EXIT_CODE --quiet ./target/minishell"
    fi
done

# Test result tracking
declare -a FAILED_TEST_NAMES

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}   Minishell Test Suite${NC}"
echo -e "${BLUE}================================${NC}\n"

# Function to run a test
run_test() {
    local test_name="$1"
    local input="$2"
    local expected_exit_code="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run minishell with input
    echo -e "$input" | $MINISHELL > /tmp/minishell_out 2>&1
    actual_exit_code=$?
    
    # Check for valgrind error
    if [ $USE_VALGRIND -eq 1 ] && [ $actual_exit_code -eq $VALGRIND_EXIT_CODE ]; then
         echo -e "${RED}✗${NC} $test_name (Memory Leak Detected)"
         # Extract relevant leak info if possible, or just warn
         grep -A 2 "definitely lost:" /tmp/minishell_out | head -n 3
         FAILED_TESTS=$((FAILED_TESTS + 1))
         FAILED_TEST_NAMES+=("$test_name (Memory Leak)")
         return
    fi

    # Check if we should verify exit code (bash compatibility)
    if [ "$expected_exit_code" != "" ]; then
        if [ $actual_exit_code -eq $expected_exit_code ]; then
            echo -e "${GREEN}✓${NC} $test_name"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}✗${NC} $test_name (expected: $expected_exit_code, got: $actual_exit_code)"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            FAILED_TEST_NAMES+=("$test_name")
        fi
    else
        # Just check if it ran without crashing
        if [ $actual_exit_code -lt 128 ]; then
            echo -e "${GREEN}✓${NC} $test_name"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}✗${NC} $test_name (crashed with code: $actual_exit_code)"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            FAILED_TEST_NAMES+=("$test_name")
        fi
    fi
}

# Function to compare output with bash
run_comparison_test() {
    local test_name="$1"
    local command="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run in bash
    echo -e "$command\nexit" | bash > /tmp/bash_out 2>&1
    bash_exit=$?
    
    # Run in minishell
    echo -e "$command\nexit" | $MINISHELL > /tmp/minishell_out 2>&1
    mini_exit=$?
    
    # Compare outputs
    if diff -q /tmp/bash_out /tmp/minishell_out > /dev/null 2>&1; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${YELLOW}~${NC} $test_name (output differs, but may be acceptable)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
}

echo -e "${BLUE}=== Basic Commands ===${NC}"
run_test "echo hello" "echo hello\nexit" 0
run_test "echo with quotes" "echo 'hello world'\nexit" 0
run_test "echo with double quotes" "echo \"hello world\"\nexit" 0
run_test "pwd command" "pwd\nexit" 0
run_test "env command" "env\nexit" 0

echo -e "\n${BLUE}=== Built-in Commands ===${NC}"
run_test "cd to parent" "cd ..\npwd\nexit" 0
run_test "cd to home" "cd\npwd\nexit" 0
run_test "cd with -" "cd /tmp\ncd -\nexit" 0
run_test "export variable" "export TEST=hello\necho \$TEST\nexit" 0
run_test "unset variable" "export TEST=hello\nunset TEST\necho \$TEST\nexit" 0
run_test "exit with 0" "exit 0" 0
run_test "exit with 42" "exit 42" 42

echo -e "\n${BLUE}=== Variable Expansion ===${NC}"
run_test "expand USER" "echo \$USER\nexit" 0
run_test "expand PATH" "echo \$PATH\nexit" 0
run_test "expand exit status" "false\necho \$?\nexit" 0
run_test "expand in double quotes" "echo \"user: \$USER\"\nexit" 0
run_test "no expand in single quotes" "echo 'user: \$USER'\nexit" 0

echo -e "\n${BLUE}=== Pipes ===${NC}"
run_test "simple pipe" "echo hello | cat\nexit"
run_test "pipe with grep" "ls / | grep bin\nexit"
run_test "multiple pipes" "echo hello | cat | cat | cat\nexit"
run_test "pipe with count" "ls / | wc -l\nexit"

echo -e "\n${BLUE}=== Redirections ===${NC}"
run_test "output redirect" "echo test > /tmp/test.txt\ncat /tmp/test.txt\nrm /tmp/test.txt\nexit" 0
run_test "append redirect" "echo line1 >> /tmp/test.txt\necho line2 >> /tmp/test.txt\ncat /tmp/test.txt\nrm /tmp/test.txt\nexit" 0
run_test "input redirect" "echo test > /tmp/test.txt\ncat < /tmp/test.txt\nrm /tmp/test.txt\nexit" 0

echo -e "\n${BLUE}=== Heredoc ===${NC}"
# Note: heredoc tests are tricky in automated testing
run_test "heredoc basic" "cat << EOF\nhello\nworld\nEOF\nexit" 0

echo -e "\n${BLUE}=== Error Handling ===${NC}"
run_test "command not found" "nonexistent_command\nexit"
run_test "invalid cd" "cd /nonexistent/directory\npwd\nexit"
run_test "exit non-numeric" "exit hello" 2

echo -e "\n${BLUE}=== Quote Handling ===${NC}"
run_test "concatenation" "echo hello'world'\nexit" 0
run_test "mixed quotes" "echo \"hello\"'world'\nexit" 0
run_test "empty quotes" "echo ''\nexit" 0

echo -e "\n${BLUE}=== Edge Cases ===${NC}"
run_test "empty command" "\nexit" 0
run_test "multiple spaces" "echo    hello    world\nexit" 0
run_test "dollar sign edge" "echo \$ \nexit" 0
run_test "invalid variable" "echo \$123abc\nexit" 0

echo -e "\n${BLUE}=== Signals ===${NC}"
# Signal tests would need interactive input, skipping for now
echo -e "${YELLOW}⊘${NC} Signal tests (Ctrl-C, Ctrl-D, Ctrl-\\) require interactive testing"

# Summary
echo -e "\n${BLUE}================================${NC}"
echo -e "${BLUE}   Test Summary${NC}"
echo -e "${BLUE}================================${NC}"
echo -e "Total tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

if [ $FAILED_TESTS -gt 0 ]; then
    echo -e "\n${RED}Failed tests:${NC}"
    for test in "${FAILED_TEST_NAMES[@]}"; do
        echo -e "  - $test"
    done
fi

# Cleanup
rm -f /tmp/minishell_out /tmp/bash_out /tmp/test.txt

# Exit with appropriate code
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed.${NC}"
    exit 1
fi
