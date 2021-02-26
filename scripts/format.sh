set -e
test "${PWD##*/}" == "fluffy-computing-machine"
find . -regextype awk -regex '^\./(app|include|src|tests).*\.(cpp|hpp)' -exec clang-format -i {} \;
