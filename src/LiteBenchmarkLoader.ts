import Clava from "@specs-feup/clava/api/clava/Clava.js";
import ClavaJoinPoints from "@specs-feup/clava/api/clava/ClavaJoinPoints.js";
import Io from "@specs-feup/lara/api/lara/Io.js";
import chalk from "chalk";
import { readdirSync } from "fs";
import path from "path";
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
}

export type LoadResult = {
    success: boolean,
    app: string,
    topFunction: string
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
            topFunction: "<none>"
        };
    }

    const sources = readSourcesInFolder(fullPath);
    log(`Found ${sources.length} files for ${appSummary.canonicalName}`);

    Clava.pushAst(ClavaJoinPoints.program());
    for (const source of sources) {
        Clava.addExistingFile(source);
    }
    Clava.rebuild();

    return {
        success: true,
        app: appSummary.canonicalName,
        topFunction: appSummary.topFunction
    };
}

function readSourcesInFolder(folderPath: string): string[] {
    const sources: string[] = [];

    try {
        const files = readdirSync(folderPath);
        for (const file of files) {
            if (typeof file === "string") {
                if ([".c", ".cpp", ".h", ".hpp"].some(char => file.endsWith(char))) {
                    sources.push(`${folderPath}/${file}`);
                }
            }
        }
    } catch (err) {
        log(`Error reading files: ${err}`);
    }
    return sources;
}

function log(message: string): void {
    console.log(`[${chalk.yellowBright("BenchmarkLoader")}] ---------------------- ${message}`);
}