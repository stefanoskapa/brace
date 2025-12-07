#/bin/bash
FOLDER="tests/testfiles"


assertTrue() {
    if [ $? -eq 0 ]; then
        echo "SUCCESS"
    else
        echo "FAILURE"
    fi
}

assertFalse() {
    if [ $? -eq 0 ]; then
        echo "FAILURE"
    else
        echo "SUCCESS"
    fi
}

echo "Test 1"
./brace -s "{}" "$FOLDER/empty_file.txt" >/dev/null 2>&1
assertTrue
echo


echo "Test 2"
./brace -s "{}" "$FOLDER/one_closing_sym.txt" >/dev/null 2>&1
assertFalse
echo

echo "Test 3"
./brace -s "{}" "$FOLDER/one_opening_sym.txt" >/dev/null 2>&1
assertFalse
echo


echo "Test 4"
./brace -s "{}" "$FOLDER/happy_curly_1.txt" >/dev/null 2>&1
assertTrue
echo


echo "Test 5"
./brace -s "{}" "$FOLDER/happy_curly_2.txt" >/dev/null 2>&1
assertTrue
echo


echo "Test 6"
./brace -s "{}" "$FOLDER/bad_curly_1.txt" >/dev/null 2>&1
assertFalse
echo

echo "Test 7"
./brace "$FOLDER/bad_curly_1.txt" >/dev/null 2>&1
assertFalse
echo


echo "Test 8"
./brace -s "()" "$FOLDER/bad_curly_1.txt" >/dev/null 2>&1
assertTrue
echo


echo "Test 9"
./brace "$FOLDER/bad_curly_2.txt" >/dev/null 2>&1
assertFalse
echo


echo "Test 10"
cat "$FOLDER/bad_curly_1.txt" | ./brace  >/dev/null 2>&1
assertFalse
echo

echo "Test 11"
cat "$FOLDER/happy_mixed_1.txt" | ./brace  >/dev/null 2>&1
assertTrue
echo

echo "Test 12"
cat "$FOLDER/happy_mixed_2.txt" | ./brace  >/dev/null 2>&1
assertTrue
echo

echo "Test 13"
./brace "$FOLDER/happy_mixed_2.txt" >/dev/null 2>&1
assertTrue
echo
