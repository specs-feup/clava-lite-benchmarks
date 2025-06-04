import { Amalgamator } from "@specs-feup/clava-code-transforms/Amalgamator";
import { Outliner } from "@specs-feup/clava-code-transforms/Outliner";
import Clava from "@specs-feup/clava/api/clava/Clava.js";
import ClavaJoinPoints from "@specs-feup/clava/api/clava/ClavaJoinPoints.js";
import { FunctionJp, WrapperStmt } from "@specs-feup/clava/api/Joinpoints.js";
import Io from "@specs-feup/lara/api/lara/Io.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import chalk from "chalk";
import { copyFileSync, existsSync, lstatSync, mkdirSync, readdirSync } from "fs";
import path, { basename, join } from "path";
import { fileURLToPath } from "url";

export type BenchmarkSuite = {
    name: string,
    path: string,
    apps: { [key: string]: AppSummary },
    flags: string[],
};

export type AppSummary = {
    canonicalName: string,
    standard: string,
    topFunction: string,
    altTopFunction?: string
    inputs?: string,
    amalgamate?: boolean
}

export type LoadResult = {
    success: boolean,
    app: string,
    topFunction: string,
    altTopFunction?: string
    direntsToCopy: string[]
}

export function appList(suite: BenchmarkSuite): string[] {
    return Object.keys(suite.apps);
}

export function* loadSuite(suite: BenchmarkSuite): Generator<LoadResult> {
    for (const app of appList(suite)) {
        log(`Loading app: ${app}`);

        const res = loadApp(suite, suite.apps[app]);
        yield res;
    }
}

export function loadApp(suite: BenchmarkSuite, appSummary: AppSummary, cachedPath?: string): LoadResult {
    const fullPath = cachedPath != undefined ? cachedPath : (() => {
        const __filename = fileURLToPath(import.meta.url);
        const __dirname = path.dirname(__filename);

        return path.join(__dirname, "../../", `${suite.path}${appSummary.canonicalName}`);
    })();

    Clava.getData().setStandard(appSummary.standard);
    log(`Selected standard: ${Clava.getStandard()}`);

    Clava.getData().setFlags(suite.flags.join(" "));
    if (suite.flags.length > 0) {
        log(`Selected flags: ${suite.flags.join(" ")}`);
    }
    else {
        log(`No Clang flags were selected`);
    }

    if (!Io.isFolder(fullPath)) {
        log(`Benchmark folder not found: ${fullPath}`);
        if (cachedPath === undefined) {
            log("Cached path is invalid.");
        }
        return {
            success: false,
            app: appSummary.canonicalName,
            topFunction: "<none>",
            direntsToCopy: [],
        };
    }

    const [sources, nonSources, subdirectories] = readSourcesInFolder(fullPath);
    log(`Found ${sources.length} files for ${appSummary.canonicalName}`);

    if (nonSources.length > 0) {
        log(`Found ${nonSources.length} non-source files for ${appSummary.canonicalName}`);
    }

    if (subdirectories.length > 0) {
        log(`Found ${subdirectories.length} subdirectories for ${appSummary.canonicalName}`);
    }

    let maxAttempts = 5;
    let keepTrying = true;
    do {
        Clava.pushAst(ClavaJoinPoints.program());
        for (const source of sources) {
            Clava.addExistingFile(source);
        }
        keepTrying = Clava.rebuild();

        if (!keepTrying) {
            log(`Error parsing AST for ${appSummary.canonicalName}, trying again...`);
            maxAttempts--;
            Clava.popAst();
        } else {
            log(`AST for ${appSummary.canonicalName} parsed successfully`);
        }
    } while (!keepTrying && maxAttempts > 0);

    transformApp(appSummary);

    const res: LoadResult = {
        success: keepTrying,
        app: appSummary.canonicalName,
        topFunction: appSummary.topFunction,
        direntsToCopy: [...nonSources, ...subdirectories]
    };
    if (appSummary.altTopFunction) {
        res.altTopFunction = appSummary.altTopFunction;
    }
    return res;
}

function copyRecursive(src: string, dest: string): void {
    const stat = lstatSync(src);

    if (stat.isDirectory()) {
        if (!existsSync(dest)) {
            mkdirSync(dest, { recursive: true });
        }

        const entries = readdirSync(src);
        for (const entry of entries) {
            const srcPath = join(src, entry);
            const destPath = join(dest, entry);
            copyRecursive(srcPath, destPath);
        }
    } else if (stat.isFile()) {
        copyFileSync(src, dest);
    } else {
        console.warn(`Skipping unsupported file type: ${src}`);
    }
}

export function copyDirents(dirents: string[], targetPath: string): void {
    if (!existsSync(targetPath)) {
        mkdirSync(targetPath, { recursive: true });
    }

    for (const dirent of dirents) {
        const destPath = join(targetPath, basename(dirent));
        copyRecursive(dirent, destPath);
    }
}

function transformApp(appSummary: AppSummary): boolean {
    try {
        if (appSummary.amalgamate) {
            const amalgamator = new Amalgamator();
            const [amalgFile, includes] = amalgamator.amalgamate(appSummary.canonicalName);
            amalgamator.replaceAstWithAmalgamation(amalgFile, includes);
            log(`Amalgamated files for ${appSummary.canonicalName}`);
        }
    } catch (error) {
        log(`Error amalgamating files for ${appSummary.canonicalName}: ${error}`);
        return false;
    }

    const topFunctionName = appSummary.topFunction;
    ensureTopFunctionExists(topFunctionName);

    if (appSummary.altTopFunction) {
        ensureTopFunctionExists(appSummary.altTopFunction);
    }

    log("Rebuilding AST after transformations...");
    try {
        //Clava.rebuild();
        log(`AST for ${appSummary.canonicalName} transformed successfully`);
        return true;
    } catch (error) {
        log(`Error transforming AST for ${appSummary.canonicalName}`);
        return false;
    }
}

function ensureTopFunctionExists(name: string): void {
    const exists = Query.search(FunctionJp, { name: name }).get().length > 0;

    if (!exists) {
        log(`Top function ${name} not found, checking for outlining directives...`);

        const pragmaBegin = `#pragma clava begin_outline ${name}`;
        const pragmaEnd = `#pragma clava end_outline ${name}`;

        let begin: WrapperStmt | undefined;
        let end: WrapperStmt | undefined;

        for (const stmt of Query.search(WrapperStmt)) {
            if (stmt.code.trim() === pragmaBegin) {
                log(`Found outlining begin directive for ${name}`);
                begin = stmt;
            }
            if (stmt.code.trim() === pragmaEnd) {
                log(`Found outlining end directive for ${name}`);
                end = stmt;
            }
        }

        if (begin && end) {
            const outliner = new Outliner();

            outliner.setDefaultPrefix("");
            outliner.outlineWithName(begin, end, name);

            begin.detach();
            end.detach();
        } else {
            log(`No outlining directives found for ${name}, cannot create function.`);
        }

    } else {
        log(`Top function ${name} exists in the AST.`);
    }
}

function readSourcesInFolder(folderPath: string): [string[], string[], string[]] {
    const sources: string[] = [];
    const nonSources: string[] = [];
    const subdirectories: string[] = [];

    try {
        const files = readdirSync(folderPath, { withFileTypes: true });

        for (const file of files) {
            if (file.isDirectory()) {
                subdirectories.push(`${folderPath}/${file.name}`);
            }
            else if (file.isFile()) {
                if ([".c", ".cpp", ".h", ".hpp"].some(ext => file.name.endsWith(ext))) {
                    sources.push(`${folderPath}/${file.name}`);
                }
                else {
                    nonSources.push(`${folderPath}/${file.name}`);
                }
            }
        }
    } catch (err) {
        log(`Error reading files: ${err}`);
    }
    return [sources, nonSources, subdirectories];
}

function log(message: string): void {
    console.log(`[${chalk.yellowBright("BenchmarkLoader")}] ${message}`);
}