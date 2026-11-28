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
    bool is_uint;

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
    Ucg *ucg;
    Node *type;
    Node *declared;
    expand(Node *, locals);

	union {
        Node *left;
        Node *var;
        Node *base;
        Node *data_type;
        Node *ret_type;
        Node *to_ret;
        Node *cond;
        Node *operand;
    };
    union {
        Node *right;
        Node *params;
        Node *attr;
        Node *value;
        Node *next;
        Node *from_parent;
        Node *to_type;
        Node *index;
    };

    expand(Node *, children);
};

struct Keyword {
    char *value;
    Token token;
};
