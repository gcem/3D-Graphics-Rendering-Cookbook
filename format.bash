#!/bin/bash

find -regextype posix-extended -regex '.*\.(cpp|c|h)' -and -not -regex '.*\b(glad|KHR)\b.*' | parallel -j "200%" clang-format -i "{}"
