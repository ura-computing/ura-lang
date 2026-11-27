/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        main.c                          */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 01:00:44    */
/*                                                             */
/*   updated: 2026/11/27 07:02:56                              */
/*                                                             */
/* *********************************************************** */

#include "./includes/libc.h"
#include "./includes/llvm.h"
#include "./includes/macros.h"
#include "./includes/ucg.h"
#include "./includes/ura.h"
#include "./includes/protos.h"
#include "./includes/arena.h"
#include "./includes/string.h"
#include "./includes/fmt.h"
#include "./includes/fs.h"
#include "./includes/ps.h"

#include "./ucg.c"

struct {
    expand(ucgFile *, files);
    expand(ucgFile *, uses);
    expand(ucgFile *, configs);
    Node *config;
    ucgFile *file;
    int line;
    int space;

    expand(Token *, tokens);

    Node *tree;

    expand(Node *, scopes);
    Node *scope;
    Node *types;
    expand(char *, modules);

    int exe_pos;
    bool throws;
} ura;

Keyword keywords[] = {
    // clang-format off
    { "void",     { .type = VOID,  .is_type = true } },

    // Signed integers
    { "bool",     { .type = INT,   .is_type = true, .bits = 1 } },
    { "i1",       { .type = INT,   .is_type = true, .bits = 1 } },
    { "char",     { .type = INT,   .is_type = true, .bits = 8 } },
    { "i8",       { .type = INT,   .is_type = true, .bits = 8 } },
    { "i16",      { .type = INT,   .is_type = true, .bits = 16 } },
    { "i32",      { .type = INT,   .is_type = true, .bits = 32 } },
    { "i64",      { .type = INT,   .is_type = true, .bits = 64 } },
    { "i128",     { .type = INT,   .is_type = true, .bits = 128 } },
    { "i256",     { .type = INT,   .is_type = true, .bits = 256 } },

    // Unsigned integers
    { "u8",       { .type = INT,   .is_type = true, .bits = 8,   .is_uint = true } },
    { "u16",      { .type = INT,   .is_type = true, .bits = 16,  .is_uint = true } },
    { "u32",      { .type = INT,   .is_type = true, .bits = 32,  .is_uint = true } },
    { "u64",      { .type = INT,   .is_type = true, .bits = 64,  .is_uint = true } },
    { "u128",     { .type = INT,   .is_type = true, .bits = 128, .is_uint = true } },
    { "u256",     { .type = INT,   .is_type = true, .bits = 256, .is_uint = true } },

	// Unicode
    { "unic",     { .type = INT,   .is_type = true, .bits = 21 } },

    // Floating-point
    { "f16",      { .type = FLOAT, .is_type = true, .bits = 16 } },
    { "f32",      { .type = FLOAT, .is_type = true, .bits = 32 } },
    { "f64",      { .type = FLOAT, .is_type = true, .bits = 64 } },
    { "f80",      { .type = FLOAT, .is_type = true, .bits = 80 } },
    { "f128",     { .type = FLOAT, .is_type = true, .bits = 128 } },

    // Boolean literals
    { "True",     { .type = INT,   .bits = 1, .num = "1" } },
    { "False",    { .type = INT,   .bits = 1, .num = "0" } },

    // Declarations and definitions
    { "struct",   { .type = STRUCT } }, { "enum",     { .type = ENUM } },
    { "fn",       { .type = FN_DEC } },
    { "proto",    { .type = PROTO } },
    { "operator", { .type = OPERATOR } },
    { "template", { .type = TEMPLATE } },
    { "pub",      { .type = PUB } },
    { "use",      { .type = USE } },
    { "link",     { .type = LINK } },
    { "mod",      { .type = MODULE } },

    // Operators and conditions
    { "and",      { .type = AND_LIT } }, { "or",       { .type = OR_LIT } },
    { "not",      { .type = NOT } },     { "is",       { .type = EQ } },
    { "as",       { .type = AS } },

    // Control flow
    { "if",       { .type = IF } },      { "elif",     { .type = ELIF } },
    { "else",     { .type = ELSE } },
    { "while",    { .type = LOOP } },
    { "for",      { .type = FOR } },     { "in",       { .type = IN } },
    { "break",    { .type = BRK } },     { "continue", { .type = CNT } },
    { "match",    { .type = MATCH } },
    { "case",     { .type = CASE } },    { "default",  { .type = DEFAULT } },
    { "return",   { .type = RETURN } },
    { "try",      { .type = TRY } },     { "catch",    { .type = CATCH } },
    { "throw",    { .type = THROW } },

    // Other keywords
    { "null",     { .type = ZERO } },
    { "from",     { .type = FROM } },
    { "self",     { .type = SELF, .name = "self" } },

    // Macros
    { "@if",      { .type = MACRO_IF } },      { "@elif",    { .type = MACRO_ELIF } },
    { "@else",    { .type = MACRO_ELSE } },
    { "@error",   { .type = MACRO_ERROR } },
    { "@name",    { .type = MACRO_NAME } },
    { "@version", { .type = MACRO_VERSION } },
    { "@entry",   { .type = MACRO_ENTRY } },
    { "@script",  { .type = MACRO_SCRIPT } },
    { "@dep",     { .type = MACRO_DEPENDENCY } },
    { "@url",     { .type = MACRO_URL } },
    { "@cmd",     { .type = MACRO_CMD } },
    { "@path",    { .type = MACRO_PATH } },

    { NULL },
    // clang-format on
};

Keyword specials[] = {
    // clang-format off
    // Delimiters
    { "(",   { .type = LPARENT } },       { ")",   { .type = RPARENT } },
    { "[",   { .type = LBRACK } },        { "]",   { .type = RBRACK } },
    { "{",   { .type = LCBRACK } },       { "}",   { .type = RCBRACK } },
    { "::",  { .type = DOUBLE_DOTS } },   { ":",   { .type = DOTS } },
    { "...", { .type = VARIADIC } },      { ".",   { .type = DOT } },
    { ",",   { .type = COMA } },

    // Assignment
    { "=",   { .type = ASSIGN } },
    { "+=",  { .type = ADD_ASSIGN } },    { "-=",  { .type = SUB_ASSIGN } },
    { "*=",  { .type = MUL_ASSIGN } },    { "/=",  { .type = DIV_ASSIGN } },
    { "%=",  { .type = MOD_ASSIGN } },
    { "<<=", { .type = LSHIFT_ASSIGN } }, { ">>=", { .type = RSHIFT_ASSIGN } },
    { "&=",  { .type = BAND_ASSIGN } },   { "|=",  { .type = BOR_ASSIGN } },
    { "^=",  { .type = BXOR_ASSIGN } },

    // Arithmetic
    { "+",   { .type = ADD } },           { "-",   { .type = SUB } },
    { "*",   { .type = MUL } },           { "/",   { .type = DIV } },
    { "%",   { .type = MOD } },

    // Comparison
    { "==",  { .type = EQ } },            { "!=",  { .type = NQ } },
    { ">=",  { .type = GE } },            { "<=",  { .type = LE } },
    { ">",   { .type = GT } },            { "<",   { .type = LT } },

    // Bitwise and logical
    { "&&",  { .type = AND } },           { "||",  { .type = OR } },
    { "&",   { .type = REF } },           { "|",   { .type = BOR } },
    { "^",   { .type = BXOR } },
    { "<<",  { .type = LSHIFT } },        { ">>",  { .type = RSHIFT } },
    { "!",   { .type = NOT } },           { "~",   { .type = BNOT } },

    // Other
    { "$",   { .type = FORMAT } },

    { NULL },
    // clang-format on
};

char *binary_ops[LAST + 1] = {
    // clang-format off
    // Arithmetic
    [ADD] = "+",          [SUB] = "-",          [MUL] = "*",          [DIV] = "/",
    [MOD] = "%",          [NEG] = "-",

    // Assignment
    [ASSIGN] = "=",
    [ADD_ASSIGN] = "+=",  [SUB_ASSIGN] = "-=",  [MUL_ASSIGN] = "*=",
    [DIV_ASSIGN] = "/=",  [MOD_ASSIGN] = "%=",

    // Bitwise
    [BAND] = "&",         [BOR] = "|",          [BXOR] = "^",         [BNOT] = "~",
    [LSHIFT] = "<<",      [RSHIFT] = ">>",

    // Bitwise assignment
    [BAND_ASSIGN] = "&=", [BOR_ASSIGN] = "|=",  [BXOR_ASSIGN] = "^=",
    [LSHIFT_ASSIGN] = "<<=", [RSHIFT_ASSIGN] = ">>=",

    // Comparison
    [EQ] = "==",          [NQ] = "!=",
    [LT] = "<",           [GT] = ">",           [LE] = "<=",           [GE] = ">=",

    // Logical
    [AND] = "&&",         [OR] = "||",
    [AND_LIT] = "and",    [OR_LIT] = "or",

    // Other
    [AS] = "as",          [IN] = "in",           [DROP] = "drop",
    [LBRACK] = "[]",      [FORMAT] = "$",
    // clang-format on
};

int precedence[LAST + 1] = {
    // clang-format off
    // Assignment
    [ASSIGN] = 1,
    [ADD_ASSIGN] = 1,  [SUB_ASSIGN] = 1,  [MUL_ASSIGN] = 1,
    [DIV_ASSIGN] = 1,   [MOD_ASSIGN] = 1,
    [BAND_ASSIGN] = 1, [BOR_ASSIGN] = 1,  [BXOR_ASSIGN] = 1,
    [LSHIFT_ASSIGN] = 1, [RSHIFT_ASSIGN] = 1,

    // Logical
    [OR] = 11,          [AND] = 12,

    // Comparison
    [EQ] = 21,           [NQ] = 21,
    [GT] = 22,           [LT] = 22,       [GE] = 22,       [LE] = 22,

    // Bitwise
    [BOR] = 23,          [BXOR] = 24,     [BAND] = 25,
    [LSHIFT] = 28,       [RSHIFT] = 28,

    // Arithmetic
    [ADD] = 30,          [SUB] = 30,
    [MUL] = 31,          [DIV] = 31,      [MOD] = 31,

    // Cast
    [AS] = 35,

    // Access
    [DOT] = 51,          [DOUBLE_DOTS] = 51, [LBRACK] = 51,
    // clang-format on
};

char *escaped[256] = {
    // clang-format off
    // Escape sequences
    ['n'] = "\n",   ['t'] = "\t",   ['r'] = "\r",   ['b'] = "\b",
    ['f'] = "\f",   ['v'] = "\v",   ['a'] = "\a",   ['?'] = "\?",
    ['0'] = "\0",   ['"'] = "\"",   ['\''] = "\'",  ['\\'] = "\\",
    ['e'] = "\033",

    // Braces
    ['{'] = "{",    ['}'] = "}",

    // Escape display strings
    ['\n'] = "\\n", ['\t'] = "\\t", ['\r'] = "\\r", ['\0'] = "\\0",
    // clang-format on
};

// TOKENIZE
Token *new_token(Type type, int s, int e) {
    Token *new = arena_alloc(1, sizeof(Token));
    new->type = type;
    new->filename = ura.file->path;
    new->line = ura.line;
    new->space = ura.space;
    new->s = s;
    new->e = e;

#if DEBUG
    static int debug_index;
    new->tk_index = ++debug_index;
    // if(new->tk_index == 125) {
    // 	raise(SIGSEGV);
    // }
#endif
    append(ura.file->tokens, new);
    return new;
}

int align_space(int space) {
    return space / TAB + ((space % TAB) != 0);
}

bool fits_int(char *num, bool negative, bool is_uint, int bits) {
    assert(bits > 0 && bits <= UCG_MAX_INT_BITS);

    int power = is_uint ? bits : bits - 1;
    char limit[80] = "1";
    for (int i = 0; i < power; i++) {
        int carry = 0;
        for (int j = (int)strlen(limit) - 1; j >= 0; j--) {
            int n = (limit[j] - '0') * 2 + carry;
            limit[j] = '0' + n % 10;
            carry = n / 10;
        }
        if (carry) {
            memmove(limit + 1, limit, strlen(limit) + 1);
            limit[0] = '0' + carry;
        }
    }

    while (*num == '0')
        num++;

    size_t num_len = strlen(num);
    size_t limit_len = strlen(limit);
    int cmp;
    if (num_len != limit_len)
        cmp = (num_len < limit_len) ? -1 : 1;
    else
        cmp = strcmp(num, limit);

    if (cmp < 0)
        return true;
    if (cmp > 0)
        return false;

    return negative && !is_uint;
}

Token *parse_int(int s, int e) {
    Token *new = new_token(INT, s, e);
    char *buff = ura.file->buff;
    int len = e - s;

    bool is_uint = false;
    if (len > 0 && buff[e - 1] == 'u') {
        is_uint = true;
        len--;
    }

    new->is_uint = is_uint;
    new->num = strndup2(buff + s, len);

    int widths[] = { 32, 64, 128, 256 };
    for (int i = 0; i < 4; i++) {
        if (fits_int(new->num, false, is_uint, widths[i])) {
            new->bits = widths[i];
            return new;
        }
    }

    ucg_error(
        ura.file,
        s,
        e,
        format("integer literal %s is too big", new->num),
        "the biggest integer is 256 bits"
    );
    return new;
}

Token *parse_float(int s, int e) {
    Token *new = new_token(FLOAT, s, e);
    char *buff = ura.file->buff;
    int len = e - s;

    new->bits = 64;
    for (int i = s; i < e; i++) {
        if (buff[i] != 'f')
            continue;
        int bits = 0;
        for (int j = i + 1; j < e; j++)
            bits = bits * 10 + (buff[j] - '0');
        if (i + 1 == e)
            bits = 32;
        bool ok = bits == 16 || bits == 32 || bits == 64 || bits == 80 || bits == 128;
        if (ok)
            new->bits = bits;
        else
            ucg_error(
                ura.file,
                s,
                e,
                format("invalid float width f%d", bits),
                "use f16, f32, f64, f80 or f128"
            );
        len = i - s;
        break;
    }

    new->num = strndup2(buff + s, len);
    return new;
}

int parse_number(int s) {
    char *buff = ura.file->buff;
    int e = s;
    while (isdigit(buff[e]))
        e++;
    bool is_float = buff[e] == '.' && isdigit(buff[e + 1]);
    if (is_float) {
        e++;
        while (isdigit(buff[e]))
            e++;
    }
    if (buff[e] == 'f') {
        is_float = true;
        e++;
        while (isdigit(buff[e]))
            e++;
    } else if (buff[e] == 'u' && !is_float) {
        e++;
    }
    if (is_float)
        parse_float(s, e);
    else
        parse_int(s, e);
    return e;
}

int parse_id(int s) {
    char *buff = ura.file->buff;
    int e = s + 1;
    while (isalnum(buff[e]) || buff[e] == '_')
        e++;

    Token *new = new_token(ID, s, e);

    for (int i = 0; keywords[i].value; i++) {
        char *value = keywords[i].value;
        size_t len = strlen(value);
        if (e - s == len && strncmp2(buff + s, value, e - s)) {
            *new = keywords[i].token;
            break;
        }
    }
    if (new->type == ID)
        new->name = strndup2(buff + s, e - s);

    new->filename = ura.file->path;
    new->line = ura.line;
    new->space = ura.space;
    new->s = s;
    new->e = e;
    return e;
}

Token *parse_char(int s, int e) {
    Token *new = new_token(INT, s, e);
    new->bits = 8;
    unsigned char *str =
        (unsigned char *)parse_escaped(escaped, ura.file, s + 1, e - 1);
    int count = 1;
    int value = str[0];
    if (str[0] >= 0xF0) {
        count = 4;
        value = str[0] - 0xF0;
    } else if (str[0] >= 0xE0) {
        count = 3;
        value = str[0] - 0xE0;
    } else if (str[0] >= 0xC0) {
        count = 2;
        value = str[0] - 0xC0;
    }
    for (int i = 1; i < count; i++)
        value = value * 64 + (str[i] - 0x80);

    new->num = format("%d", value);
    if (count > 1)
        new->bits = 21;
    return new;
}

int parse_quoted(int s) {
    char *buff = ura.file->buff;
    char quote = buff[s];
    int e = s;
    bool joins = false;
    do {
        e++;
        while (buff[e] && buff[e] != quote && buff[e] != '\n')
            e += buff[e] == '\\' && buff[e + 1] ? 2 : 1;
        if (buff[e] != quote) {
            ucg_error(
                ura.file,
                s,
                e,
                format("unterminated %c", quote),
                format("close it with %c on the same line", quote)
            );
            return e;
        }
        e++;
        int next = e;
        while (isspace(buff[next]))
            next++;
        bool has_newline = memchr(buff + e, '\n', next - e) != NULL;
        joins = quote == '"' && buff[next] == '"' && has_newline;
        if (joins)
            e = next;
    } while (joins);

    if (quote == '\'')
        parse_char(s, e);
    else
        new_token(CHARS, s, e)->string =
            parse_escaped(escaped, ura.file, s + 1, e - 1);
    for (int i = s; i < e; i++)
        ura.line += buff[i] == '\n';
    return e;
}

int parse_comment(int s) {
    char *buff = ura.file->buff;
    char *close = strstr(buff + s + 2, "*/");
    if (close == NULL) {
        ucg_error(ura.file, s, s + 2, "unterminated comment", "close it with */");
        return ura.file->len;
    }
    int e = close - buff + 2;
    for (int i = s; i < e; i++)
        ura.line += buff[i] == '\n';
    return e;
}

int parse_indent(int s) {
    char *buff = ura.file->buff;
    int e = s + 1;
    while (buff[e] == ' ')
        e++;
    ura.line++;
    ura.space = align_space(e - s - 1);
    return e;
}

int parse_special(int s) {
    char *buff = ura.file->buff;
    for (int i = 0; specials[i].value; i++) {
        char *value = specials[i].value;
        Type type = specials[i].token.type;
        int len = (int)strlen(value);
        if (!strncmp2(value, buff + s, len))
            continue;
        new_token(type, s, s + len);
        if (type == DOTS)
            ura.space++;
        return s + len;
    }
    ucg_error(ura.file, s, s + 1, format("unexpected '%c'", buff[s]), NULL);
    return s + 1;
}

void tokenize_range(int start, int end) {
    char *buff = ura.file->buff;
    int s = start;
    while (s < end && buff[s]) {
        char c = buff[s];
        bool is_blank = c == ' ' || c == '\r';
        bool is_comment = strncmp2(buff + s, "//", 2);
        bool is_multi_comment = strncmp2(buff + s, "/*", 2);
        bool is_quote = c == '"' || c == '\'';
        bool is_at = c == '@' && isalpha(buff[s + 1]);
        bool is_name = isalpha(c) || c == '_' || is_at;
        int e = s + 1;
        if (c == '\n') {
            e = parse_indent(s);
        } else if (is_blank) {
        } else if (is_comment) {
            while (buff[e] && buff[e] != '\n')
                e++;
        } else if (is_multi_comment)
            e = parse_comment(s);
        else if (is_quote)
            e = parse_quoted(s);
        else if (isdigit(c))
            e = parse_number(s);
        else if (is_name)
            e = parse_id(s);
        else
            e = parse_special(s);

        s = e;
    }
}

void tokenize(ucgFile *file) {
    ura.file = file;
    ura.line = 1;
    ura.space = 0;
    tokenize_range(0, file->len);
}

ucgFile *add_file(char *path) {
    char *real = realpath2(path);
    for (int i = 0; i < ura.files_count; i++) {
        if (strcmp2(ura.files[i]->path, real))
            return ura.files[i];
    }
    ucgFile *file = new_file(real);
    if (!open_file(file))
        return NULL;
    append(ura.files, file);
    tokenize(file);
    return file;
}

void use_file(ucgFile *file, int space) {
    file->parsed = true;
    for (int i = 0; i < file->tokens_count; i++) {
        Token *token = file->tokens[i];
        Token *name = i + 1 < file->tokens_count ? file->tokens[i + 1] : NULL;
        bool is_use = token->type == USE && name && name->type == CHARS;
        if (!is_use) {
            token->space += space;
            append(ura.tokens, token);
            continue;
        }
        i++;
        char *path = format("%s/%s.ura", dirname2(file->path), name->string);
        ucgFile *used = add_file(path);
        if (used == NULL) {
            ucg_error(
                file,
                name->s,
                name->e,
                format("can't find %s", name->string),
                "check the path"
            );
            continue;
        }
        if (!used->parsed)
            use_file(used, space + token->space);
    }
}


void codegen() {
}


int main(int ac, char **av) {
    atexit(arena_free);
    arena_reserve(10000);

    for (int i = 1; i < ac; i++) {
        ucgFile *file = add_file(av[i]);
        if (file == NULL)
            ucg_error(NULL, 0, 0, format("can't open %s", av[i]), "check the path");
        else if (!file->parsed)
            use_file(file, 0);
    }
    ucg_check_errors();
    for (int i = 0; i < ura.tokens_count; i++)
        print("%k\n", ura.tokens[i]);

    // Ucg *fn_main = ucg_fn("main", ucg_int_type(32), NULL, 0);
    // {
    //     Ucg *v = ucg_numeric_value(ucg_int_type(32), "123");
    // 	ucg_ret(v);
    //     // ucg_numeric_value(ucg_int_type(64), "123");
    //     // ucg_numeric_value(ucg_int_type(32), "123");
    //     // ucg_numeric_value(ucg_int_type(32), "123");
    // }
    // ucg_end(fn_main);

    // ucg_print();

    // ucg_asm_start("ucg", OUTPUT_EXE);
    // ucg_asm_compile();
    // printf(GREEN("===========================\n"));
    // ucg_asm_print();
    // printf(GREEN("===========================\n"));
    // ucg_asm_end();

    // ucg_write_ll();
    // ucg_bin();
    return 0;
}

void parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        print_help();
        exit(1);
    }
	if (strcmp2(av[1], "-help") || strcmp2(av[1], "-h")) {
        print_help();
        exit(0);
    }
	if (strcmp2(av[1], "-version") || strcmp2(av[1], "-v")) {
        help("ura %s (LLVM %s)\n", URA_VERSION, LLVM_VERSION_STRING);
        exit(0);
    }
	// if (strcmp2(av[1], "compile")) {
    // //     parse_compile_args(ac, av);
    // //     return;
    // // }
	// if (strcmp2(av[1], "init")) {
    //     if (ac < 3) {
    //         eprint("expected a project name:\n");
    //         help("ura init <name>\n");
    //         exit(1);
    //     }
    //     init_project(av[2]);
    //     exit(0);
    // }
	
}

char *compile_flags[][2] = {
    { "-o <exe>", "name the executable (default build/exe.out)" },
    { "-ll [path]", "also write the LLVM IR (default build/out.ll)" },
    { "-S [path]", "write the assembly, no executable (default build/out.s)" },
    { "-c [path]", "write the object file, no executable (default build/out.o)" },
    { NULL, NULL },
};

void print_help(void) {
    help("compile one file (it reaches the others with use):\n");
    help("  ura compile <file>.ura [flags]\n");
    help("\n");
    help("flags:\n");
    for (int i = 0; compile_flags[i][0]; i++) {
        char *flag = compile_flags[i][0];
        int pad = 12 - (int)strlen(flag);
        help("  %s%i%s\n", flag, pad, compile_flags[i][1]);
    }
    help("\n");
    help("projects (ura.conf):\n");
    help("  ura init <name>         create a project\n");
    help("  ura deps install        get the dependencies\n");
    help("  ura deps add <name>[@version] [repo url | ./folder]\n");
    help(
        "                          add a dependency (no url: "
        "ura-computing/ura-<name>)\n"
    );
    help("  ura deps update [name]  move dependencies to their latest version\n");
    help("  ura deps remove <name>[@version] ...\n");
    help(
        "                          remove dependencies (no version: every version)\n"
    );
    help("  ura deps list           show the dependencies\n");
    help("  ura <script>            run a @script (run, build, ...)\n");
    help("\n");
    help("  ura -version / -v       print the ura and LLVM versions\n");
    help("  ura -help / -h          show this help\n");
}