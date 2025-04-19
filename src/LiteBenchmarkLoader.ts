import Clava from "@specs-feup/clava/api/clava/Clava.js";
import ClavaJoinPoints from "@specs-feup/clava/api/clava/ClavaJoinPoints.js";
import Io from "@specs-feup/lara/api/lara/Io.js";
import chalk from "chalk";
import { readdirSync } from "fs";
import path from "path";
import { fileURLToPath } from "url";

export class LiteBenchmarkLoader {
    static load(suite: BenchmarkSuite, app: string, cachedPath?: string): string {
        const summary = suite.appDetails[app];

        const fullPath = cachedPath != undefined ? cachedPath : (() => {
            const __filename = fileURLToPath(import.meta.url);
            const __dirname = path.dirname(__filename);

            return path.join(__dirname, "../../", `${suite.path}${app}`);
        })();

        Clava.getData().setStandard(summary.standard);
        this.log(`Selected standard: ${Clava.getStandard()}`);

        Clava.getData().setFlags(suite.flags.join(" "));
        if (suite.flags.length > 0) {
            this.log(`Selected flags: ${suite.flags.join(" ")}`);
        }
        else {
            this.log(`No Clang flags were selected`);
        }

        if (!Io.isFolder(fullPath)) {
            LiteBenchmarkLoader.log(`Benchmark folder not found: ${fullPath}`);
            if (cachedPath === undefined) {
                LiteBenchmarkLoader.log(`Have you cloned the submodule github.com/specs-feup/clava-benchmarks?`);
            }
            else {
                LiteBenchmarkLoader.log("Cached path is invalid.");
            }
            return "<none>";
        }

        const sources = LiteBenchmarkLoader.readSourcesInFolder(fullPath);
        LiteBenchmarkLoader.log(`Found ${sources.length} files for ${app}`);

        Clava.pushAst(ClavaJoinPoints.program());
        for (const source of sources) {
            Clava.addExistingFile(source);
        }
        Clava.rebuild();

        return summary.topFunction;
    }

    public static readSourcesInFolder(folderPath: string): string[] {
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
            LiteBenchmarkLoader.log(`Error reading files: ${err}`);
        }
        return sources;
    }

    private static log(message: string): void {
        console.log(`[${chalk.yellowBright("BenchmarkLoader")}] ---------------------- ${message}`);
    }
}

export type BenchmarkSuite = {
    name: string,
    path: string,
    apps: string[],
    appDetails: Record<string, AppSummary>,
    flags: string[]
};

export type AppSummary = {
    standard: string,
    topFunction: string,
    input?: string,
    alternateTopFunction?: string
}