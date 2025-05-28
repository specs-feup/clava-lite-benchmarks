import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { loadApp } from "../src/LiteBenchmarkLoader.js";
import { APPS } from "../src/BenchmarkSuites.js";

let suite = APPS;
let app = APPS.apps["edgedetect"];
loadApp(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}