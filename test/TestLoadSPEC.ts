import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { SPEC2017 } from "../src/BenchmarkSuites.js";
import { loadApp } from "../src/LiteBenchmarkLoader.js";

let suite = SPEC2017;
let app = SPEC2017.apps["519_lbm_r"];
loadApp(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}
