import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { LiteBenchmarkLoader } from "../src/LiteBenchmarkLoader.js";
import { APPS, SuiteSelector } from "../src/BenchmarkSuites.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";

let suite = APPS;
let app = APPS.apps;
LiteBenchmarkLoader.load(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}