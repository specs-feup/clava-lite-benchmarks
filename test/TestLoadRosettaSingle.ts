import Query from "@specs-feup/lara/api/weaver/Query.js";
import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { loadApp } from "../src/LiteBenchmarkLoader.js";
import { ROSETTA } from "../src/BenchmarkSuites.js";

let suite = ROSETTA;
let app = "spam-filter"
loadApp(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}