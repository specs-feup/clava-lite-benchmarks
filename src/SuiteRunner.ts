/* eslint-disable @typescript-eslint/no-explicit-any */
import Clava from "@specs-feup/clava/api/clava/Clava.js";
import chalk from "chalk";
import { BenchmarkSuite, loadApp, LoadResult } from "./LiteBenchmarkLoader.js";

export abstract class SuiteRunner {
    private lineLength;

    constructor(lineLength: number = 60) {
        this.lineLength = lineLength;
    }

    public runScriptForSuite(suite: BenchmarkSuite, apps: string[], config: Record<string, any>, disableCaching: boolean = true): boolean {
        for (const app of apps) {
            this.log(`Running ${this.getScriptName()} for app ${app} of benchmark suite ${suite.name}`);
            const cachedPath = `${config.outputDir}/${app}/src/trans`;

            let invalidCache = false;
            let res: LoadResult;

            if (disableCaching) {
                this.log(`Caching is disabled, loading original version of ${app}...`);
                res = loadApp(suite, app);

                if (!res.success) {
                    this.log(`Could not load app ${app}, skipping...`);
                    continue;
                }
                invalidCache = true;
                this.log(`Loaded original version of app ${app} with top function ${res.topFunction}`);
            }
            else {
                this.log(`Trying to load cached version app ${app} from ${cachedPath}...`);
                res = loadApp(suite, app, cachedPath);

                if (!res.success) {
                    this.log(`Could not load cached app ${app}, loading original version instead`);
                    invalidCache = true;

                    this.log(`Loading original version of ${app}...`);
                    res = loadApp(suite, app);

                    if (!res.success) {
                        this.log(`Could not load app ${app}, skipping...`);
                        return false;
                    }
                    this.log(`Loaded original version of app ${app} with top function ${res.topFunction}`);
                }
                else {
                    this.log(`Loaded cached version of app ${app} with top function ${res.topFunction}`);
                }
            }

            try {
                const success = this.runScript(app, res.topFunction, !invalidCache, config);
                if (!success) {
                    this.log(`${this.getScriptName()} failed for app ${app}`);
                    this.log("-".repeat(this.lineLength));
                }
                else {
                    this.log(`Finished running ${this.getScriptName()} for app ${app} of benchmark suite ${suite.name}`);
                }

            } catch (e) {
                this.log((e instanceof Error) ? e.message : String(e));
                this.log(`${chalk.red("Error: ")} exception while running ${this.getScriptName()} for app ${app} of benchmark suite ${suite.name}`);
            }
            this.log("-".repeat(this.lineLength));
            if (apps.length > 1) {
                Clava.popAst();
            }
        }
        if (apps.length > 1) {
            this.log(`Finished running ${this.getScriptName()} for ${apps.length} apps from benchmark suite ${suite.name}`);
        }
        return true;
    }

    protected log(msg: string): void {
        const header = chalk.yellowBright("SuiteRunner");
        console.log(`[${header}] -------------------------- ${msg}`);
    }

    protected abstract getScriptName(): string;

    protected abstract runScript(app: string, topFunctionName: string, isCached: boolean, config: Record<string, any>): boolean;
}