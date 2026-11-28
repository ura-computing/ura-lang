# config.sh - source it to get the ura-lang commands in your shell:
#
#   source config.sh
#
# Every command below just runs main.lua, which does the real work.
#
#   build [-D DEBUG=true]       rebuild bin/ura
#   tests [target] [number]     compile and check the tests
#   record [target] [number]    (re)write ucg.md, llvm.md and output.md of the tests
#   copy <ura_file> <file> <description>
#   run <target> <number>       compile one test and run it
#   extension                   install the VS Code extension
#   set-llvm [version]          list the LLVMs, or use one in this shell (LLVM_VERSION)
#   update                      re-source this file
#
# Works when sourced from either bash or zsh. make dev starts bash with this
# file as its rcfile.

# started by make dev: load the user's bashrc first, and forget make's variables
if [ -n "$MAKELEVEL" ]; then
    [ -f "$HOME/.bashrc" ] && . "$HOME/.bashrc"
    unset MAKELEVEL MAKEFLAGS MFLAGS
fi

# where this file lives: bash and zsh report it differently when sourced
if [ -n "$ZSH_VERSION" ]; then
    _ura_self=$0
else
    _ura_self=${BASH_SOURCE[0]}
fi
URA_ROOT="$(cd "$(dirname "$_ura_self")" && pwd)"
unset _ura_self

# bin/ on PATH, so `ura` is callable by name (once, even when re-sourced)
URA_BIN_DIR="$(cd "$URA_ROOT/.." && pwd)/bin"
case ":$PATH:" in
    *":$URA_BIN_DIR:"*) ;;
    *) PATH="$URA_BIN_DIR:$PATH" ;;
esac
export PATH

# prompt: prepend "(ura-lang) " in green, once
if [ -z "$URA_LANG_PS1_SET" ]; then
    URA_LANG_PS1_SET=1
    if [ -n "$ZSH_VERSION" ]; then
        PS1="%F{green}(ura-lang)%f $PS1"
    else
        PS1="\[\033[0;32m\](ura-lang) \[\033[0m\]$PS1"
    fi
fi

# the commands
build()     { lua "$URA_ROOT/main.lua" build "$@"; }
tests()     { lua "$URA_ROOT/main.lua" tests "$@"; }
record()    { lua "$URA_ROOT/main.lua" record "$@"; }
copy()      { lua "$URA_ROOT/main.lua" copy "$@"; }
run()       { lua "$URA_ROOT/main.lua" run "$@"; }
extension() { lua "$URA_ROOT/main.lua" extension "$@"; }

# no version: list the LLVMs; with one: check it, then use it in this shell
set-llvm() {
    if [ -z "$1" ]; then
        lua "$URA_ROOT/main.lua" set-llvm
        return
    fi
    lua "$URA_ROOT/main.lua" set-llvm "$1" || return 1
    export LLVM_VERSION="$1"
}

update() { source "$URA_ROOT/config.sh"; }

printf '\033[0;32m(ura-lang)\033[0m ready — commands: build, tests, record, copy, run, extension, set-llvm, update\n'
