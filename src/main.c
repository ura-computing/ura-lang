/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        main.c                          */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 01:00:44    */
/*                                                             */
/*   updated: 2026/11/27 21:57:10                              */
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
    ucgFile *file;
    int line;
    int space;

    expand(Token *, tokens);

    Node *tree;

    expand(Node *, scopes);
    Node *scope;

    int exe_pos;

    expand(char *, paths);
    bool print_tokens;
    bool print_tree;
    bool print_insts;
    bool print_ll;
} ura;

Keyword keywords[] = {
    // clang-format off
    { "void",     { .type = VOID,  .is_type = true } },

    // Signed integers
    { "bool",     { .type = INT,   .is_type = true, .is_uint = true, .bits = 1 } },
    { "True",     { .type = INT,   .is_uint = true, .bits = 1, .num = "1" } },
    { "False",    { .type = INT,   .is_uint = true, .bits = 1, .num = "0" } },
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
    [OR_LIT] = 11,      [AND_LIT] = 12,

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
            int digit = (limit[j] - '0') * 2 + carry;
            limit[j] = '0' + digit % 10;
            carry = digit / 10;
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
    bool has_sign = buff[e + 1] == '-' || buff[e + 1] == '+';
    bool is_exponent =
        (buff[e] == 'e' || buff[e] == 'E') && isdigit(buff[e + 1 + has_sign]);
    if (is_exponent) {
        is_float = true;
        e += 1 + has_sign;
        while (isdigit(buff[e]))
            e++;
    }
    int end = e;
    bool is_uint = buff[e] == 'u' && !is_float;
    if (is_uint)
        e++;
    bool has_width = buff[e] == 'f';
    int bits = 0;
    if (has_width) {
        is_float = true;
        e++;
        while (isdigit(buff[e]))
            bits = bits * 10 + (buff[e++] - '0');
    }

    Token *new = new_token(is_float ? FLOAT : INT, s, e);
    new->num = strndup2(buff + s, end - s);
    new->is_uint = is_uint;
    if (is_float) {
        if (has_width && bits == 0)
            bits = 32;
        if (!has_width)
            bits = 64;
        new->bits = bits;
        bool ok = bits == 16 || bits == 32 || bits == 64 || bits == 80 || bits == 128;
        if (!ok)
            ucg_error(
                ura.file,
                s,
                e,
                format("invalid float width f%d", bits),
                "use f16, f32, f64, f80 or f128"
            );
        return e;
    }
    int widths[] = { 32, 64, 128, 256 };
    for (int i = 0; i < 4; i++) {
        if (fits_int(new->num, false, is_uint, widths[i])) {
            new->bits = widths[i];
            return e;
        }
    }
    ucg_error(
        ura.file,
        s,
        e,
        format("integer literal %s is too big", new->num),
        "the biggest integer is 256 bits"
    );
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

    char *text = parse_escaped(escaped, ura.file, s + 1, e - 1);
    if (quote == '"')
        new_token(CHARS, s, e)->string = text;
    if (quote == '\'') {
        unsigned char *bytes = (unsigned char *)text;
        int count = 1;
        int value = bytes[0];
        if (bytes[0] >= 0xF0) {
            count = 4;
            value = bytes[0] - 0xF0;
        } else if (bytes[0] >= 0xE0) {
            count = 3;
            value = bytes[0] - 0xE0;
        } else if (bytes[0] >= 0xC0) {
            count = 2;
            value = bytes[0] - 0xC0;
        }
        for (int i = 1; i < count; i++)
            value = value * 64 + (bytes[i] - 0x80);
        Token *new = new_token(INT, s, e);
        new->num = format("%d", value);
        new->bits = count > 1 ? 21 : 8;
    }
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
    int found = -1;
    int found_len = 0;
    for (int i = 0; specials[i].value; i++) {
        int len = (int)strlen(specials[i].value);
        bool is_longer = len > found_len;
        if (is_longer && strncmp2(specials[i].value, buff + s, len)) {
            found = i;
            found_len = len;
        }
    }
    if (found < 0) {
        ucg_error(ura.file, s, s + 1, format("unexpected '%c'", buff[s]), NULL);
        return s + 1;
    }
    Type type = specials[found].token.type;
    new_token(type, s, s + found_len);
    if (type == DOTS)
        ura.space++;
    return s + found_len;
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

ucgFile *open_file(char *path) {
    ucgFile *found = ucg_find_file(realpath2(path));
    if (found)
        return found;
    ucgFile *file = ucg_open_file(path);
    if (file == NULL)
        return NULL;
    ura.file = file;
    ura.line = 1;
    ura.space = 0;
    tokenize_range(0, file->len);
    return file;
}

void use_file(ucgFile *file, int space) {
    file->parsed = true;
    for (int i = 0; i < file->tokens_count; i++) {
        Token *token = file->tokens[i];
        Token *name = NULL;
        if (i + 1 < file->tokens_count)
            name = file->tokens[i + 1];
        bool is_use = token->type == USE && name && name->type == CHARS;
        if (!is_use) {
            token->space += space;
            append(ura.tokens, token);
            continue;
        }
        i++;
        char *path = format("%s/%s.ura", dirname2(file->path), name->string);
        ucgFile *used = open_file(path);
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

// AST
void add_error(Token *token, char *msg, char *help) {
    ucg_error(ucg_find_file(token->filename), token->s, token->e, msg, help);
}

Node *new_node(Token *token) {
    Node *new = arena_alloc(1, sizeof(Node));
    new->token = token;
    return new;
}

Token *peek(int pos) {
    int index = ura.exe_pos + pos;
    if (index >= ura.tokens_count)
        index = ura.tokens_count - 1;
    return ura.tokens[index];
}

Token *next(void) {
    Token *token = peek(0);
    if (token->type != LAST)
        ura.exe_pos++;
    return token;
}

Token *expect(Type type, char *msg, char *help) {
    Token *token = peek(0);
    if (token->type == type)
        return next();
    add_error(token, msg, help);
    return NULL;
}

void parse_block(Node *node) {
    int space = node->token->space;
    while (peek(0)->space > space)
        append(node->children, parse_expr(0));
}

Node *parse_type(Token *token) {
    Node *root = new_node(token);
    Node *base = root;
    Node *last = NULL;
    while (peek(0)->type == LBRACK && peek(0)->line == token->line) {
        Node *array = new_node(next());
        array->token->type = ARRAY;
        array->data_type = base;
        if (last)
            last->data_type = array;
        else
            root = array;
        last = array;
        if (peek(0)->type != RBRACK)
            append(array->children, parse_expr(0));
        expect(RBRACK, "expected ']'", "i32[2][3]");
    }
    return root;
}

Node *parse_list(Token *open, Type close) {
    Node *list = new_node(open);
    list->token->type = PARAMS;
    while (!includes(peek(0)->type, close, LAST, NULL)) {
        append(list->children, parse_expr(0));
        if (peek(0)->type != COMA)
            break;
        next();
    }
    expect(close, close == RBRACK ? "expected ']'" : "expected ')'", NULL);
    return list;
}

Node *parse_fn(Token *token) {
    Node *node = new_node(token);
    Token *name = expect(ID, "expected a function name", "fn main() i32:");
    if (name)
        token->name = name->name;
    Token *open = expect(LPARENT, "expected '(' after the function name", NULL);
    if (open)
        node->params = parse_list(open, RPARENT);
    else
        node->params = new_node(new_token(PARAMS, token->s, token->e));
    for (int i = 0; i < node->params->children_count; i++)
        append(node->children, node->params->children[i]);
    if (peek(0)->is_type)
        node->ret_type = parse_type(next());
    if (token->type == PROTO)
        return node;
    expect(DOTS, "expected ':' before the function body", NULL);
    parse_block(node);
    return node;
}

Node *parse_prime(void) {
    Token *token = next();
    switch (token->type) {
    case ID: {
        Node *node = new_node(token);
        bool is_call = peek(0)->type == LPARENT && peek(0)->line == token->line;
        if (is_call) {
            token->type = FN_CALL;
            node->params = parse_list(next(), RPARENT);
            return node;
        }
        Token *after = peek(0);
        bool is_type = after->is_type || after->type == ID;
        bool is_declare = is_type && after->line == token->line;
        if (is_declare) {
            token->type = DECLARE;
            node->data_type = parse_type(next());
        }
        return node;
    } // clang-format off
    case INT: case FLOAT: case CHARS: { // clang-format on
        if (token->is_type)
            return parse_type(token);
        return new_node(token);
    }
    case STRUCT: {
        Node *node = new_node(token);
        Token *name = expect(ID, "expected a struct name", "struct Point:");
        if (name)
            token->name = name->name;
        expect(DOTS, "expected ':' after the struct name", NULL);
        parse_block(node);
        return node;
    }
    case LPARENT: {
        Node *node = parse_expr(0);
        expect(RPARENT, "expected ')'", NULL);
        return node;
    }
    case LBRACK: {
        Node *node = parse_list(token, RBRACK);
        node->token->type = ARRAY_LIT;
        return node;
    }
    case SUB: {
        Node *node = new_node(token);
        token->type = NEG;
        node->operand = parse_prime();
        return node;
    } // clang-format off
    case PROTO: case FN_DEC: return parse_fn(token); // clang-format on
    case RETURN: {
        Node *node = new_node(token);
        bool has_value = peek(0)->line == token->line && peek(0)->type != LAST;
        if (has_value)
            node->to_ret = parse_expr(0);
        return node;
    } // clang-format off
    case IF: case ELIF: case ELSE: case LOOP: { // clang-format on
        Node *node = new_node(token);
        if (token->type != ELSE)
            node->cond = parse_expr(0);
        expect(DOTS, "expected ':'", NULL);
        bool is_inline = peek(0)->line == token->line;
        if (!is_inline)
            parse_block(node);
        else
            append(node->children, parse_expr(0));
        bool can_chain = includes(token->type, IF, ELIF, NULL);
        bool is_next = includes(peek(0)->type, ELIF, ELSE, NULL);
        bool same_space = peek(0)->space == token->space;
        bool same_line = peek(0)->line == token->line;
        if (can_chain && is_next && (same_space || same_line))
            node->next = parse_prime();
        return node;
    } // clang-format off
    case BRK: case CNT: return new_node(token); // clang-format on
    default: {
        add_error(token, format("unexpected %t", token->type), NULL);
        return new_node(token);
    }
    }
}

Node *parse_expr(int level) {
    Node *left = parse_prime();
    while (precedence[peek(0)->type] > level) {
        Node *node = new_node(next());
        node->left = left;
        if (node->token->type == LBRACK)
            node->token->type = ACCESS;
        node->right = parse_expr(precedence[node->token->type]);
        if (node->token->type == ACCESS)
            expect(RBRACK, "expected ']'", NULL);
        left = node;
    }
    return left;
}

// ANALYZE
Node *analyze_capture(Node *fn, Node *decl) {
    for (int i = 0; i < fn->locals_count; i++) {
        Node *local = fn->locals[i];
        if (local->from_parent == decl)
            return local;
    }
    Node *param = new_node(decl->token);
    param->data_type = decl->data_type;
    param->from_parent = decl;
    append(fn->params->children, param);
    append(fn->locals, param);
    return param;
}

Node *find_local(Token *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = scope->locals_count - 1; j >= 0; j--) {
            Node *decl = scope->locals[j];
            if (strcmp2(decl->token->name, name->name))
                return decl;
        }
    }
    return NULL;
}

void analyze_closure(Node *locals, Node *fn, Node *node) {
    if (node == NULL)
        return;
    Token *token = node->token;
    if (token->type == DECLARE)
        append(locals->children, node);
    bool is_local = false;
    for (int i = 0; token->type == ID && i < locals->children_count; i++)
        is_local = is_local || strcmp2(locals->children[i]->token->name, token->name);
    Node *decl = NULL;
    if (token->type == ID && !is_local)
        decl = find_node(DECLARE, token->name);
    if (decl && decl->token->space > 0)
        analyze_capture(fn, decl);
    switch (token->type) { // clang-format off
    case DOT:     analyze_closure(locals, fn, node->base);    return;
    case AS:      analyze_closure(locals, fn, node->operand); return;
    case DECLARE: return;
    case ARRAY: case FN_DEC: break;
    default: // clang-format on
        analyze_closure(locals, fn, node->left);
        analyze_closure(locals, fn, node->right);
    }
    for (int i = 0; i < node->children_count; i++)
        analyze_closure(locals, fn, node->children[i]);
}

void analyze_block(Node *node) {
    append(ura.scopes, node);
    ura.scope = node;
    analyze(node->left);
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == FN_DEC && find_scope(FN_DEC))
            analyze_closure(new_node(child->token), child, child);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == FN_DEC)
            analyze(child);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type != FN_DEC)
            analyze(child);
    }
    ura.scopes_count--;
    ura.scope = NULL;
    if (ura.scopes_count)
        ura.scope = ura.scopes[ura.scopes_count - 1];
    if (includes(node->token->type, IF, ELIF, NULL))
        analyze(node->next);
}

void analyze(Node *node) {
    if (node == NULL)
        return;
    Token *token = node->token;
    switch (token->type) {
    case ID: {
        node->declared = find_node(STRUCT, token->name);
        if (node->declared) {
            token->type = STRUCT_CALL;
            break;
        }
        node->declared = find_local(token);
        if (node->declared == NULL) {
            add_error(token, format("'%s' is not declared", token->name), NULL);
            break;
        }
        token->type = VAR;
        break;
    }
    case DECLARE: {
        if (find_node(STRUCT, token->name))
            add_error(token, format("'%s' is a struct name", token->name), NULL);
        analyze(node->data_type);
        append(ura.scope->locals, node);
        break;
    }
    case URA: {
        if (ucg_has_errors())
            return;
        analyze_block(node);
        break;
    } // clang-format off
    case STRUCT: case PROTO: case FN_DEC:
    case IF: case ELIF: case ELSE: case LOOP: { // clang-format on
        analyze_block(node);
        break;
    }
    case DOT: {
        analyze(node->base);
        break;
    }
    case FN_CALL: {
        Node *fn = find_node(FN_DEC, token->name);
        if (fn == NULL)
            fn = find_node(PROTO, token->name);
        if (fn == NULL) {
            add_error(token, format("'%s' is not declared", token->name), NULL);
            break;
        }
        node->declared = fn;
        Node *list = fn->params;
        int expected = 0;
        for (int i = 0; i < list->children_count; i++)
            expected += list->children[i]->from_parent == NULL;
        if (node->params->children_count != expected) {
            char *msg = format("'%s' takes %d arguments", token->name, expected);
            add_error(token, msg, NULL);
        }
        analyze(node->params);
        for (int i = 0; i < list->children_count; i++) {
            Node *param = list->children[i];
            if (param->from_parent == NULL)
                continue;
            Node *arg = new_node(param->token);
            arg->declared = find_local(param->token);
            append(node->children, arg);
        }
        break;
    }
    default: {
        analyze(node->left);
        analyze(node->right);
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        break;
    }
    }
}

// CODEGEN
Ucg *type_of(Node *node) {
    Token *token = node->token;
    if (includes(token->type, INT, FLOAT, NULL))
        return ucg_number_type(token->type, token->bits, token->is_uint);
    if (token->type == ARRAY && node->children_count) {
        add_error(token, "a type has no size", "a i32[] = i32[3]");
        return NULL;
    }
    if (token->type == ARRAY)
        return ucg_array_type(type_of(node->data_type), 0);
    if (token->type == STRUCT_CALL)
        return codegen_struct(node->declared);
    add_error(token, format("type %t is not supported yet", token->type), NULL);
    return NULL;
}

Node *find_node(Type type, char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            Node *found = child->token->type == ASSIGN ? child->var : child;
            bool is_type = found->token->type == type;
            if (is_type && strcmp2(found->token->name, name))
                return found;
        }
    }
    return NULL;
}

Ucg *find_variable(Node *node) {
    Token *token = node->token;
    if (token->type == DOT) {
        Ucg *base = find_variable(node->base);
        if (base == NULL)
            return NULL;
        Token *attr = node->attr->token;
        Ucg *var = ucg_get_attr(base, attr->name);
        if (var == NULL) {
            char *msg = format("no attribute '%s'", attr->name);
            add_error(attr, msg, NULL);
        }
        return var;
    }
    if (token->type == ACCESS) {
        Ucg *base = find_variable(node->base);
        Ucg *index = codegen(node->index, ucg_number_type(INT, 32, false));
        if (base == NULL)
            return NULL;
        return ucg_get_item(base, index);
    }
    Node *decl = node->declared;
    if (decl == NULL)
        return NULL;
    if (decl->from_parent)
        return ucg_point_to(decl->ucg);
    return decl->ucg;
}

bool is_literal(Node *node) {
    Node *lit = node->token->type == NEG ? node->operand : node;
    return includes(lit->token->type, INT, FLOAT, NULL) && !lit->token->is_type;
}

Ucg *codegen_number(Node *node, Node *lit, Ucg *type) {
    Token *token = lit->token;
    bool is_neg = node != lit;
    char *num = token->num;
    if (is_neg)
        num = format("-%s", token->num);
    bool is_same_kind = type && type->type == token->type;
    if (!is_same_kind)
        return ucg_numeric_value(type_of(lit), num);
    bool fits = fits_int(token->num, is_neg, type->is_uint, type->bits);
    if (token->type == FLOAT) {
        double value = strtod(token->num, NULL);
        double max = type->bits == 16 ? 65504.0 : type->bits == 32 ? FLT_MAX : DBL_MAX;
        fits = !isinf(value) && value <= max;
    }
    if (!fits) {
        char *msg = format("%s doesn't fit in %t%d", num, type->type, type->bits);
        add_error(node->token, msg, NULL);
    }
    return ucg_numeric_value(type, num);
}

void check_type(Node *node, Ucg *expected, Ucg *value) {
    if (!expected || !value || !value->value_type)
        return;
    Ucg *got = value->value_type;
    Ucg *expected_elem = expected;
    Ucg *got_elem = got;
    while (expected_elem->type == ARRAY && got_elem->type == ARRAY) {
        expected_elem = expected_elem->arr.elem;
        got_elem = got_elem->arr.elem;
    }
    if (expected_elem != got_elem)
        add_error(node->token, format("expected %U, got %U", expected, got), NULL);
}

Node *find_scope(Type type) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        if (ura.scopes[i]->token->type == type)
            return ura.scopes[i];
    }
    return NULL;
}

Ucg *codegen_block(Node *node, Ucg *type) {
    append(ura.scopes, node);
    ura.scope = node;
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(child->token->type, FN_DEC, PROTO, NULL))
            codegen_proto(child);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type != FN_DEC)
            continue;
        ucg_entry(child->ucg);
        codegen_block(child, NULL);
        ucg_end(child->ucg);
    }
    Ucg *value = NULL;
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == FN_DEC)
            continue;
        Ucg *expected = NULL;
        if (i == node->children_count - 1)
            expected = type;
        value = codegen(child, expected);
        if (includes(child->token->type, RETURN, BRK, CNT, NULL))
            break;
    }
    ura.scopes_count--;
    ura.scope = NULL;
    if (ura.scopes_count)
        ura.scope = ura.scopes[ura.scopes_count - 1];
    return value;
}

Ucg *codegen_if(Node *node, Ucg *type) {
    Ucg *result = NULL;
    if (type)
        result = ucg_var("result", type);
    Ucg *stmt = ucg_branch(IF, NULL);
    Node *last = node;
    for (Node *branch = node; branch; branch = branch->next) {
        bool is_first = branch == node;
        Ucg *part = is_first ? stmt : ucg_branch(branch->token->type, stmt);
        Ucg *cond = NULL;
        if (branch->cond)
            cond = codegen(branch->cond, ucg_number_type(INT, 1, true));
        ucg_then(part, cond);
        Ucg *value = codegen_block(branch, type);
        bool has_value = value && value->value_type;
        if (result && has_value)
            ucg_assign(result, value);
        last = branch;
    }
    ucg_end(stmt);
    if (result == NULL)
        return stmt;
    if (last->token->type != ELSE)
        add_error(
            node->token, "an if used as a value needs an else", "if a: 1 else: 2"
        );
    return ucg_load(result);
}

Ucg *codegen_proto(Node *node) {
    Ucg *ret = node->ret_type ? type_of(node->ret_type) : ucg_new(VOID);
    expand(Ucg *, params);
    zero_init(params);
    for (int i = 0; i < node->params->children_count; i++) {
        Node *param = node->params->children[i];
        Ucg *type = type_of(param->data_type);
        if (param->from_parent)
            type = ucg_ref_type(type);
        append(params, param->ucg = ucg_param(param->token->name, type));
    }
    Node *parent = find_scope(FN_DEC);
    char *name = node->token->name;
    bool is_local = parent && node->token->type == FN_DEC;
    if (is_local && ura.scope != parent)
        name = format("%s.%d.%s", parent->ucg->name, ura.scope->token->line, name);
    else if (is_local)
        name = format("%s.%s", parent->ucg->name, name);
    return node->ucg = ucg_proto(name, ret, params, params_count);
}

Ucg *codegen_struct(Node *node) {
    Token *token = node->token;
    if (node->ucg)
        return node->ucg;
    Ucg *type = node->ucg = ucg_struct_type(token->name);
    for (int i = 0; i < node->children_count; i++) {
        Node *attr = node->children[i];
        if (attr->token->type != DECLARE)
            continue;
        Ucg *attr_type = type_of(attr->data_type);
        append(type->obj.attrs, ucg_param(attr->token->name, attr_type));
    }
    return type;
}

Node *sizes_of(Node *sizes, Node *node) {
    Token *token = node->token;
    if (token->type == ACCESS) {
        Node *elem = sizes_of(sizes, node->base);
        append(sizes->children, node->index);
        return elem;
    }
    if (token->type != ARRAY)
        return node;
    if (node->children_count == 0) {
        add_error(token, "a stack array needs a size", "i32[3]");
        return NULL;
    }
    append(sizes->children, node->children[0]);
    return sizes_of(sizes, node->data_type);
}

Ucg *codegen_stack_array(Node *node) {
    Node *sizes = new_node(node->token);
    Node *elem = sizes_of(sizes, node);
    if (elem == NULL)
        return NULL;
    expand(Ucg *, values);
    zero_init(values);
    for (int i = 0; i < sizes->children_count; i++)
        append(values, codegen(sizes->children[i], ucg_number_type(INT, 32, false)));
    return ucg_stack_array(type_of(elem), values, values_count);
}

// TODO: to be checked
Ucg *codegen_array(Node *node, Ucg *type) {
    Ucg *elem = NULL;
    if (type && type->type == ARRAY)
        elem = type->arr.elem;
    if (node->children_count == 0 && elem == NULL) {
        add_error(
            node->token, "can't tell the type of an empty array", "a i32[] = []"
        );
        return NULL;
    }
    expand(Ucg *, items);
    zero_init(items);
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        Ucg *item = codegen(child, elem);
        if (elem == NULL && item)
            elem = item->value_type;
        append(items, item);
    }
    return ucg_array_from(elem, items, items_count);
}

Ucg *codegen(Node *node, Ucg *type) {
    Token *token = node->token;
    Ucg *value = NULL;
    switch (token->type) {
    case URA: {
        if (ucg_has_errors())
            break;
        ucg_start("ura", OUTPUT_EXE);
        value = codegen_block(node, NULL);
        break;
    } // clang-format off
    case VAR: case DOT: value = ucg_load(find_variable(node)); break;
    case DECLARE: { // clang-format on
        if (node->ucg == NULL)
            node->ucg = ucg_var(token->name, type_of(node->data_type));
        value = node->ucg;
        break;
    } // clang-format off
    case INT: case FLOAT: value = codegen_number(node, node, type); break;
    case CHARS:           value = ucg_string_value(token->string);  break;
    case STRUCT:          value = codegen_struct(node);             break;
    case ARRAY:           value = codegen_stack_array(node);        break;
    case ARRAY_LIT:       value = codegen_array(node, type);        break;
    case ACCESS: { // clang-format on
        Node *base = node->base;
        while (base->token->type == ACCESS)
            base = base->base;
        if (base->token->type == STRUCT_CALL)
            value = codegen_stack_array(node);
        else
            value = ucg_load(find_variable(node));
        break;
    }
    case ASSIGN: {
        Ucg *var = NULL;
        if (node->var->token->type == DECLARE)
            var = codegen(node->var, NULL);
        else
            var = find_variable(node->var);
        if (var == NULL)
            break;
        value = ucg_assign(var, codegen(node->value, var->data_type));
        break;
    } // clang-format off
    case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
    case DIV_ASSIGN: case MOD_ASSIGN: { // clang-format on
        Type ops[LAST + 1] = {
            [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
            [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
        };
        Ucg *var = find_variable(node->var);
        if (var == NULL)
            break;
        Ucg *right = codegen(node->value, var->data_type);
        Ucg *result = ucg_op(ops[token->type], ucg_load(var), right);
        value = ucg_assign(var, result);
        break;
    } // clang-format off
    case ADD: case SUB: case MUL: case DIV: case MOD:
    case GT: case LT: case GE: case LE: case EQ: case NQ:
    case AND: case OR: case AND_LIT: case OR_LIT: {
        // clang-format on
        // TODO: chekc this one
        Ucg *left = NULL;
        Ucg *right = NULL;
        if (is_literal(node->left)) {
            right = codegen(node->right, NULL);
            if (right)
                left = codegen(node->left, right->value_type);
        } else {
            left = codegen(node->left, NULL);
            Ucg *expected = NULL;
            if (left && is_literal(node->right))
                expected = left->value_type;
            right = codegen(node->right, expected);
        }
        if (left == NULL || right == NULL)
            break;
        left = ucg_widen(left, right->value_type);
        right = ucg_widen(right, left->value_type);
        check_type(node->right, left->value_type, right);
        value = ucg_op(token->type, left, right);
        break;
    }
    case NEG: {
        if (is_literal(node)) {
            value = codegen_number(node, node->operand, type);
            break;
        }
        Ucg *operand = codegen(node->operand, NULL);
        if (operand == NULL)
            break;
        Ucg *zero = ucg_numeric_value(operand->value_type, "0");
        value = ucg_op(SUB, zero, operand);
        break;
    }
    case PROTO: {
        value = node->ucg;
        break;
    }
    case FN_CALL: {
        Ucg *proto = node->declared->ucg;
        expand(Ucg *, params);
        zero_init(params);
        for (int i = 0; i < node->params->children_count; i++) {
            Ucg *param_type = proto->fn.params[i]->data_type;
            append(params, codegen(node->params->children[i], param_type));
        }
        for (int i = 0; i < node->children_count; i++)
            append(params, ucg_ref(find_variable(node->children[i])));
        value = ucg_call(proto, params, params_count);
        break;
    }
    case RETURN: {
        Node *fn = find_scope(FN_DEC);
        if (fn == NULL) {
            add_error(token, "'return' outside a function", NULL);
            break;
        }
        Ucg *result = NULL;
        if (node->to_ret)
            result = codegen(node->to_ret, fn->ucg->fn.ret);
        value = ucg_ret(result);
        break;
    }
    case AS: {
        Ucg *operand = codegen(node->operand, NULL);
        value = ucg_cast(operand, type_of(node->to_type));
        break;
    }
    case IF:
        value = codegen_if(node, type);
        break;
    case LOOP: {
        Ucg *loop = ucg_branch(LOOP, NULL);
        node->ucg = loop;
        ucg_then(loop, codegen(node->cond, ucg_number_type(INT, 1, true)));
        codegen_block(node, NULL);
        ucg_end(loop);
        value = loop;
        break;
    } // clang-format off
    case BRK: case CNT: { // clang-format on
        Node *loop = find_scope(LOOP);
        if (loop == NULL) {
            add_error(token, format("%t outside a loop", token->type), NULL);
            break;
        }
        value = ucg_jump(token->type, loop->ucg);
        break;
    }
    default: {
        add_error(token, format("%t is not supported yet", token->type), NULL);
        break;
    }
    }
    if (type == NULL || value == NULL)
        return value;
    value = ucg_widen(value, type);
    check_type(node, type, value);
    return value;
}

void read_args(int ac, char **av) {
    for (int i = 1; i < ac; i++) {
        char *arg = av[i];
        if (arg[0] != '-') {
            append(ura.paths, arg);
            continue;
        }
        bool is_help = strcmp2(arg, "-help") || strcmp2(arg, "-h");
        bool is_version = strcmp2(arg, "-version") || strcmp2(arg, "-v");
        if (is_help)
            print_help();
        if (is_version)
            help("ura %s (LLVM %s)\n", URA_VERSION, LLVM_VERSION_STRING);
        if (is_help || is_version)
            exit(0);
        if (strcmp2(arg, "-tokens"))
            ura.print_tokens = true;
        else if (strcmp2(arg, "-tree"))
            ura.print_tree = true;
        else if (strcmp2(arg, "-ucg"))
            ura.print_insts = true;
        else if (strcmp2(arg, "-ll"))
            ura.print_ll = true;
        else
            ucg_error(
                NULL,
                0,
                0,
                format("unknown flag %s", arg),
                "ura main.ura -tokens -tree -ucg -ll"
            );
    }
    if (ura.paths_count == 0)
        ucg_error(NULL, 0, 0, "expected a .ura file", "ura main.ura");
    ucg_check_errors();
}

void tokenize(void) {
    for (int i = 0; i < ura.paths_count; i++) {
        char *path = ura.paths[i];
        ucgFile *file = open_file(path);
        if (file == NULL)
            ucg_error(NULL, 0, 0, format("can't open %s", path), "check the path");
        else if (!file->parsed)
            use_file(file, 0);
    }
    ucg_check_errors();
    ura.space = -1;
    append(ura.tokens, new_token(LAST, ura.file->len, ura.file->len));
}

void print_header(char *name) {
    char *rule = "";
    for (int i = strlen(name); i < 50; i++)
        rule = format("%s─", rule);
    print(GREEN("──── %s %s") "\n", name, rule);
}

int main(int ac, char **av) {
    atexit(arena_free);
    arena_reserve(10000);
    read_args(ac, av);
    tokenize();
    if (ura.print_tokens)
        print_header("tokens");
    for (int i = 0; ura.print_tokens && i < ura.tokens_count; i++) {
        Token *token = ura.tokens[i];
        bool is_new_line = i == 0 || token->line != ura.tokens[i - 1]->line;
        if (is_new_line)
            print("%4d │ ", token->line);
        else
            print("     │ ");
        print("%k\n", token);
    }
    ura.tree = new_node(new_token(URA, 0, 0));
    parse_block(ura.tree);
    // if (ura.print_tree)
    //     print("[AST]:\n%n", ura.tree);
    analyze(ura.tree);
    if (ura.print_tree) {
        print_header("tree");
        print("%n", ura.tree);
    }
    codegen(ura.tree, NULL);
    if (ura.print_insts) {
        print_header("ucg");
        ucg_print_insts();
    }
    ucg_compile();
    if (ura.print_ll) {
        print_header("llvm");
        ucg_print_ll();
    }
    ucg_bin(ura.paths[0]);
    ucg_check_errors();
    ucg_finish();

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

char *compile_flags[][2] = {
    { "-tokens", "print the tokens" },
    { "-tree", "print the tree after parse and after analyze" },
    { "-ucg", "print the ucg instructions" },
    { "-ll", "print the LLVM IR" },
    { NULL, NULL },
};

void print_help(void) {
    help("compile files into an executable named after the first one:\n");
    help("  ura <file>.ura [more.ura] [flags]\n");
    help("\n");
    help("flags:\n");
    for (int i = 0; compile_flags[i][0]; i++) {
        help("  %-12s%s\n", compile_flags[i][0], compile_flags[i][1]);
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