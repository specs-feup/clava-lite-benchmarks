import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import path from "path";
import { APPS, ROSETTA } from "../src/BenchmarkSuites.js";
import { loadApp } from "../src/LiteBenchmarkLoader.js";

function expectAppToLoad(
    suite: typeof APPS | typeof ROSETTA,
    appName: string,
    benchmarkPath: string
): void {
    const app = suite.apps[appName];
    const result = loadApp(suite, app, path.resolve(benchmarkPath));

    expect(result.success).toBe(true);
    expect(Query.search(FunctionJp, { name: app.topFunction }).get().length).toBeGreaterThan(0);
}

describe("LiteBenchmarkLoader", () => {
    it("loads the edgedetect application", () => {
        expectAppToLoad(APPS, "edgedetect", "apps/edgedetect");
    });

    it("loads the Rosetta spam filter", () => {
        expectAppToLoad(ROSETTA, "spam-filter", "benchmarks/Rosetta/spam-filter");
    });
});
