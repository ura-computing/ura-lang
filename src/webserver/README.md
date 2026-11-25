# webserver

A [ura](https://github.com/ura-computing/ura-lang) project.

```
main.ura        says hello to the user
src/user.ura    finds the user name on every OS, used with: use "./src/user"
```

| command | what it does |
|---|---|
| `ura deps install` | clone the dependencies (run it first) |
| `ura run` | build `main.ura` and run it |
| `ura build` | build it into `build/webserver` |
| `ura deps add <name>[@version] [repo url]` | add a dependency |
| `ura deps update [name]` | move to the latest versions |
| `ura deps remove <name>[@version]` | remove a dependency |
| `ura deps list` | show the dependencies |
