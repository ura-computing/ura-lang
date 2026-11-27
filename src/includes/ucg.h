/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        structs.h                       */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 01:31:05    */
/*                                                             */
/*   updated: 2026/11/27 05:15:13                              */
/*                                                             */
/* *********************************************************** */

#pragma once

typedef enum Type Type;
typedef struct Ucg Ucg;
typedef struct UcgError UcgError;
typedef struct Arena Arena;
typedef struct ucgFile ucgFile;
typedef struct Token Token;
typedef enum UcgOutputMode UcgOutputMode;

enum UcgOutputMode {
    OUTPUT_EXE,
    OUTPUT_SHARED,
    OUTPUT_STATIC
};

enum Type {
    // clang-format off

    // --- special ---
    NONE, ERR, SCOPE, URA,

    // --- variables (ID becomes VAR after getting declared) ---
    ID, VAR, PARAM, GLOBAL,
    DECLARE, INIT_VAR, DREF, ARG,
    VAL_CREATE,

    // --- types ---
    TEMPLATE, TYPES, TEMPLATE_TYPE,
    ANY, VOID,
    INT, FLOAT,
    CHARS,
    REF, POINT_TO, ZERO,

    // --- structs and enums ---
    STRUCT, DOT, ATTR, ENUM,

    // --- punctuation ---
    LPARENT, RPARENT, DOTS, DOUBLE_DOTS, COMA,
    OPERATOR, PUB,

    // --- brackets and arrays ---
    LBRACK, RBRACK,
    LCBRACK, RCBRACK,
    ARRAY, ARRAY_LIT, ACCESS,

    // --- assignment ---
    ASSIGN,
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN, LSHIFT_ASSIGN, RSHIFT_ASSIGN,

    // --- arithmetic and bitwise ---
    ADD, SUB, MUL, DIV, MOD,
    BAND, BOR, BXOR, LSHIFT, RSHIFT,

    // --- comparison ---
    GT, LT, GE, LE, EQ, NQ,

    // --- logic ---
    AND, OR, NOT, NEG, BNOT,

    // --- functions ---
    PROTO,
    FN_DEC, ARGS, PARAMS, VARIADIC, FN_CALL, RETURN, FORMAT,
    FN_ENTRY,

    // --- directives (link, use, #macros) ---
    LINK, USE,
    MACRO_IF, MACRO_ELIF, MACRO_ELSE, MACRO_ERROR,
    MACRO_NAME, MACRO_VERSION, MACRO_ENTRY, MACRO_SCRIPT, MACRO_DEPENDENCY,
    MACRO_URL, MACRO_PATH, MACRO_CMD,

    // --- built-ins ---
    SIZEOF, TYPEOF, DROP, COPY, IS_ZERO, ALLOC, STACK_ALLOC, MEMCPY, FREE,

    // --- keywords and program info ---
    AS, FROM, AND_LIT, OR_LIT, SELF, FOR, IN, MODULE,
    ARGC, ARGV,
    LINE_NUMBER, FILE_NAME, FN_NAME,

    // --- control flow ---
    BLOC,
    IF, ELIF, ELSE, THEN,
    LOOP, BRK, CNT,
    MATCH, CASE, DEFAULT, CLEAN,

    // --- error handling ---
    TRY, CATCH, THROW, CHECK,

    // --- markers ---
    END,
    LAST,

    // clang-format on
};

struct Ucg {
    unsigned long inst_index;

    char *name;

    Type type;
    int bits;
    bool is_set;
    bool is_uint;
    bool is_throwing;
    int used;

    // llvm stuff
    struct {
        TypeRef lltype;
        Value elem;
        Bloc bloc;
        Bloc then;
        Bloc caller_bloc;
    };
	
	/*
	holds: 
		+ string literal
		+ i32, i64, i128... 
	*/ 
	char *value;
    Ucg *value_type;

    struct {
        bool is_varg;
        Ucg *ret;
        expand(Ucg *, params);
    } fn;

    struct {
        Ucg *elem;
        expand(Ucg *, items);
    } arr;

    struct {
        expand(Ucg *, attrs);
        int index;
    } obj;

    union {
        Ucg *left;

        Ucg *data_type;

        Ucg *to_enter; // entry
        Ucg *to_ret;
        Ucg *to_dref;
        Ucg *to_ref;
        Ucg *to_end; // used in ENDIF
        Ucg *to_call;
        Ucg *stmt;
        Ucg *to_point;
        Ucg *to_size;
    };
    struct {
		Ucg *cond;
        Ucg *next;
        Ucg *end;
        Ucg *base;
    };
    union {
        Ucg *right;
        Ucg *to_type;
        Ucg *attr;
        Ucg *index;
    };
    expand(Ucg *, cleans);
};

struct ucgFile {
    char *path;
    char *buff;
    int len;
    expand(Token *, tokens);
    bool parsed;
};

struct UcgError {
    ucgFile *file;
    int s;
    int e;
    char *msg;
    char *help;
};

