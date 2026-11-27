/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        ura.h                           */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/27 02:32:25    */
/*                                                             */
/*   updated: 2026/11/27 06:23:56                              */
/*                                                             */
/* *********************************************************** */

#pragma once

#define URA_VERSION "0.0.1"

#ifndef TOKENIZE
#    define TOKENIZE 1
#endif

#if TOKENIZE
#    define AST 1
#endif

#if AST
#    define ANALYZE 1
#endif

#if ANALYZE
#    define ASM 1
#endif

typedef struct Token Token;
typedef struct Node Node;
typedef struct Keyword Keyword;

struct Token {
    Type type;
    char *name;
	
    char *filename;
    int line;
    int space;
    int s;
    int e;

    bool is_type;
    bool is_varg;
    bool is_pub;
    bool is_uint;
    bool is_used; // for optimization

    Ucg *ucg;

#if DEBUG
	int tk_index;
#endif
	
    union { // clang-format off
        struct { int bits; char *num; }; // INT, FLOAT
		int index; // attribute index
		char *string; // string litteral
    }; // clang-format on
};

struct Node {
	Token *token;
	
	union {
        Node *left;
        Node *var;
        Node *data_type;
        Node *ret_type;
        Node *cond;
        Node *to_call;
        Node *loop;
        Node *operand;
        Node *scope;
        Node *symbol;
        Node *url;
        Node *version;
        Node *cmd;
    };
    union {
        Node *right;
        Node *args;
        Node *next;
        Node *declared;
        Node *to_type;
        Node *index;
        Node *to_copy;
    };

    expand(Node *, children);
    expand(Ucg *, drops);
};

struct Keyword {
    char *value;
    Token token;
};
