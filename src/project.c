/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        project.c                       */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/24 21:39:29    */
/*                                                             */
/*   updated: 2026/11/24 22:09:52                              */
/*                                                             */
/* *********************************************************** */

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
