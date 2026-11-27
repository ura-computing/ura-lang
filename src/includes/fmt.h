/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        fmt.h                           */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 03:22:12    */
/*                                                             */
/*   updated: 2026/11/27 20:26:35                              */
/*                                                             */
/* *********************************************************** */

#pragma once

void format_ucg(ioFile out, Ucg *ucg) {
    if (ucg == NULL) {
        fputs("(null)", out);
        return;
    }
    switch (ucg->type) {
    case VOID: fputs("void", out); break;
    case INT: fprintf(out, "%c%d", ucg->is_uint ? 'u' : 'i', ucg->bits); break;
    case FLOAT: fprintf(out, "f%d", ucg->bits); break;
    case STRUCT: fputs(ucg->name, out); break;
    case REF: {
        fputc('&', out);
        format_ucg(out, ucg->to_ref);
        break;
    }
    case ARRAY: {
        format_ucg(out, ucg->arr.elem);
        if (ucg->arr.items_count)
            fprintf(out, "[%zu]", ucg->arr.items_count);
        else
            fputs("[]", out);
        break;
    }
    case VAL_CREATE: {
        if (ucg->data_type->type != ARRAY) {
            fprintf(out, GREEN("%s") " ", ucg->value);
            format_ucg(out, ucg->data_type);
            break;
        }
        fputs(GREEN("\""), out);
        for (int i = 0; i + 1 < ucg->arr.items_count; i++) {
            char c = atoi(ucg->arr.items[i]->value);
            if (c == '\n')
                fputs(GREEN("\\n"), out);
            else
                fprintf(out, GREEN("%c"), c);
        }
        fputs(GREEN("\""), out);
        break;
    }
    // clang-format off
    case DREF: case VAR: case PARAM: case ATTR: case ACCESS: case POINT_TO: case FN_DEC: {
        // clang-format on
        char *mark = "";
        if (ucg->type == DREF)
            mark = "$";
        if (ucg->name)
            fprintf(out, GREEN("%s%s"), mark, ucg->name);
        else
            fprintf(out, GREEN("t%lu"), ucg->inst_index);
        break;
    }
    default: fprintf(out, GREEN("t%lu"), ucg->inst_index); break;
    }
}

void format_token(ioFile out, Token *token, int pad) {
    if (token == NULL) {
        fputs("(null token)", out);
        return;
    }
    Type type = token->type;
    fprintf(out, CYAN("%-*s"), pad, to_string(type));
    bool is_number = includes(type, INT, FLOAT, NULL);
    char kind = type == FLOAT ? 'f' : token->is_uint ? 'u' : 'i';
    if (token->name)
        fprintf(out, " " GREEN("%s"), token->name);
    else if (is_number && token->is_type)
        fprintf(out, " %c%d", kind, token->bits);
    else if (is_number)
        fprintf(out, " " GREEN("%s"), token->num);
    else if (type == STRING_VALUE)
        fprintf(out, " " GREEN("\"%s\""), token->string);
}

void format_type(ioFile out, Node *type) {
    Token *token = type->token;
    if (token->type == ARRAY) {
        format_type(out, type->data_type);
        fputs("[]", out);
        return;
    }
    if (token->name) {
        fputs(token->name, out);
        return;
    }
    fprintf(out, "%c%d", token->type == FLOAT ? 'f' : token->is_uint ? 'u' : 'i', token->bits);
}

void format_node(ioFile out, Node *node, char *indent, char *branch, int parent_line) {
    Token *token = node->token;
    bool has_line = parent_line != -1 && token->line != parent_line;
    if (has_line)
        fprintf(out, "%4d │ ", token->line);
    else
        fputs("     │ ", out);
    fprintf(out, "%s%s", indent, branch);
    format_token(out, token, 0);
    if (node->type) {
        fputc(' ', out);
        format_type(out, node->type);
    }
    fputc('\n', out);

    expand(Node *, kids);
    zero_init(kids);
    if (node->left)
        append(kids, node->left);
    for (int i = 0; i < node->children_count; i++)
        append(kids, node->children[i]);
    bool is_fn = includes(token->type, FN_DEC, PROTO, NULL);
    if (node->right && !is_fn)
        append(kids, node->right);

    char *kids_indent = indent;
    if (strcmp2(branch, "├── "))
        kids_indent = format("%s│   ", indent);
    if (strcmp2(branch, "└── "))
        kids_indent = format("%s    ", indent);
    for (int i = 0; i < kids_count; i++) {
        char *kid_branch = i == kids_count - 1 ? "└── " : "├── ";
        format_node(out, kids[i], kids_indent, kid_branch, token->line);
    }
}

void vfprint2(ioFile out, const char *fmt, va_list ap) {
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            fputc(fmt[i], out);
            continue;
        }
        i++;
        if (fmt[i] == '%') {
            fputc('%', out);
            continue;
        }

        char spec[32] = "%"; // TODO: check this might crash
        size_t spec_size = 1;
        size_t spec_max = sizeof(spec) - 2;

        while (fmt[i] && strchr("-+ #0", fmt[i]) && spec_size < spec_max)
            spec[spec_size++] = fmt[i++];

        if (fmt[i] == '*') {
            i++;
            int width = va_arg(ap, int);
            spec_size += snprintf(spec + spec_size, spec_max - spec_size, "%d", width);
        } else {
            while (isdigit(fmt[i]) && spec_size < spec_max)
                spec[spec_size++] = fmt[i++];
        }

        if (fmt[i] == '.') {
            spec[spec_size++] = fmt[i++];
            if (fmt[i] == '*') {
                i++;
                int prec = va_arg(ap, int);
                spec_size += snprintf(spec + spec_size, spec_max - spec_size, "%d", prec);
            } else {
                while (isdigit(fmt[i]) && spec_size < spec_max)
                    spec[spec_size++] = fmt[i++];
            }
        }

        while (fmt[i] && strchr("hlz", fmt[i]) && spec_size < spec_max)
            spec[spec_size++] = fmt[i++];

        if (fmt[i] == '\0')
            break;

        char conv = fmt[i];
        spec[spec_size++] = conv;
        spec[spec_size] = '\0';

        bool is_ll = strstr(spec, "ll") != NULL;
        bool is_l = !is_ll && strchr(spec, 'l') != NULL;
        bool is_z = strchr(spec, 'z') != NULL;

        switch (conv) { // clang-format off
        case 't': fputs(to_string(va_arg(ap, Type)), out); break;
        case 'U': format_ucg(out, va_arg(ap, Ucg *)); break;
        case 'T': format_type(out, va_arg(ap, Node *)); break;
        case 'k': format_token(out, va_arg(ap, Token *), 9); break;
        case 'n': format_node(out, va_arg(ap, Node *), "", "", -1); break;
        case 's': {
            char *text = va_arg(ap, char *);
            fprintf(out, spec, text ? text : "(null)");
            break;
        }
        case 'c': fprintf(out, spec, va_arg(ap, int)); break;
        case 'p': fprintf(out, spec, va_arg(ap, void *)); break;
        case 'd': case 'i': {
			if (is_ll)     fprintf(out, spec, va_arg(ap, long long));
            else if (is_l) fprintf(out, spec, va_arg(ap, long));
            else if (is_z) fprintf(out, spec, va_arg(ap, ssize_t));
            else           fprintf(out, spec, va_arg(ap, int));
            break;
		}
        case 'u': case 'x': case 'X': case 'o': {	
			if (is_ll)     fprintf(out, spec, va_arg(ap, unsigned long long));
            else if (is_l) fprintf(out, spec, va_arg(ap, unsigned long));
            else if (is_z) fprintf(out, spec, va_arg(ap, size_t));
            else           fprintf(out, spec, va_arg(ap, unsigned));
            break;
		}
		case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A':  {
			fprintf(out, spec, va_arg(ap, double));
			break;
		}
        default: fputs(spec, out); break;
        } // clang-format on
    }
}

char *format(const char *fmt, ...) {
    char *text = NULL;
    size_t size = 0;
    ioFile out = open_memstream(&text, &size);
    if (out == NULL) {
        fputs("format failed\n", stderr);
        exit(1);
    }

    va_list ap;
    va_start(ap, fmt);
    vfprint2(out, fmt, ap);
    va_end(ap);

    fclose(out);
    char *copy = strdup2(text);
    free(text);
    return copy;
}

const char *to_string(Type type) {
#define NAME(t) [t] = #t
    char *types[LAST + 1] = {
        // clang-format off
        NAME(ERR),
        NAME(ID),            NAME(SCOPE),         NAME(URA),

        NAME(VAR),           NAME(DECLARE),       NAME(INIT_VAR),
        NAME(ARG),           NAME(PARAM),         NAME(GLOBAL),

        NAME(VAL_CREATE),    NAME(TEMPLATE),      NAME(TEMPLATE_TYPE),
        NAME(TYPES),

        NAME(ANY),           NAME(VOID),          NAME(INT),
        NAME(FLOAT),         NAME(STRING_VALUE),  NAME(REF),
        NAME(POINT_TO),      NAME(ZERO),

        NAME(STRUCT),        NAME(STRUCT_CALL),   NAME(STRUCT_VALUE),
        NAME(DOT),           NAME(ATTR),          NAME(ENUM),
        NAME(DREF),          NAME(OWN),

        NAME(LPARENT),       NAME(RPARENT),       NAME(LBRACK),
        NAME(RBRACK),        NAME(LCBRACK),       NAME(RCBRACK),
        NAME(DOTS),          NAME(DOUBLE_DOTS),   NAME(COMA),
        NAME(OPERATOR),      NAME(PUB),

        NAME(ARRAY),         NAME(ARRAY_VALUE),   NAME(ACCESS),

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
        NAME(ARGS),          NAME(PARAMS),        NAME(VARIADIC),
        NAME(FN_CALL),
        NAME(RETURN),        NAME(FORMAT),
        NAME(LINK),          NAME(USE),
        NAME(MACRO_IF),      NAME(MACRO_ELIF),    NAME(MACRO_ELSE),
        NAME(MACRO_ERROR),   NAME(MACRO_NAME),    NAME(MACRO_VERSION),
        NAME(MACRO_ENTRY),   NAME(MACRO_SCRIPT),  NAME(MACRO_DEPENDENCY),
        NAME(MACRO_URL),     NAME(MACRO_PATH),    NAME(MACRO_CMD),
        NAME(SIZEOF),        NAME(TYPEOF),        NAME(DROP),
        NAME(COPY),          NAME(HEAP_ALLOC),    NAME(MEMCPY),
        NAME(FREE),          NAME(IS_ZERO),       NAME(STACK_ALLOC),

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

    if ((unsigned)type > LAST || types[type] == NULL)
        return format("UNKNOWN(%d)", type);
    return types[type];
}