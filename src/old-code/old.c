/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        main.c                          */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/24 21:39:29    */
/*                                                             */
/*   updated: 2026/11/26 08:02:59                              */
/*                                                             */
/* *********************************************************** */

// TYPEDEFS
typedef struct Token Token;
typedef struct Node Node;
typedef struct NodePrint NodePrint;
typedef struct Keyword Keyword;

#define print(...)  (DEBUG ? fputs(format(__VA_ARGS__), stdout) : 0)
#define help(...)   fputs(format(__VA_ARGS__), stdout)
#define eprint(...) fputs(format(RED("error: ") __VA_ARGS__), stderr)
#define vfprint     _fprint

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

#define URA_VERSION "0.0.1"

struct Token {
    Type type;
    char *name;
    int space;
#if DEBUG
	int tk_index;
#endif

    char *filename;
    int line;
    int s;
    int e;

    bool is_type;
    bool is_varg;
    bool is_pub;
    bool is_uint;
    bool is_used;
    // uraIR ir;

    Ucg *ucg;
    int shift;
    union {
        struct {
            int index;
        } attr;
        struct {
            long long value;
            double real;
        } num;
        struct {
            char *value;
        } chars;
        struct {
            int start;
            int end;
            Node *types;
            Node *parents;
        } body;
    };
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
    expand(Ucg *, temporaries);
    expand(Node *, temporary_types);
};

struct NodePrint {
    expand(int, depths);
    expand(Node *, nodes);
};

struct Keyword {
    char *value;
    Token token;
};

// Global Ura
struct {
    expand(File *, files);
    expand(File *, uses);
    expand(File *, configs);
    Node *config;
    File *file;
    int line;

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
    
	{ "bool",     { .type = INT,   .is_type = true, .shift = 1 } },
    { "i1",       { .type = INT,   .is_type = true, .shift = 1 } },
    { "char",     { .type = INT,   .is_type = true, .shift = 8 } },
    { "i8",       { .type = INT,   .is_type = true, .shift = 8 } },
    { "i16",      { .type = INT,   .is_type = true, .shift = 16 } },
    { "i32",      { .type = INT,   .is_type = true, .shift = 32 } },
    { "i64",      { .type = INT,   .is_type = true, .shift = 64 } },
    
    { "u8",       { .type = INT,   .is_type = true, .shift = 8,  .is_uint = true } },
    { "u16",      { .type = INT,   .is_type = true, .shift = 16, .is_uint = true } },
    { "u32",      { .type = INT,   .is_type = true, .shift = 32, .is_uint = true } },
    { "u64",      { .type = INT,   .is_type = true, .shift = 64, .is_uint = true } },

    { "unic",     { .type = INT,   .is_type = true, .shift = 21 } },

	{ "f32",      { .type = FLOAT, .is_type = true, .shift = 32 } },
    { "f64",      { .type = FLOAT, .is_type = true, .shift = 64 } },

    { "True",     { .type = INT,   .shift = 1, .num = { .value = true } } },
    { "False",    { .type = INT,   .shift = 1, .num = { .value = false } } },

    { "struct",   { .type = STRUCT } },
    { "enum",     { .type = ENUM } },

    { "and",      { .type = AND_LIT } },
    { "or",       { .type = OR_LIT } },
    { "not",      { .type = NOT } },
    { "is",       { .type = EQ } },
    { "proto",    { .type = PROTO } },
    { "link",     { .type = LINK } },
    { "use",      { .type = USE } },
    
	// macros
	{ "@if",      { .type = MACRO_IF } },
    { "@elif",    { .type = MACRO_ELIF } },
    { "@else",    { .type = MACRO_ELSE } },
    { "@error",   { .type = MACRO_ERROR } },
    { "@name",    { .type = MACRO_NAME } },
    { "@version", { .type = MACRO_VERSION } },
    { "@entry",   { .type = MACRO_ENTRY } },
    { "@script",  { .type = MACRO_SCRIPT } },
    { "@dep", 	  { .type = MACRO_DEPENDENCY } },
    { "@url",     { .type = MACRO_URL } },
    { "@cmd",     { .type = MACRO_CMD } },
    { "@path",    { .type = MACRO_PATH } },
    { "operator", { .type = OPERATOR } },
    { "pub",      { .type = PUB } },
    { "fn",       { .type = FN_DEC } },
    { "return",   { .type = RETURN } },
    { "if",       { .type = IF } },
    { "elif",     { .type = ELIF } },
    { "else",     { .type = ELSE } },
    { "while",    { .type = LOOP } },
    { "for",      { .type = FOR } },
    { "in",       { .type = IN } },
    { "mod",      { .type = MODULE } },
    { "break",    { .type = BRK } },
    { "continue", { .type = CNT } },
    { "match",    { .type = MATCH } },
    { "case",     { .type = CASE } },
    { "default",  { .type = DEFAULT } },
    { "try",      { .type = TRY } },
    { "catch",    { .type = CATCH } },
    { "throw",    { .type = THROW } },
    { "template", { .type = TEMPLATE } },
    { "null",     { .type = ZERO } },
    { "as",       { .type = AS } },
    { "from",     { .type = FROM } },
    { "self",     { .type = SELF, .name = "self" } },
    { NULL },
};

Keyword builtins[] = {
    { "$",             { .type = FORMAT } },
    { "sizeof",        { .type = SIZEOF } },
    { "typeof",        { .type = TYPEOF } },
    { "drop",          { .type = DROP } },
    { "isZero",        { .type = IS_ZERO } },
    { "memory_alloc",  { .type = ALLOC } },
    { "memory_copy",   { .type = MEMCPY } },
    { "memory_free",   { .type = FREE } },
    { "ura::argc",     { .type = ARGC } },
    { "ura::argv",     { .type = ARGV } },
    { "ura::line",     { .type = LINE_NUMBER } },
    { "ura::filename", { .type = FILE_NAME } },
    { "ura::function", { .type = FN_NAME } },
    { NULL },
};

Keyword specials[] = {
    { "(",   { .type = LPARENT } },       { ")",   { .type = RPARENT } },
    { "::",  { .type = DOUBLE_DOTS } },   { ":",   { .type = DOTS } },
    { "[",   { .type = LBRACK } },        { "]",   { .type = RBRACK } },
    { "{",   { .type = LCBRACK } },       { "}",   { .type = RCBRACK } },

    { "+=",  { .type = ADD_ASSIGN } },    { "-=",  { .type = SUB_ASSIGN } },
    { "*=",  { .type = MUL_ASSIGN } },    { "/=",  { .type = DIV_ASSIGN } },
    { "%=",  { .type = MOD_ASSIGN } },
    { "<<=", { .type = LSHIFT_ASSIGN } }, { ">>=", { .type = RSHIFT_ASSIGN } },
    { "&=",  { .type = BAND_ASSIGN } },   { "|=",  { .type = BOR_ASSIGN } },
    { "^=",  { .type = BXOR_ASSIGN } },
    { "<<",  { .type = LSHIFT } },        { ">>",  { .type = RSHIFT } },

    { "...", { .type = VARIADIC } },      { ".",   { .type = DOT } },

    { "+",   { .type = ADD } },           { "-",   { .type = SUB } },
    { "*",   { .type = MUL } },           { "/",   { .type = DIV } },
    { "%",   { .type = MOD } },

    { ">=",  { .type = GE } },            { "<=",  { .type = LE } },
    { ">",   { .type = GT } },            { "<",   { .type = LT } },
    { "==",  { .type = EQ } },            { "!=",  { .type = NQ } },
    { "!",   { .type = NOT } },           { "~",   { .type = BNOT } },
    { "$",   { .type = FORMAT } },

    { "=",   { .type = ASSIGN } },        { ",",   { .type = COMA } },

    { "&&",  { .type = AND } },           { "||",  { .type = OR } },
    { "&",   { .type = REF } },           { "|",   { .type = BOR } },
    { "^",   { .type = BXOR } },

    { NULL },
}; // clang-format on

char *binary_ops[LAST + 1] = {
    // clang-format off
    [ADD] = "+",          [SUB] = "-",          [MUL] = "*",          [DIV] = "/",
    [MOD] = "%",          [AS] = "as",          [LBRACK] = "[]",      [FORMAT] = "$",
    [IN] = "in",          [DROP] = "drop",

    [ASSIGN] = "=",       [ADD_ASSIGN] = "+=",  [SUB_ASSIGN] = "-=",  [MUL_ASSIGN] = "*=",
    [DIV_ASSIGN] = "/=",  [MOD_ASSIGN] = "%=",

    [BAND] = "&",         [BOR] = "|",          [BXOR] = "^",
    [LSHIFT] = "<<",      [RSHIFT] = ">>",

    [BAND_ASSIGN] = "&=", [BOR_ASSIGN] = "|=", [BXOR_ASSIGN] = "^=",
    [LSHIFT_ASSIGN] = "<<=", [RSHIFT_ASSIGN] = ">>=",

    [EQ] = "==",          [NQ] = "!=",
    [LT] = "<",           [GT] = ">",           [LE] = "<=",           [GE] = ">=",

    [AND] = "&&",         [OR] = "||",
    [AND_LIT] = "and",    [OR_LIT] = "or",

    [NEG] = "-",          [BNOT] = "~",
}; // clang-format on

int precedence[LAST + 1] = {
    // clang-format off
    [ASSIGN] = 1,      [ADD_ASSIGN] = 1,    [SUB_ASSIGN] = 1,    [MUL_ASSIGN] = 1,
    [DIV_ASSIGN] = 1,  [MOD_ASSIGN] = 1,    [BAND_ASSIGN] = 1,   [BOR_ASSIGN] = 1,
    [BXOR_ASSIGN] = 1, [LSHIFT_ASSIGN] = 1, [RSHIFT_ASSIGN] = 1,

    [OR] = 11,         [AND] = 12,

    [EQ] = 21,         [NQ] = 21,           [GT] = 22,           [LT] = 22,
    [GE] = 22,         [LE] = 22,

    [BOR] = 23,        [BXOR] = 24,         [BAND] = 25,         [LSHIFT] = 28,
    [RSHIFT] = 28,

    [ADD] = 30,        [SUB] = 30,          [MUL] = 31,          [DIV] = 31,
    [MOD] = 31,

    [AS] = 35,

    [DOT] = 51,        [DOUBLE_DOTS] = 51,  [LBRACK] = 51,
}; // clang-format on

char *escaped[256] = {
    // clang-format off
    ['n'] = "\n",   ['t'] = "\t",   ['r'] = "\r",   ['b'] = "\b",
    ['f'] = "\f",   ['v'] = "\v",   ['a'] = "\a",   ['?'] = "\?",
    ['0'] = "\0",   ['"'] = "\"",   ['\''] = "\'",  ['\\'] = "\\",
    ['{'] = "{",    ['}'] = "}",    ['e'] = "\033",

    ['\n'] = "\\n", ['\t'] = "\\t", ['\r'] = "\\r", ['\0'] = "\\0",
}; // clang-format on

#include "./assert.c"
#include "./project.c"

// TOKENIZE
Token *new_token(Type type, Token from) {
    // TODO: maybe we need to copy also the shift
    Token *new = arena_alloc(1, sizeof(Token));
    new->type = type;
    new->filename = from.filename;
    new->line = from.line;
    new->space = from.space;
    new->s = from.s;
    new->e = from.e;
#if DEBUG
	static int debug_index;
	new->tk_index = ++debug_index;
	// if(new->tk_index == 125) {
	// 	raise(SIGSEGV);
	// }
#endif
    append(ura.tokens, new);
    return new;
}

File *file_at(int pos) {
    File *found = NULL;
    for (int i = 0; i < ura.files_count + ura.configs_count; i++) {
        bool is_source = i < ura.files_count;
        File *file = is_source ? ura.files[i] : ura.configs[i - ura.files_count];
        bool is_before = found && file->start < found->start;
        if (!file->opened || file->start > pos || is_before)
            continue;
        found = file;
    }
    return found;
}

char *text_at(int pos) {
    File *file = file_at(pos);
    return file->buff + pos - file->start;
}

int end_of_sources(void) {
    int end = 0;
    for (int i = 0; i < ura.files_count + ura.configs_count; i++) {
        bool is_source = i < ura.files_count;
        File *file = is_source ? ura.files[i] : ura.configs[i - ura.files_count];
        if (file->opened && file->start + file->len + 1 > end)
            end = file->start + file->len + 1;
    }
    return end;
}

int line_start_in(char *buff, int pos) {
    while (pos > 0 && buff[pos - 1] != '\n')
        pos--;
    while (buff[pos] == ' ')
        pos++;
    return pos;
}

int line_end_in(char *buff, int pos) {
    while (buff[pos] && buff[pos] != '\n')
        pos++;
    return pos;
}

int indent_in(char *buff, int pos) {
    while (pos > 0 && buff[pos - 1] != '\n')
        pos--;
    int indent = 0;
    while (buff[pos + indent] == ' ')
        indent++;
    return indent;
}

bool ends_string(char *buff, int s, int i) {
    bool is_block = buff[s] == '"' && buff[s + 1] == '\n';
    if (buff[i] != '\n' || is_block)
        return false;
    int next = i;
    while (isspace(buff[next]))
        next++;
    bool is_char = buff[s] == '\'';
    return is_char || indent_in(buff, next) <= indent_in(buff, s);
}

Token *find_builtin(char *name) {
    for (int i = 0; builtins[i].value; i++) {
        if (strcmp2(builtins[i].value, name))
            return &builtins[i].token;
    }
    return NULL;
}

Token *source_value(Token *token) {
    Node *fn = NULL;
    for (int i = 0; i < ura.scopes_count; i++) {
        if (ura.scopes[i]->token->type != FN_DEC)
            continue;
        fn = ura.scopes[i];
    }
    switch (token->type) { // clang-format off
    case LINE_NUMBER: token->num.value = line_at(token->s); break;
    case FILE_NAME:   token->chars.value = relative_path(file_at(token->s)->path); break;
    case FN_NAME:     token->chars.value = fn ? fn->token->name : ""; break;
    default:          break; // clang-format on
    }
    bool is_line = token->type == LINE_NUMBER;
    token->type = is_line ? INT : CHARS;
    token->shift = is_line ? 32 : 0;
    return token;
}

Token *parse_id(int s, int e, int space) {
    Token *new = new_token(ID, (Token){});
    char *buff = ura.file->buff;

    int i = 0;
    for (; keywords[i].value; i++) {
        char *value = keywords[i].value;
        size_t len = strlen(value);
        if (e - s == len && strncmp2(buff + s, value, e - s)) {
            // TODO: this part maybe is buggy
            *new = keywords[i].token;
            break;
        }
    }
    if (keywords[i].value == NULL)
        new->name = strndup2(buff + s, e - s);
    // if (new->type == RETURN) {
    //     print("return has %d space in line %d\n", space, ura.line);
    //     exit(1);
    // }
    new->filename = ura.file->path;
    new->line = ura.line;
    new->space = space / TAB + ((space % TAB) != 0);
    new->s = s;
    new->e = e;
    print("new token %t\n", new->type);
    return new;
}

Token *parse_num(int s, int e, int space) {
    Token *new = new_token(
        INT,
        (Token){
            .filename = ura.file->path,
            .line = ura.line,
            .space = space / TAB + ((space % TAB) != 0),
            .s = s,
            .e = e,
        }
    );

    char *buff = ura.file->buff;
    new->num.value = strtoll(buff + s, NULL, 10);
    new->shift = new->num.value > 2147483647 ? 64 : 32;
    if (memchr(buff + s, '.', e - s)) {
        new->type = FLOAT;
        new->num.real = strtod(buff + s, NULL);
        new->shift = buff[e - 1] == 'f' ? 32 : 64;
    }
    print("new token %t\n", new->type);
    return new;
}

Token *parse_string(int s, int e, int space) {
    Token *new = new_token(
        CHARS,
        (Token){
            .filename = ura.file->path,
            .line = ura.line,
            .space = space / TAB + ((space % TAB) != 0),
            .s = s,
            .e = e,
        }
    );
    char *buff = ura.file->buff;
    new->chars.value = parse_escaped(buff, s + 1, e - 1);

    print("new token %t\n", new->type);
    return new;
}

int closing_brace(int open) {
    char *buff = ura.file->buff;
    int depth = 0;
    int i = open;
    for (; buff[i] != '\0' && buff[i] != '\n'; i++) {
        if (buff[i] == '{')
            depth++;
        if (buff[i] == '}')
            depth--;
        if (depth == 0)
            break;
    }
    return i;
}

int parse_format_text(int s, int space) {
    char *buff = ura.file->buff;
    int e = s;
    while (buff[e] != '\0' && !strchr2("\"\n{", buff[e]))
        e += buff[e] == '\\' ? 2 : 1;
    parse_string(s - 1, e + 1, space);
    return e;
}

// $"hp: {hp}!" -> $("hp: ", hp, "!")
int parse_format(int s, int space) {
    char *buff = ura.file->buff;
    Token *token = NULL;

    token = new_token(
        ID,
        (Token){
            .filename = ura.file->path,
            .line = ura.line,
            .space = space / TAB + ((space % TAB) != 0),
            .s = s,
            .e = s + 1,
        }
    );
    token->name = "$";

    token = new_token(
        LPARENT,
        (Token){
            .filename = ura.file->path,
            .line = ura.line,
            .space = space / TAB + ((space % TAB) != 0),
            .s = s,
            .e = s + 1,
        }
    );
    int e = parse_format_text(s + 2, space);
    while (buff[e] == '{') {
        int close = closing_brace(e);
        new_token(
            COMA,
            (Token){
                .filename = ura.file->path,
                .line = ura.line,
                .space = space / TAB + ((space % TAB) != 0),
                .s = e,
                .e = e,
            }
        );
        tokenize_range(e + 1, close, space);
        new_token(
            COMA,
            (Token){
                .filename = ura.file->path,
                .line = ura.line,
                .space = space / TAB + ((space % TAB) != 0),
                .s = close,
                .e = close,
            }
        );
        e = parse_format_text(close + 1, space);
    }
    new_token(
        RPARENT,
        (Token){
            .filename = ura.file->path,
            .line = ura.line,
            .space = space / TAB + ((space % TAB) != 0),
            .s = e,
            .e = e + 1,
        }
    );
    if (buff[e] == '"')
        e++;
    assert_lexeme(buff, s + 1, e);
    return e;
}

Token *parse_char(int s, int e, int space) {
    Token *new = new_token(
        INT,
        (Token){
            .filename = ura.file->path,
            .line = ura.line,
            .s = s,
            .e = e,
            .space = space / TAB + ((space % TAB) != 0),
        }
    );
    new->shift = 8;
    char *buff = ura.file->buff;

    // UTF-8: a character is 1 to 4 bytes, read like a number in base 64
    // the first byte says how many bytes follow, minus its marker it is the first
    // digit
    //   0xC0 = 192 = 110xxxxx -> 2 bytes (é)
    //   0xE0 = 224 = 1110xxxx -> 3 bytes (€)
    //   0xF0 = 240 = 11110xxx -> 4 bytes (🦀)
    //   below 0x80 = 128      -> 1 byte, plain ascii char ('a')
    // every next byte is 0x80 (128) + a digit from 0 to 63
    // example 'é' = C3 A9: (0xC3 - 0xC0) * 64 + (0xA9 - 0x80) = 3 * 64 + 41 = 233
    unsigned char *str = (unsigned char *)parse_escaped(buff, s + 1, e - 1);
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
    new->num.value = value;
    // more than 1 byte does not fit a char, it is a unic
    if (count > 1)
        new->shift = 21;


    print("new token %t\n", new->type);
    return new;
}

void tokenize(File *file) {
    ura.file = file;
	file->buff = clean_text_size(file->buff);
	file->len = strlen(file->buff);
    int first_token = ura.tokens_count;
    tokenize_range(0, file->len, 0);
    for (int i = first_token; i < ura.tokens_count; i++) {
        ura.tokens[i]->s += file->start;
        ura.tokens[i]->e += file->start;
    }
    expand_uses(first_token);
}

char *clean_text_size(char *text) {
	char *res = arena_alloc(strlen(text) * TAB + 1, 1);
	int j = 0;
	for(int i = 0; text[i]; i++) {
		if(text[i] == '\t') {
			for(int k = 0; k < TAB; k++)
				res[j++] = ' ';
			continue;
		}
		res[j++] = text[i];
	}
	return res;
}

void tokenize_range(int start, int end, int space) {
    int s = start;
    int e = start;

    char *buff = ura.file->buff;
    while (e < end && buff[e] != '\0') {
        s = e;
        if (isspace(buff[e])) {
            while (buff[e] == '\n') {
                ura.line++;
                e++;
            }
            if (buff[s] == '\n') {
                space = 0;
                s = e - 1;
            }
            while (buff[e] != '\n' && isspace(buff[e])) {
                // calculated spaces after hitting \n in s position
                if (buff[s] == '\n') {
					if(buff[e] == ' ')
	                    space++;
					else { // TODO: check all space types
						eprint("unhandled space type\n");
						exit(1);
					}
				}
                e++;
            }
            //  if (buff[s] == '\n' && buff[e] && buff[e] != '\n') // a line with code
            //     assert_indent(s + 1, e, ura.scope->token->line, space);
            continue;
        }
        while (strncmp2(buff + s, "//", 2) && buff[e] && buff[e] != '\n')
            e++;
        if (e != s)
            continue;
        if (strncmp2(buff + s, "/*", 2)) {
            e = s + 2; // skip /*
            while (buff[e] != '\0' && !strncmp2(buff + e, "*/", 2)) {
                if (buff[e] == '\n')
                    ura.line++;
                e++;
            }
            assert_lexeme(buff, s, e);
            if (buff[e] == '\0')
                break;
            e += 2;
            continue;
        }

        if (strncmp2(buff + s, "$\"", 2)) {
            e = parse_format(s, space);
            continue;
        }
        if (buff[s] != '\0' && strchr2("\"\'", buff[s])) {
            char c = buff[s];
            e++;
            while (buff[e] != '\0' && buff[e] != c) {
                if (ends_string(buff, s, e))
                    break;
                char next = buff[e + 1];
                bool is_escape =
                    buff[e] == '\\' && next != '\0' && !strchr2("\n", next);
                e += is_escape ? 2 : 1;
            }
            if (buff[e] == c)
                e++; // skip last ' or "
            if (!assert_lexeme(buff, s, e))
                continue;
            if (c == '"')
                parse_string(s, e, space);
            else
                parse_char(s, e, space);
            for (int i = s; i < e; i++)
                ura.line += buff[i] == '\n';
            continue;
        }
        while (isdigit(buff[e]))
            e++;
        if (e != s && buff[e] == '.' && isdigit(buff[e + 1])) {
            e++; // skip .
            while (isdigit(buff[e]))
                e++;
            bool ends_name = !isalnum(buff[e + 1]) && buff[e + 1] != '_';
            if (buff[e] == 'f' && ends_name)
                e++;
        }
        if (e != s) {
            if (assert_lexeme(buff, s, e))
                parse_num(s, e, space);
            continue;
        }
        if (buff[s] == '@' && isalpha(buff[s + 1]))
            e++; // SKIP @
        bool starts_name = isalpha(buff[s]) || buff[s] == '_' || buff[s] == '@';
        while (starts_name && (isalnum(buff[e]) || buff[e] == '_'))
            e++;
        if (e != s) {
            parse_id(s, e, space);
            continue;
        }
        for (int i = 0; specials[i].value; i++) {
            char *value = specials[i].value;
            Type type = specials[i].token.type;
            int len = (int)strlen(value);
            if (strncmp2(value, buff + s, len)) {
#if 1
                Token *token = new_token(
                    type,
                    (Token){
                        .filename = ura.file->path,
                        .line = ura.line,
                        .s = s,
                        .e = s + len,
                        .space = space / TAB + ((space % TAB) != 0),
                    }
                );
#else
                // Token *token =
                new_token_at(type, s, s + len, space / TAB + ((space % TAB) != 0));
                // new_token(
                //     type,
                //     (Token){
                //         .s = s,
                //         .e = s + len,
                //         .space = space / TAB + ((space % TAB) != 0),
                //     }
                // );
                //  Token *token =
#endif
                if (type == DOTS)
                    space += TAB;
                e += len;
                break;
            }
        }
        if (e != s) {
            continue;
        }
        assert_lexeme(buff, s, s + 1);
        e++;
    }
}

void use_file(Token *use, Token *name) {
    char *path = find_package(name->chars.value, ura.file);
    bool ok = path != NULL;
    if (!assert_package(ok, name))
        return;
    File *file = new_file(path);
    open_file(file);
    if (!assert_reuse(name, file))
        return;
    file->start = end_of_sources();
    append(ura.files, file);
    append(ura.uses, file);
    File *outer = ura.file;
    int line = ura.line;
    int start = ura.tokens_count;
    ura.line = 1;
    tokenize(file);
    ura.file = outer;
    ura.line = line;
    ura.uses_count--;
    for (int i = start; i < ura.tokens_count; i++)
        ura.tokens[i]->space += use->space;
}

void expand_uses(int first_token) {
    int count = ura.tokens_count - first_token;
    Token **tokens = arena_alloc(count + 1, sizeof(Token *));
    memcpy(tokens, ura.tokens + first_token, count * sizeof(Token *));
    ura.tokens_count = first_token;
    for (int i = 0; i < count; i++) {
        Token *token = tokens[i];
        Token *name = i + 1 < count ? tokens[i + 1] : NULL;
        bool is_inner = token->space > 0 || ura.uses_count > 0;
        bool is_named = name && name->type == CHARS;
        if (token->type != USE || !is_inner || !is_named) {
            append(ura.tokens, token);
            continue;
        }
        use_file(token, name);
        i++; // SKIP the file name
    }
}

// AST
Node *new_node(Token *token) {
    Node *new = arena_alloc(1, sizeof(Node));
    new->token = token;
    print("new node %k\n", token);
    return new;
}

Token *peek(int pos) {
    assert(ura.exe_pos + pos <= ura.tokens_count);
    return ura.tokens[ura.exe_pos + pos];
}

Token *next() {
    Token *token = peek(0);
    if (token->type != LAST)
        ura.exe_pos++;
    return token;
}

int line_start(int pos) {
    File *file = file_at(pos);
    return file->start + line_start_in(file->buff, pos - file->start);
}

int line_at(int pos) {
    File *file = file_at(pos);
    int line = 1;
    for (int i = 0; i < pos - file->start; i++)
        line += file->buff[i] == '\n';
    return line;
}

char *token_text(Token *token) {
    return format("%.*s", token->e - token->s, text_at(token->s));
}

int line_end(int pos) {
    File *file = file_at(pos);
    return file->start + line_end_in(file->buff, pos - file->start);
}

bool inside(int space) {
    return ura.tokens[ura.exe_pos]->space > space;
}

void enter_scope(Node *node) {
    print(CYAN("enter scope %s\n"), node->token->name);
    append(ura.scopes, node);
    ura.scope = node;
}

void exit_scope() {
    print(CYAN("exit scope %s\n"), ura.scope->token->name);
    ura.scopes_count--;
    ura.scopes[ura.scopes_count] = NULL;
    if (ura.scopes_count > 0)
        ura.scope = ura.scopes[ura.scopes_count - 1];
    else
        ura.scope = NULL;
}

Node *find_template(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (child->token->type != TEMPLATE)
                continue;
            for (int k = 0; k < child->children_count; k++) {
                if (!strcmp2(child->children[k]->token->name, name))
                    continue;
                return child;
            }
        }
    }
    return NULL;
}

Node *find_template_type(char *name) {
    if (ura.types == NULL)
        return NULL;
    Node *params = ura.types->declared->args;
    for (int i = 0; i < ura.types->children_count; i++) {
        if (!strcmp2(params->children[i]->token->name, name))
            continue;
        return ura.types->children[i];
    }
    return NULL;
}

void parse_tokens(Node *scope, int start, Node *types) {
    int exe_pos = ura.exe_pos;
    Node *outer = ura.types;
    ura.exe_pos = start;
    ura.types = types;
    while (peek(0)->type != LAST) {
        Node *child = parse_expr(0);
        if (child->token->type != VAR)
            append(scope->children, child);
    }
    ura.exe_pos = exe_pos;
    ura.types = outer;
}

void parse_package(File *package) {
    open_file(package);
    for (int i = 0; i < ura.files_count; i++) {
        if (strcmp2(ura.files[i]->path, package->path))
            return;
    }
    package->start = end_of_sources();
    append(ura.files, package);

    File *file = ura.file;
    int line = ura.line;
    int start = ura.tokens_count;
    ura.line = 1;
    tokenize(package);
    new_token(LAST, (Token){});
    ura.file = file;
    ura.line = line;
    parse_tokens(ura.scope, start, ura.types);
}

int copy_tokens(int from_pos, int to_pos, char *from, char *to, int space) {
    int start = ura.tokens_count;
    for (int i = from_pos; i < to_pos; i++) {
        Token *copy = new_token(ura.tokens[i]->type, (Token){});
        *copy = *ura.tokens[i];
        copy->space += space;
        if (from && copy->type == ID && strcmp2(copy->name, from))
            copy->name = to;
    }
    new_token(LAST, (Token){});
    return start;
}

bool is_declaration(int i) {
    Token *curr = ura.tokens[i];
    Token *after = ura.tokens[i + 1];
    bool is_same_line = after->line == curr->line;
    return curr->type == ID && is_same_line && (after->is_type || after->type == ID);
}

bool is_line_start(int i) {
    Token *curr = ura.tokens[i];
    Token *prev = ura.tokens[i - 1];
    return curr->line != prev->line || file_at(curr->s) != file_at(prev->s);
}

void prefix_names(Token *mod) {
    expand(char *, names);
    zero_init(names);
    int start = ura.exe_pos;
    int end = start;
    while (ura.tokens[end]->space > mod->space)
        end++;
    int space = ura.tokens[start]->space;
    int macro_space = -1;
    for (int i = start; i < end; i++) {
        Token *curr = ura.tokens[i];
        Token *after = ura.tokens[i + 1];
        bool starts_line = is_line_start(i);
        if (starts_line && curr->space <= macro_space)
            macro_space = -1;

        bool is_macro = includes(curr->type, MACRO_IF, MACRO_ELIF, MACRO_ELSE, 0);
        if (starts_line && curr->space == space && is_macro)
            macro_space = curr->space;

        bool is_in_macro = macro_space != -1 && curr->space == macro_space + 1;
        bool is_head = starts_line && (curr->space == space || is_in_macro);
        bool is_head_type = includes(
            curr->type,
            STRUCT, FN_DEC, PROTO, ENUM, MODULE,
            0
        );
        bool is_named = is_head_type && after->type == ID;
        if (is_head && is_named)
            append(names, after->name);
        if (is_head && is_declaration(i))
            append(names, curr->name);
    }
    int struct_space = -1;
    for (int i = start; i < end; i++) {
        Token *curr = ura.tokens[i];
        bool starts_line = is_line_start(i);
        if (starts_line && curr->space <= struct_space)
            struct_space = -1;
        if (starts_line && curr->type == STRUCT)
            struct_space = curr->space;
        bool is_member = includes(ura.tokens[i - 1]->type, DOT, DOUBLE_DOTS, 0);
        bool is_in_struct = struct_space != -1 && curr->space == struct_space + 1;
        bool is_attr = starts_line && is_in_struct && is_declaration(i);
        if (curr->type != ID || is_member || is_attr)
            continue;
        for (size_t j = 0; j < names_count; j++) {
            if (!strcmp2(curr->name, names[j]))
                continue;
            curr->name = format("%s::%s", mod->name, curr->name);
            break;
        }
    }
}

void parse_module_name(Token *token) {
    while (peek(0)->type == DOUBLE_DOTS && peek(1)->type == ID) {
        if (!is_module(token->name))
            return;
        next(); // SKIP ::
        Token *member = next();
        token->name = format("%s::%s", token->name, member->name);
        token->e = member->e;
    }
}

void inherit_from_parent(Node *st, Node *parent) {
    Token *body = parent->token;
    Node *parents = body->body.parents;
    for (int i = 0; parents && i < parents->children_count; i++)
        inherit_from_parent(st, parents->children[i]);
    int space = st->token->space - parent->token->space;
    int start = copy_tokens(
        body->body.start, body->body.end, parent->token->name, st->token->name, space
    );
    parse_tokens(st, start, body->body.types);
}

void remove_overridden(Node *st) {
    for (int i = 0; i < st->children_count; i++) {
        Node *method = st->children[i];
        if (method->token->type != FN_DEC)
            continue;
        for (int j = i + 1; j < st->children_count; j++) {
            Node *other = st->children[j];
            if (other->token->type != FN_DEC)
                continue;
            if (!strcmp2(other->token->name, method->token->name))
                continue;
            if (other->token->is_pub != method->token->is_pub)
                continue;
            if (!same_params(other, method))
                continue;
            for (int k = i; k < st->children_count - 1; k++)
                st->children[k] = st->children[k + 1];
            st->children_count--;
            i--;
            break;
        }
    }
}

Node *parse_types(Token *token) {
    Node *types = new_node(new_token(TYPES, *token));
    types->token->name = token->name;
    next(); // SKIP <
    while (!includes(peek(0)->type, GT, LAST, 0)) {
        Node *type = parse_type();
        if (type == NULL)
            break;
        append(types->children, type);
        if (peek(0)->type != COMA)
            break;
        next(); // SKIP ,
    }
    Token *close = peek(0);
    if (close->type == RSHIFT) {
        close->type = GT;
        close->s++;
        return types;
    }
    expect(GT, format("expected '>' after the types of '%s'", token->name), ">");
    return types;
}

char *init_from_template(Node *template, Node *types) {
    Token *token = types->token;
    char *name = format("%s<", token->name);
    for (int i = 0; i < types->children_count; i++)
        name = format("%s%s%s", name, i ? "," : "", type_name(types->children[i]));
    name = format("%s>", name);

    if (find_struct(name) || find_function(name))
        return name;

    Token *body = template->token;
    int start = copy_tokens(body->body.start, body->body.end, NULL, NULL, 0);
    int end = ura.tokens_count - 1;
    int top = ura.tokens[start]->space;
    char *suffix = name + strlen(token->name);
    for (int i = start; i + 1 < end; i++) {
        Token *keyword = ura.tokens[i];
        Token *declared = ura.tokens[i + 1];
        bool is_declaration = includes(keyword->type, FN_DEC, STRUCT, 0) &&
                              keyword->space == top && declared->type == ID;
        if (!is_declaration)
            continue;
        char *old = declared->name;
        char *renamed = format("%s%s", old, suffix);
        for (int j = start; j < end; j++)
            if (ura.tokens[j]->type == ID && strcmp2(ura.tokens[j]->name, old))
                ura.tokens[j]->name = renamed;
    }
    types->declared = template;
    parse_tokens(template->scope, start, types);
    return name;
}

Node *parse_type(void) {
    Token *token = peek(0);
    Token *prev = ura.tokens[ura.exe_pos - 1];
    Token *after = peek(1);
    bool is_ref = includes(token->type, REF, AND, 0) && token->line == prev->line;
    bool is_id = after->type == ID;
    bool is_variable = is_id && find_variable(after->name, after->s);
    bool is_function = is_id && find_function(after->name);
    bool is_struct = is_id && find_struct(after->name);
    bool is_name = is_id && !is_variable && !is_function;
    bool is_unary = includes(after->type, ADD, SUB, LBRACK, 0);
    bool is_operator = precedence[after->type] && !is_unary;
    bool is_type_name = after->is_type || is_name || is_struct;
    if (is_ref && (is_type_name || is_operator)) {
        next(); // SKIP & or &&
        Node *ref = new_node(token);
        ref->token->is_type = true;
        ref->data_type = parse_type();
        if (token->type == AND)
            ref->data_type = new_type(REF, 0, ref->data_type);
        token->type = REF;
        return ref;
    }
    if (token->type == ID && token->line == prev->line)
        token->is_type = true;
    if (token->is_type) {
        next();
        parse_module_name(token);
        Node *node = token->type == ID ? find_template_type(token->name) : NULL;
        Node *template = token->type == ID ? find_template(token->name) : NULL;
        if (node == NULL)
            node = new_node(token);
        Node *types = peek(0)->type == LT ? parse_types(token) : NULL;
        if (assert_template_use(token, template, types))
            token->name = init_from_template(template, types);
        while (peek(0)->type == LBRACK && peek(0)->line == token->line) {
            next(); // SKIP [
            Node *array = new_node(new_token(ARRAY, *token));
            array->token->is_type = true;
            array->data_type = node;
            bool has_size = peek(0)->type != RBRACK && peek(0)->line == token->line;
            if (has_size)
                array->index = parse_expr(0);
            expect(RBRACK, "expected ']' after '['", "]");
            node = array;
        }
        return node;
    }
    return NULL;
}

Node *parse_block(Token *token) {
    Node *node = new_node(token);
    enter_scope(node);
    bool is_same_line = peek(0)->line == token->line;
    bool has_cond = peek(0)->type != DOTS && (token->type != ELSE || is_same_line);
    if (has_cond)
        node->cond = parse_expr(0);
    char *after[LAST + 1] = {
        [IF] = "the if condition",
        [ELIF] = "the elif condition",
        [LOOP] = "the while condition",
        [ELSE] = "else",
    };
    expect(DOTS, format("expected ':' after %s", after[token->type]), ":");
    while (inside(node->token->space)) {
        append(node->children, parse_expr(0));
    }
    exit_scope();
    return node;
}

Node *parse_prime() {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) { // clang-format off
    case INT: case FLOAT: case CHARS: { // clang-format on
        if (token->is_type)
            assert_type_first(token);
        return new_node(token);
    }
    case ZERO: {
        return new_node(token);
    }
    case SCOPE: {
        node = new_node(token);
        enter_scope(node);
        append(ura.modules, "ura");
        while (peek(0)->type != LAST)
            append(node->children, parse_expr(0));
        exit_scope();
        return node;
    }
    case ID: {
        Token *prev = ura.tokens[ura.exe_pos - 2];
        bool is_member = includes(prev->type, DOT, DOUBLE_DOTS, 0);
        bool starts_line = is_line_start(ura.exe_pos - 1);
        parse_module_name(token);
        Token *builtin = is_member ? NULL : find_builtin(token->name);
        if (builtin) {
            token->type = builtin->type;
            token->num = builtin->num;
        }
        bool is_value = builtin && includes(builtin->type, ARGC, ARGV, 0);
        if (is_value)
            return new_node(token);
        bool is_source = includes(token->type, LINE_NUMBER, FILE_NAME, FN_NAME, 0);
        if (is_source)
            return new_node(source_value(token));
        if (builtin && peek(0)->type != LPARENT) {
            expect(LPARENT, format("expected '(' after '%s'", token->name), "(x)");
            node = new_node(token);
            append(node->children, parse_prime());
            return node;
        }
        Node *template_type = find_template_type(token->name);
        Node *template = find_template(token->name);
        bool is_template_use = template && peek(0)->type == LT;
        if (template_type || is_template_use || find_struct(token->name))
            assert_type_first(token);
        if (template_type)
            return template_type;
        Node *types = is_template_use ? parse_types(token) : NULL;
        if (types && assert_template_use(token, template, types))
            token->name = init_from_template(template, types);
        Node *type = builtin || !starts_line ? NULL : parse_type();
        if (type != NULL) {
            node = new_node(new_token(DECLARE, *token));
            node->var = new_node(token);
            node->var->token->type = VAR;
            node->var->data_type = type;
            append(ura.scope->children, node);
            node = node->var;
        } else if (peek(0)->type == LPARENT) {
            if (builtin == NULL)
                token->type = FN_CALL;
            node = new_node(token);
            next(); // SKIP (
            while (!includes(peek(0)->type, RPARENT, LAST, 0)) {
                append(node->children, parse_expr(0));
                if (assert_comma(token, peek(0), "arguments"))
                    continue;
                if (peek(0)->type != COMA)
                    break;
                assert_comma(next(), peek(0), "arguments");
            }
            expect(
                RPARENT,
                format("expected ')' to close the call to '%s'", token->name),
                ")"
            );
        } else
            node = new_node(token);
        return node;
    }
    case SELF: {
        return new_node(token);
    }
    case LPARENT: {
        node = parse_expr(0);
        expect(RPARENT, "expected ')' to close this '('", ")");
        return node;
    } // clang-format off
    case SUB: case BNOT: { // clang-format on
        if (token->type == SUB)
            token->type = NEG;
        node = new_node(token);
        node->operand = parse_expr(precedence[DOT] - 1);
        return node;
    }
    case REF: {
        node = new_node(token);
        node->operand = parse_expr(precedence[DOT] - 1);
        return node;
    }
    case AND: {
        token->type = REF;
        node = new_node(token);
        node->operand = new_node(new_token(REF, *token));
        node->operand->operand = parse_expr(precedence[DOT] - 1);
        return node;
    }
    case ADD: {
        return parse_prime();
    } // clang-format off
    case FN_DEC: case PROTO: case OPERATOR: { // clang-format on
        node = new_node(token);
        bool is_operator = token->type == OPERATOR;
        if (is_operator) {
            token->type = FN_DEC;
            token = next();
            node->token->name = token_text(token);
            if (token->type == LBRACK) {
                expect(RBRACK, "expected ']' after 'operator ['", "]");
                node->token->name = "[]";
            }
            // TODO: loop onze sizeif binary_ops
            for (int i = 0; i <= LAST; i++) {
                if (binary_ops[i] && strcmp2(binary_ops[i], node->token->name)) {
                    token->type = i;
                    break;
                }
            }
        } else if (peek(0)->type != ID)
            node->token->name = "";
        else {
            token = next();
            node->token->name = token->name;
        }
        node->right = new_node(new_token(ARGS, *token));
        if (is_operator)
            node->args->symbol = new_node(token);
        if (peek(0)->type == LPARENT) {
            next(); // SKIP (
            while (peek(0)->type != RPARENT) {
                Token *token = peek(0);
                if (token->type == ID) {
                    token->type = PARAM;
                    Node *param = new_node(token);
                    next(); // skip ID
                    param->data_type = parse_type();
                    bool ok = param->data_type != NULL;
                    assert_param_type(ok, param);
                    append(node->right->children, param);
                } else if (token->type == VARIADIC) {
                    append(node->right->children, new_node(token));
                    next(); // skip ...
                } else
                    break;
                if (assert_comma(token, peek(0), "parameters"))
                    continue;
                if (peek(0)->type != COMA)
                    break;
                next(); // SKIP ,
            }
            expect(RPARENT, "expected ')' after the parameters", ")");
        } else
            expect(LPARENT, "expected '(' after the function name", "()");
        node->ret_type = parse_type();
        if (node->token->type == PROTO)
            return node;
        expect(DOTS, "expected ':' after the function header", ":");
        enter_scope(node);
        while (inside(node->token->space)) {
            append(node->children, parse_expr(0));
        }
        exit_scope();
        return node;
    }
    case RETURN: {
		// print("%s:%d >> return %k\n", FILE, LINE, token);
        node = new_node(token);
        if (peek(0)->line == token->line)
            node->left = parse_expr(0);
		// print("%s:%d %n\n", FILE, LINE, node);
		// exit(1);
        return node;
    }
    case IF:
    case LOOP: {
        node = parse_block(token);
        if (token->type != IF)
            return node;
        Node *branch = node;
        while (includes(peek(0)->type, ELIF, ELSE, 0)) {
            if (peek(0)->space != token->space)
                break;
            branch->next = parse_block(next());
            branch = branch->next;
        }
        return node;
    }
    case FOR: {
        node = new_node(token);
        enter_scope(node);
        Token *name = peek(0);
        expect(ID, "expected a name after 'for'", " x");
        Node *declare = new_node(new_token(DECLARE, *name));
        declare->var = new_node(name);
        declare->var->token->type = VAR;
        node->var = declare->var;
        append(node->children, declare);
        expect(IN, "expected 'in' after the loop name", " in");
        node->right = parse_expr(0);
        expect(DOTS, "expected ':' after the for line", ":");
        while (inside(token->space))
            append(node->children, parse_expr(0));
        exit_scope();
        return node;
    }
    case MATCH: {
        node = new_node(token);
        enter_scope(node);
        Node *declare = new_node(new_token(DECLARE, *token));
        declare->var = new_node(new_token(VAR, *token));
        declare->var->token->name = format("match@%d", token->s);
        append(node->children, declare);
        node->left = new_node(new_token(ASSIGN, *token));
        node->left->left = declare->var;
        node->left->right = parse_expr(0);
        expect(DOTS, "expected ':' after the match value", ":");
        while (inside(token->space)) {
            Token *head = next();
            if (!includes(head->type, CASE, DEFAULT, 0)) {
                assert_unexpected(head);
                continue;
            }
            Node *branch = new_node(head);
            while (head->type == CASE && !includes(peek(0)->type, DOTS, LAST, 0)) {
                Node *value = new_node(new_token(ID, *head));
                value->token->name = declare->var->token->name;
                Node *eq = new_node(new_token(EQ, *head));
                eq->left = value;
                eq->right = parse_expr(0);
                Node *cond = eq;
                if (branch->cond) {
                    cond = new_node(new_token(OR, *head));
                    cond->left = branch->cond;
                    cond->right = eq;
                }
                branch->cond = cond;
                if (peek(0)->type != COMA)
                    break;
                next(); // SKIP ,
            }
            expect(DOTS, format("expected ':' after %s", token_text(head)), ":");
            enter_scope(branch);
            while (inside(head->space))
                append(branch->children, parse_expr(0));
            exit_scope();
            append(node->children, branch);
        }
        exit_scope();
        return node;
    }
    case TRY: {
        ura.throws = true;
        node = new_node(token);
        expect(DOTS, "expected ':' after 'try'", ":");
        enter_scope(node);
        while (inside(token->space))
            append(node->children, parse_expr(0));
        exit_scope();
        Node *branch = node;
        while (peek(0)->type == CATCH && peek(0)->space == token->space) {
            Token *head = next();
            branch->next = new_node(head);
            branch = branch->next;
            enter_scope(branch);
            if (peek(0)->type == ID && peek(0)->line == head->line) {
                Token *name = next();
                Node *declare = new_node(new_token(DECLARE, *name));
                declare->var = new_node(name);
                declare->var->token->type = VAR;
                declare->var->data_type = parse_type();
                branch->var = declare->var;
                if (declare->var->data_type)
                    append(branch->children, declare);
            }
            expect(DOTS, "expected ':' after the catch type", ":");
            while (inside(head->space))
                append(branch->children, parse_expr(0));
            exit_scope();
        }
        return node;
    }
    case THROW: {
        ura.throws = true;
        node = new_node(token);
        node->left = parse_expr(0);
        return node;
    } // clang-format off
    case CATCH: case CASE: case DEFAULT: { // clang-format on
        assert_unexpected(token);
        while (peek(0)->line == token->line || inside(token->space))
            next();
        return new_node(token);
    } // clang-format off
    case ELIF: case ELSE: { // clang-format on
        assert_unexpected(token);
        return parse_block(token);
    }
    case PUB: {
        node = parse_prime();
        node->token->is_pub = true;
        return node;
    }
    case LINK: {
        node = new_node(token);
        if (peek(0)->type == CHARS)
            node->token->name = next()->chars.value;
        else
            expect(CHARS, "expected a library name after 'link'", " \"name\"");
        return node;
    }
    case LCBRACK: {
        node = new_node(token);
        while (peek(0)->type == ID) {
			// print(RED("%s:%d i found >> %k\n"), FILE, LINE, peek(0));
            Node *attr = new_node(next());
            attr->token->type = ATTR;
            expect(DOTS, "expected ':' after the attribute name", ":");
            attr->left = parse_expr(0);
            append(node->children, attr);
			// print(RED("%s:%d >> \n%n\n"), FILE, LINE, attr);
            if (peek(0)->type != COMA) {
				// print(RED("%s:%d break the loop >> %k\n"), FILE, LINE, peek(0));
                break;
			}
			// print(RED("%s:%d skip >> %k\n"), FILE, LINE, peek(0));
			while(peek(0)->type == COMA)
	            next(); // SKIP ,
        }
		// exit(1);
		// print(RED("%s:%d after loop i found >> %k\n"), FILE, LINE, peek(0));
        if(!expect(RCBRACK, "expected '}' to close the struct value", "}")){
			// Token *tmp = peek(0);
			// print("%s:%d i found >> %k index %d\n", FILE, LINE, tmp, tmp->tk_index);
			// print(">>> %n\n", node);
			// exit(1);
		}
        return node;
    }
    case LBRACK: {
        node = new_node(token);
        while (!includes(peek(0)->type, RBRACK, LAST, 0)) {
            append(node->children, parse_expr(0));
            if (assert_comma(token, peek(0), "elements"))
                continue;
            if (peek(0)->type != COMA)
                break;
            next(); // SKIP ,
        }
        expect(RBRACK, "expected ']' to close the array", "]");
		// print("%s:%d retuning %n", FILE, LINE, node);
		// print("curr token is %k\n", peek(0));
        return node;
    }
    case NOT: {
        node = new_node(token);
        node->operand = parse_expr(precedence[AND]);
        return node;
    }
    case MACRO_IF: {
        node = new_node(token);
        bool is_done = false;
        Token *branch = token;
        while (true) {
            bool is_chosen = !is_done;
            if (branch->type != MACRO_ELSE)
                is_chosen = macro_cond(parse_expr(0)) && !is_done;
            expect(DOTS, format("expected ':' after %s", token_text(branch)), ":");
            while (inside(branch->space)) {
                if (is_chosen) {
                    append(ura.scope->children, parse_expr(0));
                } else
                    next();
            }
            is_done = is_done || is_chosen;
            bool is_branch = includes(peek(0)->type, MACRO_ELIF, MACRO_ELSE, 0);
            bool is_same_level = peek(0)->space == token->space;
            if (!is_branch || !is_same_level)
                break;
            branch = next();
        }
        return node;
    }
    case MACRO_ERROR: {
        node = new_node(token);
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            node->left = new_node(next());
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            node->right = new_node(next());
        assert_macro(node);
        return node;
    } // clang-format off
    case MACRO_NAME: case MACRO_VERSION: case MACRO_ENTRY:
    case MACRO_URL: case MACRO_PATH: case MACRO_CMD: { // clang-format on
        node = new_node(token);
        while (peek(0)->type == CHARS && peek(0)->line == token->line)
            append(node->children, new_node(next()));
        assert_config(node);
        return node;
    } // clang-format off
    case MACRO_DEPENDENCY: case MACRO_SCRIPT: { // clang-format on
        node = new_node(token);
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            token->name = next()->chars.value;
        expect(DOTS, "expected ':' after the name", ":");
        Node *version = NULL;
        while (inside(token->space)) {
            Node *field = parse_expr(0);
            if (!assert_config_field(node, field))
                continue;
            if (field->token->type == MACRO_VERSION)
                version = field;
            else if (field->token->type == MACRO_CMD)
                node->cmd = field;
            else
                node->url = field;
        }
        if (node->url && token->type == MACRO_DEPENDENCY)
            node->url->version = version;
        assert_config(node);
        assert_config_block(node);
        return node;
    }
    case USE: {
        node = new_node(token);
        if (peek(0)->type != CHARS) {
            expect(CHARS, "expected a package name after 'use'", " \"@/name\"");
            return node;
        }
        Token *name = next();
        char *path = find_package(name->chars.value, file_at(name->s));
        bool ok = path != NULL;
        if (assert_package(ok, name))
            parse_package(new_file(path));
        return node;
    }
    case TEMPLATE: {
        node = new_node(token);
        node->scope = ura.scope;
        node->args = new_node(new_token(ARGS, *token));
        expect(LT, "expected '<' after 'template'", "<T>");
        while (!includes(peek(0)->type, GT, LAST, 0)) {
            append(node->args->children, new_node(next()));
            if (peek(0)->type != COMA)
                break;
            next(); // SKIP ,
        }
        expect(GT, "expected '>' after the template names", ">");
        expect(DOTS, "expected ':' after 'template'", ":");
        int space = peek(0)->space;
        token->body.start = ura.exe_pos;
        while (inside(token->space)) {
            Token *prev = ura.tokens[ura.exe_pos - 1];
            Token *curr = next();
            bool is_head = curr->space == space && curr->line != prev->line;
            bool is_named = includes(curr->type, STRUCT, FN_DEC, PUB, 0);
            if (is_head && is_named && peek(0)->type == ID)
                append(node->children, new_node(peek(0)));
        }
        token->body.end = ura.exe_pos;
        return node;
    }
    case ENUM: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else
            expect(ID, "expected an enum name after 'enum'", " Name");
        expect(DOTS, "expected ':' after the enum name", ":");
        long long value = 0;
        while (true) {
            Token *name = peek(0);
            bool is_near = name->line == token->line || inside(token->space);
            if (name->type != ID || !is_near)
                break;
            Node *variant = new_node(next());
            if (peek(0)->type == ASSIGN) {
                next(); // SKIP =
                variant->left = parse_prime();
                if (variant->left->token->type == INT)
                    value = variant->left->token->num.value;
            }
            name->num.value = value++;
            append(node->children, variant);
            if (peek(0)->type == COMA)
                next(); // SKIP ,
        }
        return node;
    }
    case MODULE: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else
            expect(ID, "expected a mod name after 'mod'", " name");
        expect(DOTS, "expected ':' after the mod name", ":");
        bool ok = ura.scope->token->type == SCOPE;
        if (!assert_module(ok, node))
            return node;
        append(ura.modules, node->token->name);
        prefix_names(node->token);
        while (inside(token->space)) {
            Node *child = parse_expr(0);
            if (child->token->type != VAR)
                append(ura.scope->children, child);
        }
        return node;
    }
    case STRUCT: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else {
            expect(ID, "expected a struct name after 'struct'", " Name");
            node->token->name = "";
        }
        Node *parents = new_node(new_token(TYPES, *token));
        if (peek(0)->type == FROM) {
            next(); // SKIP from
            while (true) {
                Token *name = peek(0);
                Node *type = parse_type();
                Node *parent = type ? find_struct(type->token->name) : NULL;
                bool ok = parent != NULL;
                if (assert_parent(ok, name))
                    append(parents->children, parent);
                if (peek(0)->type != AND_LIT)
                    break;
                next(); // SKIP and
            }
        }
        expect(DOTS, "expected ':' after the struct name", ":");
        enter_scope(node);
        for (int i = 0; i < parents->children_count; i++)
            inherit_from_parent(node, parents->children[i]);
        int end = ura.exe_pos;
        while (ura.tokens[end]->type != LAST && ura.tokens[end]->space > token->space)
            end++;
        token->body.start = copy_tokens(ura.exe_pos, end, NULL, NULL, 0);
        token->body.end = token->body.start + end - ura.exe_pos;
        token->body.types = ura.types;
        token->body.parents = parents;
        while (inside(node->token->space)) {
            Node *child = parse_expr(0);
            if (!includes(child->token->type, FN_DEC, TEMPLATE, STRUCT, 0)) {
                bool ok = includes(child->token->type, VAR, MACRO_IF, MACRO_ERROR, 0);
                if (!assert_attribute(ok, child)) {
                    // print("%s:%d >>> %n\n", FILE, LINE, child);
                    // exit(1);
                }
                continue;
            }
            append(node->children, child);
        }
        exit_scope();
        remove_overridden(node);
        for (int i = 0; i < node->children_count; i++) {
            Node *method = node->children[i];
            if (method->token->type != FN_DEC || method->token->is_pub)
                continue;
            Node *type = new_type(ID, 0, NULL);
            type->token->name = node->token->name;
            Node *self = new_node(new_token(PARAM, *method->token));
            self->token->name = "self";
            self->data_type = new_type(REF, 0, type);
            Node *args = method->args;
            append(args->children, self);
            for (int j = args->children_count - 1; j > 0; j--)
                args->children[j] = args->children[j - 1];
            args->children[0] = self;
        }
        return node;
    } // clang-format off
    case BRK: case CNT: { // clang-format on
        node = new_node(token);
        for (int i = ura.scopes_count - 1; i >= 0 && node->loop == NULL; i--) {
            Type type = ura.scopes[i]->token->type;
            bool is_loop = includes(type, LOOP, FOR, 0);
            bool is_target = is_loop || (type == MATCH && token->type == BRK);
            if (is_target)
                node->loop = ura.scopes[i];
        }
        return node;
    }
    default: {
        assert_unexpected(token);
        return new_node(token);
    }
    }
    return NULL;
}

Node *parse_expr(int min_op) {
    Node *left = parse_prime();
    while (true) {
        Token *prev = ura.tokens[ura.exe_pos - 1];
        bool is_band = peek(0)->type == REF && peek(0)->line == prev->line;
        if (is_band)
            peek(0)->type = BAND;
        if (peek(0)->type == AND_LIT)
            peek(0)->type = AND;
        if (peek(0)->type == OR_LIT)
            peek(0)->type = OR;
        int e = peek(0)->type;
        if (e == LAST)
            break;
        assert(e >= 0 && e <= LAST);
        // printf("accessing %d\n", e);
        int op = precedence[e];
        if (op <= min_op)
            break;
        Node *node = new_node(next());
        node->left = left;
        Token *token = node->token;
        bool is_missing = peek(0)->type == LAST || peek(0)->line != token->line;
        if (!is_missing && token->type == LBRACK) {
            token->type = ACCESS;
            node->index = parse_expr(0);
            expect(RBRACK, "expected ']' after the index", "]");
        } else if (!is_missing && token->type == AS)
            node->to_type = parse_type();
        else if (!is_missing)
            node->right = parse_expr(op);
        assert_operand(node);
        if (is_missing)
            return node;
        left = node;
    }
    return left;
}

// ANALYZE
Node *find_in_children(Node *parent, char *name) {
    for (int j = 0; j < parent->children_count; j++) {
        Node *child = parent->children[j];
        if (!includes(child->token->type, DECLARE, PARAM, 0))
            continue;
        Node *id = child->token->type == DECLARE ? child->var : child;
        if (strcmp2(id->token->name, name))
            return id;
    }
    return NULL;
}

Node *find_in_scope(Node *scope, char *name) {
    Node *found = NULL;
    if (scope->token->type == FN_DEC)
        found = find_in_children(scope->args, name);
    if (found == NULL)
        found = find_in_children(scope, name);
    return found;
}

Node *find_variable(char *name, int pos) {
    bool is_past_fn = false;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        bool is_global = scope->token->type == SCOPE;
        if (is_past_fn && !is_global)
            continue;
        is_past_fn = is_past_fn || scope->token->type == FN_DEC;
        Node *param =
            scope->token->type == FN_DEC ? find_in_children(scope->args, name) : NULL;
        if (param)
            return param;
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            if (child->token->type != DECLARE ||
                !strcmp2(child->var->token->name, name))
                continue;
            if (is_global || child->var->token->s <= pos)
                return child->var;
        }
    }
    return NULL;
}

Node *find_scope(Type type) {
    for (int i = ura.scopes_count - 1; i >= 0; i--)
        if (ura.scopes[i]->token->type == type)
            return ura.scopes[i];
    return NULL;
}

int name_diff(char *a, char *b) {
    int diff = 0;
    int k = 0;
    for (; a[k] && b[k]; k++)
        diff += a[k] != b[k];
    return diff + strlen(a + k) + strlen(b + k);
}

Node *find_variable_like(char *name, int pos, int max_diff) {
    Node *found = NULL;
    int found_diff = max_diff + 1;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            bool is_before =
                child->token->type == DECLARE && child->var->token->s <= pos;
            if (!is_before)
                continue;
            int diff = name_diff(child->var->token->name, name);
            if (diff < found_diff) {
                found = child->var;
                found_diff = diff;
            }
        }
    }
    return found;
}

Node *find_function_like(char *name, int max_diff) {
    Node *found = NULL;
    int found_diff = max_diff + 1;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        if (curr->token->type == STRUCT)
            continue;
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            bool is_function = includes(child->token->type, FN_DEC, PROTO, 0);
            bool is_operator = child->args && child->args->symbol;
            if (!is_function || is_operator)
                continue;
            int diff = name_diff(child->token->name, name);
            if (diff >= found_diff)
                continue;
            found = child;
            found_diff = diff;
        }
    }
    return found;
}

Node *find_function(char *name) {
    return find_function_like(name, 0);
}

Node *find_method_named(Node *st, char *name) {
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        bool is_method = child->token->type == FN_DEC && !child->token->is_pub;
        if (is_method && strcmp2(child->token->name, name))
            return child;
    }
    return NULL;
}

Node *find_attr(Node *st, char *name, int *index) {
    *index = 0;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != DECLARE)
            continue;
        if (strcmp2(child->var->token->name, name))
            return child->var;
        (*index)++;
    }
    return NULL;
}

bool is_parent(char *name, Node *st) {
    Node *parents = st->token->body.parents;
    for (int i = 0; parents && i < parents->children_count; i++) {
        Node *parent = parents->children[i];
        if (strcmp2(parent->token->name, name) || is_parent(name, parent))
            return true;
    }
    return false;
}

bool is_child_ref(Node *found, Node *type) {
    if (found && found->token->type == REF)
        found = found->data_type;
    bool takes_ref = type && type->token->type == REF;
    Node *base = takes_ref ? type->data_type : NULL;
    bool is_struct = found && found->token->type == ID && found->declared;
    bool is_named = base && base->token->type == ID;
    return is_struct && is_named && is_parent(base->token->name, found->declared);
}

bool fits_args(Node *fn, Node **args, int args_count, int self) {
    if (fn->args->children_count != args_count + self)
        return false;
    for (int j = 0; j < args_count; j++) {
        Node *type = fn->args->children[j + self]->data_type;
        Node *found = type_of(args[j]);
        bool is_unknown = found == NULL || type == NULL;
        bool is_text = args[j]->token->type == CHARS && is_chars(type);
        bool is_list = args[j]->token->type == LBRACK && type && element_type_of(type);
        if (is_unknown || is_text || is_list ||
            (is_number(args[j]) && is_number(type)))
            continue;
        bool takes_ref = type->token->type == REF && found->token->type != REF;
        if (is_child_ref(found, type))
            continue;
        if (!same_type(found, takes_ref ? type->data_type : type))
            return false;
    }
    return true;
}

Node *find_method(Node *st, char *name, Node **args, int args_count, bool is_pub) {
    int self = is_pub ? 0 : 1;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != FN_DEC || child->token->is_pub != is_pub)
            continue;
        if (strcmp2(child->token->name, name) &&
            fits_args(child, args, args_count, self))
            return child;
    }
    return NULL;
}

Node *find_operator(Type op, Node *type, Node **args, int args_count) {
    if (type && type->token->type == REF)
        type = type->data_type;
    for (int i = ura.scopes_count - 1; i >= 0 && type; i--) {
        Node *scope = ura.scopes[i];
        for (int j = 0; j < scope->children_count; j++) {
            Node *fn = scope->children[j];
            bool is_fn = fn->token->type == FN_DEC && fn->args;
            bool is_operator = is_fn && fn->args->symbol;
            if (!is_operator || fn->args->children_count == 0)
                continue;
            if (fn->args->symbol->token->type != op)
                continue;
            Node *first = fn->args->children[0]->data_type;
            bool is_ref = first && first->token->type == REF;
            Node *left = is_ref ? first->data_type : first;
            bool is_same = left && same_type(left, type);
            if (!is_same && !is_child_ref(type, first))
                continue;
            if (args == NULL || fits_args(fn, args, args_count, 1))
                return fn;
        }
    }
    return NULL;
}

Node *find_overload(char *name, Node **args, int args_count) {
    Node *proto = NULL;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            bool is_function =
                includes(child->token->type, FN_DEC, PROTO, 0) && child->args;
            if (!is_function || !strcmp2(child->token->name, name))
                continue;
            if (!fits_args(child, args, args_count, 0))
                continue;
            if (child->token->type == FN_DEC)
                return child;
            proto = proto ? proto : child;
        }
    }
    return proto;
}

Node *find_struct(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (child->token->type != STRUCT)
                continue;
            if (!strcmp2(child->token->name, name))
                continue;
            return child;
        }
    }
    return NULL;
}

Node *find_enum(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (child->token->type == ENUM && strcmp2(child->token->name, name))
                return child;
        }
    }
    return NULL;
}

bool is_unused_global(Token *var) {
    bool is_package = file_at(var->s) != ura.files[0];
    return is_package && !var->is_used;
}

bool is_module(char *name) {
    for (size_t i = 0; i < ura.modules_count; i++) {
        if (strcmp2(ura.modules[i], name))
            return true;
    }
    return false;
}

Node *new_type(Type type, int shift, Node *data_type) {
    Node *node = new_node(new_token(type, (Token){}));
    node->token->is_type = true;
    node->token->shift = shift;
    node->data_type = data_type;
    return node;
}

Node *bool_type(void) {
    return new_type(INT, 1, NULL);
}

Node *chars_type(void) {
    return new_type(ARRAY, 0, new_type(INT, 8, NULL));
}

bool is_chars(Node *type) {
    Node *element = type && type->token->type == ARRAY ? type->data_type : NULL;
    return element && element->token->type == INT && element->token->shift == 8;
}

Node *element_type_of(Node *type) {
    if (type->token->type == ARRAY)
        return type->data_type;
    bool is_array =
        type->token->type == ID && strncmp2(type->token->name, "Array<", 6);
    if (is_array && type->declared == NULL)
        type->declared = find_struct(type->token->name);
    if (!is_array || type->declared == NULL)
        return NULL;
    int index = 0;
    Node *data = find_attr(type->declared, "data", &index);
    return data ? data->data_type->data_type : NULL;
}

Node *string_type(void) {
    Node *st = find_struct("String");
    if (st == NULL)
        return chars_type();
    Node *type = new_node(new_token(ID, (Token){}));
    type->token->name = st->token->name;
    type->token->is_type = true;
    type->declared = st;
    return type;
}

Node *ret_type_of(Node *fn) {
    return fn->ret_type ? fn->ret_type : new_type(VOID, 0, NULL);
}

bool is_ref_variable(Node *node) {
    if (!includes(node->token->type, VAR, PARAM, 0) || node->data_type == NULL)
        return false;
    return node->data_type->token->type == REF;
}

bool is_ref_binding(Node *node) {
    if (node->token->type != ASSIGN || !is_ref_variable(node->left))
        return false;
    Node *right = node->right;
    bool is_member = includes(right->token->type, DOT, DOUBLE_DOTS, 0);
    Node *call = is_member ? right->right : right;
    bool is_address = right->token->type == REF && !right->token->is_type;
    bool is_ref_call = call->token->type == FN_CALL && lives_in_variable(right);
    bool is_declaration = node->left->declared == NULL;
    bool is_null = right->token->type == ZERO;
    return is_address || is_null || (is_ref_call && is_declaration);
}

char *type_name(Node *type) {
    switch (type->token->type) {
    case INT: {
        int shift = type->token->shift;
        if (shift == 1)
            return "bool";
        if (shift == 21)
            return "unic";
        return format("%c%d", type->token->is_uint ? 'u' : 'i', shift);
    } // clang-format off
    case FLOAT: return format("f%d", type->token->shift);
    case ARRAY: return format("%s[]", type_name(type->data_type));
    case REF: return format("&%s", type_name(type->data_type));
    case VOID: return "void";
    case ID: return type->token->name;
    default: return (char *)to_string(type->token->type);
    } // clang-format on
}

// TODO: check if we really need to call new_node inside type_of 
Node *type_of(Node *node) {
    // assert(node);
    // assert(node->token);
    switch (node->token->type) { // clang-format off
    case INT: case FLOAT: return node;
    case AS:        return node->to_type;
    case LCBRACK:   return node->data_type;
    case LBRACK:    return node->data_type;
    case ZERO:      return node->data_type;
    case DOUBLE_DOTS: return type_of(node->right);
    case DOT: {
        if (node->right->token->type == FN_CALL)
            return type_of(node->right);
        return node->right->declared ? node->right->declared->data_type : NULL;
    }
    case ACCESS: {
        Node *type = type_of(node->left);
        return type && type->token->type == ARRAY ? type->data_type : NULL;
    }
    case CHARS:     return node->data_type ? node->data_type : string_type();
    case VAR: case PARAM: return is_ref_variable(node) ? node->data_type->data_type : node->data_type;
    case REF: {
        Node *type = type_of(node->operand);
        if (type && type->token->type == REF)
            return type;
        return type ? new_type(REF, 0, type) : NULL;
    }
    case FN_CALL: {
        Node *type = node->to_call ? ret_type_of(node->to_call) : NULL;
        return type && type->token->type == REF ? type->data_type : type;
    }
    case NEG: {
        Node *type = type_of(node->operand);
        return is_numeric(type) ? type : NULL;
    }
    case BNOT: {
        Node *type = type_of(node->operand);
        bool is_integer = is_numeric(type) && type->token->type == INT;
        return is_integer ? type : NULL;
    }
    case COPY: {
        Node *first = node->to_call->args->children[0]->data_type;
        Node *found = type_of(node->to_copy);
        if (is_child_ref(found, first))
            return found;
        return first->token->type == REF ? first->data_type : first;
    }
    case SIZEOF: case ARGC:
		return new_type(INT, 32, NULL);
    case TYPEOF: case ALLOC:
		return chars_type();
    case FORMAT:
		return string_type();
    case IS_ZERO: 
		return bool_type();
    case ARGV: 
		return new_type(ARRAY, 0, chars_type());
    case DROP: case MEMCPY: case FREE: 
		return new_type(VOID, 0, NULL);
    case LT: case GT: case EQ: case LE: case GE: case NQ:
    case AND: case OR: case NOT: 
		return bool_type();
    case ID: case SELF:
		return NULL;
	case AND_LIT: case OR_LIT: 
		return bool_type();
    case MUL: case ADD: case SUB: case DIV: case MOD:
    case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
    case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
    case DIV_ASSIGN: case MOD_ASSIGN: case BAND_ASSIGN: case BOR_ASSIGN:
    case BXOR_ASSIGN: case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
    case DECLARE: case ATTR: 
		return type_of(node->left);
    default: {
		eprint("%s:%d handle this case \n%n\n", FILE, LINE, node);
		Token *token = node->token;
		eprint("%s:%d \n", token->filename, token->line);
		// eprint("%s:%d\n", node->token->);
		// raise(SIGSEGV);
		exit(1);
		//  type_of(node->left); // clang-format on
		return NULL;
    }
    }
}

bool same_type(Node *a, Node *b) {
    if (a->token->type != b->token->type)
        return false;
    if (includes(a->token->type, REF, ARRAY, 0))
        return same_type(a->data_type, b->data_type);
    if (a->token->type == ID)
        return strcmp2(a->token->name, b->token->name);
    if (a->token->is_uint != b->token->is_uint)
        return false;
    return a->token->shift == b->token->shift;
}

bool same_params(Node *a, Node *b) {
    if (a->args->children_count != b->args->children_count)
        return false;
    for (int i = 0; i < a->args->children_count; i++) {
        Node *left = a->args->children[i];
        Node *right = b->args->children[i];
        if (left->token->type != right->token->type)
            return false;
        bool has_types = left->data_type && right->data_type;
        if (has_types && !same_type(left->data_type, right->data_type))
            return false;
    }
    return true;
}

bool is_number(Node *node) {
    return node->token->type == INT && node->token->shift != 1;
}

void analyze_block(Node *node) {
    enter_scope(node);
    for (int i = 0; i < node->children_count; i++)
        analyze(node->children[i]);
    exit_scope();
}

void analyze(Node *node) {
    Type type = node->token->type;
    bool is_op = precedence[type] && precedence[type] < precedence[AS];
    if (is_op) {
        analyze(node->left);
        analyze(node->right);
        // print("=================================\n");
        // print("%n", node);
        // print("=================================\n");
        assert_operation(node);
        return;
    }
    switch (node->token->type) { // clang-format off
    case INT: case FLOAT: case VOID: 
    case VAR: case PARAM: case CHARS: 
	case VARIADIC: case USE: case ZERO:
    case MACRO_IF: case MACRO_ERROR: case MODULE: { // clang-format on
        break;
    } // clang-format off
    case FORMAT: case SIZEOF: case TYPEOF: 
	case DROP: case IS_ZERO: case ALLOC: 
	case MEMCPY: case FREE: case ARGC: case ARGV: { // clang-format on
        assert_builtin(node);
        break;
    } // clang-format off
    case ID: case SELF: { // clang-format on
        assert_name(node);
        break;
    }
    case DECLARE: {
        assert_declare(node);
        analyze(node->var->data_type);
        break;
    } // clang-format off
    case SCOPE: case FN_DEC: case PROTO: case STRUCT: { // clang-format on
        enter_scope(node);
        if (node->ret_type)
            analyze(node->ret_type);
        if (node->args) {
            for (int i = 0; i < node->args->children_count; i++) {
                Node *arg = node->args->children[i];
                analyze(arg);
                if (arg->data_type)
                    analyze(arg->data_type);
            }
        }
        if (includes(node->token->type, FN_DEC, PROTO, 0))
            assert_function(node, ura.scopes[ura.scopes_count - 2]);
        else if (node->token->type == STRUCT)
            assert_struct(node, ura.scopes[ura.scopes_count - 2]);

        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    }
    case RETURN: {
        if (node->left)
            analyze(node->left);
        assert_return(node);
        break;
    } // clang-format off
    case MATCH: {
        enter_scope(node);
        analyze(node->left->right);
        assert_match(node);
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    } // clang-format off
    case THROW: {
        analyze(node->left);
        assert_throw(node);
        break;
    } // clang-format off
    case TRY: case CATCH: { // clang-format on
        assert_try(node);
        analyze_block(node);
        if (node->next)
            analyze(node->next);
        break;
    } // clang-format off
    case IF: case ELIF: case ELSE: case LOOP: 
	case CASE: case DEFAULT: { // clang-format on
        assert_cond(node);
        analyze_block(node);
        if (node->next)
            analyze(node->next);
        break;
    } // clang-format off
    case BRK: case CNT: { // clang-format on
        assert_jump(node);
        break;
    }
    case DOUBLE_DOTS: {
        assert_static_call(node);
        break;
    }
    case ENUM: {
        assert_enum(node);
        break;
    }
    case TEMPLATE: {
        assert_template(node);
        break;
    }
    case LINK: {
        assert_link(node);
        break;
    } // clang-format off
    case NEG: case BNOT: { // clang-format on
        analyze(node->operand);
        assert_unary(node);
        break;
    }
    case NOT: {
        analyze(node->operand);
        assert_type(node->operand, bool_type(), NULL);
        break;
    } // clang-format off
    case REF: case ARRAY: { // clang-format on
        if (node->token->is_type && node->data_type)
            analyze(node->data_type);
        else if (!node->token->is_type)
            analyze(node->operand);
        bool ok = !node->token->is_type || node->index == NULL;
        if (node->token->type == REF)
            assert_ref(node);
        else
            assert_array(ok, node);
        break;
    }
    case AS: {
        analyze(node->left);
        if (node->to_type)
            analyze(node->to_type);
        assert_cast(node);
        break;
    }
    case LCBRACK: {
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]->left);
        break;
    }
    case LBRACK: {
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        assert_array_literal(node, NULL);
        break;
    }
    case FOR: {
        analyze(node->right);
        assert_for(node);
        analyze_block(node);
        break;
    }
    case ACCESS: {
        analyze(node->left);
        analyze(node->index);
        assert_access(node);
        break;
    }
    case DOT: {
        analyze(node->left);
        assert_member(node);
        break;
    }
    case FN_CALL: {
        assert_call(node);
        break;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, node->token->type);
        exit(1);
        break;
    }
    }
}

// CODE GEN
void codegen_drop(Ucg *place, Node *type) {
    Node *drop = find_drop(type);
    if (drop) {
        Ucg *self = ucg_ref(place);
        ucg_fn_call(drop->token->ucg, &self, 1);
    }
    Node *st = type->declared;
    for (int i = st->children_count - 1; i >= 0; i--) {
        Node *attr = st->children[i];
        if (attr->token->type != DECLARE)
            continue;
        if (!needs_drop(attr->var->data_type))
            continue;
        int index = 0;
        find_attr(st, attr->var->token->name, &index);
        codegen_drop(ucg_get_attr(place, index), attr->var->data_type);
    }
}

void codegen_clean(Ucg *place, Node *type) {
    if (!needs_drop(type))
        return;
    ucg_clean(place);
    codegen_drop(place, type);
    ucg_clean_end();
}

void codegen_global_drops(Node *scope) {
    for (int i = 0; i < scope->children_count; i++) {
        Node *child = scope->children[i];
        bool is_declare = child->token->type == DECLARE;
        if (is_declare && !is_unused_global(child->var->token))
            codegen_clean(child->var->token->ucg, child->var->data_type);
    }
}

void codegen_drop_temporaries(void) {
    Node *scope = ura.scope;
    for (int i = scope->temporaries_count - 1; i >= 0; i--)
        codegen_drop(scope->temporaries[i], scope->temporary_types[i]);
    scope->temporaries_count = 0;
    scope->temporary_types_count = 0;
}

void codegen_statement(Node *node) {
    bool is_method = node->token->type == DOT && node->right->token->type == FN_CALL;
    bool is_call = includes(node->token->type, FN_CALL, DOUBLE_DOTS, 0) || is_method;
    if (is_call && needs_drop(type_of(node)))
        codegen_in_variable(node);
    else
        codegen(node);
    codegen_drop_temporaries();
}

void codegen_branch(Node *node, Ucg *stmt) {
    node->token->ucg = stmt;
    Ucg *cond = NULL;
    if (node->cond) {
        codegen(node->cond);
        cond = node->cond->token->ucg;
        codegen_drop_temporaries();
    }
    ucg_then(stmt, cond);
    enter_scope(node);
    codegen_block(node, 0);
    exit_scope();
}

void codegen_block(Node *node, int start) {
    codegen_structs(node);
    codegen_protos(node);
    for (int i = start; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == PROTO)
            continue;
        if (child->token->type == STRUCT) {
            codegen(child);
            continue;
        }
        codegen_statement(child);
        if (includes(child->token->type, RETURN, BRK, CNT, THROW, 0))
            break;
    }
}

void codegen_try(Node *node) {
    Ucg *try_stmt = ucg_try();
    enter_scope(node);
    codegen_block(node, 0);
    exit_scope();
    Ucg *prev = try_stmt;
    Ucg *first = NULL;
    for (Node *branch = node->next; branch; branch = branch->next) {
        Node *type = branch->var->data_type;
        codegen(type);
        Ucg *catch_stmt = ucg_catch(prev, type->token->ucg);
        first = first ? first : catch_stmt;
        enter_scope(branch);
        codegen(branch->children[0]);
        ucg_assign(branch->var->token->ucg, ucg_error(type->token->ucg));
        codegen_block(branch, 1);
        exit_scope();
        prev = catch_stmt;
    }
    ucg_end_catch(first);
}

void codegen_uncaught(Ucg *try_stmt) {
    Node *hook = find_function("uncaught");
    bool has_one_param = hook && hook->args && hook->args->children_count == 1;
    Node *param = has_one_param ? hook->args->children[0]->data_type : NULL;
    Node *type = param && param->token->type == REF ? param->data_type : param;
    if (type) {
        Ucg *catch_stmt = ucg_catch(try_stmt, type->token->ucg);
        Ucg *ref = ucg_ref(ucg_error(type->token->ucg));
        Ucg *code = ucg_call(hook->token->ucg, 1, ref);
        ucg_ret(code);
        ucg_end(catch_stmt);
    } else
        ucg_end_try(try_stmt);
    Ucg *uncaught = ucg_if();
    ucg_then(uncaught, ucg_thrown());
    ucg_ret(ucg_i32_value(1));
    ucg_end(uncaught);
}

void codegen_proto(Node *node, char *name) {
    Ucg *ret_type = NULL;
    if (node->ret_type) {
        codegen(node->ret_type);
        ret_type = node->ret_type->token->ucg;
    } else
        ret_type = ucg_void_type();

    Node **children = node->args->children;
    int count = node->args->children_count;
    Ucg **args = NULL;
    if (count != 0) {
        args = arena_alloc(count, sizeof(Ucg *));
        for (int i = 0; i < count; i++) {
            Node *param = children[i];
            if (param->token->type == VARIADIC) {
                args[i] = ucg_varg_type();
                continue;
            }
            codegen(param->data_type);
            args[i] = ucg_param(param->token->name, param->data_type->token->ucg);
            param->token->ucg = args[i];
        }
    }
    node->token->ucg = ucg_proto(name, ret_type, args, count);
}

void codegen_structs(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == STRUCT)
            child->token->ucg = ucg_struct(child->token->name);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == STRUCT)
            codegen_structs(child);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type != STRUCT)
            continue;
        Ucg **attrs = arena_alloc(child->children_count + 1, sizeof(Ucg *));
        int attrs_count = 0;
        for (int j = 0; j < child->children_count; j++) {
            Node *attr = child->children[j];
            if (attr->token->type != DECLARE)
                continue;
            codegen(attr->var->data_type);
            Ucg *type = attr->var->data_type->token->ucg;
            attrs[attrs_count++] = ucg_attr(attr->var->token->name, type);
        }
        ucg_attrs(child->token->ucg, attrs, attrs_count);
    }
}

Ucg *codegen_call_result(Node *fn, Ucg *call) {
    Node *type = ret_type_of(fn);
    if (type->token->type != REF)
        return call;
    Ucg *ref = ucg_var("ref", type->token->ucg);
    ucg_assign(ref, call);
    return ucg_point_to(ref);
}

bool is_struct_value(Node *type) {
    bool is_struct = type && type->token->type == ID && type->declared;
    return is_struct && !same_type(type, string_type());
}

Ucg **codegen_args(Node *call) {
    Ucg **args = arena_alloc(call->children_count + 1, sizeof(Ucg *));
    for (int i = 0; i < call->children_count; i++) {
        codegen(call->children[i]);
        args[i] = call->children[i]->token->ucg;
    }
    return args;
}

Node *codegen_type_of(Node *arg) {
    Node *type = arg->token->is_type ? arg : type_of(arg);
    codegen(type);
    return type;
}

void codegen_builtin(Node *node) {
    Ucg **args = codegen_args(node);
    Node *arg = node->children_count ? node->children[0] : NULL;
    Ucg *ucg = NULL;
    switch (node->token->type) { // clang-format off
    case SIZEOF: ucg = ucg_sizeof(codegen_type_of(arg)->token->ucg); break;
    case TYPEOF: ucg = ucg_typeof(codegen_type_of(arg)->token->ucg); break;
    case ALLOC:  ucg = ucg_alloc(ucg_i8_type(), args[0]); break;
    case MEMCPY: ucg = ucg_copy(args[0], args[1], args[2]); break;
    case FREE:   ucg = ucg_free(args[0]); break;
    case ARGC:   ucg = ucg_argc(); break;
    case ARGV:   ucg = ucg_argv(); break; // clang-format on
    case IS_ZERO: {
        bool is_ref = is_ref_variable(arg);
        Node *type = is_ref ? arg->data_type : type_of(arg);
        Ucg *value = is_ref ? arg->declared->token->ucg : args[0];
        codegen(type);
        ucg = ucg_op(EQ, value, ucg_null(type->token->ucg));
        break;
    }
    case DROP: {
        Node *type = codegen_type_of(arg);
        if (needs_drop(type))
            codegen_drop(args[0], type);
        ucg = ucg_assign(args[0], ucg_null(type->token->ucg));
        break;
    }
    default:
        break;
    }
    node->token->ucg = ucg;
}

Ucg *codegen_text_part(Node *node) {
    Node *type = type_of(node);
    bool is_string = type && type->declared && same_type(type, string_type());
    if (is_string) {
        Node *chars = find_method_named(type->declared, "chars");
        Ucg *self = ucg_ref(codegen_in_variable(node));
        return ucg_call(chars->token->ucg, 1, self);
    }
    codegen(node);
    return node->token->ucg;
}

void codegen_format(Node *node) {
    Ucg **args = arena_alloc(node->children_count + 1, sizeof(Ucg *));
    for (int i = 0; i < node->children_count; i++)
        args[i] = codegen_text_part(node->children[i]);
    Node *type = string_type();
    codegen(type);
    Ucg *size = ucg_var("format.size", ucg_i32_type());
    Ucg *text = ucg_format(size, args, node->children_count);
    Ucg *cap = ucg_op(ADD, size, ucg_i32_value(1));
    Ucg *ucg = ucg_struct_value(type->token->ucg, (Ucg *[]){ text, size, cap }, 3);
    node->token->ucg = ucg;
}

Ucg *codegen_in_variable(Node *node) {
    codegen(node);
    Ucg *ucg = node->token->ucg;
    if (is_locator(ucg))
        return ucg;
    Node *type = type_of(node);
    codegen(type);
    Ucg *tmp = ucg_var("tmp", type->token->ucg);
    ucg_assign(tmp, ucg);
    if (needs_drop(type)) {
        append(ura.scope->temporaries, tmp);
        append(ura.scope->temporary_types, type);
    }
    return tmp;
}

void codegen_method_protos(Node *st) {
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type == STRUCT)
            codegen_method_protos(child);
        if (child->token->type != FN_DEC)
            continue;
        codegen_proto(child, format("%s.%s", st->token->name, child->token->name));
    }
}

void codegen_protos(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        bool is_fn = child->token->type == FN_DEC && child->args;
        bool is_operator = is_fn && child->args->symbol;
        Node *left = is_operator ? child->args->children[0]->data_type : NULL;
        if (left && left->token->type == REF)
            left = left->data_type;
        char *name = child->token->name;
        bool is_proto = child->token->type == PROTO;
        char *bare = is_proto ? strrchr(name, ':') : NULL;
        if (bare)
            name = bare + 1;
        if (left)
            name = format("%s.%s", type_name(left), name);
        if (includes(child->token->type, FN_DEC, PROTO, 0))
            codegen_proto(child, name);
        if (child->token->type == STRUCT)
            codegen_method_protos(child);
    }
}

void codegen_statements(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(child->token->type, FN_DEC, PROTO, DECLARE, STRUCT, LINK, 0))
            continue;
        bool is_assign = child->token->type == ASSIGN;
        bool is_global = is_assign && child->left->token->type == VAR;
        if (is_global && is_unused_global(child->left->token))
            continue;
        codegen_statement(child);
    }
}

void codegen(Node *node) {
    Type type = node->token->type;
    bool is_op = precedence[type] && precedence[type] < precedence[AS];
    if (is_op) {
        codegen_operation(node);
        return;
    }
    Ucg *ucg = NULL;
    switch (node->token->type) {
    case VAR:
    case PARAM: {
        if (node->declared)
            node->token->ucg = node->declared->token->ucg;
        if (node->declared && is_ref_variable(node))
            node->token->ucg = ucg_point_to(node->token->ucg);
        break;
    }

    case INT: {
        if (node->token->is_type && node->token->is_uint)
            ucg = ucg_uint_type(node->token->shift);
        else if (node->token->is_type)
            ucg = ucg_int_type(node->token->shift);
        else
            ucg = ucg_int_value(node->token->shift, node->token->num.value);

        node->token->ucg = ucg;
        break;
    }
    case VOID: {
        node->token->ucg = ucg_void_type();
        break;
    }
    case FLOAT: {
        if (node->token->is_type)
            ucg = ucg_float_type(node->token->shift);
        else
            ucg = ucg_float_value(node->token->shift, node->token->num.real);
        node->token->ucg = ucg;
        break;
    }
    case REF:
    case ARRAY: {
        if (node->token->is_type) {
            codegen(node->data_type);
            node->token->ucg = ucg_ref_type(node->data_type->token->ucg);
        } else {
            Ucg *place = codegen_in_variable(node->operand);
            Node *type = type_of(node->operand);
            bool is_ref = type && type->token->type == REF;
            node->token->ucg = is_ref ? place : ucg_ref(place);
        }
        break;
    }
    case COPY: {
        Node *type = type_of(node);
        codegen(type);
        Ucg *copy = ucg_var("copy", type->token->ucg);
        Ucg *args[] = {
            ucg_ref(copy),
            ucg_ref(codegen_in_variable(node->to_copy)),
        };
        ucg_fn_call(node->to_call->token->ucg, args, 2);
        node->token->ucg = copy;
        break;
    }
    case CHARS: {
        char *text = node->token->chars.value;
        Node *type = type_of(node);
        node->token->ucg = ucg_string_value(text);
        if (type->token->type == ARRAY)
            break;
        codegen(type);
        // TODO: why this ?????
        node->token->ucg = ucg_struct_value(
            type->token->ucg,
            (Ucg *[]){
                node->token->ucg,
                ucg_i32_value(strlen(text)),
                ucg_i32_value(0),
            },
            3
        );
        break;
    }
    case DECLARE: {
        codegen(node->var->data_type); // data type

        char *name = node->var->token->name;
        Ucg *type = node->var->data_type->token->ucg;
        if (ura.scope->token->type == SCOPE)
            ucg = ucg_global(name, type);
        else {
            ucg = ucg_var(name, type);
            codegen_clean(ucg, node->var->data_type);
        }
        node->var->token->ucg = ucg;
        break;
    }
    case SCOPE: {
        if (ura.throws)
            ucg_throws();
        enter_scope(node);
        codegen_structs(node);
        codegen_protos(node);
        bool has_main = false;
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            bool is_declare = child->token->type == DECLARE;
            if (is_declare && !is_unused_global(child->var->token))
                codegen(child);
            if (child->token->type == LINK)
                ucg_link(child->token->chars.value);
            if (child->token->type == FN_DEC && strcmp2(child->token->name, "main"))
                has_main = true;
        }
        if (!has_main) {
            Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
            codegen_statements(node);
            codegen_global_drops(node);
            ucg_end(fn_main);
        }
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (includes(child->token->type, FN_DEC, STRUCT, 0))
                codegen(child);
        }
        exit_scope();
        break;
    }
    case FN_DEC: {
        Node *parent = ura.scope;
        enter_scope(node);
        ucg_entry(node->token->ucg);
        for (int i = 0; i < node->args->children_count; i++) {
            Node *param = node->args->children[i];
            if (param->token->type == PARAM)
                codegen_clean(param->token->ucg, param->data_type);
        }
        bool is_main =
            parent->token->type == SCOPE && strcmp2(node->token->name, "main");
        Ucg *try_stmt = is_main && ura.throws ? ucg_try() : NULL;
        if (is_main) {
            codegen_statements(parent);
            codegen_global_drops(parent);
        }
        codegen_block(node, 0);
        if (try_stmt)
            codegen_uncaught(try_stmt);
        ucg_end(node->token->ucg);
        exit_scope();
        break;
    }
    case STRUCT: {
        enter_scope(node);
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (includes(child->token->type, FN_DEC, STRUCT, 0))
                codegen(child);
        }
        exit_scope();
        break;
    }
    case FN_CALL: {
        Ucg **args = codegen_args(node);
        Ucg *call = ucg_fn_call(node->to_call->token->ucg, args, node->children_count);
        node->token->ucg = codegen_call_result(node->to_call, call);
        break;
    }
    case FORMAT: {
        codegen_format(node);
        break;
    } // clang-format off
    case SIZEOF: case TYPEOF: case DROP: case IS_ZERO:
    case ALLOC: case MEMCPY: case FREE: case ARGC: case ARGV: { // clang-format on
        codegen_builtin(node);
        break;
    }
    case RETURN: {
        if (node->left) {
            codegen(node->left);
            bool is_local = is_local_variable(node->left);
            Ucg *value = node->left->token->ucg;
            if (!is_local)
                value = ucg_value(value);
            codegen_drop_temporaries();
            ucg = ucg_ret(value);
        } else
            ucg = ucg_return();
        node->token->ucg = ucg;
        break;
    }
    case IF: {
        Ucg *if_stmt = ucg_if();
        codegen_branch(node, if_stmt);
        for (Node *branch = node->next; branch; branch = branch->next) {
            Ucg *stmt = NULL;
            if (branch->token->type == ELIF)
                stmt = ucg_elif(if_stmt);
            else
                stmt = ucg_else(if_stmt);
            codegen_branch(branch, stmt);
        }
        ucg_end(if_stmt);
        break;
    }
    case LOOP: {
        Ucg *loop = ucg_loop();
        codegen_branch(node, loop);
        ucg_end(loop);
        break;
    }
    case FOR: {
        Node *type = type_of(node->right);
        Node *in = find_operator(IN, type, NULL, 0);
        Ucg *iter = codegen_in_variable(node->right);
        Ucg *counter = ucg_var("i", ucg_i32_type());
        ucg_assign(counter, ucg_i32_value(0));
        enter_scope(node);
        codegen(node->children[0]);
        Ucg *loop = ucg_loop();
        node->token->ucg = loop;
        Ucg *args[] = {
            ucg_ref(iter),
            ucg_ref(counter),
            ucg_ref(node->var->token->ucg),
        };
        ucg_then(loop, ucg_fn_call(in->token->ucg, args, 3));
        codegen_block(node, 1);
        exit_scope();
        ucg_end(loop);
        break;
    }
    case MATCH: {
        enter_scope(node);
        codegen(node->children[0]);
        codegen_statement(node->left);
        Ucg *if_stmt = NULL;
        for (int i = 1; i < node->children_count; i++) {
            Node *branch = node->children[i];
            Ucg *stmt = NULL;
            if (if_stmt == NULL)
                stmt = if_stmt = node->token->ucg = ucg_if();
            else
                stmt = branch->cond ? ucg_elif(if_stmt) : ucg_else(if_stmt);
            codegen_branch(branch, stmt);
        }
        if (if_stmt)
            ucg_end(if_stmt);
        exit_scope();
        break;
    }
    case TRY: {
        codegen_try(node);
        break;
    }
    case THROW: {
        codegen(node->left);
        Ucg *value = ucg_value(node->left->token->ucg);
        codegen_drop_temporaries();
        node->token->ucg = ucg_throw(value);
        break;
    }
    case BRK: {
        ucg_brk(node->loop->token->ucg);
        break;
    } // clang-format off
    case TEMPLATE: case USE: case MACRO_IF: 
    case MACRO_ERROR: case ENUM: case MODULE: { // clang-format on
        break;
    }
    case ID: {
        node->token->ucg = node->declared->token->ucg;
        break;
    }
    case ZERO: {
        codegen(node->data_type);
        node->token->ucg = ucg_null(node->data_type->token->ucg);
        break;
    }
    case LBRACK: {
        Node *type = node->data_type;
        Node *elem = element_type_of(type);
        codegen(elem);
        Ucg **items = arena_alloc(node->children_count + 1, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            codegen(node->children[i]);
            items[i] = node->children[i]->token->ucg;
        }
        Ucg *data = ucg_array_from(elem->token->ucg, items, node->children_count);
        if (type->token->type == ARRAY) {
            node->token->ucg = data;
            break;
        }
        codegen(type);
        Ucg *fields[] = {
            data,
            ucg_i32_value(node->children_count),
            ucg_i32_value(0),
        };
        Ucg *array = ucg_var("array", type->token->ucg);
        ucg_assign(array, ucg_struct_value(type->token->ucg, fields, 3));
        Node *own = find_method_named(type->declared, "own");
        ucg_call(own->token->ucg, 1, ucg_ref(array));
        node->token->ucg = array;
        break;
    }
    case LCBRACK: {
        codegen(node->data_type);
        Node *st = node->data_type->declared;
        int values_count = st->children_count + 1;
        Ucg **values = arena_alloc(values_count, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            Node *attr = node->children[i];
            codegen(attr->left);
            values[attr->token->attr.index] = attr->left->token->ucg;
        }
        Ucg *ucg = ucg_struct_value(node->data_type->token->ucg, values, values_count);
        node->token->ucg = ucg;
        break;
    }
    case ACCESS: {
        codegen(node->left);
        codegen(node->index);
        node->token->ucg = ucg_access(node->left->token->ucg, node->index->token->ucg);
        break;
    }
    case DOT: {
        Ucg *base = codegen_in_variable(node->left);
        Node *left_type = type_of(node->left);
        if (left_type && left_type->token->type == REF)
            base = ucg_point_to(base);
        if (node->right->token->type == FN_CALL) {
            Node *call = node->right;
            Ucg **args = arena_alloc(call->children_count + 2, sizeof(Ucg *));
            args[0] = ucg_ref(base);
            for (int i = 0; i < call->children_count; i++) {
                codegen(call->children[i]);
                args[i + 1] = call->children[i]->token->ucg;
            }
            Ucg *method = call->to_call->token->ucg;
            node->token->ucg = codegen_call_result(
                call->to_call, ucg_fn_call(method, args, call->children_count + 1)
            );
            break;
        }
        int index = node->right->token->attr.index;
        node->token->ucg = ucg_get_attr(base, index);
        break;
    }
    case DOUBLE_DOTS: {
        Node *call = node->right;
        Ucg **args = codegen_args(call);
        node->token->ucg = codegen_call_result(
            call->to_call,
            ucg_fn_call(call->to_call->token->ucg, args, call->children_count)
        );
        break;
    }
    case AS: {
        codegen(node->left);
        codegen(node->to_type);
        node->token->ucg = ucg_cast(node->left->token->ucg, node->to_type->token->ucg);
        break;
    }
    case NEG: {
        codegen(node->operand);
        node->token->ucg = ucg_neg(node->operand->token->ucg);
        break;
    }
    case BNOT: {
        codegen(node->operand);
        Node *type = type_of(node->operand);
        Ucg *ones = ucg_int_value(type->token->shift, -1);
        node->token->ucg = ucg_op(BXOR, node->operand->token->ucg, ones);
        break;
    }
    case NOT: {
        codegen(node->operand);
        node->token->ucg = ucg_not(node->operand->token->ucg);
        break;
    }
    case CNT: {
        ucg_cnt(node->loop->token->ucg);
        break;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, node->token->type);
        exit(1);
        break;
    }
    }
}

void codegen_operation(Node *node) {
    if (is_ref_binding(node)) {
        Node *slot = node->left->declared ? node->left->declared : node->left;
        codegen(node->right);
        Ucg *address = node->right->token->ucg;
        if (!includes(node->right->token->type, REF, ZERO, 0))
            address = ucg_ref(address);
        node->token->ucg = ucg_assign(slot->token->ucg, address);
        return;
    }
    codegen(node->left);
    Node *type = type_of(node->left);
    bool is_var = node->left->token->type == VAR;
    bool is_declaration = is_var && node->left->declared == NULL;
    bool is_literal = node->right->token->type == CHARS;
    if (is_declaration && is_literal && is_chars(type))
        node->right->token->ucg = ucg_string_array(node->right->token->chars.value);
    else
        codegen(node->right);
    if (node->token->type == ASSIGN && !is_declaration && needs_drop(type)) {
        Ucg *value = ucg_value(node->right->token->ucg);
        codegen_drop(node->left->token->ucg, type);
        node->token->ucg = ucg_assign(node->left->token->ucg, value);
        return;
    }
    Ucg *left = node->left->token->ucg;
    Ucg *right = node->right->token->ucg;
    bool is_null_compare = node->right->token->type == ZERO;
    if (is_null_compare && is_ref_variable(node->left))
        left = node->left->declared->token->ucg;
    node->token->ucg = ucg_op(node->token->type, left, right);
}


int main(int ac, char **av) {
    atexit(arena_free);
    arena_reserve(1000); // TODO: this value mght need to change
    ucg_bin_path("build/exe.out");
    parse_args(ac, av);
    int code = 0;
    ura.tokens_count = 0;
    ura.exe_pos = 0;

#if TOKENIZE
    print(GREEN("======================================\n"));
    print(GREEN("[TOKENIZE]:\n"));
    new_token(SCOPE, (Token){ .name = "ura-scope" });
    for (int i = 0; i < ura.files_count; i++) {
        File *file = ura.files[i];
        if (file->opened)
            continue;
        file->start = end_of_sources();
        open_file(file);
        arena_reserve(file->len * 192);
        tokenize(file);
    }
    new_token(LAST, (Token){ .space = -TAB });

    ucg_check_errors();

    print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tokens_count; i++)
        print(CYAN("token ") "%k\n", ura.tokens[i]);

#    if AST
    print(GREEN("======================================\n"));
    print(GREEN("[AST]:\n"));
    ura.tree = parse_expr(0);
    ucg_check_errors();
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#        if ANALYZE
    print(GREEN("======================================\n"));
    print(GREEN("[ANALYZE]:\n"));
    analyze(ura.tree);
    ucg_check_errors();
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#            if ASM
    print(GREEN("======================================\n"));
    print(GREEN("[CODE GEN]:\n"));
    ucg_start("ura");
    codegen(ura.tree);
    print(GREEN("======================================\n"));
    print(GREEN("[URA IR]:\n"));
    // print_ucgs();
    ucg_compile();
    ucg_gen();
    ucg_finish();
#            endif // ASM
#        endif     // ANALYZE
#    endif         // AST
#endif             // TOKENIZE
    // TODO: I think it should be removed
    print("did allocate %zu\n", arena_size());
    return code;
}

// UTILS / FORMATING / PUTING
int _print_type(ioFile fp, Node *node) {
    // clang-format off
    if (node == NULL) return fprintf(fp, "void");
    switch(node->token->type) {
    case REF:		return fprintf(fp, "&") + _print_type(fp, node->left);
    case STRUCT:	return fprintf(fp, "%s", node->token->name);
    case ARRAY:		return _print_type(fp, node->left) + fprintf(fp, "[]");
    default:		return fprintf(fp, "%s", to_string(node->token->type));
    }
    // clang-format on
};

int _print_char(ioFile fp, char c) {
    unsigned char u = (unsigned char)c;
    if (iscntrl(u) && escaped[u])
        return fprintf(fp, "%s", escaped[u]);
    return fprintf(fp, "%c", c);
}

// clang-format off
int _print_token(ioFile fp, Token *token) {
    if (token == NULL) return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name != NULL) r += fprintf(fp, " name (%s)", token->name);
    if (token->name == NULL && !token->is_type) { 
        switch(token->type) {
        // case I8:    r += fprintf(fp, " value (%c)", token->i8.value); break;
        case INT:   r += fprintf(fp, " value (%lld)", token->num.value); break;
        case CHARS: {
            #if 1
            r += fprintf(fp, " value (");
            char *value = token->chars.value;
            for(int i = 0; value && value[i]; i++)
                r += _print_char(fp, value[i]);
            r += fprintf(fp, ")"); 
            #else
            r += fprintf(fp, " value (%s)", token->chars.value);
            #endif
            break;
        }
        // case BOOL: {
        //     char *b1 = token->b1.value ? "True" : "False";
        //     r += fprintf(fp, " value (%s)", b1);
        //     break;
        // }
        default: break;
        }
    } 
    r += fprintf(fp, " space (%d)", token->space);
    return 0;
}

void _print_node_helper(NodePrint *elems, Node *node, int depth) {
    if (node == NULL)
        return;

    append(elems->nodes, node);
    append(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, FN_CALL, TEMPLATE, 0)) {
        Node *left = node->left;
        bool is_struct = left && left->token->type == STRUCT;
        if (is_struct) {
            append(elems->nodes, left);
            append(elems->depths, depth + 3);
            // print(">>>> %n\n", left);
            // exit(1);
            // if (left->left) {
            //     append(elems->nodes, left->left);
            //     append(elems->depths, depth + 3);
            // }
        } else
            _print_node_helper(elems, left, depth + 3);
    }
    if (!includes(node->token->type, ID, VAR, PARAM, 0))
        _print_node_helper(elems, node->right, depth + 3);
    for (size_t i = 0; i < node->children_count; i++)
        _print_node_helper(elems, node->children[i], depth + 3);
}

bool is_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++) {
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    }
    return 0;
}

int _print_node(ioFile fp, Node *node) {
    NodePrint elems = {};
    int r = 0;
    _print_node_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            r += fprintf(fp, "%s", is_open(&elems, i, level) ? "│ " : " ");
        if (depth)
            r += fprintf(fp, "%s", is_open(&elems, i, depth) ? "├─" : "└─");
        r += _print_token(fp, elems.nodes[i]->token) + fprintf(fp, "\n");
    }
    return 0;
}

int _fprint(ioFile fp, const char *fmt, va_list ap) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }

        char spec[128] = "%";
        size_t j = 1;

		i++; // skip %
        while(fmt[i] && strchr("-+ #0123456789.*hlz", fmt[i])) {
            if (fmt[i] == '*') {
                int n = va_arg(ap, int);
                j += snprintf(spec + j, sizeof(spec) - j, "%d", n);
            } else if (j < sizeof(spec) - 2) {
                spec[j++] = fmt[i];
            }
			i++;
        }

        char conversion = fmt[i];

        if (!conversion)
            break;

        spec[j++] = conversion;
        spec[j] = '\0';

        bool is_long_long = strstr(spec, "ll") != NULL;
        bool is_long = strchr(spec, 'l') != NULL;
        bool is_size = strchr(spec, 'z') != NULL;
        switch (conversion) { // clang-format off
        case 't': r += fprintf(fp, "%s", to_string(va_arg(ap, Type))); break;
        case 'k': r += _print_token(fp, va_arg(ap, Token *)); break;
        case 'n': r += _print_node(fp, va_arg(ap, Node *)); break;
        case 'N': r += _print_type(fp, va_arg(ap, Node *)); break;
        case 'i': r += fprintf(fp, "%*s", va_arg(ap, int), ""); break;
        case '%': r += fprintf(fp, "%%"); break;
        case 's': r += fprintf(fp, spec, va_arg(ap, char *)); break;
        case 'p': r += fprintf(fp, spec, va_arg(ap, void *)); break;
        case 'c': r += fprintf(fp, spec, va_arg(ap, int)); break;
        case 'f': r += fprintf(fp, spec, va_arg(ap, double)); break;
        default:  r += fprintf(fp, "%s", spec); break;
        case 'd': case 'u': case 'x': // clang-format on
            if (is_long_long)
                r += fprintf(fp, spec, va_arg(ap, long long));
            else if (is_size)
                r += fprintf(fp, spec, va_arg(ap, size_t));
            else if (is_long)
                r += fprintf(fp, spec, va_arg(ap, long));
            else
                r += fprintf(fp, spec, va_arg(ap, int));
            break;
        }
    }
    return r;
}

__attribute__((sentinel)) bool includes(Type to_find, ...) {
    va_list ap;
    va_start(ap, to_find);
    Type curr = va_arg(ap, Type);
    while (curr) {
        if (curr == to_find)
            return true;
        curr = va_arg(ap, Type);
    }
    return false;
}

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

void parse_compile_args(int ac, char **av) {
    char *project = find_folder_with(getcwd2(), "ura.conf");
    if (project)
        load_project(load_config(project));
    bool has_exe = false;
    bool no_link = false;
    for (int i = 2; i < ac && !ucg_has_errors(); i++) {
        char *arg = av[i];
        char *next = i + 1 < ac ? av[i + 1] : NULL;
        bool has_path = next && next[0] != '-' && !ends_with(next, ".ura");
        if (strcmp2(arg, "-o")) {
            if (next == NULL) {
                eprint("expected an executable name after -o\n");
                help("ura compile main.ura -o build/main\n");
                exit(1);
            }
            ucg_bin_path(av[++i]);
            has_exe = true;
        } else if (strcmp2(arg, "-ll")) {
            ucg_path(has_path ? av[++i] : "build/out.ll");
        } else if (strcmp2(arg, "-S")) {
            ucg_asm_path(has_path ? av[++i] : "build/out.s");
            no_link = true;
        } else if (strcmp2(arg, "-c")) {
            ucg_obj_path(has_path ? av[++i] : "build/out.o");
            no_link = true;
        } else if (!ends_with(arg, ".ura")) {
            eprint("unknown flag or file '%s'\n", arg);
            help("see the flags with: ura -help\n");
            exit(1);
        } else if (ura.files_count > 0) {
            eprint("ura compile takes one file, '%s' is a second one\n", arg);
            help("reach the other files with use \"./<file>\" in the main file\n");
            exit(1);
        } else {
            append(ura.files, new_file(arg));
        }
    }
    if (ura.files_count == 0) {
        eprint("expected a .ura file to compile\n");
        help("ura compile main.ura\n");
        exit(1);
    }
    if (no_link && !has_exe)
        ucg_bin_path(NULL);
}

void parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        print_help();
        exit(1);
    }
    // help
    if (strcmp2(av[1], "-help") || strcmp2(av[1], "-h")) {
        print_help();
        exit(0);
    }
    // see version
    if (strcmp2(av[1], "-version") || strcmp2(av[1], "-v")) {
        help("ura %s (LLVM %s)\n", URA_VERSION, LLVM_VERSION_STRING);
        exit(0);
    }
    // init a project
    if (strcmp2(av[1], "init")) {
        if (ac < 3) {
            eprint("expected a project name:\n");
            help("ura init <name>\n");
            exit(1);
        }
        init_project(av[2]);
        exit(0);
    }
    if (strcmp2(av[1], "compile")) {
        parse_compile_args(ac, av);
        return;
    }
    // with ura.conf
    if (strcmp2(av[1], "deps")) {
        // TODO: it will be nice if we remove deps add
        // we use only insstall since install already add neeed ones
        // install also should be able to take more than one arguments
        // how we do like uv
        // ura install <arg> (install arg dependecy)
        // ura install: install everyhing
        char *command = ac > 2 ? av[2] : "";
        char *name = ac > 3 ? av[3] : NULL;
        if (strcmp2(command, "install") && ac == 3)
            install_project(load_config(find_project()), NULL, false, 0);
        else if (strcmp2(command, "add") && (ac == 4 || ac == 5))
            add_dependency(find_project(), name, ac == 5 ? av[4] : NULL);
        else if (strcmp2(command, "update") && ac <= 4)
            install_project(load_config(find_project()), name, true, 0);
        else if (strcmp2(command, "list") && ac == 3)
            list_project(load_config(find_project()), "", 0);
        else if (!strcmp2(command, "remove") || ac < 4) {
            eprint("expected: ura deps install | add | update | remove | list\n");
            help("see them with: ura -help\n");
            exit(1);
        }
        for (int i = 3; strcmp2(command, "remove") && i < ac; i++)
            remove_dependency(find_project(), av[i]);
        exit(0);
    }
    bool is_compile = av[1][0] == '-' || ends_with(av[1], ".ura");
    if (is_compile) {
        eprint("'%s' is not a command\n", av[1]);
        help("compile a file with: ura compile %s\n", av[1]);
        exit(1);
    }
    run_script(av[1]);
}

char *parse_escaped(char *input, int s, int e) {
    char *res = arena_alloc(e - s + 1, sizeof(char));
    int r = 0;
    while (s < e) {
        if (input[s] == '\n') {
            while (s < e && isspace(input[s]))
                s++;
            bool is_word_end = r > 0 && !isspace((unsigned char)res[r - 1]);
            if (is_word_end && s < e)
                res[r++] = ' ';
            continue;
        }
        int c = -1;
        if (s < e - 1 && input[s] == '\\') {
            char *str = escaped[(unsigned char)input[s + 1]];
            if (str != NULL) {
                c = str[0];
                s += 2;
            }
        }
        if (c == -1)
            c = (unsigned char)input[s++];
        res[r++] = c;
    }
    return res;
}

// UCG
bool run_test(int p) {
    ucg_clear();
    switch (p) {
    case 0: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            // Ucg *a = ucg_var("a", ucg_i32_type());
            // Ucg *b = ucg_var("b", ucg_i32_type());

            // ucg_assign(a, ucg_i32_value(10));

            // ucg_assign(b, ucg_op(MUL, a, ucg_i32_value(2)));
            // ucg_assign(a, ucg_op(ADD, a, b));

            // ucg_op(ADD_ASSIGN, a, ucg_i32_value(5));
            // ucg_op(SUB_ASSIGN, a, ucg_i32_value(3));
            // ucg_op(MUL_ASSIGN, a, ucg_i32_value(2));
            // ucg_op(DIV_ASSIGN, a, ucg_i32_value(4));
            // ucg_op(MOD_ASSIGN, a, ucg_i32_value(5));

            // ucg_ret(ucg_op(ADD, a, b));
        }
        ucg_end(fn_main);
        break;
    }
    case 1: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *x = ucg_var("a", ucg_i32_type());
            Ucg *b = ucg_var("b", ucg_bool_type());
            Ucg *c = ucg_var("c", ucg_char_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    OR,
                    ucg_op(EQ, b, ucg_bool_value(1)),
                    ucg_op(NQ, c, ucg_char_value(0))
                )
            );
            {
                ucg_ret(ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(x);
        }
        ucg_end(fn_main);
        break;
    }
    case 2: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *flag = ucg_var("flag", ucg_bool_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, flag, ucg_bool_value(1)));
            {
                ucg_ret(ucg_i32_value(1));
            }
            Ucg *else_stmt = ucg_else(if_stmt);
            ucg_then(else_stmt, NULL);
            {
                ucg_ret(ucg_i32_value(2));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 3: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            Ucg *big = ucg_var("big", ucg_bool_type());

            ucg_assign(big, ucg_op(LT, a, ucg_i32_value(3)));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_assign(big, ucg_op(LT, a, ucg_i32_value(9)));

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_assign(big, ucg_bool_value(0));

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(100));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, big, ucg_bool_value(0)));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 4: {
        Ucg *is_even = ucg_fn(
            "is_even",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *n = ucg_get_param(is_even, 0);
            ucg_ret(ucg_op(EQ, ucg_op(MOD, n, ucg_i32_value(2)), ucg_i32_value(0)));
        }
        ucg_end(is_even);

        Ucg *both = ucg_fn(
            "both",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("a", ucg_bool_type()),
                ucg_param("b", ucg_bool_type()),
            },
            2
        );
        {
            Ucg *a = ucg_get_param(both, 0);
            Ucg *b = ucg_get_param(both, 1);
            ucg_ret(ucg_op(AND, a, b));
        }
        ucg_end(both);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 5: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *flag = ucg_var("flag", ucg_bool_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, flag, ucg_bool_value(1)));
            {
                ucg_ret(ucg_i32_value(1));
            }
            Ucg *elif_stmt = ucg_elif(if_stmt);
            ucg_then(elif_stmt, ucg_op(EQ, flag, ucg_bool_value(0)));
            {
                ucg_ret(ucg_i32_value(2));
            }
            Ucg *else_stmt = ucg_else(if_stmt);
            ucg_then(else_stmt, NULL);
            {
                ucg_ret(ucg_i32_value(3));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 6: {
        Ucg *is_even = ucg_fn(
            "is_even",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("n1", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *n = ucg_get_param(is_even, 0);
            ucg_ret(ucg_op(EQ, ucg_op(MOD, n, ucg_i32_value(2)), ucg_i32_value(0)));
        }
        ucg_end(is_even);

        Ucg *both = ucg_fn(
            "both",
            ucg_bool_type(),
            (Ucg *[]){
                ucg_param("a", ucg_bool_type()),
                ucg_param("b", ucg_bool_type()),
            },
            2
        );
        {
            Ucg *a = ucg_get_param(both, 0);
            Ucg *b = ucg_get_param(both, 1);
            ucg_ret(ucg_op(AND, a, b));
        }
        ucg_end(both);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *n0 = ucg_var("n0", ucg_i32_type());

            Ucg *if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        is_even,
                        (Ucg *[]){
                            ucg_i32_value(4),
                        },
                        1
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        both,
                        (Ucg *[]){
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(2) }, 1),
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(6) }, 1),
                        },
                        2
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(2));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    EQ,
                    ucg_fn_call(
                        both,
                        (Ucg *[]){
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(2) }, 1),
                            ucg_fn_call(is_even, (Ucg *[]){ ucg_i32_value(7) }, 1),
                        },
                        2
                    ),
                    ucg_bool_value(1)
                )
            );
            {
                ucg_op(ADD_ASSIGN, n0, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_ret(n0);
        }
        ucg_end(fn_main);
        break;
    }
    case 7: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_bool_type());
            Ucg *b = ucg_var("b", ucg_bool_type());
            Ucg *n = ucg_var("n", ucg_i32_type());

            ucg_assign(a, ucg_bool_value(0));
            ucg_assign(a, ucg_bool_value(1));
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(OR, a, b));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(5));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(AND, a, b));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(50));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 8: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_i32_type());
            ucg_assign(a, ucg_i32_value(0));

            Ucg *loop = ucg_loop();
            ucg_then(loop, ucg_op(LT, a, ucg_i32_value(5)));
            {
                ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));
            }
            ucg_end(loop);

            ucg_ret(a);
        }
        ucg_end(fn_main);
        break;
    }
    case 9: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *i = ucg_var("i", ucg_i32_type());
            ucg_assign(i, ucg_i32_value(0));

            Ucg *going = ucg_var("going", ucg_bool_type());
            ucg_assign(going, ucg_bool_value(1));

            Ucg *loop = ucg_loop();
            ucg_then(loop, ucg_op(EQ, going, ucg_bool_value(1)));
            {
                ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
                ucg_assign(going, ucg_op(LT, i, ucg_i32_value(4)));
            }
            ucg_end(loop);

            ucg_ret(i);
        }
        ucg_end(fn_main);
        break;
    }
    case 10: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *a = ucg_var("a", ucg_char_type());
            Ucg *b = ucg_var("b", ucg_char_type());

            ucg_assign(a, ucg_char_value('a'));
            ucg_assign(b, ucg_char_value('b'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("<%c%c>\n"),
                    a,
                    b,
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 11: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *c = ucg_var("c", ucg_char_type());
            ucg_assign(c, ucg_char_value('x'));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, c, ucg_char_value('x')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(NQ, c, ucg_char_value('y')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, c, ucg_char_value('z')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(10));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 12: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *c = ucg_var("c", ucg_char_type());
            ucg_assign(c, ucg_char_value('m'));

            Ucg *n = ucg_var("n", ucg_i32_type());
            ucg_assign(n, ucg_i32_value(0));

            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(GT, c, ucg_char_value('a')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(LT, c, ucg_char_value('z')));
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            if_stmt = ucg_if();
            ucg_then(
                if_stmt,
                ucg_op(
                    AND,
                    ucg_op(GE, c, ucg_char_value('m')),
                    ucg_op(LE, c, ucg_char_value('m'))
                )
            );
            {
                ucg_op(ADD_ASSIGN, n, ucg_i32_value(1));
            }
            ucg_end(if_stmt);

            ucg_ret(n);
        }
        ucg_end(fn_main);
        break;
    }
    case 13: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *nl = ucg_var("nl", ucg_char_type());
            ucg_assign(nl, ucg_char_value('\n'));

            Ucg *tab = ucg_var("tab", ucg_char_type());
            ucg_assign(tab, ucg_char_value('\t'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    nl,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    tab,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c] [%c]\n"),
                    ucg_char_value('\''),
                    ucg_char_value('\\'),
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 14: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            Ucg *nl = ucg_var("nl", ucg_char_type());
            ucg_assign(nl, ucg_char_value('\n'));

            Ucg *tab = ucg_var("tab", ucg_char_type());
            ucg_assign(tab, ucg_char_value('\t'));

            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    nl,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c]\n"),
                    tab,
                },
                2
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("[%c] [%c]\n"),
                    ucg_char_value('\''),
                    ucg_char_value('\\'),
                },
                3
            );

            ucg_ret(ucg_i32_value(0));
        }
        ucg_end(fn_main);
        break;
    }
    case 15: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );

        Ucg *answer = ucg_fn(
            "answer",
            ucg_char_type(),
            (Ucg *[]){
                ucg_param("yes", ucg_bool_type()),
            },
            1
        );
        {
            Ucg *yes = ucg_get_param(answer, 0);
            Ucg *if_stmt = ucg_if();
            ucg_then(if_stmt, ucg_op(EQ, yes, ucg_bool_value(1)));
            {
                ucg_ret(ucg_char_value('Y'));
            }
            ucg_end(if_stmt);

            ucg_ret(ucg_char_value('N'));
        }
        ucg_end(answer);

        Ucg *show = ucg_fn(
            "show",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("c", ucg_char_type()),
            },
            1
        );
        {
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("%c\n"),
                    ucg_get_param(show, 0),
                },
                2
            );
        }
        ucg_end(show);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        {
            ucg_fn_call(
                show,
                (Ucg *[]){
                    ucg_fn_call(answer, (Ucg *[]){ ucg_bool_value(1) }, 1),
                },
                1
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("\n"),
                },
                1
            );
            ucg_fn_call(
                show,
                (Ucg *[]){
                    ucg_fn_call(answer, (Ucg *[]){ ucg_bool_value(0) }, 1),
                },
                1
            );
            ucg_fn_call(
                printf,
                (Ucg *[]){
                    ucg_string_value("\n"),
                },
                1
            );
        }
        ucg_end(fn_main);
        break;
    }
    case 16: {
        Ucg *fact = ucg_fn(
            "fact",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        {
            Ucg *stmt = ucg_if();
            {
                ucg_then(stmt, ucg_op(LE, ucg_get_param(fact, 0), ucg_i32_value(1)));
                ucg_ret(ucg_i32_value(1));
            }
            ucg_end(stmt);
            ucg_ret(ucg_op(
                MUL,
                ucg_get_param(fact, 0),
                ucg_fn_call(
                    fact,
                    (Ucg *[]){
                        ucg_op(SUB, ucg_get_param(fact, 0), ucg_i32_value(1)),
                    },
                    1
                )
            ));
        }
        ucg_end(fact);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);

        Ucg *call = ucg_fn_call(
            fact,
            (Ucg *[]){
                ucg_i32_value(5),
            },
            1
        );
        ucg_ret(call);
        ucg_end(fn_main);
        break;
    }
    case 17: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, a, ucg_i32_value(10)));
            Ucg *stmt = ucg_if();
            {
                ucg_then(stmt, ucg_op(EQ, a, ucg_i32_value(3)));
                ucg_brk(loop);
            }
            ucg_end(stmt);
            ucg_op(ADD_ASSIGN, a, ucg_i32_value(1));
        }
        ucg_end(loop);

        ucg_ret(a);
        ucg_end(fn_main);
        break;
    }
    case 18: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *i = ucg_var("i", ucg_i32_type());
        Ucg *sum = ucg_var("sum", ucg_i32_type());
        ucg_assign(i, ucg_i32_value(0));
        ucg_assign(sum, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, i, ucg_i32_value(10)));
            ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
            Ucg *stmt = ucg_if();
            {
                ucg_then(
                    stmt,
                    ucg_op(EQ, ucg_op(MOD, i, ucg_i32_value(2)), ucg_i32_value(0))
                );
                ucg_cnt(loop);
            }
            ucg_end(stmt);
            ucg_op(ADD_ASSIGN, sum, i);
        }
        ucg_end(loop);

        ucg_ret(sum);
        ucg_end(fn_main);
        break;
    }
    case 19: {
        Ucg *count = ucg_global("count", ucg_i32_type());

        Ucg *tick = ucg_fn("tick", ucg_void_type(), NULL, 0);
        ucg_op(ADD_ASSIGN, count, ucg_i32_value(1));
        ucg_end(tick);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *i = ucg_var("i", ucg_i32_type());
        ucg_assign(count, ucg_i32_value(0));
        ucg_assign(i, ucg_i32_value(0));

        Ucg *loop = ucg_loop();
        {
            ucg_then(loop, ucg_op(LT, i, ucg_i32_value(4)));
            ucg_fn_call(tick, NULL, 0);
            ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
        }
        ucg_end(loop);

        ucg_ret(count);
        ucg_end(fn_main);
        break;
    }
    case 20: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(5));
        ucg_ret(ucg_op(ADD, ucg_neg(a), ucg_i32_value(12)));
        ucg_end(fn_main);
        break;
    }
    case 21: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(3));

        Ucg *help = ucg_fn("help", ucg_i32_type(), NULL, 0);
        ucg_ret(ucg_i32_value(4));
        ucg_end(help);

        ucg_ret(ucg_op(ADD, a, ucg_fn_call(help, NULL, 0)));
        ucg_end(fn_main);
        break;
    }
    case 22: {
        Ucg *printf = ucg_proto(
            "printf",
            ucg_i32_type(),
            (Ucg *[]){
                ucg_param("fmt", ucg_string_type()),
                ucg_varg_type(),
            },
            2
        );
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_float_type(32));
        Ucg *b = ucg_var("b", ucg_float_type(64));
        ucg_assign(a, ucg_float_value(32, 1.5));
        ucg_assign(b, ucg_float_value(64, 2.25));
        ucg_assign(b, ucg_op(MUL, b, ucg_cast(a, ucg_float_type(64))));
        ucg_fn_call(
            printf,
            (Ucg *[]){
                ucg_string_value("%f %f\n"),
                a,
                b,
            },
            3
        );

        Ucg *stmt = ucg_if();
        {
            ucg_then(stmt, ucg_op(GT, b, ucg_float_value(64, 3.0)));
            ucg_ret(ucg_cast(ucg_neg(b), ucg_i32_type()));
        }
        ucg_end(stmt);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 23: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *rect = ucg_struct("Rect");
        ucg_attrs(
            rect,
            (Ucg *[]){
                ucg_attr("a", point),
                ucg_attr("b", point),
            },
            2
        );

        Ucg *make = ucg_fn(
            "make",
            point,
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        Ucg *p = ucg_var("p", point);
        ucg_assign(ucg_get_attr(p, 0), ucg_get_param(make, 0));
        ucg_ret(p);
        ucg_end(make);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *r = ucg_var("r", rect);
        ucg_assign(ucg_get_attr(ucg_get_attr(r, 0), 0), ucg_i32_value(1));
        ucg_assign(
            ucg_get_attr(r, 1),
            ucg_fn_call(
                make,
                (Ucg *[]){
                    ucg_i32_value(5),
                },
                1
            )
        );
        ucg_assign(ucg_get_attr(ucg_get_attr(r, 1), 1), ucg_i32_value(2));
        Ucg *diff = ucg_op(
            SUB,
            ucg_get_attr(ucg_get_attr(r, 1), 0),
            ucg_get_attr(ucg_get_attr(r, 0), 0)
        );
        ucg_ret(ucg_op(ADD, diff, ucg_get_attr(ucg_get_attr(r, 1), 1)));
        ucg_end(fn_main);
        break;
    }
    case 24: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *p = ucg_var("p", point);
        Ucg *n = ucg_var("n", ucg_i32_type());
        Ucg *sum = ucg_op(ADD, ucg_get_attr(p, 0), ucg_get_attr(p, 1));
        ucg_ret(ucg_op(ADD, sum, n));
        ucg_end(fn_main);
        break;
    }
    case 25: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *p = ucg_var("p", point);
        ucg_assign(
            p,
            ucg_struct_value(
                point,
                (Ucg *[]){
                    ucg_i32_value(10),
                },
                1
            )
        );
        ucg_ret(ucg_op(ADD, ucg_get_attr(p, 0), ucg_get_attr(p, 1)));
        ucg_end(fn_main);
        break;
    }
    case 26: {
        Ucg *point = ucg_struct("Point");
        ucg_attrs(
            point,
            (Ucg *[]){
                ucg_attr("x", ucg_i32_type()),
                ucg_attr("y", ucg_i32_type()),
            },
            2
        );

        Ucg *swap = ucg_fn(
            "swap",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("x", ucg_ref_type(ucg_i32_type())),
                ucg_param("y", ucg_ref_type(ucg_i32_type())),
            },
            2
        );
        Ucg *x = ucg_get_param(swap, 0);
        Ucg *y = ucg_get_param(swap, 1);
        Ucg *t = ucg_var("t", ucg_i32_type());
        ucg_assign(t, ucg_point_to(x));
        ucg_assign(ucg_point_to(x), ucg_point_to(y));
        ucg_assign(ucg_point_to(y), t);
        ucg_end(swap);

        Ucg *move = ucg_fn(
            "move",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("p", ucg_ref_type(point)),
            },
            1
        );
        ucg_op(
            ADD_ASSIGN,
            ucg_get_attr(ucg_point_to(ucg_get_param(move, 0)), 0),
            ucg_i32_value(5)
        );
        ucg_end(move);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        Ucg *b = ucg_var("b", ucg_i32_type());
        ucg_assign(a, ucg_i32_value(1));
        ucg_assign(b, ucg_i32_value(2));
        ucg_fn_call(
            swap,
            (Ucg *[]){
                ucg_ref(a),
                ucg_ref(b),
            },
            2
        );

        Ucg *p = ucg_var("p", point);
        ucg_fn_call(
            move,
            (Ucg *[]){
                ucg_ref(p),
            },
            1
        );

        Ucg *r = ucg_var("r", ucg_ref_type(ucg_i32_type()));
        ucg_assign(r, ucg_ref(a));
        ucg_assign(ucg_point_to(r), ucg_i32_value(7));

        Ucg *sum = ucg_op(ADD, ucg_op(MUL, a, ucg_i32_value(10)), b);
        ucg_ret(ucg_op(ADD, sum, ucg_get_attr(p, 0)));
        ucg_end(fn_main);
        break;
    }
    case 27: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *hp = ucg_var("hp", ucg_i32_type());
        Ucg *speed = ucg_var("speed", ucg_float_type(32));
        Ucg *alive = ucg_var("alive", ucg_i1_type());
        Ucg *grade = ucg_var("grade", ucg_char_type());
        ucg_assign(hp, ucg_i32_value(100));
        ucg_assign(speed, ucg_float_value(32, 1.5));
        ucg_assign(alive, ucg_i1_value(1));
        ucg_assign(grade, ucg_char_value('S'));
        Ucg *values[] = {
            ucg_string_value("hp: "),     hp,    ucg_string_value(" speed: "), speed,
            ucg_string_value(" alive: "), alive, ucg_string_value(" grade: "), grade,
            ucg_string_value(" 100%\n"),
        };
        ucg_write(ucg_i32_value(1), values, 9);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 28: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *first = ucg_var("first", ucg_string_type());
        Ucg *second = ucg_var("second", ucg_string_type());
        ucg_assign(first, ucg_read(ucg_i32_value(0)));
        ucg_assign(second, ucg_read(ucg_i32_value(0)));
        Ucg *values[] = {
            ucg_string_value("["),   first, ucg_string_value("] ["), second,
            ucg_string_value("]\n"),
        };
        ucg_write(ucg_i32_value(1), values, 5);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 29: {
        Ucg *i32 = ucg_i32_type();
        Ucg *malloc = ucg_proto(
            "malloc",
            ucg_string_type(),
            (Ucg *[]){
                ucg_param("size", i32),
            },
            1
        );
        Ucg *fn_main = ucg_fn("main", i32, NULL, 0);
        Ucg *arr = ucg_var("arr", ucg_ref_type(i32));
        Ucg *size = ucg_op(MUL, ucg_sizeof(i32), ucg_i32_value(4));
        ucg_assign(
            arr,
            ucg_cast(
                ucg_fn_call(
                    malloc,
                    (Ucg *[]){
                        size,
                    },
                    1
                ),
                ucg_ref_type(i32)
            )
        );
        ucg_assign(ucg_access(arr, ucg_i32_value(2)), ucg_i32_value(7));
        ucg_assign(
            ucg_access(arr, ucg_i32_value(0)),
            ucg_op(ADD, ucg_access(arr, ucg_i32_value(2)), ucg_i32_value(1))
        );
        Ucg *values[] = {
            ucg_typeof(ucg_ref_type(i32)),
            ucg_string_value(" "),
            ucg_access(arr, ucg_i32_value(0)),
            ucg_string_value(" "),
            ucg_sizeof(i32),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 6);
        ucg_ret(ucg_access(arr, ucg_i32_value(0)));
        ucg_end(fn_main);
        break;
    }
    case 30: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *small = ucg_var("small", ucg_uint_type(8));
        ucg_assign(small, ucg_int_value(8, 250));
        ucg_op(ADD_ASSIGN, small, ucg_int_value(8, 10));
        Ucg *big = ucg_var("big", ucg_uint_type(32));
        ucg_assign(big, ucg_int_value(32, 4000000000));
        Ucg *half = ucg_var("half", ucg_uint_type(32));
        ucg_assign(half, ucg_op(DIV, big, ucg_int_value(32, 2)));
        Ucg *is_big = ucg_var("is_big", ucg_bool_type());
        ucg_assign(is_big, ucg_op(GT, big, ucg_int_value(32, 100)));
        Ucg *values[] = {
            ucg_typeof(ucg_uint_type(8)),
            ucg_string_value(" "),
            small,
            ucg_string_value(" "),
            big,
            ucg_string_value(" "),
            half,
            ucg_string_value(" "),
            is_big,
            ucg_string_value(" "),
            ucg_cast(big, ucg_float_type(64)),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 12);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 31: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *x = ucg_var("x", ucg_i32_type());
        ucg_assign(x, ucg_i32_value(5));
        Ucg *small = ucg_var("small", ucg_bool_type());
        ucg_assign(small, ucg_not(ucg_op(GT, x, ucg_i32_value(3))));
        Ucg *values[] = {
            small,
            ucg_string_value(" "),
            ucg_not(small),
            ucg_string_value("\n"),
        };
        ucg_write(ucg_i32_value(1), values, 4);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 32: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_i32_type());
        ucg_clean(a);
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_string_value("clean a\n"),
            },
            1
        );
        ucg_clean_end();
        Ucg *i = ucg_var("i", ucg_i32_type());
        ucg_assign(i, ucg_i32_value(0));
        Ucg *loop = ucg_loop();
        ucg_then(loop, ucg_op(LT, i, ucg_i32_value(3)));
        Ucg *b = ucg_var("b", ucg_i32_type());
        ucg_assign(b, i);
        ucg_clean(b);
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_string_value("clean b "),
                b,
                ucg_string_value("\n"),
            },
            3
        );
        ucg_clean_end();
        ucg_op(ADD_ASSIGN, i, ucg_i32_value(1));
        Ucg *stop = ucg_if();
        ucg_then(stop, ucg_op(EQ, i, ucg_i32_value(2)));
        ucg_brk(loop);
        ucg_end(stop);
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_string_value("loop\n"),
            },
            1
        );
        ucg_end(loop);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 33: {
        ucg_throws();
        Ucg *boom = ucg_fn(
            "boom",
            ucg_void_type(),
            (Ucg *[]){
                ucg_param("n", ucg_i32_type()),
            },
            1
        );
        Ucg *pick = ucg_if();
        ucg_then(pick, ucg_op(EQ, ucg_get_param(boom, 0), ucg_i32_value(1)));
        ucg_throw(ucg_i32_value(7));
        ucg_then(ucg_else(pick), NULL);
        ucg_throw(ucg_float_value(64, 2.5));
        ucg_end(pick);
        ucg_end(boom);

        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        for (int n = 1; n <= 2; n++) {
            Ucg *try_stmt = ucg_try();
            Ucg *x = ucg_var("x", ucg_i32_type());
            ucg_clean(x);
            ucg_write(
                ucg_i32_value(1),
                (Ucg *[]){
                    ucg_string_value("clean x\n"),
                },
                1
            );
            ucg_clean_end();
            ucg_fn_call(
                boom,
                (Ucg *[]){
                    ucg_i32_value(n),
                },
                1
            );
            Ucg *as_int = ucg_catch(try_stmt, ucg_i32_type());
            ucg_write(
                ucg_i32_value(1),
                (Ucg *[]){
                    ucg_string_value("caught i32 "),
                    ucg_error(ucg_i32_type()),
                    ucg_string_value("\n"),
                },
                3
            );
            ucg_catch(as_int, ucg_float_type(64));
            ucg_write(
                ucg_i32_value(1),
                (Ucg *[]){
                    ucg_string_value("caught f64 "),
                    ucg_error(ucg_float_type(64)),
                    ucg_string_value("\n"),
                },
                3
            );
            ucg_end_catch(as_int);
        }
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 34: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *a = ucg_var("a", ucg_ref_type(ucg_i32_type()));
        Ucg *b = ucg_var("b", ucg_ref_type(ucg_i32_type()));
        ucg_assign(a, ucg_alloc(ucg_i32_type(), ucg_i32_value(3)));
        ucg_assign(b, ucg_alloc(ucg_i32_type(), ucg_i32_value(3)));
        for (int i = 0; i < 3; i++)
            ucg_assign(ucg_access(a, ucg_i32_value(i)), ucg_i32_value(10 * (i + 1)));
        ucg_copy(b, a, ucg_i32_value(3));
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_access(b, ucg_i32_value(0)),
                ucg_string_value(" "),
                ucg_access(b, ucg_i32_value(1)),
                ucg_string_value(" "),
                ucg_access(b, ucg_i32_value(2)),
                ucg_string_value("\n"),
            },
            6
        );
        ucg_free(a);
        ucg_free(b);
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 35: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *s = ucg_var("s", ucg_string_type());
        ucg_assign(s, ucg_string_array("hello"));
        ucg_assign(ucg_access(s, ucg_i32_value(0)), ucg_char_value('j'));
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                s,
                ucg_string_value("\n"),
            },
            2
        );
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    case 36: {
        Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
        Ucg *first = ucg_access(ucg_argv(), ucg_i32_value(0));
        ucg_write(
            ucg_i32_value(1),
            (Ucg *[]){
                ucg_argc(),
                ucg_string_value(" "),
                first,
                ucg_string_value("\n"),
            },
            4
        );
        ucg_ret(ucg_i32_value(0));
        ucg_end(fn_main);
        break;
    }
    default: {
        return false;
    }
    }
    return true;
}

bool compile_test(int p) {
    print(CYAN("Test %d: ================================\n"), p);
    ucg_start(format("test%d", p));
    bool r = run_test(p);
    print_ucgs();
    // ucg_compile();
    // mkdir2("build");
    // ucg_path("build/out.ll");
    // ucg_bin_path(r ? "build/exe.out" : NULL);
    // ucg_gen();
    // if (r)
    //     ucg_run_bin();
    ucg_finish();
    return r;
}

#include "./lib/header.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <signal.h> // SIGSEGV
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

// LLVM STUFF
#include <llvm/Config/llvm-config.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/DebugInfo.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm/Config/llvm-config.h>

#if LLVM_VERSION_MAJOR >= 13
#    undef LLVM_C_STRICT_PROTOTYPES_BEGIN
#    undef LLVM_C_STRICT_PROTOTYPES_END
#    define LLVM_C_STRICT_PROTOTYPES_BEGIN
#    define LLVM_C_STRICT_PROTOTYPES_END
#    include <llvm-c/Transforms/PassBuilder.h>
typedef LLVMErrorRef ErrorRef;
typedef LLVMPassBuilderOptionsRef PassBuilderOptions;
#else
#    include <llvm-c/Transforms/PassManagerBuilder.h>
#endif

typedef LLVMTypeRef TypeRef;
typedef LLVMContextRef Context;
typedef LLVMModuleRef Module;
typedef LLVMBuilderRef Builder;
typedef LLVMBasicBlockRef Bloc;
typedef LLVMValueRef Value;
typedef LLVMTargetDataRef TargetData;
typedef LLVMTargetRef Target;
typedef LLVMTargetMachineRef TargetMachine;
typedef LLVMCodeGenOptLevel CodeGenOptLevel;
typedef LLVMRelocMode RelocMode;
typedef LLVMCodeModel CodeModel;
typedef LLVMTypeKind TypeKind;
typedef LLVMAttributeRef AttributeRef;
typedef LLVMMetadataRef MetadataRef;
typedef LLVMOpcode OpCode;

typedef enum Type Type;
typedef struct Ucg Ucg;

#include "proto.h"


enum Type {
    // clang-format off
    NONE, ERR, SCOPE, URA,
    
    // ID become VAR, after getting declared
    ID, VAR, 
	PARAM, GLOBAL,
    DECLARE,
    INIT_VAR,
    DREF,
    ARG,

    VAL_CREATE,

    TEMPLATE,
    TYPES, TEMPLATE_TYPE, 
    ANY,
    VOID, 
    
    // BOOL, I8, I32,
    // CHAR, BOOL,
    INT, FLOAT,
    
    CHARS,
    
    REF, POINT_TO,
    ZERO,

    STRUCT, DOT, ATTR, ENUM,

    //OWN,

    LPARENT, RPARENT, DOTS, DOUBLE_DOTS, COMA,
    OPERATOR, PUB,
    LBRACK, RBRACK,
    LCBRACK, RCBRACK,
    ARRAY, ARRAY_LIT, ACCESS,

    ASSIGN,
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN, LSHIFT_ASSIGN, RSHIFT_ASSIGN,
    ADD, SUB, MUL, DIV, MOD,
    BAND, BOR, BXOR, LSHIFT, RSHIFT,

    GT, LT, GE, LE, EQ, NQ,

    AND, OR, NOT, NEG, BNOT,

    PROTO, 
    FN_DEC, ARGS, VARIADIC, FN_CALL, RETURN, FORMAT,
    LINK, USE, MACRO_IF, MACRO_ELIF, MACRO_ELSE, MACRO_ERROR,
    MACRO_NAME, MACRO_VERSION, MACRO_ENTRY, MACRO_SCRIPT, MACRO_DEPENDENCY,
    MACRO_URL, MACRO_PATH, MACRO_CMD,
	FN_ENTRY,
    SIZEOF, TYPEOF, DROP, COPY, IS_ZERO, ALLOC, MEMCPY, FREE,
    AS, FROM, AND_LIT, OR_LIT, SELF, FOR, IN, MODULE, ARGC, ARGV,
    LINE_NUMBER, FILE_NAME, FN_NAME,
	BLOC,
    IF, ELIF, ELSE, THEN,
    LOOP, BRK, CNT, MATCH, CASE, DEFAULT, CLEAN,
    TRY, CATCH, THROW, CHECK,
    END,
    LAST,
    // clang-format on
};

#define Struct(T)     \
    struct {          \
        T *arr;       \
        size_t count; \
        size_t size;  \
    }

struct Ucg {
    unsigned long inst_index;

    char *name;

    Type type;
    int shift;
    bool is_val;
    bool is_set;
    bool is_uint;
    bool is_throwing;
    int used;

    struct { // llvm stuff
        TypeRef rtype;
        Value elem;
        Bloc bloc;
        Bloc then;
        Bloc caller_bloc;
    };

    struct {
        bool varg;
        Ucg *ret;
        // Struct(Ucg *, params);
        // struct {
        //     Ucg **arr;
        //     size_t count;
        //     size_t size;
        // }
        expand(Ucg *, params);
    } fn;

    struct {
        long long value;
        double real;
    } num;

    struct {
        Ucg *elem;
        expand(Ucg *, items);
    } arr;

    struct {
        expand(Ucg *, attrs);
        int index;
    } obj;

    // struct {
    //     Bloc cond;
    //     Bloc then;
    //     Bloc end;
    // } bloc;

    union {
        Ucg *left;

        Ucg *data_type;

        Ucg *to_enter; // entry
        Ucg *to_ret;
        Ucg *to_create;
        Ucg *to_dref;
        Ucg *to_ref;
        Ucg *to_end; // used in ENDIF
        Ucg *to_call;
        Ucg *stmt;
        Ucg *base;
        Ucg *to_point;
        Ucg *to_size;
        // Ucg
    };
    struct {
        Ucg *cond;
        // Ucg *then;
        Ucg *next; // next of the if statement
        // Ucg *head;
        // Ucg *last;
        Ucg *end; // ENDIF
    };
    union {
        Ucg *right;
        Ucg *to_type;
        Ucg *attr;
        Ucg *index;
    };
    expand(Ucg *, cleans);
};

typedef struct {
    File *file;
    int s;
    int e;
    char *msg;
    char *help;
} UcgError;

struct {
    struct {
        TypeRef vd;
        // TypeRef i1;
        // TypeRef i8;
        // TypeRef i16;
        // TypeRef i32;
        // TypeRef i64;
        TypeRef num[255];
    } types;

    Context context;
    Module module;
    Builder builder;
    TargetMachine machine;

    char *dir;
    char *ll_path;
    char *asm_path;
    char *obj_path;
    char *exe_path;
    expand(char *, libs);

    char *error_msg;

    expand(Ucg *, insts);
    expand(Ucg *, runtime);

    expand(Ucg *, blocks);
    int clean_start;
    Ucg *clean_var;

    bool is_cleaning;
    bool throws;
    expand(Ucg *, tries);
    Ucg *thrown;
    Ucg *error;
    Ucg *error_type;
    expand(Ucg *, error_types);
    expand(Ucg *, error_ids);

    expand(UcgError, errors);
} gb_ucg;

void ucg_add_error(File *file, int s, int e, char *msg, char *help) {
    UcgError error = { .file = file, .s = s, .e = e, .msg = msg, .help = help };
    append(gb_ucg.errors, error);
}

bool ucg_has_errors(void) {
    return gb_ucg.errors_count > 0;
}

void ucg_check_errors(void) {
    if (gb_ucg.errors_count == 0)
        return;
    for (int i = 0; i < gb_ucg.errors_count; i++) {
        UcgError *error = &gb_ucg.errors[i];
        char *buff = error->file->buff;
        char *path = relative_path(error->file->path);
        int line = 1;
        for (int j = 0; j < error->s; j++)
            line += buff[j] == '\n';
        int start = error->s;
        while (start > 0 && buff[start - 1] != '\n')
            start--;
        int end = error->s;
        while (buff[end] && buff[end] != '\n')
            end++;
        int col = error->s - start;
        int width = snprintf(NULL, 0, "%d", line);

        fprintf(stderr, "%s: %s\n", RED("error"), error->msg);
        fprintf(stderr, "at %s:%d:%d\n", path, line, col + 1);
        fprintf(stderr, "%*s |\n", width, "");
        fprintf(stderr, "%d | %.*s\n", line, end - start, buff + start);
        fprintf(stderr, "%*s | %*s", width, "", col, "");
        for (int j = error->s; j < error->e || j == error->s; j++)
            fprintf(stderr, RED("^"));
        fprintf(stderr, "\n");
        if (error->help)
            fprintf(stderr, "%s: %s\n", CYAN("help"), error->help);
    }
    fprintf(
        stderr,
        "%s: aborting due to %d error%s\n",
        RED("error"),
        (int)gb_ucg.errors_count,
        gb_ucg.errors_count > 1 ? "s" : ""
    );
    exit(1);
}

void ucg_start(char *name) {
    // atexit(ucg_finish);

    gb_ucg.context = LLVMContextCreate();
    zero_init(gb_ucg.runtime);
    zero_init(gb_ucg.libs);
    gb_ucg.module = LLVMModuleCreateWithNameInContext(name, gb_ucg.context);
    gb_ucg.builder = LLVMCreateBuilderInContext(gb_ucg.context);

    gb_ucg.types.vd = LLVMVoidTypeInContext(gb_ucg.context);

    for (int i = 1; i < 255; i++)
        gb_ucg.types.num[i] = LLVMIntTypeInContext(gb_ucg.context, i);

    // Initialize native target.
    if (LLVMInitializeNativeTarget()) {
        eprint("%s:%d Failed to initialize native target\n", FILE, LINE);
        exit(1);
    }

    if (LLVMInitializeNativeAsmPrinter()) {
        eprint("%s:%d Failed to initialize native asm printer\n", FILE, LINE);
        exit(1);
    }

    char *triple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(gb_ucg.module, triple);

    Target target;
    char *error = NULL;

    if (LLVMGetTargetFromTriple(triple, &target, &error)) {
        eprint("%s:%d GetTargetFromTriple error: %s\n", FILE, LINE, error);
        exit(1);
    }

    gb_ucg.machine = LLVMCreateTargetMachine(
        target,
        triple,
        "", // CPU
        "", // Features
        LLVMCodeGenLevelDefault,
        LLVMRelocPIC,
        LLVMCodeModelDefault
    );

    if (!gb_ucg.machine) {
        eprint("%s:%d Failed to create target machine\n", FILE, LINE);
        LLVMDisposeMessage(triple);
        exit(1);
    }

    TargetData layout = LLVMCreateTargetDataLayout(gb_ucg.machine);
    LLVMSetModuleDataLayout(gb_ucg.module, layout);

    LLVMDisposeTargetData(layout);
    LLVMDisposeMessage(triple);

}

void ucg_finish(void) {
    if (gb_ucg.error_msg != NULL) {
        print("there is an error %s\n", gb_ucg.error_msg);
        // LLVMDisposeMessage(gb_ucg.error_msg);
        eprint("%s: <%s>\n", RED("error"), gb_ucg.error_msg);
    }
    LLVMDisposeBuilder(gb_ucg.builder);
    LLVMDisposeModule(gb_ucg.module);
    LLVMDisposeTargetMachine(gb_ucg.machine);
    LLVMContextDispose(gb_ucg.context);
    gb_ucg.throws = false;
    gb_ucg.thrown = NULL;
    gb_ucg.error = NULL;
    gb_ucg.error_type = NULL;
    gb_ucg.error_types_count = 0;
    gb_ucg.error_ids_count = 0;
    gb_ucg.tries_count = 0;
    gb_ucg.blocks_count = 0;
    // arena_free();
}

void ucg_gen_ll(void) {
    mkdir2(dirname2(gb_ucg.ll_path));

    char *error = NULL;

    if (LLVMPrintModuleToFile(gb_ucg.module, gb_ucg.ll_path, &error)) {
        eprint("%s:%d LLVM IR error: \n%s\n", FILE, LINE, error);
        LLVMDisposeMessage(error);
    }
}

bool llvm_verify(void) {
    char *error = NULL;
    bool failed = LLVMVerifyModule(gb_ucg.module, LLVMReturnStatusAction, &error);
    if (failed)
        eprint("%s:%d LLVM verification failed:\n%s\n", FILE, LINE, error);
    LLVMDisposeMessage(error);
    return !failed;
}

void llvm_emit(char *path, LLVMCodeGenFileType type) {
    mkdir2(dirname2(path));

    char *error = NULL;

    assert(gb_ucg.machine);
    assert(gb_ucg.module);

    if (LLVMTargetMachineEmitToFile(
            gb_ucg.machine, gb_ucg.module, path, type, &error
        )) {
        eprint("LLVM codegen error: %s\n", error);
        LLVMDisposeMessage(error);
        exit(1);
    }
}

void ucg_gen_bin(void) {
    char *obj_path = gb_ucg.obj_path;
    if (obj_path == NULL) {
        obj_path = format("%s.o", gb_ucg.exe_path);
        llvm_emit(obj_path, LLVMObjectFile);
    }

    char **argv = arena_alloc(gb_ucg.libs_count + 5, sizeof(char *));
    int argc = 0;
    argv[argc++] = "clang";
    argv[argc++] = obj_path;
    argv[argc++] = "-o";
    argv[argc++] = gb_ucg.exe_path;
    for (int i = 0; i < gb_ucg.libs_count; i++)
        argv[argc++] = gb_ucg.libs[i];

    pid_t pid = fork();
    if (pid == 0) {
        execvp("clang", argv);
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        eprint("Linking failed\n");
        exit(1);
    }
    if (gb_ucg.obj_path == NULL)
        remove(obj_path);
}

void ucg_gen(void) {
    bool valid = llvm_verify();
    if (gb_ucg.ll_path)
        ucg_gen_ll();
    if (!valid)
        exit(1);
    if (gb_ucg.asm_path)
        llvm_emit(gb_ucg.asm_path, LLVMAssemblyFile);
    if (gb_ucg.obj_path)
        llvm_emit(gb_ucg.obj_path, LLVMObjectFile);
    if (gb_ucg.exe_path)
        ucg_gen_bin();
}

int ucg_run_bin(void) {
    pid_t pid = fork();

    if (pid == 0) {
        execl(gb_ucg.exe_path, gb_ucg.exe_path, NULL);
        _exit(127);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        print("Exit code: %d\n", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }
    print("Terminated by signal: %d\n", WTERMSIG(status));
    return 128 + WTERMSIG(status);
}

void llvm_fn_dec(Ucg *ir) {
    // assert(ir->fn.ret_type);
    assert(ir->name);

    expand(TypeRef, types);
    zero_init(types);

    for (int i = 0; i < ir->fn.params_count; i++) {
        Ucg *param = ir->fn.params[i];
        // param->elem = LLVMGetParam(ir->elem, i);
        assert(param);
        assert(param->type);
        // printf(CYAN("access %d -> %d\n"), i, param->type);
        param->rtype = llvm_type(param);
        append(types, param->rtype);
    }

    // TypeRef tmp = LLVMIntTypeInContext(gb_ucg.context, 32);

    ir->rtype =
        LLVMFunctionType(llvm_type(ir->fn.ret), types, types_count, ir->fn.varg);

    ir->elem = LLVMGetNamedFunction(gb_ucg.module, ir->name);
    if (ir->elem == NULL || LLVMGlobalGetValueType(ir->elem) != ir->rtype)
        ir->elem = LLVMAddFunction(gb_ucg.module, ir->name, ir->rtype);
    // // printf("declare %s, has %zu params\n", ir->name, ir->fn.params_count);
    for (int i = 0; i < ir->fn.params_count; i++) {
        Ucg *param = ir->fn.params[i];
        param->elem = LLVMGetParam(ir->elem, i);
        // append(types, param->rtype);
    }
}

void llvm_fn_call(Ucg *ir) {
    Ucg *fn_dec = ir->to_call;
    expand(Value, params);
    zero_init(params);

    for (int i = 0; i < ir->fn.params_count; i++) {
        Value value = ir->fn.params[i]->elem;
        bool is_extra = i >= fn_dec->fn.params_count; // printf passed f32 as f64
        if (is_extra && llvm_is_float(value) && llvm_bits(value) == 32)
            value = LLVMBuildFPExt(gb_ucg.builder, value, llvm_float_type(64), "");
        TypeRef type = is_extra ? NULL : fn_dec->fn.params[i]->rtype;
        bool is_pointer = LLVMGetTypeKind(LLVMTypeOf(value)) == LLVMPointerTypeKind;
        if (type && is_pointer && LLVMTypeOf(value) != type)
            value = LLVMBuildBitCast(gb_ucg.builder, value, type, "");
        append(params, value);
    }
    TypeRef rtype = fn_dec->rtype;
    Value fn_elem = fn_dec->elem;
    char *name = fn_dec->fn.ret->type != VOID ? ir->name : "";
    Value elem =
        LLVMBuildCall2(gb_ucg.builder, rtype, fn_elem, params, params_count, name);
    ir->elem = elem;
}

bool is_bloc_terminated() {
    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    return bloc != NULL && LLVMGetBasicBlockTerminator(bloc) != NULL;
}

void llvm_end_fn(Ucg *fn) {
    if (!is_bloc_terminated()) {
        if (fn->fn.ret->type == VOID)
            LLVMBuildRetVoid(gb_ucg.builder);
        else {
            TypeRef rtype = llvm_type(fn->fn.ret);
            LLVMBuildRet(gb_ucg.builder, LLVMConstNull(rtype));
        }
    }
    if (fn->caller_bloc)
        llvm_set_pos(fn->caller_bloc);
}

void llvm_ret(Ucg *ir) {
    if (is_bloc_terminated())
        return;

    if (ir == NULL) {
        LLVMBuildRetVoid(gb_ucg.builder);
        return;
    }
    LLVMBuildRet(gb_ucg.builder, ir->elem);
}


TypeRef llvm_float_type(int bits) {
    if (bits == 32)
        return LLVMFloatTypeInContext(gb_ucg.context);
    return LLVMDoubleTypeInContext(gb_ucg.context);
}

TypeRef llvm_type(Ucg *ir) {
    switch (ir->type) { // clang-format off
    case INT:        	return gb_ucg.types.num[ir->shift];
    case FLOAT:			return llvm_float_type(ir->shift);
    case CHARS:			return LLVMPointerType(gb_ucg.types.num[8], 0);
    case STRUCT:		return ir->rtype;
    case POINT_TO:		return llvm_type(ucg_type_of(ir));
    case ATTR:			return llvm_type(ir->data_type);
    case DOT:			return llvm_type(ir->attr);
    case ACCESS:		return llvm_type(ucg_type_of(ir));
	case VOID:			return gb_ucg.types.vd;
    case VAL_CREATE:	return llvm_type(ir->to_create);
    case DECLARE:		return llvm_type(ir->data_type);
    case PARAM:			return llvm_type(ir->data_type);
    case GLOBAL:		return llvm_type(ir->data_type);
	case ARRAY: {
		Ucg* elem = ir->arr.elem;
		int count = ir->arr.items_count;
		return LLVMArrayType(llvm_type(elem), count);
	}
	case REF: {
		return LLVMPointerType(llvm_type(ir->to_ref), 0);
	}
    default: { 
        eprint("%s: %s:%d handle this case %s\n", 
            RED("error"), FILE, LINE, to_string(ir->type)); 
        exit(1);
    }
    }
    // clang-format on
    return NULL;
}

void llvm_val_create(Ucg *ir) {
    // assert(ir->rtype);
    Ucg *to_create = ir->to_create;
    TypeRef rtype = llvm_type(to_create);
    switch (to_create->type) { // clang-format off
    case INT:   ir->elem = LLVMConstInt(rtype, to_create->num.value, 0); break;
    case FLOAT: ir->elem = LLVMConstReal(rtype, to_create->num.real); break;
    case REF:   ir->elem = LLVMConstNull(rtype); break; // clang-format on
    case STRUCT: {
        Value agg = LLVMConstNull(rtype);
        for (int i = 0; i < ir->arr.items_count; i++) {
            Ucg *item = ir->arr.items[i];
            if (item)
                agg = LLVMBuildInsertValue(gb_ucg.builder, agg, item->elem, i, "");
        }
        ir->elem = agg;
        break;
    }
    case CHARS: {
        if (ir->used < 0)
            break;
        int count = to_create->arr.items_count;
        char *text = arena_alloc(count + 1, sizeof(char));
        for (int i = 0; i < count; i++)
            text[i] = to_create->arr.items[i]->to_create->num.value;
        Value global = LLVMBuildGlobalString(gb_ucg.builder, text, "str");
        ir->elem = LLVMBuildPointerCast(gb_ucg.builder, global, rtype, "");
        break;
    }
    case ARRAY: {
        Value agg = LLVMGetUndef(rtype);
        for (int i = 0; i < to_create->arr.items_count; i++) {
            agg = LLVMBuildInsertValue(
                gb_ucg.builder, agg, to_create->arr.items[i]->elem, i, ""
            );
        }

        Value stack = llvm_alloc(rtype, "arr");
        LLVMBuildStore(gb_ucg.builder, agg, stack);

        TypeRef i32 = gb_ucg.types.num[32];
        Value zero[] = {
            LLVMConstInt(i32, 0, 0),
            LLVMConstInt(i32, 0, 0),
        };
        ir->elem = LLVMBuildInBoundsGEP2(gb_ucg.builder, rtype, stack, zero, 2, "");

        break;
    }
    default: {
        eprint(
            "%s: %s:%d handle this case %s\n",
            RED("error"),
            FILE,
            LINE,
            to_string(to_create->type)
        );
        exit(1);
    }
    }
}


Value llvm_alloc(TypeRef rtype, char *name) {
    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    assert(bloc);
    Value basic = LLVMGetBasicBlockParent(bloc);
    assert(basic);
    Bloc entry = LLVMGetEntryBasicBlock(basic);

    Value inst = LLVMGetFirstInstruction(entry);
    while (inst && LLVMIsAAllocaInst(inst))
        inst = LLVMGetNextInstruction(inst);

    if (inst)
        LLVMPositionBuilderBefore(gb_ucg.builder, inst);
    else
        LLVMPositionBuilderAtEnd(gb_ucg.builder, entry);

    Value elem = LLVMBuildAlloca(gb_ucg.builder, rtype, name);
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
    return elem;
}

void llvm_declare_var(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    // assert(ir->rtype);
    assert(ir->name);
    ir->elem = llvm_alloc(ir->rtype, ir->name);
    if (!ir->is_set)
        llvm_zero_init(ir);
}

void llvm_struct_create(Ucg *ir) {
    ir->rtype = LLVMStructCreateNamed(gb_ucg.context, ir->name);
}

void llvm_struct_body(Ucg *ir) {
    int count = ir->obj.attrs_count;
    TypeRef *types = arena_alloc(count + 1, sizeof(TypeRef));
    for (int i = 0; i < count; i++)
        types[i] = llvm_type(ir->obj.attrs[i]);
    LLVMStructSetBody(ir->rtype, types, count, 0);
}

char *ucg_string_text(Ucg *value) {
    if (value->type != VAL_CREATE || value->to_create->type != CHARS)
        return NULL;
    Ucg *arr = value->to_create;
    char *text = arena_alloc(arr->arr.items_count * 2 + 1, sizeof(char));
    int len = 0;
    for (int i = 0; i < arr->arr.items_count; i++) {
        char c = arr->arr.items[i]->to_create->num.value;
        if (c == '\0')
            break;
        if (c == '%')
            text[len++] = '%';
        text[len++] = c;
    }
    return text;
}

void llvm_ref(Ucg *ir) {
    ir->elem = ir->to_ref->elem;
}

void llvm_point_to(Ucg *ir) {
    Ucg *ref = ir->to_point;
    ir->elem = LLVMBuildLoad2(gb_ucg.builder, llvm_type(ref), ref->elem, ir->name);
}

void llvm_get_attr(Ucg *ir) {
    Ucg *base = ir->base;
    TypeRef rtype = llvm_type(base);
    int index = ir->attr->obj.index;
    ir->elem = LLVMBuildStructGEP2(gb_ucg.builder, rtype, base->elem, index, ir->name);
}

void llvm_access(Ucg *ir) {
    TypeRef rtype = llvm_type(ucg_type_of(ir));
    Value index = ir->index->elem;
    ir->elem = LLVMBuildInBoundsGEP2(
        gb_ucg.builder, rtype, ir->base->elem, &index, 1, ir->name
    );
}

void llvm_sizeof(Ucg *ir) {
    Value size = LLVMSizeOf(llvm_type(ir->to_size));
    ir->elem =
        LLVMBuildIntCast2(gb_ucg.builder, size, gb_ucg.types.num[32], 0, "SIZEOF");
}

void llvm_typeof(Ucg *ir) {
    char *name = ucg_type_name(ir->to_type);
    ir->elem = LLVMBuildGlobalStringPtr(gb_ucg.builder, name, "TYPEOF");
}

void llvm_declare_global(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    assert(ir->name);
    ir->elem = LLVMAddGlobal(gb_ucg.module, ir->rtype, ir->name);
    LLVMSetInitializer(ir->elem, LLVMConstNull(ir->rtype));
}

void llvm_declare_param(Ucg *ir) {
    ir->rtype = llvm_type(ir->data_type);
    Value elem = llvm_alloc(ir->rtype, ir->name);

    LLVMBuildStore(gb_ucg.builder, ir->elem, elem);
    ir->elem = elem;
}


void llvm_load(Ucg *ir) {
    Ucg *to_dref = ir->to_dref;
    // to_dref->rtype;
    Value elem = to_dref->elem;
    TypeRef rtype = llvm_type(to_dref);
    char *name = ir->name;

    assert(elem);
    assert(name);
    // assert(rtype);
    ir->elem = LLVMBuildLoad2(gb_ucg.builder, rtype, elem, name);
    // ir->rtype = rtype;
}

void llvm_zero_init(Ucg *ir) {
    TypeRef rtype = ir->rtype;
    Value elem = ir->elem;
    LLVMBuildStore(gb_ucg.builder, LLVMConstNull(rtype), elem);
}

void llvm_op(Ucg *ir) {
    // Ucg *left = ir->left;
    // Ucg *right = ir->right;

    // clang-format off
    // Type ops[LAST + 1] = {
    //     [ADD_ASSIGN] = ADD, [SUB_ASSIGN] = SUB, [MUL_ASSIGN] = MUL,
    //     [DIV_ASSIGN] = DIV, [MOD_ASSIGN] = MOD,
    // };
    OpCode opcodes[LAST + 1] = {
        [ADD] = LLVMAdd,  [SUB] = LLVMSub,  [MUL] = LLVMMul,
        [DIV] = LLVMSDiv, [MOD] = LLVMSRem,
        [AND] = LLVMAnd,  [OR] = LLVMOr,
        [BAND] = LLVMAnd, [BOR] = LLVMOr,   [BXOR] = LLVMXor,
        [LSHIFT] = LLVMShl, [RSHIFT] = LLVMAShr,
    };
    LLVMIntPredicate predicates[LAST + 1] = {
        [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE,
        [LE] = LLVMIntSLE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
    };
    OpCode float_opcodes[LAST + 1] = {
        [ADD] = LLVMFAdd, [SUB] = LLVMFSub, [MUL] = LLVMFMul,
        [DIV] = LLVMFDiv, [MOD] = LLVMFRem,
    };
    LLVMRealPredicate float_predicates[LAST + 1] = {
        [GT] = LLVMRealOGT, [LT] = LLVMRealOLT, [GE] = LLVMRealOGE,
        [LE] = LLVMRealOLE, [EQ] = LLVMRealOEQ, [NQ] = LLVMRealUNE,
    };
    // clang-format on
    if (ir->is_uint) {
        opcodes[DIV] = LLVMUDiv;
        opcodes[MOD] = LLVMURem;
        opcodes[RSHIFT] = LLVMLShr;
        predicates[GT] = LLVMIntUGT;
        predicates[LT] = LLVMIntULT;
        predicates[GE] = LLVMIntUGE;
        predicates[LE] = LLVMIntULE;
    }
    Type type = ir->type;
    Value left = ir->left->elem;
    Value right = ir->right->elem;
    bool is_float = llvm_is_float(right);

    const char *name = to_string(ir->type); // node->token->llvm.name;
    Value value = NULL;
    if (is_float && float_opcodes[type])
        value = LLVMBuildBinOp(gb_ucg.builder, float_opcodes[type], left, right, name);
    else if (is_float && float_predicates[type])
        value =
            LLVMBuildFCmp(gb_ucg.builder, float_predicates[type], left, right, name);
    else if (!is_float && opcodes[type])
        value = LLVMBuildBinOp(gb_ucg.builder, opcodes[type], left, right, name);
    else if (!is_float && predicates[type])
        value = LLVMBuildICmp(gb_ucg.builder, predicates[type], left, right, name);
    else if (ir->type == ASSIGN) {
        value = LLVMBuildStore(gb_ucg.builder, right, left);
    } else {
        eprint("unknown operation %s\n", to_string(type));
        exit(1);
    }
    ir->elem = value;
}

bool llvm_is_float(Value value) {
    TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(value));
    return kind == LLVMFloatTypeKind || kind == LLVMDoubleTypeKind;
}

int llvm_bits(Value value) {
    TypeRef type = LLVMTypeOf(value);
    switch (LLVMGetTypeKind(type)) {
    case LLVMFloatTypeKind:
        return 32;
    case LLVMDoubleTypeKind:
        return 64;
    default:
        return LLVMGetIntTypeWidth(type);
    }
}

void llvm_cast(Ucg *ir) {
    Value value = ir->left->elem;
    Ucg *to = ir->to_type;
    bool from_pointer = LLVMGetTypeKind(LLVMTypeOf(value)) == LLVMPointerTypeKind;
    if (to->type == REF) {
        OpCode op = from_pointer ? LLVMBitCast : LLVMIntToPtr;
        ir->elem = LLVMBuildCast(gb_ucg.builder, op, value, llvm_type(to), "AS");
        return;
    }
    if (from_pointer) {
        ir->elem = LLVMBuildPtrToInt(gb_ucg.builder, value, llvm_type(to), "AS");
        return;
    }
    bool from_float = llvm_is_float(value);
    bool to_float = to->type == FLOAT;
    int from_bits = llvm_bits(value);
    int to_bits = to->shift;
    if (to->type == INT && to_bits == 1 && from_bits != 1) {
        Value zero = LLVMConstNull(LLVMTypeOf(value));
        if (from_float)
            ir->elem = LLVMBuildFCmp(gb_ucg.builder, LLVMRealUNE, value, zero, "AS");
        else
            ir->elem = LLVMBuildICmp(gb_ucg.builder, LLVMIntNE, value, zero, "AS");
        return;
    }

    if (from_float == to_float && from_bits == to_bits) {
        ir->elem = value;
        return;
    }
    bool from_uint = from_bits == 1 || ir->is_uint;
    OpCode op;
    if (from_float && to_float)
        op = to_bits > from_bits ? LLVMFPExt : LLVMFPTrunc;
    else if (from_float)
        op = to->is_uint ? LLVMFPToUI : LLVMFPToSI;
    else if (to_float)
        op = from_uint ? LLVMUIToFP : LLVMSIToFP;
    else if (to_bits > from_bits)
        op = from_uint ? LLVMZExt : LLVMSExt;
    else
        op = LLVMTrunc;
    ir->elem = LLVMBuildCast(gb_ucg.builder, op, value, llvm_type(to), "AS");
}

void llvm_neg(Ucg *ir) {
    Value value = ir->right->elem;
    if (llvm_is_float(value))
        ir->elem = LLVMBuildFNeg(gb_ucg.builder, value, "NEG");
    else
        ir->elem = LLVMBuildNeg(gb_ucg.builder, value, "NEG");
}

void llvm_not(Ucg *ir) {
    ir->elem = LLVMBuildNot(gb_ucg.builder, ir->right->elem, "NOT");
}

Value llvm_parent_bloc() {
    assert(gb_ucg.builder);
    assert(LLVMGetInsertBlock(gb_ucg.builder));
    return LLVMGetBasicBlockParent(LLVMGetInsertBlock(gb_ucg.builder));
}

Bloc llvm_bloc(Value fn, char *name) {
    return LLVMAppendBasicBlockInContext(gb_ucg.context, fn, name);
}

Bloc llvm_get_pos(void) {
    return LLVMGetInsertBlock(gb_ucg.builder);
}

void llvm_set_pos(Bloc bloc) {
    LLVMPositionBuilderAtEnd(gb_ucg.builder, bloc);
}

void llvm_jmp(Bloc bloc) {
    if (!is_bloc_terminated())
        LLVMBuildBr(gb_ucg.builder, bloc);
}

void llvm_cond(Ucg *ir) {
    assert(ir->then);

    if (ir->cond) {
        Ucg *cond = ir->cond;
        assert(cond->elem);
        Ucg *next = ir->next ? ir->next : ir->end;
        assert(next && next->bloc);
        LLVMBuildCondBr(gb_ucg.builder, cond->elem, ir->then, next->bloc);
    }
    llvm_set_pos(ir->then);
}

void ucg_clear(void) {
    memset(gb_ucg.insts, 0, gb_ucg.insts_count * sizeof(Ucg *));
    gb_ucg.insts_count = 0;
}

Ucg *ucg_new(Type type) {
    assert(type != 0);
    Ucg *new = arena_alloc(1, sizeof(Ucg));
    new->type = type;
    static int inst_index = 0;
    new->inst_index = inst_index;
    inst_index++;
    return new;
}

Ucg *ucg_int_type(int shift) {
    Ucg *new = ucg_new(INT);
    // printf(">>>> %d\n", shift);
    new->shift = shift;
    // printf(">>>> %d\n", new->shift);
    return new;
}

Ucg *ucg_uint_type(int shift) {
    Ucg *new = ucg_int_type(shift);
    new->is_uint = true;
    return new;
}

Ucg *ucg_i1_type() { // boolean
    return ucg_int_type(1);
}

Ucg *ucg_i8_type() {
    return ucg_int_type(8);
}

Ucg *ucg_char_type() {
    return ucg_i8_type();
}

Ucg *ucg_i32_type() {
    return ucg_int_type(32);
}

Ucg *ucg_bool_type() {
    return ucg_i1_type();
}

Ucg *ucg_void_type() {
    return ucg_new(VOID);
}

Ucg *ucg_ref_type(Ucg *to_ref) {
    Ucg *new = ucg_new(REF);
    new->to_ref = to_ref;
    return new;
}

Ucg *ucg_array_type(Ucg *elem, int count) {
    Ucg *new = ucg_new(ARRAY);
    new->arr.elem = elem;
    new->arr.items_count = count;
    return new;
}

Ucg *ucg_float_type(int shift) {
    Ucg *new = ucg_new(FLOAT);
    new->shift = shift;
    return new;
}

Ucg *ucg_float_value(int shift, double value) {
    Ucg *new = ucg_new(VAL_CREATE);

    new->to_create = ucg_float_type(shift);
    new->to_create->num.real = value;

    new->to_create->is_val = true;
    append(gb_ucg.insts, new->to_create);

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_int_value(int shift, long long value) {
    Ucg *new = ucg_new(VAL_CREATE);

    new->to_create = ucg_int_type(shift);
    new->to_create->num.value = value;

    new->to_create->is_val = true;
    append(gb_ucg.insts, new->to_create);

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_i1_value(long long value) {
    return ucg_int_value(1, value);
}

Ucg *ucg_i8_value(long long value) {
    return ucg_int_value(8, value);
}

Ucg *ucg_bool_value(long long value) {
    return ucg_i1_value(value);
}

Ucg *ucg_char_value(long long value) {
    return ucg_i8_value(value);
}

Ucg *ucg_i32_value(long long value) {
    return ucg_int_value(32, value);
}

Ucg *ucg_array_new(Ucg *elem) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = ucg_new(ARRAY);
    new->to_create->arr.elem = elem;
    return new;
}

__attribute__((sentinel)) Ucg *ucg_array_value(Ucg *elem, ...) {
    Ucg *new = ucg_array_new(elem);
    Ucg *to_create = new->to_create;
    va_list ap;
    va_start(ap, elem);
    for (Ucg *v = va_arg(ap, Ucg *); v; v = va_arg(ap, Ucg *))
        append(to_create->arr.items, v);
    va_end(ap);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_array_from(Ucg *elem, Ucg **items, int count) {
    Ucg *new = ucg_array_new(elem);
    for (int i = 0; i < count; i++)
        append(new->to_create->arr.items, ucg_value(items[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_value(char *str) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = ucg_new(CHARS);
    Ucg *to_create = new->to_create;
    for (int i = 0; i <= strlen(str); i++)
        append(to_create->arr.items, ucg_i8_value(str[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_array(char *str) {
    Ucg *new = ucg_array_new(ucg_char_type());
    Ucg *to_create = new->to_create;
    for (int i = 0; i <= strlen(str); i++)
        append(to_create->arr.items, ucg_i8_value(str[i]));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_type(void) {
    return ucg_ref_type(ucg_char_type());
}

Ucg *ucg_ref(Ucg *to_ref) {
    Ucg *new = ucg_new(REF);
    new->to_ref = to_ref;
    const char *name = to_ref->name ? to_ref->name : to_string(to_ref->type);
    new->name = format("&%s", name);

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_point_to(Ucg *ref) {
    Ucg *new = ucg_new(POINT_TO);
    new->to_point = ref;
    const char *name = ref->name ? ref->name : to_string(ref->type);
    new->name = format("*%s", name);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_dref(Ucg *var) {
    Ucg *new = ucg_new(DREF);
    new->to_dref = var;
    new->name = format("$%s", var->name ? var->name : to_string(var->type));

    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_proto(char *name, Ucg *ret, Ucg **params, int params_count) {
    Ucg *new = ucg_new(FN_DEC);
    new->name = name;
    new->fn.ret = ret;

    for (int i = 0; i < params_count; i++) {
        Ucg *param = params[i];
        if (param->type == VARIADIC)
            new->fn.varg = true;
        else
            append(new->fn.params, param);
    }
    append(gb_ucg.insts, new);
    return new;
}

void ucg_clean(Ucg *var) {
    gb_ucg.clean_start = gb_ucg.insts_count;
    gb_ucg.clean_var = var;
    gb_ucg.is_cleaning = true;
}

void ucg_clean_end(void) {
    Ucg *clean = ucg_new(CLEAN);
    clean->left = gb_ucg.clean_var;
    for (int i = gb_ucg.clean_start; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        if (inst->type == FN_CALL)
            inst->to_call->used--;
        append(clean->cleans, inst);
    }
    gb_ucg.insts_count = gb_ucg.clean_start;
    gb_ucg.is_cleaning = false;
    Ucg *block = gb_ucg.blocks[gb_ucg.blocks_count - 1];
    append(block->cleans, clean);
}

void ucg_run_cleans(Ucg *block, Ucg *moved) {
    for (int i = block->cleans_count - 1; i >= 0; i--) {
        Ucg *clean = block->cleans[i];
        if (moved && clean->left == moved)
            continue;
        for (int j = 0; j < clean->cleans_count; j++) {
            Ucg *inst = clean->cleans[j];
            if (inst->type == FN_CALL)
                inst->to_call->used++;
            append(gb_ucg.insts, inst);
        }
    }
}

bool ucg_ends_with_jump(void) {
    Ucg *last = gb_ucg.insts[gb_ucg.insts_count - 1];
    return last->type == RETURN || last->type == BRK || last->type == CNT;
}

void ucg_close_block(void) {
    Ucg *block = gb_ucg.blocks[gb_ucg.blocks_count - 1];
    gb_ucg.blocks_count--;
    if (!ucg_ends_with_jump())
        ucg_run_cleans(block, NULL);
}

bool ucg_is_block_of(Ucg *block, Ucg *target) {
    if (target == NULL)
        return block->type == FN_DEC;
    for (Ucg *stmt = target; stmt; stmt = stmt->next)
        if (stmt == block)
            return true;
    return false;
}

void ucg_leave_blocks(Ucg *target, Ucg *moved) {
    for (int i = gb_ucg.blocks_count - 1; i >= 0; i--) {
        Ucg *block = gb_ucg.blocks[i];
        ucg_run_cleans(block, moved);
        if (ucg_is_block_of(block, target))
            return;
    }
}

Ucg *ucg_entry(Ucg *fn) {
    Ucg *new = ucg_new(FN_ENTRY);
    new->to_enter = fn;
    append(gb_ucg.insts, new);
    append(gb_ucg.blocks, fn);
    return new;
}

Ucg *ucg_fn(char *name, Ucg *ret, Ucg **params, int params_count) {
    Ucg *new = ucg_proto(name, ret, params, params_count);
    ucg_entry(new);
    return new;
}

Ucg *ucg_fn_call(Ucg *fn_dec, Ucg **args, int args_count) {
    Ucg *new = ucg_new(FN_CALL);
    new->name = format("call.%s", fn_dec->name);
    new->to_call = fn_dec;
    fn_dec->used++;
    for (int i = 0; i < args_count; i++)
        append(new->fn.params, ucg_value(args[i]));
    append(gb_ucg.insts, new);
    if (gb_ucg.throws && !gb_ucg.is_cleaning)
        ucg_check(fn_dec);
    return new;
}

Ucg *ucg_varg_type(void) {
    return ucg_new(VARIADIC);
}

Ucg *ucg_get_param(Ucg *fn, int index) {
    return fn->fn.params[index];
}

Ucg *ucg_add_param(Ucg *fn, Ucg *param) {
    append(fn->fn.params, param);
    return param;
}

Ucg *ucg_ret(Ucg *ret) {
    Ucg *new = ucg_new(RETURN);
    new->to_ret = ucg_value(ret);
    // new->right = ret;
    // new->name = name;
    // new->fn.ret = ret;
    // new->fn.params = params;

    ucg_leave_blocks(NULL, ret);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_return() {
    Ucg *new = ucg_new(RETURN);
    ucg_leave_blocks(NULL, NULL);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_var(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(DECLARE);
    new->name = name;
    new->data_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_global(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(GLOBAL);
    new->name = name;
    new->data_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_runtime(char *name) {
    for (int i = 0; i < gb_ucg.runtime_count; i++) {
        if (strcmp2(gb_ucg.runtime[i]->name, name))
            return gb_ucg.runtime[i];
    }
    return NULL;
}

Ucg *ucg_extern(char *name, Ucg *ret, int count, ...) {
    Ucg *fn = ucg_runtime(name);
    if (fn)
        return fn;
    Ucg **params = arena_alloc(count + 1, sizeof(Ucg *));
    va_list ap;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        Ucg *type = va_arg(ap, Ucg *);
        params[i] = type->type == VARIADIC ? type : ucg_param("arg", type);
    }
    va_end(ap);
    fn = ucg_proto(name, ret, params, count);
    append(gb_ucg.runtime, fn);
    return fn;
}

Ucg *ucg_call(Ucg *fn, int count, ...) {
    Ucg **args = arena_alloc(count + 1, sizeof(Ucg *));
    va_list ap;
    va_start(ap, count);
    for (int i = 0; i < count; i++)
        args[i] = va_arg(ap, Ucg *);
    va_end(ap);
    return ucg_fn_call(fn, args, count);
}

Ucg *ucg_main_arg(char *name, Ucg *type) {
    Ucg *global = ucg_runtime(name);
    if (global == NULL) {
        global = ucg_new(GLOBAL);
        global->name = name;
        global->data_type = type;
        append(gb_ucg.runtime, global);
    }
    return ucg_dref(global);
}

Ucg *ucg_argc(void) {
    return ucg_main_arg("ucg.argc", ucg_i32_type());
}

Ucg *ucg_argv(void) {
    return ucg_main_arg("ucg.argv", ucg_ref_type(ucg_string_type()));
}

void ucg_main_args(void) {
    Ucg *argc = ucg_runtime("ucg.argc");
    Ucg *argv = ucg_runtime("ucg.argv");
    Ucg *main = NULL;
    bool is_used = false;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        bool is_arg = inst->to_dref == argc || inst->to_dref == argv;
        is_used = is_used || (inst->type == DREF && is_arg);
        if (inst->type == FN_DEC && strcmp2(inst->name, "main"))
            main = inst;
    }
    if (!is_used || main == NULL)
        return;
    if (argc)
        append(gb_ucg.insts, argc);
    if (argv)
        append(gb_ucg.insts, argv);
    if (main->fn.params_count != 0)
        return;
    append(main->fn.params, ucg_param("argc", ucg_i32_type()));
    append(main->fn.params, ucg_param("argv", ucg_ref_type(ucg_string_type())));
}

void llvm_main_args(Ucg *fn) {
    Ucg *args[] = { ucg_runtime("ucg.argc"), ucg_runtime("ucg.argv") };
    bool is_main = strcmp2(fn->name, "main") && fn->fn.params_count >= 2;
    for (int i = 0; is_main && i < 2; i++) {
        if (args[i] && args[i]->elem)
            LLVMBuildStore(gb_ucg.builder, LLVMGetParam(fn->elem, i), args[i]->elem);
    }
}

Ucg *ucg_null(Ucg *ref_type) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = ref_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_struct(char *name) {
    Ucg *new = ucg_new(STRUCT);
    new->name = name;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_attr(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(ATTR);
    new->name = name;
    new->data_type = data_type;
    return new;
}

void ucg_attrs(Ucg *st, Ucg **attrs, int attrs_count) {
    for (int i = 0; i < attrs_count; i++) {
        attrs[i]->obj.index = st->obj.attrs_count;
        append(st->obj.attrs, attrs[i]);
    }
}

Ucg *ucg_type_of(Ucg *place) {
    if (place->type == DOT)
        return place->attr->data_type;
    if (place->type == POINT_TO)
        return ucg_type_of(place->to_point)->to_ref;
    if (place->type == ACCESS)
        return ucg_value_type(place->base)->to_ref;
    return place->data_type;
}

Ucg *ucg_value_type(Ucg *value) {
    bool is_array = value->type == VAL_CREATE && value->to_create->type == ARRAY;
    if (is_array)
        return ucg_ref_type(value->to_create->arr.elem);
    switch (value->type) { // clang-format off
    case DREF:       return ucg_type_of(value->to_dref);
    case VAL_CREATE: return value->to_create;
    case REF:        return ucg_ref_type(ucg_type_of(value->to_ref));
    case FN_CALL:    return value->to_call->fn.ret;
    case AS:         return value->to_type;
    case SIZEOF:     return ucg_i32_type();
    case TYPEOF:     return ucg_string_type();
    case NEG:        return ucg_value_type(value->right);
    case LT: case GT: case EQ: case LE: case GE: case NQ: case NOT:
                     return ucg_i1_type();
    default:         return ucg_value_type(value->left); // clang-format on
    }
}

int ucg_layout(Ucg **values, int values_count, Ucg **args, int at) {
    int args_count = at;
    char *layout = "";
    for (int i = 0; i < values_count; i++) {
        Ucg *value = ucg_value(values[i]);
        char *text = ucg_string_text(value);
        if (text) {
            layout = format("%s%s", layout, text);
            value->used = -1;
            continue;
        }
        Ucg *type = ucg_value_type(value);
        int bits = type->shift;
        char *spec = "%s";
        if (type->type == FLOAT) {
            spec = "%f";
            value = ucg_cast(value, ucg_float_type(64));
        } else if (type->type == INT && bits == 1) {
            Ucg *text_var = ucg_var("bool.text", ucg_string_type());
            Ucg *stmt = ucg_if();
            ucg_then(stmt, value);
            ucg_assign(text_var, ucg_string_value("True"));
            ucg_then(ucg_else(stmt), NULL);
            ucg_assign(text_var, ucg_string_value("False"));
            ucg_end(stmt);
            value = text_var;
        } else if (type->type == INT && type->is_uint && bits == 64) {
            spec = "%llu";
        } else if (type->type == INT && type->is_uint) {
            spec = "%u";
            value = ucg_cast(value, ucg_uint_type(32));
        } else if (type->type == INT && bits == 21) {
            value = ucg_unic_text(value);
        } else if (type->type == INT && bits == 8) {
            spec = "%c";
            value = ucg_cast(value, ucg_i32_type());
        } else if (type->type == INT && bits == 64) {
            spec = "%lld";
        } else if (type->type == INT) {
            spec = "%d";
            value = ucg_cast(value, ucg_i32_type());
        }
        layout = format("%s%s", layout, spec);
        args[args_count++] = value;
    }
    args[at - 1] = ucg_string_value(layout);
    return args_count;
}

Ucg *ucg_write(Ucg *fd, Ucg **values, int values_count) {
    Ucg *i32 = ucg_i32_type();
    Ucg *str = ucg_string_type();
    Ucg *dprintf = ucg_extern("dprintf", i32, 3, i32, str, ucg_varg_type());
    Ucg **args = arena_alloc(values_count + 3, sizeof(Ucg *));
    int args_count = ucg_layout(values, values_count, args, 2);
    args[0] = fd;
    return ucg_fn_call(dprintf, args, args_count);
}

Ucg *ucg_format(Ucg *size, Ucg **values, int values_count) {
    Ucg *i32 = ucg_i32_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *str = ucg_string_type();
    Ucg *snprintf = ucg_extern("snprintf", i32, 4, str, i64, str, ucg_varg_type());
    Ucg **args = arena_alloc(values_count + 4, sizeof(Ucg *));
    int args_count = ucg_layout(values, values_count, args, 3);
    args[0] = ucg_null(str);
    args[1] = ucg_int_value(64, 0);
    ucg_assign(size, ucg_fn_call(snprintf, args, args_count));

    Ucg *cap = ucg_cast(ucg_op(ADD, size, ucg_i32_value(1)), i64);
    Ucg *text = ucg_var("format.text", str);
    ucg_assign(text, ucg_alloc(ucg_i8_type(), cap));
    Ucg **fill = arena_alloc(args_count + 1, sizeof(Ucg *));
    memcpy(fill, args, args_count * sizeof(Ucg *));
    fill[0] = text;
    fill[1] = cap;
    ucg_fn_call(snprintf, fill, args_count);
    return text;
}

Ucg *ucg_read_fn(void) {
    Ucg *str = ucg_string_type();
    Ucg *i32 = ucg_i32_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *line_ref = ucg_ref_type(str);
    Ucg *getline = ucg_extern("getline", i64, 3, line_ref, ucg_ref_type(i64), str);
    Ucg *find = ucg_extern("strchr", str, 2, str, i32);
    Ucg *copy = ucg_extern("strdup", str, 1, str);
    Ucg *open = ucg_extern("fdopen", str, 2, i32, str);
    Ucg *file = ucg_global("ura.read_file", str);
    Ucg *file_fd = ucg_global("ura.read_fd", i32);
    Ucg *fn = ucg_fn("ura.read", str, (Ucg *[]){ ucg_param("fd", i32) }, 1);
    append(gb_ucg.runtime, fn);
    Ucg *fd = ucg_get_param(fn, 0);

    Ucg *reopen = ucg_if();
    Ucg *is_closed = ucg_op(EQ, file, ucg_null(str));
    ucg_then(reopen, ucg_op(OR, is_closed, ucg_op(NQ, fd, file_fd)));
    ucg_assign(file, ucg_call(open, 2, fd, ucg_string_value("r")));
    ucg_assign(file_fd, fd);
    ucg_end(reopen);

    Ucg *line = ucg_var("line", str);
    Ucg *cap = ucg_var("cap", i64);
    Ucg *len = ucg_var("len", i64);
    ucg_assign(len, ucg_call(getline, 3, ucg_ref(line), ucg_ref(cap), file));

    Ucg *eof = ucg_if();
    ucg_then(eof, ucg_op(LT, len, ucg_int_value(64, 1)));
    ucg_ret(ucg_call(copy, 1, ucg_string_value("")));
    ucg_end(eof);

    Ucg *newline = ucg_var("newline", str);
    ucg_assign(newline, ucg_call(find, 2, line, ucg_i32_value('\n')));
    Ucg *found = ucg_if();
    ucg_then(found, ucg_op(NQ, newline, ucg_null(str)));
    ucg_assign(ucg_point_to(newline), ucg_char_value(0));
    ucg_end(found);
    ucg_ret(line);
    ucg_end(fn);
    return fn;
}

Ucg *ucg_read(Ucg *fd) {
    Ucg *fn = ucg_runtime("ura.read");
    return ucg_call(fn ? fn : ucg_read_fn(), 1, fd);
}

Ucg *ucg_unic_fn(void) {
    Ucg *i32 = ucg_i32_type();
    Ucg *str = ucg_string_type();
    Ucg *params[] = { ucg_param("code", i32), ucg_param("text", str) };
    Ucg *fn = ucg_fn("ura.unic", str, params, 2);
    append(gb_ucg.runtime, fn);
    Ucg *code = ucg_get_param(fn, 0);
    Ucg *text = ucg_get_param(fn, 1);

    // code < 128     -> [code]
    // code < 2048    -> [192 + code / 64,     128 + code % 64]
    // code < 65536   -> [224 + code / 4096,   128 + code / 64 % 64,   128 + code % 64]
    // code < 1114112 -> [240 + code / 262144, 128 + code / 4096 % 64, ...]
    // 233 (é)        -> [192 + 3, 128 + 41] = [C3, A9]
    int below[] = { 128, 2048, 65536, 1114112 };
    int marker[] = { 0, 192, 224, 240 };
    Ucg *stmt = ucg_if();
    for (int count = 1; count <= 4; count++) {
        Ucg *branch = count == 1 ? stmt : ucg_elif(stmt);
        ucg_then(branch, ucg_op(LT, code, ucg_i32_value(below[count - 1])));
        int place = 1;
        for (int i = count - 1; i >= 0; i--) {
            Ucg *digit = ucg_op(DIV, code, ucg_i32_value(place));
            Ucg *first = ucg_op(ADD, digit, ucg_i32_value(marker[count - 1]));
            Ucg *next = ucg_op(MOD, digit, ucg_i32_value(64));
            next = ucg_op(ADD, next, ucg_i32_value(128));
            Ucg *byte = ucg_cast(i == 0 ? first : next, ucg_char_type());
            ucg_assign(ucg_access(text, ucg_i32_value(i)), byte);
            place *= 64;
        }
        ucg_assign(ucg_access(text, ucg_i32_value(count)), ucg_char_value(0));
    }
    ucg_end(stmt);
    ucg_ret(text);
    ucg_end(fn);
    return fn;
}

Ucg *ucg_unic_text(Ucg *code) {
    Ucg *fn = ucg_runtime("ura.unic");
    if (fn == NULL)
        fn = ucg_unic_fn();
    Ucg *zero = ucg_char_value(0);
    Ucg *text = ucg_array_value(ucg_char_type(), zero, zero, zero, zero, zero, NULL);
    return ucg_call(fn, 2, ucg_cast(code, ucg_i32_type()), text);
}

Ucg *ucg_struct_value(Ucg *st, Ucg **values, int values_count) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->to_create = st;
    for (int i = 0; i < st->obj.attrs_count; i++) {
        bool has_value = i < values_count && values[i];
        append(new->arr.items, has_value ? ucg_value(values[i]) : NULL);
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_get_attr(Ucg *base, int index) {
    Ucg *attr = ucg_type_of(base)->obj.attrs[index];
    Ucg *new = ucg_new(DOT);
    new->base = base;
    new->attr = attr;
    const char *name = base->name ? base->name : to_string(base->type);
    new->name = format("%s.%s", name, attr->name);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_access(Ucg *base, Ucg *index) {
    Ucg *new = ucg_new(ACCESS);
    new->base = ucg_value(base);
    bool is_uint = ucg_is_uint(index);
    new->index = is_uint ? ucg_cast(index, ucg_uint_type(64)) : ucg_value(index);
    const char *name = base->name ? base->name : to_string(base->type);
    new->name = format("%s[]", name);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_param(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(PARAM);
    new->name = name;
    new->data_type = data_type;
    // append(gb_ucg.insts, new);
    return new;
}

bool is_locator(Ucg *ir) {
    bool is_variable = ir->type == DECLARE || ir->type == PARAM || ir->type == GLOBAL;
    bool is_part_of_variable =
        ir->type == DOT || ir->type == POINT_TO || ir->type == ACCESS;
    return is_variable || is_part_of_variable;
}

Ucg *ucg_value(Ucg *ir) {
    assert(ir);
    return is_locator(ir) ? ucg_dref(ir) : ir;
}

bool is_first_write(Ucg *var) {
    if (var->type != DECLARE)
        return false;
    for (int i = gb_ucg.insts_count - 1; i >= 0 && gb_ucg.insts[i] != var; i--) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) { // clang-format off
        case IF: case ELIF: case ELSE: case THEN: case LOOP: case END:
        case BRK: case CNT: case RETURN: case FN_ENTRY:
            return false;
        case DREF: if (curr->to_dref == var) return false; break;
        case DOT:  if (curr->base == var) return false; break;
        case REF:  if (curr->to_ref == var) return false; break;
        case POINT_TO: if (curr->to_point == var) return false; break;
        default:   break; // clang-format on
        }
    }
    return true;
}

bool ucg_is_uint(Ucg *value) {
    Ucg *type = is_locator(value) ? ucg_type_of(value) : ucg_value_type(value);
    return type->type == INT && type->is_uint;
}

Ucg *ucg_op(Type type, Ucg *left, Ucg *right) {
    assert(left);
    assert(right);
    bool is_uint = type != ASSIGN && (ucg_is_uint(left) || ucg_is_uint(right));
    Type ops[LAST + 1] = {
        [ADD_ASSIGN] = ADD,       [SUB_ASSIGN] = SUB,   [MUL_ASSIGN] = MUL,
        [DIV_ASSIGN] = DIV,       [MOD_ASSIGN] = MOD,   [BAND_ASSIGN] = BAND,
        [BOR_ASSIGN] = BOR,       [BXOR_ASSIGN] = BXOR, [LSHIFT_ASSIGN] = LSHIFT,
        [RSHIFT_ASSIGN] = RSHIFT,
    };

    if (type != ASSIGN && !ops[type])
        left = ucg_value(left);
    right = ucg_value(right);

    if (ops[type]) {
        type = ops[type];
        Ucg *binary_op = ucg_new(type);
        binary_op->left = ucg_dref(left);
        binary_op->right = right;
        binary_op->is_uint = is_uint;
        right = binary_op;
        append(gb_ucg.insts, binary_op);
        type = ASSIGN;
    }
    if (type == ASSIGN && is_first_write(left))
        left->is_set = true;
    Ucg *new = ucg_new(type);
    new->left = left;
    new->right = right;
    new->is_uint = is_uint;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_assign(Ucg *left, Ucg *right) {
    return ucg_op(ASSIGN, left, right);
}

Ucg *ucg_cast(Ucg *value, Ucg *type) {
    Ucg *new = ucg_new(AS);
    new->left = ucg_value(value);
    new->to_type = type;
    new->is_uint = ucg_is_uint(new->left);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_sizeof(Ucg *type) {
    Ucg *new = ucg_new(SIZEOF);
    new->to_size = type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_alloc(Ucg *type, Ucg *count) {
    Ucg *bytes = ucg_string_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *calloc = ucg_extern("calloc", bytes, 2, i64, i64);
    Ucg *size = ucg_cast(ucg_sizeof(type), i64);
    Ucg *memory = ucg_call(calloc, 2, ucg_cast(count, i64), size);
    return ucg_cast(memory, ucg_ref_type(type));
}

Ucg *ucg_copy(Ucg *dst, Ucg *src, Ucg *count) {
    Ucg *bytes = ucg_string_type();
    Ucg *i64 = ucg_int_type(64);
    Ucg *memcpy = ucg_extern("memcpy", bytes, 3, bytes, bytes, i64);
    dst = ucg_value(dst);
    Ucg *size = ucg_cast(ucg_sizeof(ucg_value_type(dst)->to_ref), i64);
    Ucg *to = ucg_cast(dst, bytes);
    Ucg *from = ucg_cast(src, bytes);
    return ucg_call(memcpy, 3, to, from, ucg_op(MUL, ucg_cast(count, i64), size));
}

Ucg *ucg_free(Ucg *memory) {
    Ucg *bytes = ucg_string_type();
    Ucg *free = ucg_extern("free", ucg_void_type(), 1, bytes);
    return ucg_call(free, 1, ucg_cast(memory, bytes));
}

char *ucg_type_name(Ucg *type) {
    switch (type->type) {
    case INT: {
        int bits = type->shift;
        if (bits == 1)
            return "bool";
        return format("%c%d", type->is_uint ? 'u' : 'i', bits);
    }
    case FLOAT:
        return format("f%d", type->shift);
    case REF:
        return format("%s[]", ucg_type_name(type->to_ref));
    case VOID:
        return "void";
    default:
        return type->name;
    }
}

Ucg *ucg_typeof(Ucg *type) {
    Ucg *new = ucg_new(TYPEOF);
    new->to_type = type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_neg(Ucg *value) {
    Ucg *new = ucg_new(NEG);
    new->right = ucg_value(value);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_not(Ucg *value) {
    Ucg *new = ucg_new(NOT);
    new->right = ucg_value(value);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_bloc(char *name) {
    Ucg *new = ucg_new(BLOC);
    new->name = name;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_branch(Type type, Ucg *prev_stmt) {
    Ucg *new = ucg_new(type);
    if (prev_stmt) {
        while (prev_stmt->next)
            prev_stmt = prev_stmt->next;
        prev_stmt->next = new;
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_loop() {
    return ucg_branch(LOOP, NULL);
}

Ucg *ucg_if(void) {
    return ucg_branch(IF, NULL);
}

Ucg *ucg_elif(Ucg *prev_stmt) {
    ucg_close_block();
    return ucg_branch(ELIF, prev_stmt);
}

Ucg *ucg_else(Ucg *prev_stmt) {
    ucg_close_block();
    return ucg_branch(ELSE, prev_stmt);
}

Ucg *ucg_then(Ucg *stmt, Ucg *cond) {
    Ucg *new = ucg_new(THEN);
    stmt->cond = cond ? ucg_value(cond) : NULL;
    new->stmt = stmt;
    append(gb_ucg.insts, new);
    append(gb_ucg.blocks, stmt);
    return new;
}

Ucg *ucg_end(Ucg *stmt) {
    ucg_close_block();
    Ucg *new = ucg_new(END);
    new->to_end = stmt;
    while (stmt) {
        stmt->end = new;
        stmt = stmt->next;
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_brk(Ucg *loop) {
    Ucg *new = ucg_new(BRK);
    new->stmt = loop;
    ucg_leave_blocks(loop, NULL);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_cnt(Ucg *loop) {
    ucg_leave_blocks(loop, NULL);
    Ucg *new = ucg_new(CNT);
    new->stmt = loop;
    append(gb_ucg.insts, new);
    return new;
}

void ucg_throws(void) {
    gb_ucg.throws = true;
    gb_ucg.thrown = ucg_global("ucg.thrown", ucg_bool_type());
}

Ucg *ucg_thrown(void) {
    return gb_ucg.thrown;
}

Ucg *ucg_error_type(void) {
    if (gb_ucg.error_type == NULL)
        gb_ucg.error_type = ucg_global("ucg.error_type", ucg_i32_type());
    return gb_ucg.error_type;
}

Ucg *ucg_type_id(Ucg *type) {
    Ucg *id = ucg_i32_value(0);
    append(gb_ucg.error_types, type);
    append(gb_ucg.error_ids, id);
    return id;
}

Ucg *ucg_error(Ucg *type) {
    if (gb_ucg.error == NULL)
        gb_ucg.error = ucg_global("ucg.error", ucg_int_type(64));
    Ucg *at = ucg_var("error.at", ucg_ref_type(type));
    ucg_assign(at, ucg_cast(ucg_ref(gb_ucg.error), ucg_ref_type(type)));
    return ucg_point_to(at);
}

Ucg *ucg_find_try(void) {
    for (int i = gb_ucg.blocks_count - 1; i >= 0; i--) {
        Ucg *block = gb_ucg.blocks[i];
        if (block->type == FN_DEC)
            return NULL;
        for (int j = 0; j < gb_ucg.tries_count; j++)
            if (gb_ucg.tries[j] == block)
                return block;
    }
    return NULL;
}

Ucg *ucg_goto_catch(void) {
    Ucg *try_stmt = ucg_find_try();
    Ucg *fn = NULL;
    for (int i = gb_ucg.blocks_count - 1; i >= 0 && fn == NULL; i--)
        if (gb_ucg.blocks[i]->type == FN_DEC)
            fn = gb_ucg.blocks[i];
    ucg_assign(ucg_thrown(), ucg_bool_value(0));
    ucg_leave_blocks(try_stmt, NULL);
    ucg_assign(ucg_thrown(), ucg_bool_value(1));
    Ucg *new = ucg_new(try_stmt ? BRK : RETURN);
    if (try_stmt)
        new->stmt = try_stmt;
    else if (fn->fn.ret->type != VOID)
        new->to_ret = ucg_value(ucg_null(fn->fn.ret));
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_throw(Ucg *value) {
    value = ucg_value(value);
    Ucg *type = ucg_value_type(value);
    ucg_assign(ucg_error(type), value);
    ucg_assign(ucg_error_type(), ucg_type_id(type));
    return ucg_goto_catch();
}

void ucg_check(Ucg *fn) {
    if (gb_ucg.blocks_count == 0)
        return;
    int start = gb_ucg.insts_count;
    Ucg *stmt = ucg_if();
    stmt->name = "thrown";
    ucg_then(stmt, ucg_thrown());
    ucg_goto_catch();
    ucg_end(stmt);
    Ucg *check = ucg_new(CHECK);
    check->to_call = fn;
    for (int i = start; i < gb_ucg.insts_count; i++)
        append(check->cleans, gb_ucg.insts[i]);
    gb_ucg.insts_count = start;
    append(gb_ucg.insts, check);
}

Ucg *ucg_try(void) {
    Ucg *stmt = ucg_if();
    stmt->name = "try";
    ucg_then(stmt, ucg_bool_value(1));
    append(gb_ucg.tries, stmt);
    return stmt;
}

Ucg *ucg_end_try(Ucg *try_stmt) {
    gb_ucg.tries_count--;
    return ucg_end(try_stmt);
}

Ucg *ucg_catch(Ucg *stmt, Ucg *type) {
    bool is_first = gb_ucg.tries_count && gb_ucg.tries[gb_ucg.tries_count - 1] == stmt;
    Ucg *catch_stmt = NULL;
    if (is_first) {
        ucg_end_try(stmt);
        catch_stmt = ucg_if();
    } else
        catch_stmt = ucg_elif(stmt);
    catch_stmt->name = "catch";
    Ucg *is_type = ucg_op(EQ, ucg_error_type(), ucg_type_id(type));
    ucg_then(catch_stmt, ucg_op(AND, ucg_thrown(), is_type));
    ucg_assign(ucg_thrown(), ucg_bool_value(0));
    return catch_stmt;
}

Ucg *ucg_end_catch(Ucg *catch_stmt) {
    Ucg *end = ucg_end(catch_stmt);
    Ucg *uncaught = ucg_if();
    uncaught->name = "rethrow";
    ucg_then(uncaught, ucg_thrown());
    ucg_goto_catch();
    ucg_end(uncaught);
    return end;
}

// void print_ucg(Ucg *inst, char end) {
//     char *text = "";

//     text = format("%s%s%s", text, to_string(inst->type), " ");
//     if (inst->name)
//         text = format("%s(%s)", text, inst->name ? inst->name : "");
//     else if (inst->is_val) {
//         switch (inst->type) { // clang-format off
//         case INT:   text = format("%s(%lld)", text, inst->num.value); break;
//         case FLOAT: text = format("%s(%f)", text, inst->num.real); break;
//         // clang-format on
//         default: {
//             eprint(
//                 "%s: %s:%d handle this case %s\n",
//                 RED("error"),
//                 FILE,
//                 LINE,
//                 to_string(inst->type)
//             );
//             exit(1);
//         }
//         }
//     }

//     text = format("[%03d] %-16s", inst->inst_index, text);
//     if (inst->left)
//         text = format("%s [%03d]", text, inst->left->inst_index);
//     if (inst->right)
//         text = format("%s [%03d]", text, inst->right->inst_index);
//     print("%s%c", text, end);
// }

void print_ucg(Ucg *inst, char end) {
    char *text = "";

    text = format("%s%s%s", text, to_string(inst->type), " ");
    if (inst->name)
        text = format("%s(%s)", text, inst->name ? inst->name : "");
    else if (inst->is_val) {
        switch (inst->type) { // clang-format off
        case INT:   text = format("%s(%lld)", text, inst->num.value); break;
        case FLOAT: text = format("%s(%f)", text, inst->num.real); break;
        // clang-format on
        default: {
            eprint(
                "%s: %s:%d handle this case %s\n",
                RED("error"),
                FILE,
                LINE,
                to_string(inst->type)
            );
            exit(1);
        }
        }
    }

    text = format("[%03d] %-16s", inst->inst_index, text);
    if (inst->left)
        text = format("%s [%03d]", text, inst->left->inst_index);
    if (inst->right)
        text = format("%s [%03d]", text, inst->right->inst_index);
    print("%s%c", text, end);
}

bool ucg_can_throw(Ucg *fn) {
    bool is_inside = false;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        if (inst->type == FN_ENTRY && inst->to_enter == fn)
            is_inside = true;
        if (!is_inside)
            continue;
        if (inst->type == END && inst->to_end == fn)
            return false;
        bool is_error = gb_ucg.error_type && inst->left == gb_ucg.error_type;
        bool is_throw = inst->type == ASSIGN && is_error;
        bool is_call = inst->type == CHECK && inst->to_call->is_throwing;
        if (is_throw || is_call)
            return true;
    }
    return false;
}

void ucg_find_throwing(void) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < gb_ucg.insts_count; i++) {
            Ucg *fn = gb_ucg.insts[i];
            if (fn->type != FN_DEC || fn->is_throwing || !ucg_can_throw(fn))
                continue;
            fn->is_throwing = true;
            changed = true;
        }
    }
}

void ucg_splice_checks(void) {
    int total = gb_ucg.insts_count;
    for (int i = 0; i < gb_ucg.insts_count; i++)
        total += gb_ucg.insts[i]->cleans_count;
    Ucg **insts = arena_alloc(total + 2, sizeof(Ucg *));
    int count = 0;
    ucg_find_throwing();
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        if (inst->type != CHECK) {
            insts[count++] = inst;
            continue;
        }
        bool is_throwing = inst->to_call->is_throwing;
        for (int j = 0; j < inst->cleans_count; j++) {
            Ucg *part = inst->cleans[j];
            if (is_throwing)
                insts[count++] = part;
            else if (part->type == FN_CALL)
                part->to_call->used--;
        }
    }
    gb_ucg.insts = insts;
    gb_ucg.insts_count = count;
    gb_ucg.insts_size = total + 2;
}

void ucg_number_errors(void) {
    if (gb_ucg.error == NULL)
        return;
    TargetData layout = LLVMCreateTargetDataLayout(gb_ucg.machine);
    TypeRef *seen = arena_alloc(gb_ucg.error_types_count + 1, sizeof(TypeRef));
    int seen_count = 0;
    unsigned long long size = 8;
    for (int i = 0; i < gb_ucg.error_types_count; i++) {
        TypeRef type = llvm_type(gb_ucg.error_types[i]);
        int id = 0;
        while (id < seen_count && seen[id] != type)
            id++;
        if (id == seen_count)
            seen[seen_count++] = type;
        gb_ucg.error_ids[i]->to_create->num.value = id + 1;
        unsigned long long type_size = LLVMABISizeOfType(layout, type);
        if (type_size > size)
            size = type_size;
    }
    gb_ucg.error->data_type = ucg_array_type(ucg_int_type(64), (size + 7) / 8);
    LLVMDisposeTargetData(layout);
}

void ucg_optimize(void) {
    bool removed = true;
    while (removed) {
        removed = false;
        for (int i = 0; i < gb_ucg.insts_count; i++) {
            Ucg *fn = gb_ucg.insts[i];
            bool is_unused = fn->type == FN_DEC && fn->used == 0;
            if (!is_unused || strcmp2(fn->name, "main"))
                continue;
            fn->used = -1;
            removed = true;
            bool is_inside = false;
            for (int j = i + 1; j < gb_ucg.insts_count; j++) {
                Ucg *inst = gb_ucg.insts[j];
                if (inst->type == FN_ENTRY && inst->to_enter == fn)
                    is_inside = true;
                if (!is_inside)
                    continue;
                if (inst->type == END && inst->to_end == fn)
                    break;
                if (inst->type == FN_CALL)
                    inst->to_call->used--;
            }
        }
    }
    int count = 0;
    Ucg *removed_fn = NULL;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *inst = gb_ucg.insts[i];
        bool is_removed = inst->type == FN_DEC && inst->used < 0;
        if (inst->type == FN_ENTRY && inst->to_enter->used < 0 && !removed_fn)
            removed_fn = inst->to_enter;
        bool is_body_end =
            removed_fn && inst->type == END && inst->to_end == removed_fn;
        bool is_module_level =
            inst->type == FN_DEC || inst->type == GLOBAL || inst->type == STRUCT;
        if ((!removed_fn || is_module_level) && !is_removed)
            gb_ucg.insts[count++] = inst;
        if (is_body_end)
            removed_fn = NULL;
    }
    gb_ucg.insts_count = count;
}

void ucg_compile() {
    ucg_splice_checks();
    ucg_optimize();
    ucg_main_args();
    Value fn = NULL;
    expand(Value, outer_fns);
    zero_init(outer_fns);
    int stmt_index = 0;
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        if (curr->type == STRUCT)
            llvm_struct_create(curr);
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        if (curr->type == STRUCT)
            llvm_struct_body(curr);
    }
    ucg_number_errors();
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        switch (curr->type) { // clang-format off
        case FN_DEC: llvm_fn_dec(curr); break;
        case GLOBAL: llvm_declare_global(curr); break; // clang-format on
        case FN_ENTRY: {
            append(outer_fns, fn);
            fn = curr->to_enter->elem;
            curr->bloc = llvm_bloc(fn, "entry");
            break;
        }
        case LOOP: {
            curr->bloc = llvm_bloc(fn, format("while%d", stmt_index));
            curr->then = llvm_bloc(fn, format("while%d.then", stmt_index));
            stmt_index++;
            break;
        }
        case IF: {
            char *kind = curr->name ? curr->name : "if";
            curr->bloc = llvm_bloc(fn, format("%s%d", kind, stmt_index));
            curr->then = llvm_bloc(fn, format("%s%d.then", kind, stmt_index));
            stmt_index++;
            break;
        }
        case ELIF: {
            char *kind = curr->name ? curr->name : "elif";
            curr->bloc = llvm_bloc(fn, format("%s%d", kind, stmt_index));
            curr->then = llvm_bloc(fn, format("%s%d.then", kind, stmt_index));
            stmt_index++;
            break;
        }
        case ELSE: {
            curr->bloc = llvm_bloc(fn, format("else%d", stmt_index));
            curr->then = curr->bloc;
            stmt_index++;
            break;
        }
        case END: {
            Ucg *to_end = curr->to_end;
            if (to_end->type == FN_DEC) {
                fn = outer_fns[--outer_fns_count];
                break;
            }
            char *name = to_end->type == LOOP ? "endloop" : "endif";
            if (to_end->name)
                name = format("end%s", to_end->name);
            curr->bloc = llvm_bloc(fn, name);
            // stmt_index++;
            break;
        }
        default: break;
        }
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        // print_ucg(curr, '\n');
        switch (curr->type) { // clang-format off
        case INT:
        case FLOAT:      break;
        case FN_DEC:     break;
        case GLOBAL:     break;
        case STRUCT:     break;
        case RETURN:     llvm_ret(curr->to_ret); break;
        case FN_CALL:    llvm_fn_call(curr); break;
        case VAL_CREATE: llvm_val_create(curr); break;
        case DECLARE:    llvm_declare_var(curr); break;
        case DREF:       llvm_load(curr); break;
        case THEN:       llvm_cond(curr->stmt); break;
        case BRK:        llvm_jmp(curr->stmt->end->bloc); break;
        case CNT:        llvm_jmp(curr->stmt->bloc); break;
        case NEG:        llvm_neg(curr); break;
        case NOT:        llvm_not(curr); break;
        case AS:         llvm_cast(curr); break;
        case DOT:        llvm_get_attr(curr); break;
        case ACCESS:     llvm_access(curr); break;
        case SIZEOF:     llvm_sizeof(curr); break;
        case TYPEOF:     llvm_typeof(curr); break;
        case REF:        llvm_ref(curr); break;
        case POINT_TO:   llvm_point_to(curr); break;
        case ASSIGN:
        case ADD: case SUB: case MUL: case DIV: case MOD:         // math
        case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: // bitwise
        case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:        // composite
        case DIV_ASSIGN: case MOD_ASSIGN:
        case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
        case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
        case LT: case GT: case EQ: case LE: case GE: case NQ:     // comparison
        case AND: case OR:                                        // logic
            llvm_op(curr);
            break;
        // clang-format on
        case FN_ENTRY: {
            curr->to_enter->caller_bloc = llvm_get_pos();
            llvm_set_pos(curr->bloc);
            Ucg *fn = curr->to_enter;
            // assert()
            for (int i = 0; i < fn->fn.params_count; i++) {
                Ucg *param = fn->fn.params[i];
                llvm_declare_param(param);
            }
            llvm_main_args(fn);

            break;
        } // clang-format off
        case IF: case ELIF: case ELSE: { // clang-format on
            assert(curr->end);

            llvm_jmp(curr->type == IF ? curr->bloc : curr->end->bloc);
            llvm_set_pos(curr->bloc);
            break;
        }
        case LOOP: {
            llvm_jmp(curr->bloc);
            llvm_set_pos(curr->bloc);
            break;
        }
        case END: {
            // position at
            Ucg *stmt = curr->to_end;
            if (stmt->type == FN_DEC) {
                llvm_end_fn(stmt);
                break;
            }
            llvm_jmp(stmt->type == LOOP ? stmt->bloc : curr->bloc);
            llvm_set_pos(curr->bloc);
            break;
        }
        default: {
            eprint(
                "%s: %s:%d handle this case %s\n",
                RED("error"),
                FILE,
                LINE,
                to_string(curr->type)
            );
            // exit(1);
            break;
        }
        }
    }
}

void print_ucgs(void) {
    for (size_t i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *curr = gb_ucg.insts[i];
        print_ucg(curr, '\n');
    }
}

void ucg_path(char *path) {
    gb_ucg.ll_path = path;
    gb_ucg.dir = dirname2(path);
}

void ucg_bin_path(char *path) {
    gb_ucg.exe_path = path;
}

void ucg_asm_path(char *path) {
    gb_ucg.asm_path = path;
}

void ucg_obj_path(char *path) {
    gb_ucg.obj_path = path;
}

char *ucg_find_lib_in(char *dir, char *name) {
    DIR *d = dir ? opendir(dir) : NULL;
    if (d == NULL)
        return NULL;
    char *prefix = format("lib%s", name);
    size_t len = strlen(prefix);
    char *found = NULL;
    for (struct dirent *entry = readdir(d); entry && !found; entry = readdir(d)) {
        char *file = entry->d_name;
        if (!strncmp2(file, prefix, len))
            continue;
        if (file[len] != '.' && file[len] != '-')
            continue;
        if (!strstr(file, ".so") && !strstr(file, ".dylib"))
            continue;
        found = format("%s/%s", dir, file);
    }
    closedir(d);
    return found;
}

char *ucg_find_lib(char *name) {
    char *dirs[] = {
        getenv("URA_LIBC"),
        "/usr/local/lib",
        "/opt/homebrew/lib",
        "/usr/lib/x86_64-linux-gnu",
        "/usr/lib/aarch64-linux-gnu",
        "/usr/lib64",
        "/usr/lib",
    };
    for (size_t i = 0; i < sizeof(dirs) / sizeof(*dirs); i++) {
        char *path = ucg_find_lib_in(dirs[i], name);
        if (path)
            return path;
    }
    return NULL;
}

void ucg_link(char *path) {
    append(gb_ucg.libs, path);
}


// UTILS
const char *to_string(Type type) {
#define NAME(t) [t] = #t
    char *types[LAST + 1] = { // clang-format off
        NAME(ERR),
        NAME(ID),            NAME(SCOPE),         NAME(URA),

        NAME(VAR),           NAME(DECLARE),       NAME(INIT_VAR),
        NAME(ARG),           NAME(PARAM),         NAME(GLOBAL),

        NAME(VAL_CREATE),    NAME(TEMPLATE),      NAME(TEMPLATE_TYPE),
        NAME(TYPES),

        NAME(ANY),           NAME(VOID),          NAME(INT),
        NAME(FLOAT),         NAME(CHARS),         NAME(REF),
        NAME(POINT_TO),      NAME(ZERO),

        NAME(STRUCT),        NAME(DOT),           NAME(ATTR),
        NAME(ENUM),          NAME(DREF),

        NAME(LPARENT),       NAME(RPARENT),       NAME(LBRACK),
        NAME(RBRACK),        NAME(LCBRACK),       NAME(RCBRACK),
        NAME(DOTS),          NAME(DOUBLE_DOTS),   NAME(COMA),
        NAME(OPERATOR),      NAME(PUB),

        NAME(ARRAY),         NAME(ARRAY_LIT),     NAME(ACCESS),

        NAME(ASSIGN),        NAME(ADD_ASSIGN),    NAME(SUB_ASSIGN),
        NAME(MUL_ASSIGN),    NAME(DIV_ASSIGN),    NAME(MOD_ASSIGN),
        NAME(BAND_ASSIGN),   NAME(BOR_ASSIGN),    NAME(BXOR_ASSIGN),
        NAME(LSHIFT_ASSIGN), NAME(RSHIFT_ASSIGN),

        NAME(ADD),           NAME(SUB),           NAME(MUL),
        NAME(DIV),           NAME(MOD),
        NAME(BAND),          NAME(BOR),           NAME(BXOR),
        NAME(LSHIFT),        NAME(RSHIFT),

        NAME(GT),            NAME(LT),            NAME(GE),
        NAME(LE),            NAME(EQ),            NAME(NQ),

        NAME(AND),           NAME(OR),            NAME(NOT),
        NAME(NEG),           NAME(BNOT),

        NAME(PROTO),         NAME(FN_ENTRY),      NAME(FN_DEC),
        NAME(ARGS),          NAME(VARIADIC),      NAME(FN_CALL),
        NAME(RETURN),        NAME(FORMAT),
        NAME(LINK),          NAME(USE),
        NAME(MACRO_IF),      NAME(MACRO_ELIF),    NAME(MACRO_ELSE),
        NAME(MACRO_ERROR),   NAME(MACRO_NAME),    NAME(MACRO_VERSION),
        NAME(MACRO_ENTRY),   NAME(MACRO_SCRIPT),  NAME(MACRO_DEPENDENCY),
        NAME(MACRO_URL),     NAME(MACRO_PATH),    NAME(MACRO_CMD),
        NAME(SIZEOF),        NAME(TYPEOF),        NAME(DROP),
        NAME(COPY),          NAME(ALLOC),         NAME(MEMCPY),
        NAME(FREE),          NAME(IS_ZERO),

        NAME(IF),            NAME(ELIF),          NAME(ELSE),
        NAME(THEN),          NAME(END),           NAME(BLOC),
        NAME(LOOP),          NAME(BRK),           NAME(CNT),
        NAME(MATCH),         NAME(CASE),          NAME(DEFAULT),
        NAME(CLEAN),
        NAME(TRY),           NAME(CATCH),         NAME(THROW),
        NAME(CHECK),

        NAME(AS),            NAME(FROM),          NAME(AND_LIT),
        NAME(OR_LIT),        NAME(SELF),          NAME(FOR),
        NAME(IN),            NAME(MODULE),        NAME(ARGC),
        NAME(ARGV),          NAME(LINE_NUMBER),   NAME(FILE_NAME),
        NAME(FN_NAME),
        NAME(LAST),
    }; // clang-format on
#undef NAME

    if (types[type] == NULL)
        return format("UNKNOWN(%d)", type);

    return types[type];
}

// PROJECT
// CONFIG
Node *find_config(Node *config, char *dir) {
    if (config == NULL || strcmp2(config->token->name, dir))
        return config;
    for (int i = 0; i < config->children_count; i++) {
        Node *node = config->children[i];
        bool is_loaded = node->token->type == MACRO_DEPENDENCY && node->children_count;
        Node *found = is_loaded ? find_config(node->children[0], dir) : NULL;
        if (found)
            return found;
    }
    return NULL;
}

Node *parse_config(char *dir, bool is_fresh_clone) {
    dir = realpath2(dir);
    File *file = new_file(format("%s/ura.conf", dir));
    file->start = end_of_sources();
    open_file(file);
    append(ura.configs, file);

    int first = ura.tokens_count;
    int line = ura.line;
    tokenize(file);
    new_token(LAST, (Token){})->space = -TAB;

    Node *config = new_node(new_token(SCOPE, (Token){}));
    config->token->name = dir;
    ura.exe_pos = first;
    enter_scope(config);
    while (peek(0)->type != LAST)
        append(config->children, parse_expr(0));
    exit_scope();

    if (is_fresh_clone && ucg_has_errors())
        remove2(dir);
    ucg_check_errors();

    ura.tokens_count = first;
    ura.exe_pos = 0;
    ura.line = line;
    return config;
}

Node *load_config(char *dir) {
    if (ura.config == NULL)
        ura.config = parse_config(dir, false);
    return ura.config;
}

char *entry_value(Node *node, int index) {
    return node->children[index]->token->chars.value;
}

char *config_value(Node *config, Type macro, char *name) {
    for (int i = 0; i < config->children_count; i++) {
        Node *node = config->children[i];
        if (node->token->type != macro)
            continue;
        if (name == NULL && node->children_count)
            return entry_value(node, 0);
        if (name && strcmp2(node->token->name, name))
            return entry_value(node->cmd, 0);
    }
    return NULL;
}

char *dependency_dir(char *project, Node *dependency) {
    char *where = entry_value(dependency->url, 0);
    Node *version = dependency->url->version;
    bool is_local = dependency->url->token->type == MACRO_PATH;

    if (is_local)
        return where[0] == '/' ? where : format("%s/%s", project, where);
    if (version == NULL)
        return NULL;

    char *name = dependency->token->name;
    return format("%s/deps/%s@%s", project, name, entry_value(version, 0));
}

Node *find_dependency_node(Node *config, char *name) {
    for (int i = 0; i < config->children_count; i++) {
        Node *node = config->children[i];
        bool is_dependency = node->token->type == MACRO_DEPENDENCY;
        if (is_dependency && strcmp2(node->token->name, name))
            return node;
    }
    return NULL;
}

void load_project(Node *config) {
    char *dir = config->token->name;
    for (int i = 0; i < config->children_count; i++) {
        Node *node = config->children[i];
        if (node->token->type != MACRO_DEPENDENCY)
            continue;

        char *folder = dependency_dir(dir, node);
        bool ok = folder && access(folder, F_OK) == 0;
        if (!assert_installed(ok, node, folder))
            continue;

        bool has_config = access(format("%s/ura.conf", folder), R_OK) == 0;
        if (!has_config || find_config(ura.config, realpath2(folder)))
            continue;
        append(node->children, parse_config(folder, false));
        load_project(node->children[0]);
    }
    ucg_check_errors();
}

// DEPENDENCIES
bool is_version(char *text) {
    int major, minor, patch, end = 0;
    int count = sscanf(text, "%d.%d.%d%n", &major, &minor, &patch, &end);
    return isdigit(text[0]) && count == 3 && text[end] == '\0';
}

char *latest_version(char *url) {
    char *tags = format("%s/.ura/tags", home2());
    mkdir2(format("%s/.ura", home2()));
    char *ls[] = { "git", "ls-remote", "--tags", "--refs", url, NULL };
    bool ok = run_command(ls, tags) == 0;
    assert_true(ok, format("can't read the versions of %s", url), NULL);

    File *file = new_file(tags);
    open_file(file);
    char *latest = NULL;
    long best = -1;
    char *tag = file->buff;
    while ((tag = strstr(tag, "refs/tags/"))) {
        tag += strlen("refs/tags/");
        char *name = strndup2(tag, strcspn(tag, "\r\n"));
        int major = 0, minor = 0, patch = 0;
        sscanf(name, "%d.%d.%d", &major, &minor, &patch);
        long rank = (major * 1000L + minor) * 1000L + patch;
        if (!is_version(name) || rank <= best)
            continue;
        latest = name;
        best = rank;
    }

    ok = latest != NULL;
    assert_true(ok, format("%s has no version tag like 0.0.1", url), NULL);
    return latest;
}

int install_dependency(char *project, Node *dependency, bool is_update, int depth) {
    char *name = dependency->token->name;
    char *where = entry_value(dependency->url, 0);
    Node *pinned = dependency->url->version;
    bool is_local = dependency->url->token->type == MACRO_PATH;
    char *version = pinned && !is_update ? entry_value(pinned, 0) : NULL;

    bool is_unpinned = !is_local && version == NULL;
    if (is_unpinned)
        version = latest_version(where);
    bool is_same = pinned && version && strcmp2(entry_value(pinned, 0), version);
    if (is_unpinned && !is_same) {
        set_dependency(project, name, "@url", where, version);
        help("%s %s written in %s/ura.conf\n", name, version, project);
    }

    char *folder = dependency_dir(project, dependency);
    char *cache = NULL;
    if (!is_local) {
        folder = format("%s/deps/%s@%s", project, name, version);
        cache = format("%s/.ura/deps/%s/%s", home2(), name, version);
    }
    bool is_missing = access(folder, F_OK) != 0;
    bool is_new = cache && access(cache, F_OK) != 0;
    bool ok = !is_local || !is_missing;
    assert_installed(ok, dependency, folder);
    ucg_check_errors();

    if (is_new) {
        char *clone[] = { "git",      "clone", "-q",  "--depth", "1",
                          "--branch", version, where, cache,     NULL };
        ok = run_command(clone, NULL) == 0;
        assert_true(ok, format("can't get %s %s from %s", name, version, where), NULL);
        help("installed %s %s\n", name, version);
    }
    if (is_missing) {
        mkdir2(format("%s/deps", project));
        symlink2(cache, folder);
    }

    bool has_config = access(format("%s/ura.conf", folder), R_OK) == 0;
    if (has_config)
        append(dependency->children, parse_config(folder, is_new));
    int installed = is_missing;
    if (has_config && depth < 16)
        installed += install_project(dependency->children[0], NULL, false, depth + 1);
    return installed;
}

int install_project(Node *config, char *name, bool is_update, int depth) {
    int count = 0;
    int installed = 0;
    for (int i = 0; i < config->children_count; i++) {
        Node *node = config->children[i];
        if (node->token->type != MACRO_DEPENDENCY)
            continue;
        if (name && !strcmp2(node->token->name, name))
            continue;
        installed += install_dependency(config->token->name, node, is_update, depth);
        count++;
    }

    bool ok = name == NULL || count > 0;
    assert_true(
        ok, format("'%s' is not a dependency in ura.conf", name ? name : ""), NULL
    );
    if (depth == 0 && count == 0)
        help("no dependencies in ura.conf\n");
    else if (depth == 0 && installed == 0)
        help("every dependency is installed\n");
    return installed;
}

void set_dependency(
    char *project, char *name, char *field, char *where, char *version
) {
    File *file = new_file(format("%s/ura.conf", project));
    open_file(file);
    char *entry = format("@dep \"%s\"", name);
    char *block = "";
    if (field)
        block = format("%s:\n    %s \"%s\"\n", entry, field, where);
    if (field && version)
        block = format("%s    @version \"%s\"\n", block, version);

    char *out = "";
    bool is_inside = false;
    bool is_found = false;
    char *line = file->buff;
    while (*line) {
        char *newline = strchr(line, '\n');
        size_t len = newline ? (size_t)(newline - line + 1) : strlen(line);
        char *row = strndup2(line, len);
        line += len;
        bool is_indented = row[0] == ' ' || row[0] == '\t';
        if (is_inside && is_indented)
            continue;
        is_inside = strncmp2(row, entry, strlen(entry));
        is_found = is_found || is_inside;
        out = format("%s%s", out, is_inside ? block : row);
    }

    bool needs_newline = out[0] && !ends_with(out, "\n");
    if (!is_found)
        out = format("%s%s%s", out, needs_newline ? "\n" : "", block);
    write_file(file->path, out);
}

void add_dependency(char *project, char *spec, char *where) {
    char *at = strchr(spec, '@');
    char *name = at ? strndup2(spec, at - spec) : spec;
    char *version = at ? at + 1 : NULL;
    bool is_local = where && (where[0] == '.' || where[0] == '/');
    bool ok = name[0] && !strchr(name, '/') && !strstr(name, "..");
    assert_true(ok, format("'%s' is not a package name", name), NULL);
    ok = !is_local || !version;
    assert_true(
        ok,
        format("%s is a folder, it has no version", where),
        format("ura deps add %s %s", name, where)
    );
    if (where == NULL)
	where = format("https://github.com/ura-computing/ura-%s", name);
	print("installing %s\n", name);
	print("from %s\n", where);
    set_dependency(project, name, is_local ? "@path" : "@url", where, version);
    help("added %s to ura.conf\n", spec);
    install_project(load_config(project), NULL, false, 0);
}

void remove_dependency(char *project, char *spec) {
    char *at = strchr(spec, '@');
    char *name = at ? strndup2(spec, at - spec) : spec;
    char *version = at ? at + 1 : NULL;

    Node *config = load_config(project);
    Node *node = find_dependency_node(config, name);
    char *cache = format("%s/.ura/deps/%s", home2(), name);
    bool ok = node || access(cache, F_OK) == 0;
    assert_true(ok, format("'%s' is not a dependency in ura.conf", name), NULL);

    char **versions = version ? (char *[]){ version, NULL } : readdir2(cache);
    for (int i = 0; versions[i]; i++) {
        remove2(format("%s/%s", cache, versions[i]));
        remove2(format("%s/deps/%s@%s", config->token->name, name, versions[i]));
    }
    if (version == NULL)
        remove2(cache);

    Node *pinned = node ? node->url->version : NULL;
    bool is_pinned = version && pinned && strcmp2(entry_value(pinned, 0), version);
    bool is_listed = node && (version == NULL || is_pinned);
    if (is_listed)
        set_dependency(config->token->name, name, NULL, NULL, NULL);
    help("removed %s\n", spec);
}

void list_project(Node *config, char *indent, int depth) {
    if (depth == 0) {
        char *name = config_value(config, MACRO_NAME, NULL);
        char *version = config_value(config, MACRO_VERSION, NULL);
        help("%s %s\n", name ? name : "(no @name)", version ? version : "");
    }

    Node *last = NULL;
    for (int i = 0; i < config->children_count; i++) {
        if (config->children[i]->token->type == MACRO_DEPENDENCY)
            last = config->children[i];
    }

    for (int i = 0; i < config->children_count; i++) {
        Node *node = config->children[i];
        if (node->token->type != MACRO_DEPENDENCY)
            continue;

        Node *version = node->url->version;
        bool is_local = node->url->token->type == MACRO_PATH;
        char *folder = dependency_dir(config->token->name, node);
        bool is_missing = folder == NULL || access(folder, F_OK) != 0;
        bool has_config =
            !is_missing && access(format("%s/ura.conf", folder), R_OK) == 0;
        bool is_last = node == last;

        help(
            "%s%s%s %s  %s  %s%s\n",
            indent,
            is_last ? "└── " : "├── ",
            node->token->name,
            version ? entry_value(version, 0) : "",
            is_local ? "folder" : entry_value(node->url, 0),
            folder ? folder : "",
            is_missing ? "  missing" : ""
        );

        if (has_config && depth < 16)
            list_project(
                parse_config(folder, false),
                format("%s%s", indent, is_last ? "    " : "│   "),
                depth + 1
            );
    }
}

// SCRIPTS AND PACKAGES
void run_script(char *name) {
    char *project = find_project();
    char *script = config_value(load_config(project), MACRO_SCRIPT, name);
    bool ok = script != NULL;
    assert_true(
        ok,
        format("no @script \"%s\" in ura.conf", name),
        format("add: @script \"%s\":\n          @cmd \"<command>\"", name)
    );

    ok = chdir(project) == 0;
    assert_true(ok, format("can't enter %s", project), NULL);
    int status = system(script);
    exit(WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

char *find_package(char *name, File *from) {
    char *dir = dirname2(from->path);
    if (!strncmp2(name, "@/", 2)) {
        char *path = format("%s/%s.ura", dir, name);
        return access(path, R_OK) == 0 ? path : NULL;
    }

    char *project = find_folder_with(dir, "ura.conf");
    Node *config = project ? find_config(ura.config, realpath2(project)) : NULL;
    if (config == NULL)
        return NULL;

    char *slash = strchr(name + 2, '/');
    char *dependency = slash ? strndup2(name + 2, slash - name - 2) : name + 2;
    Node *node = find_dependency_node(config, dependency);
    if (node == NULL)
        return NULL;

    char *folder = dependency_dir(config->token->name, node);
    Node *own = find_config(ura.config, realpath2(folder));
    char *entry = own ? config_value(own, MACRO_ENTRY, NULL) : NULL;
    char *file = slash ? format("%s.ura", slash + 1) : entry;
    if (file == NULL)
        return NULL;
    char *path = format("%s/%s", folder, file);
    return access(path, R_OK) == 0 ? path : NULL;
}

// INIT
char *compile_flags[][2] = {
    { "-o <exe>", "name the executable (default build/exe.out)" },
    { "-ll [path]", "also write the LLVM IR (default build/out.ll)" },
    { "-S [path]", "write the assembly, no executable (default build/out.s)" },
    { "-c [path]", "write the object file, no executable (default build/out.o)" },
    { NULL, NULL },
};

char *template_files[][2] = {
    { "main.ura",
      "use \"@/core\"\n"
      "use \"./src/user\"\n\n"
      "fn main() i32:\n"
      "    name String = user()\n"
      "    if name.is_empty():\n"
      "        name = \"world\"\n\n"
      "    wave unic = '👋'\n"
      "    io::out($\"hello {name} {wave}\\n\")\n\n"
      "    return 0\n" },
    { "src/user.ura",
      "use \"@/posix\"\n\n"
      "fn user() String:\n"
      "    @if os == \"windows\":\n"
      "        return env::get(\"USERNAME\")\n"
      "    @elif os == \"linux\":\n"
      "        return env::get(\"USER\")\n"
      "    @elif os == \"macos\":\n"
      "        return env::get(\"USER\")\n"
      "    @else:\n"
      "        return \"unknown user\"\n" },
    { NULL, NULL },
};

void init_project(char *name) {
    if (access(format("%s/ura.conf", name), F_OK) == 0) {
        eprint("'%s' already has a ura.conf, replace its starter files? [y/N] ", name);
        char answer[8] = "";
        bool has_answer = fgets(answer, sizeof(answer), stdin) != NULL;
        bool is_yes = has_answer && (answer[0] == 'y' || answer[0] == 'Y');
        if (!is_yes)
            exit(1);
    }

    mkdir2(name);
    mkdir2(format("%s/src", name));

    char *config = format(
        "@name \"%s\"\n"
        "@version \"0.0.1\"\n"
        "\n"
        "@dep \"core\":\n"
        "    @url \"https://github.com/ura-computing/ura-core\"\n"
        "    @version \"0.0.1\"\n"
        "@dep \"posix\":\n"
        "    @url \"https://github.com/ura-computing/ura-posix\"\n"
        "    @version \"0.0.1\"\n"
        "\n"
        "// ura run: compile and run\n"
        "@script \"run\":\n"
        "    @cmd \"ura compile main.ura -o build/%s && ./build/%s\"\n"
        "// ura build: compile into build/%s\n"
        "@script \"build\":\n"
        "    @cmd \"ura compile main.ura -o build/%s\"\n"
        "\n"
        "// ura compile flags, add them to a script:\n",
        name,
        name,
        name,
        name,
        name
    );
    for (int i = 0; compile_flags[i][0]; i++)
        config = format(
            "%s//   %-12s%s\n", config, compile_flags[i][0], compile_flags[i][1]
        );

    char *readme = format(
        "# %s\n\n"
        "A [ura](https://github.com/ura-computing/ura-lang) project.\n\n"
        "```\n"
        "main.ura        says hello to the user\n"
        "src/user.ura    finds the user name on every OS, used with: use "
        "\"./src/user\"\n"
        "```\n\n"
        "| command | what it does |\n"
        "|---|---|\n"
        "| `ura deps install` | clone the dependencies (run it first) |\n"
        "| `ura run` | build `main.ura` and run it |\n"
        "| `ura build` | build it into `build/%s` |\n"
        "| `ura deps add <name>[@version] [repo url]` | add a dependency |\n"
        "| `ura deps update [name]` | move to the latest versions |\n"
        "| `ura deps remove <name>[@version]` | remove a dependency |\n"
        "| `ura deps list` | show the dependencies |\n",
        name,
        name
    );

    char *files[][2] = {
        { "ura.conf", config },
        { "README.md", readme },
        { ".gitignore", "build/\ndeps/\n" },
    };
    for (size_t i = 0; i < sizeof(files) / sizeof(*files); i++)
        write_file(format("%s/%s", name, files[i][0]), files[i][1]);
    for (int i = 0; template_files[i][0]; i++)
        write_file(format("%s/%s", name, template_files[i][0]), template_files[i][1]);

    help(GREEN("created %s/") "\n\n", name);
    help(DIM("next steps:") "\n");
    help("    " CYAN("cd %s") "\n", name);
    help("    " CYAN("ura deps install") "\n");
    help("    " CYAN("ura run") "\n");
}

char *find_project(void) {
    char *project = find_folder_with(getcwd2(), "ura.conf");
    bool ok = project != NULL;
    assert_true(
        ok,
        "no ura.conf in this folder or above",
        "create a project with: ura init <name>"
    );
    return project;
}

// ASSERTS
void add_error(int s, int e, char *msg, char *help) {
    File *file = file_at(s);
    ucg_add_error(file, s - file->start, e - file->start, msg, help);
}

bool expect(Type type, char *msg, char *fix) {
    if (peek(0)->type == type) {
        next();
        return true;
    }
    Token *prev = ura.tokens[ura.exe_pos - 1];
    Token *at = peek(0)->line == prev->line ? peek(0) : prev;
    int start = line_start(prev->s);
    int end = line_end(prev->e);
    add_error(
        at->s,
        at->e,
        msg,
        format(
            "use %.*s%s%.*s",
            prev->e - start,
            text_at(start),
            fix,
            end - prev->e,
            text_at(prev->e)
        )
    );
	return false;
}

bool assert_lexeme(char *buff, int s, int e) {
    char c = buff[s];
    if (isdigit(c)) {
        bool is_real = memchr(buff + s, '.', e - s) != NULL;
        errno = 0;
        if (is_real)
            strtod(buff + s, NULL);
        else
            strtoll(buff + s, NULL, 10);
        if (errno != ERANGE)
            return true;
        ucg_add_error(
            ura.file,
            s,
            e,
            format("%.*s is too big for %s", e - s, buff + s, is_real ? "f64" : "i64"),
            NULL
        );
        return false;
    }
    if (strncmp2(buff + s, "/*", 2)) {
        if (buff[e] != '\0')
            return true;
        ucg_add_error(
            ura.file,
            s,
            s + 2,
            "unclosed comment",
            "add */ to close it"
        );
        return false;
    }
    if (c != '"' && c != '\'') {
        ucg_add_error(
            ura.file,
            s,
            s + 1,
            format("unknown character '%c'", c),
            "remove it"
        );
        return false;
    }
    bool is_valid = true;
    int letters = 0;
    int i = s + 1;
    while (buff[i] != '\0' && buff[i] != c) {
        if (ends_string(buff, s, i))
            break;
        char next = buff[i + 1];
        bool is_escape = buff[i] == '\\' && next != '\0' && !strchr2("\n", next);
        bool is_known = isprint(next) && escaped[(unsigned char)next];
        if (is_escape && !is_known) {
            ucg_add_error(
                ura.file,
                i,
                i + 2,
                format("unknown escape '\\%c'", next),
                "the escapes are \\n \\t \\r \\0 \\\\ \\\" \\'"
            );
            is_valid = false;
        }
        // 'é' = [C3 A9]: C3 starts the letter, A9 (0x80..0xBF) continues it
        unsigned char byte = buff[i];
        bool is_continuation = byte >= 0x80 && byte < 0xC0;
        i += is_escape ? 2 : 1;
        if (!is_continuation)
            letters++;
    }
    if (buff[i] != c) {
        int start = line_start_in(buff, s);
        int end = line_end_in(buff, s);
        ucg_add_error(
            ura.file,
            s,
            end,
            c == '"' ? "unclosed string" : "unclosed character",
            format("use %.*s%c", end - start, buff + start, c)
        );
        return false;
    }
    if (is_valid && c == '\'' && letters > 1) {
        ucg_add_error(
            ura.file,
            s,
            e,
            format("a character holds one letter, not %d", letters),
            format("use \"%.*s\" for a string", e - s - 2, buff + s + 1)
        );
        return false;
    }
    return is_valid;
}

void assert_unexpected(Token *token) {
    Token *scope = ura.scope->token;
    bool is_in_match = scope->type == MATCH && token->line != scope->line;
    char *text = token_text(token);
    int end = line_end(token->e);
    char *msg = format("unexpected '%s'", text);
    char *help = NULL;
    if (is_in_match)
        msg = "expected 'case' or 'default' inside a match";
    else if (token->type == CATCH)
        msg = "'catch' without a matching 'try'";
    else if (includes(token->type, CASE, DEFAULT, 0))
        msg = format("'%s' without a matching 'match'", text);
    else if (includes(token->type, ELIF, ELSE, 0))
        msg = format("%s without an if before it", text);
    if (!is_in_match && token->type == ELIF)
        help = format("use if%.*s", end - token->e, text_at(token->e));
    add_error(
        token->s,
        token->e,
        msg,
        help
    );
}

void assert_template(Node *node) {
    Token *token = node->token;
    for (int i = 0; i < node->args->children_count; i++) {
        Token *param = node->args->children[i]->token;
        if (param->type != ID)
            add_error(
                param->s,
                param->e,
                "expected names after template",
                "use template<T, V>"
            );
    }
    int space = ura.tokens[token->body.start]->space;
    for (int i = token->body.start; i < token->body.end; i++) {
        Token *curr = ura.tokens[i];
        Token *prev = ura.tokens[i - 1];
        bool is_head = curr->space == space && curr->line != prev->line;
        bool is_declaration =
            includes(curr->type, STRUCT, FN_DEC, PROTO, PUB, OPERATOR, 0);
        if (is_head && !is_declaration)
            add_error(
                curr->s,
                curr->e,
                "expected struct/fn/operator after template",
                NULL
            );
    }
}

bool assert_template_use(Token *token, Node *template, Node *types) {
    if (template == NULL && types == NULL)
        return false;
    if (template == NULL) {
        add_error(
            token->s,
            token->e,
            format("no template named '%s'", token->name),
            format("declare it: template<T>: struct %s:", token->name)
        );
        return false;
    }
    if (types == NULL) {
        add_error(
            token->s,
            token->e,
            format("expected '<' after '%s'", token->name),
            NULL
        );
        return false;
    }
    Node *params = template->args;
    if (types->children_count == params->children_count)
        return true;
    char *declared = "";
    for (int i = 0; i < params->children_count; i++) {
        char *name = params->children[i]->token->name;
        declared = format("%s%s%s", declared, i ? ", " : "", name);
    }
    add_error(
        token->s,
        token->e,
        format(
            "'%s' takes %d type argument%s, got %d",
            token->name,
            params->children_count,
            params->children_count == 1 ? "" : "s",
            types->children_count
        ),
        format("it is declared 'template<%s>'", declared)
    );
    return false;
}

void assert_macro(Node *node) {
    Token *token = node->token;
    add_error(
        token->s,
        token->e,
        node->left ? node->left->token->chars.value : "@error",
        node->right ? node->right->token->chars.value : NULL
    );
}

void assert_config(Node *node) {
    Token *token = node->token;
    char *text = token_text(token);
    bool is_config = strcmp2(basename(strdup2(file_at(token->s)->path)), "ura.conf");
    if (!is_config) {
        add_error(
            token->s,
            token->e,
            format("'%s' only works in ura.conf", text),
            "move it to the project's ura.conf"
        );
        return;
    }
    bool is_block = includes(token->type, MACRO_DEPENDENCY, MACRO_SCRIPT, 0);
    if (is_block || node->children_count == 1)
        return;
    add_error(
        token->s,
        token->e,
        format("'%s' takes one string", text),
        format("like: %s \"value\"", text)
    );
}

bool assert_config_field(Node *node, Node *field) {
    Token *token = field->token;
    bool is_script = node->token->type == MACRO_SCRIPT;
    bool is_cmd = token->type == MACRO_CMD;
    bool is_source = includes(token->type, MACRO_VERSION, MACRO_URL, MACRO_PATH, 0);
    if (is_script ? is_cmd : is_source)
        return true;
    char *block = token_text(node->token);
    add_error(
        token->s,
        token->e,
        format("'%s' doesn't belong in %s", token_text(token), block),
        is_script ? "use @cmd" : "use @url, @path or @version"
    );
    return false;
}

void assert_config_block(Node *node) {
    Token *token = node->token;
    char *text = token_text(token);
    bool is_script = token->type == MACRO_SCRIPT;
    Node *url = is_script ? NULL : node->url;
    if (token->name == NULL)
        add_error(
            token->s,
            token->e,
            format("'%s' needs a name", text),
            format("like: %s \"%s\":", text, is_script ? "run" : "core")
        );
    else if (is_script && node->cmd == NULL)
        add_error(
            token->s,
            token->e,
            format("script %s needs @cmd", token->name),
            "add: @cmd \"ura compile main.ura -o build/main\""
        );
    else if (is_script)
        return;
    else if (url == NULL)
        add_error(
            token->s,
            token->e,
            format("dependency %s needs @url or @path", token->name),
            "add: @url \"https://github.com/<user>/<repo>\" (or @path \"./folder\")"
        );
    else if (url->token->type == MACRO_PATH && url->version)
        add_error(
            url->version->token->s,
            url->version->token->e,
            format("dependency %s is a folder, it has no @version", token->name),
            "remove the @version line"
        );
    else if (url->version && url->version->children_count)
        assert_version(is_version(entry_value(url->version, 0)), url->version);
}

bool assert_version(bool ok, Node *version) {
    if (ok) return true;
    Token *token = version->children[0]->token;
    add_error(
        token->s,
        token->e,
        format("version '%s' is not like 0.0.1", token->chars.value),
        "use three numbers: @version \"0.0.1\""
    );
    return false;
}

bool assert_installed(bool ok, Node *dependency, char *folder) {
    if (ok) return true;
    Token *token = dependency->token;
    Token *url = dependency->url->token;
    if (url->type == MACRO_PATH)
        add_error(
            url->s,
            url->e,
            format("dependency %s: no folder %s", token->name, folder),
            "fix the @path"
        );
    else
        add_error(
            token->s,
            token->e,
            format("dependency %s is not installed", token->name),
            "run: ura deps install"
        );
    return false;
}

bool assert_true(bool ok, char *msg, char *help) {
    if (ok) return true;
    eprint("%s\n", msg);
    if (help)
        fprintf(stderr, "help: %s\n", help);
    exit(1);
}

bool assert_module(bool ok, Node *node) {
    if (ok) return true;
    Token *token = node->token;
    add_error(
        token->s,
        token->e,
        "'mod' only works at the top level",
        "move it out of the block"
    );
    return false;
}

bool assert_package(bool ok, Token *name) {
    if (ok) return true;
    char *package = name->chars.value;
    bool is_package = strncmp2(package, "@/", 2);
    add_error(
        name->s,
        name->e,
        format("package '%s' not found", package),
        is_package ? "add a dependency that has it: ura deps add <name>" : NULL
    );
    return false;
}

bool assert_reuse(Token *name, File *file) {
    for (int i = 0; i < ura.uses_count; i++) {
        if (!strcmp2(ura.uses[i]->path, file->path))
            continue;
        add_error(
            name->s,
            name->e,
            format("circular use of '%s'", name->chars.value),
            "a used file can't use itself again"
        );
        return false;
    }
    return true;
}

bool assert_parent(bool ok, Token *name) {
    if (ok) return true;
    add_error(
        name->s,
        name->e,
        format("no struct named '%s' to take from", token_text(name)),
        NULL
    );
    return false;
}

void assert_operand(Node *node) {
    Token *token = node->token;
    if (node->right)
        return;
    char *what = "a value";
    if (token->type == AS)
        what = "a type";
    else if (token->type == DOT)
        what = "an attribute or method name";
    add_error(
        token->s,
        token->e,
        format("expected %s after '%s'", what, token_text(token)),
        NULL
    );
}

void assert_type_first(Token *token) {
    int end = 0;
    for (int depth = 0; peek(end)->type == LT || depth > 0; end++) {
        if (peek(end)->type == LAST)
            break;
        if (peek(end)->type == LT)
            depth++;
        if (peek(end)->type == GT)
            depth--;
    }
    if (peek(end)->type == LBRACK && peek(end + 1)->type == RBRACK)
        end += 2;
    Token *name = peek(end);
    if (name->type != ID || name->line != token->line)
        return;
    Token *last = ura.tokens[ura.exe_pos + end - 1];
    add_error(
        token->s,
        last->e,
        "the type goes after the name",
        format(
            "use %s %.*s%.*s",
            name->name,
            last->e - token->s,
            text_at(token->s),
            line_end(name->e) - name->e,
            text_at(name->e)
        )
    );
    ura.exe_pos += end;
}

char *macro_to_text(Node *node) {
    Token *token = node->token;
    if (token->type == CHARS)
        return token->chars.value;
    if (token->type == ID && strcmp2(token->name, "os")) {
#if defined(_WIN32)
        return "windows";
#elif defined(__APPLE__)
        return "macos";
#else
        return "linux";
#endif
    }
    if (token->type == ID && strcmp2(token->name, "arch")) {
#if defined(__aarch64__) || defined(__arm64__)
        return "arm64";
#else
        return "x86_64";
#endif
    }
    return NULL;
}

bool macro_cond(Node *cond) {
    Token *token = cond->token;
    switch (token->type) {
    case AND:
        return macro_cond(cond->left) && macro_cond(cond->right);
    case OR:
        return macro_cond(cond->left) || macro_cond(cond->right);
    case NOT:
        return !macro_cond(cond->operand);
    case INT:
        return token->num.value != 0;
    case EQ:
    case NQ: {
        char *left = macro_to_text(cond->left);
        char *right = macro_to_text(cond->right);
        if (left && right)
            return strcmp2(left, right) == (token->type == EQ);
        break;
    }
    case FN_CALL: {
        bool is_installed = strcmp2(token->name, "installed");
        bool has_one_arg = cond->children_count == 1;
        bool has_name = has_one_arg && cond->children[0]->token->type == CHARS;
        if (is_installed && has_name)
            return ucg_find_lib(cond->children[0]->token->chars.value) != NULL;
        break;
    }
    default:
        break;
    }
    add_error(
        token->s,
        token->e,
        "@if checks os, arch and installed(\"lib\")",
        "use @if os == \"linux\" and installed(\"SDL2\"):"
    );
    return false;
}

Node *find_drop(Node *type) {
    if (type == NULL || type->token->type != ID || type->declared == NULL)
        return NULL;
    return find_operator(DROP, type, NULL, 0);
}

bool needs_drop(Node *type) {
    if (find_drop(type))
        return true;
    if (type == NULL || type->token->type != ID || type->declared == NULL)
        return false;
    Node *st = type->declared;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != DECLARE)
            continue;
        if (needs_drop(child->var->data_type))
            return true;
    }
    return false;
}

bool lives_in_variable(Node *node) {
    bool is_member = includes(node->token->type, DOT, DOUBLE_DOTS, 0);
    Node *call = is_member ? node->right : node;
    if (call->token->type == FN_CALL)
        return call->to_call && ret_type_of(call->to_call)->token->type == REF;
    if (is_member)
        return true;
    return includes(node->token->type, VAR, PARAM, ACCESS, 0);
}

bool is_local_variable(Node *node) {
    if (!includes(node->token->type, VAR, PARAM, 0) || is_ref_variable(node))
        return false;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        if (find_in_scope(scope, node->token->name) == node->declared)
            return true;
        if (scope->token->type == FN_DEC)
            return false;
    }
    return false;
}

bool is_math(Node *node) {
    return includes(
        node->token->type,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        BAND,
        BOR,
        BXOR,
        LSHIFT,
        RSHIFT,
        ADD_ASSIGN,
        SUB_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        BAND_ASSIGN,
        BOR_ASSIGN,
        BXOR_ASSIGN,
        LSHIFT_ASSIGN,
        RSHIFT_ASSIGN,
        0
    );
}

bool is_numeric(Node *type) {
    bool is_number_type = type && includes(type->token->type, INT, FLOAT, 0);
    return is_number_type && type->token->shift != 1;
}
// TODO: this is the ugliest code ever written
void assert_type(Node *value, Node *expected, char *help) {
    if (expected && value->token->type == LCBRACK) {
        assert_struct_value(value, expected);
        return;
    }
    if (value->token->type == LBRACK) {
        assert_array_literal(value, expected);
        return;
    }
    if (value->token->type == ZERO) {
        assert_null(value, expected);
        return;
    }
	// if(value->left == NULL) {
	// 	raise(SIGSEGV);
	// 	print(">>%n\n", value);
	// 	print(">>%n\n", value->left);
	// 	print(">>%n\n", value->right);
	// 	exit(1);
	// }
	// assert(value->left);
    bool is_numeric_left = is_numeric(type_of(value));
    bool is_numeric_right = is_numeric(type_of(value));
    bool is_numeric_sides = is_numeric_left && is_numeric_right;
    bool is_numeric_math = is_math(value) && is_numeric(expected) && is_numeric_sides;
    if (is_numeric_math) {
        assert_type(value->left, expected, help);
        assert_type(value->right, expected, help);
        return;
    }
    if (value->token->type == CHARS && is_chars(expected)) {
        value->data_type = expected;
        return;
    }
    Node *found = type_of(value);
    bool unknown = expected && expected->token->type == ID && !expected->declared;
    if (found == NULL || expected == NULL || unknown)
        return;
    Token *token = value->token;
    bool is_negated = token->type == NEG && is_number(value->operand);
    Node *literal_number = is_negated ? value->operand : value;
    if (is_number(literal_number) && is_number(expected)) {
        Token *digits = literal_number->token;
        int bits = expected->token->shift - !expected->token->is_uint;
        long long max = bits == 64 ? ~0ULL >> 1 : (1ULL << bits) - 1;
        long long min = expected->token->is_uint ? 0 : -max - 1;
        long long number = is_negated ? -digits->num.value : digits->num.value;
        if (number < min || number > max)
            add_error(
                token->s,
                digits->e,
                format("%lld doesn't fit in %s", number, type_name(expected)),
                format("%s goes from %lld to %lld", type_name(expected), min, max)
            );
        digits->shift = expected->token->shift;
        digits->is_uint = expected->token->is_uint;
        return;
    }
    bool is_real = value->token->type == FLOAT && !value->token->is_type;
    if (is_real && expected->token->type == FLOAT) {
        token->shift = expected->token->shift;
        return;
    }
    Node *literal = token->type == NEG ? value->operand : NULL;
    bool is_float_literal = literal && literal->token->type == FLOAT;
    bool is_real_literal = is_float_literal && !literal->token->is_type;
    if (is_real_literal && expected->token->type == FLOAT && help == NULL)
        help = format("write -%sf for an f32", token_text(literal->token));
    bool is_pointer = includes(found->token->type, REF, ARRAY, 0);
    bool is_expected_pointer = includes(expected->token->type, REF, ARRAY, 0);
    bool is_both = is_pointer && is_expected_pointer;
    if (is_both && same_type(found->data_type, expected->data_type))
        return;
    if (!same_type(found, expected))
        add_error(
            token->s,
            token->e,
            format("expected '%s', found '%s'", type_name(expected), type_name(found)),
            help
        );
}

void assert_null_compare(Node *node) {
    bool is_right = node->right->token->type == ZERO;
    Node *null = is_right ? node->right : node->left;
    Node *other = is_right ? node->left : node->right;
    if (other->token->type == ZERO)
        other->data_type = chars_type();
    Node *type = is_ref_variable(other) ? other->data_type : type_of(other);
    bool is_pointer = type && includes(type->token->type, REF, ARRAY, 0);
    if (is_pointer) {
        null->data_type = type;
        return;
    }
    Token *token = other->token;
    add_error(
        token->s,
        token->e,
        format(
            "'%s' is never null, so this comparison is always the same",
            token_text(token)
        ),
        NULL
    );
}

void assert_null(Node *node, Node *expected) {
    bool is_pointer = expected && includes(expected->token->type, REF, ARRAY, 0);
    if (is_pointer) {
        node->data_type = expected;
        return;
    }
    Token *token = node->token;
    char *msg = "null needs a type here";
    if (expected)
        msg = format("'%s' can't be null", type_name(expected));
    add_error(
        token->s,
        token->e,
        msg,
        "null fits a ref (&T) or an array (T[])"
    );
}

void assert_array_literal(Node *node, Node *expected) {
    Token *token = node->token;
    if (expected == NULL) {
        Node *first = node->children_count ? type_of(node->children[0]) : NULL;
        bool is_number = first && includes(first->token->type, INT, FLOAT, 0);
        if (is_number && !first->token->is_type)
            first = new_type(first->token->type, first->token->shift, NULL);
        node->data_type = first ? new_type(ARRAY, 0, first) : NULL;
        return;
    }
    if (expected->token->type == ID && expected->declared == NULL)
        expected->declared = find_struct(expected->token->name);
    Node *elem = element_type_of(expected);
    if (elem == NULL) {
        add_error(
            token->s,
            token->e,
            format("expected '%s', found an array literal", type_name(expected)),
            NULL
        );
        return;
    }
    for (int i = 0; i < node->children_count; i++)
        assert_type(node->children[i], elem, NULL);
    node->data_type = expected;
}

void assert_struct_value(Node *value, Node *expected) {
    Token *token = value->token;
    if (expected->token->type != ID) {
        add_error(
            token->s,
            token->e,
            format("expected '%s', found a struct value", type_name(expected)),
            NULL
        );
        return;
    }
    if (expected->declared == NULL)
        return;
    value->data_type = expected;
    Node *st = expected->declared;
    for (int i = 0; i < value->children_count; i++) {
        Node *attr = value->children[i];
        Token *name = attr->token;
        attr->declared = find_attr(st, name->name, &name->attr.index);
        if (attr->declared == NULL) {
            add_error(
                name->s,
                name->e,
                format("'%s' has no attribute '%s'", st->token->name, name->name),
                NULL
            );
            continue;
        }
        assert_type(attr->left, attr->declared->data_type, NULL);
        attr->left = assert_copy(attr->left, attr->declared->data_type);
    }
}

Node *assert_copy(Node *value, Node *type) {
    Node *found = type_of(value);
    bool is_named = type && type->token->type == ID;
    if (is_named && type->declared == NULL)
        type->declared = find_struct(type->token->name);
    bool is_struct = is_named && type->declared;
    bool is_same = is_struct && found && same_type(found, type);
    bool is_copy = is_same && lives_in_variable(value);
    bool is_ref = found && found->token->type == REF;
    Node *value_type = is_ref ? found->data_type : found;
    bool is_literal = includes(value->token->type, CHARS, LBRACK, 0);
    bool is_value = type && type->token->type != REF && !is_literal;
    bool is_other = is_value && value_type && !same_type(value_type, type);
    if (!is_copy && !is_other)
        return value;
    Node *assign = find_operator(ASSIGN, type, &value, 1);
    if (assign) {
        Node *copy = new_node(new_token(COPY, *value->token));
        copy->to_call = assign;
        copy->to_copy = value;
        return copy;
    }
    if (!is_copy || !needs_drop(type))
        return value;
    Token *token = value->token;
    char *name = type_name(type);
    add_error(
        token->s,
        token->e,
        format("'%s' can't be copied: it needs a drop and has no operator =", name),
        format(
            "define operator =(left &%s, right &%s), or take it as &%s",
            name,
            name,
            name
        )
    );
    return value;
}

char *signature_of(Node *fn) {
    Node *params = fn->args;
    char *text = "";
    for (int i = 0; i < params->children_count; i++) {
        Node *param = params->children[i];
        char *part = "...";
        if (param->token->type != VARIADIC)
            part = format("%s %s", param->token->name, type_name(param->data_type));
        text = format("%s%s%s", text, i ? ", " : "", part);
    }

    char *kind = fn->token->type == PROTO ? "proto" : "fn";
    char *ret = fn->ret_type ? format(" %s", type_name(fn->ret_type)) : "";
    char *where = format("%s:%d", relative_path(fn->token->filename), fn->token->line);
    return format(
        "%s %s(%s)%s\ndeclared at %s",
        kind,
        fn->token->name,
        text,
        ret,
        where
    );
}

void assert_args(Node *fn, Node **args, int count, int self) {
    for (int i = 0; i < count && i + self < fn->args->children_count; i++) {
        Node *type = fn->args->children[i + self]->data_type;
        Node *found = type_of(args[i]);
        bool is_child = is_child_ref(found, type);
        bool is_address = args[i]->token->type == REF;
        if (type == NULL)
            continue;
        if (is_address && !is_child)
            continue;
        bool takes_ref = type->token->type == REF;
        Node *param = takes_ref ? type->data_type : type;
        bool is_other = found && !same_type(found, param) && !is_child;
        if (!takes_ref || is_other)
            args[i] = assert_copy(args[i], param);
        if (!is_child)
            assert_type(args[i], param, signature_of(fn));
        if (!takes_ref)
            continue;
        if (is_address)
            continue;
        Node *ref = new_node(new_token(REF, *args[i]->token));
        ref->operand = args[i];
        args[i] = ref;
    }
}

void analyze_operator(Node *node, Type symbol) {
    Token *op = node->token;
    Node *left = node->left;
    Node *right = node->right;
    Node *type = type_of(left);
    int count = right != NULL;
    Node *fn = find_operator(symbol, type, &node->right, count);
    if (fn == NULL) {
        Node *right_type = count ? type_of(right) : NULL;
        char *name = right_type ? type_name(right_type) : "?";
        char *takes = count ? format(" that takes '%s'", name) : "";
        char *owner = type ? type_name(type) : "?";
        add_error(
            op->s,
            op->e,
            format("'%s' has no operator '%s'%s", owner, binary_ops[symbol], takes),
            NULL
        );
        return;
    }
    op->type = FN_CALL;
    op->name = fn->token->name;
    node->to_call = fn;
    node->right = NULL;
    append(node->children, left);
    if (count)
        append(node->children, right);
    assert_args(fn, node->children, node->children_count, 0);
}

void assert_name(Node *node) {
    Token *token = node->token;
    if (token->is_type && find_enum(token->name)) {
        token->type = INT;
        token->shift = 32;
        return;
    }
    if (token->is_type) {
        node->declared = find_struct(token->name);
        if (node->declared == NULL)
            add_error(
                token->s,
                token->e,
                format("unknown symbol '%s'", token->name),
                NULL
            );
        return;
    }
    Node *var = find_variable(token->name, token->s);
    if (var == NULL && find_function(token->name)) {
        add_error(
            token->s,
            token->e,
            format("'%s' is a function, not a value", token->name),
            format("call it: %s(...)", token->name)
        );
        return;
    }
    Node *later = var ? NULL : find_variable(token->name, __INT_MAX__);
    if (later) {
        add_error(
            token->s,
            token->e,
            format(
                "'%s' is used before its declaration, on line %d",
                token->name,
                line_at(later->token->s)
            ),
            "move the declaration above this line"
        );
        return;
    }
    if (var == NULL) {
        Node *like = find_variable_like(token->name, token->s, 2);
        bool is_outer = like && strcmp2(like->token->name, token->name);
        Node *st = find_scope(STRUCT);
        int index = 0;
        bool is_attr = st && find_attr(st, token->name, &index);
        char *msg = format("undeclared variable '%s'", token->name);
        char *help = format("declare it first, like: %s i32 = 0", token->name);
        if (token->type == SELF) {
            msg = "'self' only exists inside a method";
            help = NULL;
        } else if (is_attr)
            help = format("did you mean 'self.%s'?", token->name);
        else if (find_struct(token->name)) {
            msg = format("'%s' is a type, not a value", token->name);
            help = NULL;
        } else if (is_module(token->name)) {
            msg = format("'%s' is a mod, not a value", token->name);
            help = format("reach its members with '%s::'", token->name);
        } else if (is_outer) {
            msg = format("'%s' belongs to an enclosing function", token->name);
            help = "pass it as a parameter";
        } else if (like)
            help = format("did you mean '%s'?", like->token->name);
        add_error(
            token->s,
            token->e,
            msg,
            help
        );
        return;
    }
    token->type = var->token->type;
    node->data_type = var->data_type;
    node->declared = var;
    var->token->is_used = true;
}

void assert_throw(Node *node) {
    Token *token = node->token;
    Node *type = type_of(node->left);
    bool is_void = type == NULL || type->token->type == VOID;
    if (is_void) {
        add_error(
            token->s,
            token->e,
            "throw needs a value",
            "like: throw Error::create(\"...\")"
        );
        return;
    }
    node->left = assert_copy(node->left, type);
}

void assert_match(Node *node) {
    Token *token = node->token;
    Node *var = node->left->left;
    Node *type = type_of(node->left->right);
    if (type && type->token->type == REF)
        type = type->data_type;
    var->data_type = type ? type : new_type(INT, 32, NULL);
    if (type)
        assert_operation(node->left);
    if (node->children_count < 2 || node->children[1]->token->type == DEFAULT)
        add_error(
            token->s,
            token->e,
            "a match needs a case before default",
            "like: case 1:"
        );
    for (int i = 1; i < node->children_count - 1; i++) {
        Token *branch = node->children[i]->token;
        if (branch->type == DEFAULT)
            add_error(
                branch->s,
                branch->e,
                "default must be the last branch of a match",
                NULL
            );
    }
}

bool assert_param_type(bool ok, Node *param) {
    if (ok) return true;
    Token *token = param->token;
    add_error(
        token->s,
        token->e,
        format("parameter '%s' has no type", token->name),
        format("like: %s i32", token->name)
    );
    param->data_type = new_type(INT, 32, NULL);
    return false;
}

bool assert_comma(Token *item, Token *after, char *list) {
    bool is_comma = item->type == COMA;
    bool is_closing = includes(after->type, COMA, RPARENT, RBRACK, DOTS, LAST, 0);
    bool is_end = is_closing || after->is_type;
    bool is_trailing = is_comma && after->type == RPARENT;
    bool is_missing = !is_comma && !is_end && after->line == item->line;
    if (is_trailing)
        add_error(
            after->s,
            after->e,
            "expected an argument after ','",
            NULL
        );
    else if (is_missing)
        add_error(
            after->s,
            after->e,
            format("expected ',' between %s", list),
            NULL
        );
    return is_missing;
}

int name_after(Token *keyword) {
    int pos = keyword->e;
    while (text_at(pos)[0] == ' ')
        pos++;
    return pos;
}

bool assert_attribute(bool ok, Node *node) {
    if (ok) return true;
    Token *token = node->token;
    char *msg = "a struct holds only attributes and methods";
    char *fix = NULL;
    if (token->type == ID) {
        msg = format("attribute '%s' needs a type", token->name);
        fix = format("like: %s i32", token->name);
    }
    add_error(
        token->s,
        token->e,
        msg,
        fix
    );
    return false;
}

void assert_struct(Node *node, Node *scope) {
    Token *token = node->token;
    int name_s = name_after(token);
    int name_e = name_s + strlen(token->name);
    for (int i = 0; i < scope->children_count; i++) {
        Token *other = scope->children[i]->token;
        if (other->type != STRUCT)
            continue;
        if (other->s >= token->s)
            continue;
        if (!strcmp2(other->name, token->name))
            continue;
        add_error(
            name_s,
            name_e,
            format(
                "struct '%s' is already declared, on line %d",
                token->name,
                line_at(other->s)
            ),
            NULL
        );
        return;
    }
    if (node->children_count == 0)
        add_error(
            name_s,
            name_e,
            format("struct '%s' has no attributes", token->name),
            NULL
        );
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type != DECLARE)
            continue;
        Node *attr_type = child->var->data_type;
        if (!contains_struct(attr_type, token->name, 0))
            continue;
        Token *type = attr_type->token;
        add_error(
            type->s,
            type->e,
            format("struct '%s' can't contain itself", token->name),
            format("use a ref: '&%s'", type->name)
        );
        child->var->data_type = new_type(REF, 0, attr_type);
    }
}

bool contains_struct(Node *type, char *name, int depth) {
    Node *st = type->token->type == ID ? find_struct(type->token->name) : NULL;
    if (st == NULL || depth > 32)
        return false;
    if (strcmp2(st->token->name, name))
        return true;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        bool is_attr = child->token->type == DECLARE;
        if (is_attr && contains_struct(child->var->data_type, name, depth + 1))
            return true;
    }
    return false;
}

void assert_function(Node *node, Node *scope) {
    Token *token = node->token;
    int name_s = name_after(token);
    int name_e = name_s + strlen(token->name);
    if (token->name[0] == '\0') {
        int start = line_start(name_s);
        add_error(
            name_s,
            name_s + 1,
            "expected a name after 'fn'",
            format(
                "use %.*sname%.*s",
                name_s - start,
                text_at(start),
                line_end(name_s) - name_s,
                text_at(name_s)
            )
        );
        return;
    }
    Node **params = node->args->children;
    int count = node->args->children_count;
    for (int i = 0; i + 1 < count; i++) {
        Token *param = params[i]->token;
        if (param->type == VARIADIC)
            add_error(
                param->s,
                param->e,
                "'...' must be the last parameter",
                NULL
            );
    }
    for (int i = 0; token->type == PROTO && i < scope->children_count; i++) {
        Node *other = scope->children[i];
        if (other->token->type != PROTO)
            continue;
        if (other->token->s >= token->s)
            continue;
        if (!strcmp2(other->token->name, token->name))
            continue;
        bool is_same_ret = same_type(ret_type_of(node), ret_type_of(other));
        if (same_params(other, node) && is_same_ret)
            continue;
        add_error(
            name_s,
            name_e,
            format("'%s' is already declared with other parameters", token->name),
            format("the first one is %s", signature_of(other))
        );
        break;
    }
    if (token->type == PROTO)
        return;
    for (int i = 0; i < count; i++)
        for (int j = 0; j < i; j++)
            if (strcmp2(params[i]->token->name, params[j]->token->name))
                add_error(
                    params[i]->token->s,
                    params[i]->token->e,
                    format("'%s' is already declared", params[i]->token->name),
                    NULL
                );
    Node *symbol = node->args->symbol;
    bool is_in_struct = scope->token->type == STRUCT;
    if (is_in_struct && symbol)
        add_error(
            token->s,
            token->e,
            "'operator' must be declared outside the struct",
            "move it after the struct, the left side is the first parameter"
        );
    if (symbol && symbol->token->type == AS)
        add_error(
            token->s,
            token->e,
            "'as' is a cast, it can't be an operator",
            "convert with operator =(left &To, right &From)"
        );
    if (!is_in_struct && token->is_pub)
        add_error(
            token->s,
            token->e,
            "'pub' may only be used inside a struct",
            NULL
        );
    bool is_drop = symbol && strcmp2(token->name, "drop");
    bool returns_value = ret_type_of(node)->token->type != VOID;
    char *drop_msg = "operator drop returns nothing";
    if (count != 1)
        drop_msg = "operator drop takes one parameter, the value";
    if (is_drop && (count != 1 || returns_value))
        add_error(
            token->s,
            token->e,
            drop_msg,
            "write operator drop(value &T):"
        );
    bool is_in = symbol && strcmp2(token->name, "in");
    bool is_binary = symbol && !is_drop && !is_in;
    if (is_binary && (count < 1 || count > 2))
        add_error(
            token->s,
            token->e,
            format("operator '%s' takes a left and a right side", token->name),
            format("use operator %s(left &T, right &T)", token->name)
        );
    Node *left = symbol && count == 2 ? params[0]->data_type : NULL;
    Node *right = left ? params[1]->data_type : NULL;
    bool is_by_value = right && right->token->type == ID && left->token->type == REF;
    bool is_same_type = is_by_value && same_type(left->data_type, right);
    char *name = right ? type_name(right) : NULL;
    if (is_same_type && symbol->token->type == ASSIGN)
        add_error(
            token->s,
            token->e,
            "operator = takes the right side by reference, a copy would need itself",
            format("use operator =(left &%s, right &%s)", name, name)
        );
    for (int i = 0; i < scope->children_count; i++) {
        Node *other = scope->children[i];
        if (!includes(other->token->type, FN_DEC, PROTO, 0))
            continue;
        if (other == node)
            continue;
        if (!strcmp2(other->token->name, node->token->name))
            continue;
        bool is_same = same_params(other, node);
        bool is_earlier = other->token->s < node->token->s;
        bool is_earlier_fn = other->token->type == FN_DEC && is_earlier;
        bool is_proto = other->token->type == PROTO;
        Node *ret = ret_type_of(node);
        bool is_same_ret = same_type(ret, ret_type_of(other));
        char *fn_name = node->token->name;
        if (is_same && is_earlier_fn && is_same_ret)
            add_error(
                name_s,
                name_e,
                format("'%s' is already defined with these parameters", fn_name),
                NULL
            );
        else if (is_same && is_earlier_fn)
            add_error(
                name_s,
                name_e,
                format("'%s' can't be overloaded on its return type", fn_name),
                NULL
            );
        else if (is_same && is_proto && !is_same_ret)
            add_error(
                name_s,
                name_e,
                format(
                    "'%s' returns '%s', but its proto returns '%s'",
                    fn_name,
                    type_name(ret),
                    type_name(ret_type_of(other))
                ),
                "a fn replaces a proto only when both have the same parameters and "
                "return type"
            );
    }
}

void assert_enum(Node *node) {
    Token *token = node->token;
    if (node->children_count == 0)
        add_error(
            token->s,
            token->e,
            format("enum '%s' has no variants", token->name),
            format("add some: enum %s: A, B", token->name)
        );
    for (int i = 0; i < node->children_count; i++) {
        Node *variant = node->children[i];
        Token *name = variant->token;
        Node *value = variant->left;
        bool is_int = value && value->token->type == INT;
        bool is_integer = value == NULL || (is_int && !value->token->is_type);
        if (!is_integer)
            add_error(
                value->token->s,
                value->token->e,
                format("the value of '%s' must be an integer", name->name),
                NULL
            );
        for (int j = 0; j < i; j++)
            if (strcmp2(node->children[j]->token->name, name->name))
                add_error(
                    name->s,
                    name->e,
                    format("'%s' is already declared", name->name),
                    NULL
                );
    }
}

void assert_declare(Node *node) {
    Token *token = node->var->token;
    Node *first = find_in_scope(ura.scope, token->name);
    if (first == node->var)
        return;
    int type_end = node->var->data_type->token->e;
    char *rest = format("%.*s", line_end(type_end) - type_end, text_at(type_end));
    char *fix = strchr(rest, '=') ? format("use %s%s", token->name, rest) : NULL;
    add_error(
        token->s,
        token->e,
        format(
            "'%s' is already declared, on line %d",
            token->name,
            line_at(first->token->s)
        ),
        fix
    );
}

void assert_return(Node *node) {
    Token *token = node->token;
    Node *fn = find_scope(FN_DEC);
    if (fn == NULL && find_function("main")) {
        add_error(
            token->s,
            token->e,
            "'return' outside of a function",
            "top-level code returns only in a file without main"
        );
        return;
    }
    if (fn == NULL)
        return;
    Node *ret = ret_type_of(fn);
    if (node->left == NULL) {
        if (ret->token->type != VOID)
            add_error(
                token->s,
                token->e,
                format(
                    "'%s' must return a value of type '%s'",
                    fn->token->name,
                    type_name(ret)
                ),
                NULL
            );
        return;
    }
    assert_type(node->left, ret_type_of(fn), NULL);
    if (!is_local_variable(node->left))
        node->left = assert_copy(node->left, ret_type_of(fn));
}

void assert_ref(Node *node) {
    Token *token = node->token;
    bool is_type = token->is_type;
    Node *to = node->operand;
    Token *at = token;
    char *msg = NULL;
    bool is_value = to && to->token->type != ID && !lives_in_variable(to);
    if (to == NULL) {
        msg = "expected a type after '&'";
        node->data_type = new_type(INT, 32, NULL);
    } else if (to->token->type == REF && is_type) {
        msg = "a ref can't point to a ref";
        node->operand = to->operand;
    } else if (to->token->type == REF) {
        msg = "can't take the address of an address";
        node->operand = to->operand;
    } else if (!is_type && is_value) {
        at = to->token;
        msg = "'&' expects a variable or a struct attribute";
    }
    if (msg == NULL)
        return;
    add_error(
        at->s,
        at->e,
        msg,
        NULL
    );
}

bool assert_array(bool ok, Node *node) {
    if (ok) return true;
    Token *size = node->index->token;
    add_error(
        size->s,
        size->e,
        "arrays have no size in the type",
        format("use '%s[]'", type_name(node->data_type))
    );
    return false;
}

void assert_cast(Node *node) {
    Node *from = type_of(node->left);
    Node *to = node->to_type;
    if (from == NULL || to == NULL || same_type(from, to))
        return;
    bool has_struct = from->token->type == ID || to->token->type == ID;
    if (!has_struct)
        return;
    char *name = type_name(from);
    char *target = type_name(to);
    add_error(
        node->token->s,
        node->token->e,
        format("can't cast '%s' to '%s'", name, target),
        format("define operator =(left &%s, right &%s), then assign it", target, name)
    );
}

void assert_unary(Node *node) {
    Token *token = node->token;
    Node *type = type_of(node->operand);
    bool is_integer = is_numeric(type) && type->token->type == INT;
    bool fits = token->type == BNOT ? is_integer : is_numeric(type);
    if (type == NULL || fits)
        return;
    add_error(
        token->s,
        token->e,
        format("can't apply '%s' to '%s'", binary_ops[token->type], type_name(type)),
        NULL
    );
}

void assert_for(Node *node) {
    Node *type = type_of(node->right);
    Node *in = find_operator(IN, type, NULL, 0);
    bool fits = in && in->args->children_count == 3;
    if (fits) {
        node->var->data_type = in->args->children[2]->data_type->data_type;
        return;
    }
    Token *token = node->right->token;
    add_error(
        token->s,
        token->e,
        format("can't loop over '%s'", type ? type_name(type) : token_text(token)),
        "add operator in(value &T, i &i32, x &E) bool"
    );
    node->var->data_type = new_type(INT, 32, NULL);
}

void assert_jump(Node *node) {
    Token *token = node->token;
    if (node->loop)
        return;
    if (token->type == BRK)
        add_error(
            token->s,
            token->e,
            "break outside a loop or match",
            "break only works inside a while or a match"
        );
    else
        add_error(
            token->s,
            token->e,
            "continue outside a loop",
            "continue only works inside a while"
        );
}

void assert_try(Node *node) {
    Token *token = node->token;
    char *help = "like: catch e Error:";
    if (token->type == TRY && node->next == NULL)
        add_error(
            token->s,
            token->e,
            "expected 'catch' after the try block",
            help
        );
    else if (token->type == CATCH && node->var == NULL)
        add_error(
            token->s,
            token->e,
            "expected a name after 'catch'",
            help
        );
    else if (token->type == CATCH && node->var->data_type == NULL)
        add_error(
            node->var->token->s,
            node->var->token->e,
            "expected a type after the catch name",
            help
        );
}

void assert_cond(Node *node) {
    Token *token = node->token;
    Node *cond = node->cond;
    bool takes_cond = !includes(token->type, ELSE, DEFAULT, 0);
    if (cond && !takes_cond) {
        int end = line_end(token->e);
        add_error(
            token->s,
            token->e,
            "else has no condition",
            format("use elif%.*s", end - token->e, text_at(token->e))
        );
        return;
    }
    if (cond == NULL && token->type == CASE)
        add_error(
            token->s,
            token->e,
            "expected a value after 'case'",
            "like: case 1:"
        );
    else if (cond == NULL && takes_cond)
        add_error(
            token->s,
            token->e,
            format("expected a condition after '%s'", token_text(token)),
            NULL
        );
    if (cond == NULL)
        return;
    analyze(cond);
    char *help = NULL;
    if (includes(cond->token->type, VAR, PARAM, 0))
        help = format("write '%s != 0'", cond->token->name);
    assert_type(cond, bool_type(), help);
}

void assert_variant(Node *node, Node *en) {
    Token *name = node->right->token;
    for (int i = 0; i < en->children_count; i++) {
        Token *variant = en->children[i]->token;
        if (!strcmp2(variant->name, name->name))
            continue;
        node->token->type = INT;
        node->token->shift = 32;
        node->token->num.value = variant->num.value;
        return;
    }
    add_error(
        name->s,
        name->e,
        format("enum '%s' has no variant '%s'", en->token->name, token_text(name)),
        NULL
    );
}

void assert_static_call(Node *node) {
    Token *name = node->left->token;
    if (is_module(name->name)) {
        Token *member = node->right->token;
        member->name = format("%s::%s", name->name, member->name);
        *node = *node->right;
        analyze(node);
        return;
    }
    Node *en = find_enum(name->name);
    if (en) {
        assert_variant(node, en);
        return;
    }
    Node *st = find_struct(name->name);
    if (st == NULL) {
        add_error(
            name->s,
            name->e,
            format("unknown type '%s'", name->name),
            NULL
        );
        return;
    }
    Node *call = node->right;
    for (int i = 0; i < call->children_count; i++)
        analyze(call->children[i]);
    call->to_call =
        find_method(st, call->token->name, call->children, call->children_count, true);
    if (call->to_call == NULL) {
        add_error(
            call->token->s,
            call->token->e,
            format("'%s' has no pub fn '%s'", st->token->name, call->token->name),
            NULL
        );
        return;
    }
    assert_args(call->to_call, call->children, call->children_count, 0);
}

void assert_link(Node *node) {
    Token *token = node->token;
    if (token->name == NULL)
        return;
    token->chars.value = ucg_find_lib(token->name);
    if (token->chars.value == NULL)
        add_error(
            token->s,
            token->e,
            format("library '%s' not found", token->name),
            "install it, or set URA_LIBC to the folder that holds it"
        );
}

void assert_index(Node *index) {
    Node *type = type_of(index);
    bool is_integer = type && type->token->type == INT && type->token->shift > 1;
    if (type == NULL || is_integer)
        return;
    add_error(
        index->token->s,
        index->token->e,
        format("expected an integer index, found '%s'", type_name(type)),
        NULL
    );
}

void assert_access(Node *node) {
    Node *type = type_of(node->left);
    bool is_struct = type && type->token->type == ID && type->declared;
    if (is_struct) {
        analyze_operator(node, LBRACK);
        return;
    }
    assert_index(node->index);
    if (type == NULL || type->token->type == ARRAY)
        return;
    Token *base = node->left->token;
    add_error(
        base->s,
        base->e,
        format("'%s' is '%s', not an array", token_text(base), type_name(type)),
        NULL
    );
}

char *names_in(Node *st, Type type) {
    char *names = NULL;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != type)
            continue;
        if (child->token->is_pub)
            continue;
        char *name = type == DECLARE ? child->var->token->name : child->token->name;
        names = names ? format("%s, %s", names, name) : name;
    }
    return names;
}

void assert_member(Node *node) {
    Node *type = type_of(node->left);
    if (type == NULL)
        return;
    if (type->token->type == REF)
        type = type->data_type;
    Token *base = node->left->token;
    if (type->token->type != ID) {
        add_error(
            base->s,
            base->e,
            format("'%s' is '%s', not a struct", token_text(base), type_name(type)),
            NULL
        );
        return;
    }
    if (type->declared == NULL)
        type->declared = find_struct(type->token->name);
    if (type->declared == NULL)
        return;
    Node *st = type->declared;
    if (node->right->token->type == FN_CALL) {
        Node *call = node->right;
        for (int i = 0; i < call->children_count; i++)
            analyze(call->children[i]);
        call->to_call = find_method(
            st,
            call->token->name,
            call->children,
            call->children_count,
            false
        );
        if (call->to_call == NULL) {
            char *method = call->token->name;
            Node *named = find_method_named(st, method);
            char *message = format("'%s' has no method '%s'", st->token->name, method);
            char *names = names_in(st, FN_DEC);
            char *help = names ? format("%s has: %s", st->token->name, names) : NULL;
            if (named) {
                char *given = "";
                for (int i = 0; i < call->children_count; i++) {
                    Node *type = type_of(call->children[i]);
                    char *text = type ? type_name(type) : "?";
                    given = format("%s%s%s", given, i ? ", " : "", text);
                }
                char *takes = "";
                for (int i = 1; i < named->args->children_count; i++) {
                    Node *type = named->args->children[i]->data_type;
                    char *text = type ? type_name(type) : "?";
                    takes = format("%s%s%s", takes, i > 1 ? ", " : "", text);
                }
                message = format("%s that takes (%s)", message, given);
                help = format("'%s' takes (%s)", call->token->name, takes);
            }
            add_error(
                call->token->s,
                call->token->e,
                message,
                help
            );
            return;
        }
        assert_args(call->to_call, call->children, call->children_count, 1);
        return;
    }
    Token *attr = node->right->token;
    node->right->declared = find_attr(st, attr->name, &attr->attr.index);
    char *names = names_in(st, DECLARE);
    if (node->right->declared == NULL)
        add_error(
            attr->s,
            attr->e,
            format("'%s' has no attribute '%s'", st->token->name, attr->name),
            names ? format("%s has: %s", st->token->name, names) : NULL
        );
}

void assert_arg_count(Node *node) {
    Node *fn = node->to_call;
    Node *params = fn->args;
    int count = params->children_count;
    bool is_variadic = count && params->children[count - 1]->token->type == VARIADIC;
    int needed = is_variadic ? count - 1 : count;
    int given = node->children_count;
    bool is_wrong = is_variadic ? given < needed : given != needed;
    if (!is_wrong)
        return;
    add_error(
        node->token->s,
        node->token->e,
        format(
            "'%s' takes %s%d argument%s, not %d",
            fn->token->name,
            is_variadic ? "at least " : "",
            needed,
            needed == 1 ? "" : "s",
            given
        ),
        signature_of(fn)
    );
}

void assert_call(Node *node) {
    Token *token = node->token;
    char *name = token->name;
    node->to_call = find_function(name);
    if (node->to_call == NULL) {
        Node *like = find_function_like(name, 2);
        Node *st = find_scope(STRUCT);
        char *msg = format("no function named '%s'", name);
        char *help = like ? format("did you mean '%s'?", like->token->name) : NULL;
        if (find_variable(name, token->s)) {
            msg = format("'%s' is a variable, not a function", name);
            help = NULL;
        } else if (st && find_method_named(st, name))
            help = format("did you mean 'self.%s()'?", name);
        add_error(
            token->s,
            token->e,
            msg,
            help
        );
        return;
    }
    for (int i = 0; i < node->children_count; i++)
        analyze(node->children[i]);
    Node *overload = find_overload(name, node->children, node->children_count);
    if (overload)
        node->to_call = overload;
    Node *params = node->to_call->args;
    if (params && includes(node->to_call->token->type, FN_DEC, PROTO, 0))
        assert_arg_count(node);
    for (int i = 0; i < node->children_count; i++) {
        if (params == NULL)
            continue;
        if (i >= params->children_count)
            continue;
        Node *param = params->children[i];
        if (param->token->type == VARIADIC)
            continue;
        Node *arg = node->children[i];
        bool takes_ref = param->data_type && param->data_type->token->type == REF;
        bool is_member = includes(arg->token->type, DOT, DOUBLE_DOTS, 0);
        Node *call = is_member ? arg->right : arg;
        bool is_ref_call = call->token->type == FN_CALL && lives_in_variable(arg);
        if (takes_ref && is_ref_call) {
            Node *ref = new_node(new_token(REF, *arg->token));
            ref->operand = arg;
            node->children[i] = arg = ref;
        }
        bool is_variable = arg->token->type != REF && lives_in_variable(arg);
        char *help = signature_of(node->to_call);
        if (takes_ref && is_variable)
            help = format("pass its address: '&%s'", token_text(arg->token));
        node->children[i] = arg = assert_copy(arg, param->data_type);
        assert_type(arg, param->data_type, help);
    }
}

void assert_builtin(Node *node) {
    Token *token = node->token;
    bool takes_one = includes(token->type, SIZEOF, TYPEOF, DROP, IS_ZERO, 0);
    if (takes_one && node->children_count != 1)
        add_error(
            token->s,
            token->e,
            format(
                "'%s' takes one argument, got %d",
                token->name,
                node->children_count
            ),
            format("use %s(x)", token->name)
        );
    for (int i = 0; i < node->children_count; i++) {
        Node *arg = node->children[i];
        Node *st = arg->token->type == ID ? find_struct(arg->token->name) : NULL;
        if (token->type == SIZEOF && st) {
            arg->token->is_type = true;
            arg->declared = st;
            continue;
        }
        if (arg->token->is_type)
            continue;
        analyze(arg);
        bool is_text = token->type == FORMAT;
        if (is_text && arg->token->type == CHARS)
            assert_type(arg, chars_type(), NULL);
        if (is_text && arg->token->type == LBRACK)
            add_error(
                arg->token->s,
                arg->token->e,
                "a $ string can't format an array literal yet",
                "format its elements one by one"
            );
        Node *type = type_of(arg);
        if (is_text && is_struct_value(type)) {
			// TODO: FORMAT should have children instead of left
            Node *op = new_node(new_token(FORMAT, *arg->token));
            op->left = arg;
            analyze_operator(op, FORMAT);
            node->children[i] = op;
        }
    }
    bool has_one_arg = node->children_count == 1;
    if (token->type == DROP && has_one_arg && !lives_in_variable(node->children[0]))
        add_error(
            node->children[0]->token->s,
            node->children[0]->token->e,
            "drop takes a variable, an attribute or an element",
            NULL
        );
}

// TODO: this code is full of shit
bool assert_assign_operation(Node *var) {
    bool is_undeclared = var->token->type == ID;

    if (!is_undeclared && !lives_in_variable(var)) {
        Token *left = var->token;
        add_error(left->s, left->e, "can't assign to a value", NULL);
        return false;
    }

    return true;
}

bool is_assign_operation(Node *node) {
    return includes(
        node->token->type,
        ASSIGN, ADD_ASSIGN, SUB_ASSIGN,
        MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
        BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN,
        LSHIFT_ASSIGN, RSHIFT_ASSIGN,
        0
    );
}

bool is_compare_operation(Node *node) {
    return includes(node->token->type, EQ, NQ, 0);
}

bool assert_ref_initialization(Node *node) {
    Node *left = node->left;
    Node *right = node->right;

    bool is_ref_declaration = is_ref_variable(left) && left->declared == NULL;

    if (node->token->type != ASSIGN || !is_ref_declaration || is_ref_binding(node))
        return true;

    int end = line_end(right->token->s);
    char *help = NULL;

    if (lives_in_variable(right)) {
        help = format(
            "write '%.*s&%.*s'",
            right->token->s - left->token->s,
            text_at(left->token->s),
            end - right->token->s,
            text_at(right->token->s)
        );
    }

    add_error(
        right->token->s,
        right->token->e,
        format("'%s' is a ref, initialize it with an address", left->token->name),
        help
    );

    return false;
}

bool assert_struct_operation(Node *node, Node *left_type, Node *right_type) {
    bool is_left_id = left_type && left_type->token->type == ID;
    bool is_right_ref = right_type && right_type->token->type == REF;

    Node *right_value = right_type;

    if (is_right_ref && (!left_type || left_type->token->type != REF))
        right_value = right_type->data_type;

    bool is_right_id = right_value && right_value->token->type == ID;

    if (is_left_id && !left_type->declared)
        left_type->declared = find_struct(left_type->token->name);

    if (is_right_id && !right_value->declared)
        right_value->declared = find_struct(right_value->token->name);

    bool is_left_struct = is_left_id && left_type->declared;
    bool is_right_struct = is_right_id && right_value->declared &&
        node->right->token->type != CHARS;

    if (!is_left_struct && !is_right_struct)
        return true;

    bool is_known = left_type && right_type;
    bool is_conversion = is_known && !same_type(right_type, left_type);
    bool has_assign = find_operator(ASSIGN, left_type, &node->right, 1) != NULL;

    if (node->token->type != ASSIGN || (is_conversion && has_assign)) {
        analyze_operator(node, node->token->type);
        return false;
    }

    if (is_left_struct)
        node->right = assert_copy(node->right, left_type);

    return true;
}

bool assert_math_operation(Node *node, Node *left_type, Node *right_type) {
    bool is_both_numeric = is_numeric(left_type) && is_numeric(right_type);
    bool is_known = left_type && right_type;
    bool is_different = is_known && !same_type(left_type, right_type);

    if (!is_math(node) || !is_different || is_both_numeric)
        return true;

    add_error(
        node->token->s,
        node->token->e,
        format(
            "can't apply '%s' to '%s' and '%s'",
            binary_ops[node->token->type],
            type_name(left_type),
            type_name(right_type)
        ),
        NULL
    );

    return false;
}

bool assert_bitwise_operation(Node *node, Node *left_type, Node *right_type) {
    bool is_bitwise = includes(
        node->token->type,
        BAND, BOR, BXOR,
        LSHIFT, RSHIFT,
        BAND_ASSIGN, BOR_ASSIGN, BXOR_ASSIGN,
        LSHIFT_ASSIGN, RSHIFT_ASSIGN,
        0
    );

    bool is_left_real = left_type && left_type->token->type == FLOAT;
    bool is_right_real = right_type && right_type->token->type == FLOAT;

    if (!is_bitwise || (!is_left_real && !is_right_real))
        return true;

    Node *real = is_left_real ? left_type : right_type;

    add_error(
        node->token->s,
        node->token->e,
        format(
            "can't apply '%s' to '%s'",
            binary_ops[node->token->type],
            type_name(real)
        ),
        "bitwise operators work on integers"
    );

    return false;
}

void assert_operation(Node *node) {
    Node *left = node->left;
    Node *right = node->right;
    Type op = node->token->type;

    if (is_assign_operation(node) && !assert_assign_operation(left))
        return;

    if (is_compare_operation(node) &&
        (left->token->type == ZERO || right->token->type == ZERO)) {
        assert_null_compare(node);
        return;
    }

    if (!assert_ref_initialization(node))
        return;

    Node *left_type = type_of(left);
    Node *right_type = type_of(right);

    if (!assert_struct_operation(node, left_type, right_type))
        return;

    // assert_copy() may have replaced the right operand.
    right_type = type_of(node->right);

    if (!assert_math_operation(node, left_type, right_type))
        return;

    if (!assert_bitwise_operation(node, left_type, right_type))
        return;

    bool is_logic = includes(op, AND, OR, 0);
    bool is_left_bool = !left_type || same_type(left_type, bool_type());

    bool is_address = includes(right->token->type, REF, ZERO, 0);
    Node *ref_type = left->data_type;

    bool is_both_numbers = is_number(left) && is_number(right);
    bool is_left_wider = is_both_numbers && left->token->shift > right->token->shift;

    if (is_ref_binding(node))
        assert_type(right, is_address ? ref_type : ref_type->data_type, NULL);
    else if (!is_logic && is_number(left) && !is_left_wider)
        assert_type(left, right_type, NULL);
    else if (!is_logic)
        assert_type(right, left_type, NULL);
    else if (is_left_bool)
        assert_type(right, bool_type(), NULL);
    else
        assert_type(left, bool_type(), NULL);
}

// 

