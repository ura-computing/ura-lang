// A project is a folder with a ura.conf file:
//
//     @name "raytracer"
//     @main "src/main.ura"
//     @script "run":
//         @cmd "ura compile src/main.ura -o build/raytracer && ./build/raytracer"
//     @dep "core":
//         @url "https://github.com/ura-computing/ura-core"
//         @version "0.0.1"
//     @dep "geo":
//         @path "../geometry"
//
// Same rules as the compiler's src/project.c. Pure: file access is injected.

import * as path from "path";

export const CONFIG = "ura.conf";

export interface Dependency {
    name: string;
    version?: string; // missing until `ura deps install` writes the latest one
    where: string; // the @url repo, or the @path folder
    isLocal: boolean; // @path
}

export interface Project {
    dir: string;
    name?: string;
    version?: string;
    main: string; // relative to dir, "main.ura" when not given
    scripts: Map<string, string>;
    dependencies: Dependency[];
}

// Top level `@macro "value"` lines, and the indented fields of `@dep "name":`
// and `@script "name":` blocks; lines inside `@if` blocks count too.
export function parseConfig(dir: string, text: string): Project {
    const project: Project = { dir, main: "main.ura", scripts: new Map(), dependencies: [] };
    let dependency: Dependency | undefined;
    let script: string | undefined;
    for (const line of text.split(/\r?\n/)) {
        const macro = /^\s*@(\w+)/.exec(line)?.[1];
        const value = /"([^"]*)"/.exec(line)?.[1];
        const isField = /^\s/.test(line);
        if (!isField) {
            dependency = undefined;
            script = undefined;
        }
        if (macro === undefined || value === undefined) {
            continue;
        }
        if (isField && dependency && macro === "url") {
            dependency.where = value;
        } else if (isField && dependency && macro === "path") {
            dependency.where = value;
            dependency.isLocal = true;
        } else if (isField && dependency && macro === "version") {
            dependency.version = value;
        } else if (isField && script !== undefined && macro === "cmd") {
            project.scripts.set(script, value);
        } else if (macro === "dep") {
            dependency = { name: value, where: "", isLocal: false };
            project.dependencies.push(dependency);
        } else if (macro === "script") {
            script = value;
        } else if (macro === "name") {
            project.name = value;
        } else if (macro === "version") {
            project.version = value;
        } else if (macro === "main") {
            project.main = value;
        }
    }
    return project;
}

// The folder holding ura.conf at or above `file`'s folder, or undefined.
export function findProject(file: string, exists: (file: string) => boolean): string | undefined {
    for (let dir = path.dirname(file); ; dir = path.dirname(dir)) {
        if (exists(path.join(dir, CONFIG))) {
            return dir;
        }
        if (path.dirname(dir) === dir) {
            return undefined;
        }
    }
}

// Where a dependency is on disk: a @path folder is relative to the project; a
// repo is linked into the project as deps/<name>@<version>.
export function dependencyDir(project: string, dependency: Dependency): string {
    const { name, version, where, isLocal } = dependency;
    if (isLocal) {
        return path.resolve(project, where);
    }
    return path.join(project, "deps", `${name}@${version ?? "latest"}`);
}

// The folders where `use "@/name"` looks for name.ura: the project's own
// folder, then each dependency.
export function packageDirs(project: Project): string[] {
    return [project.dir, ...project.dependencies.map((d) => dependencyDir(project.dir, d))];
}

// The ura.conf for a copy of the project in another folder: the real text,
// line for line (so errors keep their positions). An installed @url
// dependency becomes `@path "<its real folder>"` and its @version line goes
// blank, so the copy compiles against the same folders.
export function stagedConfig(project: Project, text: string): string {
    let dependency: Dependency | undefined;
    const lines = text.split(/\r?\n/).map((line) => {
        const macro = /^\s*@(\w+)/.exec(line)?.[1];
        const value = /"([^"]*)"/.exec(line)?.[1];
        if (!/^\s/.test(line)) {
            dependency = macro === "dep" ? project.dependencies.find((d) => d.name === value) : undefined;
        }
        const isInstalled = dependency !== undefined && !dependency.isLocal && dependency.version !== undefined;
        if (isInstalled && macro === "url") {
            return `    @path "${dependencyDir(project.dir, dependency!)}"`;
        }
        if (isInstalled && macro === "version") {
            return "";
        }
        if (dependency?.isLocal && macro === "path") {
            return `    @path "${dependencyDir(project.dir, dependency)}"`;
        }
        return line;
    });
    return lines.join("\n");
}
