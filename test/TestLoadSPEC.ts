import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { LiteBenchmarkLoader } from "../src/LiteBenchmarkLoader.js";
import { SuiteSelector } from "../src/BenchmarkSuites.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";

let suite = SuiteSelector.SPEC2017;
let app = "519_lbm_r";
LiteBenchmarkLoader.load(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}
