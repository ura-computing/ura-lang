/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        fmt.h                           */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 03:22:12    */
/*                                                             */
/*   updated: 2026/11/26 11:24:33                              */
/*                                                             */
/* *********************************************************** */

#pragma once

void format_ucg(ioFile out, Ucg *ucg) {
    if (ucg == NULL) {
        fputs("(null ucg)", out);
        return;
    }
    fputs(to_string(ucg->type), out);
    if (ucg->name)
        fprintf(out, " %s", ucg->name);
    if (includes(ucg->type, INT, FLOAT, 0) && ucg->is_val)
        fprintf(out, "(%s)", ucg->value);
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

        // flags, width, precision, length: "-5.2ld"
        char spec[32] = "%";
        size_t j = 1;
        while (fmt[i] && strchr("-+ #0123456789.hlz", fmt[i]) && j < sizeof(spec) - 2)
            spec[j++] = fmt[i++];
        if (fmt[i] == '\0')
            break;
        char conv = fmt[i];
        spec[j++] = conv;
        spec[j] = '\0';

        bool is_ll = strstr(spec, "ll") != NULL;
        bool is_l = !is_ll && strchr(spec, 'l') != NULL;
        bool is_z = strchr(spec, 'z') != NULL;

        switch (conv) { // clang-format off
        case 't': fputs(to_string(va_arg(ap, Type)), out); break;
        case 'U': format_ucg(out, va_arg(ap, Ucg *)); break;

        case 's': {
            char *str = va_arg(ap, char *);
            fprintf(out, spec, str ? str : "(null)");
            break;
        }
        case 'c': fprintf(out, spec, va_arg(ap, int)); break;
        case 'f': fprintf(out, spec, va_arg(ap, double)); break;
        case 'p': fprintf(out, spec, va_arg(ap, void *)); break;

        case 'd': case 'i':
            if (is_ll)     fprintf(out, spec, va_arg(ap, long long));
            else if (is_l) fprintf(out, spec, va_arg(ap, long));
            else if (is_z) fprintf(out, spec, va_arg(ap, ssize_t));
            else           fprintf(out, spec, va_arg(ap, int));
            break;
        case 'u': case 'x': case 'X': case 'o':
            if (is_ll)     fprintf(out, spec, va_arg(ap, unsigned long long));
            else if (is_l) fprintf(out, spec, va_arg(ap, unsigned long));
            else if (is_z) fprintf(out, spec, va_arg(ap, size_t));
            else           fprintf(out, spec, va_arg(ap, unsigned));
            break;

        default: fputs(spec, out); break;
        } // clang-format on
    }
}

// void fprint2(ioFile out, const char *fmt, ...) {
//     va_list ap;
//     va_start(ap, fmt);
//     vfprint2(out, fmt, ap);
//     va_end(ap);
// }

char *format(const char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    ioFile out = open_memstream(&buf, &size);
    if (out == NULL) {
        fputs("format failed\n", stderr);
        exit(1);
    }

    va_list ap;
    va_start(ap, fmt);
    vfprint2(out, fmt, ap);
    va_end(ap);

    fclose(out);
    char *res = strdup2(buf);
    free(buf);
    return res;
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