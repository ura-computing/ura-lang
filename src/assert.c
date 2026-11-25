/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        assert.c                        */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/24 21:39:29    */
/*                                                             */
/*   updated: 2026/11/25 00:42:14                              */
/*                                                             */
/* *********************************************************** */

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