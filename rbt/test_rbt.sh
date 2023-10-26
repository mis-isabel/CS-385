#!/bin/bash

targetfile=rbtree.h
cppfile=testrbt.cpp
maxtime=8

if [ ! -f "$targetfile" ]; then
    echo "Error: file $targetfile not found"
    echo "Final score: score - penalties = 0 - 0 = 0"
    exit 1
fi

# Required by the Honor System
missing_name=0
head -n 20 "$targetfile" | egrep -i "author.*[a-zA-Z]+"
if [ $? -ne 0 ]; then
    echo "Student name missing"
    missing_name=1
fi

# Required by the Honor System
missing_pledge=0
head -n 20 "$targetfile" | egrep -i "I.*pledge.*my.*honor.*that.*I.*have.*abided.*by.*the.*Stevens.*Honor.*System"
if [ $? -ne 0 ]; then
    echo -e "Pledge missing"
    missing_pledge=1
fi

# Compiling
echo
results=$(make 2>&1)
if [ $? -ne 0 ]; then
    echo "$results"
    echo "Final score: score - penalties = 0 - 0 = 0"
    exit 1
fi

num_tests=0
num_right=0
memory_problems=0
command="./${cppfile%.*}"

run_test_with_args_and_input() {
    ((num_tests++))
    echo -n "Test $num_tests..."

    args="$1"
    input="$2"
    expected_output="$3"

    outputfile=$(mktemp)
    inputfile=$(mktemp)
    statusfile=$(mktemp)

    echo -e "$input" > "$inputfile"

    start=$(date +%s.%N)
    # Run run run, little program!
    (timeout --preserve-status "$maxtime" "$command" $args < "$inputfile" &> "$outputfile"; echo $? > "$statusfile") &> /dev/null
    end=$(date +%s.%N)
    status=$(cat "$statusfile")

    case $status in
        # $command: 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
        135)
            echo "failed (bus error)"
            ;;
        # $command: 128 + SIGSEGV = 128 + 11 = 139
        139)
            echo "failed (segmentation fault)"
            ;;
        # $command: 128 + SIGTERM (sent by timeout(1)) = 128 + 15 = 143
        143)
            echo "failed (time out)"
            ;;
        *)
            # bash doesn't like null bytes so we substitute by hand.
            computed_output=$(sed -e 's/\x0/(NULL BYTE)/g' "$outputfile")
            if [ "$computed_output" = "$expected_output" ]; then
                ((num_right++))
                echo $start $end | awk '{printf "ok (%.3fs)\tvalgrind...", $2 - $1}'
                # Why 93?  Why not 93!
                (valgrind --leak-check=full --error-exitcode=93 $command $args < "$inputfile" &> /dev/null; echo $? > "$statusfile") &> /dev/null
                vgstatus=$(cat "$statusfile")
                case $vgstatus in
                    # valgrind detected an error when running $command
                    93)
                        ((memory_problems++))
                        echo "failed"
                        ;;
                    # valgrind not installed or not in $PATH
                    127)
                        echo "not found"
                        ;;
                    # valgrind: 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
                    135)
                        ((memory_problems++))
                        echo "failed (bus error)"
                        ;;
                    # valgrind: 128 + SIGSEGV = 128 + 11 = 139
                    139)
                        ((memory_problems++))
                        echo "failed (segmentation fault)"
                        ;;
                    # compare with expected status from running $command without valgrind
                    $status)
                        echo "ok"
                        ;;
                    *)
                        ((memory_problems++))
                        echo "unknown status $vgstatus"
                        ;;
                esac
            else
                echo "failed"
                echo "==================== Expected ===================="
                echo "$expected_output"
                echo "==================== Received ===================="
                echo "$computed_output"
                echo "=================================================="
            fi
            ;;
    esac
    rm -f "$inputfile" "$outputfile" "$statusfile"
}

run_test_with_args() {
    run_test_with_args_and_input "$1" "" "$2"
}
run_test_with_input() {
    run_test_with_args_and_input "" "$1" "$2"
}

############################################################
run_test_with_args "" "Root is null."$'\n'$'\n'"Height:                   -1"$'\n'"Total nodes:              0"$'\n'"Leaf count:               0"$'\n'"Internal nodes:           0"$'\n'"Diameter:                 0"$'\n'"Maximum width:            0"$'\n'"Successful search cost:   0.000"$'\n'"Unsuccessful search cost: 0.000"$'\n'"Inorder traversal:        []"
run_test_with_args "50" "50"$'\n'$'\n'"Height:                   0"$'\n'"Total nodes:              1"$'\n'"Leaf count:               1"$'\n'"Internal nodes:           0"$'\n'"Diameter:                 0"$'\n'"Maximum width:            1"$'\n'"Successful search cost:   1.000"$'\n'"Unsuccessful search cost: 1.000"$'\n'"Inorder traversal:        [50]"
run_test_with_args "50 60" "50"$'\n'"  \\"$'\n'"  60"$'\n'$'\n'"Height:                   1"$'\n'"Total nodes:              2"$'\n'"Leaf count:               1"$'\n'"Internal nodes:           1"$'\n'"Diameter:                 1"$'\n'"Maximum width:            1"$'\n'"Successful search cost:   1.500"$'\n'"Unsuccessful search cost: 1.667"$'\n'"Inorder traversal:        [50, 60]"
run_test_with_args "50 60 70" " 60"$'\n'" / \\"$'\n'"50 70"$'\n'$'\n'"Height:                   1"$'\n'"Total nodes:              3"$'\n'"Leaf count:               2"$'\n'"Internal nodes:           1"$'\n'"Diameter:                 2"$'\n'"Maximum width:            2"$'\n'"Successful search cost:   1.667"$'\n'"Unsuccessful search cost: 2.000"$'\n'"Inorder traversal:        [50, 60, 70]"
run_test_with_args "50 60 70 40 30 20 50" "Warning: Attempt to insert duplicate key '50'."$'\n'"     60"$'\n'"     / \\"$'\n'"    40 70"$'\n'"   / \\"$'\n'"  30 50"$'\n'" /"$'\n'"20"$'\n'$'\n'"Height:                   3"$'\n'"Total nodes:              6"$'\n'"Leaf count:               3"$'\n'"Internal nodes:           3"$'\n'"Diameter:                 4"$'\n'"Maximum width:            2"$'\n'"Successful search cost:   2.500"$'\n'"Unsuccessful search cost: 3.000"$'\n'"Inorder traversal:        [20, 30, 40, 50, 60, 70]"
run_test_with_args "23 56 -8 90 25 18 42 99 -5" "     23"$'\n'"     / \\"$'\n'"    /   \\"$'\n'"   /     \\"$'\n'"  -5     56"$'\n'" / \\     / \\"$'\n'"-8 18   /   \\"$'\n'"       25   90"$'\n'"        \\     \\"$'\n'"        42    99"$'\n'$'\n'"Height:                   3"$'\n'"Total nodes:              9"$'\n'"Leaf count:               4"$'\n'"Internal nodes:           5"$'\n'"Diameter:                 5"$'\n'"Maximum width:            4"$'\n'"Successful search cost:   2.778"$'\n'"Unsuccessful search cost: 3.400"$'\n'"Inorder traversal:        [-8, -5, 18, 23, 25, 42, 56, 90, 99]"
run_test_with_args "Jim Connie Mike Michelle John Amy Bill Kyle Jill Mike Jill" "Warning: Attempt to insert duplicate key 'Mike'."$'\n'"Warning: Attempt to insert duplicate key 'Jill'."$'\n'"         Jim"$'\n'"         / \\"$'\n'"        /   \\"$'\n'"       /     \\"$'\n'"      /       \\"$'\n'"     /         \\"$'\n'"   Bill     Michelle"$'\n'"   / \\         / \\"$'\n'"  /   \\       /   \\"$'\n'"Amy Connie   /     \\"$'\n'"        \\  John   Mike"$'\n'"       Jill  \\"$'\n'"            Kyle"$'\n'$'\n'"Height:                   3"$'\n'"Total nodes:              9"$'\n'"Leaf count:               4"$'\n'"Internal nodes:           5"$'\n'"Diameter:                 6"$'\n'"Maximum width:            4"$'\n'"Successful search cost:   2.778"$'\n'"Unsuccessful search cost: 3.400"$'\n'"Inorder traversal:        [Amy, Bill, Connie, Jill, Jim, John, Kyle, Michelle, Mike]"
run_test_with_args "A Z B Y C X D W E V F U G T H S I R J Q K P L O M N" "              H"$'\n'"             / \\"$'\n'"            /   \\"$'\n'"           /     \\"$'\n'"          /       \\"$'\n'"         /         \\"$'\n'"        /           \\"$'\n'"       /             \\"$'\n'"      D               S"$'\n'"     / \\             / \\"$'\n'"    /   \\           /   \\"$'\n'"   /     \\         /     \\"$'\n'"  B       F       /       \\"$'\n'" / \\     / \\     /         \\"$'\n'"A   C   E   G   /           \\"$'\n'"               /             \\"$'\n'"              L               W"$'\n'"             / \\             / \\"$'\n'"            /   \\           /   \\"$'\n'"           /     \\         /     \\"$'\n'"          J       Q       U       Y"$'\n'"         / \\     / \\     / \\     / \\"$'\n'"        I   K   O   R   T   V   X   Z"$'\n'"               / \\"$'\n'"              M   P"$'\n'"               \\"$'\n'"                N"$'\n'$'\n'"Height:                   6"$'\n'"Total nodes:              26"$'\n'"Leaf count:               13"$'\n'"Internal nodes:           13"$'\n'"Diameter:                 9"$'\n'"Maximum width:            8"$'\n'"Successful search cost:   4.154"$'\n'"Unsuccessful search cost: 4.963"$'\n'"Inorder traversal:        [A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z]"
run_test_with_args "30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1" "                             23"$'\n'"                             / \\"$'\n'"                            /   \\"$'\n'"                           /     \\"$'\n'"                          /       \\"$'\n'"                         /         \\"$'\n'"                        /           \\"$'\n'"                       /             \\"$'\n'"                      15             27"$'\n'"                     / \\             / \\"$'\n'"                    /   \\           /   \\"$'\n'"                   /     \\         /     \\"$'\n'"                  /       \\       25     29"$'\n'"                 /         \\     / \\     / \\"$'\n'"                /           \\   24 26   28 30"$'\n'"               /             \\"$'\n'"              11             19"$'\n'"             / \\             / \\"$'\n'"            /   \\           /   \\"$'\n'"           /     \\         /     \\"$'\n'"          7      13       17     21"$'\n'"         / \\     / \\     / \\     / \\"$'\n'"        /   \\   12 14   16 18   20 22"$'\n'"       /     \\"$'\n'"      5       9"$'\n'"     / \\     / \\"$'\n'"    3   6   8  10"$'\n'"   / \\"$'\n'"  2   4"$'\n'" /"$'\n'"1"$'\n'$'\n'"Height:                   7"$'\n'"Total nodes:              30"$'\n'"Leaf count:               15"$'\n'"Internal nodes:           15"$'\n'"Diameter:                 10"$'\n'"Maximum width:            8"$'\n'"Successful search cost:   4.500"$'\n'"Unsuccessful search cost: 5.323"$'\n'"Inorder traversal:        [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30]"
run_test_with_args "Engling Borowski Duggan Naumann Damopoulos Wetzel Nicolosi Ateniese Bonelli Mordohai Klappholz Kleinberg Portokalidis Wang Compagnoni" "                  Duggan"$'\n'"                    / \\"$'\n'"                   /   \\"$'\n'"                  /     \\"$'\n'"                 /       \\"$'\n'"                /         \\"$'\n'"               /           \\"$'\n'"              /             \\"$'\n'"             /               \\"$'\n'"            /                 \\"$'\n'"           /                   \\"$'\n'"       Borowski              Naumann"$'\n'"         / \\                   / \\"$'\n'"        /   \\                 /   \\"$'\n'"       /     \\               /     \\"$'\n'"      /       \\             /       \\"$'\n'"     /         \\           /         \\"$'\n'"    /           \\         /           \\"$'\n'"Ateniese    Damopoulos   /             \\"$'\n'"    \\           /       /               \\"$'\n'"  Bonelli  Compagnoni  /                 \\"$'\n'"                      /                   \\"$'\n'"                 Klappholz           Portokalidis"$'\n'"                    / \\                   / \\"$'\n'"                   /   \\                 /   \\"$'\n'"                  /     \\               /     \\"$'\n'"                 /       \\             /       \\"$'\n'"                /         \\           /         \\"$'\n'"            Engling    Mordohai   Nicolosi    Wetzel"$'\n'"                          /                     /"$'\n'"                     Kleinberg                Wang"$'\n'$'\n'"Height:                   4"$'\n'"Total nodes:              15"$'\n'"Leaf count:               6"$'\n'"Internal nodes:           9"$'\n'"Diameter:                 7"$'\n'"Maximum width:            6"$'\n'"Successful search cost:   3.400"$'\n'"Unsuccessful search cost: 4.125"$'\n'"Inorder traversal:        [Ateniese, Bonelli, Borowski, Compagnoni, Damopoulos, Duggan, Engling, Klappholz, Kleinberg, Mordohai, Naumann, Nicolosi, Portokalidis, Wang, Wetzel]"
############################################################
echo
echo "Total tests run: $num_tests"
echo "Number correct : $num_right"
score=$((100 * $num_right / $num_tests))
echo "Percent correct: $score%"
if [ $missing_name == 1 ]; then
    echo "Missing Name: -5"
fi
if [ $missing_pledge == 1 ]; then
    echo "Missing or incorrect pledge: -5"
fi

if [ $memory_problems -gt 1 ]; then
    echo "Memory problems: $memory_problems (-5 each, max of -15)"
    if [ $memory_problems -gt 3 ]; then
        memory_problems=3
    fi
fi

penalties=$((5 * $missing_name + 5 * $missing_pledge + 5 * $memory_problems))
final_score=$(($score - $penalties))
if [ $final_score -lt 0 ]; then
    final_score=0
fi
echo "Final score: score - penalties = $score - $penalties = $final_score"

make clean > /dev/null 2>&1
